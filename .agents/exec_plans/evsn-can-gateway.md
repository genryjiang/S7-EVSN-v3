# VSN-0017 - EVSN CAN Gateway

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: blocked

## Scope

Create an implementation plan for a deterministic EVSN CAN gateway that bridges
QNX CAN resource-manager endpoints to RTI Connext DDS raw CAN topics.

The intended implementation is a new package under `evsn-runtime/can_gateway/`
unless the EVSN runtime rewrite deliberately changes the package layout first.
One gateway process instance owns one concrete non-CAN0 bus by default. CAN0 is
the drive-control bus and is owned by `evsn_can0_drive_bus_rt` under
`VSN-0023`, not by the generic DDS CAN gateway.

The gateway is responsible for:

- Opening a configured CAN RX endpoint through the target-selected CAN HAL,
  such as `/dev/can1/rx0` for QNX raw `devctl()` backends.
- Publishing received raw frames to a configured DDS topic, normally
  `/can<bus>/from_can_bus`.
- Subscribing to a configured DDS topic, normally `/can<bus>/to_can_bus`.
- Transmitting accepted outbound raw frames through the target-selected CAN HAL,
  such as `/dev/can1/tx0` for QNX raw `devctl()` backends.
- Supporting FD-capable bus topics with `EVSNFDCanFrame`.
- Supporting CAN 2.0-only bus topics with `EVSNCan20Frame`.
- Keeping hardware I/O outside DDS callbacks by using bounded internal
  queues and a deterministic poll/drain loop.
- Recording diagnostics for rejected frames, queue pressure, QNX `devctl()`
  errors, DDS write/read failures, and shutdown state.

The gateway boundary remains raw CAN. Nodes that own a CAN ID continue to do
DBC pack/unpack locally after subscribing to `/can<bus>/from_can_bus`, and
publish raw packed frames to `/can<bus>/to_can_bus` for gateway transmission.

The bus/frame source of truth for unchanged EMBD-v2-HighLevel hardware is the
hardware-equivalence matrix in
`.agents/exec_plans/evsn-network-port-roadmap.md`. `VSN-0022` adapts this
generic gateway plan to that matrix and explicitly excludes CAN0 TX from the
generic gateway.

## Non-Goals

- Do not decode CAN signals or publish one decoded DDS topic per CAN ID.
- Do not move DBC pack/unpack logic out of CAN-ID-owning nodes.
- Do not implement the DDS-to-IoT bridge or any cloud telemetry path here.
- Do not change motor, pedal, or other vehicle-control application logic beyond
  any integration tests required to prove the gateway contract.
- Do not create a generic CAN0 TX route. CAN0 writes belong to
  `evsn_can0_drive_bus_rt`; final motor commands must never pass through DDS or
  this generic gateway.
- Do not claim physical CAN-FD hardware support until target hardware evidence
  exists; local `dev-can-linux` evidence is SIL/raw-frame evidence only.
- Do not expose QNX, `dev-can-linux`, or future TDA4 driver headers in public
  C++ gateway headers.
