# 2026-06-20 - evsn-can-gateway-plan

Work item: `VSN-0017`
Plan: `.agents/exec_plans/evsn-can-gateway.md`
Status: blocked before implementation

## Actions

- Ran startup checks and preserved the existing dirty/untracked worktree.
- Read the harness context files required by `AGENTS.md`.
- Loaded the computer-use skill requested by the user and inspected available
  browser/chat context.
- Inspected local Codex session history for the prior CAN gateway discussion.
- Inspected current EVSN runtime layout and CAN DDS IDL contracts.
- Inspected local SR-Mjolnir architecture, topic registry, CAN gateway plan,
  CAN 2.0 plan, IoT bridge plan, and motor driver CAN codec context.
- Inspected local EMBD-v2-HighLevel `ros2_socketcan`, launch, `can_broker`,
  and driver files for the existing CAN bridge topic naming convention.
- Inspected local `dev-can-linux` helper API, README, and work-item evidence.
- Consulted QNX online documentation for CAN raw-frame `devctl()` usage,
  standard and extended MID encoding, `devctl()` error handling, and the QNX
  resource-manager client model.
- Attempted the required RTI Connext MCP guidance query for the gateway DDS
  design; the query timed out after approximately 120 seconds.
- Consulted the RTI Connext MCP for the narrower topic-name rename from
  `/from/can/<bus>` and `/to/can/<bus>` to `/can<bus>/from_can_bus` and
  `/can<bus>/to_can_bus`.
- Created the EVSN CAN gateway execution plan and added durable work-item
  traceability.

## Findings

- The gateway boundary should remain raw CAN: hardware frames publish to a
  bus-specific raw CAN topic, nodes filter by CAN ID and unpack locally, nodes
  pack outbound frames locally and publish to a bus-specific TX topic.
- EMBD-v2-HighLevel uses bus-first CAN bridge topics. The socketcan receiver
  publishes `from_can_bus` within a bus namespace, the sender subscribes
  `to_can_bus` within that namespace, and `can_broker` examples use
  `/can1/from_can_bus` and `/can1/to_can_bus`.
- EVSN gateway defaults now copy that convention:
  `/can<bus>/from_can_bus` and `/can<bus>/to_can_bus`, with concrete examples
  `/can0/from_can_bus` and `/can0/to_can_bus`.
- Current EVSN DDS contracts are:
  - `EVSNFDCanFrame`: `id` plus bounded 64-byte `data`.
  - `EVSNCan20Frame`: `id`, `extended_id`, `remote_frame`, `dlc`, and bounded
    8-byte `data`.
- `EVSNFDCanFrame` cannot preserve extended-ID or remote-frame metadata. The
  gateway must restrict unsupported FD metadata combinations or a future plan
  must change the DDS contract.
- QNX `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` supports deterministic polling by
  returning `EAGAIN` when no frame is available.
- QNX `CAN_DEVCTL_TX_FRAME_RAW` may return `EAGAIN` for TX queue pressure; the
  gateway must handle that without unbounded blocking.
- Standard 11-bit CAN IDs are encoded in bits 18 through 28 of `can_msg.mid`;
  extended 29-bit IDs are encoded in bits 0 through 28.
- Local `dev-can-linux` evidence supports SIL raw-frame testing and 64-byte raw
  container traffic, but not physical CAN-FD target behavior.
- RTI Connext guidance for the topic-name update reported no DDS-specific
  blocker. The rename is a system contract change: matching endpoints must use
  exactly equal topic names, compatible types, and compatible QoS. DDS
  partitions are not required for this rename because bus identity is encoded
  in the topic name.

## Blocker

The full required RTI Connext MCP guidance for gateway QoS and lifecycle timed
out. Source implementation and final DDS QoS decisions are blocked until a
successful RTI MCP query is recorded in the plan. The narrower topic-name
guidance has been recorded and permits the bus-first naming update.

## Evidence

- `.agents/exec_plans/evsn-can-gateway.md`
- `.agents/feature_list.json` entry `VSN-0017`
- EMBD-v2-HighLevel source files:
  - `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/ros2_socketcan/src/socket_can_receiver_node.cpp`
  - `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/ros2_socketcan/src/socket_can_sender_node.cpp`
  - `/Users/henryjiang/Documents/Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_broker/include/can_broker/device_driver.hpp`

## Next Action

Rerun the full RTI Connext MCP query successfully, record exact QoS and
lifecycle guidance in the execution plan, then begin implementation with pure
conversion and bounded TX queue tests under `evsn-runtime/can_gateway/`.
