

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_params.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#include <iosfwd>
#include <iomanip>
#include <atomic>
#include <cmath>
#include <limits>

#ifndef NDDS_STANDALONE_TYPE
#include "rti/topic/cdr/Serialization.hpp"
#include "evsn_paramsPlugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_params.hpp"

#include <rti/util/ostream_operators.hpp>

std::ostream& operator << (std::ostream& o,const EVSNParameterType& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    switch(sample){
        case EVSNParameterType::EVSN_PARAM_BOOL:
        o << "EVSNParameterType::EVSN_PARAM_BOOL" << " ";
        break;
        case EVSNParameterType::EVSN_PARAM_INT64:
        o << "EVSNParameterType::EVSN_PARAM_INT64" << " ";
        break;
        case EVSNParameterType::EVSN_PARAM_FLOAT64:
        o << "EVSNParameterType::EVSN_PARAM_FLOAT64" << " ";
        break;
        case EVSNParameterType::EVSN_PARAM_STRING:
        o << "EVSNParameterType::EVSN_PARAM_STRING" << " ";
        break;
        default:
        {
            /* Prevents compiler warnings */
        }
    }
    return o;
}

std::ostream& operator << (std::ostream& o,const EVSNParameterOperation& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    switch(sample){
        case EVSNParameterOperation::EVSN_PARAM_GET:
        o << "EVSNParameterOperation::EVSN_PARAM_GET" << " ";
        break;
        case EVSNParameterOperation::EVSN_PARAM_SET:
        o << "EVSNParameterOperation::EVSN_PARAM_SET" << " ";
        break;
        case EVSNParameterOperation::EVSN_PARAM_LIST:
        o << "EVSNParameterOperation::EVSN_PARAM_LIST" << " ";
        break;
        case EVSNParameterOperation::EVSN_PARAM_DESCRIBE:
        o << "EVSNParameterOperation::EVSN_PARAM_DESCRIBE" << " ";
        break;
        default:
        {
            /* Prevents compiler warnings */
        }
    }
    return o;
}

// ---- EVSNParameterValue: 

EVSNParameterValue::EVSNParameterValue() :
    m_value_type_(EVSNParameterType::EVSN_PARAM_BOOL) ,
    m_bool_value_ (0) ,
    m_int_value_ (0ll) ,
    m_float_value_ (0.0) ,
    m_string_value_ ("")  {

}   

EVSNParameterValue::EVSNParameterValue (const ::EVSNParameterType& value_type_,bool bool_value_,int64_t int_value_,double float_value_,const std::string& string_value_):
    m_value_type_(value_type_), 
    m_bool_value_(bool_value_), 
    m_int_value_(int_value_), 
    m_float_value_(float_value_), 
    m_string_value_(string_value_) {
}

void EVSNParameterValue::swap(EVSNParameterValue& other_)  noexcept 
{
    using std::swap;
    swap(m_value_type_, other_.m_value_type_);
    swap(m_bool_value_, other_.m_bool_value_);
    swap(m_int_value_, other_.m_int_value_);
    swap(m_float_value_, other_.m_float_value_);
    swap(m_string_value_, other_.m_string_value_);
}  

bool EVSNParameterValue::operator == (const EVSNParameterValue& other_) const {
    if (m_value_type_ != other_.m_value_type_) {
        return false;
    }
    if (m_bool_value_ != other_.m_bool_value_) {
        return false;
    }
    if (m_int_value_ != other_.m_int_value_) {
        return false;
    }
    if (std::fabs(m_float_value_ - other_.m_float_value_) > std::numeric_limits< double>::epsilon()
    && !(std::fabs(m_float_value_ - other_.m_float_value_) < (std::numeric_limits< double>::min)())) {
        return false;
    }
    if (m_string_value_ != other_.m_string_value_) {
        return false;
    }
    return true;
}

bool EVSNParameterValue::operator != (const EVSNParameterValue& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNParameterValue& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "value_type: " << sample.value_type ()<<", ";
    o << "bool_value: " << sample.bool_value ()<<", ";
    o << "int_value: " << sample.int_value ()<<", ";
    o << "float_value: " << std::setprecision(15) << sample.float_value ()<<", ";
    o << "string_value: " << sample.string_value ();
    o <<"]";
    return o;
}

// ---- EVSNParameterDescriptor: 

EVSNParameterDescriptor::EVSNParameterDescriptor() :
    m_node_name_ ("") ,
    m_parameter_name_ ("") ,
    m_value_type_(EVSNParameterType::EVSN_PARAM_BOOL) ,
    m_mutable_at_runtime_ (0) ,
    m_has_min_ (0) ,
    m_min_value_ (0.0) ,
    m_has_max_ (0) ,
    m_max_value_ (0.0) ,
    m_unit_ ("") ,
    m_description_ ("") ,
    m_fallback_behavior_ ("")  {

}   

EVSNParameterDescriptor::EVSNParameterDescriptor (const std::string& node_name_,const std::string& parameter_name_,const ::EVSNParameterType& value_type_,const ::EVSNParameterValue& default_value_,bool mutable_at_runtime_,bool has_min_,double min_value_,bool has_max_,double max_value_,const std::string& unit_,const std::string& description_,const std::string& fallback_behavior_):
    m_node_name_(node_name_), 
    m_parameter_name_(parameter_name_), 
    m_value_type_(value_type_), 
    m_default_value_(default_value_), 
    m_mutable_at_runtime_(mutable_at_runtime_), 
    m_has_min_(has_min_), 
    m_min_value_(min_value_), 
    m_has_max_(has_max_), 
    m_max_value_(max_value_), 
    m_unit_(unit_), 
    m_description_(description_), 
    m_fallback_behavior_(fallback_behavior_) {
}

void EVSNParameterDescriptor::swap(EVSNParameterDescriptor& other_)  noexcept 
{
    using std::swap;
    swap(m_node_name_, other_.m_node_name_);
    swap(m_parameter_name_, other_.m_parameter_name_);
    swap(m_value_type_, other_.m_value_type_);
    swap(m_default_value_, other_.m_default_value_);
    swap(m_mutable_at_runtime_, other_.m_mutable_at_runtime_);
    swap(m_has_min_, other_.m_has_min_);
    swap(m_min_value_, other_.m_min_value_);
    swap(m_has_max_, other_.m_has_max_);
    swap(m_max_value_, other_.m_max_value_);
    swap(m_unit_, other_.m_unit_);
    swap(m_description_, other_.m_description_);
    swap(m_fallback_behavior_, other_.m_fallback_behavior_);
}  

