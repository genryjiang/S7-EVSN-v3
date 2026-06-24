

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_params.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_params_288152163_hpp
#define evsn_params_288152163_hpp

#include <iosfwd>

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef RTIUSERDllExport
#define RTIUSERDllExport __declspec(dllexport)
#endif

#include "dds/core/SafeEnumeration.hpp"
#include "dds/core/String.hpp"
#include "dds/core/array.hpp"
#include "dds/core/vector.hpp"
#include "dds/core/External.hpp"
#include "rti/core/LongDouble.hpp"
#include "rti/core/Pointer.hpp"
#include "rti/core/array.hpp"
#include "rti/topic/TopicTraits.hpp"

#include "omg/types/string_view.hpp"

#include "rti/core/BoundedSequence.hpp"
#include "dds/core/Optional.hpp"

#ifndef NDDS_STANDALONE_TYPE
#include "cdr/cdr_typeCode.h"
#include "dds/domain/DomainParticipant.hpp"
#include "dds/topic/TopicTraits.hpp"
#include "dds/core/xtypes/DynamicType.hpp"
#include "dds/core/xtypes/StructType.hpp"
#include "dds/core/xtypes/UnionType.hpp"
#include "dds/core/xtypes/EnumType.hpp"
#include "dds/core/xtypes/AliasType.hpp"
#include "rti/util/StreamFlagSaver.hpp"
#include "rti/domain/PluginSupport.hpp"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef RTIUSERDllExport
#define RTIUSERDllExport
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

enum class EVSNParameterType {
    EVSN_PARAM_BOOL, 
    EVSN_PARAM_INT64, 
    EVSN_PARAM_FLOAT64, 
    EVSN_PARAM_STRING
};

NDDSUSERDllExport std::ostream& operator << (std::ostream& o,const EVSNParameterType& sample);
enum class EVSNParameterOperation {
    EVSN_PARAM_GET, 
    EVSN_PARAM_SET, 
    EVSN_PARAM_LIST, 
    EVSN_PARAM_DESCRIBE
};

NDDSUSERDllExport std::ostream& operator << (std::ostream& o,const EVSNParameterOperation& sample);

class NDDSUSERDllExport EVSNParameterValue {
  public:

    EVSNParameterValue();

    EVSNParameterValue(const ::EVSNParameterType& value_type_,bool bool_value_,int64_t int_value_,double float_value_,const std::string& string_value_);

    ::EVSNParameterType& value_type() noexcept {
        return m_value_type_;
    }

    const ::EVSNParameterType& value_type() const noexcept {
        return m_value_type_;
    }

    void value_type(const ::EVSNParameterType& value) {

        m_value_type_ = value;
    }

    void value_type(::EVSNParameterType&& value) {
        m_value_type_ = std::move(value);
    }
    bool& bool_value() noexcept {
        return m_bool_value_;
    }

    const bool& bool_value() const noexcept {
        return m_bool_value_;
    }

    void bool_value(bool value) {

        m_bool_value_ = value;
    }

    int64_t& int_value() noexcept {
        return m_int_value_;
    }

    const int64_t& int_value() const noexcept {
        return m_int_value_;
    }

    void int_value(int64_t value) {

        m_int_value_ = value;
    }

    double& float_value() noexcept {
        return m_float_value_;
    }

    const double& float_value() const noexcept {
        return m_float_value_;
    }

    void float_value(double value) {

        m_float_value_ = value;
    }

    std::string& string_value() noexcept {
        return m_string_value_;
    }

    const std::string& string_value() const noexcept {
        return m_string_value_;
    }

    void string_value(const std::string& value) {

        m_string_value_ = value;
    }

    void string_value(std::string&& value) {
        m_string_value_ = std::move(value);
    }
    bool operator == (const EVSNParameterValue& other_) const;
    bool operator != (const EVSNParameterValue& other_) const;

    void swap(EVSNParameterValue& other_) noexcept ;

  private:

    ::EVSNParameterType m_value_type_;
    bool m_bool_value_;
    int64_t m_int_value_;
    double m_float_value_;
    std::string m_string_value_;

};

