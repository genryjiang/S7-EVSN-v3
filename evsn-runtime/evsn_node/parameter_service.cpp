#include "parameter_service.hpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
constexpr auto kParameterServiceName = "EVSNParameterService";
constexpr auto kParameterStateTopic = "/evsn/parameters/state";
constexpr auto kParameterEventTopic = "/evsn/parameters/event";
constexpr auto kParameterStateMaxInstances = 128;
constexpr auto kParameterEventQueueDepth = 128;
constexpr auto kParameterRequestQueueDepth = 32;

struct ParameterDraft {
    std::map<std::string, std::string> fields{};
};

std::string trim(const std::string& text) {
    auto begin = text.begin();
    while (begin != text.end() && std::isspace(static_cast<unsigned char>(*begin)) != 0) {
        ++begin;
    }
    auto end = text.end();
    while (end != begin && std::isspace(static_cast<unsigned char>(*(end - 1))) != 0) {
        --end;
    }
    return std::string{begin, end};
}

std::string strip_comment(const std::string& line) {
    auto in_string = false;
    auto escaped = false;
    for (auto i = std::size_t{0}; i < line.size(); ++i) {
        const auto c = line[i];
        if (escaped) {
            escaped = false;
            continue;
        }
        if (c == '\\' && in_string) {
            escaped = true;
            continue;
        }
        if (c == '"') {
            in_string = !in_string;
            continue;
        }
        if (c == '#' && !in_string) {
            return line.substr(0, i);
        }
    }
    return line;
}

std::string parse_string(const std::string& raw) {
    const auto text = trim(raw);
    if (text.size() < 2 || text.front() != '"' || text.back() != '"') {
        throw std::invalid_argument("expected quoted string value");
    }
    auto value = std::string{};
    auto escaped = false;
    for (auto i = std::size_t{1}; i + 1 < text.size(); ++i) {
        const auto c = text[i];
        if (escaped) {
            if (c == '"' || c == '\\') {
                value.push_back(c);
            } else if (c == 'n') {
                value.push_back('\n');
            } else {
                throw std::invalid_argument("unsupported string escape");
            }
            escaped = false;
            continue;
        }
        if (c == '\\') {
            escaped = true;
            continue;
        }
        value.push_back(c);
    }
    if (escaped) {
        throw std::invalid_argument("unterminated string escape");
    }
    return value;
}

bool parse_bool(const std::string& raw) {
    const auto text = trim(raw);
    if (text == "true") {
        return true;
    }
    if (text == "false") {
        return false;
    }
    throw std::invalid_argument("expected boolean value");
}

std::int64_t parse_int64(const std::string& raw) {
    const auto text = trim(raw);
    auto consumed = std::size_t{0};
    const auto value = std::stoll(text, &consumed, 10);
    if (consumed != text.size()) {
        throw std::invalid_argument("expected int64 value");
    }
    return value;
}

double parse_double(const std::string& raw) {
    const auto text = trim(raw);
    auto consumed = std::size_t{0};
    const auto value = std::stod(text, &consumed);
    if (consumed != text.size()) {
        throw std::invalid_argument("expected numeric value");
    }
    return value;
}

EVSNParameterType parse_type(const std::string& raw) {
    const auto text = parse_string(raw);
    if (text == "bool") {
        return EVSNParameterType::EVSN_PARAM_BOOL;
    }
    if (text == "int64") {
        return EVSNParameterType::EVSN_PARAM_INT64;
    }
    if (text == "float64") {
        return EVSNParameterType::EVSN_PARAM_FLOAT64;
    }
    if (text == "string") {
        return EVSNParameterType::EVSN_PARAM_STRING;
    }
    throw std::invalid_argument("unknown parameter type: " + text);
}

std::string type_to_string(EVSNParameterType type) {
    switch (type) {
    case EVSNParameterType::EVSN_PARAM_BOOL:
        return "bool";
    case EVSNParameterType::EVSN_PARAM_INT64:
        return "int64";
    case EVSNParameterType::EVSN_PARAM_FLOAT64:
        return "float64";
    case EVSNParameterType::EVSN_PARAM_STRING:
        return "string";
    }
    return "unknown";
}