- Do not implement source changes until the RTI Connext MCP guidance gate is
  satisfied.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/subagents/README.md`
- `.agents/exec_plans/README.md`
- `.agents/skills/rti-connext-dds/SKILL.md`
- `.agents/skills/qnx-cpp-rtos/SKILL.md`
- `.agents/skills/safety-critical-cpp/SKILL.md`
- `.agents/skills/rti-connext-dds/references/dds-middleware.yaml`
- `.agents/skills/qnx-cpp-rtos/references/qnx-neutrino-rtos.yaml`
- `.agents/skills/qnx-cpp-rtos/references/qnx-advanced.yaml`
- `.agents/skills/safety-critical-cpp/references/misra-cpp-2023.md`
- `.agents/skills/safety-critical-cpp/references/real-time-coding-rules.md`
- `.agents/skills/safety-critical-cpp/references/concurrency-rules.md`
- `.agents/skills/safety-critical-cpp/references/memory-management-rules.md`
- `.agents/skills/safety-critical-cpp/references/code-review-checklist.md`
- `evsn-runtime/CMakeLists.txt`
- `evsn-runtime/dds_types/std_types/CMakeLists.txt`
- `evsn-runtime/dds_types/std_types/evsn_fdcan/evsn_fdcan.idl`
- `evsn-runtime/dds_types/std_types/evsn_can20/evsn_can20.idl`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/exec_plans/evsn-can-broker-adaptation.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence matrix

Local SR-Mjolnir and chat context inspected:

- `/Users/henryjiang/.codex/sessions/2026/06/14/rollout-2026-06-14T18-56-02-019ec558-5be0-7372-b775-24cd43d22b2c.jsonl`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/ARCHITECTURE.md`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/docs/TOPICS_REGISTRY.md`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/docs/exec-plans/active/15-06-26_iot_bridge_nodes.md`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/docs/exec-plans/active/17-06-26_can_gateway.md`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/docs/exec-plans/active/18-06-26_can-20-support.md`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/core/dds_types/std_types/sr_fdcan/sr_fdcan.idl`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/core/dds_types/std_types/sr_can20/sr_can20.idl`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/src/vehicle_interface/motor_driver/README.md`
- `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/src/vehicle_interface/motor_driver/src/motor_can_codec.cpp`
- `/Users/henryjiang/Documents/Sunswift/dev-can-linux/dev-can-linux/commands.h`
- `/Users/henryjiang/Documents/Sunswift/dev-can-linux/README.md`
- `/Users/henryjiang/Documents/Sunswift/dev-can-linux/harness/work-items.json`

Local EMBD-v2-HighLevel context inspected for CAN bridge topic names:

- `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/ros2_socketcan/src/socket_can_receiver_node.cpp`
- `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/ros2_socketcan/src/socket_can_sender_node.cpp`
- `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_broker/include/can_broker/device_driver.hpp`
- `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/template_driver/src/template_driver.cpp`
- `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/orion2_bms_driver/src/orion2_bms_driver.cpp`

Important chat conclusion:

- The gateway receives raw hardware frames, wraps them in the raw DDS CAN frame
  type, and publishes to a bus-specific raw CAN topic.
- Vehicle nodes subscribe to the bus-specific raw CAN topic, filter by CAN ID,
  and unpack the payload with their local DBC-generated or approved codec.
- Closed-loop systems that local nodes need should still flow through DDS so a
  later DDS-to-IoT bridge can observe or export them where approved.
- The earlier `/from/can/<bus>` and `/to/can/<bus>` provisional names are
  superseded by the EMBD-v2-HighLevel-compatible defaults:
  `/can<bus>/from_can_bus` and `/can<bus>/to_can_bus`.

## External Guidance

- RTI MCP:
  - Query attempted: guidance for a QNX CAN gateway node that publishes
    `/from/can/<bus>`, subscribes `/to/can/<bus>`, uses bounded queues, and
    must choose DDS QoS for raw CAN telemetry and transmit-command topics.
  - Result: the available RTI Connext MCP call timed out after approximately
    120 seconds.
  - Design consequence: source implementation and final DDS QoS decisions are
    blocked. The architecture below is provisional and derived from existing
    SR-Mjolnir plans, current EVSN DDS types, QNX documentation, and local
    driver evidence. Before implementation, rerun the RTI MCP query and record
    the exact recommendation for durability, reliability, history depth,
    resource limits, deadline, liveliness, WaitSet/listener behavior, and
    generated type integration.
  - Topic-name update query: asked whether changing the gateway defaults from
    `/from/can/<bus>` and `/to/can/<bus>` to bus-first names
    `/can<bus>/from_can_bus` and `/can<bus>/to_can_bus` creates RTI Connext
    concerns for topic compatibility, partitions, discovery, QoS matching, or
    topic naming syntax.
  - Topic-name result: RTI Connext guidance reported no DDS-specific blocker.
    The rename is a system contract change; endpoints must use exactly equal
    topic names, compatible types, and compatible QoS. Slashes in these names
    are acceptable as a naming convention. The rename does not require DDS
    partitions because bus identity is encoded in the topic name. Topic names
    should remain configurable.
  - Topic-name design consequence: adopt `/can<bus>/from_can_bus` and
    `/can<bus>/to_can_bus` as defaults to align with EMBD-v2-HighLevel, record
    the rename as breaking for consumers that used the provisional names, and
    keep implementation blocked until the remaining gateway QoS/lifecycle MCP
    guidance is recorded.

- QNX online docs:
  - Product documentation index:
    https://www.qnx.com/developers/docs/index.html
  - QNX SDP 8.0 documentation:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
  - CAN `devctl()` command index:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
  - `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
  - `CAN_DEVCTL_TX_FRAME_RAW`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
  - `CAN_DEVCTL_SET_MID`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_set_mid.html
  - `CAN_DEVCTL_SET_TIMING`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_set_timing.html
  - C library `devctl()` reference:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/d/devctl.html
  - Resource-manager `devctl()` handling:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.resmgr/topic/messages_HANDLING_devctl.html
  - Resource manager client model:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/resource_WhyWrite.html

