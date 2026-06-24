

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_params.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_paramsPlugin_288152163_h
#define evsn_paramsPlugin_288152163_h

#include "evsn_params.hpp"

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
EVSNParameterTypePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern unsigned int 
EVSNParameterTypePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterTypePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* ----------------------------------------------------------------------------
Support functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern void
EVSNParameterTypePluginSupport_print_data(
    const EVSNParameterType *sample,
    const char *desc,
    unsigned int indent_level);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern unsigned int 
EVSNParameterOperationPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern unsigned int 
EVSNParameterOperationPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterOperationPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* ----------------------------------------------------------------------------
Support functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern void
EVSNParameterOperationPluginSupport_print_data(
    const EVSNParameterOperation *sample,
    const char *desc,
    unsigned int indent_level);

#define EVSNParameterValuePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNParameterValuePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNParameterValuePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNParameterValuePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNParameterValuePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNParameterValue*
EVSNParameterValuePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNParameterValue*
EVSNParameterValuePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterValue*
EVSNParameterValuePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNParameterValuePluginSupport_copy_data(
    EVSNParameterValue *out,
    const EVSNParameterValue *in);

NDDSUSERDllExport extern void 
EVSNParameterValuePluginSupport_destroy_data_w_params(
    EVSNParameterValue *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNParameterValuePluginSupport_destroy_data_ex(
    EVSNParameterValue *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterValuePluginSupport_destroy_data(
    EVSNParameterValue *sample);

NDDSUSERDllExport extern void 
EVSNParameterValuePluginSupport_print_data(
    const EVSNParameterValue *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNParameterValuePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNParameterValuePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNParameterValuePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNParameterValuePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNParameterValuePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterValue *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNParameterValuePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterValue *out,
    const EVSNParameterValue *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNParameterValuePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNParameterValue *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNParameterValuePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterValue **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNParameterValuePlugin_deserialize_from_cdr_buffer(
    EVSNParameterValue *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNParameterValuePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNParameterValuePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNParameterValuePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterValuePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNParameterValuePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterValue ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNParameterValuePlugin_new(void);

NDDSUSERDllExport extern void
EVSNParameterValuePlugin_delete(struct PRESTypePlugin *);

#define EVSNParameterDescriptorPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNParameterDescriptorPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNParameterDescriptorPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNParameterDescriptorPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNParameterDescriptorPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNParameterDescriptor*
EVSNParameterDescriptorPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNParameterDescriptor*
EVSNParameterDescriptorPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterDescriptor*
EVSNParameterDescriptorPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNParameterDescriptorPluginSupport_copy_data(
    EVSNParameterDescriptor *out,
    const EVSNParameterDescriptor *in);

NDDSUSERDllExport extern void 
EVSNParameterDescriptorPluginSupport_destroy_data_w_params(
    EVSNParameterDescriptor *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNParameterDescriptorPluginSupport_destroy_data_ex(
    EVSNParameterDescriptor *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterDescriptorPluginSupport_destroy_data(
    EVSNParameterDescriptor *sample);

NDDSUSERDllExport extern void 
EVSNParameterDescriptorPluginSupport_print_data(
    const EVSNParameterDescriptor *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNParameterDescriptorPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNParameterDescriptorPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNParameterDescriptorPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNParameterDescriptorPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNParameterDescriptorPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterDescriptor *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNParameterDescriptorPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterDescriptor *out,
    const EVSNParameterDescriptor *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNParameterDescriptorPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNParameterDescriptor *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNParameterDescriptorPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterDescriptor **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNParameterDescriptorPlugin_deserialize_from_cdr_buffer(
    EVSNParameterDescriptor *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNParameterDescriptorPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNParameterDescriptorPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNParameterDescriptorPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterDescriptorPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNParameterDescriptorPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterDescriptor ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNParameterDescriptorPlugin_new(void);

NDDSUSERDllExport extern void
EVSNParameterDescriptorPlugin_delete(struct PRESTypePlugin *);

/* The type used to store keys for instances of type struct
* AnotherSimple.
*
* By default, this type is struct EVSNParameterState
* itself. However, if for some reason this choice is not practical for your
* system (e.g. if sizeof(struct EVSNParameterState)
* is very large), you may redefine this typedef in terms of another type of
* your choosing. HOWEVER, if you define the KeyHolder type to be something
* other than struct AnotherSimple, the
* following restriction applies: the key of struct
* EVSNParameterState must consist of a
* single field of your redefined KeyHolder type and that field must be the
* first field in struct EVSNParameterState.
*/
typedef class EVSNParameterState EVSNParameterStateKeyHolder;

#define EVSNParameterStatePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNParameterStatePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNParameterStatePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNParameterStatePlugin_get_key PRESTypePluginDefaultEndpointData_getKey 
#define EVSNParameterStatePlugin_return_key PRESTypePluginDefaultEndpointData_returnKey

#define EVSNParameterStatePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNParameterStatePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNParameterState*
EVSNParameterStatePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNParameterState*
EVSNParameterStatePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterState*
EVSNParameterStatePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePluginSupport_copy_data(
    EVSNParameterState *out,
    const EVSNParameterState *in);

NDDSUSERDllExport extern void 
EVSNParameterStatePluginSupport_destroy_data_w_params(
    EVSNParameterState *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNParameterStatePluginSupport_destroy_data_ex(
    EVSNParameterState *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterStatePluginSupport_destroy_data(
    EVSNParameterState *sample);

NDDSUSERDllExport extern void 
EVSNParameterStatePluginSupport_print_data(
    const EVSNParameterState *sample,
    const char *desc,
    unsigned int indent);

NDDSUSERDllExport extern EVSNParameterState*
EVSNParameterStatePluginSupport_create_key_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterState*
EVSNParameterStatePluginSupport_create_key(void);

NDDSUSERDllExport extern void 
EVSNParameterStatePluginSupport_destroy_key_ex(
    EVSNParameterStateKeyHolder *key,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterStatePluginSupport_destroy_key(
    EVSNParameterStateKeyHolder *key);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNParameterStatePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNParameterStatePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNParameterStatePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNParameterStatePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNParameterStatePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterState *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterState *out,
    const EVSNParameterState *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNParameterStatePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNParameterState *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterState **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNParameterStatePlugin_deserialize_from_cdr_buffer(
    EVSNParameterState *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNParameterStatePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNParameterStatePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNParameterStatePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterStatePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterState ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePlugin_instance_to_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterStateKeyHolder *key, 
    const EVSNParameterState *instance);

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePlugin_key_to_instance(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterState *instance, 
    const EVSNParameterStateKeyHolder *key);

NDDSUSERDllExport extern RTIBool 
EVSNParameterStatePlugin_serialized_sample_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *cdrStream, 
    DDS_KeyHash_t *keyhash,
    RTIBool deserialize_encapsulation,
    void *endpoint_plugin_qos); 

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNParameterStatePlugin_new(void);

NDDSUSERDllExport extern void
EVSNParameterStatePlugin_delete(struct PRESTypePlugin *);

#define EVSNParameterEventPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNParameterEventPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNParameterEventPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNParameterEventPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNParameterEventPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNParameterEvent*
EVSNParameterEventPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNParameterEvent*
EVSNParameterEventPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterEvent*
EVSNParameterEventPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNParameterEventPluginSupport_copy_data(
    EVSNParameterEvent *out,
    const EVSNParameterEvent *in);

NDDSUSERDllExport extern void 
EVSNParameterEventPluginSupport_destroy_data_w_params(
    EVSNParameterEvent *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNParameterEventPluginSupport_destroy_data_ex(
    EVSNParameterEvent *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterEventPluginSupport_destroy_data(
    EVSNParameterEvent *sample);

NDDSUSERDllExport extern void 
EVSNParameterEventPluginSupport_print_data(
    const EVSNParameterEvent *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNParameterEventPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNParameterEventPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNParameterEventPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNParameterEventPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNParameterEventPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterEvent *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNParameterEventPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterEvent *out,
    const EVSNParameterEvent *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNParameterEventPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNParameterEvent *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNParameterEventPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterEvent **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNParameterEventPlugin_deserialize_from_cdr_buffer(
    EVSNParameterEvent *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNParameterEventPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNParameterEventPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNParameterEventPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterEventPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNParameterEventPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterEvent ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNParameterEventPlugin_new(void);

NDDSUSERDllExport extern void
EVSNParameterEventPlugin_delete(struct PRESTypePlugin *);

#define EVSNParameterRequestPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNParameterRequestPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNParameterRequestPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNParameterRequestPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNParameterRequestPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNParameterRequest*
EVSNParameterRequestPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNParameterRequest*
EVSNParameterRequestPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterRequest*
EVSNParameterRequestPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNParameterRequestPluginSupport_copy_data(
    EVSNParameterRequest *out,
    const EVSNParameterRequest *in);

NDDSUSERDllExport extern void 
EVSNParameterRequestPluginSupport_destroy_data_w_params(
    EVSNParameterRequest *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNParameterRequestPluginSupport_destroy_data_ex(
    EVSNParameterRequest *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterRequestPluginSupport_destroy_data(
    EVSNParameterRequest *sample);

NDDSUSERDllExport extern void 
EVSNParameterRequestPluginSupport_print_data(
    const EVSNParameterRequest *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNParameterRequestPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNParameterRequestPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNParameterRequestPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNParameterRequestPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNParameterRequestPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterRequest *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNParameterRequestPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterRequest *out,
    const EVSNParameterRequest *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNParameterRequestPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNParameterRequest *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNParameterRequestPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterRequest **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNParameterRequestPlugin_deserialize_from_cdr_buffer(
    EVSNParameterRequest *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNParameterRequestPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNParameterRequestPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNParameterRequestPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterRequestPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNParameterRequestPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterRequest ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNParameterRequestPlugin_new(void);

NDDSUSERDllExport extern void
EVSNParameterRequestPlugin_delete(struct PRESTypePlugin *);

#define EVSNParameterReplyPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNParameterReplyPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNParameterReplyPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNParameterReplyPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNParameterReplyPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNParameterReply*
EVSNParameterReplyPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNParameterReply*
EVSNParameterReplyPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNParameterReply*
EVSNParameterReplyPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNParameterReplyPluginSupport_copy_data(
    EVSNParameterReply *out,
    const EVSNParameterReply *in);

NDDSUSERDllExport extern void 
EVSNParameterReplyPluginSupport_destroy_data_w_params(
    EVSNParameterReply *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNParameterReplyPluginSupport_destroy_data_ex(
    EVSNParameterReply *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNParameterReplyPluginSupport_destroy_data(
    EVSNParameterReply *sample);

NDDSUSERDllExport extern void 
EVSNParameterReplyPluginSupport_print_data(
    const EVSNParameterReply *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNParameterReplyPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNParameterReplyPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNParameterReplyPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNParameterReplyPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNParameterReplyPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterReply *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNParameterReplyPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterReply *out,
    const EVSNParameterReply *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNParameterReplyPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNParameterReply *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNParameterReplyPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterReply **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNParameterReplyPlugin_deserialize_from_cdr_buffer(
    EVSNParameterReply *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNParameterReplyPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNParameterReplyPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNParameterReplyPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNParameterReplyPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNParameterReplyPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNParameterReply ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNParameterReplyPlugin_new(void);

NDDSUSERDllExport extern void
EVSNParameterReplyPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_paramsPlugin_288152163_h */

