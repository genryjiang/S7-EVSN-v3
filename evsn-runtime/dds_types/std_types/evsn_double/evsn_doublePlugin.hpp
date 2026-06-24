

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_double.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_doublePlugin_19894554_h
#define evsn_doublePlugin_19894554_h

#include "evsn_double.hpp"

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

#define EVSNDoublePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNDoublePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNDoublePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNDoublePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNDoublePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNDouble*
EVSNDoublePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNDouble*
EVSNDoublePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNDouble*
EVSNDoublePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNDoublePluginSupport_copy_data(
    EVSNDouble *out,
    const EVSNDouble *in);

NDDSUSERDllExport extern void 
EVSNDoublePluginSupport_destroy_data_w_params(
    EVSNDouble *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNDoublePluginSupport_destroy_data_ex(
    EVSNDouble *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNDoublePluginSupport_destroy_data(
    EVSNDouble *sample);

NDDSUSERDllExport extern void 
EVSNDoublePluginSupport_print_data(
    const EVSNDouble *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNDoublePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNDoublePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNDoublePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNDoublePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNDoublePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDouble *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNDoublePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDouble *out,
    const EVSNDouble *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNDoublePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNDouble *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNDoublePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDouble **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNDoublePlugin_deserialize_from_cdr_buffer(
    EVSNDouble *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNDoublePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNDoublePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNDoublePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNDoublePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNDoublePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDouble ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNDoublePlugin_new(void);

NDDSUSERDllExport extern void
EVSNDoublePlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_doublePlugin_19894554_h */

