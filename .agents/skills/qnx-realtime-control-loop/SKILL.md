---
name: qnx-realtime-control-loop
description: QNX Neutrino RTOS real-time control-loop implementation and review guidance. Use when implementing, reviewing, testing, or documenting any periodic or deadline-driven control loop where deterministic scheduling, CPU affinity, stale-input fail-safe behavior, bounded actuator/IO output, timing instrumentation, and worst-case jitter evidence matter. Examples include vehicle control, actuator command loops, sensor-to-output loops, CAN command loops, and other safety-relevant real-time paths with a chosen control period.
---

# QNX Real-Time Control Loop

## Overview

Use this skill to design or review a QNX RTOS control loop as an isolated real-time execution path, not as a normal application timer loop. The control period is a design parameter chosen from the system requirement, timing budget, and target measurements; do not bake in a period such as 2 ms unless the requirement states it.

Keep the loop bounded, deterministic, isolated from logging/telemetry/UI/DDS, measured under worst-case load, and able to fail safe on stale inputs, output failures, or missed deadlines.

For exact QNX API details, also use `qnx-cpp-rtos` and `qnx-cpp-performance`. For production C++ review, also use `safety-critical-cpp`. For verification evidence, also use `test-coverage-evidence`, `cpp-integration-testing`, and `fault-injection-resilience-testing` as relevant.

## Hard No

Do not implement the control loop as a relative-sleep application loop:

```cpp
while (running) {
    read_inputs();
    compute_command();
    send_output();
    sleep_for(control_period);
}
```

Do not put logging, DDS, heap allocation, filesystem I/O, blocking IPC, network I/O, dynamic memory allocation, or telemetry publishing inside the real-time hot path.

## Required Architecture

Use separated ownership:

- Real-time control owner: a dedicated QNX process or tightly controlled process with one real-time control thread. It owns the periodic or deadline-driven loop, computes the final control command, and sends the bounded actuator/IO output. It has no telemetry, logging, DDS, UI, filesystem, network, or diagnostic work in the loop.
- Input receiver threads/processes: receive sensor, CAN, IPC, driver, or middleware inputs; timestamp each input; and update atomic or double-buffered snapshots. Never block the control loop.
- Output, logger, diagnostics, and telemetry processes: consume copied snapshots at lower priority. Never provide back-pressure to the real-time loop.

## QNX Configuration Rules

1. Use fixed-priority real-time scheduling. Prefer `SCHED_FIFO` for the control thread unless there is a documented reason to use another policy.
2. Set and document the control-thread priority. Do not choose a priority that can starve QNX clock, timer, interrupt, or driver services the loop depends on.
3. Pin the control thread to a chosen CPU/core using QNX runmask or processor-affinity APIs such as `ThreadCtl(_NTO_TCTL_RUNMASK, ...)`, where appropriate for the target.
4. Keep DDS, telemetry, UI, logging, network stack work, non-critical diagnostics, and other heavy work off the control core.
5. Select and document the control period, deadline, jitter budget, and worst-case execution-time budget from the system requirement. Examples such as 2 ms, 5 ms, or 10 ms are requirements, not defaults.
6. Use absolute-time periodic scheduling. Prefer `clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ...)` or a QNX timer/pulse design. Do not use relative sleeps that accumulate drift.
7. Preallocate all buffers, output frames, state objects, lookup tables, and telemetry snapshots before entering the loop.
8. Avoid mutexes in the hot path. Prefer atomic sequence counters, lock-free snapshots, or double buffering. If a mutex is unavoidable, require priority inheritance and a documented bounded hold time.
9. Check every fallible QNX/POSIX/driver/IO call and define the failure transition.

## Runtime Behavior

Implement the loop so each cycle has explicit phases:

1. Wake at the next absolute release time for the configured control period.
2. Measure wakeup jitter against the scheduled wake time.
3. Copy the latest input snapshots using bounded atomic or double-buffered reads.
4. Validate freshness for every input used by the control law or safety gate.
5. Compute the final command with bounded execution and no allocation.
6. Send the prebuilt or bounded output command through a measured transmit path.
7. Record timing counters in preallocated memory.
8. Advance the next absolute release time; if the loop is already late, count a deadline miss and move through the documented safe-state policy.

## Fail-Safe Rules

Treat all inputs as stale unless freshly timestamped with the same monotonic timebase used by the loop. Define stale thresholds in constants or configuration loaded before the loop starts.

- Stale safety-critical input -> safe command, conservative command limit, or safe state as defined by the system safety concept.
- Stale operator/request input -> neutral command unless a different fail-safe value is documented.
- Stale feedback input -> safe state or conservative open-loop fallback only when explicitly justified.
- Missed loop deadline -> increment a fault counter and eventually enter safe state.
- Output transmit failure -> safe state or a documented bounded retry/disable policy.
- Invalid or out-of-range input -> clamp only where the safety concept allows it; otherwise transition to safe state.

## Timing Instrumentation

Final code must include instrumentation that can be used on the QNX target without changing hot-path behavior materially. Prefer QNX tracing and/or cycle-counter style measurements where appropriate.

Measure and preserve evidence for:

- Loop wakeup jitter.
- Compute time.
- Output transmit call duration.
- End-to-end period.
- Deadline misses.
- Max and percentile latency under load.

Use preallocated counters, ring buffers, or trace events. Do not log to files, publish telemetry, allocate memory, or perform formatting in the real-time loop.

## Acceptance Criteria

A solution is not complete unless it provides:

- A real-time control-thread implementation.
- Explicit priority and scheduling policy.
- CPU affinity/runmask setup or a documented target-specific reason it is not used.
- Absolute-time loop at the documented control period.
- Freshness and stale-input handling for all control-relevant and safety-relevant inputs.
- No logging, DDS, filesystem, UI, network, allocation, or blocking telemetry in the hot path.
- Bounded output behavior with checked failure handling.
- Timing instrumentation for jitter, compute time, output transmit duration, period, and deadline misses.
- A stress-test plan with telemetry, DDS, logging, diagnostics, and representative system load active.
- Documented worst-case timing result, target hardware, QNX version, build configuration, CPU/runmask configuration, and remaining evidence gaps.

## Review Questions

Ask these when reviewing QNX real-time control code:

1. Is this a real-time thread/process or a normal application loop?
2. What is its priority and scheduling policy?
3. What CPU/core does it run on?
4. Can logging, DDS, telemetry, UI, network, filesystem I/O, IPC, queues, or diagnostics delay it?
5. Does it use absolute-time wakeups?
6. Does it allocate memory in the loop?
7. Can it block on a mutex, IPC, file, socket, driver call, or queue?
8. What is the configured control period, deadline, and jitter budget, and where do those requirements come from?
9. How does it handle stale, invalid, or contradictory inputs?
10. What happens if output transmission fails or exceeds its timing budget?
11. What is the measured worst-case jitter under full system load?

## Evidence Notes

Do not claim MISRA C++ compliance or ISO 26262 compliance from this skill or from generated tests. Say "MISRA C++-oriented" or "prepared for MISRA C++ review", and say "supports future ISO 26262 evidence" unless the project has approved lifecycle evidence, static-analysis results, deviation records, and review artifacts.
