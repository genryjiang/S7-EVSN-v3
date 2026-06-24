---
name: fault-injection-resilience-testing
description: General resilience and fault-injection testing for C++ middleware and system behavior. Use for injected failures, network loss, dropped messages, malformed payloads, delayed samples, process restarts, resource exhaustion, and degraded-mode checks.
---

# Fault Injection And Resilience Testing

Use this skill for controlled failure-mode tests and degraded-mode verification.

## Core Guidance

- Prefer controlled, reproducible fault injection over random chaos unless randomization is seeded and recorded.
- Test network disconnects, delayed delivery, duplicate messages, stale messages, invalid payloads, full queues, failed allocations, failed API calls, and process restarts where relevant.
- For DDS/RTI, include discovery delay, writer/reader restart, QoS mismatch, deadline miss, and liveliness loss scenarios where relevant.
- For MQTT, include broker disconnect, client reconnect, offline buffer replay, duplicate QoS 1 delivery, lost QoS 0 telemetry, and LWT publication.
- Require expected behavior, timeout, log evidence, and pass/fail criteria for each injected fault.
- Do not claim safety validation completeness without requirements traceability and review evidence.

## References

Read the relevant references before designing resilience or fault-injection tests:

- `references/test-automation-rules.md`
- `references/simulation-testing-rules.md`
- `references/safety-validation-rules.md`
- `references/safety-critical-code-rules.md`
