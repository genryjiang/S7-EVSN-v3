# VSN-0038 - J784S4 MCAN Libcan Driver

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: blocked

## Scope

Implement a QNX 8.0 `libcan` resource-manager driver for the TI Jacinto 7
J784S4 EVM MCAN controllers. The intended driver executable is
`devcan-mcan-j784s4`; it should expose QNX CAN endpoints such as
`/dev/can0/rx0` and `/dev/can0/tx0` so EVSN QNX services can use direct CAN
I/O without an R5F CAN gateway.

The driver is a QNX-native driver, not a Linux SocketCAN driver recompile. Use
QNX CAN DDK/libcan for the resource-manager shape, TI RTOS/PDK/CSL MCAN code
for hardware/register access, and TI Linux `m_can` plus J784S4 device-tree
content only as behavioral and resource references.

Initial target mapping:

- `/dev/can0` -> J42 / MCU_MCAN0
- `/dev/can1` -> J43 / MCU_MCAN1
- `/dev/can2` -> J45 / MAIN_MCAN4
- `/dev/can3` -> J46 / MAIN_MCAN16

Phase 1 may start with only `/dev/can0` for J42 / MCU_MCAN0. Multi-instance,
CAN-FD, filters, status, bus-off, and production hardening are later phases.

## Non-Goals

- Do not build an R5F firmware CAN gateway.
- Do not compile Linux `drivers/net/can/m_can` as the QNX driver.
- Do not copy GPL Linux driver code into EVSN without explicit legal approval.
- Do not change RTI Connext DDS types, QoS, topics, or generated-code
  integration in this work item.
