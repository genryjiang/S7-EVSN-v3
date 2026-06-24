

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_can20.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_can20_799635832_hpp
#define evsn_can20_799635832_hpp

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

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
// On Windows, dll-export template instantiations of standard types used by
// other dll-exported types
template class NDDSUSERDllExport std::allocator< uint8_t >;
template class NDDSUSERDllExport std::vector< uint8_t >;
#endif
class NDDSUSERDllExport EVSNCan20Frame {
  public:

    EVSNCan20Frame();

    EVSNCan20Frame(uint32_t id_,bool extended_id_,bool remote_frame_,uint8_t dlc_,const ::rti::core::bounded_sequence< uint8_t, 8L >& data_);

    uint32_t& id() noexcept {
        return m_id_;
    }

    const uint32_t& id() const noexcept {
        return m_id_;
    }

    void id(uint32_t value) {

        m_id_ = value;
    }

    bool& extended_id() noexcept {
        return m_extended_id_;
    }

    const bool& extended_id() const noexcept {
        return m_extended_id_;
    }

    void extended_id(bool value) {

        m_extended_id_ = value;
    }

    bool& remote_frame() noexcept {
        return m_remote_frame_;
    }

    const bool& remote_frame() const noexcept {
        return m_remote_frame_;
    }

    void remote_frame(bool value) {

        m_remote_frame_ = value;
    }

    uint8_t& dlc() noexcept {
        return m_dlc_;
    }

    const uint8_t& dlc() const noexcept {
        return m_dlc_;
    }

    void dlc(uint8_t value) {

        m_dlc_ = value;
    }

    ::rti::core::bounded_sequence< uint8_t, 8L >& data() noexcept {
        return m_data_;
    }

    const ::rti::core::bounded_sequence< uint8_t, 8L >& data() const noexcept {
        return m_data_;
    }

    void data(const ::rti::core::bounded_sequence< uint8_t, 8L >& value) {

        m_data_ = value;
    }

    void data(::rti::core::bounded_sequence< uint8_t, 8L >&& value) {
        m_data_ = std::move(value);
    }
    bool operator == (const EVSNCan20Frame& other_) const;
    bool operator != (const EVSNCan20Frame& other_) const;

    void swap(EVSNCan20Frame& other_) noexcept ;

  private:

    uint32_t m_id_;
    bool m_extended_id_;
    bool m_remote_frame_;
    uint8_t m_dlc_;
    ::rti::core::bounded_sequence< uint8_t, 8L > m_data_;

};

inline void swap(EVSNCan20Frame& a, EVSNCan20Frame& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNCan20Frame& sample);

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
        struct topic_type_name< ::EVSNCan20Frame > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNCan20Frame";
            }
        };

        template<>
        struct is_topic_type< ::EVSNCan20Frame > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNCan20Frame > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNCan20Frame& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNCan20Frame& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNCan20Frame& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNCan20Frame& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
    }
}

namespace rti { 
    namespace topic {

        template<>
        struct dynamic_type< ::EVSNCan20Frame > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNCan20Frame > {
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

#endif // evsn_can20_799635832_hpp

