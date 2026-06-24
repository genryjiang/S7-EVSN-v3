

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_uint16.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_uint16Plugin_976825672_h
#define evsn_uint16Plugin_976825672_h

#include "evsn_uint16.hpp"

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

#define EVSNUint16Plugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNUint16Plugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNUint16Plugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNUint16Plugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNUint16Plugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNUint16*
EVSNUint16PluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNUint16*
EVSNUint16PluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNUint16*
EVSNUint16PluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNUint16PluginSupport_copy_data(
    EVSNUint16 *out,
    const EVSNUint16 *in);

NDDSUSERDllExport extern void 
EVSNUint16PluginSupport_destroy_data_w_params(
    EVSNUint16 *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNUint16PluginSupport_destroy_data_ex(
    EVSNUint16 *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNUint16PluginSupport_destroy_data(
    EVSNUint16 *sample);

NDDSUSERDllExport extern void 
EVSNUint16PluginSupport_print_data(
    const EVSNUint16 *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNUint16Plugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNUint16Plugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNUint16Plugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNUint16Plugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNUint16Plugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint16 *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNUint16Plugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint16 *out,
    const EVSNUint16 *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNUint16Plugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNUint16 *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNUint16Plugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint16 **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNUint16Plugin_deserialize_from_cdr_buffer(
    EVSNUint16 *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNUint16Plugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNUint16Plugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNUint16Plugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNUint16Plugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNUint16Plugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNUint16 ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNUint16Plugin_new(void);

NDDSUSERDllExport extern void
EVSNUint16Plugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_uint16Plugin_976825672_h */