EVSNParameterValue parse_value(EVSNParameterType type, const std::string& raw) {
    auto value = EVSNParameterValue{};
    value.value_type(type);
    switch (type) {
    case EVSNParameterType::EVSN_PARAM_BOOL:
        value.bool_value(parse_bool(raw));
        break;
    case EVSNParameterType::EVSN_PARAM_INT64:
        value.int_value(parse_int64(raw));
        break;
    case EVSNParameterType::EVSN_PARAM_FLOAT64:
        value.float_value(parse_double(raw));
        break;
    case EVSNParameterType::EVSN_PARAM_STRING:
        value.string_value(parse_string(raw));
        break;
    }
    return value;
}

std::string require_field(const ParameterDraft& draft, const std::string& name) {
    const auto it = draft.fields.find(name);
    if (it == draft.fields.end()) {
        throw std::invalid_argument("missing parameter field: " + name);
    }
    return it->second;
}

void check_bounded(const std::string& field, const std::string& value, std::size_t max_size) {
    if (value.size() > max_size) {
        throw std::invalid_argument(field + " exceeds DDS bound");
    }
}

double numeric_value(const EVSNParameterValue& value) {
    if (value.value_type() == EVSNParameterType::EVSN_PARAM_INT64) {
        return static_cast<double>(value.int_value());
    }
    if (value.value_type() == EVSNParameterType::EVSN_PARAM_FLOAT64) {
        return value.float_value();
    }
    throw std::logic_error("parameter value is not numeric");
}

std::int64_t unix_time_ms() {
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

dds::core::Duration reliable_writer_blocking_time() {
    return dds::core::Duration{0, 10000000};
}

dds::pub::qos::DataWriterQos state_writer_qos() {
    auto qos = dds::core::QosProvider::Default().datawriter_qos();
    qos << dds::core::policy::Reliability::Reliable(reliable_writer_blocking_time())
        << dds::core::policy::History::KeepLast(1)
        << dds::core::policy::Durability::TransientLocal()
        << dds::core::policy::ResourceLimits(
            kParameterStateMaxInstances,
            kParameterStateMaxInstances,
            1);
    return qos;
}

dds::pub::qos::DataWriterQos event_writer_qos() {
    auto qos = dds::core::QosProvider::Default().datawriter_qos();
    qos << dds::core::policy::Reliability::Reliable(reliable_writer_blocking_time())
        << dds::core::policy::History::KeepAll()
        << dds::core::policy::Durability::Volatile()
        << dds::core::policy::ResourceLimits(
            kParameterEventQueueDepth,
            1,
            kParameterEventQueueDepth);
    return qos;
}

dds::sub::qos::DataReaderQos request_reader_qos() {
    auto qos = dds::core::QosProvider::Default().datareader_qos();
    qos << dds::core::policy::Reliability::Reliable()
        << dds::core::policy::History::KeepLast(kParameterRequestQueueDepth)
        << dds::core::policy::Durability::Volatile()
        << dds::core::policy::ResourceLimits(
            kParameterRequestQueueDepth,
            1,
            kParameterRequestQueueDepth);
    return qos;
}

dds::pub::qos::DataWriterQos reply_writer_qos() {
    auto qos = dds::core::QosProvider::Default().datawriter_qos();
    qos << dds::core::policy::Reliability::Reliable(reliable_writer_blocking_time())
        << dds::core::policy::History::KeepLast(kParameterRequestQueueDepth)
        << dds::core::policy::Durability::Volatile()
        << dds::core::policy::ResourceLimits(
            kParameterRequestQueueDepth,
            1,
            kParameterRequestQueueDepth);
    return qos;
}
}  // namespace

