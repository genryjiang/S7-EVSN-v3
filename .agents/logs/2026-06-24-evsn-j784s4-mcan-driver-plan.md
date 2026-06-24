# 2026-06-24 - evsn-j784s4-mcan-driver-plan

Status: planned and blocked before implementation

Added `VSN-0038` for a QNX-native `libcan` MCAN driver on the TI Jacinto 7
J784S4 EVM.

Planning decision:

- Frame the work as `devcan-mcan-j784s4`, a QNX CAN resource-manager driver
  that exposes `/dev/can*` endpoints.
- Do not scope this as recompiling Linux SocketCAN `m_can` code on QNX.
- Use QNX CAN DDK/libcan for driver/resource-manager structure.
- Use TI RTOS/PDK/CSL MCAN as the hardware/register layer after license and
  import approval.
- Use TI Linux `m_can` and J784S4 device-tree files only as behavior and
  resource references unless legal explicitly approves a different use.
- Keep RTI Connext application integration out of scope until `VSN-0017` RTI
  MCP guidance clears.
- Prefer the eventual EVSN path:
  `evsn_can_hal qnx-devctl backend -> /dev/canX/rxY|txZ ->
  devcan-mcan-j784s4 -> J784S4 MCAN hardware`.

External evidence used:

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- QNX J784S4 BSP release notes:
  https://www.qnx.com/developers/docs/BSP8.0/com.qnx.doc.bsp.releasenotes/topic/rel_sdp80.bsp.ti.j784s4.evm.html
- QNX CAN DDK implementation:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/implementation.html
- QNX CAN library reference:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/lib_ref.html
- QNX `can-mcp2515` sample and `libcan` integration:
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/sample.html
  https://www.qnx.com/developers/docs/qnxeverywhere/com.qnx.doc.ddk/topic/can/sample_source.html
- TI Processor SDK QNX J784S4 11.02.00.04, released Jan. 26, 2026:
  https://www.ti.com/tool/download/PROCESSOR-SDK-QNX-J784S4/11.02.00.04
- TI Processor SDK RTOS J784S4 11.02.00.06, released Jan. 26, 2026:
  https://www.ti.com/tool/download/PROCESSOR-SDK-RTOS-J784S4/11.02.00.06
- TI Processor SDK Linux J784S4 11.02.00.04, released Jan. 25, 2026:
  https://www.ti.com/tool/download/PROCESSOR-SDK-LINUX-J784S4/11.02.00.04
- TI Linux MCAN guide showing J784S4 EVM with six header-connected M_CAN
  instances enabled by default:
  https://software-dl.ti.com/jacinto7/esd/processor-sdk-linux-j784s4/10_00_00_08/exports/docs/linux/Foundational_Components/Kernel/Kernel_Drivers/MCAN.html
- TI PDK MCAN guide showing J42/J43 and J45/J46 external-loopback mappings:
  https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-j784s4/10_01_00_04/exports/docs/pdk_j784s4_10_01_00_25/docs/userguide/j784s4/modules/mcan.html

Files touched:

- `.agents/feature_list.json`
- `.agents/exec_plans/evsn-j784s4-mcan-libcan-driver.md`
- `.agents/logs/2026-06-24-evsn-j784s4-mcan-driver-plan.md`
- `.agents/progress.md`
- `.agents/session-handoff.md`

Blockers:

- QNX CAN connectivity package, TI SDK source packages, and J784S4 target
  resources are not present in this checkout.
- QNX/A72 ownership of MCU-domain MCAN, clocks, interrupts, message RAM, and
  security policy must be confirmed.
- TI PDK/CSL import strategy and license terms must be approved before
  implementation.
- Physical bench and PCAN-Router wiring evidence are required before claiming
  vehicle-control usability.

Verification:

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed; harness validated 38 work items and links in 135
  Markdown files.
- `git diff --check` passed.
