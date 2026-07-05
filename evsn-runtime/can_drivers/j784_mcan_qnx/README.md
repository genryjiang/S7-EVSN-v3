# J784 MCAN QNX Driver

This package contains the EVSN-owned QNX resource-manager path for the Bosch
MCAN controllers on the TI J78/J784 platform. It sits below a portable CAN
HAL. Application code should not include these headers directly unless it is
inside this private driver package or an explicit test harness.

Current status: the driver is implemented far enough for host tests, QNX
builds, internal-loopback target smoke, IRQ/IST internal-loopback smoke, QNX
classic CAN compatibility smoke, and guarded PROC141E5 transceiver bring-up
commands. The production goal is to port the Linux SDK-owned PROC141E5 MCAN
paths to QNX ownership. It is not production external CAN-FD until physical
connector, QNX startup ownership, transceiver, and analyzer evidence are
recorded.

## What It Owns

The driver is intended to own the PROC141E5 MCAN hardware paths that the
inspected Linux SDK image enabled, from a QNX user-space resource manager:

- MCAN controller configuration and message RAM layout.
- Private EVSN CAN/CAN-FD frame transfer ABI.
- Optional classic QNX CAN compatibility endpoints.
- QNX resource-manager `read()`, `write()`, `devctl()`, and `ionotify()` paths.
- QNX interrupt service thread for confirmed logical IRQs.
- Board-specific PROC141E5 transceiver bring-up actions.

The required Linux-to-QNX porting scope is:

- `MCU_MCAN0` on J42.
- `MCU_MCAN1` on J43.
- `MAIN_MCAN4` on J45.
- `MAIN_MCAN16` on J46.

The driver must not double-own MCAN instances that are already owned by R5F
firmware, MCAL, EthFw, a TI BSP driver, or another QNX process.

## Important Boundaries

The package has three different readiness levels:

| Level | Meaning | Current state |
| --- | --- | --- |
| Build/test readiness | Host tests and QNX AArch64 build pass. | Implemented. |
| Internal hardware readiness | QNX can map MCAN registers/message RAM and run internal loopback. | Proven on MCU_MCAN1 in earlier EVM smoke. |
| Production external CAN-FD | Frames move through real pins, transceiver, connector, cable/analyzer, IRQ path, and recovery paths. | Not yet proven. |

Do not enable the production `j784-mcan-qnx` CAN HAL backend until the third
level has been recorded.

## Source Map

| File | Purpose |
| --- | --- |
| `include/.../config.hpp` | Bounded config types, status codes, endpoint path construction, hardware evidence gates. |
| `include/.../devctl_abi.hpp` | EVSN-private frame, capability, diagnostic, and devctl ABI types. Supports CAN-FD payloads up to 64 bytes. |
| `include/.../mcan_core.hpp` and `src/mcan_core.cpp` | Host-testable MCAN core model with explicit states, timing validation, bounded queues, and diagnostics. |
| `include/.../mcan_registers.hpp` and `src/mcan_registers.cpp` | EVSN-owned MCAN register programming helpers. |
| `include/.../mcan_message_ram.hpp` and `src/mcan_message_ram.cpp` | Message RAM layout validation and TX/RX element encode/decode helpers. |
| `include/.../j784_board_config.hpp` and `src/j784_board_config.cpp` | J784/PROC141E5 MCAN instance metadata, base addresses, IRQ metadata, connector evidence, and explicit-enable board notes. |
| `include/.../qnx_mmio_mapper.hpp` and `src/qnx_mmio_mapper.cpp` | QNX-only MMIO mapping wrapper; fails closed on non-QNX hosts. |
| `include/.../resource_manager_runtime.hpp` and `src/resource_manager_runtime.cpp` | Host-testable bounded runtime used by software-loopback resource-manager smoke. |
| `include/.../qnx_can_compat.hpp` and `src/qnx_can_compat.cpp` | NXP/FlexCAN-style startup parsing and classic QNX CAN ABI conversion. |
| `include/.../transceiver_control.hpp` and `src/transceiver_control.cpp` | Bounded PROC141E5 transceiver action plans and validation. |
| `src/resource_manager_qnx.cpp` | QNX resource-manager executable, target smoke modes, MMIO-backed loopback, IRQ/IST path, compatibility endpoints, and transceiver apply/probe commands. |
| `bsp/` | EVSN-owned QNX BSP startup and mkifs/buildfile overlay fragments for porting the Linux-enabled MCAN ownership into the QNX IFS. |
| `test/` | Host unit tests and boundary scans. |

## Runtime Interfaces

### Private EVSN Interface

The private namespace is the preferred driver ABI for EVSN-owned CAN-FD
functionality:

```text
/dev/evsn/mcan<N>/rx<M>
/dev/evsn/mcan<N>/tx<M>
/dev/evsn/mcan<N>/ctl
```

Use this path for:

- CAN-FD frames.
- BRS and ESI flags.
- 64-byte payloads.
- EVSN-private capability and diagnostic devctls.
- MCAN-specific controls.