EVSNParameterService::EVSNParameterService(
    dds::domain::DomainParticipant participant,
    std::string node_name,
    std::string parameter_file) :
    node_name_{std::move(node_name)},
    participant_{std::move(participant)},
    publisher_{participant_} {
    if (!parameter_file.empty()) {
        load_parameter_file(parameter_file);
    }
    state_topic_ = std::make_unique<dds::topic::Topic<EVSNParameterState>>(
        participant_,
        kParameterStateTopic);
    event_topic_ = std::make_unique<dds::topic::Topic<EVSNParameterEvent>>(
        participant_,
        kParameterEventTopic);
    state_writer_ = std::make_unique<dds::pub::DataWriter<EVSNParameterState>>(
        publisher_,
        *state_topic_,
        state_writer_qos());
    event_writer_ = std::make_unique<dds::pub::DataWriter<EVSNParameterEvent>>(
        publisher_,
        *event_topic_,
        event_writer_qos());
    auto replier_params = rti::request::ReplierParams{participant_};
    replier_params.service_name(kParameterServiceName);
    replier_params.datareader_qos(request_reader_qos());
    replier_params.datawriter_qos(reply_writer_qos());
    replier_ = std::make_unique<rti::request::Replier<EVSNParameterRequest, EVSNParameterReply>>(
        replier_params);
    publish_all_states();
}

EVSNParameterService::~EVSNParameterService() = default;

void EVSNParameterService::poll() {
    auto requests = replier_->receive_requests(dds::core::Duration::from_millisecs(0));
    for (const auto& request : requests) {
        if (request.info().valid() && request_targets_this_node(request.data())) {
            handle_request(request.data(), request.info());
        }
    }
}

std::size_t EVSNParameterService::parameter_count() const {
    return parameters_.size();
}

std::optional<EVSNParameterState> EVSNParameterService::parameter_state(const std::string& name) const {
    const auto it = parameters_.find(name);
    if (it == parameters_.end()) {
        return std::nullopt;
    }
    return make_state(it->second);
}

std::string EVSNParameterService::parameter_value_as_string(const std::string& name) const {
    const auto it = parameters_.find(name);
    if (it == parameters_.end()) {
        throw std::out_of_range("unknown parameter: " + name);
    }
    return value_to_string(it->second.value);
}

void EVSNParameterService::load_parameter_file(const std::string& parameter_file) {
    auto file = std::ifstream{parameter_file};
    if (!file) {
        throw std::runtime_error("parameter defaults file not found: " + parameter_file);
    }
    auto node_name_from_file = std::string{};
    auto drafts = std::vector<ParameterDraft>{};
    auto current = std::optional<ParameterDraft>{};
    auto line = std::string{};
    auto line_number = std::size_t{0};
    while (std::getline(file, line)) {
        ++line_number;
        const auto text = trim(strip_comment(line));
        if (text.empty()) {
            continue;
        }
        if (text == "[[parameters]]") {
            if (current.has_value()) {
                drafts.push_back(*current);
            }
            current = ParameterDraft{};
            continue;
        }
        const auto equals = text.find('=');
        if (equals == std::string::npos) {
            throw std::invalid_argument(
                parameter_file + ":" + std::to_string(line_number) + ": expected key = value");
        }
        const auto key = trim(text.substr(0, equals));
        const auto value = trim(text.substr(equals + 1));
        if (current.has_value()) {
            current->fields[key] = value;
        } else if (key == "node") {
            node_name_from_file = parse_string(value);
        } else if (key != "schema_version" && key != "fallback") {
            throw std::invalid_argument(
                parameter_file + ":" + std::to_string(line_number) + ": unexpected global key");
        }
    }
    if (current.has_value()) {
        drafts.push_back(*current);
    }
    if (!node_name_from_file.empty() && node_name_from_file != node_name_) {
        throw std::invalid_argument("parameter defaults node name does not match " + node_name_);
    }
    for (const auto& draft : drafts) {
        auto parameter = ParameterDefinition{};
        const auto name = parse_string(require_field(draft, "name"));
        check_bounded("parameter name", name, 128);
        if (parameters_.find(name) != parameters_.end()) {
            throw std::invalid_argument("duplicate parameter: " + name);
        }
        const auto type = parse_type(require_field(draft, "type"));
        const auto default_value = parse_value(type, require_field(draft, "default"));
        const auto mutable_at_runtime = parse_bool(require_field(draft, "mutable"));
        const auto unit = parse_string(require_field(draft, "unit"));
        const auto description = parse_string(require_field(draft, "description"));
        const auto fallback = parse_string(require_field(draft, "fallback"));
        check_bounded("unit", unit, 32);
        check_bounded("description", description, 256);
        check_bounded("fallback", fallback, 256);
        parameter.descriptor.node_name(node_name_);
        parameter.descriptor.parameter_name(name);
        parameter.descriptor.value_type(type);
        parameter.descriptor.default_value(default_value);
        parameter.descriptor.mutable_at_runtime(mutable_at_runtime);
        parameter.descriptor.unit(unit);
        parameter.descriptor.description(description);
        parameter.descriptor.fallback_behavior(fallback);
        const auto min_it = draft.fields.find("min");
        const auto max_it = draft.fields.find("max");
        if (min_it != draft.fields.end() || max_it != draft.fields.end()) {
            if (type != EVSNParameterType::EVSN_PARAM_INT64 && type != EVSNParameterType::EVSN_PARAM_FLOAT64) {
                throw std::invalid_argument("bounds are only valid for numeric parameter: " + name);
            }
        }
        if (min_it != draft.fields.end()) {
            parameter.descriptor.has_min(true);
            parameter.descriptor.min_value(parse_double(min_it->second));
        }
        if (max_it != draft.fields.end()) {
            parameter.descriptor.has_max(true);
            parameter.descriptor.max_value(parse_double(max_it->second));
        }
        parameter.value = default_value;
        auto reason = std::string{};
        if (!validate_candidate(parameter, parameter.value, reason)) {
            throw std::invalid_argument("invalid default for " + name + ": " + reason);
        }
        parameters_.emplace(name, std::move(parameter));
    }
}

