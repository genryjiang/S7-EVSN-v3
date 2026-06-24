# VSN-0015 - EVSNNode Review And Rewrite

Branch: feature/evsn-node-runtime-rewrite
Owner: main-agent with bounded review/verification subagents
Status: passing with QNX QEMU runtime evidence

## Scope

Review and rewrite the shared EVSN node runtime under `evsn-runtime/evsn_node/`
so production nodes have a deterministic, reviewable QNX/RTI execution model.
The rewrite is a full replacement of the core EVSNNode implementation, not a
cosmetic rename, formatting pass, or line-by-line refactor of the current code.

The work covers:

- `EVSNNode` construction, shutdown, timer dispatch, DDS WaitSet dispatch, and
  callback ownership.
- Typed publisher and subscriber creation APIs and their compatibility contract.
- DDS entity lifetime, topic naming, QoS selection, resource limits, deadline,
  liveliness, durability, and stale-data assumptions.
- DDS-native parameter service polling, request/reply behavior, state/event
  topics, and startup defaults loading.
- QNX HAM supervision integration, heartbeat timing, restart policy, and failure
  handling.
- Runtime allocation, blocking, exception, logging, and timing behavior in the
  spin path.
- Linux test seams for deterministic unit tests and RTI DDS integration tests.

Use these repository skills during the work:

- `qnx-cpp-rtos` for QNX runtime/API/build behavior.
- `qnx-cpp-performance` for timing, allocation, blocking, concurrency, tracing,
  and deterministic latency review.
- `rti-connext-dds` for DDS/RTI contract design, QoS, WaitSet/request-reply
  design, and generated-code boundaries.

## Originality Boundary

The current EVSNNode implementation was copied or ported from another codebase.
This work item must produce an original EVSN implementation guided by approved
behavioral requirements, repository tests, QNX/RTI documentation, and the agent
skills listed above.

Rules for the rewrite:

- Treat the existing code as an input for behavior inventory only. Do not use it
  as a source template for structure, helper boundaries, comments, control flow,
  or naming beyond the public API that is deliberately preserved.
- Preserve only documented public contracts that current EVSN application nodes
  need, such as constructor shape, publisher/subscriber/timer APIs, parameter
  behavior, and build target names. Any preserved API must be listed as an
  intentional compatibility decision.
- Replace the internal implementation architecture: executor, timer scheduler,
  DDS adapter, parameter-service adapter, supervision adapter, validation, error
  handling, and tests should be newly designed.
- Do not mechanically translate existing loops, callbacks, parsing functions,
  topic validation, HAM setup, or parameter-service implementation into new
  files.
- Derive the new design from documented requirements: deterministic latency,
  bounded memory and blocking, explicit DDS contracts, QNX failure handling,
  RTI Connext MCP guidance, and focused tests.
- Keep generated RTI type-support files and externally supplied APIs separate
  from this originality requirement. Generated/vendor material should be
  identified explicitly when reviewed.
- Record a short source-provenance and originality review before marking the
  implementation complete. This is engineering hygiene and traceability, not a
  legal clearance claim.

## Non-Goals

- Do not change application node behavior unless the runtime API requires a
  documented migration.
- Do not change CAN gateway contracts, DBC ownership, or vehicle-control
  behavior.
- Do not reintroduce per-node MQTT, AWS IoT, or cloud ingress into ordinary
  EVSN nodes.
- Do not regenerate DDS type support unless RTI Connext MCP guidance requires a
  DDS contract change and the regeneration flow is available.
- Do not deploy to a live vehicle or target hardware as part of the rewrite.
- Do not claim MISRA C++ compliance or ISO 26262 compliance; describe outputs as
  MISRA C++-oriented or supporting future ISO 26262 evidence only when backed by
  review/test artifacts.
