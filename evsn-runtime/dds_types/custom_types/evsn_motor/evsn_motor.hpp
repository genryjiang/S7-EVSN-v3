

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_motor.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_motor_1371888787_hpp
#define evsn_motor_1371888787_hpp

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

enum class EVSNMcuMode {
    EVSN_MCU_STANDBY, 
    EVSN_MCU_TORQUE_CONTROL, 
    EVSN_MCU_SPEED_CONTROL, 
    EVSN_MCU_CLOSE_CLUTCH, 
    EVSN_MCU_AUTO_ANGLE_OFFSET, 
    EVSN_MCU_ERROR, 
    EVSN_MCU_UNUSED_6, 
    EVSN_MCU_UNUSED_7, 
    EVSN_MCU_UNUSED_8, 
    EVSN_MCU_UNUSED_9, 
    EVSN_MCU_UNUSED_10, 
    EVSN_MCU_BOOT, 
    EVSN_MCU_CHARGE, 
    EVSN_MCU_SHUTDOWN, 
    EVSN_MCU_UNUSED_14, 
    EVSN_MCU_INITIALIZATION
};

NDDSUSERDllExport std::ostream& operator << (std::ostream& o,const EVSNMcuMode& sample);

class NDDSUSERDllExport EVSNMotorRequest {
  public:

    EVSNMotorRequest();

    EVSNMotorRequest(bool enable_,const ::EVSNMcuMode& requested_mode_,float target_torque_nm_,float vehicle_speed_kmh_,uint8_t max_recu_power_kw_,uint8_t max_boost_power_kw_);

    bool& enable() noexcept {
        return m_enable_;
    }

    const bool& enable() const noexcept {
        return m_enable_;
    }

    void enable(bool value) {

        m_enable_ = value;
    }

    ::EVSNMcuMode& requested_mode() noexcept {
        return m_requested_mode_;
    }

    const ::EVSNMcuMode& requested_mode() const noexcept {
        return m_requested_mode_;
    }

    void requested_mode(const ::EVSNMcuMode& value) {

        m_requested_mode_ = value;
    }

    void requested_mode(::EVSNMcuMode&& value) {
        m_requested_mode_ = std::move(value);
    }
    float& target_torque_nm() noexcept {
        return m_target_torque_nm_;
    }

    const float& target_torque_nm() const noexcept {
        return m_target_torque_nm_;
    }

    void target_torque_nm(float value) {

        m_target_torque_nm_ = value;
    }

    float& vehicle_speed_kmh() noexcept {
        return m_vehicle_speed_kmh_;
    }

    const float& vehicle_speed_kmh() const noexcept {
        return m_vehicle_speed_kmh_;
    }

    void vehicle_speed_kmh(float value) {

        m_vehicle_speed_kmh_ = value;
    }

    uint8_t& max_recu_power_kw() noexcept {
        return m_max_recu_power_kw_;
    }

    const uint8_t& max_recu_power_kw() const noexcept {
        return m_max_recu_power_kw_;
    }

    void max_recu_power_kw(uint8_t value) {

        m_max_recu_power_kw_ = value;
    }

    uint8_t& max_boost_power_kw() noexcept {
        return m_max_boost_power_kw_;
    }

    const uint8_t& max_boost_power_kw() const noexcept {
        return m_max_boost_power_kw_;
    }

    void max_boost_power_kw(uint8_t value) {

        m_max_boost_power_kw_ = value;
    }

    bool operator == (const EVSNMotorRequest& other_) const;
    bool operator != (const EVSNMotorRequest& other_) const;

    void swap(EVSNMotorRequest& other_) noexcept ;

  private:

    bool m_enable_;
    ::EVSNMcuMode m_requested_mode_;
    float m_target_torque_nm_;
    float m_vehicle_speed_kmh_;
    uint8_t m_max_recu_power_kw_;
    uint8_t m_max_boost_power_kw_;

};

inline void swap(EVSNMotorRequest& a, EVSNMotorRequest& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMotorRequest& sample);

class NDDSUSERDllExport EVSNMcuControl {
  public:

    EVSNMcuControl();