bool EVSNParameterDescriptor::operator == (const EVSNParameterDescriptor& other_) const {
    if (m_node_name_ != other_.m_node_name_) {
        return false;
    }
    if (m_parameter_name_ != other_.m_parameter_name_) {
        return false;
    }
    if (m_value_type_ != other_.m_value_type_) {
        return false;
    }
    if (m_default_value_ != other_.m_default_value_) {
        return false;
    }
    if (m_mutable_at_runtime_ != other_.m_mutable_at_runtime_) {
        return false;
    }
    if (m_has_min_ != other_.m_has_min_) {
        return false;
    }
    if (std::fabs(m_min_value_ - other_.m_min_value_) > std::numeric_limits< double>::epsilon()
    && !(std::fabs(m_min_value_ - other_.m_min_value_) < (std::numeric_limits< double>::min)())) {
        return false;
    }
    if (m_has_max_ != other_.m_has_max_) {
        return false;
    }
    if (std::fabs(m_max_value_ - other_.m_max_value_) > std::numeric_limits< double>::epsilon()
    && !(std::fabs(m_max_value_ - other_.m_max_value_) < (std::numeric_limits< double>::min)())) {
        return false;
    }
    if (m_unit_ != other_.m_unit_) {
        return false;
    }
    if (m_description_ != other_.m_description_) {
        return false;
    }
    if (m_fallback_behavior_ != other_.m_fallback_behavior_) {
        return false;
    }
    return true;
}

bool EVSNParameterDescriptor::operator != (const EVSNParameterDescriptor& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNParameterDescriptor& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "node_name: " << sample.node_name ()<<", ";
    o << "parameter_name: " << sample.parameter_name ()<<", ";
    o << "value_type: " << sample.value_type ()<<", ";
    o << "default_value: " << sample.default_value ()<<", ";
    o << "mutable_at_runtime: " << sample.mutable_at_runtime ()<<", ";
    o << "has_min: " << sample.has_min ()<<", ";
    o << "min_value: " << std::setprecision(15) << sample.min_value ()<<", ";
    o << "has_max: " << sample.has_max ()<<", ";
    o << "max_value: " << std::setprecision(15) << sample.max_value ()<<", ";
    o << "unit: " << sample.unit ()<<", ";
    o << "description: " << sample.description ()<<", ";
    o << "fallback_behavior: " << sample.fallback_behavior ();
    o <<"]";
    return o;
}

// ---- EVSNParameterState: 

EVSNParameterState::EVSNParameterState() :
    m_node_name_ ("") ,
    m_parameter_name_ ("") ,
    m_mutable_at_runtime_ (0) ,
    m_unit_ ("") ,
    m_description_ ("")  {

}   

EVSNParameterState::EVSNParameterState (const std::string& node_name_,const std::string& parameter_name_,const ::EVSNParameterValue& value_,bool mutable_at_runtime_,const std::string& unit_,const std::string& description_):
    m_node_name_(node_name_), 
    m_parameter_name_(parameter_name_), 
    m_value_(value_), 
    m_mutable_at_runtime_(mutable_at_runtime_), 
    m_unit_(unit_), 
    m_description_(description_) {
}

void EVSNParameterState::swap(EVSNParameterState& other_)  noexcept 
{
    using std::swap;
    swap(m_node_name_, other_.m_node_name_);
    swap(m_parameter_name_, other_.m_parameter_name_);
    swap(m_value_, other_.m_value_);
    swap(m_mutable_at_runtime_, other_.m_mutable_at_runtime_);
    swap(m_unit_, other_.m_unit_);
    swap(m_description_, other_.m_description_);
}  

bool EVSNParameterState::operator == (const EVSNParameterState& other_) const {
    if (m_node_name_ != other_.m_node_name_) {
        return false;
    }
    if (m_parameter_name_ != other_.m_parameter_name_) {
        return false;
    }
    if (m_value_ != other_.m_value_) {
        return false;
    }
    if (m_mutable_at_runtime_ != other_.m_mutable_at_runtime_) {
        return false;
    }
    if (m_unit_ != other_.m_unit_) {
        return false;
    }
    if (m_description_ != other_.m_description_) {
        return false;
    }
    return true;
}

bool EVSNParameterState::operator != (const EVSNParameterState& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNParameterState& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "node_name: " << sample.node_name ()<<", ";
    o << "parameter_name: " << sample.parameter_name ()<<", ";
    o << "value: " << sample.value ()<<", ";
    o << "mutable_at_runtime: " << sample.mutable_at_runtime ()<<", ";
    o << "unit: " << sample.unit ()<<", ";
    o << "description: " << sample.description ();
    o <<"]";
    return o;
}

// ---- EVSNParameterEvent: 

EVSNParameterEvent::EVSNParameterEvent() :
    m_node_name_ ("") ,
    m_parameter_name_ ("") ,
    m_requester_ ("") ,
    m_source_ ("") ,
    m_timestamp_unix_ms_ (0ll) ,
    m_accepted_ (0) ,
    m_reason_ ("")  {

}   

EVSNParameterEvent::EVSNParameterEvent (const std::string& node_name_,const std::string& parameter_name_,const std::string& requester_,const std::string& source_,int64_t timestamp_unix_ms_,bool accepted_,const std::string& reason_,const ::EVSNParameterValue& requested_value_,const ::EVSNParameterValue& resulting_value_):
    m_node_name_(node_name_), 
    m_parameter_name_(parameter_name_), 
    m_requester_(requester_), 
    m_source_(source_), 
    m_timestamp_unix_ms_(timestamp_unix_ms_), 
    m_accepted_(accepted_), 
    m_reason_(reason_), 
    m_requested_value_(requested_value_), 
    m_resulting_value_(resulting_value_) {
}

void EVSNParameterEvent::swap(EVSNParameterEvent& other_)  noexcept 
{
    using std::swap;
    swap(m_node_name_, other_.m_node_name_);
    swap(m_parameter_name_, other_.m_parameter_name_);
    swap(m_requester_, other_.m_requester_);
    swap(m_source_, other_.m_source_);
    swap(m_timestamp_unix_ms_, other_.m_timestamp_unix_ms_);
    swap(m_accepted_, other_.m_accepted_);
    swap(m_reason_, other_.m_reason_);
    swap(m_requested_value_, other_.m_requested_value_);
    swap(m_resulting_value_, other_.m_resulting_value_);
}  

