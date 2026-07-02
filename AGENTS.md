# Agent Harness

This checkout consolidates details repository harness layout, populated with
SR-Mjolnir's QNX, RTI DDS, CAN, and safety-oriented project guidance. Keep agent
work restartable from disk, not from chat history.

## Startup

1. Run `git status --short` and preserve any user changes.
2. Read `.agents/feature_list.json`, `.agents/progress.md`,
   `.agents/session-handoff.md`, and `.agents/agent-context.md`.
3. For non-trivial work, create or update an execution plan in
   `.agents/exec_plans/`.
4. Use a small feature branch for new work. CI branch naming expects `feature`
   in the branch name, such as `feature/<slug>` or `name/feature/<slug>`.
5. Log material actions and evidence in `.agents/logs/YYYY-MM-DD-<feature-id>.md`.

## Project Context

This harness carries SR-Mjolnir's assumptions for QNX Neutrino RTOS and
embedded C++ environments.

Prioritise:

- deterministic latency over average throughput
- bounded execution and memory use
- explicit error handling
- MISRA C++-oriented style
- future ISO 26262 evidence preservation
- simple, reviewable implementations

RTI Connext is the application-level middleware. QNX is the runtime
environment. MQTT is used only for vehicle/cloud or telemetry/cloud paths where
explicitly applicable.

## External Documentation Gates

These are strict guards, not suggestions.

- `RTI_MCP_GUIDANCE_REQUIRED`: Before designing, creating, or materially
  changing any RTI-Connext based node, RTI Connext-based node, DDS type,
  QoS profile, participant,
  publisher, subscriber, reader, writer, WaitSet/listener path, discovery,
  durability, liveliness, deadline behavior, or generated-code integration,
  run the RTI Connext MCP for guidance. Record the MCP query, relevant
  result, and design consequence in the active execution plan or log. If the
  RTI Connext MCP is unavailable, stop and record a blocker unless the user
  explicitly narrows the task to non-RTI code.
- `QNX_ONLINE_DOCS_REQUIRED`: Before implementing or reviewing any QNX-facing
  behavior, QNX/POSIX API use, target build or deployment flow, IPC,
  scheduling, resource-manager behavior, HAM behavior, startup/process/thread
  behavior, filesystem/networking behavior, or compiler/toolchain behavior,
  consult the current online QNX documentation. Start from the official QNX
  Product Documentation page and the QNX SDP 8.0 documentation:
  `https://www.qnx.com/developers/docs/index.html` and
  `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`.
  Record the URLs or section titles used in the active execution plan or log.
  If online QNX documentation is inaccessible, stop and record a blocker. Do
  not implement or review QNX-facing behavior from memory or offline references
  alone.

## Execution Plan Gate

Do not implement large features without an active execution plan in
`.agents/exec_plans/`. A plan is required before implementation when work adds
or changes a feature, changes vehicle behavior, changes shared DDS/CAN/QNX/RTI
contracts, touches build or deployment flow, spans multiple modules, is
safety-relevant, or is likely to continue across sessions.

Small questions, repository inspection, explanations, narrow documentation
fixes, tiny low-risk bug fixes, and focused tests may proceed without a full
plan. If scope is ambiguous, create or update a plan before editing production
code.

## Work Items

The durable work queue is `.agents/feature_list.json`. It intentionally keeps
SR-Mjolnir's work-item schema and evidence fields from `harness/work-items.json`.
Label work items with uppercase `VSN-XXXX` IDs, such as `VSN-0001`; do not add
legacy work-item IDs.
Update status, verification evidence, blockers, touched files, and next action
before ending a session.

## Subagents

Use subagents for bounded work that improves scope control or
verification. The coordinator owns repository understanding, final scope, and
completion decisions.

- Give every subagent a self-contained prompt with the work item, allowed files,
  constraints, tools, and required evidence.
- Do not let subagents recursively spawn other subagents.
- Keep one active owner per work item or file area.
- Research subagents must not mutate files.
- Implementation subagents may edit only assigned files and must run or report
  assigned verification commands.
- Review subagents should prioritize correctness, QNX/RTI/CAN behavior, timing,
  allocation, safety assumptions, and missing tests.
- Subagent prompts for RTI Connext node work must include the
  `RTI_MCP_GUIDANCE_REQUIRED` evidence requirement.
- Subagent prompts for QNX-facing work must include the
  `QNX_ONLINE_DOCS_REQUIRED` evidence requirement.
- Record durable findings in `.agents/feature_list.json`, `.agents/logs/`, or
  the active execution plan.

See `.agents/subagents/README.md` for approved roles, workflow, and prompt
templates.

## Canonical Skills

The canonical SR-Mjolnir skills are stored under `.agents/skills/`.

Use:

- `qnx-cpp-rtos` for QNX implementation, review, debugging, deployment, IPC,
  resource managers, and QNX/POSIX API usage.
- `qnx-cpp-performance` for QNX optimization, latency, scheduling, tracing,
  memory behavior, IPC performance, and throughput work.
- `safety-critical-cpp` for production C++ and MISRA C++-oriented review.
- `iso26262-readiness` for safety artifacts, traceability, HSI, verification
  evidence, and safety-readiness work.
- `rti-connext-dds` for DDS/Connext architecture, topic/type/QoS contracts, and
  middleware design.
- `mqtt-middleware` for MQTT telemetry, remote-command, IoT/cloud telemetry,
  LWT, TLS/mTLS, reconnect/offline-buffering, and DDS-to-MQTT bridge design.
- `cpp-unit-testing`, `cpp-integration-testing`, `middleware-testing`,
  `fault-injection-resilience-testing`, and `test-coverage-evidence` for
  focused verification work.

## Hard Constraints

- Do not claim generated code is MISRA C++ compliant or ISO 26262 compliant.
- Say "MISRA C++-oriented" or "prepared for MISRA C++ review" unless
  static-analysis evidence, deviation records, review evidence, and project
  approval exist.
- Say "ISO 26262-ready" or "supports future ISO 26262 evidence" unless the
  project has actual lifecycle evidence and approval.
- Use CMake for building C++ applications for QNX, and direct `q++` only for
  focused QNX C++ target builds when needed.
- Do not create or materially change RTI Connext DDS nodes or shared DDS
  contracts without recorded RTI Connext MCP guidance.
- Do not create, review, or materially change QNX-facing code, build,
  deployment, or runtime behavior without recorded current online QNX
  documentation guidance.
- Do not assume Linux behavior where QNX behavior may differ.
- Avoid dynamic allocation after initialization in real-time or safety-relevant
  paths unless explicitly approved.
- Avoid exceptions, RTTI, recursion, unbounded queues, unbounded blocking, and
  unbounded STL containers in real-time or safety-relevant paths unless
  explicitly approved.
- Check every fallible QNX/POSIX API call.
- Prefer simple, analyzable C++ over clever C++.

## Verification

Run `./init.sh check` before claiming harness health. For code changes, run the
focused checks listed on the active work item, then record exact commands and
results in `.agents/feature_list.json` and the active log.
