# J784 MCAN QNX Verification Guide

This guide defines the evidence needed to review and eventually promote the
J784 MCAN QNX driver from bring-up status to production external CAN-FD use.
It is intentionally stricter than "the binary builds" or "internal loopback
works".

Use this guide with work item `VSN-0038`.

## Evidence Levels

| Evidence level | What it proves | What it does not prove |
| --- | --- | --- |
| Host unit tests | Pure logic, ABI validation, bounded queues, board metadata, conversion rules. | QNX API behavior, hardware access, physical CAN. |
| QNX build | Code compiles and links for AArch64 QNX with the target sysroot. | Runtime correctness on the EVM. |
| QEMU smoke | Resource-manager startup and namespace behavior on QNX-like userspace. | J784 registers, IRQs, transceivers, pins, CAN-FD bus timing. |
| J784 internal loopback | A72/QNX can touch MCAN registers/message RAM and run internal MCAN TX/RX. | Connector pinmux, transceiver enablement, external bus behavior. |
| J784 IRQ/IST internal loopback | Logical IRQ and interrupt-driven resource-manager path work internally. | External CAN wiring or CAN-FD analyzer behavior. |
| External loopback/analyzer | Physical CAN traffic reaches the connector and bus at the expected format and bitrate. | Long-term reliability without additional endurance/fault tests. |

## Current Verified Baseline

Already recorded for VSN-0038:

- Host tests pass through `./scripts/linux-test`.
- QNX build passes through `./scripts/qnx-build --target dev-evsn-mcan-j784`.
- Private boundary, no-IPC, and no-GPL-copy scans pass.
- Physical J784 EVM read-only MCAN register probe passed for the mapped
  instances in earlier bring-up.
- MCU_MCAN1 endpoint 1 passed hardware internal-loopback smoke.
- MCU_MCAN1 endpoint 1 passed IRQ/IST-backed internal-loopback smoke with
  logical IRQ `867`.
- Classic QNX `/dev/canN/rxM` and `/dev/canN/txM` compatibility smoke passed in
  software and hardware internal-loopback modes.
- PROC141E5 transceiver action planning and QNX build integration exist.

Still missing:

- Real transceiver apply evidence on the J784 target.
- Runtime pad/pinmux readback evidence.
- Clock, reset, firewall, and ownership evidence.
- Applied QNX BSP startup/IFS evidence for the Linux SDK-enabled MCAN paths.
- External CAN-FD bus capture or physical loopback evidence.

## QNX BSP And IFS Evidence

Before external CAN tests, prove the production QNX image has replaced the
Linux SDK ownership setup for the in-scope MCAN paths.

Apply or inspect:

```text
evsn-runtime/can_drivers/j784_mcan_qnx/bsp/j784s4_evm_mcan_startup_overlay.c
evsn-runtime/can_drivers/j784_mcan_qnx/bsp/j784s4-evm-ti.mcan.buildfile.fragment
```

Expected BSP integration points:

```text
psdkqa/qnx/bsp/src/hardware/startup/boards/ti-j7/j784s4/evm/hw_init.c
psdkqa/qnx/bsp/BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342/images/j784s4-evm-ti.build
```

Evidence to record:

```text
BSP source tree path:
hw_init.c diff or overlay application proof:
evsn_init_mcan() call location:
EVSN_ENABLE_MAIN_MCAN4_STARTUP value and rationale:
buildfile diff or fragment application proof:
IFS image path/hash:
startup console output:
```

Minimum expectations:

- `evsn_init_mcan()` runs while control MMR access is unlocked.
- TI SCI enables clocks/resets for `MCU_MCAN0`, `MCU_MCAN1`, and
  `MAIN_MCAN16`.
- Startup padmux covers `MCU_MCAN0`, `MCU_MCAN1`, and `MAIN_MCAN16`.
- `MAIN_MCAN4` device/clock/reset and padmux are enabled only after the EthFw
  conflict is resolved, or a formal de-scope decision is recorded.
- The IFS stages `dev-evsn-mcan-j784`.
- The IFS does not auto-start external resource managers until target evidence
  exists; bench-image launch commands must be explicitly uncommented for each
  enabled path.

