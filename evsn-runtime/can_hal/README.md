# EVSN CAN HAL

`evsn_can_hal` is the target-selectable CAN hardware abstraction for EVSN CAN
gateway and vehicle-node code.

The public headers expose only portable EVSN types and standard C++ headers.
QNX raw `devctl()`, `dev-can-linux`, and future TDA4 FD-CAN driver headers must
remain private to backend source files.

Backend selection is controlled by `EVSN_CAN_BACKEND`:

- `fake`: Linux/unit-test backend with deterministic injected RX and recorded
  TX frames.
- `qnx-devctl`: QNX backend using official raw CAN `devctl()` commands.
- `qnx-dev-can-linux`: blocked placeholder for optional SIL-only extensions.
- `tda4-fdcan`: blocked placeholder until vendor artifacts and bench evidence
  are approved.
- `j784-mcan-qnx`: blocked placeholder for the future EVSN-owned QNX
  resource-manager driver for J78/J784 Bosch MCAN controllers, tracked by
  `VSN-0038`.

The QNX raw `devctl()` backend treats the official QNX `struct can_msg` ABI as
classic CAN only. `dev-can-linux` virtual CAN evidence for 64-byte containers is
SIL evidence, not physical CAN-FD target evidence.

Physical J78/J784 CAN-FD support must come through `VSN-0038` target evidence:
resource-manager ABI, MCAN instance mapping, QNX startup, internal loopback,
external CAN-FD bench capture, and public-boundary scans. Linux SocketCAN,
`dev-can-linux`, and `qnxCAN1000` are reference material for architecture and
behavior; implementation reuse needs explicit license/provenance approval.