- Do not keep internal source shape solely to minimize effort. Similarity with
  the current copied implementation is acceptable only for approved public API,
  generated code, or externally dictated RTI/QNX API usage.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/README.md`
- `harness-creator` skill
- `.agents/skills/qnx-cpp-rtos/SKILL.md`
- `.agents/skills/qnx-cpp-performance/SKILL.md`
- `.agents/skills/rti-connext-dds/SKILL.md`
- QNX local references:
  `.agents/skills/qnx-cpp-rtos/references/qnx-neutrino-rtos.yaml`,
  `.agents/skills/qnx-cpp-rtos/references/qnx-advanced.yaml`
- QNX performance references:
  `.agents/skills/qnx-cpp-performance/references/real-time-coding-rules.md`,
  `.agents/skills/qnx-cpp-performance/references/memory-management-rules.md`,
  `.agents/skills/qnx-cpp-performance/references/concurrency-rules.md`,
  `.agents/skills/qnx-cpp-performance/references/test-automation-rules.md`
- DDS local reference:
  `.agents/skills/rti-connext-dds/references/dds-middleware.yaml`
- Current EVSN runtime files:
  `evsn_node.hpp`, `evsn_node.cpp`, `parameter_service.*`,
  `node_supervisor.*`, `noop_node_supervisor.cpp`,
  `qnx_ham_node_supervisor.cpp`, `CMakeLists.txt`, and runtime tests.

## External Guidance

- RTI MCP:
  - Required before source review conclusions or rewrite implementation.
  - Initial planning sessions did not expose a callable RTI Connext MCP.
  - Rechecked on 2026-06-20 in this implementation session:
    `tool_search` exposed `mcp__rti_connext`.
  - Query: WaitSet-driven single-thread dispatch, `LoanedSamples`,
    `AnyDataWriter`/`AnyDataReader`, parameter request/reply, QoS classes, and
    HAM restart/discovery behavior for a deterministic QNX node runtime.
  - Relevant results:
    - Prefer one application-owned WaitSet thread and `GuardCondition` for
      controlled shutdown; keep DDS entity creation/destruction and
      WaitSet attach/detach on the owner thread where practical.
    - Prefer `wait()` plus explicit dispatch ordering over listener callbacks;
      if `dispatch()` handlers are used, keep handlers small and lifetime-bound.
    - Own `ReadCondition`/`StatusCondition` with the reader and avoid dynamic
      condition creation in steady state.
    - Treat `LoanedSamples<T>` as lexical and ephemeral. Do not keep sample
      references after the `LoanedSamples` object is destroyed or returned;
      copy data before crossing callback/thread boundaries.
    - Use typed `DataWriter<T>`/`DataReader<T>` as operational handles and use
      `AnyDataWriter`/`AnyDataReader` only as type-erased registry references.
    - Parameter request/reply should be WaitSet or bounded-poll driven rather
      than listener driven. Parameter state should be a keyed
      reliable/transient-local keep-last(1) last-value cache.
    - Recommended QoS classes: command/control reliable volatile event-style
      QoS with bounded writer blocking; periodic status best-effort volatile
      keep-last with deadline; parameter state reliable transient-local
      keep-last(1); parameter events reliable volatile keep-all or bounded
      keep-last; request/reply reliable volatile RPC-style QoS with explicit
      timeouts/resource limits.
    - HAM restart must be treated as a brand-new DDS participant. Volatile data
      does not survive restart, and transient-local writer cache does not
      survive writer process death without an external durability service.
  - Design consequence: rewrite may proceed with a single-owner runtime loop,
    explicit dispatch ownership, lexical sample handling, typed operational
    endpoints, bounded parameter polling, and documented restart assumptions.
- QNX online docs:
  - Consulted the QNX Product Documentation index and QNX SDP 8.0 documentation
    bookset on 2026-06-20.
  - The QNX SDP 8.0 bookset page identifies the QNX OS System Architecture,
    OS Components & Operations, Programming, System Security Guide, Utilities &
    Libraries, and QNX Software in the Cloud documentation sets.
  - Search results identified QNX SDP 8.0 pages for HAM API, `ham_condition()`,
    `ham_action_restart()`, clock/timer services, timers, timer tolerance, and
    `TraceEvent()`.
  - Implementation-session pages/sections used:
    - QNX Product Documentation index:
      `https://www.qnx.com/developers/docs/index.html`
    - QNX SDP 8.0 bookset:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
    - `ham_attach_self()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.ham/topic/hamapi/ham_attach_self.html`
    - `ham_heartbeat()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.ham/topic/hamapi/ham_heartbeat.html`
    - `ham_condition()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.ham/topic/hamapi/ham_condition.html`
    - `ham_action_restart()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.ham/topic/hamapi/ham_action_restart.html`
    - `ham_action_execute()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.ham/topic/hamapi/ham_action_execute.html`
    - `ham_action_log()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.ham/topic/hamapi/ham_action_log.html`
    - QNX timing and monotonic clock overview:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/timing_Time_functions.html`
    - High-resolution timers:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/timing_tolerance.html`
    - Scheduling policies and `pthread_setschedparam()`:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_SCHEDS.html`,
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setschedparam.html`
    - Signal waiting guidance:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sigwait.html`
    - Trace instrumentation:
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/t/traceevent.html`,
      `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/t/trace_logf.html`
  - Design consequence: QNX HAM calls remain isolated to the QNX backend, every
    fallible HAM setup/detach call is checked, the spin loop uses monotonic
    time, signal handling remains minimal, and any production priority/trace
    policy must be configured and verified on target.