## Host Verification

Run:

```sh
./scripts/linux-test
```

Expected:

- All CTest tests pass.
- `j784_mcan_qnx_transceiver_control_test` confirms:
  - `MCU_MCAN0` uses WKUP GPIO0 line 69.
  - `MCU_MCAN1` uses WKUP GPIO0 line 2.
  - `MAIN_MCAN4` uses TCA6424 port/pin actions that require explicit
    Ethernet-firmware conflict override.
  - `MAIN_MCAN16` is a no-standby no-op.
  - probe mode validates without requiring MMIO/I2C backends.
  - apply mode fails explicitly when explicit-enable or I2C evidence is
    missing.
- `j784_mcan_qnx_qnx_can_compat_test` confirms classic-only QNX compatibility
  conversions and startup parsing behavior.
- Boundary scans confirm no public HAL leakage and no copied GPL/vendor driver
  source.

Record:

```text
date:
git branch:
git rev-parse HEAD:
command:
result:
failing tests, if any:
```

## QNX Build Verification

Run:

```sh
./scripts/qnx-build --target dev-evsn-mcan-j784
file out/qnx/bin/dev-evsn-mcan-j784
```

Expected:

- Build succeeds.
- `file` reports AArch64 QNX ELF executable.
- Record the BuildID.

Record:

```text
date:
command:
BuildID:
builder/container:
QNX SDP/sysroot version, if known:
```

## QNX Target Deployment

Use the working target account for the board. Earlier evidence used root SSH on
`192.168.0.140`; `qnxuser` access was not consistently authorized.

Example:

```sh
scp out/qnx/bin/dev-evsn-mcan-j784 root@192.168.0.140:/tmp/dev-evsn-mcan-j784
ssh root@192.168.0.140 'chmod +x /tmp/dev-evsn-mcan-j784'
```

Record:

```text
target hostname:
uname -a:
binary path:
binary BuildID:
operator:
```

## Safe Target Probes

These commands are safe because they do not intentionally transmit on the
external CAN bus.

```sh
/tmp/dev-evsn-mcan-j784 --validate-config-only
/tmp/dev-evsn-mcan-j784 --dry-run-hardware-config
/tmp/dev-evsn-mcan-j784 --probe-all-hardware
/tmp/dev-evsn-mcan-j784 --probe-transceivers
```

Expected:

- Config validation passes.
- Hardware dry-run passes without MMIO writes.
- Hardware probe maps/read-only probes the MCANSS/controller/message RAM.
- Transceiver probe prints action plans and reports `write=no`.

Evidence to save:

- Full command output.
- `slog2info` excerpts for MCAN/CAN/errors.
- Target identity from `uname -a`.

## Transceiver Apply Evidence

Run only when the board is safe for CAN transceiver enablement.

Actions that do not require explicit Ethernet-firmware conflict override:

```sh
/tmp/dev-evsn-mcan-j784 --enable-transceivers
```

MAIN_MCAN4 path requiring explicit Ethernet-firmware conflict override:

```sh
/tmp/dev-evsn-mcan-j784 --enable-transceivers-main-mcan4 /dev/i2cX
```

Do not run the MAIN_MCAN4 command until EthFw ownership is understood. The
inspected Ethernet-firmware overlay disables `MAIN_MCAN4`, `transceiver3`, and
`mux1`.

Expected:

- `MCU_MCAN0` and `MCU_MCAN1` WKUP GPIO actions report successful writes.
- `MAIN_MCAN4` is skipped by default and only applied through explicit opt-in.
- Missing MAIN_MCAN4 I2C device fails with `missing_argument=i2c-dev` or
  `hardware_access_unavailable`, not silent startup.

Recommended physical confirmation:

- Scope or meter TCAN1042 standby lines before and after apply.
- Read back relevant WKUP GPIO output/direction registers.
- For MAIN_MCAN4, read TCA6424 output/config registers before and after.

## Ownership Evidence

Goal: prove QNX/A72 owns the MCAN hardware that is being ported from the
Linux SDK image into the production QNX image.

