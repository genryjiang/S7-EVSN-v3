# Session Handoff

Last updated: 2026-06-24

## Resume Path

1. Run `git status --short`.
2. Read `AGENTS.md`.
3. Read `.agents/feature_list.json` and choose the active work item.
4. Read `.agents/agent-context.md`.
5. Read the matching execution plan in `.agents/exec_plans/` when one exists.
6. Read the latest log in `.agents/logs/`.
7. Continue only after recording any discovered user changes.

## Current Feature

Feature: `VSN-0019` / `evsn-can-abstraction-layer`

Goal: Provide a target-selectable CAN HAL so gateway and vehicle-node code can
use fake Linux tests, QNX raw `devctl()` endpoints, and future TDA4 FD-CAN
drivers without leaking target-specific headers into portable code.

Current state:

- Execution plan: `.agents/exec_plans/evsn-can-abstraction-layer.md`.
- Work item: `VSN-0019`.
- Status: in_progress.
- Branch status: the checkout reports no HEAD commit, so normal branch setup is
  still not meaningful even though the current branch name exists.
- Work item IDs use uppercase `VSN-XXXX` format; do not create legacy work-item
  IDs.
- Implemented package: `evsn-runtime/can_hal/`.
- Public HAL headers expose portable EVSN/standard C++ types only:
  `CanFrame`, `CanResult`, `CanOpenConfig`, `CanCapabilities`,
  `CanDiagnostics`, and `CanChannel`.
- `EVSN_CAN_BACKEND` defaults to `fake` for Linux and `qnx-devctl` for QNX.
  `qnx-dev-can-linux` and `tda4-fdcan` intentionally fail configure until
  approval/vendor evidence exists.
- The fake backend is deterministic and fixed-capacity, supporting injected RX,
  recorded TX, and forced status tests.
- The QNX raw backend includes only official QNX headers, uses
  `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK` and `CAN_DEVCTL_TX_FRAME_RAW`, maps RX
  `EAGAIN` to `no_frame`, maps TX `EAGAIN` to `tx_full`, handles standard MID
  bits 18..28 and extended MID bits 0..28, and treats official raw QNX
  `struct can_msg` as classic CAN only.
- QNX raw timestamps are not exposed as monotonic HAL timestamps yet because
  the unit/clock contract is not accepted.
- `dev-can-linux/commands.h` remains excluded from EVSN HAL public headers and
  source because it is GPL helper code; `dev-can-linux` evidence is SIL-only.
- QNX-only smoke executable `qnx_can_hal_vcan_smoke` is linked against
  `evsn_can_hal` and installed by `./scripts/qnx-build --target
  qnx_can_hal_vcan_smoke`.
- Researcher subagent `019eeda3-d4c2-7e82-b550-4e0ba3927ce4` and reviewer
  subagent `019eedab-5439-7122-b73a-eca8466293be` were used for `VSN-0019`.
- Verification passed:
  - `./scripts/linux-test`
  - `./scripts/qnx-build --target evsn_can_hal`
  - `bash evsn-runtime/can_hal/test/check_public_boundary.sh evsn-runtime/can_hal`
  - QNX/SIL vcan smoke on
    `/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64` using
    `dev-can-linux -L 1`, `cansend`, and `candump`
  - QNX/SIL HAL-linked vcan smoke on the same target using `dev-can-linux -L 1`
    and `qnx_can_hal_vcan_smoke /dev/can0/rx0 /dev/can0/tx0`
  - `git diff --check`
  - `./init.sh check`
- Durable logs:
  - `.agents/logs/2026-06-22-evsn-can-abstraction-layer-implementation.md`.
  - `.agents/logs/2026-06-22-evsn-can-hal-qnx-sil-smoke.md`.
  - `.agents/logs/2026-06-24-evsn-can-hal-qnx-linked-smoke.md`.
- QNX/SIL vcan runtime smoke is captured, but it does not prove physical CAN-FD
  timing or target-controller behavior.
- TDA4 FD-CAN remains blocked on vendor headers, libraries, ABI, license terms,
  target architecture, and bench evidence.