- Do not implement vehicle-control application logic or control algorithms.
- Do not replace the PCAN-Router or claim production safety certification.
- Do not claim physical J784S4 CAN evidence from QNX vcan/SIL tests.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/skills/qnx-cpp-rtos/SKILL.md`
- `.agents/skills/qnx-cpp-rtos/references/qnx-neutrino-rtos.yaml`
- `.agents/skills/qnx-cpp-rtos/references/qnx-advanced.yaml`
- `harness-creator` skill for durable work-item/plan handling
- User-provided attachment:
  `/Users/henryjiang/.codex/attachments/0db579d4-1062-4048-b6d8-cae184194700/pasted-text.txt`

## External Guidance

RTI MCP:

- Not required for this plan because the work item deliberately excludes DDS
  participants, DDS types, QoS, readers, writers, WaitSets/listeners, topic
  names, generated-code integration, and application-node lifecycle changes.
- Design consequence: the QNX driver can be planned as a CAN resource-manager
  device provider. Any later RTI Connext CAN bridge validation remains governed
  by `VSN-0017` and its RTI MCP blocker.

QNX online docs:

- Product documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- QNX J784S4 BSP release notes:
  https://www.qnx.com/developers/docs/BSP8.0/com.qnx.doc.bsp.releasenotes/topic/rel_sdp80.bsp.ti.j784s4.evm.html
- QNX CAN DDK implementation:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/implementation.html
- QNX CAN library reference:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/lib_ref.html
- QNX `can-mcp2515` sample driver:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/sample.html
- QNX `can-mcp2515` libcan integration:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/sample_source.html
- QNX `can-mcp2515` quick start:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/sample_quickstart.html

QNX design consequences:

- The public QNX J784S4 BSP release notes list startup, I2C, OSPI flash, CPSW
  Ethernet, RTC, SD/MMC, serial, TRNG, and USB OTG host support, but no CAN or
  MCAN driver. Do not assume `/dev/can*` exists on the J784S4 QNX BSP.
- QNX CAN DDK expects hardware-specific `transmit()`, `devctl()`, and
  `event_handler()` callbacks supplied through `can_drvr_funcs_t`.
- The driver should call `can_resmgr_init()`, then
  `can_resmgr_init_device()` for each RX/TX device, then
  `can_resmgr_create_device()` to create `/dev/canX/rxY` and
  `/dev/canX/txZ` paths.
- Interrupt integration should use `can_resmgr_attach_intr()` and cleanly
  detach/destroy/finalize when `can_resmgr_start()` returns.
- The `can-mcp2515` sample is the QNX-side structure reference; its MCP2515
  hardware library is not a J784S4 hardware reference.

TI guidance:

- PROCESSOR-SDK-QNX-J784S4 latest version verified from TI on 2026-06-24:
  `11.02.00.04`, released Jan. 26, 2026.
  https://www.ti.com/tool/download/PROCESSOR-SDK-QNX-J784S4/11.02.00.04
- PROCESSOR-SDK-RTOS-J784S4 latest version verified from TI on 2026-06-24:
  `11.02.00.06`, released Jan. 26, 2026.
  https://www.ti.com/tool/download/PROCESSOR-SDK-RTOS-J784S4/11.02.00.06
- PROCESSOR-SDK-LINUX-J784S4 latest version verified from TI on 2026-06-24:
  `11.02.00.04`, released Jan. 25, 2026.
  https://www.ti.com/tool/download/PROCESSOR-SDK-LINUX-J784S4/11.02.00.04
- TI Linux MCAN documentation records J784S4 EVM with six header-connected
  M_CAN instances enabled by default and Linux usage through names such as
  `mcu_mcan0`.
  https://software-dl.ti.com/jacinto7/esd/processor-sdk-linux-j784s4/10_00_00_08/exports/docs/linux/Foundational_Components/Kernel/Kernel_Drivers/MCAN.html
- TI RTOS/PDK MCAN documentation records internal loopback, external loopback,
  CAN-FD examples, J42/J43 for MCU_MCAN0/MCU_MCAN1, and J45/J46 for
  MAIN_MCAN4/MAIN_MCAN16.
  https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-j784s4/10_01_00_04/exports/docs/pdk_j784s4_10_01_00_25/docs/userguide/j784s4/modules/mcan.html

TI design consequences:

- Use TI RTOS/PDK/CSL MCAN as the primary hardware access source after license
  review and source import approval.
- Use TI Linux `m_can.c`, `m_can.h`, `m_can_platform.c`, and
  `k3-j784s4*.dts*` as behavior/resource references only.
- Before implementation, confirm register bases, interrupts, clocks, message
  RAM layout, TISCI/sciclient requirements, security policy, and which MCAN
  instances A72/QNX may own without remote-core conflicts.

## Required Inputs

- QNX SDP 8.0 and J784S4 BSP.
- QNX CAN connectivity package:
  `com.qnx.qnx800.target.connectivity.can`.
- TI Processor SDK QNX J784S4:
  `ti-processor-sdk-qnx_800_j784s4_11_02_00_04.tar.gz`.
- TI Processor SDK RTOS J784S4:
  `ti-processor-sdk-rtos-j784s4-evm-11_02_00_06.tar.gz`.
- TI Processor SDK Linux J784S4:
  `ti-processor-sdk-linux-adas-j784s4-evm-11_02_00_04-Linux-x86-Install.bin`.
- QNX full-source `can-mcp2515` sample:
  `https://gitlab.com/qnx/projects/drivers/can-mcp2515.git`.
- J784S4 EVM access with known boot path, privilege policy, and debug access.
- PCAN-Router or equivalent bench analyzer for external validation.

## Architecture

### Driver Shape

Create a QNX driver executable:

```text
devcan-mcan-j784s4 -u0 -b500K -p <mcan_base> -r <irq> -v
```

The first skeleton should provide:

- QNX build integration through the existing EVSN QNX build wrappers or a
  documented QNX recursive-make import path.
- Command-line parser for unit, base, IRQ, bitrate, loopback, CAN-FD, and
  verbosity.
- `slog2`/`can_slogf()` diagnostics.
- `libcan` initialization.
- One CAN unit with one RX endpoint and one TX endpoint.
- Stubbed `transmit()`, `devctl()`, and `event_handler()` callbacks that fail
  explicitly until hardware hookup is complete.
- Clean shutdown that detaches interrupts, destroys devices, finalizes libcan,
  unmaps resources, and reports failures.

### Hardware Layer

Bring in only the TI RTOS/PDK/CSL pieces needed for MCAN operation:

- register definitions and MCAN structures
- initialization/configuration APIs
- bit-timing APIs
- message RAM configuration APIs
- TX/RX APIs
- standard and extended filter APIs
- loopback mode
- interrupt/status/error APIs
- protocol status and error-counter APIs

Expected mapping:

```text
QNX driver init          -> MCAN_init, MCAN_config, MCAN_msgRAMConfig
bitrate / CAN-FD params  -> MCAN_setBitTime
TX frame                 -> MCAN_writeMsgRam + MCAN_txBufAddReq
RX frame                 -> MCAN_getRxFIFOStatus + MCAN_readMsgRam
filters                  -> MCAN_addStdMsgIDFilter / MCAN_addExtMsgIDFilter
loopback                 -> MCAN_lpbkModeEnable
interrupt handling       -> MCAN_getIntrStatus / MCAN_clearIntrStatus
health/status            -> MCAN_getErrCounters / MCAN_getProtocolStatus
```

### EVSN Integration Boundary

This driver provides `/dev/can*` endpoints. EVSN application code should
continue to use `evsn_can_hal` through its QNX `devctl()` backend and must not
include TI PDK/CSL, QNX `libcan`, or J784S4 driver-private headers in public
EVSN application interfaces.

The `tda4-fdcan` placeholder in `VSN-0019` should remain disabled until this
driver or an approved equivalent provides documented `/dev/can*` behavior and
bench evidence. Once the driver exists, EVSN may not need a direct HAL
`tda4-fdcan` backend at all; the preferred production path may be:

```text
EVSN app -> evsn_can_hal qnx-devctl backend -> /dev/canX/rxY|txZ
         -> devcan-mcan-j784s4 -> J784S4 MCAN hardware
```

## Implementation Phases

- [ ] Phase 0: discovery and feasibility.
      Confirm installed QNX packages, TI SDK versions, license terms, BSP
      source layout, CAN package headers/libraries, register bases, IRQs,
      clocks, message RAM, TISCI/sciclient access, and QNX privilege policy.
- [ ] Phase 1: QNX `libcan` skeleton.
      Build `devcan-mcan-j784s4`, parse command line, create
      `/dev/can0/rx0` and `/dev/can0/tx0`, and return explicit unsupported
      statuses from hardware callbacks.
- [ ] Phase 2: TI PDK/CSL hardware layer.
      Adapt MCAN init, bit timing, message RAM, TX, RX, filters, loopback,
      interrupt, status, and cleanup into bounded driver-private code.
- [ ] Phase 3: internal-loopback POC.
      One MCAN instance, fixed 500 kbit/s classic CAN, accept-all filter, one
      TX buffer, one RX FIFO, no external bus required.
- [ ] Phase 4: EVM external loopback.
      Verify J42 / MCU_MCAN0 to J43 / MCU_MCAN1, then reverse direction.
- [ ] Phase 5: PCAN-Router bench integration.
      Verify classic CAN RX/TX through the fixed PCAN-Router setup and record
      bus stability, error counters, and protocol status.
- [ ] Phase 6: CAN-FD support.
      Add 64-byte payloads, arbitration/data bitrates, BRS on/off, DLC
      conversion, coexistence with classic CAN, and FD status reporting.
- [ ] Phase 7: multi-instance support.
      Add J42, J43, J45, and J46 with explicit command-line or config-file
      mapping rather than relying on probe order.
- [ ] Phase 8: production hardening.
      Add listen-only, filters, bus-off, recovery policy, TX timeout, RX FIFO
      overflow, TX queue full, restart behavior, trace instrumentation,
      buildfile/security-policy integration, and stress evidence.

## Verification

- [x] Command: `python3 -m json.tool .agents/feature_list.json >/dev/null`
      Result: passed on 2026-06-24.
- [x] Command: `./init.sh check`
      Result: passed on 2026-06-24; validated 38 work items and links in 135
      Markdown files.
- [x] Command: `git diff --check`
      Result: passed on 2026-06-24.
- [ ] Command: QNX CAN package audit for `libcan.a`, `hw/libcan.h`, and
      `sys/can_dcmd.h`
      Result: pending.
- [ ] Command: TI SDK provenance/license/source audit for QNX, RTOS, and Linux
      packages
      Result: pending.
