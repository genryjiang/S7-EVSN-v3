

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_drive.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_drive_408364356_hpp
#define evsn_drive_408364356_hpp

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

enum class EVSNDriveGear {
    EVSN_DRIVE_GEAR_PARK, 
    EVSN_DRIVE_GEAR_REVERSE, 
    EVSN_DRIVE_GEAR_NEUTRAL, 
    EVSN_DRIVE_GEAR_DRIVE
};

NDDSUSERDllExport std::ostream& operator << (std::ostream& o,const EVSNDriveGear& sample);

class NDDSUSERDllExport EVSNDriveGearCommand {
  public:

    EVSNDriveGearCommand();

    EVSNDriveGearCommand(bool valid_,const ::EVSNDriveGear& gear_,uint32_t source_can_id_,int32_t raw_position_value_,uint32_t diagnostics_,uint32_t sample_counter_);

    bool& valid() noexcept {
        return m_valid_;
    }

    const bool& valid() const noexcept {
        return m_valid_;
    }

    void valid(bool value) {

        m_valid_ = value;
    }

    ::EVSNDriveGear& gear() noexcept {
        return m_gear_;
    }

    const ::EVSNDriveGear& gear() const noexcept {
        return m_gear_;
    }

    void gear(const ::EVSNDriveGear& value) {

        m_gear_ = value;
    }

    void gear(::EVSNDriveGear&& value) {
        m_gear_ = std::move(value);
    }
    uint32_t& source_can_id() noexcept {
        return m_source_can_id_;
    }

    const uint32_t& source_can_id() const noexcept {
        return m_source_can_id_;
    }

    void source_can_id(uint32_t value) {

        m_source_can_id_ = value;
    }

    int32_t& raw_position_value() noexcept {
        return m_raw_position_value_;
    }

    const int32_t& raw_position_value() const noexcept {
        return m_raw_position_value_;
    }

    void raw_position_value(int32_t value) {

        m_raw_position_value_ = value;
    }

    uint32_t& diagnostics() noexcept {
        return m_diagnostics_;
    }

    const uint32_t& diagnostics() const noexcept {
        return m_diagnostics_;
    }

    void diagnostics(uint32_t value) {

        m_diagnostics_ = value;
    }

    uint32_t& sample_counter() noexcept {
        return m_sample_counter_;
    }

    const uint32_t& sample_counter() const noexcept {
        return m_sample_counter_;
    }

    void sample_counter(uint32_t value) {

        m_sample_counter_ = value;
    }

    bool operator == (const EVSNDriveGearCommand& other_) const;
    bool operator != (const EVSNDriveGearCommand& other_) const;

    void swap(EVSNDriveGearCommand& other_) noexcept ;

  private:

    bool m_valid_;
    ::EVSNDriveGear m_gear_;
    uint32_t m_source_can_id_;
    int32_t m_raw_position_value_;
    uint32_t m_diagnostics_;
    uint32_t m_sample_counter_;

};

inline void swap(EVSNDriveGearCommand& a, EVSNDriveGearCommand& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNDriveGearCommand& sample);

class NDDSUSERDllExport EVSNMotorControllerStatus {
  public:

    EVSNMotorControllerStatus();

    EVSNMotorControllerStatus(bool pedal_fresh_,bool gear_fresh_,bool rhs_status_fresh_,bool lhs_status_fresh_,bool outputs_enabled_,const ::EVSNDriveGear& gear_,float accelerator_scalar_,float shared_limit_nm_,float rhs_limit_nm_,float lhs_limit_nm_,float rhs_requested_torque_nm_,float lhs_requested_torque_nm_,uint32_t diagnostics_,uint32_t sample_counter_);

    bool& pedal_fresh() noexcept {
        return m_pedal_fresh_;
    }

    const bool& pedal_fresh() const noexcept {
        return m_pedal_fresh_;
    }

    void pedal_fresh(bool value) {

        m_pedal_fresh_ = value;
    }

    bool& gear_fresh() noexcept {
        return m_gear_fresh_;
    }

    const bool& gear_fresh() const noexcept {
        return m_gear_fresh_;
    }

    void gear_fresh(bool value) {

        m_gear_fresh_ = value;
    }

    bool& rhs_status_fresh() noexcept {
        return m_rhs_status_fresh_;
    }

    const bool& rhs_status_fresh() const noexcept {
        return m_rhs_status_fresh_;
    }

    void rhs_status_fresh(bool value) {

        m_rhs_status_fresh_ = value;
    }

    bool& lhs_status_fresh() noexcept {
        return m_lhs_status_fresh_;
    }

    const bool& lhs_status_fresh() const noexcept {
        return m_lhs_status_fresh_;
    }

    void lhs_status_fresh(bool value) {

        m_lhs_status_fresh_ = value;
    }

    bool& outputs_enabled() noexcept {
        return m_outputs_enabled_;
    }

    const bool& outputs_enabled() const noexcept {
        return m_outputs_enabled_;
    }

    void outputs_enabled(bool value) {

        m_outputs_enabled_ = value;
    }

