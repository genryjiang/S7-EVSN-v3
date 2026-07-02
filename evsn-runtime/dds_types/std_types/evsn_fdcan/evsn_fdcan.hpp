

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_fdcan.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_fdcan_835925882_hpp
#define evsn_fdcan_835925882_hpp

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
class NDDSUSERDllExport EVSNFDCanFrame {
  public:

    EVSNFDCanFrame();

    EVSNFDCanFrame(uint32_t id_,const ::rti::core::bounded_sequence< uint8_t, 64L >& data_);

    uint32_t& id() noexcept {
        return m_id_;
    }

    const uint32_t& id() const noexcept {
        return m_id_;
    }

    void id(uint32_t value) {

        m_id_ = value;
    }

    ::rti::core::bounded_sequence< uint8_t, 64L >& data() noexcept {
        return m_data_;
    }

    const ::rti::core::bounded_sequence< uint8_t, 64L >& data() const noexcept {
        return m_data_;
    }

    void data(const ::rti::core::bounded_sequence< uint8_t, 64L >& value) {

        m_data_ = value;
    }

    void data(::rti::core::bounded_sequence< uint8_t, 64L >&& value) {
        m_data_ = std::move(value);
    }
    bool operator == (const EVSNFDCanFrame& other_) const;
    bool operator != (const EVSNFDCanFrame& other_) const;

    void swap(EVSNFDCanFrame& other_) noexcept ;

  private:

    uint32_t m_id_;
    ::rti::core::bounded_sequence< uint8_t, 64L > m_data_;

};

inline void swap(EVSNFDCanFrame& a, EVSNFDCanFrame& b)  noexcept 
{
    a.swap(b);
}

NDDSUSERDllExport std::ostream& operator<<(std::ostream& o, const EVSNFDCanFrame& sample);

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
        struct topic_type_name< ::EVSNFDCanFrame > {
            NDDSUSERDllExport static std::string value() {
                return "EVSNFDCanFrame";
            }
        };

        template<>
        struct is_topic_type< ::EVSNFDCanFrame > : public ::dds::core::true_type {};

        template<>
        struct topic_type_support< ::EVSNFDCanFrame > {
            NDDSUSERDllExport 
            static void register_type(
                ::dds::domain::DomainParticipant& participant,
                const std::string & type_name);

            NDDSUSERDllExport 
            static std::vector<char>& to_cdr_buffer(
                std::vector<char>& buffer, 
                const ::EVSNFDCanFrame& sample,
                ::dds::core::policy::DataRepresentationId representation 
                = ::dds::core::policy::DataRepresentation::auto_id());

            NDDSUSERDllExport 
            static void from_cdr_buffer(::EVSNFDCanFrame& sample, const std::vector<char>& buffer);
            NDDSUSERDllExport 
            static void reset_sample(::EVSNFDCanFrame& sample);

            NDDSUSERDllExport 
            static void allocate_sample(::EVSNFDCanFrame& sample, int, int);

            static const ::rti::topic::TypePluginKind::type type_plugin_kind = 
            ::rti::topic::TypePluginKind::STL;
        };
    }
}

namespace rti { 
    namespace topic {

        template<>
        struct dynamic_type< ::EVSNFDCanFrame > {
            typedef ::dds::core::xtypes::StructType type;
            NDDSUSERDllExport static const ::dds::core::xtypes::StructType& get();
        };

        template <>
        struct extensibility< ::EVSNFDCanFrame > {
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

#endif // evsn_fdcan_835925882_hpp

