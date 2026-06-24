#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include <dds/dds.hpp>
#include <rti/request/Replier.hpp>

#include "custom_types.hpp"

class EVSNParameterService {
public:
    EVSNParameterService(
        dds::domain::DomainParticipant participant,
        std::string node_name,
        std::string parameter_file);
    ~EVSNParameterService();

    EVSNParameterService(const EVSNParameterService&) = delete;
    EVSNParameterService& operator=(const EVSNParameterService&) = delete;
    EVSNParameterService(EVSNParameterService&&) = delete;
    EVSNParameterService& operator=(EVSNParameterService&&) = delete;

    void poll();

    std::size_t parameter_count() const;
    std::optional<EVSNParameterState> parameter_state(const std::string& name) const;
    std::string parameter_value_as_string(const std::string& name) const;

private:
    struct ParameterDefinition {
        EVSNParameterDescriptor descriptor;
        EVSNParameterValue value;
    };

    std::string node_name_;
    std::map<std::string, ParameterDefinition> parameters_{};
    dds::domain::DomainParticipant participant_;
    dds::pub::Publisher publisher_;
    std::unique_ptr<dds::topic::Topic<EVSNParameterState>> state_topic_{};
    std::unique_ptr<dds::topic::Topic<EVSNParameterEvent>> event_topic_{};
    std::unique_ptr<dds::pub::DataWriter<EVSNParameterState>> state_writer_{};
    std::unique_ptr<dds::pub::DataWriter<EVSNParameterEvent>> event_writer_{};
    std::unique_ptr<rti::request::Replier<EVSNParameterRequest, EVSNParameterReply>> replier_{};

    void load_parameter_file(const std::string& parameter_file);
    void publish_all_states();
    void publish_state(const ParameterDefinition& parameter);
    void publish_event(
        const EVSNParameterRequest& request,
        bool accepted,
        const std::string& reason,
        const EVSNParameterValue& resulting_value);
    void handle_request(const EVSNParameterRequest& request, const dds::sub::SampleInfo& info);

    EVSNParameterReply handle_get(const EVSNParameterRequest& request) const;
    EVSNParameterReply handle_set(const EVSNParameterRequest& request);
    EVSNParameterReply handle_list(const EVSNParameterRequest& request) const;
    EVSNParameterReply handle_describe(const EVSNParameterRequest& request) const;

    bool request_targets_this_node(const EVSNParameterRequest& request) const;
    bool validate_candidate(
        const ParameterDefinition& parameter,
        const EVSNParameterValue& candidate,
        std::string& reason) const;
    EVSNParameterState make_state(const ParameterDefinition& parameter) const;
    static EVSNParameterReply make_reply(
        const EVSNParameterRequest& request,
        const std::string& node_name,
        bool accepted,
        const std::string& reason);
    static std::string value_to_string(const EVSNParameterValue& value);
};
