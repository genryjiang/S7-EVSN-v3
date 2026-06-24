# 2026-06-20 - VSN-0035 QNX CANopen PedalBox Port

## Actions

- Created `.agents/exec_plans/evsn-canopen-pedalbox-qnx-port.md`.
- Split CANopen PedalBox work out of the generic `VSN-0025` input-driver plan.
- Planned a QNX-native PedalBox manager using `evsn_can_hal` from `VSN-0019`.
- Recorded the key blocker: legacy PedalBox `can0` placement conflicts with
  the current `CAN0` motor-control ownership model.
- Recommended CANopenNode as the first-pass open-source stack candidate, with
  Lely CANopen as fallback after QNX/license/allocation evaluation.

## QNX Documentation Evidence

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- QNX CAN `devctl()` command index:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- `CAN_DEVCTL_SET_TIMING`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_set_timing.html
- C library `devctl()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/d/devctl.html
- C library `open()` reference:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/o/open.html

## Open-Source CANopen Evidence

- CANopenNode:
  https://github.com/CANopenNode/CANopenNode
- CANopenNode license:
  https://raw.githubusercontent.com/CANopenNode/CANopenNode/master/LICENSE
- Lely CANopen:
  https://opensource.lely.com/canopen/
- Lely CANopen overview:
  https://opensource.lely.com/canopen/docs/overview/
- Lely CANopen license:
  https://opensource.lely.com/canopen/docs/license/

## Verification

- `python3 -m json.tool .agents/feature_list.json` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 35 work items and links in 121 Markdown
  files.