inline void swap(EVSNParameterValue& a, EVSNParameterValue& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNParameterValue& sample);

class NDDSUSERDllExport EVSNParameterDescriptor {
  public:

    EVSNParameterDescriptor();

    EVSNParameterDescriptor(const std::string& node_name_,const std::string& parameter_name_,const ::EVSNParameterType& value_type_,const ::EVSNParameterValue& default_value_,bool mutable_at_runtime_,bool has_min_,double min_value_,bool has_max_,double max_value_,const std::string& unit_,const std::string& description_,const std::string& fallback_behavior_);

    std::string& node_name() noexcept {
        return m_node_name_;
    }

    const std::string& node_name() const noexcept {
        return m_node_name_;
    }

    void node_name(const std::string& value) {

        m_node_name_ = value;
    }

    void node_name(std::string&& value) {
        m_node_name_ = std::move(value);
    }
    std::string& parameter_name() noexcept {
        return m_parameter_name_;
    }

    const std::string& parameter_name() const noexcept {
        return m_parameter_name_;
    }

    void parameter_name(const std::string& value) {

        m_parameter_name_ = value;
    }

    void parameter_name(std::string&& value) {
        m_parameter_name_ = std::move(value);
    }
    ::EVSNParameterType& value_type() noexcept {
        return m_value_type_;
    }

    const ::EVSNParameterType& value_type() const noexcept {
        return m_value_type_;
    }

    void value_type(const ::EVSNParameterType& value) {

        m_value_type_ = value;
    }

    void value_type(::EVSNParameterType&& value) {
        m_value_type_ = std::move(value);
    }
    ::EVSNParameterValue& default_value() noexcept {
        return m_default_value_;
    }

    const ::EVSNParameterValue& default_value() const noexcept {
        return m_default_value_;
    }

    void default_value(const ::EVSNParameterValue& value) {

        m_default_value_ = value;
    }

    void default_value(::EVSNParameterValue&& value) {
        m_default_value_ = std::move(value);
    }
    bool& mutable_at_runtime() noexcept {
        return m_mutable_at_runtime_;
    }

    const bool& mutable_at_runtime() const noexcept {
        return m_mutable_at_runtime_;
    }

    void mutable_at_runtime(bool value) {

        m_mutable_at_runtime_ = value;
    }

    bool& has_min() noexcept {
        return m_has_min_;
    }

    const bool& has_min() const noexcept {
        return m_has_min_;
    }

    void has_min(bool value) {

        m_has_min_ = value;
    }

    double& min_value() noexcept {
        return m_min_value_;
    }

    const double& min_value() const noexcept {
        return m_min_value_;
    }

    void min_value(double value) {

        m_min_value_ = value;
    }

    bool& has_max() noexcept {
        return m_has_max_;
    }

    const bool& has_max() const noexcept {
        return m_has_max_;
    }

    void has_max(bool value) {

        m_has_max_ = value;
    }

    double& max_value() noexcept {
        return m_max_value_;
    }

    const double& max_value() const noexcept {
        return m_max_value_;
    }

    void max_value(double value) {

        m_max_value_ = value;
    }

    std::string& unit() noexcept {
        return m_unit_;
    }

    const std::string& unit() const noexcept {
        return m_unit_;
    }

    void unit(const std::string& value) {

        m_unit_ = value;
    }

    void unit(std::string&& value) {
        m_unit_ = std::move(value);
    }
    std::string& description() noexcept {
        return m_description_;
    }

    const std::string& description() const noexcept {
        return m_description_;
    }

    void description(const std::string& value) {

        m_description_ = value;
    }

    void description(std::string&& value) {
        m_description_ = std::move(value);
    }
    std::string& fallback_behavior() noexcept {
        return m_fallback_behavior_;
    }

    const std::string& fallback_behavior() const noexcept {
        return m_fallback_behavior_;
    }

    void fallback_behavior(const std::string& value) {

        m_fallback_behavior_ = value;
    }

    void fallback_behavior(std::string&& value) {
        m_fallback_behavior_ = std::move(value);
    }
    bool operator == (const EVSNParameterDescriptor& other_) const;
    bool operator != (const EVSNParameterDescriptor& other_) const;

