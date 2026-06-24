---
name: rti-connext-dds
description: DDS and RTI Connext architecture framing. Use for DDS topic modelling, IDL/type contracts, QoS profile thinking, publisher/subscriber design, discovery, durability, liveliness, deadline, and DDS-to-application architecture.
---

# RTI Connext DDS

Use this skill for application-level DDS architecture and Connext-facing design in this repository.

## Core Guidance

- `RTI_MCP_GUIDANCE_REQUIRED`: Before designing, creating, or materially changing
  any RTI-Connext based node, RTI Connext-based node, DDS type, QoS profile,
  participant, publisher, subscriber, reader, writer, WaitSet/listener path,
  discovery, durability, liveliness, deadline behavior, or generated-code
  integration, run the RTI Connext MCP for guidance.
- Record the MCP query, relevant result, and design consequence in the active
  execution plan or log.
- If the RTI Connext MCP is unavailable, stop and record a blocker unless the
  user explicitly narrows the task to non-RTI code.
- Do not treat generic DDS examples as authoritative for RTI Connext API, QoS, XML, WaitSet, DynamicData, generated C++ code, platform support, or runtime behavior.
- Prefer explicit DDS contracts: topic name, type, keying, QoS profile, deadline and liveliness assumptions, durability expectations, ownership, stale-data behavior, and failure behavior.
- Keep IDL/type contracts stable, reviewable, and documented with units, ranges, validity semantics, and compatibility expectations.
- Separate DDS adapter code from pure application logic so deterministic logic can be unit tested without live middleware.
- Distinguish between unit tests that mock Connext adapters and integration tests that run real `DomainParticipant`, `DataReader`, and `DataWriter` entities.

## References

Read the reference for DDS architecture framing before designing DDS contracts or middleware boundaries:

- `references/dds-middleware.yaml`