Required RTI Connext MCP queries before implementation:

- WaitSet-driven single-thread dispatch for RTI Connext Modern C++ on QNX:
  recommended condition ownership, handler lifetime, dispatch behavior, and
  shutdown wakeup strategy.
- `LoanedSamples` and `DataReader::take()` behavior: allocation/lifetime
  implications and safe callback boundaries.
- `AnyDataWriter`/`AnyDataReader` storage and typed handle patterns:
  compatibility, lifetime, and failure behavior.
- Request/reply service design for EVSN parameters: replier polling, timeout,
  resource limits, keyed state/event topics, and late-joiner QoS.
- QoS profiles for EVSN control/status/parameter topics: reliability,
  durability, history, deadline, liveliness, ownership, and resource limits.
- Process restart behavior after QNX HAM restart: participant rediscovery,
  volatile/transient-local data behavior, and safe DDS entity teardown.

Required QNX documentation checks before implementation:

- HAM self-attach, heartbeat, detach, death condition, missed-heartbeat
  conditions, restart/log/execute actions, and error handling.
- Clock/timer behavior, monotonic clock usage, timeout behavior, and trace
  instrumentation options for measuring dispatch jitter.
- Signal handling constraints and the safest way to wake or stop a wait path.
- QNX scheduling/priority assumptions for the EVSN node thread when deployed as
  a production process.

## Review Steps

- [x] Freeze the current runtime API and behavior inventory:
      constructors, `spin()`, publisher/subscriber/timer creation, parameter
      service, supervision, tests, scripts, and README claims.
- [x] Classify every observed behavior as one of:
      preserve public contract, replace implementation detail, remove obsolete
      behavior, or needs owner approval.
- [x] Create a source-provenance note that lists copied/ported runtime files,
      generated/vendor files, and files that must be fully replaced.
- [x] Build a DDS contract table for every runtime-created topic/service:
      topic name, type, keying, owner, QoS source, deadline/liveliness,
      durability, history depth, resource limits, stale-data behavior, and
      shutdown/restart expectations.
- [x] Build a runtime execution table:
      each callback source, period or event trigger, expected WCET budget,
      blocking behavior, allocation behavior, exception behavior, and heartbeat
      relationship.
- [x] Audit dynamic allocation after initialization:
      vectors, maps, strings, `std::function`, regex, TOML parsing, DDS entity
      creation, request/reply polling, samples, logging, and error paths.
- [x] Audit blocking and latency paths:
      WaitSet waits, timer catch-up loop, parameter polling, callback dispatch,
      DDS writes, HAM heartbeat, shutdown, and signal handling.
- [x] Audit QNX-facing behavior:
      fallible API calls, HAM configuration parsing, `/proc/self/exefile`
      usage, restart command formation, detach failure behavior, and target-only
      compile/link dependencies.
- [x] Audit test coverage:
      pure unit tests, fake DDS/supervision seams, isolated-domain DDS
      integration tests, QNX build tests, HAM runtime smoke tests, timing tests,
      and negative/failure tests.
