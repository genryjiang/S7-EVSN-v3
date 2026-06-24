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

The QNX raw `devctl()` backend treats the official QNX `struct can_msg` ABI as
classic CAN only. `dev-can-linux` virtual CAN evidence for 64-byte containers is
SIL evidence, not physical CAN-FD target evidence.

## QNX/SIL HAL Smoke

`qnx_can_hal_vcan_smoke` is a small QNX-only executable linked against
`evsn_can_hal`. It opens CAN endpoints through `CanChannel`, writes a standard
classic frame and an extended classic frame, then waits for the same frames to
return through the HAL read path.

Build it with:

```sh
./scripts/qnx-build --target qnx_can_hal_vcan_smoke
```

On a QNX target with `dev-can-linux` available:

```sh
dev-can-linux -L 1 -u id=0,rx=1,tx=1 -b id=0,freq=1000k &
qnx_can_hal_vcan_smoke /dev/can0/rx0 /dev/can0/tx0
```

If no paths are supplied, the smoke defaults to `/dev/can0/rx0` and
`/dev/can0/tx0`. A passing run verifies the `qnx-devctl` HAL backend against
vcan loopback only; it is still not physical CAN-FD or TDA4 target evidence.