void EVSNParameterService::publish_all_states() {
    for (const auto& [name, parameter] : parameters_) {
        (void)name;
        publish_state(parameter);
    }
}

void EVSNParameterService::publish_state(const ParameterDefinition& parameter) {
    state_writer_->write(make_state(parameter));
}

void EVSNParameterService::publish_event(
    const EVSNParameterRequest& request,
    bool accepted,
    const std::string& reason,
    const EVSNParameterValue& resulting_value) {
    auto event = EVSNParameterEvent{};
    event.node_name(node_name_);
    event.parameter_name(request.parameter_name());
    event.requester(request.requester());
    event.source(request.source());
    event.timestamp_unix_ms(unix_time_ms());
    event.accepted(accepted);
    event.reason(reason);
    event.requested_value(request.value());
    event.resulting_value(resulting_value);
    event_writer_->write(event);
}

void EVSNParameterService::handle_request(
    const EVSNParameterRequest& request,
    const dds::sub::SampleInfo& info) {
    auto reply = EVSNParameterReply{};
    switch (request.operation()) {
    case EVSNParameterOperation::EVSN_PARAM_GET:
        reply = handle_get(request);
        break;
    case EVSNParameterOperation::EVSN_PARAM_SET:
        reply = handle_set(request);
        break;
    case EVSNParameterOperation::EVSN_PARAM_LIST:
        reply = handle_list(request);
        break;
    case EVSNParameterOperation::EVSN_PARAM_DESCRIBE:
        reply = handle_describe(request);
        break;
    }
    replier_->send_reply(reply, info);
}

EVSNParameterReply EVSNParameterService::handle_get(const EVSNParameterRequest& request) const {
    auto reply = make_reply(request, node_name_, true, "ok");
    if (request.parameter_name().empty()) {
        for (const auto& [name, parameter] : parameters_) {
            (void)name;
            reply.states().push_back(make_state(parameter));
        }
        return reply;
    }
    const auto it = parameters_.find(request.parameter_name());
    if (it == parameters_.end()) {
        return make_reply(request, node_name_, false, "unknown parameter");
    }
    reply.states().push_back(make_state(it->second));
    return reply;
}

EVSNParameterReply EVSNParameterService::handle_set(const EVSNParameterRequest& request) {
    const auto it = parameters_.find(request.parameter_name());
    if (it == parameters_.end()) {
        auto empty_value = EVSNParameterValue{};
        empty_value.value_type(request.value().value_type());
        publish_event(request, false, "unknown parameter", empty_value);
        return make_reply(request, node_name_, false, "unknown parameter");
    }
    auto reason = std::string{};
    if (!it->second.descriptor.mutable_at_runtime()) {
        publish_event(request, false, "parameter is not mutable at runtime", it->second.value);
        return make_reply(request, node_name_, false, "parameter is not mutable at runtime");
    }
    if (!validate_candidate(it->second, request.value(), reason)) {
        publish_event(request, false, reason, it->second.value);
        return make_reply(request, node_name_, false, reason);
    }
    it->second.value = request.value();
    publish_state(it->second);
    publish_event(request, true, "accepted", it->second.value);
    auto reply = make_reply(request, node_name_, true, "accepted");
    reply.states().push_back(make_state(it->second));
    return reply;
}