bool EVSNParameterEvent::operator == (const EVSNParameterEvent& other_) const {
    if (m_node_name_ != other_.m_node_name_) {
        return false;
    }
    if (m_parameter_name_ != other_.m_parameter_name_) {
        return false;
    }
    if (m_requester_ != other_.m_requester_) {
        return false;
    }
    if (m_source_ != other_.m_source_) {
        return false;
    }
    if (m_timestamp_unix_ms_ != other_.m_timestamp_unix_ms_) {
        return false;
    }
    if (m_accepted_ != other_.m_accepted_) {
        return false;
    }
    if (m_reason_ != other_.m_reason_) {
        return false;
    }
    if (m_requested_value_ != other_.m_requested_value_) {
        return false;
    }
    if (m_resulting_value_ != other_.m_resulting_value_) {
        return false;
    }
    return true;
}

bool EVSNParameterEvent::operator != (const EVSNParameterEvent& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNParameterEvent& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "node_name: " << sample.node_name ()<<", ";
    o << "parameter_name: " << sample.parameter_name ()<<", ";
    o << "requester: " << sample.requester ()<<", ";
    o << "source: " << sample.source ()<<", ";
    o << "timestamp_unix_ms: " << sample.timestamp_unix_ms ()<<", ";
    o << "accepted: " << sample.accepted ()<<", ";
    o << "reason: " << sample.reason ()<<", ";
    o << "requested_value: " << sample.requested_value ()<<", ";
    o << "resulting_value: " << sample.resulting_value ();
    o <<"]";
    return o;
}

// ---- EVSNParameterRequest: 

EVSNParameterRequest::EVSNParameterRequest() :
    m_node_name_ ("") ,
    m_parameter_name_ ("") ,
    m_requester_ ("") ,
    m_source_ ("") ,
    m_operation_(EVSNParameterOperation::EVSN_PARAM_GET)  {

}   

EVSNParameterRequest::EVSNParameterRequest (const std::string& node_name_,const std::string& parameter_name_,const std::string& requester_,const std::string& source_,const ::EVSNParameterOperation& operation_,const ::EVSNParameterValue& value_):
    m_node_name_(node_name_), 
    m_parameter_name_(parameter_name_), 
    m_requester_(requester_), 
    m_source_(source_), 
    m_operation_(operation_), 
    m_value_(value_) {
}

void EVSNParameterRequest::swap(EVSNParameterRequest& other_)  noexcept 
{
    using std::swap;
    swap(m_node_name_, other_.m_node_name_);
    swap(m_parameter_name_, other_.m_parameter_name_);
    swap(m_requester_, other_.m_requester_);
    swap(m_source_, other_.m_source_);
    swap(m_operation_, other_.m_operation_);
    swap(m_value_, other_.m_value_);
}  

bool EVSNParameterRequest::operator == (const EVSNParameterRequest& other_) const {
    if (m_node_name_ != other_.m_node_name_) {
        return false;
    }
    if (m_parameter_name_ != other_.m_parameter_name_) {
        return false;
    }
    if (m_requester_ != other_.m_requester_) {
        return false;
    }
    if (m_source_ != other_.m_source_) {
        return false;
    }
    if (m_operation_ != other_.m_operation_) {
        return false;
    }
    if (m_value_ != other_.m_value_) {
        return false;
    }
    return true;
}

bool EVSNParameterRequest::operator != (const EVSNParameterRequest& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNParameterRequest& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "node_name: " << sample.node_name ()<<", ";
    o << "parameter_name: " << sample.parameter_name ()<<", ";
    o << "requester: " << sample.requester ()<<", ";
    o << "source: " << sample.source ()<<", ";
    o << "operation: " << sample.operation ()<<", ";
    o << "value: " << sample.value ();
    o <<"]";
    return o;
}

// ---- EVSNParameterReply: 

EVSNParameterReply::EVSNParameterReply() :
    m_node_name_ ("") ,
    m_operation_(EVSNParameterOperation::EVSN_PARAM_GET) ,
    m_accepted_ (0) ,
    m_reason_ ("")  {

}   

EVSNParameterReply::EVSNParameterReply (const std::string& node_name_,const ::EVSNParameterOperation& operation_,bool accepted_,const std::string& reason_,const ::rti::core::bounded_sequence< ::EVSNParameterState, 128L >& states_,const ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L >& descriptors_):
    m_node_name_(node_name_), 
    m_operation_(operation_), 
    m_accepted_(accepted_), 
    m_reason_(reason_), 
    m_states_(states_), 
    m_descriptors_(descriptors_) {
}

void EVSNParameterReply::swap(EVSNParameterReply& other_)  noexcept 
{
    using std::swap;
    swap(m_node_name_, other_.m_node_name_);
    swap(m_operation_, other_.m_operation_);
    swap(m_accepted_, other_.m_accepted_);
    swap(m_reason_, other_.m_reason_);
    swap(m_states_, other_.m_states_);
    swap(m_descriptors_, other_.m_descriptors_);
}  

bool EVSNParameterReply::operator == (const EVSNParameterReply& other_) const {
    if (m_node_name_ != other_.m_node_name_) {
        return false;
    }
    if (m_operation_ != other_.m_operation_) {
        return false;
    }
    if (m_accepted_ != other_.m_accepted_) {
        return false;
    }
    if (m_reason_ != other_.m_reason_) {
        return false;
    }
    if (m_states_ != other_.m_states_) {
        return false;
    }
    if (m_descriptors_ != other_.m_descriptors_) {
        return false;
    }
    return true;
}

bool EVSNParameterReply::operator != (const EVSNParameterReply& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNParameterReply& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "node_name: " << sample.node_name ()<<", ";
    o << "operation: " << sample.operation ()<<", ";
    o << "accepted: " << sample.accepted ()<<", ";
    o << "reason: " << sample.reason ()<<", ";
    o << "states: " << sample.states ()<<", ";
    o << "descriptors: " << sample.descriptors ();
    o <<"]";
    return o;
}

#ifdef NDDS_STANDALONE_TYPE
namespace rti {
    namespace topic {
        const ::EVSNParameterType default_enumerator< ::EVSNParameterType>::value = ::EVSNParameterType::EVSN_PARAM_BOOL;
        const ::EVSNParameterOperation default_enumerator< ::EVSNParameterOperation>::value = ::EVSNParameterOperation::EVSN_PARAM_GET;
    }
}

