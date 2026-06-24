

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_pedal.idl
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
#include "evsn_pedalPlugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_pedal.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- EVSNPedalState: 

EVSNPedalState::EVSNPedalState() :
    m_accelerator_valid_ (0) ,
    m_brake_status_valid_ (0) ,
    m_input_fresh_ (0) ,
    m_input_received_at_ms_ (0ull) ,
    m_input_age_ms_ (0u) ,
    m_accelerator_percent_ (0.0f) ,
    m_accelerator_scalar_ (0.0f) ,
    m_throttle_channel_1_v_ (0.0f) ,
    m_throttle_channel_2_v_ (0.0f) ,
    m_throttle_channel_1_percent_ (0.0f) ,
    m_throttle_channel_2_percent_ (0.0f) ,
    m_brake_light_request_ (0) ,
    m_source_can_id_ (0u) ,
    m_diagnostics_ (0u) ,
    m_sample_counter_ (0u)  {

}   

EVSNPedalState::EVSNPedalState (bool accelerator_valid_,bool brake_status_valid_,bool input_fresh_,uint64_t input_received_at_ms_,uint32_t input_age_ms_,float accelerator_percent_,float accelerator_scalar_,float throttle_channel_1_v_,float throttle_channel_2_v_,float throttle_channel_1_percent_,float throttle_channel_2_percent_,bool brake_light_request_,uint32_t source_can_id_,uint32_t diagnostics_,uint32_t sample_counter_):
    m_accelerator_valid_(accelerator_valid_), 
    m_brake_status_valid_(brake_status_valid_), 
    m_input_fresh_(input_fresh_), 
    m_input_received_at_ms_(input_received_at_ms_), 
    m_input_age_ms_(input_age_ms_), 
    m_accelerator_percent_(accelerator_percent_), 
    m_accelerator_scalar_(accelerator_scalar_), 
    m_throttle_channel_1_v_(throttle_channel_1_v_), 
    m_throttle_channel_2_v_(throttle_channel_2_v_), 
    m_throttle_channel_1_percent_(throttle_channel_1_percent_), 
    m_throttle_channel_2_percent_(throttle_channel_2_percent_), 
    m_brake_light_request_(brake_light_request_), 
    m_source_can_id_(source_can_id_), 
    m_diagnostics_(diagnostics_), 
    m_sample_counter_(sample_counter_) {
}

void EVSNPedalState::swap(EVSNPedalState& other_)  noexcept 
{
    using std::swap;
    swap(m_accelerator_valid_, other_.m_accelerator_valid_);
    swap(m_brake_status_valid_, other_.m_brake_status_valid_);
    swap(m_input_fresh_, other_.m_input_fresh_);
    swap(m_input_received_at_ms_, other_.m_input_received_at_ms_);
    swap(m_input_age_ms_, other_.m_input_age_ms_);
    swap(m_accelerator_percent_, other_.m_accelerator_percent_);
    swap(m_accelerator_scalar_, other_.m_accelerator_scalar_);
    swap(m_throttle_channel_1_v_, other_.m_throttle_channel_1_v_);
    swap(m_throttle_channel_2_v_, other_.m_throttle_channel_2_v_);
    swap(m_throttle_channel_1_percent_, other_.m_throttle_channel_1_percent_);
    swap(m_throttle_channel_2_percent_, other_.m_throttle_channel_2_percent_);
    swap(m_brake_light_request_, other_.m_brake_light_request_);
    swap(m_source_can_id_, other_.m_source_can_id_);
    swap(m_diagnostics_, other_.m_diagnostics_);
    swap(m_sample_counter_, other_.m_sample_counter_);
}  

