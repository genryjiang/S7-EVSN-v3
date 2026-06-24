# 2026-06-20 EVSNNode Review/Rewrite Plan

Created an execution plan for an evidence-backed review and rewrite of the
shared EVSN node runtime.

Inputs loaded:

- `harness-creator` skill.
- `qnx-cpp-rtos` skill and local QNX references.
- `qnx-cpp-performance` skill and local real-time, memory, concurrency, and
  test-automation references.
- `rti-connext-dds` skill and local DDS middleware reference.
- Current runtime files under `evsn-runtime/evsn_node/`.
- Harness state files under `.agents/`.

Planning decisions:

- The rewrite must begin with a review/inventory of the current runtime API,
  DDS contracts, timing behavior, allocation behavior, QNX supervision behavior,
  and test gaps.
- DDS-affecting design and implementation are blocked until RTI Connext MCP
  guidance is available and recorded.
- QNX-facing review and implementation must record current QNX online
  documentation for each touched API or timing claim.
- The intended rewrite separates initialization from steady-state spin behavior,
  keeps RTI adapter code separate from deterministic runtime logic, avoids
  dynamic allocation in the steady-state path unless explicitly justified, and
  preserves current node API compatibility where practical.
- The current EVSNNode core is treated as copied/ported source that must be
  replaced with a newly designed implementation. The existing code is an input
  for behavior inventory only, not a template for structure, helper boundaries,
  comments, control flow, or internal naming.
- Any retained public API shape, file path, generated/vendor code, or
  unavoidable RTI/QNX API usage must be justified in a source-provenance and
  originality review before completion.

External guidance status:

- QNX Product Documentation index and QNX SDP 8.0 documentation bookset were
  opened on 2026-06-20.
- RTI Connext MCP was searched for but is not callable in this session.
  `tool_search` exposed AWS tools only, and the plugin install candidate list
  did not include an RTI Connext MCP/plugin.

Plan path:

- `.agents/exec_plans/evsn-node-review-rewrite.md`

Verification:

- `./init.sh check` passed on 2026-06-20, validating 15 work items and links
  in 72 Markdown files.
- `git diff --check` passed on 2026-06-20.
- After adding the full-rewrite/originality boundary, `./init.sh check` and
  `git diff --check` passed again on 2026-06-20.

## 2026-06-20 Recheck After Docker Env Setup

The EVSNNode rewrite remains blocked before implementation.

Evidence:

- `tool_search` for RTI Connext/DDS MCP guidance did not expose an RTI Connext
  MCP.
- `list_available_plugins_to_install` did not list an RTI Connext MCP/plugin
  install candidate.
- `docker/.env` exists, but `./scripts/evsn doctor` reports
  `DEV_ENV_BUILDER_IMAGE` is missing.
- The current non-comment key in `docker/.env` is `QNX_BUILD_IMAGE`; the
  repository compose file and EVSN CLI require `DEV_ENV_BUILDER_IMAGE`.
- `./scripts/evsn build --target evsn_node` still fails during preflight until
  `DEV_ENV_BUILDER_IMAGE` is defined.

No `evsn-runtime/evsn_node/` source files were edited.

## 2026-06-20 Implementation Session

Startup and gate status:

- `git status --short` showed the checkout still has no tracked baseline; all
  repository files are reported as untracked.
- Current branch is `feature/evsn-qnx-build-cli`, but `git log` reports no
  HEAD commit. The feature-branch setup blocker therefore remains a repository
  state issue rather than a source-edit decision.
- Loaded `AGENTS.md`, `.agents/feature_list.json`, `.agents/progress.md`,
  `.agents/session-handoff.md`, `.agents/agent-context.md`, and this execution
  plan before implementation.

Skills and references used:

- `.agents/skills/qnx-cpp-rtos/SKILL.md`
- `.agents/skills/qnx-cpp-performance/SKILL.md`
- `.agents/skills/rti-connext-dds/SKILL.md`
- Local QNX RTOS, QNX performance, and DDS middleware references listed in the
  execution plan.

External evidence:

- RTI Connext MCP became callable as `mcp__rti_connext`.
- RTI MCP queries covered WaitSet ownership/dispatch, GuardCondition shutdown,
  `LoanedSamples`, typed vs Any reader/writer handles, parameter request/reply,
  QoS classes, ReplierParams construction, QoS policy API names, and HAM restart
  discovery behavior.