    EVSNMcuControl(int32_t target_speed_rpm_,bool clutch_open_request_,float vehicle_speed_kmh_,uint8_t max_recu_power_kw_,uint8_t max_boost_power_kw_,uint8_t message_count_,bool recorder_flash_reset_,bool clear_errors_request_,bool active_discharge_request_,const ::EVSNMcuMode& requested_mode_,float target_torque_nm_);

    int32_t& target_speed_rpm() noexcept {
        return m_target_speed_rpm_;
    }

    const int32_t& target_speed_rpm() const noexcept {
        return m_target_speed_rpm_;
    }

    void target_speed_rpm(int32_t value) {

        m_target_speed_rpm_ = value;
    }

    bool& clutch_open_request() noexcept {
        return m_clutch_open_request_;
    }

    const bool& clutch_open_request() const noexcept {
        return m_clutch_open_request_;
    }

    void clutch_open_request(bool value) {

        m_clutch_open_request_ = value;
    }

    float& vehicle_speed_kmh() noexcept {
        return m_vehicle_speed_kmh_;
    }

    const float& vehicle_speed_kmh() const noexcept {
        return m_vehicle_speed_kmh_;
    }

    void vehicle_speed_kmh(float value) {

        m_vehicle_speed_kmh_ = value;
    }

    uint8_t& max_recu_power_kw() noexcept {
        return m_max_recu_power_kw_;
    }

    const uint8_t& max_recu_power_kw() const noexcept {
        return m_max_recu_power_kw_;
    }

    void max_recu_power_kw(uint8_t value) {

        m_max_recu_power_kw_ = value;
    }

    uint8_t& max_boost_power_kw() noexcept {
        return m_max_boost_power_kw_;
    }

    const uint8_t& max_boost_power_kw() const noexcept {
        return m_max_boost_power_kw_;
    }

    void max_boost_power_kw(uint8_t value) {

        m_max_boost_power_kw_ = value;
    }

    uint8_t& message_count() noexcept {
        return m_message_count_;
    }

    const uint8_t& message_count() const noexcept {
        return m_message_count_;
    }

    void message_count(uint8_t value) {

        m_message_count_ = value;
    }

    bool& recorder_flash_reset() noexcept {
        return m_recorder_flash_reset_;
    }

    const bool& recorder_flash_reset() const noexcept {
        return m_recorder_flash_reset_;
    }

    void recorder_flash_reset(bool value) {

        m_recorder_flash_reset_ = value;
    }

    bool& clear_errors_request() noexcept {
        return m_clear_errors_request_;
    }

    const bool& clear_errors_request() const noexcept {
        return m_clear_errors_request_;
    }

    void clear_errors_request(bool value) {

        m_clear_errors_request_ = value;
    }

    bool& active_discharge_request() noexcept {
        return m_active_discharge_request_;
    }

    const bool& active_discharge_request() const noexcept {
        return m_active_discharge_request_;
    }

    void active_discharge_request(bool value) {

        m_active_discharge_request_ = value;
    }

    ::EVSNMcuMode& requested_mode() noexcept {
        return m_requested_mode_;
    }

    const ::EVSNMcuMode& requested_mode() const noexcept {
        return m_requested_mode_;
    }

    void requested_mode(const ::EVSNMcuMode& value) {

        m_requested_mode_ = value;
    }

    void requested_mode(::EVSNMcuMode&& value) {
        m_requested_mode_ = std::move(value);
    }
    float& target_torque_nm() noexcept {
        return m_target_torque_nm_;
    }

    const float& target_torque_nm() const noexcept {
        return m_target_torque_nm_;
    }

    void target_torque_nm(float value) {

        m_target_torque_nm_ = value;
    }

    bool operator == (const EVSNMcuControl& other_) const;
    bool operator != (const EVSNMcuControl& other_) const;

    void swap(EVSNMcuControl& other_) noexcept ;

  private:

    int32_t m_target_speed_rpm_;
    bool m_clutch_open_request_;
    float m_vehicle_speed_kmh_;
    uint8_t m_max_recu_power_kw_;
    uint8_t m_max_boost_power_kw_;
    uint8_t m_message_count_;
    bool m_recorder_flash_reset_;
    bool m_clear_errors_request_;
    bool m_active_discharge_request_;
    ::EVSNMcuMode m_requested_mode_;
    float m_target_torque_nm_;

};

inline void swap(EVSNMcuControl& a, EVSNMcuControl& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMcuControl& sample);

class NDDSUSERDllExport EVSNMcuActualOperation {
  public:

    EVSNMcuActualOperation();

    EVSNMcuActualOperation(bool discharge_active_,bool contactor_open_request_,uint8_t limits_,float minimum_torque_nm_,float maximum_torque_nm_,uint8_t message_count_,const ::EVSNMcuMode& actual_mode_,uint16_t dc_link_voltage_v_,float actual_torque_nm_,int32_t actual_speed_rpm_);

    bool& discharge_active() noexcept {
        return m_discharge_active_;
    }

    const bool& discharge_active() const noexcept {
        return m_discharge_active_;
    }

    void discharge_active(bool value) {

        m_discharge_active_ = value;
    }

    bool& contactor_open_request() noexcept {
        return m_contactor_open_request_;
    }

    const bool& contactor_open_request() const noexcept {
        return m_contactor_open_request_;
    }

    void contactor_open_request(bool value) {

        m_contactor_open_request_ = value;
    }

    uint8_t& limits() noexcept {
        return m_limits_;
    }

    const uint8_t& limits() const noexcept {
        return m_limits_;
    }

    void limits(uint8_t value) {

        m_limits_ = value;
    }

    float& minimum_torque_nm() noexcept {
        return m_minimum_torque_nm_;
    }

    const float& minimum_torque_nm() const noexcept {
        return m_minimum_torque_nm_;
    }

    void minimum_torque_nm(float value) {

        m_minimum_torque_nm_ = value;
    }

    float& maximum_torque_nm() noexcept {
        return m_maximum_torque_nm_;
    }

    const float& maximum_torque_nm() const noexcept {
        return m_maximum_torque_nm_;
    }

    void maximum_torque_nm(float value) {

        m_maximum_torque_nm_ = value;
    }

    uint8_t& message_count() noexcept {
        return m_message_count_;
    }

    const uint8_t& message_count() const noexcept {
        return m_message_count_;
    }

    void message_count(uint8_t value) {

        m_message_count_ = value;
    }

    ::EVSNMcuMode& actual_mode() noexcept {
        return m_actual_mode_;
    }

    const ::EVSNMcuMode& actual_mode() const noexcept {
        return m_actual_mode_;
    }

    void actual_mode(const ::EVSNMcuMode& value) {

        m_actual_mode_ = value;
    }

    void actual_mode(::EVSNMcuMode&& value) {
        m_actual_mode_ = std::move(value);
    }
    uint16_t& dc_link_voltage_v() noexcept {
        return m_dc_link_voltage_v_;
    }

    const uint16_t& dc_link_voltage_v() const noexcept {
        return m_dc_link_voltage_v_;
    }

    void dc_link_voltage_v(uint16_t value) {

        m_dc_link_voltage_v_ = value;
    }

    float& actual_torque_nm() noexcept {
        return m_actual_torque_nm_;
    }

    const float& actual_torque_nm() const noexcept {
        return m_actual_torque_nm_;
    }

    void actual_torque_nm(float value) {

        m_actual_torque_nm_ = value;
    }

    int32_t& actual_speed_rpm() noexcept {
        return m_actual_speed_rpm_;
    }

    const int32_t& actual_speed_rpm() const noexcept {
        return m_actual_speed_rpm_;
    }

    void actual_speed_rpm(int32_t value) {

        m_actual_speed_rpm_ = value;
    }

    bool operator == (const EVSNMcuActualOperation& other_) const;
    bool operator != (const EVSNMcuActualOperation& other_) const;

    void swap(EVSNMcuActualOperation& other_) noexcept ;

  private:

    bool m_discharge_active_;
    bool m_contactor_open_request_;
    uint8_t m_limits_;
    float m_minimum_torque_nm_;
    float m_maximum_torque_nm_;
    uint8_t m_message_count_;
    ::EVSNMcuMode m_actual_mode_;
    uint16_t m_dc_link_voltage_v_;
    float m_actual_torque_nm_;
    int32_t m_actual_speed_rpm_;

};

inline void swap(EVSNMcuActualOperation& a, EVSNMcuActualOperation& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMcuActualOperation& sample);

class NDDSUSERDllExport EVSNMcuStatusInfo {
  public:

    EVSNMcuStatusInfo();

    EVSNMcuStatusInfo(uint32_t system_warnings_,uint8_t status_,uint32_t errors_,uint16_t deratings_);

    uint32_t& system_warnings() noexcept {
        return m_system_warnings_;
    }

    const uint32_t& system_warnings() const noexcept {
        return m_system_warnings_;
    }

    void system_warnings(uint32_t value) {

        m_system_warnings_ = value;
    }

    uint8_t& status() noexcept {
        return m_status_;
    }

    const uint8_t& status() const noexcept {
        return m_status_;
    }

    void status(uint8_t value) {

        m_status_ = value;
    }

    uint32_t& errors() noexcept {
        return m_errors_;
    }

    const uint32_t& errors() const noexcept {
        return m_errors_;
    }

    void errors(uint32_t value) {

        m_errors_ = value;
    }

    uint16_t& deratings() noexcept {
        return m_deratings_;
    }

    const uint16_t& deratings() const noexcept {
        return m_deratings_;
    }

    void deratings(uint16_t value) {

        m_deratings_ = value;
    }

    bool operator == (const EVSNMcuStatusInfo& other_) const;
    bool operator != (const EVSNMcuStatusInfo& other_) const;

    void swap(EVSNMcuStatusInfo& other_) noexcept ;

  private:

    uint32_t m_system_warnings_;
    uint8_t m_status_;
    uint32_t m_errors_;
    uint16_t m_deratings_;

};

inline void swap(EVSNMcuStatusInfo& a, EVSNMcuStatusInfo& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMcuStatusInfo& sample);

class NDDSUSERDllExport EVSNMcuTemperatures {
  public:

    EVSNMcuTemperatures();

    EVSNMcuTemperatures(float cooling_inlet_c_,float stator_minimum_c_,float semiconductor_minimum_c_,float dc_link_c_,float rotor_c_,float stator_maximum_c_,float semiconductor_maximum_c_,float controller_board_c_);

    float& cooling_inlet_c() noexcept {
        return m_cooling_inlet_c_;
    }

    const float& cooling_inlet_c() const noexcept {
        return m_cooling_inlet_c_;
    }

    void cooling_inlet_c(float value) {

        m_cooling_inlet_c_ = value;
    }

    float& stator_minimum_c() noexcept {
        return m_stator_minimum_c_;
    }

    const float& stator_minimum_c() const noexcept {
        return m_stator_minimum_c_;
    }

    void stator_minimum_c(float value) {

        m_stator_minimum_c_ = value;
    }

    float& semiconductor_minimum_c() noexcept {
        return m_semiconductor_minimum_c_;
    }

    const float& semiconductor_minimum_c() const noexcept {
        return m_semiconductor_minimum_c_;
    }

    void semiconductor_minimum_c(float value) {

        m_semiconductor_minimum_c_ = value;
    }

    float& dc_link_c() noexcept {
        return m_dc_link_c_;
    }

    const float& dc_link_c() const noexcept {
        return m_dc_link_c_;
    }

    void dc_link_c(float value) {

        m_dc_link_c_ = value;
    }

    float& rotor_c() noexcept {
        return m_rotor_c_;
    }

    const float& rotor_c() const noexcept {
        return m_rotor_c_;
    }

    void rotor_c(float value) {

        m_rotor_c_ = value;
    }

    float& stator_maximum_c() noexcept {
        return m_stator_maximum_c_;
    }

    const float& stator_maximum_c() const noexcept {
        return m_stator_maximum_c_;
    }

    void stator_maximum_c(float value) {

        m_stator_maximum_c_ = value;
    }

    float& semiconductor_maximum_c() noexcept {
        return m_semiconductor_maximum_c_;
    }