    ::EVSNDriveGear& gear() noexcept {
        return m_gear_;
    }

    const ::EVSNDriveGear& gear() const noexcept {
        return m_gear_;
    }

    void gear(const ::EVSNDriveGear& value) {

        m_gear_ = value;
    }

    void gear(::EVSNDriveGear&& value) {
        m_gear_ = std::move(value);
    }
    float& accelerator_scalar() noexcept {
        return m_accelerator_scalar_;
    }

    const float& accelerator_scalar() const noexcept {
        return m_accelerator_scalar_;
    }

    void accelerator_scalar(float value) {

        m_accelerator_scalar_ = value;
    }

    float& shared_limit_nm() noexcept {
        return m_shared_limit_nm_;
    }

    const float& shared_limit_nm() const noexcept {
        return m_shared_limit_nm_;
    }

    void shared_limit_nm(float value) {

        m_shared_limit_nm_ = value;
    }

    float& rhs_limit_nm() noexcept {
        return m_rhs_limit_nm_;
    }

    const float& rhs_limit_nm() const noexcept {
        return m_rhs_limit_nm_;
    }

    void rhs_limit_nm(float value) {

        m_rhs_limit_nm_ = value;
    }

    float& lhs_limit_nm() noexcept {
        return m_lhs_limit_nm_;
    }

    const float& lhs_limit_nm() const noexcept {
        return m_lhs_limit_nm_;
    }

    void lhs_limit_nm(float value) {

        m_lhs_limit_nm_ = value;
    }

    float& rhs_requested_torque_nm() noexcept {
        return m_rhs_requested_torque_nm_;
    }

    const float& rhs_requested_torque_nm() const noexcept {
        return m_rhs_requested_torque_nm_;
    }

    void rhs_requested_torque_nm(float value) {

        m_rhs_requested_torque_nm_ = value;
    }

    float& lhs_requested_torque_nm() noexcept {
        return m_lhs_requested_torque_nm_;
    }

    const float& lhs_requested_torque_nm() const noexcept {
        return m_lhs_requested_torque_nm_;
    }

    void lhs_requested_torque_nm(float value) {

        m_lhs_requested_torque_nm_ = value;
    }

    uint32_t& diagnostics() noexcept {
        return m_diagnostics_;
    }

    const uint32_t& diagnostics() const noexcept {
        return m_diagnostics_;
    }

    void diagnostics(uint32_t value) {

        m_diagnostics_ = value;
    }

    uint32_t& sample_counter() noexcept {
        return m_sample_counter_;
    }

    const uint32_t& sample_counter() const noexcept {
        return m_sample_counter_;
    }

    void sample_counter(uint32_t value) {

        m_sample_counter_ = value;
    }

    bool operator == (const EVSNMotorControllerStatus& other_) const;
    bool operator != (const EVSNMotorControllerStatus& other_) const;

    void swap(EVSNMotorControllerStatus& other_) noexcept ;

  private:

    bool m_pedal_fresh_;
    bool m_gear_fresh_;
    bool m_rhs_status_fresh_;
    bool m_lhs_status_fresh_;
    bool m_outputs_enabled_;
    ::EVSNDriveGear m_gear_;
    float m_accelerator_scalar_;
    float m_shared_limit_nm_;
    float m_rhs_limit_nm_;
    float m_lhs_limit_nm_;
    float m_rhs_requested_torque_nm_;
    float m_lhs_requested_torque_nm_;
    uint32_t m_diagnostics_;
    uint32_t m_sample_counter_;

};

inline void swap(EVSNMotorControllerStatus& a, EVSNMotorControllerStatus& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMotorControllerStatus& sample);

#ifdef NDDS_STANDALONE_TYPE
namespace rti { 
    namespace topic {
        template <>
        struct default_enumerator< ::EVSNDriveGear>
        {
            static const ::EVSNDriveGear value;
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
        struct topic_type_name< ::EVSNDriveGearCommand > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNDriveGearCommand";
            }
        };

        template<>
        struct is_topic_type< ::EVSNDriveGearCommand > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNDriveGearCommand > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNDriveGearCommand& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNDriveGearCommand& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNDriveGearCommand& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNDriveGearCommand& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMotorControllerStatus > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMotorControllerStatus";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMotorControllerStatus > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMotorControllerStatus > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMotorControllerStatus& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMotorControllerStatus& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMotorControllerStatus& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMotorControllerStatus& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
    }
}

namespace rti { 
    namespace topic {

        template <>
        struct default_enumerator< ::EVSNDriveGear>
        {
            static const ::EVSNDriveGear value;
        };
        template<>
        struct dynamic_type< ::EVSNDriveGear > {
            typedef ::dds::core::xtypes::EnumType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::EnumType& get();
        };

        template <>
        struct extensibility< ::EVSNDriveGear > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNDriveGearCommand > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNDriveGearCommand > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMotorControllerStatus > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMotorControllerStatus > {
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

#endif // evsn_drive_408364356_hpp

