---
name: middleware-testing
description: Testing middleware contracts and data movement between RTI Connext/DDS, MQTT, adapters, and bridge layers. Use for DDS contract tests, MQTT contract tests, DDS-to-MQTT bridge tests, QoS tests, topic/schema compatibility, and middleware lifecycle tests.
---

# Middleware Testing

Use this skill for DDS, MQTT, adapter, and bridge contract verification.

## Core Guidance

- Treat middleware configuration as a contract: topic names, IDL/schema, keys, QoS, retained and LWT behavior, deadlines, liveliness, durability, reconnect semantics, and failure behavior.
- For RTI Connext specifics, use the RTI MCP as the source of truth.
- For MQTT, test QoS selection, reconnect/offline buffering, LWT, duplicate delivery, retained messages, authorization assumptions, and broker/client failure modes.
- For bridges, test allowed data flow, dropped or filtered data, timestamping, correlation IDs, backpressure, retry behavior, and safe failure behavior.
- Include negative tests for mismatched QoS, schema, topic, and configuration where practical.
- Separate unit tests for adapter logic from integration tests that run real middleware.

## References

Read the relevant references before designing middleware contract or bridge tests:

- `references/dds-middleware.yaml`
- `references/mqtt-middleware.yaml`
- `references/test-automation-rules.md`
- `references/simulation-testing-rules.md`
- `references/coverage-criteria-rules.md`