    const float& semiconductor_maximum_c() const noexcept {
        return m_semiconductor_maximum_c_;
    }

    void semiconductor_maximum_c(float value) {

        m_semiconductor_maximum_c_ = value;
    }

    float& controller_board_c() noexcept {
        return m_controller_board_c_;
    }

    const float& controller_board_c() const noexcept {
        return m_controller_board_c_;
    }

    void controller_board_c(float value) {

        m_controller_board_c_ = value;
    }

    bool operator == (const EVSNMcuTemperatures& other_) const;
    bool operator != (const EVSNMcuTemperatures& other_) const;

    void swap(EVSNMcuTemperatures& other_) noexcept ;

  private:

    float m_cooling_inlet_c_;
    float m_stator_minimum_c_;
    float m_semiconductor_minimum_c_;
    float m_dc_link_c_;
    float m_rotor_c_;
    float m_stator_maximum_c_;
    float m_semiconductor_maximum_c_;
    float m_controller_board_c_;

};

inline void swap(EVSNMcuTemperatures& a, EVSNMcuTemperatures& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMcuTemperatures& sample);

class NDDSUSERDllExport EVSNMcuSystemInfo {
  public:

    EVSNMcuSystemInfo();

    EVSNMcuSystemInfo(uint8_t multiplexor_,uint8_t mgu_serial_number_,uint16_t mgu_mds_crc_,uint16_t mcu_pds_crc_,uint16_t mcu_kds_crc_,uint16_t mcu_dds_crc_,uint8_t mcu_hardware_version_,uint8_t mcu_serial_number_,uint16_t mcu_software_version_time_,uint32_t mcu_software_version_date_);

    uint8_t& multiplexor() noexcept {
        return m_multiplexor_;
    }

    const uint8_t& multiplexor() const noexcept {
        return m_multiplexor_;
    }

    void multiplexor(uint8_t value) {

        m_multiplexor_ = value;
    }

    uint8_t& mgu_serial_number() noexcept {
        return m_mgu_serial_number_;
    }

    const uint8_t& mgu_serial_number() const noexcept {
        return m_mgu_serial_number_;
    }

    void mgu_serial_number(uint8_t value) {

        m_mgu_serial_number_ = value;
    }

    uint16_t& mgu_mds_crc() noexcept {
        return m_mgu_mds_crc_;
    }

    const uint16_t& mgu_mds_crc() const noexcept {
        return m_mgu_mds_crc_;
    }

    void mgu_mds_crc(uint16_t value) {

        m_mgu_mds_crc_ = value;
    }

    uint16_t& mcu_pds_crc() noexcept {
        return m_mcu_pds_crc_;
    }

    const uint16_t& mcu_pds_crc() const noexcept {
        return m_mcu_pds_crc_;
    }

    void mcu_pds_crc(uint16_t value) {

        m_mcu_pds_crc_ = value;
    }

    uint16_t& mcu_kds_crc() noexcept {
        return m_mcu_kds_crc_;
    }

    const uint16_t& mcu_kds_crc() const noexcept {
        return m_mcu_kds_crc_;
    }

    void mcu_kds_crc(uint16_t value) {

        m_mcu_kds_crc_ = value;
    }

    uint16_t& mcu_dds_crc() noexcept {
        return m_mcu_dds_crc_;
    }

    const uint16_t& mcu_dds_crc() const noexcept {
        return m_mcu_dds_crc_;
    }

    void mcu_dds_crc(uint16_t value) {

        m_mcu_dds_crc_ = value;
    }

    uint8_t& mcu_hardware_version() noexcept {
        return m_mcu_hardware_version_;
    }

    const uint8_t& mcu_hardware_version() const noexcept {
        return m_mcu_hardware_version_;
    }

    void mcu_hardware_version(uint8_t value) {

        m_mcu_hardware_version_ = value;
    }

    uint8_t& mcu_serial_number() noexcept {
        return m_mcu_serial_number_;
    }

    const uint8_t& mcu_serial_number() const noexcept {
        return m_mcu_serial_number_;
    }