EVSNParameterReply EVSNParameterService::handle_list(const EVSNParameterRequest& request) const {
    auto reply = make_reply(request, node_name_, true, "ok");
    for (const auto& [name, parameter] : parameters_) {
        (void)name;
        reply.states().push_back(make_state(parameter));
    }
    return reply;
}

EVSNParameterReply EVSNParameterService::handle_describe(const EVSNParameterRequest& request) const {
    auto reply = make_reply(request, node_name_, true, "ok");
    if (request.parameter_name().empty()) {
        for (const auto& [name, parameter] : parameters_) {
            (void)name;
            reply.descriptors().push_back(parameter.descriptor);
        }
        return reply;
    }
    const auto it = parameters_.find(request.parameter_name());
    if (it == parameters_.end()) {
        return make_reply(request, node_name_, false, "unknown parameter");
    }
    reply.descriptors().push_back(it->second.descriptor);
    return reply;
}

bool EVSNParameterService::request_targets_this_node(const EVSNParameterRequest& request) const {
    return request.node_name().empty() || request.node_name() == node_name_ || request.node_name() == "*";
}

bool EVSNParameterService::validate_candidate(
    const ParameterDefinition& parameter,
    const EVSNParameterValue& candidate,
    std::string& reason) const {
    const auto expected_type = parameter.descriptor.value_type();
    if (candidate.value_type() != expected_type) {
        reason = "expected " + type_to_string(expected_type);
        return false;
    }
    if (expected_type == EVSNParameterType::EVSN_PARAM_STRING && candidate.string_value().size() > 256) {
        reason = "string value exceeds DDS bound";
        return false;
    }
    if (expected_type == EVSNParameterType::EVSN_PARAM_INT64 || expected_type == EVSNParameterType::EVSN_PARAM_FLOAT64) {
        const auto value = numeric_value(candidate);
        if (parameter.descriptor.has_min() && value < parameter.descriptor.min_value()) {
            reason = "value below minimum";
            return false;
        }
        if (parameter.descriptor.has_max() && value > parameter.descriptor.max_value()) {
            reason = "value above maximum";
            return false;
        }
        if (expected_type == EVSNParameterType::EVSN_PARAM_FLOAT64 && !std::isfinite(candidate.float_value())) {
            reason = "float value must be finite";
            return false;
        }
    }
    reason = "ok";
    return true;
}

EVSNParameterState EVSNParameterService::make_state(const ParameterDefinition& parameter) const {
    auto state = EVSNParameterState{};
    state.node_name(node_name_);
    state.parameter_name(parameter.descriptor.parameter_name());
    state.value(parameter.value);
    state.mutable_at_runtime(parameter.descriptor.mutable_at_runtime());
    state.unit(parameter.descriptor.unit());
    state.description(parameter.descriptor.description());
    return state;
}

EVSNParameterReply EVSNParameterService::make_reply(
    const EVSNParameterRequest& request,
    const std::string& node_name,
    bool accepted,
    const std::string& reason) {
    auto reply = EVSNParameterReply{};
    reply.node_name(node_name);
    reply.operation(request.operation());
    reply.accepted(accepted);
    reply.reason(reason);
    return reply;
}

std::string EVSNParameterService::value_to_string(const EVSNParameterValue& value) {
    auto stream = std::ostringstream{};
    switch (value.value_type()) {
    case EVSNParameterType::EVSN_PARAM_BOOL:
        return value.bool_value() ? "true" : "false";
    case EVSNParameterType::EVSN_PARAM_INT64:
        return std::to_string(value.int_value());
    case EVSNParameterType::EVSN_PARAM_FLOAT64:
        stream << value.float_value();
        return stream.str();
    case EVSNParameterType::EVSN_PARAM_STRING:
        return value.string_value();
    }
    return {};
}