Required PROC141E5 ownership scope:

- `MCU_MCAN0` on J42.
- `MCU_MCAN1` on J43.
- `MAIN_MCAN4` on J45.
- `MAIN_MCAN16` on J46.

These are the Linux SDK-enabled MCAN paths and connector pairs that the QNX
driver is expected to replace. Treat the Linux reference device-tree nodes,
Linux reference CAN interface names, and PDK loopback evidence as the source of
the QNX porting target, not as proof that the QNX production image has already
configured or owns those resources. If any of the four paths is intentionally
removed from production scope, record the board decision and reason before
closing VSN-0038.

Code-level guard:

```sh
/tmp/dev-evsn-mcan-j784 --validate-production-ownership-scope
```

Expected before physical production evidence is complete:

```text
production_config_status=ok
production_ownership_scope_status=missing_hardware_evidence
```

This command builds the four-controller production scope and intentionally
fails until every in-scope Linux SDK-enabled MCAN path has QNX ownership,
startup, transceiver, IRQ, EthFw, and external bench evidence. It is not a
hardware probe and performs no MMIO or I2C writes.

External resource-manager launch after BSP/IFS integration and target evidence:

```sh
/sbin/dev-evsn-mcan-j784 --external-can-resource-manager-endpoint 0 864
/sbin/dev-evsn-mcan-j784 --external-can-resource-manager-endpoint 1 867
/sbin/dev-evsn-mcan-j784 --external-can-resource-manager-endpoint 3 816
```

Treat endpoint 0 and endpoint 3 IRQs as candidates until target IRQ evidence
exists. Only add endpoint 2 (`MAIN_MCAN4`, candidate IRQ `168`) after the EthFw
disable path is resolved and the TCA6424 mux/standby path has been tested.

Commands:

```sh
pidin ar
ls -la /dev/can* /dev/evsn/mcan* 2>/dev/null
slog2info | grep -i -E 'mcan|can|ethfw|mcal|r5f'
```

Evidence:

- No vendor/native CAN driver owns the same `/dev/canN` namespace.
- No R5F/MCAL/EthFw process or startup configuration owns any in-scope MCAN
  controller, IRQ, message RAM, pins, or transceiver path.
- QNX startup configuration owns the in-scope MCAN pinmux, clock/reset state,
  interrupt routing, firewall/resource assignment, and transceiver controls.
- For `MAIN_MCAN4`, prove the EthFw overlay/path is absent, disabled for this
  board image, or explicitly not owning/disabling `MAIN_MCAN4`,
  `transceiver3`, and `mux1`. If production chooses not to port
  `MAIN_MCAN4`, record the de-scope decision instead of treating it as
  silently optional.

Per-instance ownership record:

```text
MCAN instance:
Linux reference device-tree node:
Linux reference CAN interface name:
QNX production owner process:
QNX startup source/buildfile:
controller base:
message RAM base:
logical IRQ:
pinmux/padconf evidence:
clock/reset evidence:
firewall/resource-owner evidence:
transceiver-control evidence:
R5F/MCAL/EthFw conflict result:
reviewer:
```

Do not treat internal loopback as ownership proof for external CAN. It proves
register access and MCAN datapath only.

## Pinmux And Pad Evidence

Goal: prove the SoC pins route the selected MCAN TX/RX signals to the intended
connector.

Evidence sources:

- PROC141E5 schematic and pinmux file.
- QNX startup source/buildfile/pad configuration.
- Runtime pad configuration register reads.
- Oscilloscope or CAN analyzer signal on the connector during transmit.

Minimum review record:

```text
MCAN instance:
TX pad:
RX pad:
connector:
padconf register addresses:
runtime padconf values:
expected mux mode:
actual mux mode:
reviewer:
```

## Clock, Reset, IRQ, And Firewall Evidence

Goal: prove the controller is usable in the production boot state.

Evidence:

- MCAN identity/status registers respond through `--probe-all-hardware`.
- Clock-select and reset registers show the selected MCAN clock is enabled and
  reset is released.