    void mcu_serial_number(uint8_t value) {

        m_mcu_serial_number_ = value;
    }

    uint16_t& mcu_software_version_time() noexcept {
        return m_mcu_software_version_time_;
    }

    const uint16_t& mcu_software_version_time() const noexcept {
        return m_mcu_software_version_time_;
    }

    void mcu_software_version_time(uint16_t value) {

        m_mcu_software_version_time_ = value;
    }

    uint32_t& mcu_software_version_date() noexcept {
        return m_mcu_software_version_date_;
    }

    const uint32_t& mcu_software_version_date() const noexcept {
        return m_mcu_software_version_date_;
    }

    void mcu_software_version_date(uint32_t value) {

        m_mcu_software_version_date_ = value;
    }

    bool operator == (const EVSNMcuSystemInfo& other_) const;
    bool operator != (const EVSNMcuSystemInfo& other_) const;

    void swap(EVSNMcuSystemInfo& other_) noexcept ;

  private:

    uint8_t m_multiplexor_;
    uint8_t m_mgu_serial_number_;
    uint16_t m_mgu_mds_crc_;
    uint16_t m_mcu_pds_crc_;
    uint16_t m_mcu_kds_crc_;
    uint16_t m_mcu_dds_crc_;
    uint8_t m_mcu_hardware_version_;
    uint8_t m_mcu_serial_number_;
    uint16_t m_mcu_software_version_time_;
    uint32_t m_mcu_software_version_date_;

};

inline void swap(EVSNMcuSystemInfo& a, EVSNMcuSystemInfo& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMcuSystemInfo& sample);

class NDDSUSERDllExport EVSNMotorStatus {
  public:

    EVSNMotorStatus();

    EVSNMotorStatus(bool actual_operation_fresh_,bool status_info_fresh_,bool temperatures_fresh_,bool system_info_fresh_,const ::EVSNMcuActualOperation& actual_operation_,const ::EVSNMcuStatusInfo& status_info_,const ::EVSNMcuTemperatures& temperatures_,const ::EVSNMcuSystemInfo& system_info_);

    bool& actual_operation_fresh() noexcept {
        return m_actual_operation_fresh_;
    }

    const bool& actual_operation_fresh() const noexcept {
        return m_actual_operation_fresh_;
    }

    void actual_operation_fresh(bool value) {

        m_actual_operation_fresh_ = value;
    }

    bool& status_info_fresh() noexcept {
        return m_status_info_fresh_;
    }

    const bool& status_info_fresh() const noexcept {
        return m_status_info_fresh_;
    }

    void status_info_fresh(bool value) {

        m_status_info_fresh_ = value;
    }

    bool& temperatures_fresh() noexcept {
        return m_temperatures_fresh_;
    }

    const bool& temperatures_fresh() const noexcept {
        return m_temperatures_fresh_;
    }

    void temperatures_fresh(bool value) {

        m_temperatures_fresh_ = value;
    }

    bool& system_info_fresh() noexcept {
        return m_system_info_fresh_;
    }

    const bool& system_info_fresh() const noexcept {
        return m_system_info_fresh_;
    }

    void system_info_fresh(bool value) {

        m_system_info_fresh_ = value;
    }

    ::EVSNMcuActualOperation& actual_operation() noexcept {
        return m_actual_operation_;
    }

    const ::EVSNMcuActualOperation& actual_operation() const noexcept {
        return m_actual_operation_;
    }

    void actual_operation(const ::EVSNMcuActualOperation& value) {

        m_actual_operation_ = value;
    }

    void actual_operation(::EVSNMcuActualOperation&& value) {
        m_actual_operation_ = std::move(value);
    }
    ::EVSNMcuStatusInfo& status_info() noexcept {
        return m_status_info_;
    }

    const ::EVSNMcuStatusInfo& status_info() const noexcept {
        return m_status_info_;
    }

    void status_info(const ::EVSNMcuStatusInfo& value) {

        m_status_info_ = value;
    }

    void status_info(::EVSNMcuStatusInfo&& value) {
        m_status_info_ = std::move(value);
    }
    ::EVSNMcuTemperatures& temperatures() noexcept {
        return m_temperatures_;
    }

