

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from evsn_drive.idl
using RTI Code Generator (rtiddsgen) version 4.3.1.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef evsn_drivePlugin_408364356_h
#define evsn_drivePlugin_408364356_h

#include "evsn_drive.hpp"

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
EVSNDriveGearPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern unsigned int 
EVSNDriveGearPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNDriveGearPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* ----------------------------------------------------------------------------
Support functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern void
EVSNDriveGearPluginSupport_print_data(
    const EVSNDriveGear *sample,
    const char *desc,
    unsigned int indent_level);

#define EVSNDriveGearCommandPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNDriveGearCommandPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNDriveGearCommandPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNDriveGearCommandPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNDriveGearCommandPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNDriveGearCommand*
EVSNDriveGearCommandPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNDriveGearCommand*
EVSNDriveGearCommandPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNDriveGearCommand*
EVSNDriveGearCommandPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNDriveGearCommandPluginSupport_copy_data(
    EVSNDriveGearCommand *out,
    const EVSNDriveGearCommand *in);

NDDSUSERDllExport extern void 
EVSNDriveGearCommandPluginSupport_destroy_data_w_params(
    EVSNDriveGearCommand *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNDriveGearCommandPluginSupport_destroy_data_ex(
    EVSNDriveGearCommand *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNDriveGearCommandPluginSupport_destroy_data(
    EVSNDriveGearCommand *sample);

NDDSUSERDllExport extern void 
EVSNDriveGearCommandPluginSupport_print_data(
    const EVSNDriveGearCommand *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNDriveGearCommandPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNDriveGearCommandPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNDriveGearCommandPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNDriveGearCommandPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNDriveGearCommandPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDriveGearCommand *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNDriveGearCommandPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDriveGearCommand *out,
    const EVSNDriveGearCommand *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNDriveGearCommandPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNDriveGearCommand *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNDriveGearCommandPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDriveGearCommand **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNDriveGearCommandPlugin_deserialize_from_cdr_buffer(
    EVSNDriveGearCommand *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNDriveGearCommandPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNDriveGearCommandPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNDriveGearCommandPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNDriveGearCommandPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNDriveGearCommandPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNDriveGearCommand ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNDriveGearCommandPlugin_new(void);

NDDSUSERDllExport extern void
EVSNDriveGearCommandPlugin_delete(struct PRESTypePlugin *);

#define EVSNMotorControllerStatusPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define EVSNMotorControllerStatusPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define EVSNMotorControllerStatusPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define EVSNMotorControllerStatusPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define EVSNMotorControllerStatusPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern EVSNMotorControllerStatus*
EVSNMotorControllerStatusPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern EVSNMotorControllerStatus*
EVSNMotorControllerStatusPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern EVSNMotorControllerStatus*
EVSNMotorControllerStatusPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
EVSNMotorControllerStatusPluginSupport_copy_data(
    EVSNMotorControllerStatus *out,
    const EVSNMotorControllerStatus *in);

NDDSUSERDllExport extern void 
EVSNMotorControllerStatusPluginSupport_destroy_data_w_params(
    EVSNMotorControllerStatus *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
EVSNMotorControllerStatusPluginSupport_destroy_data_ex(
    EVSNMotorControllerStatus *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
EVSNMotorControllerStatusPluginSupport_destroy_data(
    EVSNMotorControllerStatus *sample);

NDDSUSERDllExport extern void 
EVSNMotorControllerStatusPluginSupport_print_data(
    const EVSNMotorControllerStatus *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
EVSNMotorControllerStatusPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
EVSNMotorControllerStatusPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
EVSNMotorControllerStatusPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
EVSNMotorControllerStatusPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
EVSNMotorControllerStatusPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorControllerStatus *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
EVSNMotorControllerStatusPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorControllerStatus *out,
    const EVSNMotorControllerStatus *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
EVSNMotorControllerStatusPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const EVSNMotorControllerStatus *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
EVSNMotorControllerStatusPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorControllerStatus **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
EVSNMotorControllerStatusPlugin_deserialize_from_cdr_buffer(
    EVSNMotorControllerStatus *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
EVSNMotorControllerStatusPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
EVSNMotorControllerStatusPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
EVSNMotorControllerStatusPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
EVSNMotorControllerStatusPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
EVSNMotorControllerStatusPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    EVSNMotorControllerStatus ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
EVSNMotorControllerStatusPlugin_new(void);

NDDSUSERDllExport extern void
EVSNMotorControllerStatusPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* evsn_drivePlugin_408364356_h */

