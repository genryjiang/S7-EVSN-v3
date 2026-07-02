

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_pedal.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_pedal_1725250437_hpp
#define evsn_pedal_1725250437_hpp

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

class NDDSUSERDllExport EVSNPedalState {
  public:

    EVSNPedalState();

    EVSNPedalState(bool accelerator_valid_,bool brake_status_valid_,bool input_fresh_,uint64_t input_received_at_ms_,uint32_t input_age_ms_,float accelerator_percent_,float accelerator_scalar_,float throttle_channel_1_v_,float throttle_channel_2_v_,float throttle_channel_1_percent_,float throttle_channel_2_percent_,bool brake_light_request_,uint32_t source_can_id_,uint32_t diagnostics_,uint32_t sample_counter_);

    bool& accelerator_valid() noexcept {
        return m_accelerator_valid_;
    }

    const bool& accelerator_valid() const noexcept {
        return m_accelerator_valid_;
    }

    void accelerator_valid(bool value) {

        m_accelerator_valid_ = value;
    }

    bool& brake_status_valid() noexcept {
        return m_brake_status_valid_;
    }

    const bool& brake_status_valid() const noexcept {
        return m_brake_status_valid_;
    }

    void brake_status_valid(bool value) {

        m_brake_status_valid_ = value;
    }

    bool& input_fresh() noexcept {
        return m_input_fresh_;
    }

    const bool& input_fresh() const noexcept {
        return m_input_fresh_;
    }

    void input_fresh(bool value) {

        m_input_fresh_ = value;
    }

    uint64_t& input_received_at_ms() noexcept {
        return m_input_received_at_ms_;
    }

    const uint64_t& input_received_at_ms() const noexcept {
        return m_input_received_at_ms_;
    }

    void input_received_at_ms(uint64_t value) {

        m_input_received_at_ms_ = value;
    }

    uint32_t& input_age_ms() noexcept {
        return m_input_age_ms_;
    }

    const uint32_t& input_age_ms() const noexcept {
        return m_input_age_ms_;
    }

    void input_age_ms(uint32_t value) {

        m_input_age_ms_ = value;
    }

    float& accelerator_percent() noexcept {
        return m_accelerator_percent_;
    }

    const float& accelerator_percent() const noexcept {
        return m_accelerator_percent_;
    }

    void accelerator_percent(float value) {

        m_accelerator_percent_ = value;
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

    float& throttle_channel_1_v() noexcept {
        return m_throttle_channel_1_v_;
    }

    const float& throttle_channel_1_v() const noexcept {
        return m_throttle_channel_1_v_;
    }

    void throttle_channel_1_v(float value) {

        m_throttle_channel_1_v_ = value;
    }

    float& throttle_channel_2_v() noexcept {
        return m_throttle_channel_2_v_;
    }

    const float& throttle_channel_2_v() const noexcept {
        return m_throttle_channel_2_v_;
    }

    void throttle_channel_2_v(float value) {

        m_throttle_channel_2_v_ = value;
    }

    float& throttle_channel_1_percent() noexcept {
        return m_throttle_channel_1_percent_;
    }

    const float& throttle_channel_1_percent() const noexcept {
        return m_throttle_channel_1_percent_;
    }

    void throttle_channel_1_percent(float value) {

        m_throttle_channel_1_percent_ = value;
    }

    float& throttle_channel_2_percent() noexcept {
        return m_throttle_channel_2_percent_;
    }

    const float& throttle_channel_2_percent() const noexcept {
        return m_throttle_channel_2_percent_;
    }

    void throttle_channel_2_percent(float value) {

        m_throttle_channel_2_percent_ = value;
    }

    bool& brake_light_request() noexcept {
        return m_brake_light_request_;
    }

    const bool& brake_light_request() const noexcept {
        return m_brake_light_request_;
    }

    void brake_light_request(bool value) {

        m_brake_light_request_ = value;
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

    bool operator == (const EVSNPedalState& other_) const;
    bool operator != (const EVSNPedalState& other_) const;

    void swap(EVSNPedalState& other_) noexcept ;

  private:

    bool m_accelerator_valid_;
    bool m_brake_status_valid_;
    bool m_input_fresh_;
    uint64_t m_input_received_at_ms_;
    uint32_t m_input_age_ms_;
    float m_accelerator_percent_;
    float m_accelerator_scalar_;
    float m_throttle_channel_1_v_;
    float m_throttle_channel_2_v_;
    float m_throttle_channel_1_percent_;
    float m_throttle_channel_2_percent_;
    bool m_brake_light_request_;
    uint32_t m_source_can_id_;
    uint32_t m_diagnostics_;
    uint32_t m_sample_counter_;

};

inline void swap(EVSNPedalState& a, EVSNPedalState& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNPedalState& sample);

#ifdef NDDS_STANDALONE_TYPE
namespace rti { 
    namespace topic {
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
        struct topic_type_name< ::EVSNPedalState > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNPedalState";
            }
        };

        template<>
        struct is_topic_type< ::EVSNPedalState > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNPedalState > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNPedalState& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNPedalState& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNPedalState& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNPedalState& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
    }
}

namespace rti { 
    namespace topic {

        template<>
        struct dynamic_type< ::EVSNPedalState > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNPedalState > {
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

#endif // evsn_pedal_1725250437_hpp