- [ ] Command: QNX build of `devcan-mcan-j784s4`
      Result: blocked until source and package inputs are present.
- [ ] Command: target smoke showing `/dev/can0/rx0` and `/dev/can0/tx0`
      Result: blocked until driver skeleton and J784S4 target access exist.
- [ ] Command: internal-loopback classic CAN test on J42 / MCU_MCAN0
      Result: blocked until hardware layer exists.
- [ ] Command: external-loopback J42 <-> J43 test
      Result: blocked until two-instance hardware support exists.
- [ ] Command: PCAN-Router bench RX/TX test
      Result: blocked until bench topology is available.
- [ ] Command: CAN-FD RX/TX test with BRS on/off
      Result: blocked until CAN-FD support exists.
- [ ] Command: sustained RX/TX stress, bus-off, recovery, RX overflow, and TX
      queue pressure tests
      Result: blocked until production-hardening phase.
- [ ] Command: EVSN HAL smoke through `qnx-devctl` backend against
      `devcan-mcan-j784s4`
      Result: blocked until driver exposes QNX CAN endpoints.

## Acceptance Criteria

Minimum POC:

- QNX boots on J784S4 EVM with the needed CAN package and privilege policy.
- `devcan-mcan-j784s4` starts from command line or buildfile.
- Driver exposes `/dev/can0/rx0` and `/dev/can0/tx0`.
- Driver owns J42 / MCU_MCAN0 directly; no R5F CAN gateway is in the data path.
- Internal loopback passes.
- External J42/J43 loopback passes.
- Classic CAN 500 kbit/s RX/TX works through PCAN-Router or equivalent.
- Basic error/status reporting exists.
- EVSN HAL `qnx-devctl` smoke can open, write, read, and close the driver
  endpoints.

Expanded:

- CAN-FD works with configurable arbitration/data bitrate.
- BRS on/off works.
- Standard and extended IDs work.
- Standard and extended filters work.
- Listen-only mode works.
- At least two MCAN instances work.
- Driver integrates into the QNX image/buildfile.
- Bus-off and recovery behavior are documented and tested.
- Sustained RX/TX stress does not drop frames unexpectedly or wedge shutdown.
- RTI Connext CAN bridge can use the driver only after `VSN-0017` RTI guidance
  clears.

## Risks And Blockers

- The public QNX J784S4 BSP driver list does not include CAN/MCAN support.
- Required QNX CAN package, TI SDK source packages, and J784S4 target resources
  are not present in this checkout.
- A72/QNX ownership of MCU-domain MCAN, clocks, interrupts, message RAM, and
  security policy must be confirmed before direct hardware access.
- TI PDK/CSL is a hardware-control source, not a QNX driver; QNX resource
  manager lifecycle and `libcan` callbacks are EVSN implementation work.
- Linux `m_can` is GPL and Linux-specific; copying it is a legal and technical
  risk.
- Functional parity is larger than a one-frame POC: CAN-FD, filters, bus-off,
  timestamping, restart behavior, multi-instance support, stress, and
  diagnostics all require explicit evidence.
- Physical bench access and a fixed PCAN-Router wiring contract are required
  before marking the driver usable for vehicle-control paths.

## Evidence

- Created on 2026-06-24 from the user-provided Jacinto 7/J784S4 MCAN framing,
  repository CAN HAL context, QNX online documentation, QNX J784S4 BSP release
  notes, and TI SDK/MCAN documentation.
- Work item: `VSN-0038` in `.agents/feature_list.json`.
- Related EVSN HAL plan: `.agents/exec_plans/evsn-can-abstraction-layer.md`.
- Durable log:
  `.agents/logs/2026-06-24-evsn-j784s4-mcan-driver-plan.md`.

## Handoff

This is a planning and tracking artifact only. Do not implement the driver
until Phase 0 proves the QNX CAN package, TI source packages, J784S4 target
access, privilege/security policy, MCAN resource ownership, and licensing path.
The first implementation step should be a QNX `libcan` skeleton that creates
the expected `/dev/can*` endpoints with unsupported hardware callbacks, then a
single-instance internal-loopback POC on J42 / MCU_MCAN0.