- [x] Record review findings in this plan and in the durable log before source
      edits begin.

## Rewrite Architecture

- [x] Draft a fresh runtime design brief before editing source:
      module boundaries, public API compatibility decisions, internal class
      names, ownership model, initialization flow, spin-loop state machine,
      error model, and verification strategy.
- [x] Introduce an explicit initialization/runtime split:
      all DDS entities, callbacks, timers, parameter defaults, fixed-capacity
      containers, and supervision configuration are established before `spin()`.
- [x] Replace ad hoc runtime parsing/validation with bounded initialization-time
      validation where possible, including topic-name validation that avoids
      regex in performance-sensitive paths.
- [x] Define a deterministic executor model:
      one owner thread, monotonic time source, explicit timer catch-up policy,
      bounded callback dispatch, bounded shutdown latency, and heartbeat only
      after useful progress.
- [x] Add test seams around DDS and QNX adapters so deterministic logic can be
      unit tested without live middleware or QNX HAM.
- [x] Keep RTI Connext adapter code separate from pure scheduling/state logic.
- [x] Replace the copied core files with newly structured implementation units
      where practical. If a file path is retained for build compatibility, its
      internal structure and implementation should still be newly designed.
- [x] Replace implicit default QoS with explicit profiles or code-level QoS
      decisions backed by RTI Connext MCP evidence.
- [x] Define runtime error policy:
      initialization failures may fail fast with clear diagnostics; spin-path
      failures must have bounded handling and must not rely on unbounded
      exception propagation from callbacks or DDS/HAM operations.
- [x] Add timing instrumentation hooks suitable for Linux tests and QNX trace
      evidence without adding high-rate stdout/stderr logging.
- [x] Preserve source compatibility for current application nodes unless a
      deliberate migration note and compatibility shim are included.
- [x] Add an implementation review step that compares the rewritten runtime
      against the pre-rewrite snapshot and records why any retained public API,
      names, or code blocks remain necessary.

## Current Behavior Inventory

Preserve public contracts:

- `EVSNNode(const std::string&, int32_t, const std::string&)`, `spin()`, pure
  virtual destructor, `get_name()`, parameter inspection helpers,
  `create_subscriber<T>()`, `create_publisher<T>()`, `PublisherHandle<T>`, and
  `create_timer()`.
- Topic validation contract: names must use slash-prefixed lowercase
  alphanumeric/underscore segments.
- Derived-node construction-time publisher, subscriber, and timer creation.
- DDS-native parameter behavior: optional TOML defaults, request/reply
  get/set/list/describe, state topic, event topic, and late-joiner state.
- QNX HAM supervision policy shape: self-attach, death restart, low/high missed
  heartbeat logging, optional high-threshold execute action, Linux no-op
  backend, and heartbeat only after spin-loop progress.

Replace implementation details:

- Regex topic validation in runtime code.
- Local, non-owned WaitSet condition objects and handler-lifetime coupling.
- `EVSNNode` directly owning all timer, DDS dispatch, parameter polling, signal,
  and supervision state without a defined executor boundary.
- Static global run state as the only spin-loop control.
- Implicit default QoS for runtime-created DDS entities where the topic class is
  known.
- Infinite WaitSet wait in no-timer cases, which can delay signal-driven
  shutdown.

Needs later owner approval or target evidence:

- Production QNX scheduling policy/priority and runmask.
- Any persistence beyond transient-local DDS writer lifetime.

Source-provenance note:

- `evsn_node.hpp`, `evsn_node.cpp`, `parameter_service.*`, and
  `node_supervisor.*` are treated as copied/ported runtime implementation
  inputs for behavior inventory only and must be internally replaced.
- Generated RTI type-support files under `evsn-runtime/dds_types/` are vendor
  generated and excluded from the originality rewrite requirement.
- Public API names and file paths are retained for application and build
  compatibility, not as internal architecture templates.

## DDS Contract Table