    void swap(EVSNParameterDescriptor& other_) noexcept ;

  private:

    std::string m_node_name_;
    std::string m_parameter_name_;
    ::EVSNParameterType m_value_type_;
    ::EVSNParameterValue m_default_value_;
    bool m_mutable_at_runtime_;
    bool m_has_min_;
    double m_min_value_;
    bool m_has_max_;
    double m_max_value_;
    std::string m_unit_;
    std::string m_description_;
    std::string m_fallback_behavior_;

};

inline void swap(EVSNParameterDescriptor& a, EVSNParameterDescriptor& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNParameterDescriptor& sample);

class NDDSUSERDllExport EVSNParameterState {
  public:

    EVSNParameterState();

    EVSNParameterState(const std::string& node_name_,const std::string& parameter_name_,const ::EVSNParameterValue& value_,bool mutable_at_runtime_,const std::string& unit_,const std::string& description_);

    std::string& node_name() noexcept {
        return m_node_name_;
    }

    const std::string& node_name() const noexcept {
        return m_node_name_;
    }

    void node_name(const std::string& value) {

        m_node_name_ = value;
    }

    void node_name(std::string&& value) {
        m_node_name_ = std::move(value);
    }
    std::string& parameter_name() noexcept {
        return m_parameter_name_;
    }

    const std::string& parameter_name() const noexcept {
        return m_parameter_name_;
    }

    void parameter_name(const std::string& value) {

        m_parameter_name_ = value;
    }

    void parameter_name(std::string&& value) {
        m_parameter_name_ = std::move(value);
    }
    ::EVSNParameterValue& value() noexcept {
        return m_value_;
    }

    const ::EVSNParameterValue& value() const noexcept {
        return m_value_;
    }

    void value(const ::EVSNParameterValue& value) {

        m_value_ = value;
    }

    void value(::EVSNParameterValue&& value) {
        m_value_ = std::move(value);
    }
    bool& mutable_at_runtime() noexcept {
        return m_mutable_at_runtime_;
    }

    const bool& mutable_at_runtime() const noexcept {
        return m_mutable_at_runtime_;
    }

    void mutable_at_runtime(bool value) {

        m_mutable_at_runtime_ = value;
    }

    std::string& unit() noexcept {
        return m_unit_;
    }

    const std::string& unit() const noexcept {
        return m_unit_;
    }

    void unit(const std::string& value) {

        m_unit_ = value;
    }

    void unit(std::string&& value) {
        m_unit_ = std::move(value);
    }
    std::string& description() noexcept {
        return m_description_;
    }

    const std::string& description() const noexcept {
        return m_description_;
    }

    void description(const std::string& value) {

        m_description_ = value;
    }

    void description(std::string&& value) {
        m_description_ = std::move(value);
    }
    bool operator == (const EVSNParameterState& other_) const;
    bool operator != (const EVSNParameterState& other_) const;

    void swap(EVSNParameterState& other_) noexcept ;

  private:

    std::string m_node_name_;
    std::string m_parameter_name_;
    ::EVSNParameterValue m_value_;
    bool m_mutable_at_runtime_;
    std::string m_unit_;
    std::string m_description_;

};

inline void swap(EVSNParameterState& a, EVSNParameterState& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNParameterState& sample);

class NDDSUSERDllExport EVSNParameterEvent {
  public:

    EVSNParameterEvent();

    EVSNParameterEvent(const std::string& node_name_,const std::string& parameter_name_,const std::string& requester_,const std::string& source_,int64_t timestamp_unix_ms_,bool accepted_,const std::string& reason_,const ::EVSNParameterValue& requested_value_,const ::EVSNParameterValue& resulting_value_);

    std::string& node_name() noexcept {
        return m_node_name_;
    }

    const std::string& node_name() const noexcept {
        return m_node_name_;
    }

    void node_name(const std::string& value) {

        m_node_name_ = value;
    }

    void node_name(std::string&& value) {
        m_node_name_ = std::move(value);
    }
    std::string& parameter_name() noexcept {
        return m_parameter_name_;
    }

