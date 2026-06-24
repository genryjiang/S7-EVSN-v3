# 2026-06-20 - VSN-0036 GPS/GNSS CAN Driver Port

## Actions

- Created `.agents/exec_plans/evsn-gps-can-driver-port.md`.
- Added `VSN-0036` to `.agents/feature_list.json`.
- Updated the CAN broker/gateway plan to reference the GPS/GNSS CAN driver as
  a CAN-ID-owning decoder that consumes raw gateway frames.
- Updated roadmap, IDL, drive-controller, and regression-test plans to include
  typed GNSS/navigation state.

## Legacy Evidence

- Legacy GPS decode lives in `can_broker`, not a standalone package.
- Base CAN ID is `0x700`; accepted IDs are standard frames `0x700..0x704`.
- The old driver published ROS2 scalar topics including `/gps/lat`,
  `/gps/long`, `/gps/status`, and `/car/speed/gps`.
- The EVSN plan replaces these with typed navigation/GNSS state and prohibits
  payload decode inside the generic gateway.

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
- `clock_nanosleep()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/c/clock_nanosleep.html
- `pthread_setschedparam()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setschedparam.html

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `git diff --check` passed.
- `./init.sh check` passed, validating 36 work items and links in 123 Markdown
  files.
