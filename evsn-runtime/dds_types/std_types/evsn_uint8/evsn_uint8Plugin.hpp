

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_uint8.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_uint8Plugin_1284719397_h
#define evsn_uint8Plugin_1284719397_h

#include "evsn_uint8.hpp"

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

#define EVSNUint8Plugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNUint8Plugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNUint8Plugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNUint8Plugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNUint8Plugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNUint8*
EVSNUint8PluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNUint8*
EVSNUint8PluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNUint8*
EVSNUint8PluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNUint8PluginSupport_copy_data(
    EVSNUint8 *out,
    const EVSNUint8 *in);

NDDSUSERDllExport extern void 
EVSNUint8PluginSupport_destroy_data_w_params(
    EVSNUint8 *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNUint8PluginSupport_destroy_data_ex(
    EVSNUint8 *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNUint8PluginSupport_destroy_data(
    EVSNUint8 *sample);

NDDSUSERDllExport extern void 
EVSNUint8PluginSupport_print_data(
    const EVSNUint8 *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNUint8Plugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNUint8Plugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNUint8Plugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNUint8Plugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNUint8Plugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint8 *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNUint8Plugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint8 *out,
    const EVSNUint8 *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNUint8Plugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNUint8 *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNUint8Plugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint8 **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNUint8Plugin_deserialize_from_cdr_buffer(
    EVSNUint8 *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNUint8Plugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNUint8Plugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNUint8Plugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNUint8Plugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNUint8Plugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint8 ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNUint8Plugin_new(void);

NDDSUSERDllExport extern void
EVSNUint8Plugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_uint8Plugin_1284719397_h */