    const std::string& parameter_name() const noexcept {
        return m_parameter_name_;
    }

    void parameter_name(const std::string& value) {

        m_parameter_name_ = value;
    }

    void parameter_name(std::string&& value) {
        m_parameter_name_ = std::move(value);
    }
    std::string& requester() noexcept {
        return m_requester_;
    }

    const std::string& requester() const noexcept {
        return m_requester_;
    }

    void requester(const std::string& value) {

        m_requester_ = value;
    }

    void requester(std::string&& value) {
        m_requester_ = std::move(value);
    }
    std::string& source() noexcept {
        return m_source_;
    }

    const std::string& source() const noexcept {
        return m_source_;
    }

    void source(const std::string& value) {

        m_source_ = value;
    }

    void source(std::string&& value) {
        m_source_ = std::move(value);
    }
    int64_t& timestamp_unix_ms() noexcept {
        return m_timestamp_unix_ms_;
    }

    const int64_t& timestamp_unix_ms() const noexcept {
        return m_timestamp_unix_ms_;
    }

    void timestamp_unix_ms(int64_t value) {

        m_timestamp_unix_ms_ = value;
    }

    bool& accepted() noexcept {
        return m_accepted_;
    }

    const bool& accepted() const noexcept {
        return m_accepted_;
    }

    void accepted(bool value) {

        m_accepted_ = value;
    }

    std::string& reason() noexcept {
        return m_reason_;
    }

    const std::string& reason() const noexcept {
        return m_reason_;
    }

    void reason(const std::string& value) {

        m_reason_ = value;
    }

    void reason(std::string&& value) {
        m_reason_ = std::move(value);
    }
    ::EVSNParameterValue& requested_value() noexcept {
        return m_requested_value_;
    }

    const ::EVSNParameterValue& requested_value() const noexcept {
        return m_requested_value_;
    }

    void requested_value(const ::EVSNParameterValue& value) {

        m_requested_value_ = value;
    }

    void requested_value(::EVSNParameterValue&& value) {
        m_requested_value_ = std::move(value);
    }
    ::EVSNParameterValue& resulting_value() noexcept {
        return m_resulting_value_;
    }

    const ::EVSNParameterValue& resulting_value() const noexcept {
        return m_resulting_value_;
    }

    void resulting_value(const ::EVSNParameterValue& value) {

        m_resulting_value_ = value;
    }

    void resulting_value(::EVSNParameterValue&& value) {
        m_resulting_value_ = std::move(value);
    }
    bool operator == (const EVSNParameterEvent& other_) const;
    bool operator != (const EVSNParameterEvent& other_) const;

    void swap(EVSNParameterEvent& other_) noexcept ;

  private:

    std::string m_node_name_;
    std::string m_parameter_name_;
    std::string m_requester_;
    std::string m_source_;
    int64_t m_timestamp_unix_ms_;
    bool m_accepted_;
    std::string m_reason_;
    ::EVSNParameterValue m_requested_value_;
    ::EVSNParameterValue m_resulting_value_;

};

inline void swap(EVSNParameterEvent& a, EVSNParameterEvent& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNParameterEvent& sample);

class NDDSUSERDllExport EVSNParameterRequest {
  public:

    EVSNParameterRequest();

    EVSNParameterRequest(const std::string& node_name_,const std::string& parameter_name_,const std::string& requester_,const std::string& source_,const ::EVSNParameterOperation& operation_,const ::EVSNParameterValue& value_);

    std::string& node_name() noexcept {
        return m_node_name_;
    }

    const std::string& node_name() const noexcept {
        return m_node_name_;
    }

    void node_name(const std::string& value) {

        m_node_name_ = value;
    }

    void node_name(std::string&& value) {
        m_node_name_ = std::move(value);
    }
    std::string& parameter_name() noexcept {
        return m_parameter_name_;
    }

    const std::string& parameter_name() const noexcept {
        return m_parameter_name_;
    }

    void parameter_name(const std::string& value) {

        m_parameter_name_ = value;
    }