QNX design consequences:

- Use raw-frame `devctl()` operations for the gateway adapter.
- Prefer `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` with a bounded poll period for v1
  shutdown determinism because it returns `EAGAIN` when no frame is available.
- Treat `CAN_DEVCTL_TX_FRAME_RAW` `EAGAIN` as transmit queue pressure and record
  diagnostics instead of blocking indefinitely.
- Encode standard 11-bit message IDs in bits 18 through 28 of `can_msg.mid`;
  encode extended 29-bit IDs in bits 0 through 28.
- Check every fallible `open()`, `close()`, and `devctl()` call, including
  `EAGAIN`, `EINTR`, `EINVAL`, `EIO`, `ENOTTY`, and permission or endpoint
  errors.
- Treat bitrate/timing changes as a deployment or driver-start responsibility
  unless a later target-specific requirement needs controlled runtime changes.

## Proposed Architecture

### Package Layout

Planned files:

- `evsn-runtime/can_gateway/CMakeLists.txt`
- `evsn-runtime/can_gateway/README.md`
- `evsn-runtime/can_gateway/include/evsn/can_gateway/*.hpp`
- `evsn-runtime/can_gateway/src/*.cpp`
- `evsn-runtime/can_gateway/src/qnx/*.c`
- `evsn-runtime/can_gateway/test/*.cpp`

The package should build on Linux with the `VSN-0019` fake CAN HAL backend and
build on QNX with a selected CAN HAL backend. The public C++ headers should
expose only EVSN gateway types and interfaces, not QNX `struct can_msg`,
`commands.h`, or future TDA4 driver headers.

### Runtime Configuration

Use explicit `EVSN_` environment variables:

- `EVSN_DDS_DOMAIN_ID`
- `EVSN_CAN_GATEWAY_NODE_NAME`
- `EVSN_CAN_GATEWAY_BUS_INDEX`
- `EVSN_CAN_GATEWAY_BUS_NAME`, defaulting to `can${EVSN_CAN_GATEWAY_BUS_INDEX}`
- `EVSN_CAN_GATEWAY_RX_PATH`
- `EVSN_CAN_GATEWAY_TX_PATH`
- `EVSN_CAN_GATEWAY_FROM_TOPIC`
- `EVSN_CAN_GATEWAY_TO_TOPIC`
- `EVSN_CAN_GATEWAY_BUS_MODE` with values `fdcan` or `can20`
- `EVSN_CAN_GATEWAY_ALLOW_FD`
- `EVSN_CAN_GATEWAY_POLL_PERIOD_US`
- `EVSN_CAN_GATEWAY_LATENCY_LIMIT_MS`
- `EVSN_CAN_GATEWAY_TX_QUEUE_CAPACITY`

Defaults should be explicit and logged at startup. Invalid config should fail
before DDS participant creation or QNX endpoint mutation where practical.

Default topic construction:

