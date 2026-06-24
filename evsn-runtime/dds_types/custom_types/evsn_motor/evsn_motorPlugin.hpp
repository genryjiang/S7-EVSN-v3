

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_motor.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_motorPlugin_1371888787_h
#define evsn_motorPlugin_1371888787_h

#include "evsn_motor.hpp"

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

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern unsigned int 
EVSNMcuModePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern unsigned int 
EVSNMcuModePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMcuModePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* ----------------------------------------------------------------------------
Support functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern void
EVSNMcuModePluginSupport_print_data(
    const EVSNMcuMode *sample,
    const char *desc,
    unsigned int indent_level);

#define EVSNMotorRequestPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMotorRequestPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMotorRequestPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMotorRequestPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMotorRequestPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMotorRequest*
EVSNMotorRequestPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMotorRequest*
EVSNMotorRequestPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMotorRequest*
EVSNMotorRequestPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMotorRequestPluginSupport_copy_data(
    EVSNMotorRequest *out,
    const EVSNMotorRequest *in);

NDDSUSERDllExport extern void 
EVSNMotorRequestPluginSupport_destroy_data_w_params(
    EVSNMotorRequest *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMotorRequestPluginSupport_destroy_data_ex(
    EVSNMotorRequest *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMotorRequestPluginSupport_destroy_data(
    EVSNMotorRequest *sample);

NDDSUSERDllExport extern void 
EVSNMotorRequestPluginSupport_print_data(
    const EVSNMotorRequest *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMotorRequestPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMotorRequestPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMotorRequestPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMotorRequestPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMotorRequestPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorRequest *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMotorRequestPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorRequest *out,
    const EVSNMotorRequest *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMotorRequestPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMotorRequest *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMotorRequestPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorRequest **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMotorRequestPlugin_deserialize_from_cdr_buffer(
    EVSNMotorRequest *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMotorRequestPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMotorRequestPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMotorRequestPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMotorRequestPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMotorRequestPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorRequest ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMotorRequestPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMotorRequestPlugin_delete(struct PRESTypePlugin *);

#define EVSNMcuControlPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMcuControlPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMcuControlPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMcuControlPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMcuControlPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMcuControl*
EVSNMcuControlPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMcuControl*
EVSNMcuControlPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMcuControl*
EVSNMcuControlPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMcuControlPluginSupport_copy_data(
    EVSNMcuControl *out,
    const EVSNMcuControl *in);

NDDSUSERDllExport extern void 
EVSNMcuControlPluginSupport_destroy_data_w_params(
    EVSNMcuControl *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMcuControlPluginSupport_destroy_data_ex(
    EVSNMcuControl *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMcuControlPluginSupport_destroy_data(
    EVSNMcuControl *sample);

NDDSUSERDllExport extern void 
EVSNMcuControlPluginSupport_print_data(
    const EVSNMcuControl *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMcuControlPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMcuControlPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMcuControlPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMcuControlPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMcuControlPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuControl *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMcuControlPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuControl *out,
    const EVSNMcuControl *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMcuControlPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMcuControl *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMcuControlPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuControl **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMcuControlPlugin_deserialize_from_cdr_buffer(
    EVSNMcuControl *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMcuControlPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMcuControlPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMcuControlPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMcuControlPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMcuControlPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuControl ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMcuControlPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMcuControlPlugin_delete(struct PRESTypePlugin *);

#define EVSNMcuActualOperationPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMcuActualOperationPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMcuActualOperationPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMcuActualOperationPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMcuActualOperationPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMcuActualOperation*
EVSNMcuActualOperationPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMcuActualOperation*
EVSNMcuActualOperationPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMcuActualOperation*
EVSNMcuActualOperationPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMcuActualOperationPluginSupport_copy_data(
    EVSNMcuActualOperation *out,
    const EVSNMcuActualOperation *in);

NDDSUSERDllExport extern void 
EVSNMcuActualOperationPluginSupport_destroy_data_w_params(
    EVSNMcuActualOperation *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMcuActualOperationPluginSupport_destroy_data_ex(
    EVSNMcuActualOperation *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMcuActualOperationPluginSupport_destroy_data(
    EVSNMcuActualOperation *sample);

NDDSUSERDllExport extern void 
EVSNMcuActualOperationPluginSupport_print_data(
    const EVSNMcuActualOperation *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMcuActualOperationPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMcuActualOperationPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMcuActualOperationPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMcuActualOperationPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMcuActualOperationPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuActualOperation *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMcuActualOperationPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuActualOperation *out,
    const EVSNMcuActualOperation *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMcuActualOperationPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMcuActualOperation *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMcuActualOperationPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuActualOperation **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMcuActualOperationPlugin_deserialize_from_cdr_buffer(
    EVSNMcuActualOperation *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMcuActualOperationPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMcuActualOperationPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMcuActualOperationPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMcuActualOperationPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMcuActualOperationPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuActualOperation ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMcuActualOperationPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMcuActualOperationPlugin_delete(struct PRESTypePlugin *);

#define EVSNMcuStatusInfoPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMcuStatusInfoPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMcuStatusInfoPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMcuStatusInfoPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMcuStatusInfoPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMcuStatusInfo*
EVSNMcuStatusInfoPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMcuStatusInfo*
EVSNMcuStatusInfoPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMcuStatusInfo*
EVSNMcuStatusInfoPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMcuStatusInfoPluginSupport_copy_data(
    EVSNMcuStatusInfo *out,
    const EVSNMcuStatusInfo *in);

NDDSUSERDllExport extern void 
EVSNMcuStatusInfoPluginSupport_destroy_data_w_params(
    EVSNMcuStatusInfo *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMcuStatusInfoPluginSupport_destroy_data_ex(
    EVSNMcuStatusInfo *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMcuStatusInfoPluginSupport_destroy_data(
    EVSNMcuStatusInfo *sample);

NDDSUSERDllExport extern void 
EVSNMcuStatusInfoPluginSupport_print_data(
    const EVSNMcuStatusInfo *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMcuStatusInfoPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMcuStatusInfoPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMcuStatusInfoPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMcuStatusInfoPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMcuStatusInfoPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuStatusInfo *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMcuStatusInfoPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuStatusInfo *out,
    const EVSNMcuStatusInfo *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMcuStatusInfoPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMcuStatusInfo *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMcuStatusInfoPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuStatusInfo **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMcuStatusInfoPlugin_deserialize_from_cdr_buffer(
    EVSNMcuStatusInfo *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMcuStatusInfoPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMcuStatusInfoPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMcuStatusInfoPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMcuStatusInfoPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMcuStatusInfoPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuStatusInfo ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMcuStatusInfoPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMcuStatusInfoPlugin_delete(struct PRESTypePlugin *);

#define EVSNMcuTemperaturesPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMcuTemperaturesPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMcuTemperaturesPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMcuTemperaturesPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMcuTemperaturesPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMcuTemperatures*
EVSNMcuTemperaturesPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMcuTemperatures*
EVSNMcuTemperaturesPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMcuTemperatures*
EVSNMcuTemperaturesPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMcuTemperaturesPluginSupport_copy_data(
    EVSNMcuTemperatures *out,
    const EVSNMcuTemperatures *in);

NDDSUSERDllExport extern void 
EVSNMcuTemperaturesPluginSupport_destroy_data_w_params(
    EVSNMcuTemperatures *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMcuTemperaturesPluginSupport_destroy_data_ex(
    EVSNMcuTemperatures *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMcuTemperaturesPluginSupport_destroy_data(
    EVSNMcuTemperatures *sample);

NDDSUSERDllExport extern void 
EVSNMcuTemperaturesPluginSupport_print_data(
    const EVSNMcuTemperatures *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMcuTemperaturesPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMcuTemperaturesPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMcuTemperaturesPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMcuTemperaturesPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMcuTemperaturesPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuTemperatures *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMcuTemperaturesPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuTemperatures *out,
    const EVSNMcuTemperatures *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMcuTemperaturesPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMcuTemperatures *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMcuTemperaturesPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuTemperatures **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMcuTemperaturesPlugin_deserialize_from_cdr_buffer(
    EVSNMcuTemperatures *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMcuTemperaturesPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMcuTemperaturesPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMcuTemperaturesPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMcuTemperaturesPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMcuTemperaturesPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuTemperatures ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMcuTemperaturesPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMcuTemperaturesPlugin_delete(struct PRESTypePlugin *);

#define EVSNMcuSystemInfoPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMcuSystemInfoPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMcuSystemInfoPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMcuSystemInfoPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMcuSystemInfoPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMcuSystemInfo*
EVSNMcuSystemInfoPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMcuSystemInfo*
EVSNMcuSystemInfoPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMcuSystemInfo*
EVSNMcuSystemInfoPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMcuSystemInfoPluginSupport_copy_data(
    EVSNMcuSystemInfo *out,
    const EVSNMcuSystemInfo *in);

NDDSUSERDllExport extern void 
EVSNMcuSystemInfoPluginSupport_destroy_data_w_params(
    EVSNMcuSystemInfo *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMcuSystemInfoPluginSupport_destroy_data_ex(
    EVSNMcuSystemInfo *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMcuSystemInfoPluginSupport_destroy_data(
    EVSNMcuSystemInfo *sample);

NDDSUSERDllExport extern void 
EVSNMcuSystemInfoPluginSupport_print_data(
    const EVSNMcuSystemInfo *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMcuSystemInfoPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMcuSystemInfoPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMcuSystemInfoPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMcuSystemInfoPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMcuSystemInfoPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuSystemInfo *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMcuSystemInfoPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuSystemInfo *out,
    const EVSNMcuSystemInfo *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMcuSystemInfoPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMcuSystemInfo *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMcuSystemInfoPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuSystemInfo **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMcuSystemInfoPlugin_deserialize_from_cdr_buffer(
    EVSNMcuSystemInfo *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMcuSystemInfoPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMcuSystemInfoPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMcuSystemInfoPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMcuSystemInfoPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMcuSystemInfoPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMcuSystemInfo ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMcuSystemInfoPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMcuSystemInfoPlugin_delete(struct PRESTypePlugin *);

#define EVSNMotorStatusPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMotorStatusPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMotorStatusPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMotorStatusPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMotorStatusPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMotorStatus*
EVSNMotorStatusPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMotorStatus*
EVSNMotorStatusPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMotorStatus*
EVSNMotorStatusPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMotorStatusPluginSupport_copy_data(
    EVSNMotorStatus *out,
    const EVSNMotorStatus *in);

NDDSUSERDllExport extern void 
EVSNMotorStatusPluginSupport_destroy_data_w_params(
    EVSNMotorStatus *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMotorStatusPluginSupport_destroy_data_ex(
    EVSNMotorStatus *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMotorStatusPluginSupport_destroy_data(
    EVSNMotorStatus *sample);

NDDSUSERDllExport extern void 
EVSNMotorStatusPluginSupport_print_data(
    const EVSNMotorStatus *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMotorStatusPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMotorStatusPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMotorStatusPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMotorStatusPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMotorStatusPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorStatus *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMotorStatusPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorStatus *out,
    const EVSNMotorStatus *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMotorStatusPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMotorStatus *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMotorStatusPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorStatus **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMotorStatusPlugin_deserialize_from_cdr_buffer(
    EVSNMotorStatus *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMotorStatusPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMotorStatusPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMotorStatusPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMotorStatusPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMotorStatusPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorStatus ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMotorStatusPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMotorStatusPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_motorPlugin_1371888787_h */

