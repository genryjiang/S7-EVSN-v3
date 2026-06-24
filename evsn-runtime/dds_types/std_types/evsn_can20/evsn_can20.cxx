

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_can20.idl
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
#include "evsn_can20Plugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "evsn_can20.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- EVSNCan20Frame: 

EVSNCan20Frame::EVSNCan20Frame() :
    m_id_ (0u) ,
    m_extended_id_ (0) ,
    m_remote_frame_ (0) ,
    m_dlc_ (0)  {

}   

EVSNCan20Frame::EVSNCan20Frame (uint32_t id_,bool extended_id_,bool remote_frame_,uint8_t dlc_,const ::rti::core::bounded_sequence< uint8_t, 8L >& data_):
    m_id_(id_), 
    m_extended_id_(extended_id_), 
    m_remote_frame_(remote_frame_), 
    m_dlc_(dlc_), 
    m_data_(data_) {
}

void EVSNCan20Frame::swap(EVSNCan20Frame& other_)  noexcept 
{
    using std::swap;
    swap(m_id_, other_.m_id_);
    swap(m_extended_id_, other_.m_extended_id_);
    swap(m_remote_frame_, other_.m_remote_frame_);
    swap(m_dlc_, other_.m_dlc_);
    swap(m_data_, other_.m_data_);
}  

bool EVSNCan20Frame::operator == (const EVSNCan20Frame& other_) const {
    if (m_id_ != other_.m_id_) {
        return false;
    }
    if (m_extended_id_ != other_.m_extended_id_) {
        return false;
    }
    if (m_remote_frame_ != other_.m_remote_frame_) {
        return false;
    }
    if (m_dlc_ != other_.m_dlc_) {
        return false;
    }
    if (m_data_ != other_.m_data_) {
        return false;
    }
    return true;
}

bool EVSNCan20Frame::operator != (const EVSNCan20Frame& other_) const {
    return !this->operator ==(other_);
}