- `EVSN_CAN_GATEWAY_FROM_TOPIC` defaults to
  `/${EVSN_CAN_GATEWAY_BUS_NAME}/from_can_bus`.
- `EVSN_CAN_GATEWAY_TO_TOPIC` defaults to
  `/${EVSN_CAN_GATEWAY_BUS_NAME}/to_can_bus`.
- For bus index `0`, the concrete defaults are `/can0/from_can_bus` and
  `/can0/to_can_bus`, but the generic gateway must reject this configuration
  unless it is an explicitly approved read-only test/mirror mode. Production
  CAN0 ownership belongs to `evsn_can0_drive_bus_rt`.
- For bus index `1`, the concrete defaults are `/can1/from_can_bus` and
  `/can1/to_can_bus`.

These defaults intentionally copy EMBD-v2-HighLevel's ROS2 socketcan bridge
contract while preserving deployment-specific topic overrides.

### Internal Frame Model

Use the `VSN-0019` CAN HAL internal bounded frame representation before mapping
to DDS:

- `uint32_t id`
- `bool extended_id`
- `bool remote_frame`
- `uint8_t dlc`
- fixed 64-byte payload storage
- payload length
- bus mode
- optional monotonic receive timestamp where available

Rules:

- CAN 2.0 mode accepts payloads of 0 through 8 bytes.
- FD mode accepts payloads of 0 through 8 bytes by default.
- FD payloads above 8 bytes require `EVSN_CAN_GATEWAY_ALLOW_FD=true`.
- Remote frames are valid only when the selected DDS type can preserve them.
- Reject malformed DLC/length combinations and increment diagnostics.

### DDS Mapping

Current EVSN DDS types:

- `EVSNFDCanFrame` contains `uint32 id` and `sequence<octet, 64> data`.
- `EVSNCan20Frame` contains `uint32 id`, `boolean extended_id`,
  `boolean remote_frame`, `uint8 dlc`, and `sequence<octet, 8> data`.

Mapping consequences:

- `EVSNCan20Frame` can preserve standard/extended ID, remote-frame state, DLC,
  and payload.
- `EVSNFDCanFrame` currently cannot preserve extended-ID or remote-frame
  metadata. The gateway must either restrict FD bus traffic to the current
  documented assumptions or require a future DDS contract change before
  extended-ID or remote-frame FD traffic is accepted.
- The gateway must not silently drop metadata that changes CAN semantics.
  Reject and count unsupported metadata combinations.

Provisional QoS until RTI MCP guidance is recorded:

- `/can<bus>/from_can_bus`: volatile, bounded history, bounded resource
  limits. Final reliability and deadline policy are pending RTI MCP guidance
  and consumer timing requirements.
- `/can<bus>/to_can_bus`: volatile, bounded history, bounded resource limits,
  no unbounded writer blocking. Candidate reliability policy is pending RTI MCP
  guidance because transmit-command loss and stale command replay have safety
  consequences.

Compatibility note:

- The move from `/from/can/<bus>` and `/to/can/<bus>` to bus-first topic names
  is a breaking interface rename. Existing DDS readers/writers using the
  provisional names will not match the new topics until updated.

### CAN HAL Boundary

Gateway source must consume `evsn_can_hal` from `VSN-0019` for all CAN hardware
and virtual-CAN access. The gateway should not directly include QNX,
`dev-can-linux`, or future TDA4 CAN headers.

HAL selection is a build/deployment concern:

- Linux tests use the fake HAL backend.
- QNX SIL/cloud tests use the raw `devctl()` backend against
  `dev-can-linux -L` vcan endpoints or an approved `dev-can-linux` backend.
- TDA4 FD-CAN deployments use a future `tda4-fdcan` backend selected only when
  vendor headers and libraries are supplied.

The gateway remains responsible for DDS mapping, queueing, diagnostics, topic
configuration, and rejecting CAN metadata combinations that the selected DDS
type cannot preserve.

### DDS Callback and TX Queue

The DDS receive path for `/can<bus>/to_can_bus` must not perform QNX hardware
I/O in the DDS callback. The callback should validate the DDS sample and
enqueue it into a fixed-capacity TX queue.