- Logical IRQ is confirmed on QNX and `InterruptAttachThread()` succeeds.
- Firewall/resource ownership permits QNX MMIO, message RAM, and IRQ access.
- No access faults or permission errors appear in `slog2info`.

Record:

```text
MCAN instance:
controller base:
message RAM base:
logical IRQ:
clock source/rate:
reset state:
firewall/resource owner:
probe command output location:
```

## Internal Loopback Evidence

Polling/internal loopback:

```sh
/tmp/dev-evsn-mcan-j784 --internal-loopback-smoke-endpoint 1
```

IRQ/resource-manager internal loopback:

```sh
/tmp/dev-evsn-mcan-j784 --hardware-irq-loopback-resource-manager-endpoint 1 867 &
/tmp/dev-evsn-mcan-j784 --resource-manager-client-smoke-endpoint 1
slay dev-evsn-mcan-j784
```

QNX compatibility IRQ/internal loopback:

```sh
/tmp/dev-evsn-mcan-j784 --qnx-can-compat-hardware-irq-loopback-resource-manager-endpoint 1 867 &
/tmp/dev-evsn-mcan-j784 --qnx-can-compat-client-smoke-endpoint 1
slay dev-evsn-mcan-j784
```

Expected:

- Client smoke reports frame status `ok`.
- Diagnostics show TX and RX counts increment.
- Server exits cleanly when stopped.

This remains internal-loopback evidence only.

## External CAN-FD Evidence

This is the production promotion gate.

Required setup:

- Known-good CAN-FD analyzer or a physical loopback cable between an approved
  connector pair.
- Correct termination for the test topology.
- Transceivers enabled.
- Confirmed pinmux, clock, reset, firewall, and ownership state.
- Agreed nominal/data bitrates.

Minimum test matrix:

| Test | Requirement |
| --- | --- |
| Classic 11-bit ID, 8-byte frame | Analyzer sees exact ID/data; QNX RX receives exact frame. |
| Classic 29-bit ID, 8-byte frame | Extended-ID path works. |
| CAN-FD 11-bit ID, 64-byte payload | Private EVSN ABI carries full payload. |
| CAN-FD with BRS | Analyzer confirms bitrate switch and payload. |
| TX queue full or busy behavior | Driver returns bounded explicit error, no hang. |
| RX overflow | Diagnostic counter increments, driver remains recoverable. |
| Bus-off/error-passive | State is latched, TX rejected while unsafe, recovery path documented. |
| Shutdown cleanup | IRQ, waits, and resource-manager paths cleanly detach. |

Evidence to save:

- Driver command output.
- Analyzer capture file or screenshots.
- `slog2info` excerpts.
- Binary BuildID.
- Cabling/termination description.
- Test operator and date.

## Promotion Checklist

Do not enable `EVSN_CAN_BACKEND=j784-mcan-qnx` as a production HAL backend
until every item below is complete:

- [ ] Host tests and scans pass.
- [ ] QNX AArch64 build passes and BuildID recorded.
- [ ] Target deployment path is documented.
- [ ] Transceiver probe and apply pass on the real board.
- [ ] `MCU_MCAN0`, `MCU_MCAN1`, `MAIN_MCAN4`, and `MAIN_MCAN16` ownership is
  proven for A72/QNX, or any removed path has a recorded production de-scope
  decision.
- [ ] Pinmux/pad configuration is proven at runtime.
- [ ] Clock/reset/firewall/resource state is proven.
- [ ] Logical IRQ is confirmed for the selected production instance.
- [ ] External CAN-FD analyzer or physical loopback tests pass.
- [ ] Error and recovery behavior has target evidence.
- [ ] HAL integration policy is reviewed and recorded under `VSN-0019` and
  `VSN-0038`.

## Evidence Log Template

```text
Work item: VSN-0038
Date:
Operator:
Board:
Board revision:
QNX target uname:
Driver BuildID:
MCAN instance:
Connector:
Command(s):
Expected:
Observed:
Analyzer file:
slog2info excerpt:
Pass/fail:
Residual risk:
Next action:
```
