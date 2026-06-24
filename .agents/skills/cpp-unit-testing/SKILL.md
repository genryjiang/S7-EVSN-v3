---
name: cpp-unit-testing
description: General C++ unit testing for this embedded QNX repository. Use for testing individual classes, functions, and modules without requiring QNX target runtime or live RTI Connext unless the adapter itself is under test.
---

# C++ Unit Testing

Use this skill for fast, deterministic C++ unit tests.

## Core Guidance

- Prefer fast host-runnable unit tests where possible.
- Separate pure logic from QNX, RTI Connext, MQTT, filesystem, network, clock, and timer adapters so core logic can be tested without live middleware.
- Use fakes, mocks, and stubs for QNX APIs, RTI Connext adapters, MQTT clients, clocks, timers, filesystem, and network boundaries.
- Test normal paths, boundary values, invalid inputs, timeout paths, partial-initialization cleanup, and fallible API-call handling.
- Avoid arbitrary sleeps in unit tests. Prefer injected clocks, readiness flags, bounded polling helpers, or direct state transitions.
- Do not rewrite production architecture only for tests unless the change improves modularity, determinism, or safety reviewability.

## References

Read the relevant references before designing unit-test strategy or coverage evidence:

- `references/test-automation-rules.md`
- `references/coverage-criteria-rules.md`
- `references/api-design-guidelines.md`
