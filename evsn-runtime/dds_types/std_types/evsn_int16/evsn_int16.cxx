

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_int16.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#include <iosfwd>
#include <iomanip>
#include <atomic>

#ifndef NDDS_STANDALONE_TYPE
#include "rti/topic/cdr/Serialization.hpp"
#include "evsn_int16Plugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_int16.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- EVSNInt16: 

EVSNInt16::EVSNInt16() :
    m_data_ (0)  {

}   

EVSNInt16::EVSNInt16 (int16_t data_):
    m_data_(data_) {
}

void EVSNInt16::swap(EVSNInt16& other_)  noexcept 
{
    using std::swap;
    swap(m_data_, other_.m_data_);
}  

bool EVSNInt16::operator == (const EVSNInt16& other_) const {
    if (m_data_ != other_.m_data_) {
        return false;
    }
    return true;
}

bool EVSNInt16::operator != (const EVSNInt16& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNInt16& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "data: " << sample.data ();
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
        struct native_type_code< ::EVSNInt16 > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode_Member EVSNInt16_g_tc_members[1]=
                {

                    {
                        (char *)"data",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNInt16_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNInt16", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        1, /* Number of members */
                        EVSNInt16_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNInt16*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNInt16_g_tc;
                }

                EVSNInt16_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNInt16_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_short;

                /* Initialize the values for member annotations. */
                EVSNInt16_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_SHORT;
                EVSNInt16_g_tc_members[0]._annotations._defaultValue._u.short_value = 0;
                EVSNInt16_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_SHORT;
                EVSNInt16_g_tc_members[0]._annotations._minValue._u.short_value = RTIXCdrShort_MIN;
                EVSNInt16_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_SHORT;
                EVSNInt16_g_tc_members[0]._annotations._maxValue._u.short_value = RTIXCdrShort_MAX;

                EVSNInt16_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNInt16_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNInt16_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNInt16 *sample;

                static RTIXCdrMemberAccessInfo EVSNInt16_g_memberAccessInfos[1] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNInt16_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNInt16_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNInt16);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNInt16_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->data() - (char *)sample);

                EVSNInt16_g_sampleAccessInfo.memberAccessInfos = 
                EVSNInt16_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNInt16);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNInt16_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNInt16_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNInt16_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNInt16_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNInt16 >;

                EVSNInt16_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNInt16_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNInt16_g_typePlugin = 
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

                return &EVSNInt16_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNInt16 >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNInt16 >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNInt16 >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNInt16Plugin_new,
                ::EVSNInt16Plugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNInt16 >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNInt16& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNInt16Plugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNInt16Plugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNInt16 >::from_cdr_buffer(::EVSNInt16& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNInt16Plugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNInt16 from cdr buffer");
        }

        void topic_type_support< ::EVSNInt16 >::reset_sample(::EVSNInt16& sample) 
        {
            sample.data(0);
        }

        void topic_type_support< ::EVSNInt16 >::allocate_sample(::EVSNInt16& sample, int, int) 
        {
            RTIOsapiUtility_unusedParameter(sample);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
