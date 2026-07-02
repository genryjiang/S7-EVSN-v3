

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_drive.idl
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
#include "evsn_drivePlugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_drive.hpp"

#include <rti/util/ostream_operators.hpp>

std::ostream& operator << (std::ostream& o,const EVSNDriveGear& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    switch(sample){
        case EVSNDriveGear::EVSN_DRIVE_GEAR_PARK:
        o << "EVSNDriveGear::EVSN_DRIVE_GEAR_PARK" << " ";
        break;
        case EVSNDriveGear::EVSN_DRIVE_GEAR_REVERSE:
        o << "EVSNDriveGear::EVSN_DRIVE_GEAR_REVERSE" << " ";
        break;
        case EVSNDriveGear::EVSN_DRIVE_GEAR_NEUTRAL:
        o << "EVSNDriveGear::EVSN_DRIVE_GEAR_NEUTRAL" << " ";
        break;
        case EVSNDriveGear::EVSN_DRIVE_GEAR_DRIVE:
        o << "EVSNDriveGear::EVSN_DRIVE_GEAR_DRIVE" << " ";
        break;
        default:
        {
            /* Prevents compiler warnings */
        }
    }
    return o;
}

// ---- EVSNDriveGearCommand: 

EVSNDriveGearCommand::EVSNDriveGearCommand() :
    m_valid_ (0) ,
    m_gear_(EVSNDriveGear::EVSN_DRIVE_GEAR_PARK) ,
    m_source_can_id_ (0u) ,
    m_raw_position_value_ (0) ,
    m_diagnostics_ (0u) ,
    m_sample_counter_ (0u)  {

}   

EVSNDriveGearCommand::EVSNDriveGearCommand (bool valid_,const ::EVSNDriveGear& gear_,uint32_t source_can_id_,int32_t raw_position_value_,uint32_t diagnostics_,uint32_t sample_counter_):
    m_valid_(valid_), 
    m_gear_(gear_), 
    m_source_can_id_(source_can_id_), 
    m_raw_position_value_(raw_position_value_), 
    m_diagnostics_(diagnostics_), 
    m_sample_counter_(sample_counter_) {
}

void EVSNDriveGearCommand::swap(EVSNDriveGearCommand& other_)  noexcept 
{
    using std::swap;
    swap(m_valid_, other_.m_valid_);
    swap(m_gear_, other_.m_gear_);
    swap(m_source_can_id_, other_.m_source_can_id_);
    swap(m_raw_position_value_, other_.m_raw_position_value_);
    swap(m_diagnostics_, other_.m_diagnostics_);
    swap(m_sample_counter_, other_.m_sample_counter_);
}  

