# 2026-06-21 - VSN-0035 CAN0 Supervisor Option Selected

## Actions

- Updated `.agents/exec_plans/evsn-canopen-pedalbox-qnx-port.md` to select
  implementation option 2.
- Updated `.agents/exec_plans/evsn-motor-control-critical-path.md` so the
  single CAN0 writer contract was owned by the motor critical-path design.
  This wording was superseded later on 2026-06-21 by the CAN0 drive-control bus
  correction, which moved CAN0 HAL writes to a separate
  `evsn_can0_drive_bus_rt` process.
- Updated `VSN-0035` in `.agents/feature_list.json`.
- Recorded that the PedalBox remains on CAN0 only through a single approved
  CAN0 owner/supervisor.

## Decision

Implementation option 2 is selected:

- Keep the CANopen PedalBox on CAN0.
- A single real-time CAN0 transmit owner is required.
- Superseding correction: that owner is now planned as the separate
  `evsn_can0_drive_bus_rt` drive-control bus process, not the motor-control
  process itself.
- Final motor-command frames have absolute priority.
- The PedalBox manager must not directly open or write a CAN0 TX endpoint.
- PedalBox CANopen management traffic is submitted as bounded, whitelisted,
  rate-limited requests to the CAN0 owner/supervisor.

## QNX Documentation Evidence

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- QNX scheduling policies:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_SCHEDS.html
- `pthread_setschedparam()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setschedparam.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 36 work items and links in 124 Markdown
  files.
