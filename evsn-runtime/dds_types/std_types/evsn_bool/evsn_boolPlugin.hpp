

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_bool.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_boolPlugin_676271026_h
#define evsn_boolPlugin_676271026_h

#include "evsn_bool.hpp"

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

#define EVSNBoolPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNBoolPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNBoolPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNBoolPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNBoolPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNBool*
EVSNBoolPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNBool*
EVSNBoolPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNBool*
EVSNBoolPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNBoolPluginSupport_copy_data(
    EVSNBool *out,
    const EVSNBool *in);

NDDSUSERDllExport extern void 
EVSNBoolPluginSupport_destroy_data_w_params(
    EVSNBool *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNBoolPluginSupport_destroy_data_ex(
    EVSNBool *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNBoolPluginSupport_destroy_data(
    EVSNBool *sample);

NDDSUSERDllExport extern void 
EVSNBoolPluginSupport_print_data(
    const EVSNBool *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNBoolPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNBoolPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNBoolPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNBoolPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNBoolPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNBool *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNBoolPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNBool *out,
    const EVSNBool *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNBoolPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNBool *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNBoolPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNBool **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNBoolPlugin_deserialize_from_cdr_buffer(
    EVSNBool *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNBoolPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNBoolPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNBoolPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNBoolPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNBoolPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNBool ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNBoolPlugin_new(void);

NDDSUSERDllExport extern void
EVSNBoolPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_boolPlugin_676271026_h */

