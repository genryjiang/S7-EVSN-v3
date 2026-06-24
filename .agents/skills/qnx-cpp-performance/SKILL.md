---
name: qnx-cpp-performance
description: Optimising C++ for QNX RTOS environments. Use for latency, scheduling, IPC, profiling, tracing, memory behaviour, throughput work, and deterministic performance review on QNX.
---

# QNX C++ Performance

Use this skill for QNX performance work where timing, scheduling, IPC, memory behavior, or throughput matters.

## Core Guidance

- Optimise deterministic latency before average throughput.
- Use QNX tracing, profiling, timing measurements, or other concrete evidence before broad architectural changes.
- Use message passing for control paths.
- Use pulses for lightweight notifications.
- Use shared memory only for large payloads or data-plane paths, and document synchronization and ownership.
- Avoid dynamic allocation after initialization in real-time paths.
- Avoid unbounded blocking in high-priority threads.
- Avoid unbounded queues, unbounded retries, and priority inversions.
- Explicitly document timing, allocation, blocking, and priority assumptions.
- Preserve correctness and fail-safe behavior ahead of micro-optimisation.

## References

Read the relevant reference before recommending performance-sensitive QNX changes:

- `references/qnx-neutrino-rtos.yaml`
- `references/qnx-advanced.yaml`
- `references/real-time-coding-rules.md`
- `references/memory-management-rules.md`
- `references/concurrency-rules.md`
- `references/test-automation-rules.md`
- `references/simulation-testing-rules.md`
