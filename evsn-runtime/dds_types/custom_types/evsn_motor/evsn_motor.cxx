

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_motor.idl
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
#include "evsn_motorPlugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_motor.hpp"

#include <rti/util/ostream_operators.hpp>

std::ostream& operator << (std::ostream& o,const EVSNMcuMode& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    switch(sample){
        case EVSNMcuMode::EVSN_MCU_STANDBY:
        o << "EVSNMcuMode::EVSN_MCU_STANDBY" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_TORQUE_CONTROL:
        o << "EVSNMcuMode::EVSN_MCU_TORQUE_CONTROL" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_SPEED_CONTROL:
        o << "EVSNMcuMode::EVSN_MCU_SPEED_CONTROL" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_CLOSE_CLUTCH:
        o << "EVSNMcuMode::EVSN_MCU_CLOSE_CLUTCH" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_AUTO_ANGLE_OFFSET:
        o << "EVSNMcuMode::EVSN_MCU_AUTO_ANGLE_OFFSET" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_ERROR:
        o << "EVSNMcuMode::EVSN_MCU_ERROR" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_UNUSED_6:
        o << "EVSNMcuMode::EVSN_MCU_UNUSED_6" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_UNUSED_7:
        o << "EVSNMcuMode::EVSN_MCU_UNUSED_7" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_UNUSED_8:
        o << "EVSNMcuMode::EVSN_MCU_UNUSED_8" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_UNUSED_9:
        o << "EVSNMcuMode::EVSN_MCU_UNUSED_9" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_UNUSED_10:
        o << "EVSNMcuMode::EVSN_MCU_UNUSED_10" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_BOOT:
        o << "EVSNMcuMode::EVSN_MCU_BOOT" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_CHARGE:
        o << "EVSNMcuMode::EVSN_MCU_CHARGE" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_SHUTDOWN:
        o << "EVSNMcuMode::EVSN_MCU_SHUTDOWN" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_UNUSED_14:
        o << "EVSNMcuMode::EVSN_MCU_UNUSED_14" << " ";
        break;
        case EVSNMcuMode::EVSN_MCU_INITIALIZATION:
        o << "EVSNMcuMode::EVSN_MCU_INITIALIZATION" << " ";
        break;
        default:
        {
            /* Prevents compiler warnings */
        }
    }
    return o;
}

// ---- EVSNMotorRequest: 

EVSNMotorRequest::EVSNMotorRequest() :
    m_enable_ (0) ,
    m_requested_mode_(EVSNMcuMode::EVSN_MCU_STANDBY) ,
    m_target_torque_nm_ (0.0f) ,
    m_vehicle_speed_kmh_ (0.0f) ,
    m_max_recu_power_kw_ (0) ,
    m_max_boost_power_kw_ (0)  {

}   

EVSNMotorRequest::EVSNMotorRequest (bool enable_,const ::EVSNMcuMode& requested_mode_,float target_torque_nm_,float vehicle_speed_kmh_,uint8_t max_recu_power_kw_,uint8_t max_boost_power_kw_):
    m_enable_(enable_), 
    m_requested_mode_(requested_mode_), 
    m_target_torque_nm_(target_torque_nm_), 
    m_vehicle_speed_kmh_(vehicle_speed_kmh_), 
    m_max_recu_power_kw_(max_recu_power_kw_), 
    m_max_boost_power_kw_(max_boost_power_kw_) {
}

void EVSNMotorRequest::swap(EVSNMotorRequest& other_)  noexcept 
{
    using std::swap;
    swap(m_enable_, other_.m_enable_);
    swap(m_requested_mode_, other_.m_requested_mode_);
    swap(m_target_torque_nm_, other_.m_target_torque_nm_);
    swap(m_vehicle_speed_kmh_, other_.m_vehicle_speed_kmh_);
    swap(m_max_recu_power_kw_, other_.m_max_recu_power_kw_);
    swap(m_max_boost_power_kw_, other_.m_max_boost_power_kw_);
}  