std::ostream& operator << (std::ostream& o,const EVSNCan20Frame& sample)
{
    ::rti::util::StreamFlagSaver flag_saver (o);
    o <<"[";
    o << "id: " << sample.id ()<<", ";
    o << "extended_id: " << sample.extended_id ()<<", ";
    o << "remote_frame: " << sample.remote_frame ()<<", ";
    o << "dlc: " << (int) sample.dlc ()<<", ";
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
        struct native_type_code< ::EVSNCan20Frame > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode EVSNCan20Frame_g_tc_data_sequence;

                static DDS_TypeCode_Member EVSNCan20Frame_g_tc_members[5]=
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
                        (char *)"extended_id",/* Member name */
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
                        (char *)"remote_frame",/* Member name */
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
                        (char *)"dlc",/* Member name */
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
                        (char *)"data",/* Member name */
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
                    }
                };

                static DDS_TypeCode EVSNCan20Frame_g_tc =
                {{
                        DDS_TK_STRUCT, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"EVSNCan20Frame", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        5, /* Number of members */
                        EVSNCan20Frame_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for EVSNCan20Frame*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &EVSNCan20Frame_g_tc;
                }

                EVSNCan20Frame_g_tc_data_sequence = initialize_sequence_typecode< ::rti::core::bounded_sequence< uint8_t, 8L > >((8L));

                EVSNCan20Frame_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                EVSNCan20Frame_g_tc_data_sequence._data._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNCan20Frame_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
                EVSNCan20Frame_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNCan20Frame_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&::rti::topic::interpreter::initialize_bool_typecode();
                EVSNCan20Frame_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_octet;
                EVSNCan20Frame_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)& EVSNCan20Frame_g_tc_data_sequence;

                /* Initialize the values for member annotations. */
                EVSNCan20Frame_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_ULONG;
                EVSNCan20Frame_g_tc_members[0]._annotations._defaultValue._u.ulong_value = 0u;
                EVSNCan20Frame_g_tc_members[0]._annotations._minValue._d = RTI_XCDR_TK_ULONG;
                EVSNCan20Frame_g_tc_members[0]._annotations._minValue._u.ulong_value = RTIXCdrUnsignedLong_MIN;
                EVSNCan20Frame_g_tc_members[0]._annotations._maxValue._d = RTI_XCDR_TK_ULONG;
                EVSNCan20Frame_g_tc_members[0]._annotations._maxValue._u.ulong_value = RTIXCdrUnsignedLong_MAX;
                EVSNCan20Frame_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNCan20Frame_g_tc_members[1]._annotations._defaultValue._u.boolean_value = 0;
                EVSNCan20Frame_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_BOOLEAN;
                EVSNCan20Frame_g_tc_members[2]._annotations._defaultValue._u.boolean_value = 0;
                EVSNCan20Frame_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_OCTET;
                EVSNCan20Frame_g_tc_members[3]._annotations._defaultValue._u.octet_value = 0;
                EVSNCan20Frame_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_OCTET;
                EVSNCan20Frame_g_tc_members[3]._annotations._minValue._u.octet_value = RTIXCdrOctet_MIN;
                EVSNCan20Frame_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_OCTET;
                EVSNCan20Frame_g_tc_members[3]._annotations._maxValue._u.octet_value = RTIXCdrOctet_MAX;

                EVSNCan20Frame_g_tc._data._sampleAccessInfo = sample_access_info();
                EVSNCan20Frame_g_tc._data._typePlugin = type_plugin_info();    

                is_initialized.store(true, std::memory_order_release);

                return &EVSNCan20Frame_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                ::EVSNCan20Frame *sample;

                static RTIXCdrMemberAccessInfo EVSNCan20Frame_g_memberAccessInfos[5] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo EVSNCan20Frame_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &EVSNCan20Frame_g_sampleAccessInfo;
                }

                RTIXCdrHeap_allocateStruct(
                    &sample, 
                    ::EVSNCan20Frame);
                if (sample == NULL) {
                    return NULL;
                }

                EVSNCan20Frame_g_memberAccessInfos[0].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->id() - (char *)sample);

                EVSNCan20Frame_g_memberAccessInfos[1].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->extended_id() - (char *)sample);

                EVSNCan20Frame_g_memberAccessInfos[2].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->remote_frame() - (char *)sample);

                EVSNCan20Frame_g_memberAccessInfos[3].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->dlc() - (char *)sample);

                EVSNCan20Frame_g_memberAccessInfos[4].bindingMemberValueOffset[0] = 
                (RTIXCdrUnsignedLong) ((char *)&sample->data() - (char *)sample);

                EVSNCan20Frame_g_sampleAccessInfo.memberAccessInfos = 
                EVSNCan20Frame_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::EVSNCan20Frame);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        EVSNCan20Frame_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        EVSNCan20Frame_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                EVSNCan20Frame_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_TRUE;

                EVSNCan20Frame_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::EVSNCan20Frame >;

                EVSNCan20Frame_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_CPP_11_STL ;

                RTIXCdrHeap_freeStruct(sample);
                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &EVSNCan20Frame_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin EVSNCan20Frame_g_typePlugin = 
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

                return &EVSNCan20Frame_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::EVSNCan20Frame >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::EVSNCan20Frame >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::EVSNCan20Frame >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::EVSNCan20FramePlugin_new,
                ::EVSNCan20FramePlugin_delete);
        }

        std::vector<char>& topic_type_support< ::EVSNCan20Frame >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::EVSNCan20Frame& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = EVSNCan20FramePlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = EVSNCan20FramePlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::EVSNCan20Frame >::from_cdr_buffer(::EVSNCan20Frame& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = EVSNCan20FramePlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::EVSNCan20Frame from cdr buffer");
        }

        void topic_type_support< ::EVSNCan20Frame >::reset_sample(::EVSNCan20Frame& sample) 
        {
            sample.id(0u);
            sample.extended_id(0);
            sample.remote_frame(0);
            sample.dlc(0);
            ::rti::topic::reset_sample(sample.data());
        }

        void topic_type_support< ::EVSNCan20Frame >::allocate_sample(::EVSNCan20Frame& sample, int, int) 
        {
            ::rti::topic::allocate_sample(sample.data(),  8L, -1);
        }
    }
}  

#endif // NDDS_STANDALONE_TYPE
