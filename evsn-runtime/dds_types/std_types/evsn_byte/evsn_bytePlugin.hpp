

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_byte.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_bytePlugin_1098054350_h
#define evsn_bytePlugin_1098054350_h

#include "evsn_byte.hpp"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#define EVSNBytePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNBytePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNBytePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNBytePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNBytePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNByte*
EVSNBytePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNByte*
EVSNBytePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNByte*
EVSNBytePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNBytePluginSupport_copy_data(
    EVSNByte *out,
    const EVSNByte *in);

NDDSUSERDllExport extern void 
EVSNBytePluginSupport_destroy_data_w_params(
    EVSNByte *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNBytePluginSupport_destroy_data_ex(
    EVSNByte *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNBytePluginSupport_destroy_data(
    EVSNByte *sample);

NDDSUSERDllExport extern void 
EVSNBytePluginSupport_print_data(
    const EVSNByte *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNBytePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNBytePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNBytePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNBytePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNBytePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNByte *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNBytePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNByte *out,
    const EVSNByte *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNBytePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNByte *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNBytePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNByte **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNBytePlugin_deserialize_from_cdr_buffer(
    EVSNByte *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNBytePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNBytePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNBytePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNBytePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNBytePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNByte ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNBytePlugin_new(void);

NDDSUSERDllExport extern void
EVSNBytePlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_bytePlugin_1098054350_h */

