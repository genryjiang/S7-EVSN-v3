# VSN-0019 - QNX HAL-Linked Vcan Smoke

Date: 2026-06-24
Branch: `feature/evsn-can-abstraction-layer`
Status: passed for QNX/SIL vcan loopback through `evsn_can_hal`

## Scope

Add and verify a tiny QNX executable linked against `evsn_can_hal` so the
QNX/SIL smoke exercises the abstraction layer itself instead of only
`cansend`/`candump`.

The executable is:

- Target: `qnx_can_hal_vcan_smoke`
- Source: `evsn-runtime/can_hal/test/qnx_can_hal_vcan_smoke.cpp`
- Installed artifact: `out/qnx/bin/qnx_can_hal_vcan_smoke`

It opens `/dev/can0/rx0` and `/dev/can0/tx0` through `CanChannel`, writes a
standard classic frame and an extended classic frame, reads them back through
`CanChannel::read()`, compares the portable `CanFrame` contents, prints
diagnostics, and exits nonzero on failure.

## QNX Documentation Gate

Current official QNX documentation was consulted before changing and testing
QNX-facing behavior:

- QNX Product Documentation:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- CAN `devctl()` command index:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- C library `devctl()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/d/devctl.html

Design consequence: this smoke remains classic CAN vcan/SIL evidence through
the QNX raw `devctl()` backend. It does not prove physical CAN-FD behavior or
TDA4 controller behavior.

## Implementation

Changed files:

- `evsn-runtime/can_hal/CMakeLists.txt`
- `evsn-runtime/can_hal/test/qnx_can_hal_vcan_smoke.cpp`
- `evsn-runtime/can_hal/README.md`

The executable is QNX-only and only created when `EVSN_CAN_BACKEND` is
`qnx-devctl`. It is installed to `bin/` so the existing QNX wrapper stages it
under `out/qnx/bin`.

## Build Evidence

Command:

```sh
./scripts/qnx-build --target qnx_can_hal_vcan_smoke
```

Result: passed.

Installed artifact:

```text
out/qnx/bin/qnx_can_hal_vcan_smoke: ELF 64-bit LSB pie executable, ARM aarch64,
version 1 (SYSV), dynamically linked, interpreter /usr/lib/ldqnx-64.so.2,
BuildID[md5/uuid]=4aef54f73627e6850c479c3a0b1e8974, with debug_info,
not stripped
```

## Target Runtime Evidence

Target path:

`/Users/henryjiang/qnxprojects/targets/qemu-qnx800-aarch64`

Target identity:

```text
QNX qnx-a64 8.0.0 2025/07/30-19:17:34EDT QEMU_virt aarch64le
```

Target setup:

```sh
./dev-can-linux -L 1 -u id=0,rx=1,tx=1 -b id=0,freq=1000k
```

Observed endpoints:

```text
crw-rw-rw-  1 root root 0,   1 2026-06-24 10:26 rx0
crw-rw-rw-  1 root root 0,   1 2026-06-24 10:26 tx0
```

HAL smoke command:

```sh
./qnx_can_hal_vcan_smoke /dev/can0/rx0 /dev/can0/tx0
```

HAL smoke output:

```text
opened rx=/dev/can0/rx0 tx=/dev/can0/tx0 backend=qnx-devctl classic=1 fd=0 extended=1 remote=1
transmit: id=0x00000124 id_format=standard length=8 payload=1122334455667788
received: id=0x00000124 id_format=standard length=8 payload=1122334455667788
transmit: id=0x00001234 id_format=extended length=4 payload=aabbccdd
received: id=0x00001234 id_format=extended length=4 payload=aabbccdd
diagnostics: opens=1 closes=1 rx=2 tx=2 no_frame=1 tx_full=0 invalid=0 unsupported=0 open_failures=0 io_errors=0
qnx_can_hal_vcan_smoke: passed
```

Exit status: `0`

Cleanup:

- Killed `dev-can-linux`.
- Removed `/tmp/evsn-can-hal-smoke`.
- Follow-up target check showed no `dev-can-linux` or `qnx_can_hal_vcan_smoke`
  processes and no `/dev/can0` tree.

## Local Verification

- `bash evsn-runtime/can_hal/test/check_public_boundary.sh evsn-runtime/can_hal`
  passed.
- `git diff --check` passed.
- `./scripts/linux-test` passed all 7 CTest tests.

## Result

`VSN-0019` now has QNX/SIL vcan runtime evidence through the EVSN CAN
abstraction itself. The remaining blocker is still TDA4 FD-CAN vendor artifacts
and physical or bench evidence.