- QNX online documentation consulted:
  - `https://www.qnx.com/developers/docs/index.html`
  - `https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html`
  - `ham_attach_self()`, `ham_heartbeat()`, `ham_condition()`,
    `ham_action_restart()`, `ham_action_execute()`, and `ham_action_log()`
    pages in the QNX SDP 8.0 High Availability Framework guide.
  - QNX SDP 8.0 timing, high-resolution timer, scheduling,
    `pthread_setschedparam()`, `sigwait()`, `TraceEvent()`, and `trace_logf()`
    pages.

Review findings before source edits:

- Preserve the existing EVSNNode public API for constructor, `spin()`,
  publisher/subscriber/timer creation, `PublisherHandle<T>`, node name, and
  parameter inspection helpers.
- Preserve DDS-native parameter behavior and QNX HAM policy shape.
- Replace regex topic validation, local WaitSet condition lifetime, direct
  executor logic in `EVSNNode`, static-only run control, implicit runtime-owned
  parameter QoS, and infinite no-timer WaitSet waits.
- Treat `evsn_node.*`, `parameter_service.*`, and `node_supervisor.*` as
  copied/ported implementation inputs; generated DDS type-support files remain
  vendor/generated and out of originality scope.

Design decision:

- Proceed with a fresh internal architecture while retaining public API and
  file paths for build/application compatibility.
- Use typed subscription records that own readers and StatusConditions, keep
  loaned samples lexical, use `AnyDataWriter` only for type-erased publisher
  registry entries, and drive work from one bounded owner-thread spin loop.
- Use explicit parameter state/event/request/reply QoS and resource limits
  where the runtime owns the DDS contract.

Implementation completed:

- `evsn-runtime/evsn_node/evsn_node.hpp`
  - Added typed `Subscription<T>` records that own `DataReader<T>` and
    `StatusCondition` objects.
  - Kept `PublisherHandle<T>` source-compatible while routing publication
    through typed writer recovery from the `AnyDataWriter` registry.
- `evsn-runtime/evsn_node/evsn_node.cpp`
  - Replaced regex topic validation with a bounded scanner.
  - Replaced the monolithic spin loop with timer initialization, bounded timer
    catch-up arithmetic, bounded WaitSet waits, explicit dispatch, and checked
    signal-handler installation.
  - Detached subscription conditions during cleanup.
- `evsn-runtime/evsn_node/parameter_service.cpp`
  - Added explicit resource limits and QoS for parameter state, event,
    request, and reply paths.
  - Switched request/reply construction to MCP-confirmed
    `rti::request::ReplierParams`.
- `evsn-runtime/evsn_node/qnx_ham_node_supervisor.cpp`
  - Rejected malformed numeric environment variables instead of accepting
    partial `strtol()` parses.
  - Checked `/proc/self/exefile` open failure before reading the restart path.
- `evsn-runtime/evsn_node/test/evsn_node_supervision_test.cpp`
  - Added invalid construction-time API checks.
  - Added a WaitSet subscriber dispatch test using a separate writer
    participant.
- `evsn-runtime/evsn_node/CMakeLists.txt`
  - Runs the supervision test through the local shared-memory DDS wrapper.

Verification evidence:

- Initial `./scripts/linux-test --clean` exposed a container ownership issue:
  `/workspace/linux-build` and `/workspace/linux-deploy` were owned by root in
  the already-running builder, so the `developer` user could not configure
  CMake. Ownership was corrected in the live container before rerunning.
- `./scripts/test-no-evsnnode-mqtt` passed.
- `./scripts/linux-test --clean` passed after fixes:
  clean container build completed and all 4 CTest tests passed.
- `VERIFY_QNX=1 ./init.sh verify` passed:
  harness check passed, Linux build/tests passed 4/4, QNX configure selected
  `armv8QNX8.0qcc_cxx12.2.0`, `qnx_ham_node_supervisor.cpp` compiled, and
  `libevsn_node.a` linked.
- Final evidence-update checks passed:
  `git diff --check`, `./scripts/test-no-evsnnode-mqtt`,
  `python3 -m json.tool .agents/feature_list.json`, and `./init.sh check`.

Remaining blocker:

- No approved QNX target or QEMU runtime contract is configured for HAM attach,
  heartbeat, missed-heartbeat behavior, restart behavior, clean detach, and
  spin-loop timing/trace capture. Do not mark `VSN-0015` passing until that
  runtime evidence exists.

## 2026-06-20 QNX QEMU Runtime Evidence

User supplied QEMU target:

- `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`

Implementation update:

- Added `evsn-runtime/evsn_node/test/qnx_evsn_node_ham_smoke.cpp`.
- Added QNX-only CMake target `qnx_evsn_node_ham_smoke`, linked through
  `evsn_node` and installed to `out/qnx/bin`.
- The smoke executable instantiates a real `EVSNNode`, records attach,
  first-tick, alive, high-action, restart, and detach markers, records monotonic
  timer interval summaries, and has modes for clean, deliberate spin stall,
  process death, and restarted execution.

Build evidence:

- `./scripts/evsn build --target qnx_evsn_node_ham_smoke` passed.
- Installed artifact:
  `out/qnx/bin/qnx_evsn_node_ham_smoke`.
- QNX artifact identity: ELF 64-bit LSB PIE executable, ARM aarch64, QNX
  interpreter `/usr/lib/ldqnx-64.so.2`, BuildID
  `286fb2f59b41866864f9e76d8ecbcbba`.
- Dynamic dependencies from `ntoaarch64-readelf -d`: `libsocket.so.4`,
  `libham.so.2`, `libc++.so.2`, `libm.so.3`, `libgcc_s.so.1`, `libc.so.6`.
  RTI libraries linked statically through the configured `RTI_STATIC` build.

Target setup evidence:

- Booted the QNX 8.0.0 aarch64le QEMU image and connected over SSH on
  `localhost:2222`.
- The image did not include HAM; staged AArch64 `ham`, `hamctrl`, and
  `libham.so.2` from the QNX SDP target image under `/tmp`.
- Started HAM with `LD_LIBRARY_PATH=/tmp /tmp/ham -f /tmp/ham.log`.
- Verified `/proc/ham` existed and target tools included `tracelogger`.

Runtime smoke evidence:

- Clean mode:
  `LD_LIBRARY_PATH=/tmp EVSN_HAM_HEARTBEAT_MS=1000
  EVSN_HAM_MISSED_HEARTBEAT_LOW=3 EVSN_HAM_MISSED_HEARTBEAT_HIGH=5
  /tmp/qnx_evsn_node_ham_smoke --mode clean --prefix
  /tmp/evsn_node_ham_clean` passed.
  Markers: `attached`, `first_tick`, `alive`, `detached`.
  Timing: 25 ticks, 24 intervals, min `96432 us`, max `102364 us`.
  QNX `tracelogger -n 2 -f /tmp/evsn_node_trace.kev` produced a 41,075-byte
  trace file.
- Stall mode:
  `EVSN_HAM_MISSED_HEARTBEAT_LOW=1`,
  `EVSN_HAM_MISSED_HEARTBEAT_HIGH=2`, and
  `EVSN_HAM_HEARTBEAT_HIGH_COMMAND="/tmp/qnx_evsn_node_ham_smoke --mark
  /tmp/evsn_node_ham_stall.high high"` produced markers `stalling`, `resumed`,
  `high`, `alive`, and `detached`.
  Timing: 6 ticks, 5 intervals, min `99950 us`, max `3599988 us`.
- Death/restart mode:
  Initial process ran with
  `EVSN_HAM_RESTART_COMMAND="/tmp/qnx_evsn_node_ham_smoke --mode restarted
  --prefix /tmp/evsn_node_ham_restart"`.
  Initial process exited with status `137` after `SIGKILL`; HAM restarted the
  repository-linked executable in restarted mode.
  Markers: `restarting` contained `kill`, `restarted` contained `restarted`,
  and `detached` contained `restarted`.
  Restarted timing: 15 ticks, 14 intervals, min `97462 us`, max `104387 us`.

Durable copied evidence:

- `.agents/evidence/VSN-0015-qnx-ham/evsn_node_ham_clean.*`
- `.agents/evidence/VSN-0015-qnx-ham/evsn_node_ham_stall.*`
- `.agents/evidence/VSN-0015-qnx-ham/evsn_node_ham_restart.*`
- `.agents/evidence/VSN-0015-qnx-ham/evsn_node_trace.kev`

Design consequence:

- `VSN-0015` can be marked passing. The remaining no-HEAD branch state is a
  checkout/harness issue, not a missing EVSNNode runtime evidence item.

Final verification after evidence updates:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./scripts/test-no-evsnnode-mqtt` passed.
- `./init.sh check` passed, validating 19 work items and links in 101 Markdown
  files.
- `./scripts/linux-test --clean` passed all 4 CTest tests.
- `VERIFY_QNX=1 ./init.sh verify` passed harness checks, Linux tests, and QNX
  install of `qnx_evsn_node_ham_smoke`.