    const ::EVSNMcuTemperatures& temperatures() const noexcept {
        return m_temperatures_;
    }

    void temperatures(const ::EVSNMcuTemperatures& value) {

        m_temperatures_ = value;
    }

    void temperatures(::EVSNMcuTemperatures&& value) {
        m_temperatures_ = std::move(value);
    }
    ::EVSNMcuSystemInfo& system_info() noexcept {
        return m_system_info_;
    }

    const ::EVSNMcuSystemInfo& system_info() const noexcept {
        return m_system_info_;
    }

    void system_info(const ::EVSNMcuSystemInfo& value) {

        m_system_info_ = value;
    }

    void system_info(::EVSNMcuSystemInfo&& value) {
        m_system_info_ = std::move(value);
    }
    bool operator == (const EVSNMotorStatus& other_) const;
    bool operator != (const EVSNMotorStatus& other_) const;

    void swap(EVSNMotorStatus& other_) noexcept ;

  private:

    bool m_actual_operation_fresh_;
    bool m_status_info_fresh_;
    bool m_temperatures_fresh_;
    bool m_system_info_fresh_;
    ::EVSNMcuActualOperation m_actual_operation_;
    ::EVSNMcuStatusInfo m_status_info_;
    ::EVSNMcuTemperatures m_temperatures_;
    ::EVSNMcuSystemInfo m_system_info_;

};

inline void swap(EVSNMotorStatus& a, EVSNMotorStatus& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNMotorStatus& sample);

#ifdef NDDS_STANDALONE_TYPE
namespace rti { 
    namespace topic {
        template <>
        struct default_enumerator< ::EVSNMcuMode>
        {
            static const ::EVSNMcuMode value;
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
        struct topic_type_name< ::EVSNMotorRequest > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMotorRequest";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMotorRequest > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMotorRequest > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMotorRequest& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMotorRequest& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMotorRequest& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMotorRequest& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMcuControl > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMcuControl";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMcuControl > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMcuControl > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMcuControl& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMcuControl& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMcuControl& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMcuControl& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMcuActualOperation > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMcuActualOperation";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMcuActualOperation > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMcuActualOperation > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMcuActualOperation& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMcuActualOperation& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMcuActualOperation& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMcuActualOperation& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMcuStatusInfo > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMcuStatusInfo";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMcuStatusInfo > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMcuStatusInfo > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMcuStatusInfo& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMcuStatusInfo& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMcuStatusInfo& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMcuStatusInfo& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMcuTemperatures > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMcuTemperatures";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMcuTemperatures > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMcuTemperatures > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMcuTemperatures& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMcuTemperatures& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMcuTemperatures& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMcuTemperatures& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMcuSystemInfo > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMcuSystemInfo";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMcuSystemInfo > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMcuSystemInfo > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMcuSystemInfo& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMcuSystemInfo& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMcuSystemInfo& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMcuSystemInfo& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
        template<>
        struct topic_type_name< ::EVSNMotorStatus > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNMotorStatus";
            }
        };

        template<>
        struct is_topic_type< ::EVSNMotorStatus > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNMotorStatus > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNMotorStatus& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNMotorStatus& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNMotorStatus& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNMotorStatus& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
    }
}

namespace rti { 
    namespace topic {

        template <>
        struct default_enumerator< ::EVSNMcuMode>
        {
            static const ::EVSNMcuMode value;
        };
        template<>
        struct dynamic_type< ::EVSNMcuMode > {
            typedef ::dds::core::xtypes::EnumType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::EnumType& get();
        };

        template <>
        struct extensibility< ::EVSNMcuMode > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMotorRequest > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMotorRequest > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMcuControl > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMcuControl > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMcuActualOperation > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMcuActualOperation > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMcuStatusInfo > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMcuStatusInfo > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMcuTemperatures > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMcuTemperatures > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMcuSystemInfo > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMcuSystemInfo > {
            static const ::dds::core::xtypes::ExtensibilityKind::type kind =
            ::dds::core::xtypes::ExtensibilityKind::EXTENSIBLE;    };

        template<>
        struct dynamic_type< ::EVSNMotorStatus > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNMotorStatus > {
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

#endif // evsn_motor_1371888787_hpp

