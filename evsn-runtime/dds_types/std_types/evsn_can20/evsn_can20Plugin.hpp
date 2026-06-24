

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_can20.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_can20Plugin_799635832_h
#define evsn_can20Plugin_799635832_h

#include "evsn_can20.hpp"

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

#define EVSNCan20FramePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNCan20FramePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNCan20FramePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNCan20FramePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNCan20FramePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNCan20Frame*
EVSNCan20FramePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNCan20Frame*
EVSNCan20FramePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNCan20Frame*
EVSNCan20FramePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNCan20FramePluginSupport_copy_data(
    EVSNCan20Frame *out,
    const EVSNCan20Frame *in);

NDDSUSERDllExport extern void 
EVSNCan20FramePluginSupport_destroy_data_w_params(
    EVSNCan20Frame *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNCan20FramePluginSupport_destroy_data_ex(
    EVSNCan20Frame *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNCan20FramePluginSupport_destroy_data(
    EVSNCan20Frame *sample);

NDDSUSERDllExport extern void 
EVSNCan20FramePluginSupport_print_data(
    const EVSNCan20Frame *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNCan20FramePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNCan20FramePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNCan20FramePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNCan20FramePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNCan20FramePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNCan20Frame *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNCan20FramePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNCan20Frame *out,
    const EVSNCan20Frame *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNCan20FramePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNCan20Frame *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNCan20FramePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNCan20Frame **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNCan20FramePlugin_deserialize_from_cdr_buffer(
    EVSNCan20Frame *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNCan20FramePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNCan20FramePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNCan20FramePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNCan20FramePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNCan20FramePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNCan20Frame ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNCan20FramePlugin_new(void);

NDDSUSERDllExport extern void
EVSNCan20FramePlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_can20Plugin_799635832_h */