| Runtime surface | Topic/service | Type | Keying | QoS and bounds | Restart/stale behavior |
| --- | --- | --- | --- | --- | --- |
| Application publisher | caller-supplied topic | `T` | type-defined | caller-supplied QoS or RTI default compatibility fallback | volatile/default behavior unless caller supplies stronger QoS |
| Application subscriber | caller-supplied topic | `T` | type-defined | caller-supplied QoS or RTI default compatibility fallback | loaned samples copied before callback boundary |
| Parameter state | `/evsn/parameters/state` | `EVSNParameterState` | node name + parameter name | reliable, transient-local, keep-last(1), explicit resource limits | late joiners get current state while writer lives; cache is lost on process death |
| Parameter event | `/evsn/parameters/event` | `EVSNParameterEvent` | unkeyed | reliable, volatile, keep-all with explicit resource limits | historical replay is not guaranteed across writer restart |
| Parameter service | `EVSNParameterService` | `EVSNParameterRequest`/`EVSNParameterReply` | request/reply correlation | reliable, volatile, bounded request polling, explicit request/reply resource limits | restarted process is a new participant and service endpoint |

Compatibility decision: application publisher/subscriber default overloads keep
RTI default QoS to avoid silently changing current node behavior. Topic-class
specific QoS is applied to runtime-owned parameter topics/services where the
contract is known.

## Runtime Execution Table

| Source | Trigger | Runtime handling | Blocking/allocation policy | Heartbeat relationship |
| --- | --- | --- | --- | --- |
| Timer callbacks | monotonic steady-clock due time | fixed-order scan, one catch-up per due timer per loop turn | timers created before spin; callback behavior is caller-owned | heartbeat after at least one timer callback |
| DDS subscribers | WaitSet condition active | owner thread drains valid samples and copies each sample into callback argument | reader/condition created before spin; loan scope stays in drain call | heartbeat after at least one DDS callback |
| Parameter service | internal 20 ms timer | bounded zero-timeout request receive, handle matching requests on owner thread | service entities created before spin; request loans stay local | heartbeat after poll timer makes progress |
| QNX HAM | configured heartbeat period | supervisor heartbeat after useful loop progress only | QNX backend checks attach/condition/action errors; detach failure recorded by backend state | heartbeat is never emitted by idle wait alone |
| Signal stop | SIGINT/SIGTERM | signal handler stores atomic stop request; finite WaitSet timeout bounds wake latency | no non-signal-safe DDS/HAM calls from signal handler | spin exits without extra heartbeat requirement |

## Fresh Runtime Design Brief

- Keep `EVSNNode` as the public compatibility facade and move steady-state work
  into explicit runtime records: `TimerTask`, typed `Subscription<T>`,
  publisher registry entries, and parameter-service adapter.
- Use an initialization phase before `spin()` for DDS entities, timers,
  callbacks, parameter defaults, WaitSet condition attachment, and supervision
  construction.
- Use a single owner-thread loop: process due timers, wait with a bounded
  timeout, dispatch active DDS conditions, then heartbeat only if useful work
  occurred.
- Store typed DataReaders with their owned StatusCondition objects. Store
  `AnyDataWriter` only as a registry reference for `PublisherHandle<T>`.
- Keep loaned samples lexical inside subscription drain functions and pass a
  copied `T` value to user callbacks.
- Replace regex topic validation with bounded character scanning.
- Keep QNX HAM APIs isolated in `qnx_ham_node_supervisor.cpp`; Linux uses a
  compile-time no-op backend.
- Use exceptions for initialization and callback failure propagation as current
  API behavior, while keeping fallible QNX/HAM setup checks explicit. A later
  safety review may replace exception propagation with project-wide result
  types if required.

## Implementation Review

Implemented source changes:

- Replaced the old local-WaitSet-handler subscriber path with typed
  `Subscription<T>` records that own the `DataReader<T>` and
  `StatusCondition`, attach during initialization, detach during cleanup, and
  keep `LoanedSamples<T>` lexical inside `drain()`.
- Kept `AnyDataWriter` only as the type-erased publisher registry used by
  `PublisherHandle<T>`; the data path recovers a typed writer before `write()`.