bool EVSNDriveGearCommand::operator == (const EVSNDriveGearCommand& other_) const {
    if (m_valid_ != other_.m_valid_) {
        return false;
    }
    if (m_gear_ != other_.m_gear_) {
        return false;
    }
    if (m_source_can_id_ != other_.m_source_can_id_) {
        return false;
    }
    if (m_raw_position_value_ != other_.m_raw_position_value_) {
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

bool EVSNDriveGearCommand::operator != (const EVSNDriveGearCommand& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNDriveGearCommand& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "valid: " << sample.valid ()<<", ";
    o << "gear: " << sample.gear ()<<", ";
    o << "source_can_id: " << sample.source_can_id ()<<", ";
    o << "raw_position_value: " << sample.raw_position_value ()<<", ";
    o << "diagnostics: " << sample.diagnostics ()<<", ";
    o << "sample_counter: " << sample.sample_counter ();
    o <<"]";
    return o;
}

// ---- EVSNMotorControllerStatus: 

EVSNMotorControllerStatus::EVSNMotorControllerStatus() :
    m_pedal_fresh_ (0) ,
    m_gear_fresh_ (0) ,
    m_rhs_status_fresh_ (0) ,
    m_lhs_status_fresh_ (0) ,
    m_outputs_enabled_ (0) ,
    m_gear_(EVSNDriveGear::EVSN_DRIVE_GEAR_PARK) ,
    m_accelerator_scalar_ (0.0f) ,
    m_shared_limit_nm_ (0.0f) ,
    m_rhs_limit_nm_ (0.0f) ,
    m_lhs_limit_nm_ (0.0f) ,
    m_rhs_requested_torque_nm_ (0.0f) ,
    m_lhs_requested_torque_nm_ (0.0f) ,
    m_diagnostics_ (0u) ,
    m_sample_counter_ (0u)  {

}   

EVSNMotorControllerStatus::EVSNMotorControllerStatus (bool pedal_fresh_,bool gear_fresh_,bool rhs_status_fresh_,bool lhs_status_fresh_,bool outputs_enabled_,const ::EVSNDriveGear& gear_,float accelerator_scalar_,float shared_limit_nm_,float rhs_limit_nm_,float lhs_limit_nm_,float rhs_requested_torque_nm_,float lhs_requested_torque_nm_,uint32_t diagnostics_,uint32_t sample_counter_):
    m_pedal_fresh_(pedal_fresh_), 
    m_gear_fresh_(gear_fresh_), 
    m_rhs_status_fresh_(rhs_status_fresh_), 
    m_lhs_status_fresh_(lhs_status_fresh_), 
    m_outputs_enabled_(outputs_enabled_), 
    m_gear_(gear_), 
    m_accelerator_scalar_(accelerator_scalar_), 
    m_shared_limit_nm_(shared_limit_nm_), 
    m_rhs_limit_nm_(rhs_limit_nm_), 
    m_lhs_limit_nm_(lhs_limit_nm_), 
    m_rhs_requested_torque_nm_(rhs_requested_torque_nm_), 
    m_lhs_requested_torque_nm_(lhs_requested_torque_nm_), 
    m_diagnostics_(diagnostics_), 
    m_sample_counter_(sample_counter_) {
}

void EVSNMotorControllerStatus::swap(EVSNMotorControllerStatus& other_)  noexcept 
{
    using std::swap;
    swap(m_pedal_fresh_, other_.m_pedal_fresh_);
    swap(m_gear_fresh_, other_.m_gear_fresh_);
    swap(m_rhs_status_fresh_, other_.m_rhs_status_fresh_);
    swap(m_lhs_status_fresh_, other_.m_lhs_status_fresh_);
    swap(m_outputs_enabled_, other_.m_outputs_enabled_);
    swap(m_gear_, other_.m_gear_);
    swap(m_accelerator_scalar_, other_.m_accelerator_scalar_);
    swap(m_shared_limit_nm_, other_.m_shared_limit_nm_);
    swap(m_rhs_limit_nm_, other_.m_rhs_limit_nm_);
    swap(m_lhs_limit_nm_, other_.m_lhs_limit_nm_);
    swap(m_rhs_requested_torque_nm_, other_.m_rhs_requested_torque_nm_);
    swap(m_lhs_requested_torque_nm_, other_.m_lhs_requested_torque_nm_);
    swap(m_diagnostics_, other_.m_diagnostics_);
    swap(m_sample_counter_, other_.m_sample_counter_);
}  

bool EVSNMotorControllerStatus::operator == (const EVSNMotorControllerStatus& other_) const {
    if (m_pedal_fresh_ != other_.m_pedal_fresh_) {
        return false;
    }
    if (m_gear_fresh_ != other_.m_gear_fresh_) {
        return false;
    }
    if (m_rhs_status_fresh_ != other_.m_rhs_status_fresh_) {
        return false;
    }
    if (m_lhs_status_fresh_ != other_.m_lhs_status_fresh_) {
        return false;
    }
    if (m_outputs_enabled_ != other_.m_outputs_enabled_) {
        return false;
    }
    if (m_gear_ != other_.m_gear_) {
        return false;
    }
    if (std::fabs(m_accelerator_scalar_ - other_.m_accelerator_scalar_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_accelerator_scalar_ - other_.m_accelerator_scalar_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_shared_limit_nm_ - other_.m_shared_limit_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_shared_limit_nm_ - other_.m_shared_limit_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_rhs_limit_nm_ - other_.m_rhs_limit_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_rhs_limit_nm_ - other_.m_rhs_limit_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_lhs_limit_nm_ - other_.m_lhs_limit_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_lhs_limit_nm_ - other_.m_lhs_limit_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_rhs_requested_torque_nm_ - other_.m_rhs_requested_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_rhs_requested_torque_nm_ - other_.m_rhs_requested_torque_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_lhs_requested_torque_nm_ - other_.m_lhs_requested_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_lhs_requested_torque_nm_ - other_.m_lhs_requested_torque_nm_) < (std::numeric_limits< float>::min)())) {
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

bool EVSNMotorControllerStatus::operator != (const EVSNMotorControllerStatus& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMotorControllerStatus& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "pedal_fresh: " << sample.pedal_fresh ()<<", ";
    o << "gear_fresh: " << sample.gear_fresh ()<<", ";
    o << "rhs_status_fresh: " << sample.rhs_status_fresh ()<<", ";
    o << "lhs_status_fresh: " << sample.lhs_status_fresh ()<<", ";
    o << "outputs_enabled: " << sample.outputs_enabled ()<<", ";
    o << "gear: " << sample.gear ()<<", ";
    o << "accelerator_scalar: " << std::setprecision(9) << sample.accelerator_scalar ()<<", ";
    o << "shared_limit_nm: " << std::setprecision(9) << sample.shared_limit_nm ()<<", ";
    o << "rhs_limit_nm: " << std::setprecision(9) << sample.rhs_limit_nm ()<<", ";
    o << "lhs_limit_nm: " << std::setprecision(9) << sample.lhs_limit_nm ()<<", ";
    o << "rhs_requested_torque_nm: " << std::setprecision(9) << sample.rhs_requested_torque_nm ()<<", ";
    o << "lhs_requested_torque_nm: " << std::setprecision(9) << sample.lhs_requested_torque_nm ()<<", ";
    o << "diagnostics: " << sample.diagnostics ()<<", ";
    o << "sample_counter: " << sample.sample_counter ();
    o <<"]";
    return o;
}

#ifdef NDDS_STANDALONE_TYPE
namespace rti {
    namespace topic {
        const ::EVSNDriveGear default_enumerator< ::EVSNDriveGear>::value = ::EVSNDriveGear::EVSN_DRIVE_GEAR_PARK;
    }
}

#else
// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        const ::EVSNDriveGear default_enumerator< ::EVSNDriveGear>::value = ::EVSNDriveGear::EVSN_DRIVE_GEAR_PARK;
        template<>
        struct native_type_code< ::EVSNDriveGear > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNDriveGear_g_tc_members[4]=
                {

                    {
                        (char *)"EVSN_DRIVE_GEAR_PARK",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNDriveGear::EVSN_DRIVE_GEAR_PARK), 
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
                        (char *)"EVSN_DRIVE_GEAR_REVERSE",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNDriveGear::EVSN_DRIVE_GEAR_REVERSE), 
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
                        (char *)"EVSN_DRIVE_GEAR_NEUTRAL",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNDriveGear::EVSN_DRIVE_GEAR_NEUTRAL), 
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
                        (char *)"EVSN_DRIVE_GEAR_DRIVE",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNDriveGear::EVSN_DRIVE_GEAR_DRIVE), 
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

                static DDS_TypeCode EVSNDriveGear_g_tc =
                {{
                        DDS_TK_ENUM, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNDriveGear", /* Name */
                        NULL,     /* Base class type code is assigned later */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        4, /* Number of members */
                        EVSNDriveGear_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Type Modifier */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNDriveGear*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNDriveGear_g_tc;
                }

                EVSNDriveGear_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                /* Initialize the values for annotations. */
                EVSNDriveGear_g_tc._data._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNDriveGear_g_tc._data._annotations._defaultValue._u.long_value = 0;

                EVSNDriveGear_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNDriveGear_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNDriveGear_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                static RTIXCdrMemberAccessInfo EVSNDriveGear_g_memberAccessInfos[1] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNDriveGear_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNDriveGear_g_sampleAccessInfo;
                }

                EVSNDriveGear_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 0;

                EVSNDriveGear_g_sampleAccessInfo.memberAccessInfos = 
                EVSNDriveGear_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNDriveGear);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNDriveGear_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNDriveGear_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNDriveGear_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNDriveGear_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNDriveGear >;

                EVSNDriveGear_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNDriveGear_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNDriveGear_g_typePlugin = 
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

                return &EVSNDriveGear_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::EnumType& dynamic_type< ::EVSNDriveGear >::get()
        {
            return static_cast<const ::dds::core::xtypes::EnumType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNDriveGear >::get())));
        }

        template<>
        struct native_type_code< ::EVSNDriveGearCommand > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNDriveGearCommand_g_tc_members[6]=
                {

                    {
                        (char *)"valid",/* Member name */
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
                        (char *)"gear",/* Member name */
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
                        (char *)"source_can_id",/* Member name */
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
                        (char *)"raw_position_value",/* Member name */
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
                        (char *)"diagnostics",/* Member name */
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
                        (char *)"sample_counter",/* Member name */
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

                static DDS_TypeCode EVSNDriveGearCommand_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNDriveGearCommand", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        6, /* Number of members */
                        EVSNDriveGearCommand_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNDriveGearCommand*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNDriveGearCommand_g_tc;
                }

                EVSNDriveGearCommand_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNDriveGearCommand_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNDriveGearCommand_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNDriveGear>::get().native();
                EVSNDriveGearCommand_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNDriveGearCommand_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;
                EVSNDriveGearCommand_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNDriveGearCommand_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;

                /* Initialize the values for member annotations. */
                EVSNDriveGearCommand_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNDriveGearCommand_g_tc_members[0]._annotations._defaultValue._u.boolean_value = 0;
                EVSNDriveGearCommand_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNDriveGearCommand_g_tc_members[1]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNDriveGearCommand_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[2]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNDriveGearCommand_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[2]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNDriveGearCommand_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[2]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNDriveGearCommand_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
                EVSNDriveGearCommand_g_tc_members[3]._annotations._defaultValue._u.long_value = 0;
                EVSNDriveGearCommand_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_LONG;
                EVSNDriveGearCommand_g_tc_members[3]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
                EVSNDriveGearCommand_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
                EVSNDriveGearCommand_g_tc_members[3]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;
                EVSNDriveGearCommand_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[4]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNDriveGearCommand_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[4]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNDriveGearCommand_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[4]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNDriveGearCommand_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[5]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNDriveGearCommand_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[5]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNDriveGearCommand_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNDriveGearCommand_g_tc_members[5]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;

                EVSNDriveGearCommand_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNDriveGearCommand_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNDriveGearCommand_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNDriveGearCommand *sample;

                static RTIXCdrMemberAccessInfo EVSNDriveGearCommand_g_memberAccessInfos[6] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNDriveGearCommand_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNDriveGearCommand_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNDriveGearCommand);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNDriveGearCommand_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->valid() - (char *)sample);

                EVSNDriveGearCommand_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->gear() - (char *)sample);

                EVSNDriveGearCommand_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->source_can_id() - (char *)sample);

                EVSNDriveGearCommand_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->raw_position_value() - (char *)sample);

                EVSNDriveGearCommand_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->diagnostics() - (char *)sample);

                EVSNDriveGearCommand_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->sample_counter() - (char *)sample);

                EVSNDriveGearCommand_g_sampleAccessInfo.memberAccessInfos = 
                EVSNDriveGearCommand_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNDriveGearCommand);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNDriveGearCommand_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNDriveGearCommand_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNDriveGearCommand_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNDriveGearCommand_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNDriveGearCommand >;

                EVSNDriveGearCommand_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNDriveGearCommand_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNDriveGearCommand_g_typePlugin = 
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

                return &EVSNDriveGearCommand_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNDriveGearCommand >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNDriveGearCommand >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMotorControllerStatus > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMotorControllerStatus_g_tc_members[14]=
                {

                    {
                        (char *)"pedal_fresh",/* Member name */
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
                        (char *)"gear_fresh",/* Member name */
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
                        (char *)"rhs_status_fresh",/* Member name */
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
                        (char *)"lhs_status_fresh",/* Member name */
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
                        (char *)"outputs_enabled",/* Member name */
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
                        (char *)"gear",/* Member name */
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
                        (char *)"shared_limit_nm",/* Member name */
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
                        (char *)"rhs_limit_nm",/* Member name */
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
                        (char *)"lhs_limit_nm",/* Member name */
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
                        (char *)"rhs_requested_torque_nm",/* Member name */
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
                        (char *)"lhs_requested_torque_nm",/* Member name */
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
                        (char *)"diagnostics",/* Member name */
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
                        (char *)"sample_counter",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMotorControllerStatus_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMotorControllerStatus", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        14, /* Number of members */
                        EVSNMotorControllerStatus_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMotorControllerStatus*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMotorControllerStatus_g_tc;
                }

                EVSNMotorControllerStatus_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMotorControllerStatus_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorControllerStatus_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorControllerStatus_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorControllerStatus_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorControllerStatus_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorControllerStatus_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNDriveGear>::get().native();
                EVSNMotorControllerStatus_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorControllerStatus_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorControllerStatus_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorControllerStatus_g_tc_members[9]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorControllerStatus_g_tc_members[10]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorControllerStatus_g_tc_members[11]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorControllerStatus_g_tc_members[12]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNMotorControllerStatus_g_tc_members[13]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;

                /* Initialize the values for member annotations. */
                EVSNMotorControllerStatus_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorControllerStatus_g_tc_members[0]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorControllerStatus_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorControllerStatus_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorControllerStatus_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorControllerStatus_g_tc_members[2]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorControllerStatus_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorControllerStatus_g_tc_members[3]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorControllerStatus_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorControllerStatus_g_tc_members[4]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorControllerStatus_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNMotorControllerStatus_g_tc_members[5]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNMotorControllerStatus_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[6]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorControllerStatus_g_tc_members[6]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[6]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorControllerStatus_g_tc_members[6]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[6]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorControllerStatus_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[7]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorControllerStatus_g_tc_members[7]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[7]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorControllerStatus_g_tc_members[7]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[7]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorControllerStatus_g_tc_members[8]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[8]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorControllerStatus_g_tc_members[8]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[8]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorControllerStatus_g_tc_members[8]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[8]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorControllerStatus_g_tc_members[9]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[9]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorControllerStatus_g_tc_members[9]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[9]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorControllerStatus_g_tc_members[9]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[9]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorControllerStatus_g_tc_members[10]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[10]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorControllerStatus_g_tc_members[10]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[10]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorControllerStatus_g_tc_members[10]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[10]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorControllerStatus_g_tc_members[11]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[11]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorControllerStatus_g_tc_members[11]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[11]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorControllerStatus_g_tc_members[11]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorControllerStatus_g_tc_members[11]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorControllerStatus_g_tc_members[12]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNMotorControllerStatus_g_tc_members[12]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNMotorControllerStatus_g_tc_members[12]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNMotorControllerStatus_g_tc_members[12]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNMotorControllerStatus_g_tc_members[12]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNMotorControllerStatus_g_tc_members[12]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNMotorControllerStatus_g_tc_members[13]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNMotorControllerStatus_g_tc_members[13]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNMotorControllerStatus_g_tc_members[13]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNMotorControllerStatus_g_tc_members[13]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNMotorControllerStatus_g_tc_members[13]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNMotorControllerStatus_g_tc_members[13]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;

                EVSNMotorControllerStatus_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMotorControllerStatus_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMotorControllerStatus_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMotorControllerStatus *sample;

                static RTIXCdrMemberAccessInfo EVSNMotorControllerStatus_g_memberAccessInfos[14] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMotorControllerStatus_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMotorControllerStatus_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMotorControllerStatus);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMotorControllerStatus_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->pedal_fresh() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->gear_fresh() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->rhs_status_fresh() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->lhs_status_fresh() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->outputs_enabled() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->gear() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->accelerator_scalar() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->shared_limit_nm() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->rhs_limit_nm() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[9].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->lhs_limit_nm() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[10].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->rhs_requested_torque_nm() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[11].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->lhs_requested_torque_nm() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[12].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->diagnostics() - (char *)sample);

                EVSNMotorControllerStatus_g_memberAccessInfos[13].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->sample_counter() - (char *)sample);

                EVSNMotorControllerStatus_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMotorControllerStatus_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMotorControllerStatus);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMotorControllerStatus_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMotorControllerStatus_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMotorControllerStatus_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMotorControllerStatus_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMotorControllerStatus >;

                EVSNMotorControllerStatus_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMotorControllerStatus_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMotorControllerStatus_g_typePlugin = 
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

                return &EVSNMotorControllerStatus_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMotorControllerStatus >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMotorControllerStatus >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNDriveGearCommand >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNDriveGearCommandPlugin_new,
                ::EVSNDriveGearCommandPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNDriveGearCommand >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNDriveGearCommand& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNDriveGearCommandPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNDriveGearCommandPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNDriveGearCommand >::from_cdr_buffer(::EVSNDriveGearCommand& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNDriveGearCommandPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNDriveGearCommand from cdr buffer");
        }

        void topic_type_support< ::EVSNDriveGearCommand >::reset_sample(::EVSNDriveGearCommand& sample) 
        {
            sample.valid(0);
            sample.gear(EVSNDriveGear::EVSN_DRIVE_GEAR_PARK);
            sample.source_can_id(0u);
            sample.raw_position_value(0);
            sample.diagnostics(0u);
            sample.sample_counter(0u);
        }

        void topic_type_support< ::EVSNDriveGearCommand >::allocate_sample(::EVSNDriveGearCommand& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.gear(),  -1, -1);
        }
        void topic_type_support< ::EVSNMotorControllerStatus >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMotorControllerStatusPlugin_new,
                ::EVSNMotorControllerStatusPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMotorControllerStatus >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMotorControllerStatus& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMotorControllerStatusPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMotorControllerStatusPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMotorControllerStatus >::from_cdr_buffer(::EVSNMotorControllerStatus& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMotorControllerStatusPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMotorControllerStatus from cdr buffer");
        }

        void topic_type_support< ::EVSNMotorControllerStatus >::reset_sample(::EVSNMotorControllerStatus& sample) 
        {
            sample.pedal_fresh(0);
            sample.gear_fresh(0);
            sample.rhs_status_fresh(0);
            sample.lhs_status_fresh(0);
            sample.outputs_enabled(0);
            sample.gear(EVSNDriveGear::EVSN_DRIVE_GEAR_PARK);
            sample.accelerator_scalar(0.0f);
            sample.shared_limit_nm(0.0f);
            sample.rhs_limit_nm(0.0f);
            sample.lhs_limit_nm(0.0f);
            sample.rhs_requested_torque_nm(0.0f);
            sample.lhs_requested_torque_nm(0.0f);
            sample.diagnostics(0u);
            sample.sample_counter(0u);
        }

        void topic_type_support< ::EVSNMotorControllerStatus >::allocate_sample(::EVSNMotorControllerStatus& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.gear(),  -1, -1);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