    void parameter_name(std::string&& value) {
        m_parameter_name_ = std::move(value);
    }
    std::string& requester() noexcept {
        return m_requester_;
    }

    const std::string& requester() const noexcept {
        return m_requester_;
    }

    void requester(const std::string& value) {

        m_requester_ = value;
    }

    void requester(std::string&& value) {
        m_requester_ = std::move(value);
    }
    std::string& source() noexcept {
        return m_source_;
    }

    const std::string& source() const noexcept {
        return m_source_;
    }

    void source(const std::string& value) {

        m_source_ = value;
    }

    void source(std::string&& value) {
        m_source_ = std::move(value);
    }
    ::EVSNParameterOperation& operation() noexcept {
        return m_operation_;
    }

    const ::EVSNParameterOperation& operation() const noexcept {
        return m_operation_;
    }

    void operation(const ::EVSNParameterOperation& value) {

        m_operation_ = value;
    }

    void operation(::EVSNParameterOperation&& value) {
        m_operation_ = std::move(value);
    }
    ::EVSNParameterValue& value() noexcept {
        return m_value_;
    }

    const ::EVSNParameterValue& value() const noexcept {
        return m_value_;
    }

    void value(const ::EVSNParameterValue& value) {

        m_value_ = value;
    }

    void value(::EVSNParameterValue&& value) {
        m_value_ = std::move(value);
    }
    bool operator == (const EVSNParameterRequest& other_) const;
    bool operator != (const EVSNParameterRequest& other_) const;

    void swap(EVSNParameterRequest& other_) noexcept ;

  private:

    std::string m_node_name_;
    std::string m_parameter_name_;
    std::string m_requester_;
    std::string m_source_;
    ::EVSNParameterOperation m_operation_;
    ::EVSNParameterValue m_value_;

};

inline void swap(EVSNParameterRequest& a, EVSNParameterRequest& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNParameterRequest& sample);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
// On Windows, dll-export template instantiations of standard types used by
// other dll-exported types
template class NDDSUSERDllExport std::allocator< ::EVSNParameterState >;
template class NDDSUSERDllExport std::vector< ::EVSNParameterState >;
template class NDDSUSERDllExport std::allocator< ::EVSNParameterDescriptor >;
template class NDDSUSERDllExport std::vector< ::EVSNParameterDescriptor >;
#endif
class NDDSUSERDllExport EVSNParameterReply {
  public:

    EVSNParameterReply();

    EVSNParameterReply(const std::string& node_name_,const ::EVSNParameterOperation& operation_,bool accepted_,const std::string& reason_,const ::rti::core::bounded_sequence< ::EVSNParameterState, 128L >& states_,const ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L >& descriptors_);

    std::string& node_name() noexcept {
        return m_node_name_;
    }

    const std::string& node_name() const noexcept {
        return m_node_name_;
    }

    void node_name(const std::string& value) {

        m_node_name_ = value;
    }

    void node_name(std::string&& value) {
        m_node_name_ = std::move(value);
    }
    ::EVSNParameterOperation& operation() noexcept {
        return m_operation_;
    }

    const ::EVSNParameterOperation& operation() const noexcept {
        return m_operation_;
    }

    void operation(const ::EVSNParameterOperation& value) {

        m_operation_ = value;
    }

    void operation(::EVSNParameterOperation&& value) {
        m_operation_ = std::move(value);
    }
    bool& accepted() noexcept {
        return m_accepted_;
    }

    const bool& accepted() const noexcept {
        return m_accepted_;
    }

    void accepted(bool value) {

        m_accepted_ = value;
    }

    std::string& reason() noexcept {
        return m_reason_;
    }

    const std::string& reason() const noexcept {
        return m_reason_;
    }

    void reason(const std::string& value) {

        m_reason_ = value;
    }

    void reason(std::string&& value) {
        m_reason_ = std::move(value);
    }
    ::rti::core::bounded_sequence< ::EVSNParameterState, 128L >& states() noexcept {
        return m_states_;
    }

    const ::rti::core::bounded_sequence< ::EVSNParameterState, 128L >& states() const noexcept {
        return m_states_;
    }

