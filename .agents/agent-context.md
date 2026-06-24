# Agent Context

This file records durable repository facts that agents must know before
implementing or reviewing work items. It is intentionally short and should only
contain information that affects how work is interpreted across tasks.

## CAN Gateway Contract

The generic CAN gateway owns direct CAN hardware access only for non-CAN0 bus
instances unless an active execution plan explicitly says otherwise. CAN0 is
the drive-control bus and is owned by the separate real-time
`evsn_can0_drive_bus_rt` process described by `VSN-0023`.

For non-CAN0 buses, the gateway receives CAN frames from hardware and publishes
each frame as an `SRFDCanFrame` or equivalent EVSN raw-CAN DDS sample on a
bus-specific topic such as `/can1/from_can_bus`.

Vehicle nodes subscribe to the relevant `/can<bus>/from_can_bus` topic, inspect
the CAN identifier, and ignore frames they do not own. When a node owns a frame,
it uses the approved DBC to unpack the frame's 8-byte payload into its local
typed representation. For example, a node interested in CAN ID `0x200` should
filter for that identifier and pass the payload to an `unpack_0x200()`-style
function.

For non-CAN0 buses, sending follows the inverse path. A node converts its local
typed command into the DBC-defined 8-byte CAN payload, wraps it in an EVSN raw
CAN frame with the correct CAN identifier, and publishes it to a bus-specific
topic such as `/can1/to_can_bus`. The gateway then transmits the raw frame on
CAN hardware.

CAN0 is different. Final CAN0 motor commands are never sent through DDS or a
generic gateway. `evsn_motor_control_rt` computes the command, hands a bounded
local request to `evsn_can0_drive_bus_rt`, and `evsn_can0_drive_bus_rt` performs
the CAN0 HAL write. CAN0 receive traffic is fanned out by
`evsn_can0_drive_bus_rt` to owning processes and may be mirrored read-only to
DDS after the RTI gate clears.

Do not assume the gateway publishes one decoded DDS topic per CAN message. The
gateway boundary is raw `SRFDCanFrame`; DBC pack/unpack belongs with the node
that owns the CAN identifier unless a later execution plan explicitly changes
that contract.

The canonical default CAN gateway topic names intentionally copy
EMBD-v2-HighLevel's bus-first socketcan bridge convention:

- RX from hardware into DDS: `/can<bus>/from_can_bus`
- TX from DDS to hardware: `/can<bus>/to_can_bus`

The earlier SR-Mjolnir provisional topic names `/from/can/<bus>` and
`/to/can/<bus>` are historical context only unless a later execution plan
explicitly restores them.

## CAN Hardware Abstraction

Target-specific CAN access belongs behind the planned `VSN-0019` CAN HAL.
Gateway and vehicle-node public headers must not include QNX CAN headers,
`dev-can-linux` helper headers, or future TDA4 vendor CAN headers.

The HAL should expose a portable fixed-size raw CAN frame model, explicit
non-throwing statuses, backend capabilities, and diagnostics. Linux tests use a
fake backend. QNX virtual/cloud testing uses the QNX raw `devctl()` path against
`dev-can-linux` vcan endpoints. Future TDA4 FD-CAN support must be added as a
private backend selected by CMake only after the board-specific headers,
libraries, API contract, and bench evidence are available.

## Motor Driver Application

For unchanged EMBD-v2-HighLevel hardware, motor command and telemetry behavior
must preserve the legacy WaveSculptor WS22 CAN protocol unless a later approved
hardware change says otherwise.

`VSN-0023` supersedes the earlier ordinary DDS motor-driver assumption:

- CAN0 is the shared drive-control bus, not a motor-only bus.
- `evsn_motor_control_rt` computes final motor commands.
- `evsn_can0_drive_bus_rt` owns the CAN0 HAL endpoint and performs all CAN0
  writes.
- Do not route final CAN0 motor commands through DDS.
- Do not route final CAN0 motor commands through a generic CAN gateway node.
- Do not make telemetry responsible for sending final motor commands.
- A lower-priority DDS status/telemetry adapter may publish copied snapshots,
  but it must not back-pressure or influence the real-time motor-control loop.

The legacy motor compatibility surface is:

- final drive command frame: standard CAN ID `0x501`
- motor power command frame, if retained: standard CAN ID `0x502`
- motor reset command frame, if retained: standard CAN ID `0x503`
- left motor telemetry: standard CAN IDs `0x400..0x417`
- right motor telemetry: standard CAN IDs `0x420..0x437`
- old command sources: `/control/motors/current`, `/control/motors/velocity`,
  and `/car/motors/reset`
- new final command path: bounded local IPC from `evsn_motor_control_rt` to
  `evsn_can0_drive_bus_rt`, not DDS
- new telemetry path: CAN0 receive fanout to a motor telemetry decoder/status
  adapter, with DDS status publication outside the hot path after RTI guidance

The active safe-state output, stale-input thresholds, and reset/power-command
policy must be defined in `VSN-0023` against the WS22 compatibility surface
before implementation.
