

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_fdcan.idl
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
#include "evsn_fdcanPlugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_fdcan.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- EVSNFDCanFrame: 

EVSNFDCanFrame::EVSNFDCanFrame() :
    m_id_ (0u)  {

}   

EVSNFDCanFrame::EVSNFDCanFrame (uint32_t id_,const ::rti::core::bounded_sequence< uint8_t, 64L >& data_):
    m_id_(id_), 
    m_data_(data_) {
}

void EVSNFDCanFrame::swap(EVSNFDCanFrame& other_)  noexcept 
{
    using std::swap;
    swap(m_id_, other_.m_id_);
    swap(m_data_, other_.m_data_);
}  

bool EVSNFDCanFrame::operator == (const EVSNFDCanFrame& other_) const {
    if (m_id_ != other_.m_id_) {
        return false;
    }
    if (m_data_ != other_.m_data_) {
        return false;
    }
    return true;
}

bool EVSNFDCanFrame::operator != (const EVSNFDCanFrame& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNFDCanFrame& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "id: " << sample.id ()<<", ";
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
        struct native_type_code< ::EVSNFDCanFrame > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNFDCanFrame_g_tc_data_sequence;

                static DDS_TypeCode_Member EVSNFDCanFrame_g_tc_members[2]=
                {

                    {
                        (char *)"id",/* Member name */
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
                        (char *)"data",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNFDCanFrame_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNFDCanFrame", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        2, /* Number of members */
                        EVSNFDCanFrame_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNFDCanFrame*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNFDCanFrame_g_tc;
                }

                EVSNFDCanFrame_g_tc_data_sequence = initialize_sequence_typecode< ::rti::core::bounded_sequence< uint8_t, 64L > >((64L));

                EVSNFDCanFrame_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNFDCanFrame_g_tc_data_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNFDCanFrame_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNFDCanFrame_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)& EVSNFDCanFrame_g_tc_data_sequence;

                /* Initialize the values for member annotations. */
                EVSNFDCanFrame_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNFDCanFrame_g_tc_members[0]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNFDCanFrame_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNFDCanFrame_g_tc_members[0]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNFDCanFrame_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNFDCanFrame_g_tc_members[0]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;

                EVSNFDCanFrame_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNFDCanFrame_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNFDCanFrame_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNFDCanFrame *sample;

                static RTIXCdrMemberAccessInfo EVSNFDCanFrame_g_memberAccessInfos[2] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNFDCanFrame_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNFDCanFrame_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNFDCanFrame);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNFDCanFrame_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->id() - (char *)sample);

                EVSNFDCanFrame_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->data() - (char *)sample);

                EVSNFDCanFrame_g_sampleAccessInfo.memberAccessInfos = 
                EVSNFDCanFrame_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNFDCanFrame);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNFDCanFrame_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNFDCanFrame_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNFDCanFrame_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNFDCanFrame_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNFDCanFrame >;

                EVSNFDCanFrame_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNFDCanFrame_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNFDCanFrame_g_typePlugin = 
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

                return &EVSNFDCanFrame_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNFDCanFrame >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNFDCanFrame >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNFDCanFrame >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNFDCanFramePlugin_new,
                ::EVSNFDCanFramePlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNFDCanFrame >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNFDCanFrame& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNFDCanFramePlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNFDCanFramePlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNFDCanFrame >::from_cdr_buffer(::EVSNFDCanFrame& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNFDCanFramePlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNFDCanFrame from cdr buffer");
        }

        void topic_type_support< ::EVSNFDCanFrame >::reset_sample(::EVSNFDCanFrame& sample) 
        {
            sample.id(0u);
            ::rti::topic::reset_sample(sample.data());
        }

        void topic_type_support< ::EVSNFDCanFrame >::allocate_sample(::EVSNFDCanFrame& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.data(),  64L, -1);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