- Replaced regex topic validation with a bounded segment scanner.
- Replaced the spin loop with explicit timer initialization, bounded timer
  catch-up arithmetic, bounded WaitSet waits, explicit condition dispatch, and
  signal-handler error checking.
- Added explicit parameter state/event/request/reply QoS resource limits and
  switched parameter `Replier` construction to MCP-confirmed
  `rti::request::ReplierParams`.
- Tightened QNX HAM environment parsing and `/proc/self/exefile` failure
  checks.
- Ran the DDS supervision test under the shared-memory-only local DDS wrapper
  and added focused checks for invalid construction-time API inputs and
  WaitSet-driven subscriber callback dispatch.
- Added QNX-only `qnx_evsn_node_ham_smoke` target that links through
  `evsn_node`, instantiates a real `EVSNNode`, records marker files for
  attach/detach/restart evidence, emits monotonic timer interval summaries, and
  deliberately stalls the spin path for missed-heartbeat action evidence.
- Installed `qnx_evsn_node_ham_smoke` to `out/qnx/bin` through the existing
  QNX CMake build path.

Retained public API and file-path justification:

- `EVSNNode`, `PublisherHandle<T>`, `create_publisher`, `create_subscriber`,
  `create_timer`, parameter inspection helpers, and file paths are retained for
  current application and CMake compatibility.
- QNX HAM function names and RTI Connext API calls are retained because they are
  externally dictated APIs.
- Generated DDS type-support files are unchanged.

Originality review:

- The rewritten internals no longer mirror the previous local condition
  lifetime, regex validation, dispatch-handler-only path, or monolithic spin
  loop. Similarity that remains is limited to the compatibility facade,
  generated/vendor types, and required RTI/QNX API usage.

## Subagent Plan

- Main agent owns scope, final design decisions, repository state updates, and
  source integration.
- RTI/DDS reviewer owns MCP query evidence and DDS contract review. This role
  must not mutate files until the MCP evidence is recorded.
- QNX/performance reviewer owns QNX documentation evidence, timing/allocation
  review, and QNX build/smoke-test recommendations.
- Implementation worker, if used, may edit only `evsn-runtime/evsn_node/`,
  focused tests, and directly required CMake/docs files.
- Test evidence reviewer owns verification gaps and must prioritize missing
  negative tests, failure handling, timing evidence, and QNX runtime smoke gaps.

## Verification

- [x] Command: `./init.sh check`
      Result: Passed on 2026-06-20; validated 15 work items and links in
      72 Markdown files.
- [x] Command: `git diff --check`
      Result: Passed on 2026-06-20.
- [x] Command: `./scripts/linux-test --clean`
      Result: Passed on 2026-06-20; clean container build completed and CTest
      passed 4/4 tests: `evsn_parameter_service_test`,
      `evsn_node_no_mqtt_runtime`, `evsn_node_supervision_test`, and
      `evsn_can20_frame_test`.
- [x] Command: `./scripts/test-no-evsnnode-mqtt`
      Result: Passed on 2026-06-20.
- [x] Command: focused EVSN node unit tests through CTest
      Result: Passed on 2026-06-20 through `./scripts/linux-test --clean`.
- [x] Command: isolated-domain RTI DDS integration tests for publisher,
      subscriber, parameter request/reply, state/event topics, and shutdown
      Result: Passed on 2026-06-20 through the shared-memory local DDS CTest
      wrappers for `evsn_parameter_service_test` and
      `evsn_node_supervision_test`.
- [x] Command: `VERIFY_QNX=1 ./init.sh verify`
      Result: Passed on 2026-06-20; harness check passed, Linux build/tests
      passed 4/4, QNX cross-build configured for
      `armv8QNX8.0qcc_cxx12.2.0`, compiled `qnx_ham_node_supervisor.cpp`, and
      linked `libevsn_node.a`.
- [x] Command: QNX target or QEMU HAM smoke test for attach, heartbeat,
      missed-heartbeat behavior, restart, and clean detach
      Result: Passed on 2026-06-20 against
      `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`; staged HAM
      from the QNX SDP target image, ran the repository-linked
      `qnx_evsn_node_ham_smoke`, and captured marker files under
      `.agents/evidence/VSN-0015-qnx-ham/` for clean
      attach/heartbeat/detach, missed-heartbeat high action, and process-death
      restart.