#else
// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        const ::EVSNParameterType default_enumerator< ::EVSNParameterType>::value = ::EVSNParameterType::EVSN_PARAM_BOOL;
        template<>
        struct native_type_code< ::EVSNParameterType > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNParameterType_g_tc_members[4]=
                {

                    {
                        (char *)"EVSN_PARAM_BOOL",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterType::EVSN_PARAM_BOOL), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"EVSN_PARAM_INT64",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterType::EVSN_PARAM_INT64), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"EVSN_PARAM_FLOAT64",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterType::EVSN_PARAM_FLOAT64), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"EVSN_PARAM_STRING",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterType::EVSN_PARAM_STRING), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterType_g_tc =
                {{
                        DDS_TK_ENUM, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterType", /* Name */
                        NULL,     /* Base class type code is assigned later */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        4, /* Number of members */
                        EVSNParameterType_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Type Modifier */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterType*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterType_g_tc;
                }

                EVSNParameterType_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                /* Initialize the values for annotations. */
                EVSNParameterType_g_tc._data._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNParameterType_g_tc._data._annotations._defaultValue._u.long_value = 0;

                EVSNParameterType_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterType_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterType_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                static RTIXCdrMemberAccessInfo EVSNParameterType_g_memberAccessInfos[1] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterType_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterType_g_sampleAccessInfo;
                }

                EVSNParameterType_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 0;

                EVSNParameterType_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterType_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterType);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterType_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterType_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterType_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterType_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterType >;

                EVSNParameterType_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterType_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterType_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterType_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::EnumType& dynamic_type< ::EVSNParameterType >::get()
        {
            return static_cast<const ::dds::core::xtypes::EnumType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterType >::get())));
        }

        const ::EVSNParameterOperation default_enumerator< ::EVSNParameterOperation>::value = ::EVSNParameterOperation::EVSN_PARAM_GET;
        template<>
        struct native_type_code< ::EVSNParameterOperation > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNParameterOperation_g_tc_members[4]=
                {

                    {
                        (char *)"EVSN_PARAM_GET",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterOperation::EVSN_PARAM_GET), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"EVSN_PARAM_SET",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterOperation::EVSN_PARAM_SET), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"EVSN_PARAM_LIST",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterOperation::EVSN_PARAM_LIST), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"EVSN_PARAM_DESCRIBE",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNParameterOperation::EVSN_PARAM_DESCRIBE), 
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PRIVATE_MEMBER,/* Member visibility */ 

                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterOperation_g_tc =
                {{
                        DDS_TK_ENUM, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterOperation", /* Name */
                        NULL,     /* Base class type code is assigned later */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        4, /* Number of members */
                        EVSNParameterOperation_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Type Modifier */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterOperation*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterOperation_g_tc;
                }

                EVSNParameterOperation_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                /* Initialize the values for annotations. */
                EVSNParameterOperation_g_tc._data._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNParameterOperation_g_tc._data._annotations._defaultValue._u.long_value = 0;

                EVSNParameterOperation_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterOperation_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterOperation_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                static RTIXCdrMemberAccessInfo EVSNParameterOperation_g_memberAccessInfos[1] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterOperation_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterOperation_g_sampleAccessInfo;
                }

                EVSNParameterOperation_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 0;

                EVSNParameterOperation_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterOperation_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterOperation);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterOperation_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterOperation_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterOperation_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterOperation_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterOperation >;

                EVSNParameterOperation_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterOperation_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterOperation_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterOperation_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::EnumType& dynamic_type< ::EVSNParameterOperation >::get()
        {
            return static_cast<const ::dds::core::xtypes::EnumType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterOperation >::get())));
        }

        template<>
        struct native_type_code< ::EVSNParameterValue > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNParameterValue_g_tc_string_value_string;

                static DDS_TypeCode_Member EVSNParameterValue_g_tc_members[5]=
                {

                    {
                        (char *)"value_type",/* Member name */
                        {
                            0,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"bool_value",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"int_value",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"float_value",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"string_value",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterValue_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterValue", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        5, /* Number of members */
                        EVSNParameterValue_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterValue*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterValue_g_tc;
                }

                EVSNParameterValue_g_tc_string_value_string = initialize_string_typecode((256L));

                EVSNParameterValue_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNParameterValue_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterType>::get().native();
                EVSNParameterValue_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterValue_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_longlong;
                EVSNParameterValue_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_double;
                EVSNParameterValue_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterValue_g_tc_string_value_string;

                /* Initialize the values for member annotations. */
                EVSNParameterValue_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNParameterValue_g_tc_members[0]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNParameterValue_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterValue_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterValue_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_LONGLONG;
                EVSNParameterValue_g_tc_members[2]._annotations._defaultValue._u.long_long_value = 0ll;
                EVSNParameterValue_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_LONGLONG;
                EVSNParameterValue_g_tc_members[2]._annotations._minValue._u.long_long_value = RTIXCdrLongLong_MIN;
                EVSNParameterValue_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_LONGLONG;
                EVSNParameterValue_g_tc_members[2]._annotations._maxValue._u.long_long_value = RTIXCdrLongLong_MAX;
                EVSNParameterValue_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterValue_g_tc_members[3]._annotations._defaultValue._u.double_value = 0.0;
                EVSNParameterValue_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterValue_g_tc_members[3]._annotations._minValue._u.double_value = RTIXCdrDouble_MIN;
                EVSNParameterValue_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterValue_g_tc_members[3]._annotations._maxValue._u.double_value = RTIXCdrDouble_MAX;
                EVSNParameterValue_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterValue_g_tc_members[4]._annotations._defaultValue._u.string_value = (DDS_Char *) "";

                EVSNParameterValue_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterValue_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterValue_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNParameterValue *sample;

                static RTIXCdrMemberAccessInfo EVSNParameterValue_g_memberAccessInfos[5] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterValue_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterValue_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNParameterValue);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNParameterValue_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->value_type() - (char *)sample);

                EVSNParameterValue_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->bool_value() - (char *)sample);

                EVSNParameterValue_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->int_value() - (char *)sample);

                EVSNParameterValue_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->float_value() - (char *)sample);

                EVSNParameterValue_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->string_value() - (char *)sample);

                EVSNParameterValue_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterValue_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterValue);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterValue_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterValue_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterValue_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterValue_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterValue >;

                EVSNParameterValue_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterValue_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterValue_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterValue_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNParameterValue >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterValue >::get())));
        }

        template<>
        struct native_type_code< ::EVSNParameterDescriptor > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNParameterDescriptor_g_tc_node_name_string;
                static DDS_TypeCode EVSNParameterDescriptor_g_tc_parameter_name_string;
                static DDS_TypeCode EVSNParameterDescriptor_g_tc_unit_string;
                static DDS_TypeCode EVSNParameterDescriptor_g_tc_description_string;
                static DDS_TypeCode EVSNParameterDescriptor_g_tc_fallback_behavior_string;

                static DDS_TypeCode_Member EVSNParameterDescriptor_g_tc_members[12]=
                {

                    {
                        (char *)"node_name",/* Member name */
                        {
                            0,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"parameter_name",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"value_type",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"default_value",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"mutable_at_runtime",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"has_min",/* Member name */
                        {
                            5,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"min_value",/* Member name */
                        {
                            6,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"has_max",/* Member name */
                        {
                            7,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"max_value",/* Member name */
                        {
                            8,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"unit",/* Member name */
                        {
                            9,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"description",/* Member name */
                        {
                            10,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"fallback_behavior",/* Member name */
                        {
                            11,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterDescriptor_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterDescriptor", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        12, /* Number of members */
                        EVSNParameterDescriptor_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterDescriptor*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterDescriptor_g_tc;
                }

                EVSNParameterDescriptor_g_tc_node_name_string = initialize_string_typecode((64L));
                EVSNParameterDescriptor_g_tc_parameter_name_string = initialize_string_typecode((128L));
                EVSNParameterDescriptor_g_tc_unit_string = initialize_string_typecode((32L));
                EVSNParameterDescriptor_g_tc_description_string = initialize_string_typecode((256L));
                EVSNParameterDescriptor_g_tc_fallback_behavior_string = initialize_string_typecode((256L));

                EVSNParameterDescriptor_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNParameterDescriptor_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterDescriptor_g_tc_node_name_string;
                EVSNParameterDescriptor_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterDescriptor_g_tc_parameter_name_string;
                EVSNParameterDescriptor_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterType>::get().native();
                EVSNParameterDescriptor_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterValue>::get().native();
                EVSNParameterDescriptor_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterDescriptor_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterDescriptor_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_double;
                EVSNParameterDescriptor_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterDescriptor_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_double;
                EVSNParameterDescriptor_g_tc_members[9]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterDescriptor_g_tc_unit_string;
                EVSNParameterDescriptor_g_tc_members[10]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterDescriptor_g_tc_description_string;
                EVSNParameterDescriptor_g_tc_members[11]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterDescriptor_g_tc_fallback_behavior_string;

                /* Initialize the values for member annotations. */
                EVSNParameterDescriptor_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterDescriptor_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterDescriptor_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterDescriptor_g_tc_members[1]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterDescriptor_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNParameterDescriptor_g_tc_members[2]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNParameterDescriptor_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterDescriptor_g_tc_members[4]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterDescriptor_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterDescriptor_g_tc_members[5]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterDescriptor_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterDescriptor_g_tc_members[6]._annotations._defaultValue._u.double_value = 0.0;
                EVSNParameterDescriptor_g_tc_members[6]._annotations._minValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterDescriptor_g_tc_members[6]._annotations._minValue._u.double_value = RTIXCdrDouble_MIN;
                EVSNParameterDescriptor_g_tc_members[6]._annotations._maxValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterDescriptor_g_tc_members[6]._annotations._maxValue._u.double_value = RTIXCdrDouble_MAX;
                EVSNParameterDescriptor_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterDescriptor_g_tc_members[7]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterDescriptor_g_tc_members[8]._annotations._defaultValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterDescriptor_g_tc_members[8]._annotations._defaultValue._u.double_value = 0.0;
                EVSNParameterDescriptor_g_tc_members[8]._annotations._minValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterDescriptor_g_tc_members[8]._annotations._minValue._u.double_value = RTIXCdrDouble_MIN;
                EVSNParameterDescriptor_g_tc_members[8]._annotations._maxValue._d = RTI_XCDR_TK_DOUBLE;
                EVSNParameterDescriptor_g_tc_members[8]._annotations._maxValue._u.double_value = RTIXCdrDouble_MAX;
                EVSNParameterDescriptor_g_tc_members[9]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterDescriptor_g_tc_members[9]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterDescriptor_g_tc_members[10]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterDescriptor_g_tc_members[10]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterDescriptor_g_tc_members[11]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterDescriptor_g_tc_members[11]._annotations._defaultValue._u.string_value = (DDS_Char *) "";

                EVSNParameterDescriptor_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterDescriptor_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterDescriptor_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNParameterDescriptor *sample;

                static RTIXCdrMemberAccessInfo EVSNParameterDescriptor_g_memberAccessInfos[12] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterDescriptor_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterDescriptor_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNParameterDescriptor);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNParameterDescriptor_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->node_name() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->parameter_name() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->value_type() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->default_value() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mutable_at_runtime() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->has_min() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->min_value() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->has_max() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->max_value() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[9].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->unit() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[10].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->description() - (char *)sample);

                EVSNParameterDescriptor_g_memberAccessInfos[11].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->fallback_behavior() - (char *)sample);

                EVSNParameterDescriptor_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterDescriptor_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterDescriptor);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterDescriptor_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterDescriptor_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterDescriptor_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterDescriptor_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterDescriptor >;

                EVSNParameterDescriptor_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterDescriptor_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterDescriptor_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterDescriptor_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNParameterDescriptor >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterDescriptor >::get())));
        }

        template<>
        struct native_type_code< ::EVSNParameterState > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNParameterState_g_tc_node_name_string;
                static DDS_TypeCode EVSNParameterState_g_tc_parameter_name_string;
                static DDS_TypeCode EVSNParameterState_g_tc_unit_string;
                static DDS_TypeCode EVSNParameterState_g_tc_description_string;

                static DDS_TypeCode_Member EVSNParameterState_g_tc_members[6]=
                {

                    {
                        (char *)"node_name",/* Member name */
                        {
                            0,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_KEY_MEMBER , /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"parameter_name",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_KEY_MEMBER , /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"value",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"mutable_at_runtime",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"unit",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"description",/* Member name */
                        {
                            5,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterState_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterState", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        6, /* Number of members */
                        EVSNParameterState_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterState*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterState_g_tc;
                }

                EVSNParameterState_g_tc_node_name_string = initialize_string_typecode((64L));
                EVSNParameterState_g_tc_parameter_name_string = initialize_string_typecode((128L));
                EVSNParameterState_g_tc_unit_string = initialize_string_typecode((32L));
                EVSNParameterState_g_tc_description_string = initialize_string_typecode((256L));

                EVSNParameterState_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNParameterState_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterState_g_tc_node_name_string;
                EVSNParameterState_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterState_g_tc_parameter_name_string;
                EVSNParameterState_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterValue>::get().native();
                EVSNParameterState_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterState_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterState_g_tc_unit_string;
                EVSNParameterState_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterState_g_tc_description_string;

                /* Initialize the values for member annotations. */
                EVSNParameterState_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterState_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterState_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterState_g_tc_members[1]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterState_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterState_g_tc_members[3]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterState_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterState_g_tc_members[4]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterState_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterState_g_tc_members[5]._annotations._defaultValue._u.string_value = (DDS_Char *) "";

                EVSNParameterState_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterState_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterState_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNParameterState *sample;

                static RTIXCdrMemberAccessInfo EVSNParameterState_g_memberAccessInfos[6] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterState_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterState_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNParameterState);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNParameterState_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->node_name() - (char *)sample);

                EVSNParameterState_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->parameter_name() - (char *)sample);

                EVSNParameterState_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->value() - (char *)sample);

                EVSNParameterState_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mutable_at_runtime() - (char *)sample);

                EVSNParameterState_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->unit() - (char *)sample);

                EVSNParameterState_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->description() - (char *)sample);

                EVSNParameterState_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterState_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterState);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterState_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterState_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterState_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterState_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterState >;

                EVSNParameterState_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterState_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterState_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterState_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNParameterState >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterState >::get())));
        }

        template<>
        struct native_type_code< ::EVSNParameterEvent > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNParameterEvent_g_tc_node_name_string;
                static DDS_TypeCode EVSNParameterEvent_g_tc_parameter_name_string;
                static DDS_TypeCode EVSNParameterEvent_g_tc_requester_string;
                static DDS_TypeCode EVSNParameterEvent_g_tc_source_string;
                static DDS_TypeCode EVSNParameterEvent_g_tc_reason_string;

                static DDS_TypeCode_Member EVSNParameterEvent_g_tc_members[9]=
                {

                    {
                        (char *)"node_name",/* Member name */
                        {
                            0,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"parameter_name",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"requester",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"source",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"timestamp_unix_ms",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"accepted",/* Member name */
                        {
                            5,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"reason",/* Member name */
                        {
                            6,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"requested_value",/* Member name */
                        {
                            7,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"resulting_value",/* Member name */
                        {
                            8,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterEvent_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterEvent", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        9, /* Number of members */
                        EVSNParameterEvent_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterEvent*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterEvent_g_tc;
                }

                EVSNParameterEvent_g_tc_node_name_string = initialize_string_typecode((64L));
                EVSNParameterEvent_g_tc_parameter_name_string = initialize_string_typecode((128L));
                EVSNParameterEvent_g_tc_requester_string = initialize_string_typecode((64L));
                EVSNParameterEvent_g_tc_source_string = initialize_string_typecode((64L));
                EVSNParameterEvent_g_tc_reason_string = initialize_string_typecode((256L));

                EVSNParameterEvent_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNParameterEvent_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterEvent_g_tc_node_name_string;
                EVSNParameterEvent_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterEvent_g_tc_parameter_name_string;
                EVSNParameterEvent_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterEvent_g_tc_requester_string;
                EVSNParameterEvent_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterEvent_g_tc_source_string;
                EVSNParameterEvent_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_longlong;
                EVSNParameterEvent_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterEvent_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterEvent_g_tc_reason_string;
                EVSNParameterEvent_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterValue>::get().native();
                EVSNParameterEvent_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterValue>::get().native();

                /* Initialize the values for member annotations. */
                EVSNParameterEvent_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterEvent_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterEvent_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterEvent_g_tc_members[1]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterEvent_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterEvent_g_tc_members[2]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterEvent_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterEvent_g_tc_members[3]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterEvent_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_LONGLONG;
                EVSNParameterEvent_g_tc_members[4]._annotations._defaultValue._u.long_long_value = 0ll;
                EVSNParameterEvent_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_LONGLONG;
                EVSNParameterEvent_g_tc_members[4]._annotations._minValue._u.long_long_value = RTIXCdrLongLong_MIN;
                EVSNParameterEvent_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_LONGLONG;
                EVSNParameterEvent_g_tc_members[4]._annotations._maxValue._u.long_long_value = RTIXCdrLongLong_MAX;
                EVSNParameterEvent_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterEvent_g_tc_members[5]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterEvent_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterEvent_g_tc_members[6]._annotations._defaultValue._u.string_value = (DDS_Char *) "";

                EVSNParameterEvent_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterEvent_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterEvent_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNParameterEvent *sample;

                static RTIXCdrMemberAccessInfo EVSNParameterEvent_g_memberAccessInfos[9] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterEvent_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterEvent_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNParameterEvent);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNParameterEvent_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->node_name() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->parameter_name() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->requester() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->source() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->timestamp_unix_ms() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->accepted() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->reason() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->requested_value() - (char *)sample);

                EVSNParameterEvent_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->resulting_value() - (char *)sample);

                EVSNParameterEvent_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterEvent_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterEvent);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterEvent_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterEvent_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterEvent_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterEvent_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterEvent >;

                EVSNParameterEvent_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterEvent_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterEvent_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterEvent_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNParameterEvent >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterEvent >::get())));
        }

        template<>
        struct native_type_code< ::EVSNParameterRequest > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNParameterRequest_g_tc_node_name_string;
                static DDS_TypeCode EVSNParameterRequest_g_tc_parameter_name_string;
                static DDS_TypeCode EVSNParameterRequest_g_tc_requester_string;
                static DDS_TypeCode EVSNParameterRequest_g_tc_source_string;

                static DDS_TypeCode_Member EVSNParameterRequest_g_tc_members[6]=
                {

                    {
                        (char *)"node_name",/* Member name */
                        {
                            0,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"parameter_name",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"requester",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"source",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"operation",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"value",/* Member name */
                        {
                            5,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterRequest_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterRequest", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        6, /* Number of members */
                        EVSNParameterRequest_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterRequest*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterRequest_g_tc;
                }

                EVSNParameterRequest_g_tc_node_name_string = initialize_string_typecode((64L));
                EVSNParameterRequest_g_tc_parameter_name_string = initialize_string_typecode((128L));
                EVSNParameterRequest_g_tc_requester_string = initialize_string_typecode((64L));
                EVSNParameterRequest_g_tc_source_string = initialize_string_typecode((64L));

                EVSNParameterRequest_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNParameterRequest_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterRequest_g_tc_node_name_string;
                EVSNParameterRequest_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterRequest_g_tc_parameter_name_string;
                EVSNParameterRequest_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterRequest_g_tc_requester_string;
                EVSNParameterRequest_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterRequest_g_tc_source_string;
                EVSNParameterRequest_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterOperation>::get().native();
                EVSNParameterRequest_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterValue>::get().native();

                /* Initialize the values for member annotations. */
                EVSNParameterRequest_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterRequest_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterRequest_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterRequest_g_tc_members[1]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterRequest_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterRequest_g_tc_members[2]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterRequest_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterRequest_g_tc_members[3]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterRequest_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNParameterRequest_g_tc_members[4]._annotations._defaultValue._u.enumerated_value = 0;

                EVSNParameterRequest_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterRequest_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterRequest_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNParameterRequest *sample;

                static RTIXCdrMemberAccessInfo EVSNParameterRequest_g_memberAccessInfos[6] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterRequest_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterRequest_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNParameterRequest);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNParameterRequest_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->node_name() - (char *)sample);

                EVSNParameterRequest_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->parameter_name() - (char *)sample);

                EVSNParameterRequest_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->requester() - (char *)sample);

                EVSNParameterRequest_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->source() - (char *)sample);

                EVSNParameterRequest_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->operation() - (char *)sample);

                EVSNParameterRequest_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->value() - (char *)sample);

                EVSNParameterRequest_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterRequest_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterRequest);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterRequest_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterRequest_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterRequest_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterRequest_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterRequest >;

                EVSNParameterRequest_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterRequest_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterRequest_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterRequest_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNParameterRequest >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterRequest >::get())));
        }

        template<>
        struct native_type_code< ::EVSNParameterReply > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNParameterReply_g_tc_node_name_string;
                static DDS_TypeCode EVSNParameterReply_g_tc_reason_string;
                static DDS_TypeCode EVSNParameterReply_g_tc_states_sequence;
                static DDS_TypeCode EVSNParameterReply_g_tc_descriptors_sequence;

                static DDS_TypeCode_Member EVSNParameterReply_g_tc_members[6]=
                {

                    {
                        (char *)"node_name",/* Member name */
                        {
                            0,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"operation",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"accepted",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"reason",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"states",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"descriptors",/* Member name */
                        {
                            5,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        1,
                        NULL, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode EVSNParameterReply_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNParameterReply", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        6, /* Number of members */
                        EVSNParameterReply_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNParameterReply*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNParameterReply_g_tc;
                }

                EVSNParameterReply_g_tc_node_name_string = initialize_string_typecode((64L));
                EVSNParameterReply_g_tc_reason_string = initialize_string_typecode((256L));
                EVSNParameterReply_g_tc_states_sequence = initialize_sequence_typecode< ::rti::core::bounded_sequence< ::EVSNParameterState, 128L > >((128L));
                EVSNParameterReply_g_tc_descriptors_sequence = initialize_sequence_typecode< ::rti::core::bounded_sequence< ::EVSNParameterDescriptor, 128L > >((128L));

                EVSNParameterReply_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNParameterReply_g_tc_states_sequence._data._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterState>::get().native();
                EVSNParameterReply_g_tc_descriptors_sequence._data._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterDescriptor>::get().native();
                EVSNParameterReply_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterReply_g_tc_node_name_string;
                EVSNParameterReply_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNParameterOperation>::get().native();
                EVSNParameterReply_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNParameterReply_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&EVSNParameterReply_g_tc_reason_string;
                EVSNParameterReply_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)& EVSNParameterReply_g_tc_states_sequence;
                EVSNParameterReply_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)& EVSNParameterReply_g_tc_descriptors_sequence;

                /* Initialize the values for member annotations. */
                EVSNParameterReply_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterReply_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                EVSNParameterReply_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNParameterReply_g_tc_members[1]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNParameterReply_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNParameterReply_g_tc_members[2]._annotations._defaultValue._u.boolean_value = 0;
                EVSNParameterReply_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                EVSNParameterReply_g_tc_members[3]._annotations._defaultValue._u.string_value = (DDS_Char *) "";

                EVSNParameterReply_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNParameterReply_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNParameterReply_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNParameterReply *sample;

                static RTIXCdrMemberAccessInfo EVSNParameterReply_g_memberAccessInfos[6] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNParameterReply_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNParameterReply_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNParameterReply);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNParameterReply_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->node_name() - (char *)sample);

                EVSNParameterReply_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->operation() - (char *)sample);

                EVSNParameterReply_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->accepted() - (char *)sample);

                EVSNParameterReply_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->reason() - (char *)sample);

                EVSNParameterReply_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->states() - (char *)sample);

                EVSNParameterReply_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->descriptors() - (char *)sample);

                EVSNParameterReply_g_sampleAccessInfo.memberAccessInfos = 
                EVSNParameterReply_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNParameterReply);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNParameterReply_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNParameterReply_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNParameterReply_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNParameterReply_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNParameterReply >;

                EVSNParameterReply_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNParameterReply_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNParameterReply_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &EVSNParameterReply_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNParameterReply >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNParameterReply >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNParameterValue >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNParameterValuePlugin_new,
                ::EVSNParameterValuePlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNParameterValue >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNParameterValue& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNParameterValuePlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNParameterValuePlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNParameterValue >::from_cdr_buffer(::EVSNParameterValue& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNParameterValuePlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNParameterValue from cdr buffer");
        }

        void topic_type_support< ::EVSNParameterValue >::reset_sample(::EVSNParameterValue& sample) 
        {
            sample.value_type(EVSNParameterType::EVSN_PARAM_BOOL);
            sample.bool_value(0);
            sample.int_value(0ll);
            sample.float_value(0.0);
            sample.string_value("");
        }

        void topic_type_support< ::EVSNParameterValue >::allocate_sample(::EVSNParameterValue& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.value_type(),  -1, -1);
            ::rti::topic::allocate_sample(sample.string_value(),  -1, 256L);
        }
        void topic_type_support< ::EVSNParameterDescriptor >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNParameterDescriptorPlugin_new,
                ::EVSNParameterDescriptorPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNParameterDescriptor >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNParameterDescriptor& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNParameterDescriptorPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNParameterDescriptorPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNParameterDescriptor >::from_cdr_buffer(::EVSNParameterDescriptor& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNParameterDescriptorPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNParameterDescriptor from cdr buffer");
        }

        void topic_type_support< ::EVSNParameterDescriptor >::reset_sample(::EVSNParameterDescriptor& sample) 
        {
            sample.node_name("");
            sample.parameter_name("");
            sample.value_type(EVSNParameterType::EVSN_PARAM_BOOL);
            ::rti::topic::reset_sample(sample.default_value());
            sample.mutable_at_runtime(0);
            sample.has_min(0);
            sample.min_value(0.0);
            sample.has_max(0);
            sample.max_value(0.0);
            sample.unit("");
            sample.description("");
            sample.fallback_behavior("");
        }

        void topic_type_support< ::EVSNParameterDescriptor >::allocate_sample(::EVSNParameterDescriptor& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.node_name(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.parameter_name(),  -1, 128L);
            ::rti::topic::allocate_sample(sample.value_type(),  -1, -1);
            ::rti::topic::allocate_sample(sample.default_value(),  -1, -1);
            ::rti::topic::allocate_sample(sample.unit(),  -1, 32L);
            ::rti::topic::allocate_sample(sample.description(),  -1, 256L);
            ::rti::topic::allocate_sample(sample.fallback_behavior(),  -1, 256L);
        }
        void topic_type_support< ::EVSNParameterState >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNParameterStatePlugin_new,
                ::EVSNParameterStatePlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNParameterState >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNParameterState& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNParameterStatePlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNParameterStatePlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNParameterState >::from_cdr_buffer(::EVSNParameterState& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNParameterStatePlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNParameterState from cdr buffer");
        }

        void topic_type_support< ::EVSNParameterState >::reset_sample(::EVSNParameterState& sample) 
        {
            sample.node_name("");
            sample.parameter_name("");
            ::rti::topic::reset_sample(sample.value());
            sample.mutable_at_runtime(0);
            sample.unit("");
            sample.description("");
        }

        void topic_type_support< ::EVSNParameterState >::allocate_sample(::EVSNParameterState& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.node_name(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.parameter_name(),  -1, 128L);
            ::rti::topic::allocate_sample(sample.value(),  -1, -1);
            ::rti::topic::allocate_sample(sample.unit(),  -1, 32L);
            ::rti::topic::allocate_sample(sample.description(),  -1, 256L);
        }
        void topic_type_support< ::EVSNParameterEvent >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNParameterEventPlugin_new,
                ::EVSNParameterEventPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNParameterEvent >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNParameterEvent& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNParameterEventPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNParameterEventPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNParameterEvent >::from_cdr_buffer(::EVSNParameterEvent& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNParameterEventPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNParameterEvent from cdr buffer");
        }

        void topic_type_support< ::EVSNParameterEvent >::reset_sample(::EVSNParameterEvent& sample) 
        {
            sample.node_name("");
            sample.parameter_name("");
            sample.requester("");
            sample.source("");
            sample.timestamp_unix_ms(0ll);
            sample.accepted(0);
            sample.reason("");
            ::rti::topic::reset_sample(sample.requested_value());
            ::rti::topic::reset_sample(sample.resulting_value());
        }

        void topic_type_support< ::EVSNParameterEvent >::allocate_sample(::EVSNParameterEvent& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.node_name(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.parameter_name(),  -1, 128L);
            ::rti::topic::allocate_sample(sample.requester(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.source(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.reason(),  -1, 256L);
            ::rti::topic::allocate_sample(sample.requested_value(),  -1, -1);
            ::rti::topic::allocate_sample(sample.resulting_value(),  -1, -1);
        }
        void topic_type_support< ::EVSNParameterRequest >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNParameterRequestPlugin_new,
                ::EVSNParameterRequestPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNParameterRequest >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNParameterRequest& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNParameterRequestPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNParameterRequestPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNParameterRequest >::from_cdr_buffer(::EVSNParameterRequest& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNParameterRequestPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNParameterRequest from cdr buffer");
        }

        void topic_type_support< ::EVSNParameterRequest >::reset_sample(::EVSNParameterRequest& sample) 
        {
            sample.node_name("");
            sample.parameter_name("");
            sample.requester("");
            sample.source("");
            sample.operation(EVSNParameterOperation::EVSN_PARAM_GET);
            ::rti::topic::reset_sample(sample.value());
        }

        void topic_type_support< ::EVSNParameterRequest >::allocate_sample(::EVSNParameterRequest& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.node_name(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.parameter_name(),  -1, 128L);
            ::rti::topic::allocate_sample(sample.requester(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.source(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.operation(),  -1, -1);
            ::rti::topic::allocate_sample(sample.value(),  -1, -1);
        }
        void topic_type_support< ::EVSNParameterReply >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNParameterReplyPlugin_new,
                ::EVSNParameterReplyPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNParameterReply >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNParameterReply& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNParameterReplyPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNParameterReplyPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNParameterReply >::from_cdr_buffer(::EVSNParameterReply& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNParameterReplyPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNParameterReply from cdr buffer");
        }

        void topic_type_support< ::EVSNParameterReply >::reset_sample(::EVSNParameterReply& sample) 
        {
            sample.node_name("");
            sample.operation(EVSNParameterOperation::EVSN_PARAM_GET);
            sample.accepted(0);
            sample.reason("");
            ::rti::topic::reset_sample(sample.states());
            ::rti::topic::reset_sample(sample.descriptors());
        }

        void topic_type_support< ::EVSNParameterReply >::allocate_sample(::EVSNParameterReply& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.node_name(),  -1, 64L);
            ::rti::topic::allocate_sample(sample.operation(),  -1, -1);
            ::rti::topic::allocate_sample(sample.reason(),  -1, 256L);
            ::rti::topic::allocate_sample(sample.states(),  128L, -1);
            ::rti::topic::allocate_sample(sample.descriptors(),  128L, -1);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
