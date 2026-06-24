---
name: mqtt-middleware
description: MQTT vehicle-to-cloud telemetry, IoTCore/cloud telemetry, remote commands, OTA status, fleet telemetry, gateway/cloud integration, topic hierarchy, QoS, TLS/mTLS, LWT, reconnect, offline buffering, batching, and DDS-to-MQTT or CAN-to-MQTT bridge design.
---

# MQTT Middleware

Use this skill for MQTT and cloud telemetry paths. In this repository, MQTT is for vehicle/cloud or telemetry/cloud integration, not a replacement for RTI Connext inside the QNX application layer.

## Core Guidance

- Use MQTT for vehicle/cloud or telemetry/cloud paths, not as the internal application middleware between QNX vehicle-control nodes.
- Require TLS or mTLS assumptions for production-like MQTT.
- Require explicit topic hierarchy, QoS matrix, retained-message behavior, LWT behavior, reconnect behavior, offline buffering, payload size limits, rate limits, and failure behavior.
- For DDS-to-MQTT bridge work, document exactly what data crosses from Connext to MQTT and what does not.
- Treat MQTT topic names, payload schemas, authentication, authorization, replay protection, and audit logging as externally visible contracts.
- Avoid exposing safety-critical command paths over MQTT without explicit authentication, authorization, replay protection, audit logging, and safety review.

## References

Read the relevant references before designing MQTT or cloud bridge behavior:

- `references/mqtt-middleware.yaml`
- `references/api-design-guidelines.md`