- [x] Command: QNX timing/trace capture for spin-loop jitter and callback
      dispatch budget on approved target
      Result: Passed on 2026-06-20; clean mode recorded 25 ticks with
      96,432-102,364 us intervals, stall mode recorded a 3,599,988 us maximum
      interval while HAM high action executed, restarted mode recorded 15 ticks
      with 97,462-104,387 us intervals, and QNX `tracelogger` produced
      `.agents/evidence/VSN-0015-qnx-ham/evsn_node_trace.kev` (41,075 bytes).
- [x] Command: source-provenance/originality review of rewritten EVSNNode
      runtime against the pre-rewrite snapshot
      Result: Completed in this plan on 2026-06-20.

## Acceptance Criteria

- RTI Connext MCP evidence is recorded before DDS design decisions are made.
- QNX online documentation evidence is recorded before QNX-facing code is
  reviewed or changed.
- The current EVSNNode API and behavior are inventoried with explicit migration
  notes for any deliberate incompatibility.
- The copied/ported core EVSNNode implementation is replaced by a newly designed
  internal architecture rather than a superficial rename or mechanical rewrite.
- Any retained names, file paths, public API shapes, or code blocks are
  explicitly justified as compatibility, generated/vendor code, or unavoidable
  RTI/QNX API usage.
- Runtime-created DDS topics/services have documented type, QoS, deadline,
  liveliness, durability, resource-limit, stale-data, and restart behavior.
- The rewritten runtime has a documented initialization phase and avoids dynamic
  allocation in the steady-state spin path unless an approved exception is
  recorded.
- Every fallible QNX/POSIX/HAM call in touched code is checked and has clear
  failure behavior.
- Timer, WaitSet, callback, parameter-service, and HAM heartbeat behavior are
  covered by focused tests.
- Linux tests pass through the standard gate, and QNX build/runtime evidence is
  recorded before the work is marked passing.

## Evidence

- Planning log: `.agents/logs/2026-06-20-evsn-node-review-rewrite-plan.md`
- QNX documentation index: `https://www.qnx.com/developers/docs/index.html`
- QNX SDP 8.0 bookset:
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
- QNX target evidence on 2026-06-20:
  `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64` booted QNX
  8.0.0 aarch64le over QEMU, SSH on `localhost:2222` was used for bounded
  deployment, `qnx_evsn_node_ham_smoke` installed as an AArch64 QNX executable
  with BuildID `286fb2f59b41866864f9e76d8ecbcbba`, dynamic dependencies were
  limited to QNX system libraries plus `libham.so.2`, and marker/timing/trace
  evidence was copied to `.agents/evidence/VSN-0015-qnx-ham/`.
- Docker builder status rechecked on 2026-06-20: `docker/.env` exists, but
  `./scripts/evsn doctor` reports `DEV_ENV_BUILDER_IMAGE` is missing. The file
  currently defines `QNX_BUILD_IMAGE`, while `docker/compose.yml` and the EVSN
  CLI require `DEV_ENV_BUILDER_IMAGE`.
- Superseding builder evidence on 2026-06-20: current `docker/.env` defines
  `DEV_ENV_BUILDER_IMAGE`, `./scripts/evsn doctor --offline` passed, and
  `./scripts/evsn build --target qnx_evsn_node_ham_smoke` completed.
- Planning verification: `./init.sh check` and `git diff --check` passed on
  2026-06-20.
- Originality-boundary update verification: `./init.sh check` and
  `git diff --check` passed on 2026-06-20.
- Final implementation evidence update verification: `git diff --check`,
  `./scripts/test-no-evsnnode-mqtt`,
  `python3 -m json.tool .agents/feature_list.json`, and `./init.sh check`
  passed on 2026-06-20.

## Handoff

Source implementation, local Linux/QNX build verification, and QNX QEMU runtime
smoke verification are complete. The remaining branch setup issue is a checkout
state concern because the repository still reports no HEAD commit; it is not a
runtime evidence blocker for `VSN-0015`.