bool EVSNMotorRequest::operator == (const EVSNMotorRequest& other_) const {
    if (m_enable_ != other_.m_enable_) {
        return false;
    }
    if (m_requested_mode_ != other_.m_requested_mode_) {
        return false;
    }
    if (std::fabs(m_target_torque_nm_ - other_.m_target_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_target_torque_nm_ - other_.m_target_torque_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_vehicle_speed_kmh_ - other_.m_vehicle_speed_kmh_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_vehicle_speed_kmh_ - other_.m_vehicle_speed_kmh_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (m_max_recu_power_kw_ != other_.m_max_recu_power_kw_) {
        return false;
    }
    if (m_max_boost_power_kw_ != other_.m_max_boost_power_kw_) {
        return false;
    }
    return true;
}

bool EVSNMotorRequest::operator != (const EVSNMotorRequest& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMotorRequest& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "enable: " << sample.enable ()<<", ";
    o << "requested_mode: " << sample.requested_mode ()<<", ";
    o << "target_torque_nm: " << std::setprecision(9) << sample.target_torque_nm ()<<", ";
    o << "vehicle_speed_kmh: " << std::setprecision(9) << sample.vehicle_speed_kmh ()<<", ";
    o << "max_recu_power_kw: " << (int) sample.max_recu_power_kw ()<<", ";
    o << "max_boost_power_kw: " << (int) sample.max_boost_power_kw ();
    o <<"]";
    return o;
}

// ---- EVSNMcuControl: 

EVSNMcuControl::EVSNMcuControl() :
    m_target_speed_rpm_ (0) ,
    m_clutch_open_request_ (0) ,
    m_vehicle_speed_kmh_ (0.0f) ,
    m_max_recu_power_kw_ (0) ,
    m_max_boost_power_kw_ (0) ,
    m_message_count_ (0) ,
    m_recorder_flash_reset_ (0) ,
    m_clear_errors_request_ (0) ,
    m_active_discharge_request_ (0) ,
    m_requested_mode_(EVSNMcuMode::EVSN_MCU_STANDBY) ,
    m_target_torque_nm_ (0.0f)  {

}   

EVSNMcuControl::EVSNMcuControl (int32_t target_speed_rpm_,bool clutch_open_request_,float vehicle_speed_kmh_,uint8_t max_recu_power_kw_,uint8_t max_boost_power_kw_,uint8_t message_count_,bool recorder_flash_reset_,bool clear_errors_request_,bool active_discharge_request_,const ::EVSNMcuMode& requested_mode_,float target_torque_nm_):
    m_target_speed_rpm_(target_speed_rpm_), 
    m_clutch_open_request_(clutch_open_request_), 
    m_vehicle_speed_kmh_(vehicle_speed_kmh_), 
    m_max_recu_power_kw_(max_recu_power_kw_), 
    m_max_boost_power_kw_(max_boost_power_kw_), 
    m_message_count_(message_count_), 
    m_recorder_flash_reset_(recorder_flash_reset_), 
    m_clear_errors_request_(clear_errors_request_), 
    m_active_discharge_request_(active_discharge_request_), 
    m_requested_mode_(requested_mode_), 
    m_target_torque_nm_(target_torque_nm_) {
}

void EVSNMcuControl::swap(EVSNMcuControl& other_)  noexcept 
{
    using std::swap;
    swap(m_target_speed_rpm_, other_.m_target_speed_rpm_);
    swap(m_clutch_open_request_, other_.m_clutch_open_request_);
    swap(m_vehicle_speed_kmh_, other_.m_vehicle_speed_kmh_);
    swap(m_max_recu_power_kw_, other_.m_max_recu_power_kw_);
    swap(m_max_boost_power_kw_, other_.m_max_boost_power_kw_);
    swap(m_message_count_, other_.m_message_count_);
    swap(m_recorder_flash_reset_, other_.m_recorder_flash_reset_);
    swap(m_clear_errors_request_, other_.m_clear_errors_request_);
    swap(m_active_discharge_request_, other_.m_active_discharge_request_);
    swap(m_requested_mode_, other_.m_requested_mode_);
    swap(m_target_torque_nm_, other_.m_target_torque_nm_);
}  

bool EVSNMcuControl::operator == (const EVSNMcuControl& other_) const {
    if (m_target_speed_rpm_ != other_.m_target_speed_rpm_) {
        return false;
    }
    if (m_clutch_open_request_ != other_.m_clutch_open_request_) {
        return false;
    }
    if (std::fabs(m_vehicle_speed_kmh_ - other_.m_vehicle_speed_kmh_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_vehicle_speed_kmh_ - other_.m_vehicle_speed_kmh_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (m_max_recu_power_kw_ != other_.m_max_recu_power_kw_) {
        return false;
    }
    if (m_max_boost_power_kw_ != other_.m_max_boost_power_kw_) {
        return false;
    }
    if (m_message_count_ != other_.m_message_count_) {
        return false;
    }
    if (m_recorder_flash_reset_ != other_.m_recorder_flash_reset_) {
        return false;
    }
    if (m_clear_errors_request_ != other_.m_clear_errors_request_) {
        return false;
    }
    if (m_active_discharge_request_ != other_.m_active_discharge_request_) {
        return false;
    }
    if (m_requested_mode_ != other_.m_requested_mode_) {
        return false;
    }
    if (std::fabs(m_target_torque_nm_ - other_.m_target_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_target_torque_nm_ - other_.m_target_torque_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    return true;
}

bool EVSNMcuControl::operator != (const EVSNMcuControl& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMcuControl& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "target_speed_rpm: " << sample.target_speed_rpm ()<<", ";
    o << "clutch_open_request: " << sample.clutch_open_request ()<<", ";
    o << "vehicle_speed_kmh: " << std::setprecision(9) << sample.vehicle_speed_kmh ()<<", ";
    o << "max_recu_power_kw: " << (int) sample.max_recu_power_kw ()<<", ";
    o << "max_boost_power_kw: " << (int) sample.max_boost_power_kw ()<<", ";
    o << "message_count: " << (int) sample.message_count ()<<", ";
    o << "recorder_flash_reset: " << sample.recorder_flash_reset ()<<", ";
    o << "clear_errors_request: " << sample.clear_errors_request ()<<", ";
    o << "active_discharge_request: " << sample.active_discharge_request ()<<", ";
    o << "requested_mode: " << sample.requested_mode ()<<", ";
    o << "target_torque_nm: " << std::setprecision(9) << sample.target_torque_nm ();
    o <<"]";
    return o;
}

// ---- EVSNMcuActualOperation: 

EVSNMcuActualOperation::EVSNMcuActualOperation() :
    m_discharge_active_ (0) ,
    m_contactor_open_request_ (0) ,
    m_limits_ (0) ,
    m_minimum_torque_nm_ (0.0f) ,
    m_maximum_torque_nm_ (0.0f) ,
    m_message_count_ (0) ,
    m_actual_mode_(EVSNMcuMode::EVSN_MCU_STANDBY) ,
    m_dc_link_voltage_v_ (0) ,
    m_actual_torque_nm_ (0.0f) ,
    m_actual_speed_rpm_ (0)  {

}   

EVSNMcuActualOperation::EVSNMcuActualOperation (bool discharge_active_,bool contactor_open_request_,uint8_t limits_,float minimum_torque_nm_,float maximum_torque_nm_,uint8_t message_count_,const ::EVSNMcuMode& actual_mode_,uint16_t dc_link_voltage_v_,float actual_torque_nm_,int32_t actual_speed_rpm_):
    m_discharge_active_(discharge_active_), 
    m_contactor_open_request_(contactor_open_request_), 
    m_limits_(limits_), 
    m_minimum_torque_nm_(minimum_torque_nm_), 
    m_maximum_torque_nm_(maximum_torque_nm_), 
    m_message_count_(message_count_), 
    m_actual_mode_(actual_mode_), 
    m_dc_link_voltage_v_(dc_link_voltage_v_), 
    m_actual_torque_nm_(actual_torque_nm_), 
    m_actual_speed_rpm_(actual_speed_rpm_) {
}

void EVSNMcuActualOperation::swap(EVSNMcuActualOperation& other_)  noexcept 
{
    using std::swap;
    swap(m_discharge_active_, other_.m_discharge_active_);
    swap(m_contactor_open_request_, other_.m_contactor_open_request_);
    swap(m_limits_, other_.m_limits_);
    swap(m_minimum_torque_nm_, other_.m_minimum_torque_nm_);
    swap(m_maximum_torque_nm_, other_.m_maximum_torque_nm_);
    swap(m_message_count_, other_.m_message_count_);
    swap(m_actual_mode_, other_.m_actual_mode_);
    swap(m_dc_link_voltage_v_, other_.m_dc_link_voltage_v_);
    swap(m_actual_torque_nm_, other_.m_actual_torque_nm_);
    swap(m_actual_speed_rpm_, other_.m_actual_speed_rpm_);
}  

bool EVSNMcuActualOperation::operator == (const EVSNMcuActualOperation& other_) const {
    if (m_discharge_active_ != other_.m_discharge_active_) {
        return false;
    }
    if (m_contactor_open_request_ != other_.m_contactor_open_request_) {
        return false;
    }
    if (m_limits_ != other_.m_limits_) {
        return false;
    }
    if (std::fabs(m_minimum_torque_nm_ - other_.m_minimum_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_minimum_torque_nm_ - other_.m_minimum_torque_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_maximum_torque_nm_ - other_.m_maximum_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_maximum_torque_nm_ - other_.m_maximum_torque_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (m_message_count_ != other_.m_message_count_) {
        return false;
    }
    if (m_actual_mode_ != other_.m_actual_mode_) {
        return false;
    }
    if (m_dc_link_voltage_v_ != other_.m_dc_link_voltage_v_) {
        return false;
    }
    if (std::fabs(m_actual_torque_nm_ - other_.m_actual_torque_nm_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_actual_torque_nm_ - other_.m_actual_torque_nm_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (m_actual_speed_rpm_ != other_.m_actual_speed_rpm_) {
        return false;
    }
    return true;
}

bool EVSNMcuActualOperation::operator != (const EVSNMcuActualOperation& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMcuActualOperation& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "discharge_active: " << sample.discharge_active ()<<", ";
    o << "contactor_open_request: " << sample.contactor_open_request ()<<", ";
    o << "limits: " << (int) sample.limits ()<<", ";
    o << "minimum_torque_nm: " << std::setprecision(9) << sample.minimum_torque_nm ()<<", ";
    o << "maximum_torque_nm: " << std::setprecision(9) << sample.maximum_torque_nm ()<<", ";
    o << "message_count: " << (int) sample.message_count ()<<", ";
    o << "actual_mode: " << sample.actual_mode ()<<", ";
    o << "dc_link_voltage_v: " << sample.dc_link_voltage_v ()<<", ";
    o << "actual_torque_nm: " << std::setprecision(9) << sample.actual_torque_nm ()<<", ";
    o << "actual_speed_rpm: " << sample.actual_speed_rpm ();
    o <<"]";
    return o;
}

// ---- EVSNMcuStatusInfo: 

EVSNMcuStatusInfo::EVSNMcuStatusInfo() :
    m_system_warnings_ (0u) ,
    m_status_ (0) ,
    m_errors_ (0u) ,
    m_deratings_ (0)  {

}   

EVSNMcuStatusInfo::EVSNMcuStatusInfo (uint32_t system_warnings_,uint8_t status_,uint32_t errors_,uint16_t deratings_):
    m_system_warnings_(system_warnings_), 
    m_status_(status_), 
    m_errors_(errors_), 
    m_deratings_(deratings_) {
}

void EVSNMcuStatusInfo::swap(EVSNMcuStatusInfo& other_)  noexcept 
{
    using std::swap;
    swap(m_system_warnings_, other_.m_system_warnings_);
    swap(m_status_, other_.m_status_);
    swap(m_errors_, other_.m_errors_);
    swap(m_deratings_, other_.m_deratings_);
}  

bool EVSNMcuStatusInfo::operator == (const EVSNMcuStatusInfo& other_) const {
    if (m_system_warnings_ != other_.m_system_warnings_) {
        return false;
    }
    if (m_status_ != other_.m_status_) {
        return false;
    }
    if (m_errors_ != other_.m_errors_) {
        return false;
    }
    if (m_deratings_ != other_.m_deratings_) {
        return false;
    }
    return true;
}

bool EVSNMcuStatusInfo::operator != (const EVSNMcuStatusInfo& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMcuStatusInfo& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "system_warnings: " << sample.system_warnings ()<<", ";
    o << "status: " << (int) sample.status ()<<", ";
    o << "errors: " << sample.errors ()<<", ";
    o << "deratings: " << sample.deratings ();
    o <<"]";
    return o;
}

// ---- EVSNMcuTemperatures: 

EVSNMcuTemperatures::EVSNMcuTemperatures() :
    m_cooling_inlet_c_ (0.0f) ,
    m_stator_minimum_c_ (0.0f) ,
    m_semiconductor_minimum_c_ (0.0f) ,
    m_dc_link_c_ (0.0f) ,
    m_rotor_c_ (0.0f) ,
    m_stator_maximum_c_ (0.0f) ,
    m_semiconductor_maximum_c_ (0.0f) ,
    m_controller_board_c_ (0.0f)  {

}   

EVSNMcuTemperatures::EVSNMcuTemperatures (float cooling_inlet_c_,float stator_minimum_c_,float semiconductor_minimum_c_,float dc_link_c_,float rotor_c_,float stator_maximum_c_,float semiconductor_maximum_c_,float controller_board_c_):
    m_cooling_inlet_c_(cooling_inlet_c_), 
    m_stator_minimum_c_(stator_minimum_c_), 
    m_semiconductor_minimum_c_(semiconductor_minimum_c_), 
    m_dc_link_c_(dc_link_c_), 
    m_rotor_c_(rotor_c_), 
    m_stator_maximum_c_(stator_maximum_c_), 
    m_semiconductor_maximum_c_(semiconductor_maximum_c_), 
    m_controller_board_c_(controller_board_c_) {
}

void EVSNMcuTemperatures::swap(EVSNMcuTemperatures& other_)  noexcept 
{
    using std::swap;
    swap(m_cooling_inlet_c_, other_.m_cooling_inlet_c_);
    swap(m_stator_minimum_c_, other_.m_stator_minimum_c_);
    swap(m_semiconductor_minimum_c_, other_.m_semiconductor_minimum_c_);
    swap(m_dc_link_c_, other_.m_dc_link_c_);
    swap(m_rotor_c_, other_.m_rotor_c_);
    swap(m_stator_maximum_c_, other_.m_stator_maximum_c_);
    swap(m_semiconductor_maximum_c_, other_.m_semiconductor_maximum_c_);
    swap(m_controller_board_c_, other_.m_controller_board_c_);
}  

bool EVSNMcuTemperatures::operator == (const EVSNMcuTemperatures& other_) const {
    if (std::fabs(m_cooling_inlet_c_ - other_.m_cooling_inlet_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_cooling_inlet_c_ - other_.m_cooling_inlet_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_stator_minimum_c_ - other_.m_stator_minimum_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_stator_minimum_c_ - other_.m_stator_minimum_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_semiconductor_minimum_c_ - other_.m_semiconductor_minimum_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_semiconductor_minimum_c_ - other_.m_semiconductor_minimum_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_dc_link_c_ - other_.m_dc_link_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_dc_link_c_ - other_.m_dc_link_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_rotor_c_ - other_.m_rotor_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_rotor_c_ - other_.m_rotor_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_stator_maximum_c_ - other_.m_stator_maximum_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_stator_maximum_c_ - other_.m_stator_maximum_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_semiconductor_maximum_c_ - other_.m_semiconductor_maximum_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_semiconductor_maximum_c_ - other_.m_semiconductor_maximum_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    if (std::fabs(m_controller_board_c_ - other_.m_controller_board_c_) > std::numeric_limits< float>::epsilon()
    && !(std::fabs(m_controller_board_c_ - other_.m_controller_board_c_) < (std::numeric_limits< float>::min)())) {
        return false;
    }
    return true;
}

bool EVSNMcuTemperatures::operator != (const EVSNMcuTemperatures& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMcuTemperatures& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "cooling_inlet_c: " << std::setprecision(9) << sample.cooling_inlet_c ()<<", ";
    o << "stator_minimum_c: " << std::setprecision(9) << sample.stator_minimum_c ()<<", ";
    o << "semiconductor_minimum_c: " << std::setprecision(9) << sample.semiconductor_minimum_c ()<<", ";
    o << "dc_link_c: " << std::setprecision(9) << sample.dc_link_c ()<<", ";
    o << "rotor_c: " << std::setprecision(9) << sample.rotor_c ()<<", ";
    o << "stator_maximum_c: " << std::setprecision(9) << sample.stator_maximum_c ()<<", ";
    o << "semiconductor_maximum_c: " << std::setprecision(9) << sample.semiconductor_maximum_c ()<<", ";
    o << "controller_board_c: " << std::setprecision(9) << sample.controller_board_c ();
    o <<"]";
    return o;
}

// ---- EVSNMcuSystemInfo: 

EVSNMcuSystemInfo::EVSNMcuSystemInfo() :
    m_multiplexor_ (0) ,
    m_mgu_serial_number_ (0) ,
    m_mgu_mds_crc_ (0) ,
    m_mcu_pds_crc_ (0) ,
    m_mcu_kds_crc_ (0) ,
    m_mcu_dds_crc_ (0) ,
    m_mcu_hardware_version_ (0) ,
    m_mcu_serial_number_ (0) ,
    m_mcu_software_version_time_ (0) ,
    m_mcu_software_version_date_ (0u)  {

}   

EVSNMcuSystemInfo::EVSNMcuSystemInfo (uint8_t multiplexor_,uint8_t mgu_serial_number_,uint16_t mgu_mds_crc_,uint16_t mcu_pds_crc_,uint16_t mcu_kds_crc_,uint16_t mcu_dds_crc_,uint8_t mcu_hardware_version_,uint8_t mcu_serial_number_,uint16_t mcu_software_version_time_,uint32_t mcu_software_version_date_):
    m_multiplexor_(multiplexor_), 
    m_mgu_serial_number_(mgu_serial_number_), 
    m_mgu_mds_crc_(mgu_mds_crc_), 
    m_mcu_pds_crc_(mcu_pds_crc_), 
    m_mcu_kds_crc_(mcu_kds_crc_), 
    m_mcu_dds_crc_(mcu_dds_crc_), 
    m_mcu_hardware_version_(mcu_hardware_version_), 
    m_mcu_serial_number_(mcu_serial_number_), 
    m_mcu_software_version_time_(mcu_software_version_time_), 
    m_mcu_software_version_date_(mcu_software_version_date_) {
}

void EVSNMcuSystemInfo::swap(EVSNMcuSystemInfo& other_)  noexcept 
{
    using std::swap;
    swap(m_multiplexor_, other_.m_multiplexor_);
    swap(m_mgu_serial_number_, other_.m_mgu_serial_number_);
    swap(m_mgu_mds_crc_, other_.m_mgu_mds_crc_);
    swap(m_mcu_pds_crc_, other_.m_mcu_pds_crc_);
    swap(m_mcu_kds_crc_, other_.m_mcu_kds_crc_);
    swap(m_mcu_dds_crc_, other_.m_mcu_dds_crc_);
    swap(m_mcu_hardware_version_, other_.m_mcu_hardware_version_);
    swap(m_mcu_serial_number_, other_.m_mcu_serial_number_);
    swap(m_mcu_software_version_time_, other_.m_mcu_software_version_time_);
    swap(m_mcu_software_version_date_, other_.m_mcu_software_version_date_);
}  

bool EVSNMcuSystemInfo::operator == (const EVSNMcuSystemInfo& other_) const {
    if (m_multiplexor_ != other_.m_multiplexor_) {
        return false;
    }
    if (m_mgu_serial_number_ != other_.m_mgu_serial_number_) {
        return false;
    }
    if (m_mgu_mds_crc_ != other_.m_mgu_mds_crc_) {
        return false;
    }
    if (m_mcu_pds_crc_ != other_.m_mcu_pds_crc_) {
        return false;
    }
    if (m_mcu_kds_crc_ != other_.m_mcu_kds_crc_) {
        return false;
    }
    if (m_mcu_dds_crc_ != other_.m_mcu_dds_crc_) {
        return false;
    }
    if (m_mcu_hardware_version_ != other_.m_mcu_hardware_version_) {
        return false;
    }
    if (m_mcu_serial_number_ != other_.m_mcu_serial_number_) {
        return false;
    }
    if (m_mcu_software_version_time_ != other_.m_mcu_software_version_time_) {
        return false;
    }
    if (m_mcu_software_version_date_ != other_.m_mcu_software_version_date_) {
        return false;
    }
    return true;
}

bool EVSNMcuSystemInfo::operator != (const EVSNMcuSystemInfo& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMcuSystemInfo& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "multiplexor: " << (int) sample.multiplexor ()<<", ";
    o << "mgu_serial_number: " << (int) sample.mgu_serial_number ()<<", ";
    o << "mgu_mds_crc: " << sample.mgu_mds_crc ()<<", ";
    o << "mcu_pds_crc: " << sample.mcu_pds_crc ()<<", ";
    o << "mcu_kds_crc: " << sample.mcu_kds_crc ()<<", ";
    o << "mcu_dds_crc: " << sample.mcu_dds_crc ()<<", ";
    o << "mcu_hardware_version: " << (int) sample.mcu_hardware_version ()<<", ";
    o << "mcu_serial_number: " << (int) sample.mcu_serial_number ()<<", ";
    o << "mcu_software_version_time: " << sample.mcu_software_version_time ()<<", ";
    o << "mcu_software_version_date: " << sample.mcu_software_version_date ();
    o <<"]";
    return o;
}

// ---- EVSNMotorStatus: 

EVSNMotorStatus::EVSNMotorStatus() :
    m_actual_operation_fresh_ (0) ,
    m_status_info_fresh_ (0) ,
    m_temperatures_fresh_ (0) ,
    m_system_info_fresh_ (0)  {

}   

EVSNMotorStatus::EVSNMotorStatus (bool actual_operation_fresh_,bool status_info_fresh_,bool temperatures_fresh_,bool system_info_fresh_,const ::EVSNMcuActualOperation& actual_operation_,const ::EVSNMcuStatusInfo& status_info_,const ::EVSNMcuTemperatures& temperatures_,const ::EVSNMcuSystemInfo& system_info_):
    m_actual_operation_fresh_(actual_operation_fresh_), 
    m_status_info_fresh_(status_info_fresh_), 
    m_temperatures_fresh_(temperatures_fresh_), 
    m_system_info_fresh_(system_info_fresh_), 
    m_actual_operation_(actual_operation_), 
    m_status_info_(status_info_), 
    m_temperatures_(temperatures_), 
    m_system_info_(system_info_) {
}

void EVSNMotorStatus::swap(EVSNMotorStatus& other_)  noexcept 
{
    using std::swap;
    swap(m_actual_operation_fresh_, other_.m_actual_operation_fresh_);
    swap(m_status_info_fresh_, other_.m_status_info_fresh_);
    swap(m_temperatures_fresh_, other_.m_temperatures_fresh_);
    swap(m_system_info_fresh_, other_.m_system_info_fresh_);
    swap(m_actual_operation_, other_.m_actual_operation_);
    swap(m_status_info_, other_.m_status_info_);
    swap(m_temperatures_, other_.m_temperatures_);
    swap(m_system_info_, other_.m_system_info_);
}  

bool EVSNMotorStatus::operator == (const EVSNMotorStatus& other_) const {
    if (m_actual_operation_fresh_ != other_.m_actual_operation_fresh_) {
        return false;
    }
    if (m_status_info_fresh_ != other_.m_status_info_fresh_) {
        return false;
    }
    if (m_temperatures_fresh_ != other_.m_temperatures_fresh_) {
        return false;
    }
    if (m_system_info_fresh_ != other_.m_system_info_fresh_) {
        return false;
    }
    if (m_actual_operation_ != other_.m_actual_operation_) {
        return false;
    }
    if (m_status_info_ != other_.m_status_info_) {
        return false;
    }
    if (m_temperatures_ != other_.m_temperatures_) {
        return false;
    }
    if (m_system_info_ != other_.m_system_info_) {
        return false;
    }
    return true;
}

bool EVSNMotorStatus::operator != (const EVSNMotorStatus& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNMotorStatus& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "actual_operation_fresh: " << sample.actual_operation_fresh ()<<", ";
    o << "status_info_fresh: " << sample.status_info_fresh ()<<", ";
    o << "temperatures_fresh: " << sample.temperatures_fresh ()<<", ";
    o << "system_info_fresh: " << sample.system_info_fresh ()<<", ";
    o << "actual_operation: " << sample.actual_operation ()<<", ";
    o << "status_info: " << sample.status_info ()<<", ";
    o << "temperatures: " << sample.temperatures ()<<", ";
    o << "system_info: " << sample.system_info ();
    o <<"]";
    return o;
}

#ifdef NDDS_STANDALONE_TYPE
namespace rti {
    namespace topic {
        const ::EVSNMcuMode default_enumerator< ::EVSNMcuMode>::value = ::EVSNMcuMode::EVSN_MCU_STANDBY;
    }
}

#else
// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        const ::EVSNMcuMode default_enumerator< ::EVSNMcuMode>::value = ::EVSNMcuMode::EVSN_MCU_STANDBY;
        template<>
        struct native_type_code< ::EVSNMcuMode > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMcuMode_g_tc_members[16]=
                {

                    {
                        (char *)"EVSN_MCU_STANDBY",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_STANDBY), 
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
                        (char *)"EVSN_MCU_TORQUE_CONTROL",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_TORQUE_CONTROL), 
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
                        (char *)"EVSN_MCU_SPEED_CONTROL",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_SPEED_CONTROL), 
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
                        (char *)"EVSN_MCU_CLOSE_CLUTCH",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_CLOSE_CLUTCH), 
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
                        (char *)"EVSN_MCU_AUTO_ANGLE_OFFSET",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_AUTO_ANGLE_OFFSET), 
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
                        (char *)"EVSN_MCU_ERROR",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_ERROR), 
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
                        (char *)"EVSN_MCU_UNUSED_6",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_UNUSED_6), 
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
                        (char *)"EVSN_MCU_UNUSED_7",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_UNUSED_7), 
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
                        (char *)"EVSN_MCU_UNUSED_8",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_UNUSED_8), 
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
                        (char *)"EVSN_MCU_UNUSED_9",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_UNUSED_9), 
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
                        (char *)"EVSN_MCU_UNUSED_10",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_UNUSED_10), 
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
                        (char *)"EVSN_MCU_BOOT",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_BOOT), 
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
                        (char *)"EVSN_MCU_CHARGE",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_CHARGE), 
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
                        (char *)"EVSN_MCU_SHUTDOWN",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_SHUTDOWN), 
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
                        (char *)"EVSN_MCU_UNUSED_14",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_UNUSED_14), 
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
                        (char *)"EVSN_MCU_INITIALIZATION",/* Member name */
                        {
                            0, /* Ignored */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        static_cast<int>(::EVSNMcuMode::EVSN_MCU_INITIALIZATION), 
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

                static DDS_TypeCode EVSNMcuMode_g_tc =
                {{
                        DDS_TK_ENUM, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMcuMode", /* Name */
                        NULL,     /* Base class type code is assigned later */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        16, /* Number of members */
                        EVSNMcuMode_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Type Modifier */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMcuMode*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMcuMode_g_tc;
                }

                EVSNMcuMode_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                /* Initialize the values for annotations. */
                EVSNMcuMode_g_tc._data._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNMcuMode_g_tc._data._annotations._defaultValue._u.long_value = 0;

                EVSNMcuMode_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMcuMode_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMcuMode_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                static RTIXCdrMemberAccessInfo EVSNMcuMode_g_memberAccessInfos[1] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMcuMode_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMcuMode_g_sampleAccessInfo;
                }

                EVSNMcuMode_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 0;

                EVSNMcuMode_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMcuMode_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMcuMode);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMcuMode_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMcuMode_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMcuMode_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMcuMode_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMcuMode >;

                EVSNMcuMode_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMcuMode_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMcuMode_g_typePlugin = 
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

                return &EVSNMcuMode_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::EnumType& dynamic_type< ::EVSNMcuMode >::get()
        {
            return static_cast<const ::dds::core::xtypes::EnumType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMcuMode >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMotorRequest > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMotorRequest_g_tc_members[6]=
                {

                    {
                        (char *)"enable",/* Member name */
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
                        (char *)"requested_mode",/* Member name */
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
                        (char *)"target_torque_nm",/* Member name */
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
                        (char *)"vehicle_speed_kmh",/* Member name */
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
                        (char *)"max_recu_power_kw",/* Member name */
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
                        (char *)"max_boost_power_kw",/* Member name */
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

                static DDS_TypeCode EVSNMotorRequest_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMotorRequest", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        6, /* Number of members */
                        EVSNMotorRequest_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMotorRequest*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMotorRequest_g_tc;
                }

                EVSNMotorRequest_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMotorRequest_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorRequest_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuMode>::get().native();
                EVSNMotorRequest_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorRequest_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMotorRequest_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMotorRequest_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;

                /* Initialize the values for member annotations. */
                EVSNMotorRequest_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorRequest_g_tc_members[0]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorRequest_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNMotorRequest_g_tc_members[1]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNMotorRequest_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorRequest_g_tc_members[2]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorRequest_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorRequest_g_tc_members[2]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorRequest_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorRequest_g_tc_members[2]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorRequest_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorRequest_g_tc_members[3]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMotorRequest_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorRequest_g_tc_members[3]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMotorRequest_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMotorRequest_g_tc_members[3]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMotorRequest_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMotorRequest_g_tc_members[4]._annotations._defaultValue._u.octet_value = 0;
                EVSNMotorRequest_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMotorRequest_g_tc_members[4]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMotorRequest_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMotorRequest_g_tc_members[4]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMotorRequest_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMotorRequest_g_tc_members[5]._annotations._defaultValue._u.octet_value = 0;
                EVSNMotorRequest_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMotorRequest_g_tc_members[5]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMotorRequest_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMotorRequest_g_tc_members[5]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;

                EVSNMotorRequest_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMotorRequest_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMotorRequest_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMotorRequest *sample;

                static RTIXCdrMemberAccessInfo EVSNMotorRequest_g_memberAccessInfos[6] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMotorRequest_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMotorRequest_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMotorRequest);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMotorRequest_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->enable() - (char *)sample);

                EVSNMotorRequest_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->requested_mode() - (char *)sample);

                EVSNMotorRequest_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->target_torque_nm() - (char *)sample);

                EVSNMotorRequest_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->vehicle_speed_kmh() - (char *)sample);

                EVSNMotorRequest_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->max_recu_power_kw() - (char *)sample);

                EVSNMotorRequest_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->max_boost_power_kw() - (char *)sample);

                EVSNMotorRequest_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMotorRequest_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMotorRequest);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMotorRequest_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMotorRequest_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMotorRequest_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMotorRequest_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMotorRequest >;

                EVSNMotorRequest_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMotorRequest_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMotorRequest_g_typePlugin = 
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

                return &EVSNMotorRequest_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMotorRequest >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMotorRequest >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMcuControl > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMcuControl_g_tc_members[11]=
                {

                    {
                        (char *)"target_speed_rpm",/* Member name */
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
                        (char *)"clutch_open_request",/* Member name */
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
                        (char *)"vehicle_speed_kmh",/* Member name */
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
                        (char *)"max_recu_power_kw",/* Member name */
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
                        (char *)"max_boost_power_kw",/* Member name */
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
                        (char *)"message_count",/* Member name */
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
                        (char *)"recorder_flash_reset",/* Member name */
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
                        (char *)"clear_errors_request",/* Member name */
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
                        (char *)"active_discharge_request",/* Member name */
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
                        (char *)"requested_mode",/* Member name */
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
                        (char *)"target_torque_nm",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMcuControl_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMcuControl", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        11, /* Number of members */
                        EVSNMcuControl_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMcuControl*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMcuControl_g_tc;
                }

                EVSNMcuControl_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMcuControl_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;
                EVSNMcuControl_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMcuControl_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuControl_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuControl_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuControl_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuControl_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMcuControl_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMcuControl_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMcuControl_g_tc_members[9]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuMode>::get().native();
                EVSNMcuControl_g_tc_members[10]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;

                /* Initialize the values for member annotations. */
                EVSNMcuControl_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
                EVSNMcuControl_g_tc_members[0]._annotations._defaultValue._u.long_value = 0;
                EVSNMcuControl_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_LONG;
                EVSNMcuControl_g_tc_members[0]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
                EVSNMcuControl_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
                EVSNMcuControl_g_tc_members[0]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;
                EVSNMcuControl_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMcuControl_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMcuControl_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuControl_g_tc_members[2]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuControl_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuControl_g_tc_members[2]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuControl_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuControl_g_tc_members[2]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuControl_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[3]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuControl_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[3]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuControl_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[3]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuControl_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[4]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuControl_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[4]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuControl_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[4]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuControl_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[5]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuControl_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[5]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuControl_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuControl_g_tc_members[5]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuControl_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMcuControl_g_tc_members[6]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMcuControl_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMcuControl_g_tc_members[7]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMcuControl_g_tc_members[8]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMcuControl_g_tc_members[8]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMcuControl_g_tc_members[9]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNMcuControl_g_tc_members[9]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNMcuControl_g_tc_members[10]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuControl_g_tc_members[10]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuControl_g_tc_members[10]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuControl_g_tc_members[10]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuControl_g_tc_members[10]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuControl_g_tc_members[10]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;

                EVSNMcuControl_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMcuControl_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMcuControl_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMcuControl *sample;

                static RTIXCdrMemberAccessInfo EVSNMcuControl_g_memberAccessInfos[11] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMcuControl_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMcuControl_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMcuControl);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMcuControl_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->target_speed_rpm() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->clutch_open_request() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->vehicle_speed_kmh() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->max_recu_power_kw() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->max_boost_power_kw() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->message_count() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->recorder_flash_reset() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->clear_errors_request() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->active_discharge_request() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[9].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->requested_mode() - (char *)sample);

                EVSNMcuControl_g_memberAccessInfos[10].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->target_torque_nm() - (char *)sample);

                EVSNMcuControl_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMcuControl_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMcuControl);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMcuControl_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMcuControl_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMcuControl_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMcuControl_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMcuControl >;

                EVSNMcuControl_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMcuControl_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMcuControl_g_typePlugin = 
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

                return &EVSNMcuControl_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMcuControl >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMcuControl >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMcuActualOperation > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMcuActualOperation_g_tc_members[10]=
                {

                    {
                        (char *)"discharge_active",/* Member name */
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
                        (char *)"contactor_open_request",/* Member name */
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
                        (char *)"limits",/* Member name */
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
                        (char *)"minimum_torque_nm",/* Member name */
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
                        (char *)"maximum_torque_nm",/* Member name */
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
                        (char *)"message_count",/* Member name */
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
                        (char *)"actual_mode",/* Member name */
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
                        (char *)"dc_link_voltage_v",/* Member name */
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
                        (char *)"actual_torque_nm",/* Member name */
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
                        (char *)"actual_speed_rpm",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMcuActualOperation_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMcuActualOperation", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        10, /* Number of members */
                        EVSNMcuActualOperation_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMcuActualOperation*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMcuActualOperation_g_tc;
                }

                EVSNMcuActualOperation_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMcuActualOperation_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMcuActualOperation_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMcuActualOperation_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuActualOperation_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuActualOperation_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuActualOperation_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuActualOperation_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuMode>::get().native();
                EVSNMcuActualOperation_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;
                EVSNMcuActualOperation_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuActualOperation_g_tc_members[9]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

                /* Initialize the values for member annotations. */
                EVSNMcuActualOperation_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMcuActualOperation_g_tc_members[0]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMcuActualOperation_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMcuActualOperation_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMcuActualOperation_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuActualOperation_g_tc_members[2]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuActualOperation_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuActualOperation_g_tc_members[2]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuActualOperation_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuActualOperation_g_tc_members[2]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuActualOperation_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[3]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuActualOperation_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[3]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuActualOperation_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[3]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuActualOperation_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[4]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuActualOperation_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[4]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuActualOperation_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[4]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuActualOperation_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuActualOperation_g_tc_members[5]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuActualOperation_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuActualOperation_g_tc_members[5]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuActualOperation_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuActualOperation_g_tc_members[5]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuActualOperation_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_ENUM;
                EVSNMcuActualOperation_g_tc_members[6]._annotations._defaultValue._u.enumerated_value = 0;
                EVSNMcuActualOperation_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuActualOperation_g_tc_members[7]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuActualOperation_g_tc_members[7]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuActualOperation_g_tc_members[7]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuActualOperation_g_tc_members[7]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuActualOperation_g_tc_members[7]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;
                EVSNMcuActualOperation_g_tc_members[8]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[8]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuActualOperation_g_tc_members[8]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[8]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuActualOperation_g_tc_members[8]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuActualOperation_g_tc_members[8]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuActualOperation_g_tc_members[9]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
                EVSNMcuActualOperation_g_tc_members[9]._annotations._defaultValue._u.long_value = 0;
                EVSNMcuActualOperation_g_tc_members[9]._annotations._minValue._d = RTI_XCDR_TK_LONG;
                EVSNMcuActualOperation_g_tc_members[9]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
                EVSNMcuActualOperation_g_tc_members[9]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
                EVSNMcuActualOperation_g_tc_members[9]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;

                EVSNMcuActualOperation_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMcuActualOperation_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMcuActualOperation_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMcuActualOperation *sample;

                static RTIXCdrMemberAccessInfo EVSNMcuActualOperation_g_memberAccessInfos[10] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMcuActualOperation_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMcuActualOperation_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMcuActualOperation);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMcuActualOperation_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->discharge_active() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->contactor_open_request() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->limits() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->minimum_torque_nm() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->maximum_torque_nm() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->message_count() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->actual_mode() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->dc_link_voltage_v() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->actual_torque_nm() - (char *)sample);

                EVSNMcuActualOperation_g_memberAccessInfos[9].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->actual_speed_rpm() - (char *)sample);

                EVSNMcuActualOperation_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMcuActualOperation_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMcuActualOperation);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMcuActualOperation_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMcuActualOperation_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMcuActualOperation_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMcuActualOperation_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMcuActualOperation >;

                EVSNMcuActualOperation_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMcuActualOperation_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMcuActualOperation_g_typePlugin = 
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

                return &EVSNMcuActualOperation_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMcuActualOperation >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMcuActualOperation >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMcuStatusInfo > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMcuStatusInfo_g_tc_members[4]=
                {

                    {
                        (char *)"system_warnings",/* Member name */
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
                        (char *)"status",/* Member name */
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
                        (char *)"errors",/* Member name */
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
                        (char *)"deratings",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMcuStatusInfo_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMcuStatusInfo", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        4, /* Number of members */
                        EVSNMcuStatusInfo_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMcuStatusInfo*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMcuStatusInfo_g_tc;
                }

                EVSNMcuStatusInfo_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMcuStatusInfo_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNMcuStatusInfo_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuStatusInfo_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNMcuStatusInfo_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;

                /* Initialize the values for member annotations. */
                EVSNMcuStatusInfo_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuStatusInfo_g_tc_members[0]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNMcuStatusInfo_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuStatusInfo_g_tc_members[0]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNMcuStatusInfo_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuStatusInfo_g_tc_members[0]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNMcuStatusInfo_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuStatusInfo_g_tc_members[1]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuStatusInfo_g_tc_members[1]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuStatusInfo_g_tc_members[1]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuStatusInfo_g_tc_members[1]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuStatusInfo_g_tc_members[1]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuStatusInfo_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuStatusInfo_g_tc_members[2]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNMcuStatusInfo_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuStatusInfo_g_tc_members[2]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNMcuStatusInfo_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuStatusInfo_g_tc_members[2]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNMcuStatusInfo_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuStatusInfo_g_tc_members[3]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuStatusInfo_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuStatusInfo_g_tc_members[3]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuStatusInfo_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuStatusInfo_g_tc_members[3]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;

                EVSNMcuStatusInfo_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMcuStatusInfo_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMcuStatusInfo_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMcuStatusInfo *sample;

                static RTIXCdrMemberAccessInfo EVSNMcuStatusInfo_g_memberAccessInfos[4] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMcuStatusInfo_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMcuStatusInfo_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMcuStatusInfo);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMcuStatusInfo_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->system_warnings() - (char *)sample);

                EVSNMcuStatusInfo_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->status() - (char *)sample);

                EVSNMcuStatusInfo_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->errors() - (char *)sample);

                EVSNMcuStatusInfo_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->deratings() - (char *)sample);

                EVSNMcuStatusInfo_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMcuStatusInfo_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMcuStatusInfo);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMcuStatusInfo_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMcuStatusInfo_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMcuStatusInfo_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMcuStatusInfo_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMcuStatusInfo >;

                EVSNMcuStatusInfo_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMcuStatusInfo_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMcuStatusInfo_g_typePlugin = 
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

                return &EVSNMcuStatusInfo_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMcuStatusInfo >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMcuStatusInfo >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMcuTemperatures > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMcuTemperatures_g_tc_members[8]=
                {

                    {
                        (char *)"cooling_inlet_c",/* Member name */
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
                        (char *)"stator_minimum_c",/* Member name */
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
                        (char *)"semiconductor_minimum_c",/* Member name */
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
                        (char *)"dc_link_c",/* Member name */
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
                        (char *)"rotor_c",/* Member name */
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
                        (char *)"stator_maximum_c",/* Member name */
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
                        (char *)"semiconductor_maximum_c",/* Member name */
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
                        (char *)"controller_board_c",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMcuTemperatures_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMcuTemperatures", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        8, /* Number of members */
                        EVSNMcuTemperatures_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMcuTemperatures*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMcuTemperatures_g_tc;
                }

                EVSNMcuTemperatures_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMcuTemperatures_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;
                EVSNMcuTemperatures_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_float;

                /* Initialize the values for member annotations. */
                EVSNMcuTemperatures_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[0]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[0]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[0]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[1]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[1]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[1]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[1]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[1]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[2]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[2]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[2]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[3]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[3]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[3]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[4]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[4]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[4]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[5]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[5]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[5]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[6]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[6]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[6]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[6]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[6]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;
                EVSNMcuTemperatures_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[7]._annotations._defaultValue._u.float_value = 0.0f;
                EVSNMcuTemperatures_g_tc_members[7]._annotations._minValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[7]._annotations._minValue._u.float_value = RTIXCdrFloat_MIN;
                EVSNMcuTemperatures_g_tc_members[7]._annotations._maxValue._d = RTI_XCDR_TK_FLOAT;
                EVSNMcuTemperatures_g_tc_members[7]._annotations._maxValue._u.float_value = RTIXCdrFloat_MAX;

                EVSNMcuTemperatures_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMcuTemperatures_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMcuTemperatures_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMcuTemperatures *sample;

                static RTIXCdrMemberAccessInfo EVSNMcuTemperatures_g_memberAccessInfos[8] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMcuTemperatures_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMcuTemperatures_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMcuTemperatures);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMcuTemperatures_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->cooling_inlet_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->stator_minimum_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->semiconductor_minimum_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->dc_link_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->rotor_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->stator_maximum_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->semiconductor_maximum_c() - (char *)sample);

                EVSNMcuTemperatures_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->controller_board_c() - (char *)sample);

                EVSNMcuTemperatures_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMcuTemperatures_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMcuTemperatures);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMcuTemperatures_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMcuTemperatures_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMcuTemperatures_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMcuTemperatures_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMcuTemperatures >;

                EVSNMcuTemperatures_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMcuTemperatures_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMcuTemperatures_g_typePlugin = 
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

                return &EVSNMcuTemperatures_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMcuTemperatures >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMcuTemperatures >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMcuSystemInfo > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMcuSystemInfo_g_tc_members[10]=
                {

                    {
                        (char *)"multiplexor",/* Member name */
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
                        (char *)"mgu_serial_number",/* Member name */
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
                        (char *)"mgu_mds_crc",/* Member name */
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
                        (char *)"mcu_pds_crc",/* Member name */
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
                        (char *)"mcu_kds_crc",/* Member name */
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
                        (char *)"mcu_dds_crc",/* Member name */
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
                        (char *)"mcu_hardware_version",/* Member name */
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
                        (char *)"mcu_serial_number",/* Member name */
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
                        (char *)"mcu_software_version_time",/* Member name */
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
                        (char *)"mcu_software_version_date",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMcuSystemInfo_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMcuSystemInfo", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        10, /* Number of members */
                        EVSNMcuSystemInfo_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMcuSystemInfo*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMcuSystemInfo_g_tc;
                }

                EVSNMcuSystemInfo_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMcuSystemInfo_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuSystemInfo_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuSystemInfo_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;
                EVSNMcuSystemInfo_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;
                EVSNMcuSystemInfo_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;
                EVSNMcuSystemInfo_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;
                EVSNMcuSystemInfo_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuSystemInfo_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNMcuSystemInfo_g_tc_members[8]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ushort;
                EVSNMcuSystemInfo_g_tc_members[9]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;

                /* Initialize the values for member annotations. */
                EVSNMcuSystemInfo_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[0]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuSystemInfo_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[0]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuSystemInfo_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[0]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuSystemInfo_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[1]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuSystemInfo_g_tc_members[1]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[1]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuSystemInfo_g_tc_members[1]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[1]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuSystemInfo_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[2]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuSystemInfo_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[2]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuSystemInfo_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[2]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;
                EVSNMcuSystemInfo_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[3]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuSystemInfo_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[3]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuSystemInfo_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[3]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;
                EVSNMcuSystemInfo_g_tc_members[4]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[4]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuSystemInfo_g_tc_members[4]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[4]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuSystemInfo_g_tc_members[4]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[4]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;
                EVSNMcuSystemInfo_g_tc_members[5]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[5]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuSystemInfo_g_tc_members[5]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[5]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuSystemInfo_g_tc_members[5]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[5]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;
                EVSNMcuSystemInfo_g_tc_members[6]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[6]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuSystemInfo_g_tc_members[6]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[6]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuSystemInfo_g_tc_members[6]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[6]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuSystemInfo_g_tc_members[7]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[7]._annotations._defaultValue._u.octet_value = 0;
                EVSNMcuSystemInfo_g_tc_members[7]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[7]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNMcuSystemInfo_g_tc_members[7]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNMcuSystemInfo_g_tc_members[7]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;
                EVSNMcuSystemInfo_g_tc_members[8]._annotations._defaultValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[8]._annotations._defaultValue._u.ushort_value = 0;
                EVSNMcuSystemInfo_g_tc_members[8]._annotations._minValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[8]._annotations._minValue._u.ushort_value = RTIXCdrUnsignedShort_MIN;
                EVSNMcuSystemInfo_g_tc_members[8]._annotations._maxValue._d = RTI_XCDR_TK_USHORT;
                EVSNMcuSystemInfo_g_tc_members[8]._annotations._maxValue._u.ushort_value = RTIXCdrUnsignedShort_MAX;
                EVSNMcuSystemInfo_g_tc_members[9]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuSystemInfo_g_tc_members[9]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNMcuSystemInfo_g_tc_members[9]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuSystemInfo_g_tc_members[9]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNMcuSystemInfo_g_tc_members[9]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNMcuSystemInfo_g_tc_members[9]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;

                EVSNMcuSystemInfo_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMcuSystemInfo_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMcuSystemInfo_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMcuSystemInfo *sample;

                static RTIXCdrMemberAccessInfo EVSNMcuSystemInfo_g_memberAccessInfos[10] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMcuSystemInfo_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMcuSystemInfo_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMcuSystemInfo);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMcuSystemInfo_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->multiplexor() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mgu_serial_number() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mgu_mds_crc() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_pds_crc() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_kds_crc() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_dds_crc() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_hardware_version() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_serial_number() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[8].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_software_version_time() - (char *)sample);

                EVSNMcuSystemInfo_g_memberAccessInfos[9].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->mcu_software_version_date() - (char *)sample);

                EVSNMcuSystemInfo_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMcuSystemInfo_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMcuSystemInfo);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMcuSystemInfo_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMcuSystemInfo_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMcuSystemInfo_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMcuSystemInfo_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMcuSystemInfo >;

                EVSNMcuSystemInfo_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMcuSystemInfo_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMcuSystemInfo_g_typePlugin = 
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

                return &EVSNMcuSystemInfo_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMcuSystemInfo >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMcuSystemInfo >::get())));
        }

        template<>
        struct native_type_code< ::EVSNMotorStatus > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNMotorStatus_g_tc_members[8]=
                {

                    {
                        (char *)"actual_operation_fresh",/* Member name */
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
                        (char *)"status_info_fresh",/* Member name */
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
                        (char *)"temperatures_fresh",/* Member name */
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
                        (char *)"system_info_fresh",/* Member name */
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
                        (char *)"actual_operation",/* Member name */
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
                        (char *)"status_info",/* Member name */
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
                        (char *)"temperatures",/* Member name */
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
                        (char *)"system_info",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNMotorStatus_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNMotorStatus", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        8, /* Number of members */
                        EVSNMotorStatus_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNMotorStatus*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNMotorStatus_g_tc;
                }

                EVSNMotorStatus_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNMotorStatus_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorStatus_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorStatus_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorStatus_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNMotorStatus_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuActualOperation>::get().native();
                EVSNMotorStatus_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuStatusInfo>::get().native();
                EVSNMotorStatus_g_tc_members[6]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuTemperatures>::get().native();
                EVSNMotorStatus_g_tc_members[7]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::dynamic_type< ::EVSNMcuSystemInfo>::get().native();

                /* Initialize the values for member annotations. */
                EVSNMotorStatus_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorStatus_g_tc_members[0]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorStatus_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorStatus_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorStatus_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorStatus_g_tc_members[2]._annotations._defaultValue._u.boolean_value = 0;
                EVSNMotorStatus_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNMotorStatus_g_tc_members[3]._annotations._defaultValue._u.boolean_value = 0;

                EVSNMotorStatus_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNMotorStatus_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNMotorStatus_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNMotorStatus *sample;

                static RTIXCdrMemberAccessInfo EVSNMotorStatus_g_memberAccessInfos[8] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNMotorStatus_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNMotorStatus_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNMotorStatus);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNMotorStatus_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->actual_operation_fresh() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->status_info_fresh() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->temperatures_fresh() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->system_info_fresh() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->actual_operation() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[5].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->status_info() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[6].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->temperatures() - (char *)sample);

                EVSNMotorStatus_g_memberAccessInfos[7].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->system_info() - (char *)sample);

                EVSNMotorStatus_g_sampleAccessInfo.memberAccessInfos = 
                EVSNMotorStatus_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNMotorStatus);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNMotorStatus_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNMotorStatus_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNMotorStatus_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNMotorStatus_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNMotorStatus >;

                EVSNMotorStatus_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNMotorStatus_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNMotorStatus_g_typePlugin = 
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

                return &EVSNMotorStatus_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNMotorStatus >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNMotorStatus >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNMotorRequest >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMotorRequestPlugin_new,
                ::EVSNMotorRequestPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMotorRequest >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMotorRequest& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMotorRequestPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMotorRequestPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMotorRequest >::from_cdr_buffer(::EVSNMotorRequest& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMotorRequestPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMotorRequest from cdr buffer");
        }

        void topic_type_support< ::EVSNMotorRequest >::reset_sample(::EVSNMotorRequest& sample) 
        {
            sample.enable(0);
            sample.requested_mode(EVSNMcuMode::EVSN_MCU_STANDBY);
            sample.target_torque_nm(0.0f);
            sample.vehicle_speed_kmh(0.0f);
            sample.max_recu_power_kw(0);
            sample.max_boost_power_kw(0);
        }

        void topic_type_support< ::EVSNMotorRequest >::allocate_sample(::EVSNMotorRequest& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.requested_mode(),  -1, -1);
        }
        void topic_type_support< ::EVSNMcuControl >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMcuControlPlugin_new,
                ::EVSNMcuControlPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMcuControl >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMcuControl& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMcuControlPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMcuControlPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMcuControl >::from_cdr_buffer(::EVSNMcuControl& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMcuControlPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMcuControl from cdr buffer");
        }

        void topic_type_support< ::EVSNMcuControl >::reset_sample(::EVSNMcuControl& sample) 
        {
            sample.target_speed_rpm(0);
            sample.clutch_open_request(0);
            sample.vehicle_speed_kmh(0.0f);
            sample.max_recu_power_kw(0);
            sample.max_boost_power_kw(0);
            sample.message_count(0);
            sample.recorder_flash_reset(0);
            sample.clear_errors_request(0);
            sample.active_discharge_request(0);
            sample.requested_mode(EVSNMcuMode::EVSN_MCU_STANDBY);
            sample.target_torque_nm(0.0f);
        }

        void topic_type_support< ::EVSNMcuControl >::allocate_sample(::EVSNMcuControl& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.requested_mode(),  -1, -1);
        }
        void topic_type_support< ::EVSNMcuActualOperation >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMcuActualOperationPlugin_new,
                ::EVSNMcuActualOperationPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMcuActualOperation >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMcuActualOperation& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMcuActualOperationPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMcuActualOperationPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMcuActualOperation >::from_cdr_buffer(::EVSNMcuActualOperation& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMcuActualOperationPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMcuActualOperation from cdr buffer");
        }

        void topic_type_support< ::EVSNMcuActualOperation >::reset_sample(::EVSNMcuActualOperation& sample) 
        {
            sample.discharge_active(0);
            sample.contactor_open_request(0);
            sample.limits(0);
            sample.minimum_torque_nm(0.0f);
            sample.maximum_torque_nm(0.0f);
            sample.message_count(0);
            sample.actual_mode(EVSNMcuMode::EVSN_MCU_STANDBY);
            sample.dc_link_voltage_v(0);
            sample.actual_torque_nm(0.0f);
            sample.actual_speed_rpm(0);
        }

        void topic_type_support< ::EVSNMcuActualOperation >::allocate_sample(::EVSNMcuActualOperation& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.actual_mode(),  -1, -1);
        }
        void topic_type_support< ::EVSNMcuStatusInfo >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMcuStatusInfoPlugin_new,
                ::EVSNMcuStatusInfoPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMcuStatusInfo >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMcuStatusInfo& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMcuStatusInfoPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMcuStatusInfoPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMcuStatusInfo >::from_cdr_buffer(::EVSNMcuStatusInfo& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMcuStatusInfoPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMcuStatusInfo from cdr buffer");
        }

        void topic_type_support< ::EVSNMcuStatusInfo >::reset_sample(::EVSNMcuStatusInfo& sample) 
        {
            sample.system_warnings(0u);
            sample.status(0);
            sample.errors(0u);
            sample.deratings(0);
        }

        void topic_type_support< ::EVSNMcuStatusInfo >::allocate_sample(::EVSNMcuStatusInfo& sample, int, int) 
        {
            RTIOsapiUtility_unusedParameter(sample);
        }
        void topic_type_support< ::EVSNMcuTemperatures >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMcuTemperaturesPlugin_new,
                ::EVSNMcuTemperaturesPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMcuTemperatures >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMcuTemperatures& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMcuTemperaturesPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMcuTemperaturesPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMcuTemperatures >::from_cdr_buffer(::EVSNMcuTemperatures& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMcuTemperaturesPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMcuTemperatures from cdr buffer");
        }

        void topic_type_support< ::EVSNMcuTemperatures >::reset_sample(::EVSNMcuTemperatures& sample) 
        {
            sample.cooling_inlet_c(0.0f);
            sample.stator_minimum_c(0.0f);
            sample.semiconductor_minimum_c(0.0f);
            sample.dc_link_c(0.0f);
            sample.rotor_c(0.0f);
            sample.stator_maximum_c(0.0f);
            sample.semiconductor_maximum_c(0.0f);
            sample.controller_board_c(0.0f);
        }

        void topic_type_support< ::EVSNMcuTemperatures >::allocate_sample(::EVSNMcuTemperatures& sample, int, int) 
        {
            RTIOsapiUtility_unusedParameter(sample);
        }
        void topic_type_support< ::EVSNMcuSystemInfo >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMcuSystemInfoPlugin_new,
                ::EVSNMcuSystemInfoPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMcuSystemInfo >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMcuSystemInfo& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMcuSystemInfoPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMcuSystemInfoPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMcuSystemInfo >::from_cdr_buffer(::EVSNMcuSystemInfo& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMcuSystemInfoPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMcuSystemInfo from cdr buffer");
        }

        void topic_type_support< ::EVSNMcuSystemInfo >::reset_sample(::EVSNMcuSystemInfo& sample) 
        {
            sample.multiplexor(0);
            sample.mgu_serial_number(0);
            sample.mgu_mds_crc(0);
            sample.mcu_pds_crc(0);
            sample.mcu_kds_crc(0);
            sample.mcu_dds_crc(0);
            sample.mcu_hardware_version(0);
            sample.mcu_serial_number(0);
            sample.mcu_software_version_time(0);
            sample.mcu_software_version_date(0u);
        }

        void topic_type_support< ::EVSNMcuSystemInfo >::allocate_sample(::EVSNMcuSystemInfo& sample, int, int) 
        {
            RTIOsapiUtility_unusedParameter(sample);
        }
        void topic_type_support< ::EVSNMotorStatus >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNMotorStatusPlugin_new,
                ::EVSNMotorStatusPlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNMotorStatus >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNMotorStatus& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNMotorStatusPlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNMotorStatusPlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNMotorStatus >::from_cdr_buffer(::EVSNMotorStatus& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNMotorStatusPlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNMotorStatus from cdr buffer");
        }

        void topic_type_support< ::EVSNMotorStatus >::reset_sample(::EVSNMotorStatus& sample) 
        {
            sample.actual_operation_fresh(0);
            sample.status_info_fresh(0);
            sample.temperatures_fresh(0);
            sample.system_info_fresh(0);
            ::rti::topic::reset_sample(sample.actual_operation());
            ::rti::topic::reset_sample(sample.status_info());
            ::rti::topic::reset_sample(sample.temperatures());
            ::rti::topic::reset_sample(sample.system_info());
        }

        void topic_type_support< ::EVSNMotorStatus >::allocate_sample(::EVSNMotorStatus& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.actual_operation(),  -1, -1);
            ::rti::topic::allocate_sample(sample.status_info(),  -1, -1);
            ::rti::topic::allocate_sample(sample.temperatures(),  -1, -1);
            ::rti::topic::allocate_sample(sample.system_info(),  -1, -1);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