Queue pressure policy:

- If a new frame has the same ID as an older pending frame, replace the older
  pending frame.
- Otherwise, drop the new frame and increment a diagnostic counter.
- Do not grow the queue dynamically.
- Do not block indefinitely from the DDS callback.

### Main Loop

The main loop should:

1. Drain any accepted outbound DDS frames to the QNX TX endpoint within a
   bounded per-iteration budget.
2. Poll QNX RX with `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`.
3. Publish accepted inbound frames to the configured
   `/can<bus>/from_can_bus` topic.
4. Emit diagnostics at a bounded low rate.
5. Observe shutdown state at least once per configured poll period.

The initial design should avoid multiple steady-state threads unless RTI MCP
guidance or measured target behavior shows a stronger reason for separation.

## Strict Verification Gate

This work item must link its evidence to `VSN-0033`. Do not mark it complete
on Linux fake-gateway tests alone.

Minimum evidence:

- smoke tests for gateway startup, topic/config validation, CAN HAL selection,
  readiness, and bounded shutdown
- unit tests for raw-frame validation, queue bounds, stale bus state, backend
  errors, malformed frames, and rejected CAN0 motor command TX
- hardware-equivalence routing tests for CAN1, CAN2, and CAN3 rows in the
  roadmap matrix; CAN0 tests must prove the generic gateway rejects production
  TX/RX ownership unless explicitly configured as a read-only test/mirror
- DDS integration tests with real RTI entities on isolated domains after the
  RTI gate clears
- QNX/SIL gateway smoke with `dev-can-linux` endpoint names, loopback frames,
  status codes, and timing evidence when target access exists
- regression tests proving DBC decoding remains out of the generic gateway and
  final CAN0 motor commands remain outside DDS/generic gateway routing

## Steps

- [x] Inspect current EVSN harness, runtime layout, CAN DDS types, and
      SR-Mjolnir context.
- [x] Inspect local Codex chat history for the raw CAN gateway model.
- [x] Inspect the SR-Mjolnir CAN gateway, CAN 2.0, topic registry, and
      application bridge plans.
- [x] Inspect local `dev-can-linux` driver helpers and work-item evidence.
- [x] Consult QNX online documentation for CAN raw-frame `devctl()` behavior,
      MID encoding, resource-manager client model, and error handling.
- [x] Attempt required RTI Connext MCP guidance.
- [x] Inspect EMBD-v2-HighLevel's ROS2 socketcan bridge and CAN driver topic
      usage.
- [x] Consult RTI Connext MCP for the bus-first topic-name rename.
- [ ] Rerun RTI MCP guidance successfully and record exact QoS and entity
      lifecycle consequences in this plan.
- [ ] Confirm whether implementation should wait for the EVSNNode runtime
      rewrite in `VSN-0015` or use the current runtime API behind a thin
      adapter.
- [ ] Complete `VSN-0019` HAL frame validation, fake backend, CMake backend
      selection, and QNX raw `devctl()` backend before direct gateway hardware
      integration.
- [ ] Add `evsn-runtime/can_gateway/` with pure conversion code, bounded queue
      code, diagnostics, and fake endpoint tests.
- [ ] Add hardware-equivalence route tests for CAN1/CAN2/CAN3 and CAN0
      rejection/read-only mirror behavior.
- [ ] Add the QNX-only CAN adapter shim with no public QNX header leakage.
- [ ] Add DDS integration code after RTI MCP guidance is recorded.
- [ ] Add Linux fake-CAN DDS integration tests using isolated DDS domains.
- [ ] Add optional QNX/SIL smoke script that starts `dev-can-linux`, sends a
      known frame, observes `/can<bus>/from_can_bus`, publishes
      `/can<bus>/to_can_bus`, and verifies `candump` output.
- [ ] Document deployment examples, environment variables, diagnostics, and the
      raw-boundary DBC rule in `evsn-runtime/can_gateway/README.md`.

## Verification

- [ ] Command: `./init.sh check`
      Result: passed on 2026-06-20 after the EMBD-v2 topic-name update.