- Board-level direct J784S4 MCAN ownership is now tracked separately by
  `VSN-0038` in `.agents/exec_plans/evsn-j784s4-mcan-libcan-driver.md`.
  That plan frames the work as a QNX `libcan` resource-manager driver
  (`devcan-mcan-j784s4`), not a Linux SocketCAN driver recompile.

## Previous Planning Context

Feature: `VSN-0020` / `evsn-network-port-roadmap`

Goal: Keep the EMBD-v2-HighLevel network migration plans aligned with unchanged
hardware and the EVSN QNX/RTI framework.
- The active roadmap now contains a hardware-equivalence matrix copied from
  EMBD-v2-HighLevel launch/config sources: bus, frame ID, direction, old ROS2
  topic names, new QNX process owner, candidate DDS topic, and regression test
  ID.
- CAN0 is the drive-control bus owned by `evsn_can0_drive_bus_rt`. Final motor
  command frames are handed to that process through bounded QNX-local IPC, not
  DDS and not the generic CAN gateway.
- CAN0 receive traffic must fan out to local subscribers for telemetry and
  decode processes. The generic gateway owns non-CAN0 buses by default.
- The old WS22 motor command surface is preserved as compatibility evidence:
  final drive command ID `0x501`, optional power/reset IDs `0x502` and `0x503`,
  left telemetry IDs `0x400..0x417`, and right telemetry IDs `0x420..0x437`.
- Steering ECU/EPAS is explicitly retired. EMBD-v2-HighLevel `can_broker`
  included CAN1 steering frames `0x290`, `0x292`, and `0x298`, but the user
  confirmed the steering actuator motor has been removed. Full MVP equivalence
  therefore requires proving those frames and `/control/steering/angle` are not
  active EVSN routes or public contracts.
- Centre display and the old `display_driver` output path are explicitly
  absent from the MVP. The driver display concept is shelved to `VSN-0037`,
  where it will be rewritten last as an HDMI Qt/QML frontend instead of the old
  USB/serial display path. `VSN-0026` covers active lighting drivers only:
  proprietary Audi headlights, indicators, and taillights.
- Reviewer subagent `019eed7c-4c11-7592-8cd7-41ed5a9ce327` found the plans
  `planning-equivalent` against the active EMBD-v2-HighLevel network minus
  retired systems. This is not implemented or verified equivalence.
- A second read-only reviewer subagent
  `019eed8a-95e4-7243-9e47-afd6d9313bc6` rechecked core functionality after
  the display deferral and returned `core-planning-equivalent`. This means the
  core network/control MVP is covered at planning level only; implementation,
  RTI/QNX gates, package tests, timing, and HIL/vehicle evidence remain open.
- The roadmap now includes a launch-file traceability checklist that maps each
  `system_launch.xml` surface to an EVSN owner or explicit
  retirement/optional decision.
- The Orion 2 BMS work item summary has been corrected to `0x620..0x629`.
- The cloud telemetry bridge and regression plans now explicitly reject old
  cloud-originated motor reset, motor power, motor limit,
  torque/current/velocity, drive-enable, and final CAN0 command paths unless a
  future approved command-authority plan exists.
- Durable log for the matrix update:
  `.agents/logs/2026-06-22-evsn-hardware-equivalence-matrix.md`.
- Durable log for the core recheck:
  `.agents/logs/2026-06-22-evsn-core-equivalence-recheck.md`.
- Separate pending implementation work remains in `VSN-0021` through
  `VSN-0036`, and their linked execution plans. `VSN-0037` is intentionally
  shelved until last.

## Critical Domain Defaults

Default project assumptions come from SR-Mjolnir:

- QNX Neutrino RTOS and embedded C++ constraints matter.
- RTI Connext DDS is the application-level middleware.
- CAN gateway boundaries use raw CAN DDS topics for non-CAN0 buses unless an
  execution plan explicitly changes that contract. CAN0 is special: the
  dedicated drive-control-bus owner handles writes and read-side fanout/mirror
  behavior.
- Generated code should be described as MISRA C++-oriented or as supporting
  future ISO 26262 evidence only when evidence supports the narrower claim.

## Verification

- Run `./init.sh check` after harness edits.
- For application changes, run the focused commands listed on the relevant
  `.agents/feature_list.json` work item and record evidence before handoff.