The private frame type is `McanFrameTransfer`. It is versioned and fixed-size
so callers can validate ABI compatibility before trusting payload data.

### QNX Compatibility Interface

The optional compatibility namespace mirrors the classic QNX CAN driver shape:

```text
/dev/can<N>/rx<M>
/dev/can<N>/tx<M>
```

This path exists for classic CAN interoperability with QNX tools or code that
expects QNX raw CAN devctls. It supports classic 8-byte `struct can_msg`
conversion only. Do not use it for CAN-FD payloads.

Before attaching `/dev/can<N>/*`, the driver probes for namespace collisions so
it does not silently take over a path owned by a vendor/native QNX CAN driver.

## Board Profile

The default board evidence is `PROC141E5`.

| Endpoint | Physical instance | Linux reference device-tree node | Linux reference CAN interface | Board connector | Notes |
| --- | --- | --- | --- | --- | --- |
| 0 | `MCU_MCAN0` | `can@40528000` | `mcu_mcan0` | J42 | TCAN1042 standby via `WKUP_GPIO0_69`. |
| 1 | `MCU_MCAN1` | `can@40568000` | `mcu_mcan1` | J43 | TCAN1042 standby via `WKUP_GPIO0_2`. Internal-loopback and IRQ smoke used this endpoint. |
| 2 | `MAIN_MCAN4` | `can@2741000` | `main_mcan4` | J45 | Requires explicit enable. The inspected Ethernet-firmware overlay disables `MAIN_MCAN4`, `transceiver3`, and `mux1`. |
| 3 | `MAIN_MCAN16` | `can@26a1000` | `main_mcan16` | J46 | No standby GPIO in the inspected DTB/PDK evidence. |

The Linux reference device-tree and CAN-interface evidence, plus the PDK
external-loopback notes, define the required hardware scope for the QNX port:
QNX must own the same four MCAN paths that the Linux SDK image enabled unless a
later board decision explicitly de-scopes one.
The PDK notes identify J42/J43 as a pair and J45/J46 as a pair. That is the
bring-up target and acceptance criterion for this QNX driver, but it is not by
itself proof that the production QNX boot image already owns those controllers,
IRQs, pins, clocks, firewall resources, or transceiver controls.

## QNX BSP And IFS Integration

The Linux SDK is not used at runtime. Its DTB is evidence for the board paths
that must be ported. The equivalent QNX ownership is carried by the BSP startup
and IFS/buildfile integration under `bsp/`.

The startup overlay fragment is intended for the licensed TI QNX BSP file:

```text
psdkqa/qnx/bsp/src/hardware/startup/boards/ti-j7/j784s4/evm/hw_init.c
```

It adds `evsn_init_mcan()` for:

- TI SCI device/clock/reset enablement for `MCU_MCAN0`, `MCU_MCAN1`, and
  `MAIN_MCAN16`.
- QNX startup padmux for `MCU_MCAN0`, `MCU_MCAN1`, and `MAIN_MCAN16`.
- Guarded `MAIN_MCAN4` device/clock/reset and padmux, enabled only after the
  EthFw mux/transceiver conflict is intentionally resolved.
- WKUP GPIO standby control for `MCU_MCAN0` and `MCU_MCAN1`.

The buildfile fragment targets:

```text
psdkqa/qnx/bsp/BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342/images/j784s4-evm-ti.build
```

It stages `dev-evsn-mcan-j784` by default. Its startup commands are commented
bench-image examples until target evidence exists, including transceiver apply
and the production-named external resource-manager mode:

```sh
dev-evsn-mcan-j784 --external-can-resource-manager-endpoint <id> <irq>
```

That command is the interrupt-driven hardware path with Bosch internal
loopback disabled. Candidate endpoint launches remain commented until target
ownership, pinmux, transceiver, IRQ, and external analyzer evidence are
recorded.

## Transceiver Bring-Up

The transceiver layer has a no-write probe mode and explicit write modes.

```sh
dev-evsn-mcan-j784 --probe-transceivers
dev-evsn-mcan-j784 --enable-transceivers
dev-evsn-mcan-j784 --enable-transceivers-main-mcan4 /dev/i2cX
```

Behavior:

- `--probe-transceivers` prints deterministic planned actions and performs no
  MMIO or I2C writes.
- `--enable-transceivers` applies only actions that do not require explicit
  Ethernet-firmware conflict override:
  `MCU_MCAN0`, `MCU_MCAN1`, and the `MAIN_MCAN16` no-op.
- `--enable-transceivers-main-mcan4 <i2c-dev>` explicitly opts into the
  `MAIN_MCAN4` TCA6424 path that requires explicit Ethernet-firmware conflict
  override.

PROC141E5 action table:

| Instance | Backend | Action |
| --- | --- | --- |
| `MCU_MCAN0` | WKUP GPIO MMIO | Drive `WKUP_GPIO0_69` low. |
| `MCU_MCAN1` | WKUP GPIO MMIO | Drive `WKUP_GPIO0_2` low. |
| `MAIN_MCAN4` | TCA6424 I2C | Drive port0 pin7 low, port1 pins 5/6/7 high. Requires explicit Ethernet-firmware conflict override. |
| `MAIN_MCAN16` | None | No standby GPIO action required. |

