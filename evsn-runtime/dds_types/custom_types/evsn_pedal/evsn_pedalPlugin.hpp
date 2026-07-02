

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_pedal.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_pedalPlugin_1725250437_h
#define evsn_pedalPlugin_1725250437_h

#include "evsn_pedal.hpp"

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

#define EVSNPedalStatePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNPedalStatePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNPedalStatePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNPedalStatePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNPedalStatePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNPedalState*
EVSNPedalStatePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNPedalState*
EVSNPedalStatePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNPedalState*
EVSNPedalStatePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNPedalStatePluginSupport_copy_data(
    EVSNPedalState *out,
    const EVSNPedalState *in);

NDDSUSERDllExport extern void 
EVSNPedalStatePluginSupport_destroy_data_w_params(
    EVSNPedalState *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNPedalStatePluginSupport_destroy_data_ex(
    EVSNPedalState *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNPedalStatePluginSupport_destroy_data(
    EVSNPedalState *sample);

NDDSUSERDllExport extern void 
EVSNPedalStatePluginSupport_print_data(
    const EVSNPedalState *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNPedalStatePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNPedalStatePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNPedalStatePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNPedalStatePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNPedalStatePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNPedalState *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNPedalStatePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNPedalState *out,
    const EVSNPedalState *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNPedalStatePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNPedalState *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNPedalStatePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNPedalState **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNPedalStatePlugin_deserialize_from_cdr_buffer(
    EVSNPedalState *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNPedalStatePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNPedalStatePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNPedalStatePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNPedalStatePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNPedalStatePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNPedalState ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNPedalStatePlugin_new(void);

NDDSUSERDllExport extern void
EVSNPedalStatePlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_pedalPlugin_1725250437_h */

