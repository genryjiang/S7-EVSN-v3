# 2026-06-21 - CAN0 Drive-Control Bus Correction

## Actions

- Read the EMBD-v2-HighLevel physical architecture diagram.
- Corrected the plans that previously described CAN0 as a motor-only bus.
- Updated `VSN-0023` so motor control computes final motor commands, while a
  separate real-time `evsn_can0_drive_bus_rt` process owns CAN0 HAL writes.
- Updated `VSN-0035` so the PedalBox remains on CAN0 as part of the
  drive-control bus, with PedalBox TX disabled unless old configuration,
  EDS/DCF, device documentation, or bench evidence proves it is required.
- Updated `VSN-0022` so the generic DDS CAN gateway does not own CAN0 TX.
- Updated `VSN-0036` so GPS on legacy CAN0 consumes a read-only CAN0 mirror.

## EMBD Evidence

- `../Sunswift/EMBD-v2-HighLevel/docs/physical_architecture.png` shows CAN0
  connected to Pedal Box Controller, Steering Wheel Controller, GPS Sensor,
  Left Motor Controller, Right Motor Controller, and force sensors.
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml` launches
  `pedal_box_driver` with `can_bus=can0`.
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/configuration_pkg/config/bus.yml`
  defines the CANopen PedalBox node and TPDO `0x185`.
- `../Sunswift/EMBD-v2-HighLevel/tech_checks/sunswift7_embdv2.kcd` contains
  CAN0 messages for motor controller info, GPS sensor lat/long, steering wheel
  regen/buttons, and BMS placeholder data.

## QNX Documentation Evidence

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- QNX scheduling policies:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_SCHEDS.html

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- Stale CAN0 motor-only wording search passed with no remaining matches in
  active plans, work items, progress, or logs.
- `./init.sh check` passed, validating 36 work items and links in 125 Markdown
  files.
