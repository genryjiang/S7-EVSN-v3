# 2026-06-20 - EVSN Network Port Plans

Work items: `VSN-0020` through `VSN-0032`

## Actions

- Ran harness startup and preserved existing uncommitted checkout state.
- Loaded harness, QNX, RTI, safety, real-time, testing, and evidence skills.
- Inventoried EMBD-v2-HighLevel packages, ROS2 messages, launch topology,
  active drivers/controllers, and ignored packages.
- Created execution plans:
  - `.agents/exec_plans/evsn-network-port-roadmap.md`
  - `.agents/exec_plans/evsn-network-contracts-idl.md`
  - `.agents/exec_plans/evsn-can-broker-adaptation.md`
  - `.agents/exec_plans/evsn-motor-control-critical-path.md`
  - `.agents/exec_plans/evsn-orion2-bms-driver-port.md`
  - `.agents/exec_plans/evsn-input-drivers-port.md`
  - `.agents/exec_plans/evsn-display-lighting-drivers-port.md`
  - `.agents/exec_plans/evsn-sensor-mppt-drivers-port.md`
  - `.agents/exec_plans/evsn-drive-controller-port.md`
  - `.agents/exec_plans/evsn-cruise-controller-port.md`
  - `.agents/exec_plans/evsn-noncritical-controller-port.md`
  - `.agents/exec_plans/evsn-energy-telemetry-ittia-port.md`
  - `.agents/exec_plans/evsn-cloud-telemetry-bridge-port.md`
- Updated `.agents/agent-context.md` so future agents see the CAN0
  motor-control ownership model.
- Updated `.agents/progress.md` with planning status.

## Source Inventory

Ported or planned:

- `can_msgs`, `bms_msgs`, `cmu_msgs`
- `can_broker`, `ros2_socketcan`
- `orion2_bms_driver`
- `pedal_box_driver`
- `steerwheel_driver`
- `display_driver`
- `headlight_driver`
- `indicators_driver`
- `taillight_driver`
- `brake_pressure_sensor_driver`
- `solar_current_driver`
- `irradiance_driver`
- `tyre_pressure_driver`
- `tpee_mppt_driver`
- `elmar_mppt_driver`
- `drive_controller`
- `cruise_control`
- `non_critical_controller`
- `bwsc_energy_telem` as generic energy telemetry
- `iotcore_bridge` as generic vehicle-cloud telemetry bridge
- `configuration_pkg` folded into EVSN configuration metadata

Ignored per user request:

- Lithbal driver
- Elithion/Lithiumate driver
- ECU driver
- vehicle utils and firmware update managers
- remote driving
- IBSEs
- auto test controller
- suspension driver
- horn driver
- centre display

## RTI MCP Evidence

Query attempted through tool discovery:

- `RTI Connext DDS MCP guidance RTI Connext`
- `Connext DDS DomainParticipant DataWriter QoS IDL RTI`

Result:

- No RTI Connext MCP tool was exposed in this session.

Design consequence:

- Execution plans may record scope, dependencies, source inventory, test
  expectations, and blockers.
- Final DDS/IDL/QoS/topic/generated-code/WaitSet/participant implementation is
  blocked until the RTI Connext MCP is available and its guidance is recorded in
  the active implementation plan.

## QNX Online Documentation Evidence

Consulted:

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- Scheduling policies:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_SCHEDS.html
- `pthread_setschedparam()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setschedparam.html
- `clock_nanosleep()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/c/clock_nanosleep.html
- `ThreadCtl()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/t/threadctl.html
- Processor affinity and runmasks:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/multicore_processor_affinity.html
- `tracelogger`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/t/tracelogger.html

Design consequence:

- QNX real-time plans require `SCHED_FIFO`, explicit priority, explicit
  runmask/affinity decision, absolute-time scheduling, checked QNX/POSIX calls,
  and QNX tracing/timing evidence before target readiness claims.

## ITTIA Evidence

Consulted:

- ITTIA QNX platform page:
  https://www.ittia.com/platforms/qnx
- ITTIA DB product page:
  https://www.ittia.com/ittia-db

Design consequence:

- Energy telemetry should use local ITTIA DB storage on QNX with a fake host DB
  adapter for deterministic tests.
- Implementation needs local ITTIA SDK/API documentation and license/build
  assumptions before QNX DB adapter work.

## Verification

- `python3 -m json.tool .agents/feature_list.json >/dev/null` passed.
- `./init.sh check` passed, validating 32 work items and links in 115 Markdown
  files.
- `git diff --check` passed.