- [ ] Command: `git diff --check`
      Result: passed on 2026-06-20 after the EMBD-v2 topic-name update.
- [ ] Command: `./scripts/linux-test --clean`
      Result: future implementation verification.
- [ ] Command: focused CTest for conversion and bounded queue tests
      Result: future implementation verification.
- [ ] Command: hardware-equivalence route/rejection regression
      Result: future implementation verification.
- [ ] Command: Linux fake DDS integration test for `/can<bus>/from_can_bus`
      and `/can<bus>/to_can_bus`
      Result: future implementation verification.
- [ ] Command: `./scripts/evsn build --target can_gateway`
      Result: future QNX build verification.
- [ ] Command: `VERIFY_QNX=1 ./init.sh verify`
      Result: future QNX verification.
- [ ] Command: QNX/SIL smoke with `dev-can-linux -L 1`, `cansend`, and
      `candump`
      Result: future target or SIL verification.

## Acceptance Criteria

- The gateway package builds in Linux test mode with fake CAN endpoints and in
  QNX mode with the QNX CAN adapter.
- One process instance owns exactly one configured CAN bus.
- RX frames from the configured QNX endpoint publish to the configured
  `/can<bus>/from_can_bus` topic with bounded allocation and explicit error
  handling.
- DDS frames on `/can<bus>/to_can_bus` enqueue through a fixed-capacity TX queue
  and transmit through the configured QNX endpoint without blocking DDS
  callbacks on hardware I/O.
- CAN 2.0 topics use `EVSNCan20Frame`; FD-capable topics use `EVSNFDCanFrame`
  only where current metadata limits are acceptable or explicitly resolved.
- Default topic-name tests cover `EVSN_CAN_GATEWAY_BUS_INDEX`,
  `EVSN_CAN_GATEWAY_BUS_NAME`, and explicit topic overrides.
- MID conversion tests cover standard IDs, extended IDs, invalid IDs, payload
  length limits, DLC validation, remote-frame handling, and unsupported FD
  metadata.
- Queue tests cover same-ID replacement, different-ID overflow drop, bounded
  capacity, and diagnostics.
- QNX/POSIX fallible calls are checked and mapped to documented startup,
  steady-state, or shutdown behavior.
- The README states that DBC pack/unpack belongs with CAN-ID-owning nodes, not
  with the gateway.
- Required RTI MCP and QNX online documentation evidence is recorded before
  source implementation is marked ready.

## Risks And Blockers

- Blocking: RTI Connext MCP guidance timed out. DDS QoS and implementation
  decisions must not be finalized until a successful RTI MCP result is recorded.
- Blocking for live QNX evidence: `docker/.env` is not configured for the
  licensed QNX builder in this checkout.
- `EVSNFDCanFrame` lacks `extended_id`, `remote_frame`, and explicit DLC fields.
  That limits safe FD metadata handling unless the DDS contract changes.
- `commands.h` in local driver evidence is GPL and mostly static inline. Keep
  any use quarantined in a QNX-only shim and get legal/project approval before
  copying helper code.
- `dev-can-linux` proves SIL raw-frame behavior, including 64-byte raw payloads,
  but does not prove physical CAN-FD behavior on target hardware.
- QNX target endpoint naming, bitrate setup, and latency-limit support must be
  confirmed on the deployed driver and hardware.

## Evidence

- Created from local SR-Mjolnir chat and repository inspection on 2026-06-20.
- Topic-name defaults updated from EMBD-v2-HighLevel inspection on 2026-06-20.
- Relevant log: `.agents/logs/2026-06-20-evsn-can-gateway-plan.md`.
- Work item: `VSN-0017` in `.agents/feature_list.json`.

## Handoff

Before implementation, rerun and record successful RTI Connext MCP guidance for
the exact gateway topic QoS and entity lifecycle. After that, start with pure
conversion and bounded-queue tests before touching QNX adapter or DDS runtime
code. Keep the first source change narrow: `evsn-runtime/can_gateway/` plus
focused tests and documentation.