    void states(const ::rti::core::bounded_sequence< ::EVSNParameterState, 128L >& value) {

        m_states_ = value;
    }

    void states(::rti::core::bounded_sequence< ::EVSNParameterState, 128L >&& value) {
        m_states_ = std::move(value);
    }
    ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L >& descriptors() noexcept {
        return m_descriptors_;
    }

    const ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L >& descriptors() const noexcept {
        return m_descriptors_;
    }

    void descriptors(const ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L >& value) {

        m_descriptors_ = value;
    }

    void descriptors(::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L >&& value) {
        m_descriptors_ = std::move(value);
    }
    bool operator == (const EVSNParameterReply& other_) const;
    bool operator != (const EVSNParameterReply& other_) const;

    void swap(EVSNParameterReply& other_) noexcept ;

  private:

    std::string m_node_name_;
    ::EVSNParameterOperation m_operation_;
    bool m_accepted_;
    std::string m_reason_;
    ::rti::core::bounded_sequence< ::EVSNParameterState, 128L > m_states_;
    ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L > m_descriptors_;

};

inline void swap(EVSNParameterReply& a, EVSNParameterReply& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNParameterReply& sample);

#ifdef NDDS_STANDALONE_TYPE
namespace rti { 
    namespace topic {
        template <>
        struct default_enumerator< ::EVSNParameterType>
        {
            static const ::EVSNParameterType value;
        };
        template <>
        struct default_enumerator< ::EVSNParameterOperation>
        {
            static const ::EVSNParameterOperation value;
        };
    }
}
#else

namespace rti {
    namespace flat {
        namespace topic {
        }
    }
}
namespace dds {
    namespace topic {

        template<>
        struct topic_type_name< ::EVSNParameterValue > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNParameterValue";
            }
        };

        template<>
        struct is_topic_type< ::EVSNParameterValue > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNParameterValue > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNParameterValue& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNParameterValue& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNParameterValue& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNParameterValue& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNParameterDescriptor > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNParameterDescriptor";
            }
        };

        template<>
        struct is_topic_type< ::EVSNParameterDescriptor > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNParameterDescriptor > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNParameterDescriptor& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNParameterDescriptor& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNParameterDescriptor& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNParameterDescriptor& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNParameterState > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNParameterState";
            }
        };

        template<>
        struct is_topic_type< ::EVSNParameterState > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNParameterState > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNParameterState& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNParameterState& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNParameterState& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNParameterState& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNParameterEvent > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNParameterEvent";
            }
        };

        template<>
        struct is_topic_type< ::EVSNParameterEvent > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNParameterEvent > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNParameterEvent& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNParameterEvent& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNParameterEvent& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNParameterEvent& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNParameterRequest > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNParameterRequest";
            }
        };

        template<>
        struct is_topic_type< ::EVSNParameterRequest > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNParameterRequest > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNParameterRequest& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNParameterRequest& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNParameterRequest& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNParameterRequest& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNParameterReply > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNParameterReply";
            }
        };

        template<>
        struct is_topic_type< ::EVSNParameterReply > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNParameterReply > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNParameterReply& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNParameterReply& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNParameterReply& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNParameterReply& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
    }
}

namespace rti { 
    namespace topic {

        template <>
        struct default_enumerator< ::EVSNParameterType>
        {
            static const ::EVSNParameterType value;
        };
        template<>
        struct dynamic_type< ::EVSNParameterType > {
            typedef ::dds::core::xtypes::EnumType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::EnumType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterType > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template <>
        struct default_enumerator< ::EVSNParameterOperation>
        {
            static const ::EVSNParameterOperation value;
        };
        template<>
        struct dynamic_type< ::EVSNParameterOperation > {
            typedef ::dds::core::xtypes::EnumType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::EnumType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterOperation > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNParameterValue > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterValue > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNParameterDescriptor > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterDescriptor > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNParameterState > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterState > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNParameterEvent > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterEvent > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNParameterRequest > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterRequest > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNParameterReply > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNParameterReply > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

    }
}

#endif // NDDS_STANDALONE_TYPE
#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif // evsn_params_288152163_hpp