bool EVSNPedalState::operator == (const EVSNPedalState& other_) const {
    if (m_accelerator_valid_ != other_.m_accelerator_valid_) {
        return false;
    }
    if (m_brake_status_valid_ != other_.m_brake_status_valid_) {
        return false;
    }
    if (m_input_fresh_ != other_.m_input_fresh_) {
        return false;
    }
    if (m_input_received_at_ms_ != other_.m_input_received_at_ms_) {
        return false;
    }
    if (m_input_age_ms_ != other_.m_input_age_ms_) {
        return false;
    }
    if (std::fabs(m_accelerator_percent_ - other_.m_accelerator_percent_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_accelerator_percent_ - other_.m_accelerator_percent_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_accelerator_scalar_ - other_.m_accelerator_scalar_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_accelerator_scalar_ - other_.m_accelerator_scalar_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_throttle_channel_1_v_ - other_.m_throttle_channel_1_v_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_throttle_channel_1_v_ - other_.m_throttle_channel_1_v_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_throttle_channel_2_v_ - other_.m_throttle_channel_2_v_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_throttle_channel_2_v_ - other_.m_throttle_channel_2_v_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_throttle_channel_1_percent_ - other_.m_throttle_channel_1_percent_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_throttle_channel_1_percent_ - other_.m_throttle_channel_1_percent_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_throttle_channel_2_percent_ - other_.m_throttle_channel_2_percent_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_throttle_channel_2_percent_ - other_.m_throttle_channel_2_percent_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (m_brake_light_request_ != other_.m_brake_light_request_) {
        return false;
    }
    if (m_source_can_id_ != other_.m_source_can_id_) {
        return false;
    }
    if (m_diagnostics_ != other_.m_diagnostics_) {
        return false;
    }
    if (m_sample_counter_ != other_.m_sample_counter_) {
        return false;
    }
    return true;
}

bool EVSNPedalState::operator != (const EVSNPedalState& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNPedalState& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "accelerator_valid: " << sample.accelerator_valid ()<<", ";
    o << "brake_status_valid: " << sample.brake_status_valid ()<<", ";
    o << "input_fresh: " << sample.input_fresh ()<<", ";
    o << "input_received_at_ms: " << sample.input_received_at_ms ()<<", ";
    o << "input_age_ms: " << sample.input_age_ms ()<<", ";
    o << "accelerator_percent: " << std::setprecision(9) << sample.accelerator_percent ()<<", ";
    o << "accelerator_scalar: " << std::setprecision(9) << sample.accelerator_scalar ()<<", ";
    o << "throttle_channel_1_v: " << std::setprecision(9) << sample.throttle_channel_1_v ()<<", ";
    o << "throttle_channel_2_v: " << std::setprecision(9) << sample.throttle_channel_2_v ()<<", ";
    o << "throttle_channel_1_percent: " << std::setprecision(9) << sample.throttle_channel_1_percent ()<<", ";
    o << "throttle_channel_2_percent: " << std::setprecision(9) << sample.throttle_channel_2_percent ()<<", ";
    o << "brake_light_request: " << sample.brake_light_request ()<<", ";
    o << "source_can_id: " << sample.source_can_id ()<<", ";
    o << "diagnostics: " << sample.diagnostics ()<<", ";
    o << "sample_counter: " << sample.sample_counter ();
    o <<"]";
    return o;
}

#ifdef NDDS_STANDALONE_TYPE
namespace rti {
    namespace topic {
    }
}

#else
// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        template<>
        struct native_type_code< ::EVSNPedalState > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNPedalState_g_tc_members[15]=
                {

                    {
                        (char *)"accelerator_valid",/* Member name */
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
                        (char *)"brake_status_valid",/* Member name */
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
                        (char *)"input_fresh",/* Member name */
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
                        (char *)"input_received_at_ms",/* Member name */
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
                        (char *)"input_age_ms",/* Member name */
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
                        (char *)"accelerator_percent",/* Member name */
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
                        (char *)"accelerator_scalar",/* Member name */
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
                        (char *)"throttle_channel_1_v",/* Member name */
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
                        (char *)"throttle_channel_2_v",/* Member name */
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
                        (char *)"throttle_channel_1_percent",/* Member name */
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
                        (char *)"throttle_channel_2_percent",/* Member name */
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
                        (char *)"brake_light_request",/* Member name */
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
                    }, 
                    {
                        (char *)"source_can_id",/* Member name */
                        {
                            12,/* Representation ID */
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
                        (char *)"diagnostics",/* Member name */
                        {
                            13,/* Representation ID */
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
                        (char *)"sample_counter",/* Member name */
                        {
                            14,/* Representation ID */
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

                static DDS_TypeCode EVSNPedalState_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNPedalState", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        15, /* Number of members */
                        EVSNPedalState_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNPedalState*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNPedalState_g_tc;
                }

                EVSNPedalState_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNPedalState_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNPedalState_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNPedalState_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNPedalState_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulonglong;
                EVSNPedalState_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNPedalState_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNPedalState_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNPedalState_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNPedalState_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNPedalState_g_tc_members[9]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNPedalState_g_tc_members[10]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNPedalState_g_tc_members[11]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNPedalState_g_tc_members[12]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNPedalState_g_tc_members[13]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNPedalState_g_tc_members[14]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;

                /* Initialize the values for member annotations. */
                EVSNPedalState_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNPedalState_g_tc_members[0]._annotations._defaultValue._u.boolean_value = 0;
                EVSNPedalState_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNPedalState_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNPedalState_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNPedalState_g_tc_members[2]._annotations._defaultValue._u.boolean_value = 0;
                EVSNPedalState_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_ULONGLONG;
                EVSNPedalState_g_tc_members[3]._annotations._defaultValue._u.ulong_long_value = 0ull;
                EVSNPedalState_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_ULONGLONG;
                EVSNPedalState_g_tc_members[3]._annotations._minValue._u.ulong_long_value = RTIXCdrUnsignedLongLong_MIN;
                EVSNPedalState_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_ULONGLONG;
                EVSNPedalState_g_tc_members[3]._annotations._maxValue._u.ulong_long_value = RTIXCdrUnsignedLongLong_MAX;
                EVSNPedalState_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[4]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNPedalState_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[4]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNPedalState_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[4]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNPedalState_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[5]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNPedalState_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[5]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNPedalState_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[5]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNPedalState_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[6]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNPedalState_g_tc_members[6]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[6]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNPedalState_g_tc_members[6]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[6]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNPedalState_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[7]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNPedalState_g_tc_members[7]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[7]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNPedalState_g_tc_members[7]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[7]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNPedalState_g_tc_members[8]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[8]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNPedalState_g_tc_members[8]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[8]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNPedalState_g_tc_members[8]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[8]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNPedalState_g_tc_members[9]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[9]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNPedalState_g_tc_members[9]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[9]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNPedalState_g_tc_members[9]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[9]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNPedalState_g_tc_members[10]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[10]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNPedalState_g_tc_members[10]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[10]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNPedalState_g_tc_members[10]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNPedalState_g_tc_members[10]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNPedalState_g_tc_members[11]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNPedalState_g_tc_members[11]._annotations._defaultValue._u.boolean_value = 0;
                EVSNPedalState_g_tc_members[12]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[12]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNPedalState_g_tc_members[12]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[12]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNPedalState_g_tc_members[12]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[12]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNPedalState_g_tc_members[13]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[13]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNPedalState_g_tc_members[13]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[13]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNPedalState_g_tc_members[13]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[13]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNPedalState_g_tc_members[14]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[14]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNPedalState_g_tc_members[14]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[14]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNPedalState_g_tc_members[14]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNPedalState_g_tc_members[14]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;

                EVSNPedalState_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNPedalState_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNPedalState_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNPedalState *sample;

                static RTIXCdrMemberAccessInfo EVSNPedalState_g_memberAccessInfos[15] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNPedalState_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNPedalState_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNPedalState);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNPedalState_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->accelerator_valid() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->brake_status_valid() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->input_fresh() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->input_received_at_ms() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->input_age_ms() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->accelerator_percent() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->accelerator_scalar() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->throttle_channel_1_v() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->throttle_channel_2_v() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[9].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->throttle_channel_1_percent() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[10].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->throttle_channel_2_percent() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[11].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->brake_light_request() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[12].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->source_can_id() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[13].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->diagnostics() - (char *)sample);

                EVSNPedalState_g_memberAccessInfos[14].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->sample_counter() - (char *)sample);

                EVSNPedalState_g_sampleAccessInfo.memberAccessInfos = 
                EVSNPedalState_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNPedalState);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNPedalState_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNPedalState_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNPedalState_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNPedalState_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNPedalState >;

                EVSNPedalState_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNPedalState_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNPedalState_g_typePlugin = 
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

                return &EVSNPedalState_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNPedalState >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNPedalState >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNPedalState >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNPedalStatePlugin_new,
                ::EVSNPedalStatePlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNPedalState >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNPedalState& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNPedalStatePlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNPedalStatePlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNPedalState >::from_cdr_buffer(::EVSNPedalState& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNPedalStatePlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNPedalState from cdr buffer");
        }

        void topic_type_support< ::EVSNPedalState >::reset_sample(::EVSNPedalState& sample) 
        {
            sample.accelerator_valid(0);
            sample.brake_status_valid(0);
            sample.input_fresh(0);
            sample.input_received_at_ms(0ull);
            sample.input_age_ms(0u);
            sample.accelerator_percent(0.0f);
            sample.accelerator_scalar(0.0f);
            sample.throttle_channel_1_v(0.0f);
            sample.throttle_channel_2_v(0.0f);
            sample.throttle_channel_1_percent(0.0f);
            sample.throttle_channel_2_percent(0.0f);
            sample.brake_light_request(0);
            sample.source_can_id(0u);
            sample.diagnostics(0u);
            sample.sample_counter(0u);
        }

        void topic_type_support< ::EVSNPedalState >::allocate_sample(::EVSNPedalState& sample, int, int) 
        {
            RTIOsapiUtility_unusedParameter(sample);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