WKUP GPIO writes preload the requested output level through set/clear data and
then configure the pin as output. TCA6424 writes use QNX `DCMD_I2C_SENDRECV`
and `DCMD_I2C_SEND` for bounded register read-modify-write.

## Resource-Manager Modes

The executable has several explicit bring-up modes. The default mode registers
the private resource-manager namespace with frame I/O gated off.

| Command | Purpose |
| --- | --- |
| `--validate-config-only` | Validate the default skeleton config. |
| `--attach-smoke` | Register default resource-manager paths and exit through smoke logic. |
| `--dry-run-hardware-config` | Validate board mapping, register programming, and message RAM layout without MMIO access. |
| `--validate-production-ownership-scope` | Build the four-controller Linux-to-QNX production scope and fail closed until all required QNX ownership, startup, transceiver, IRQ, EthFw, and bench evidence is present. |
| `--probe-hardware` | Read-only probe for the default endpoint hardware mapping. |
| `--probe-all-hardware` | Read-only probe for all mapped MCAN instances. |
| `--internal-loopback-smoke` | Hardware internal-loopback smoke using default endpoint selection. |
| `--internal-loopback-smoke-endpoint <id>` | Hardware internal-loopback smoke for one endpoint. |
| `--software-loopback-resource-manager` | Run private resource manager with bounded software-loopback frame I/O. |
| `--resource-manager-client-smoke[-endpoint <id>]` | Client smoke for private endpoints. |
| `--hardware-loopback-resource-manager-endpoint <id>` | MMIO-backed internal-loopback resource-manager mode. |
| `--hardware-irq-loopback-resource-manager-endpoint <id> <irq>` | MMIO-backed internal-loopback resource-manager mode using QNX IST. |
| `--external-can-resource-manager-endpoint <id> <irq>` | Production-named external-bus resource-manager mode using QNX IST with Bosch internal loopback disabled. |
| `--qnx-can-compat-*` | Equivalent classic QNX `/dev/canN/*` compatibility smokes. |
| NXP-style arguments | Parses FlexCAN-like startup options for classic QNX compatibility. |

## Interrupt Path

The QNX IST path is enabled only when a controller config has confirmed logical
IRQ evidence. The implemented path attaches an interrupt thread, enables MCAN
RX FIFO0, TX complete/cancellation, bus-off, warning, error-passive, protocol,
and message RAM access interrupts, and wakes bounded waiters from the
resource-manager path.

Known evidence:

- MCU_MCAN1 endpoint 1 passed interrupt-driven internal-loopback smoke with
  logical IRQ `867`.
- Candidate IRQ `899` failed the same smoke with RX `EAGAIN`.

This proves the internal-loopback IRQ path for that target setup. It does not
prove connector pinmux, transceiver state, or external bus behavior.

## Build And Test

Host tests:

```sh
./scripts/linux-test
```

QNX target build:

```sh
./scripts/qnx-build --target dev-evsn-mcan-j784
```

Useful static/boundary checks:

```sh
bash evsn-runtime/can_drivers/j784_mcan_qnx/test/check_private_boundary.sh evsn-runtime/can_drivers/j784_mcan_qnx
bash evsn-runtime/can_drivers/j784_mcan_qnx/test/check_no_ipc_path.sh evsn-runtime/can_drivers/j784_mcan_qnx
bash evsn-runtime/can_drivers/j784_mcan_qnx/test/check_no_gpl_source_copy.sh evsn-runtime/can_drivers/j784_mcan_qnx
git diff --check
```

Current known harness caveat: `./init.sh check` may still fail on pre-existing
imported `vcan-drivers` Markdown links. That is not evidence against this
driver package.

## What Still Needs Verification

Before production external CAN-FD can be claimed, collect evidence for:

- Transceiver probe/apply on the real J784 QNX target.
- QNX startup pinmux and pad configuration for `MCU_MCAN0`, `MCU_MCAN1`,
  `MAIN_MCAN4`, and `MAIN_MCAN16`, unless any path is formally de-scoped.
- Clock, reset, firewall, and resource ownership state.
- A72/QNX ownership versus R5F firmware, MCAL, and EthFw for the Linux
  SDK-enabled MCAN hardware being ported to QNX.
- Physical external CAN-FD loopback or analyzer capture.
- Error-path behavior: bus-off, error-passive, RX overflow, TX cancellation,
  shutdown cleanup, and recovery.
- HAL policy that intentionally enables the `j784-mcan-qnx` backend only after
  physical evidence exists.

The code-level production-scope validator intentionally requires all four
Linux SDK-enabled paths by default. A path can pass out of scope only through an
explicit de-scope policy with a reason; `MAIN_MCAN4` additionally requires the
EthFw mux/transceiver conflict to be resolved before the full production scope
can validate.

See `docs/verification-guide.md` for the evidence checklist.
