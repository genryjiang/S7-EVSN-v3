

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_fdcan.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_fdcanPlugin_835925882_h
#define evsn_fdcanPlugin_835925882_h

#include "evsn_fdcan.hpp"

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

#define EVSNFDCanFramePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNFDCanFramePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNFDCanFramePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNFDCanFramePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNFDCanFramePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNFDCanFrame*
EVSNFDCanFramePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNFDCanFrame*
EVSNFDCanFramePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNFDCanFrame*
EVSNFDCanFramePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNFDCanFramePluginSupport_copy_data(
    EVSNFDCanFrame *out,
    const EVSNFDCanFrame *in);

NDDSUSERDllExport extern void 
EVSNFDCanFramePluginSupport_destroy_data_w_params(
    EVSNFDCanFrame *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNFDCanFramePluginSupport_destroy_data_ex(
    EVSNFDCanFrame *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNFDCanFramePluginSupport_destroy_data(
    EVSNFDCanFrame *sample);

NDDSUSERDllExport extern void 
EVSNFDCanFramePluginSupport_print_data(
    const EVSNFDCanFrame *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNFDCanFramePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNFDCanFramePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNFDCanFramePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNFDCanFramePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNFDCanFramePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNFDCanFrame *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNFDCanFramePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNFDCanFrame *out,
    const EVSNFDCanFrame *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNFDCanFramePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNFDCanFrame *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNFDCanFramePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNFDCanFrame **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNFDCanFramePlugin_deserialize_from_cdr_buffer(
    EVSNFDCanFrame *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNFDCanFramePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNFDCanFramePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNFDCanFramePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNFDCanFramePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNFDCanFramePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNFDCanFrame ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNFDCanFramePlugin_new(void);

NDDSUSERDllExport extern void
EVSNFDCanFramePlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_fdcanPlugin_835925882_h */

