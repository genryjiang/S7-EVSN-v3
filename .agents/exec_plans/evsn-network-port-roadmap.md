# VSN-0020 - EMBD Network Port Roadmap

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned

## Scope

Plan the migration of active EMBD-v2-HighLevel node packages into this EVSN
QNX/RTI runtime. This roadmap is the coordination document for the per-driver,
per-controller, CAN broker, motor-control, telemetry, and IDL execution plans.

The migration must be a rewrite into this repository's structure, not a ROS2
copy. The old code is behavioral reference only unless a plan explicitly
allows local proprietary material, such as the Audi headlight support package.

## Port Inventory

Port these packages or logic:

- `can_msgs`, `bms_msgs`, and `cmu_msgs`: convert ROS2 message definitions to
  EVSN IDL and generated RTI Connext C++ types.
- `can_broker` and `ros2_socketcan`: replace the ROS2 broker/socketcan shape
  with the EVSN CAN HAL, QNX CAN resource-manager access, and RTI raw CAN
  topics. CAN0 motor commands are excluded from generic broker routing.
- `can_broker/gps_driver`: port as a separate GPS/GNSS CAN driver that consumes
  raw gateway frames for IDs `0x700..0x704` and publishes typed navigation
  state.
- `display_driver`: do not port the old USB/serial display path for the MVP.
  Defer the driver display until the end as `VSN-0037`, rewritten as an HDMI
  Qt/QML frontend fed by copied vehicle display snapshots.
- `orion2_bms_driver`: keep BMS naming, port Orion 2 BMS decode and typed
  BMS DDS outputs.
- `pedal_box_driver`, `steerwheel_driver`, `headlight_driver`,
  `indicators_driver`, and `taillight_driver`.
- Sensor drivers: `brake_pressure_sensor_driver`, `solar_current_driver`,
  `irradiance_driver`, `tyre_pressure_driver`, `tpee_mppt_driver`, and
  `elmar_mppt_driver`.
- Controllers: `drive_controller`, `cruise_control`, and
  `non_critical_controller`.
- `bwsc_energy_telem`: port as generic energy telemetry backed by ITTIA DB on
  QNX, with no BWSC/SR/Sunswift naming in new interfaces.
- `iotcore_bridge`: port only as an explicitly generic vehicle-cloud telemetry
  bridge, with branded cloud topics, certificates, and thing names removed.
- `configuration_pkg`: fold into repo-local EVSN configuration and launch
  metadata instead of preserving ROS2 launch semantics.

Ignore these packages or systems per user direction:

- `lithbal_bms_driver`
- `elithion_lithiumate` logic
- ECU driver, including the old steering ECU/EPAS actuator path; user confirmed
  the steering actuator motor has been removed
- vehicle utils and firmware update managers
- remote driving
- IBSEs
- auto test controller
- suspension driver
- horn driver
- centre display and the old USB/serial `display_driver` output path

Steering ECU retirement: the old `can_broker` created an ECU/EPAS steering
driver on CAN1, but the user confirmed the steering actuator motor has been
removed. The `0x290`, `0x292`, and `0x298` steering ECU frames are therefore
retired evidence, not an MVP equivalence blocker. The matrix below keeps them
visible so regression can prove they are not routed or exposed as active EVSN
contracts.

Display deferral: the old launch included `display_driver`, but the USB/serial
display direction is no longer the target. The driver display is shelved until
after the network/control MVP and must be rewritten as a non-control HDMI
Qt/QML frontend under `VSN-0037`. It must consume copied display snapshots at
low priority and must not provide back-pressure or control authority.

## Non-Goals

- Do not preserve ROS2 package structure, launch files, executors, QoS, or
  message definitions as runtime dependencies.
- Do not route CAN0 motor commands through DDS, a generic CAN gateway, or a
  telemetry node.
- Do not publish proprietary Audi headlight docs or code to any public cloud
  instance, public deployment artifact, or cloud sync path.
- Do not claim MISRA C++ compliance or ISO 26262 compliance. The target is
  MISRA C++-oriented implementation with evidence for future safety review.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/README.md`
- `.agents/subagents/README.md`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/exec_plans/evsn-can-gateway.md`
- `evsn-runtime/dds_types/`
- `../Sunswift/EMBD-v2-HighLevel/src/**/package.xml`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/**/*`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_logic/**/*`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_telemetry/**/*`

Skills and references loaded:

- `harness-creator`
- `rti-connext-dds`
- `qnx-cpp-rtos`
- `qnx-cpp-performance`
- `qnx-realtime-control-loop`
- `safety-critical-cpp`
- `cpp-unit-testing`
- `cpp-integration-testing`
- `middleware-testing`
- `test-coverage-evidence`
- Harness multi-agent reference
- QNX, RTI, safety, real-time, memory, concurrency, unit, integration, and
  coverage references listed by those skills

## External Guidance

- RTI MCP:
  - Query attempted through tool discovery: `RTI Connext DDS MCP guidance RTI Connext`.
  - Follow-up search: `Connext DDS DomainParticipant DataWriter QoS IDL RTI`.
  - Result: no RTI Connext MCP tool is exposed in this session.
  - Design consequence: these plans may define work boundaries, source
    inventories, safety constraints, and required evidence, but final DDS IDL,
    topic, QoS, participant, reader/writer, WaitSet/listener, and generated-code
    implementation choices remain blocked until RTI Connext MCP guidance is
    available and recorded in the active implementation plan.

- QNX online docs:
  - Product documentation index:
    https://www.qnx.com/developers/docs/index.html
  - QNX SDP 8.0 documentation:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
  - Scheduling policies:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_SCHEDS.html
  - `pthread_setschedparam()`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setschedparam.html
  - `clock_nanosleep()`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/c/clock_nanosleep.html
  - `ThreadCtl()` runmask:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/t/threadctl.html
  - Processor affinity and runmasks:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/multicore_processor_affinity.html
  - `tracelogger`:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/t/tracelogger.html
  - QNX CAN `devctl()` command index and raw-frame pages from existing
    `VSN-0019` and `VSN-0017` plans.
  - QNX SDP 8.0 Graphics documentation:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/ui.html
  - QNX Screen Graphics Subsystem Developer's Guide:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen/topic/manual/cscreen_about.html
  - QNX OpenWF Display Server User's Guide:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen.wfd-server/topic/manual/cwfd-server_about.html
  - QNX OpenWF Display Configuration Developer's Guide:
    https://www.qnx.com/developers/docs/8.0/com.qnx.doc.screen.wfdcfg/topic/manual/cwfdcfg_about.html

- ITTIA DB:
  - QNX platform page:
    https://www.ittia.com/platforms/qnx
  - ITTIA DB product page:
    https://www.ittia.com/ittia-db
  - Design consequence: energy persistence should be local embedded
    C/C++/QNX storage with bounded ingestion and explicit schema ownership, not
    InfluxDB network writes.

## Common Architecture Rules

- For unchanged hardware, treat
  `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml` plus the active
  driver constants as the compatibility baseline. If a diagram, comment, or
  old default parameter conflicts with launch/config, launch/config wins until
  bench evidence proves otherwise.
- Each migrated node becomes an EVSN runtime package with pure logic, DDS
  adapter, QNX adapter where needed, tests, and README evidence.
- Use RTI Connext for application-level network topics except the CAN0 motor
  command path.
- Keep CAN gateway boundaries raw. DBC or packet pack/unpack belongs with the
  CAN-ID-owning driver/controller.
- Use bus-first raw CAN topic defaults for non-motor buses:
  `/can<bus>/from_can_bus` and `/can<bus>/to_can_bus`.
- Replace `/car`, `/bwsc`, `/sunswift7`, `SR`, and BWSC-specific public names
  with generic EVSN names, except BMS terms.
- Use fixed-size payloads and bounded sequences in IDL. Every field needs unit,
  range, validity, timestamp/freshness, and stale-data behavior.
- Separate pure conversion/control logic from DDS, QNX, filesystem, cloud, and
  database adapters so smoke, unit, and integration tests can be deterministic.
- Avoid exceptions, RTTI, recursion, unbounded queues, unbounded blocking, and
  steady-state dynamic allocation in safety-relevant or real-time paths.

## Launch-File Traceability Checklist

This checklist maps every launch-file surface in
`../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml` to an EVSN owner or
an explicit retirement decision. It complements the frame-level matrix below.

| Legacy launch source | Old launch status | EVSN owner or decision | Regression evidence |
|---|---|---|---|
| `firmware_update_manager` at `system_launch.xml:3` | Active | Retired per user direction; no firmware manager in MVP runtime | Retired-package scan |
| `ros2_socketcan` CAN0 receiver/sender at `system_launch.xml:16` and `system_launch.xml:21` | Active | `evsn_can0_drive_bus_rt` owns CAN0 HAL, writes, and RX fanout; optional read-only DDS mirror only after RTI gate | `REG-HW-CAN0-FANOUT` plus no generic CAN0 TX |
| `ros2_socketcan` CAN1 receiver/sender at `system_launch.xml:31` and `system_launch.xml:36` | Active | Generic non-CAN0 `evsn_can_gateway` instance | `REG-HW-CAN1-GATEWAY` |
| `ros2_socketcan` CAN2 receiver/sender at `system_launch.xml:46` and `system_launch.xml:51` | Active | Generic non-CAN0 `evsn_can_gateway` instance | `REG-HW-CAN2-GATEWAY` |
| `ros2_socketcan` CAN3 receiver/sender at `system_launch.xml:61` and `system_launch.xml:66` | Active | Generic non-CAN0 `evsn_can_gateway` instance | `REG-HW-CAN3-GATEWAY` |
| `can_broker` at `system_launch.xml:73` | Active | Split into `evsn_can_gateway`, `evsn_can0_drive_bus_rt`, CAN-ID-owning drivers, and `evsn_gnss_can_node` | CAN route-table and matrix regression |
| `orion2_bms_driver` at `system_launch.xml:76` | Active | `evsn_orion2_bms_driver` | `REG-HW-CAN0-BMS` |
| `tpee_mppt_driver` at `system_launch.xml:77` | Active | `evsn_tpee_mppt_driver` | `REG-HW-CAN1-TPEE-MPPT` |
| `elmar_mppt_driver` at `system_launch.xml:78` | Active | `evsn_elmar_mppt_driver` | `REG-HW-CAN1-ELMAR-MPPT` |
| `display_driver` at `system_launch.xml:79` | Active in old launch, deferred by current requirement | Old USB/serial path retired; future driver display shelved to `VSN-0037` as HDMI Qt/QML frontend; no EVSN display process for MVP | `REG-HW-DISPLAY-DEFERRED` |
| `headlight_driver` on CAN2 at `system_launch.xml:80` | Active | Local-only `evsn_headlight_driver` | `REG-HW-CAN2-HEADLIGHT` |
| `steerwheel_driver` on CAN3 at `system_launch.xml:83` | Active | `evsn_steerwheel_driver` | `REG-HW-CAN3-REGEN`, `REG-HW-CAN3-BUTTONS` |
| `pedal_box_driver` on CAN0 at `system_launch.xml:86` | Active | `evsn_canopen_pedalbox` consuming CAN0 owner fanout | `REG-HW-CAN0-PEDAL`, optional `REG-HW-CAN0-PEDAL-MGMT` |
| `indicators_driver` on CAN1 at `system_launch.xml:89` | Active | `evsn_indicator_driver` | `REG-HW-CAN1-INDICATORS` |
| `taillight_driver` on CAN1 at `system_launch.xml:92` | Active | `evsn_taillight_driver` | `REG-HW-CAN1-TAILLIGHT` |
| `horn_driver` at `system_launch.xml:95` | Commented out and user-retired | Retired; steerwheel horn input may be decoded only as diagnostics if needed | Retired-package and no-horn-output scan |
| `suspension_driver` at `system_launch.xml:99` | Commented out and user-retired | Retired | Retired-package scan |
| `irradiance_driver` on CAN1 at `system_launch.xml:102` | Active | `evsn_irradiance_driver` | `REG-HW-CAN1-IRRADIANCE` |
| `brake_pressure_sensor_driver` on CAN0 at `system_launch.xml:105` | Active | `evsn_brake_pressure_driver` consuming CAN0 owner fanout | `REG-HW-CAN0-BRAKE-PRESSURE` |
| `solar_current_driver` on CAN0 at `system_launch.xml:108` | Active | `evsn_solar_current_driver` consuming CAN0 owner fanout | `REG-HW-CAN0-SOLAR` |
| `tyre_pressure_driver` at `system_launch.xml:112` | Commented out | Disabled by default; optional `evsn_tyre_pressure_driver` only if current config enables it | `REG-HW-CAN0-TYRE` disabled-by-default check |
| `drive_controller` at `system_launch.xml:115` | Active | `evsn_drive_controller` plus `evsn_motor_control_rt` for final motor command computation | `REG-HW-CTRL-DRIVE` |
| `cruise_control` at `system_launch.xml:116` | Active | `evsn_cruise_controller` | `REG-HW-CTRL-CRUISE` |
| `non_critical_controller` at `system_launch.xml:117` | Active | `evsn_body_control` | `REG-HW-CTRL-BODY` |
| `iotcore_bridge` at `system_launch.xml:120` | Active | `evsn_vehicle_cloud_bridge` with generic branding and no control authority | `REG-HW-TELEM-CLOUD` |
| `bwsc_energy_telem` at `system_launch.xml:121` | Active | `evsn_energy_telemetry_ittia` backed by ITTIA DB | `REG-HW-TELEM-ITTIA` |
| `powertrain_data` at `system_launch.xml:123` | Commented out | Not in MVP unless separately approved | Retired/commented-package scan |
| `remote_webserver`, `remote_ctrl_pkg`, and `ibse_controller` at `system_launch.xml:126` through `system_launch.xml:128` | Commented out and user-retired | Retired | Retired remote/IBSE scan |

## Hardware-Equivalence Matrix

This matrix is the MVP compatibility contract for unchanged hardware. Old ROS2
topic names are reference inputs/outputs only; new DDS topic names are candidate
generic contracts and remain blocked on `VSN-0021` plus RTI Connext MCP
guidance before implementation.

### CAN0 Drive-Control Bus

`CAN0` is a shared drive-control bus. `evsn_can0_drive_bus_rt` owns the CAN0
HAL endpoint, performs all CAN0 writes, and provides bounded receive fanout to
CAN0 subscribers. It is not the generic DDS CAN gateway.

| Frame or surface | Direction | Old ROS2 owner and topics | New QNX owner/process | DDS topic if any | Required regression |
|---|---:|---|---|---|---|
| Raw CAN0 RX fanout | RX | `ros2_socketcan` published `/can0/from_can_bus` | `evsn_can0_drive_bus_rt` read path and bounded subscriber fanout | Optional read-only `/can0/from_can_bus` mirror after RTI gate | `REG-HW-CAN0-FANOUT`: every listed CAN0 ID reaches exactly the owning process; no subscriber can transmit through the fanout |
| WS22 motor drive `0x501` | TX | `wavesculptor` consumed `/control/motors/current` and `/control/motors/velocity`; command is control base `0x500` plus `WS22_COMMAND_MOTOR_DRIVE` | `evsn_motor_control_rt` computes command; `evsn_can0_drive_bus_rt` transmits final frame | None for final command | `REG-HW-CAN0-MOTOR-DRIVE`: golden old/new frame bytes for velocity/current, current clamp, stale safe state, and no DDS/generic gateway TX |
| WS22 power/reset `0x502`, `0x503` | TX | `wavesculptor` supported power command and `/car/motors/reset` | Motor-control service path via `evsn_can0_drive_bus_rt`, only if retained | No final command DDS topic | `REG-HW-CAN0-MOTOR-SERVICE`: reset/power frames are whitelisted, bounded, and lower priority than drive frame |
| Left WS22 telemetry `0x400..0x417` | RX | `can_broker` motor driver published `/car/velocity/left`, `/car/velocity_kph/left`, `/car/motors/left/*` | `evsn_can0_drive_bus_rt` RX fanout to motor telemetry decoder/status adapter | Candidate `/vehicle/motor/left/status` | `REG-HW-CAN0-MOTOR-RX-L`: every legacy telemetry ID decodes or is explicitly marked reserved/ignored with no command side effect |
| Right WS22 telemetry `0x420..0x437` | RX | `can_broker` motor driver published `/car/velocity/right`, `/car/velocity_kph/right`, `/car/motors/right/*` | `evsn_can0_drive_bus_rt` RX fanout to motor telemetry decoder/status adapter | Candidate `/vehicle/motor/right/status` | `REG-HW-CAN0-MOTOR-RX-R`: same coverage as left motor, including drive-controller speed input equivalence |
| PedalBox TPDO `0x185` | RX | `pedal_box_driver` subscribed configured `can0/from_can_bus`, published `car/accel_pos` and `car/pedalbox/disconnected` | `evsn_canopen_pedalbox` consumes CAN0 owner mirror | Candidate `/vehicle/input/pedal/state` | `REG-HW-CAN0-PEDAL`: raw `0x185` vectors reproduce old accel normalization and disconnect behavior |
| PedalBox CANopen management COB-IDs | Optional TX/RX | Legacy CANopen config used node ID `5`, master ID `1`, heartbeat `1000 ms`; simple ROS2 driver did not write management frames | `evsn_canopen_pedalbox` may request bounded whitelisted TX from `evsn_can0_drive_bus_rt` only if device evidence requires it | Candidate diagnostics only | `REG-HW-CAN0-PEDAL-MGMT`: management TX absent by default; if enabled, rate limit and motor-priority rejection are verified |
| Orion 2 BMS `0x620..0x629` | RX | `orion2_bms_driver` subscribed `/can0/from_can_bus`, published `/car/bms/pack/*`, `/car/bms/cell/*`, `/car/bms/state/*`, `/car/bms/error/*` | `evsn_orion2_bms_driver` consumes CAN0 owner mirror | Candidate `/vehicle/bms/...` | `REG-HW-CAN0-BMS`: all ten IDs decode, malformed DLC/extended IDs are rejected, stale/missing frames surface diagnostics |
| Brake pressure `0x1B3` | RX | `brake_pressure_sensor_driver` launched on CAN0, published `/car/brake/pressure` | `evsn_brake_pressure_driver` consumes CAN0 owner mirror | Candidate `/vehicle/sensors/brake_pressure/state` | `REG-HW-CAN0-BRAKE-PRESSURE`: rolling average, wrong-ID, stale, and threshold-consumer vectors |
| Solar current `0x1B4` | RX | `solar_current_driver` subscribed `/can0/from_can_bus`, published `/car/solar_current`; old `/can0/to_can_bus` publisher was a TODO calibration hook | `evsn_solar_current_driver` consumes CAN0 owner mirror; no CAN0 TX unless separately proven | Candidate `/vehicle/energy/solar/current` | `REG-HW-CAN0-SOLAR`: mA-to-A scaling and no accidental CAN0 TX |
| GPS/GNSS `0x700..0x704` | RX | `can_broker/gps_driver` published `/gps/*` and `/car/speed/gps` | `evsn_gnss_can_node` consumes CAN0 owner mirror | Candidate `/vehicle/navigation/gnss/state` | `REG-HW-CAN0-GNSS`: five-frame coherent state, UTC conversion, stale packet mask, speed compatibility |
| Tyre pressure `0x4D0..0x4D3` | RX, if enabled | Code subscribed `/can0/from_can_bus`, published `/sensors/tyre/{lf,rf,lr,rr}/*`; package was commented out in `system_launch.xml` | `evsn_tyre_pressure_driver` only if current hardware config enables it | Candidate `/vehicle/sensors/tyre/state` | `REG-HW-CAN0-TYRE`: disabled-by-default MVP check plus decode vectors if enabled |

### CAN1 Body, Sensors, And MPPT Bus

The generic non-CAN0 gateway may own CAN1 raw RX/TX after RTI guidance.

| Frame or surface | Direction | Old ROS2 owner and topics | New QNX owner/process | DDS topic if any | Required regression |
|---|---:|---|---|---|---|
| Raw CAN1 RX/TX | RX/TX | `ros2_socketcan` `/can1/from_can_bus`, `/can1/to_can_bus` | Generic `evsn_can_gateway` instance for CAN1 | `/can1/from_can_bus`, `/can1/to_can_bus` after RTI gate | `REG-HW-CAN1-GATEWAY`: raw route, malformed frame, queue pressure, and no CAN0 command leakage |
| Side indicators `0x18D`, `0x18E` | TX | `indicators_driver` consumed `/control/indicator/left` and `/control/indicator/right` | `evsn_indicator_driver` via CAN1 gateway | Candidate `/vehicle/body/lighting/command`; raw `/can1/to_can_bus` | `REG-HW-CAN1-INDICATORS`: left/right frame bytes and invalid-state handling |
| Taillight `0x18B` | TX | `taillight_driver` consumed `/control/taillights/tail`, `/control/taillights/brake`, `/control/taillights/reverse`, and indicator topics | `evsn_taillight_driver` via CAN1 gateway | Candidate `/vehicle/body/lighting/command`; raw `/can1/to_can_bus` | `REG-HW-CAN1-TAILLIGHT`: all bit combinations, repeated publish policy, and reverse-light no-op compatibility |
| Irradiance `0x1B7`, `0x1B8`, `0x1B9`, `0x1C0` | RX | `irradiance_driver` launched on CAN1, published `/sensors/irradiance/{mr,br,fr,ml}/{raw,gsi}` | `evsn_irradiance_driver` | Candidate `/vehicle/sensors/irradiance/state` | `REG-HW-CAN1-IRRADIANCE`: per-position decode, calibration, stale and malformed frames |
| TPEE MPPT `0x210`, `0x211`, `0x220`, `0x221` | RX | `tpee_mppt_driver` published `/car/mppt1/*` and `/car/mppt2/*` | `evsn_tpee_mppt_driver` | Candidate `/vehicle/energy/mppt/state` | `REG-HW-CAN1-TPEE-MPPT`: MPPT1/2 voltage/current, mode, fault, enable, temperature vectors |
| Elmar MPPT `0x600`, `0x601`, `0x602`, `0x605` | RX | `elmar_mppt_driver` published `/car/mppt3/*` | `evsn_elmar_mppt_driver` | Candidate `/vehicle/energy/mppt/state` | `REG-HW-CAN1-ELMAR-MPPT`: MPPT3 status, error bits, mode bits, and ignored gap IDs |
| Retired steering EPAS/ECU `0x290`, `0x292`, command `0x298` | Retired RX/TX reference | `can_broker` created `Ecu(0x290)` on CAN1; consumed `/control/steering/angle`; published steering telemetry under `/car/steering/*` | No EVSN process; steering ECU is explicitly retired because the actuator motor has been removed | None | `REG-HW-CAN1-STEERING-RETIRED`: prove no route-table entry, no command topic, no `/control/steering/angle` compatibility path, and no accidental `0x298` TX |

### CAN2 Headlight Bus

The old system launch overrides `headlight_driver` to `can2`; the driver
default of `can1` must not be used for the unchanged-hardware port.

| Frame or surface | Direction | Old ROS2 owner and topics | New QNX owner/process | DDS topic if any | Required regression |
|---|---:|---|---|---|---|
| Raw CAN2 TX | TX | `ros2_socketcan` `/can2/to_can_bus` | Generic `evsn_can_gateway` instance for CAN2 | `/can2/to_can_bus` after RTI gate | `REG-HW-CAN2-GATEWAY`: CAN2 routing with no CAN0/CAN1 cross-bus leakage |
| Audi headlight command frames `0x40`, `0xFD`, `0x15F`, `0x190`, `0x192`, `0x193`, `0x195`, `0x3C0`, `0x585`, `0x6B2`, `0x160..0x169`, `0x16A..0x16F`, `0x174`, `0x175`, extended `0x1A55551F`, `0x1A555520`, `0x16A954A9` | TX | `headlight_driver` consumed `/control/indicator/*`, `/control/headlight/beam/low`, `/control/headlight/drl`, `/control/headlight/init` and published to configured `can2/to_can_bus` | Local-only `evsn_headlight_driver` package; proprietary files excluded from public/cloud artifacts | Candidate `/vehicle/body/lighting/command`; raw `/can2/to_can_bus` | `REG-HW-CAN2-HEADLIGHT`: init, low beam, DRL, indicator sequences, CRC/counter vectors, extended-ID handling, and proprietary-artifact scan |

### CAN3 Steerwheel Bus

The old launch explicitly places `steerwheel_driver` on `can3`; this supersedes
earlier notes that inferred steerwheel from a physical diagram.

| Frame or surface | Direction | Old ROS2 owner and topics | New QNX owner/process | DDS topic if any | Required regression |
|---|---:|---|---|---|---|
| Raw CAN3 RX | RX | `ros2_socketcan` `/can3/from_can_bus` | Generic `evsn_can_gateway` instance for CAN3 | `/can3/from_can_bus` after RTI gate | `REG-HW-CAN3-GATEWAY`: steerwheel RX route and stale/disconnect timing |
| Steerwheel regen `0x183` | RX | `steerwheel_driver` published `/car/regen_pos` and `/car/regen_pos/raw` | `evsn_steerwheel_driver` | Candidate `/vehicle/input/steerwheel/state` | `REG-HW-CAN3-REGEN`: calibration `1370..1800`, raw/normalized value, stale/disconnect behavior |
| Steerwheel buttons/mode `0x283` | RX | `steerwheel_driver` published `/car/steerwheel/indicator/*`, `/car/steerwheel/headlight`, `/car/steerwheel/cruise/*`, `/car/steerwheel/gear`, `/car/steerwheel/drive_mode`, plus horn/interior-light topics | `evsn_steerwheel_driver`; horn output remains ignored unless re-approved | Candidate `/vehicle/input/steerwheel/state` | `REG-HW-CAN3-BUTTONS`: gear/mode/button bitfields, cruise events, horn absence, stale/disconnect behavior |

### Non-CAN Control And Telemetry Surfaces

| Surface | Old ROS2 topics | New owner/process | DDS topic if any | Required regression |
|---|---|---|---|---|
| Drive control request | Inputs `/car/accel_pos`, `/car/regen_pos`, `/car/speed/gps`, `/car/velocity_kph/right`, `/car/steerwheel/{drive_mode,gear}`, `/cruise/*`, `/car/bms/pack/ccl`; outputs `/control/motors/{velocity,current}`, retired `/control/steering/angle`, `/control/brake`, limits | `evsn_drive_controller` computes request snapshots; `evsn_motor_control_rt` computes final motor frame; steering ECU output is not ported | Candidate `/vehicle/drive/request`, `/vehicle/drive/status`; no final motor command DDS topic | `REG-HW-CTRL-DRIVE`: old input vectors reproduce request/current/velocity behavior, retired steering output absent, remote paths absent, final CAN0 TX only via CAN0 owner |
| Cruise control | Inputs `/car/steerwheel/cruise/*`, `/control/taillights/brake`, `/car/accel_pos`, motor speeds, gear; outputs `/cruise/*` | `evsn_cruise_controller` advisory request source | Candidate `/vehicle/drive/cruise/state` | `REG-HW-CTRL-CRUISE`: target adjust, disable, gear, brake, stale speed |
| Body control | Inputs steerwheel, hazards, brake pressure, regen, gear; outputs `/control/headlight/*`, `/control/taillights/*`, `/control/indicator/*`; horn output ignored | `evsn_body_control` | Candidate `/vehicle/body/lighting/command` | `REG-HW-CTRL-BODY`: lighting/indicator/taillight command equivalence, horn absent, firmware paths absent |
| Deferred driver display / old USB-serial `display_driver` output path | Old display package consumed vehicle scalar topics and wrote to a USB/serial display surface | No EVSN display process for MVP. Future `VSN-0037` rewrites display as HDMI Qt/QML frontend consuming copied display snapshots only | None for MVP; future candidate `/vehicle/display/snapshot` after RTI gate | `REG-HW-DISPLAY-DEFERRED`: prove old USB/serial display path and centre-display contracts are absent from MVP runtime; future Qt frontend has separate acceptance before activation |
| Energy telemetry | `bwsc_energy_telem` used InfluxDB path | `evsn_energy_telemetry_ittia` backed by ITTIA DB on QNX | Subscribes allowlisted vehicle telemetry topics only | `REG-HW-TELEM-ITTIA`: local DB schema/write/readback/replay and no real-time back-pressure |
| Cloud telemetry | `iotcore_bridge` cloud path | `evsn_vehicle_cloud_bridge` | Subscribes allowlisted telemetry/status topics only | `REG-HW-TELEM-CLOUD`: branding removal, allowlist, reconnect/offline buffering, no control authority |

## Subagent Workflow

Every implementation work item should use these bounded roles:

- `researcher`: inspect only the assigned old package, this repo's target
  package, topic contracts, CAN IDs, and docs. Return facts with file refs.
- `implementer`: edit only the new EVSN package and assigned IDL/test files.
  Must record RTI MCP and QNX online-doc evidence when applicable.
- `reviewer`: check QNX/RTI/CAN behavior, timing, allocation, failure paths,
  branding removal, and smoke/unit/integration evidence.
- `target-tester`: run QNX QEMU or hardware-adjacent checks only when a target
  contract exists.

## Strict Verification Gate

Every implementation plan in this roadmap must link its package evidence to
`VSN-0033`. Do not mark a migrated package complete on smoke or unit tests
alone.

Minimum evidence for each driver, controller, adapter, or contract package:

- bounded smoke test with fake inputs and deterministic readiness/exit checks
- pure unit tests for valid, invalid, boundary, stale, and fault inputs
- integration tests with real RTI entities or fake/QNX/SIL adapters where the
  package crosses middleware, CAN, DB, cloud, process, or target boundaries
- negative regression tests for banned branding, retired packages, malformed
  frames, prohibited CAN0 motor command routing, stale data, and unsafe
  degraded behavior where applicable
- exact commands, platform/target configuration, middleware domain/transport,
  logs/traces, exit status, residual risks, and known gaps recorded before
  status changes

## Steps

- [ ] Complete `VSN-0021` IDL and topic registry before porting node adapters.
- [ ] Complete or update `VSN-0019` CAN HAL before any hardware-facing gateway
      or direct CAN0 motor process implementation.
- [ ] Complete `VSN-0022` CAN broker and gateway adaptation before non-motor
      CAN drivers depend on raw CAN DDS topics.
- [ ] Complete `VSN-0023` motor-control critical path before routing any motor
      command data.
- [ ] Port drivers and controllers in dependency order:
      BMS and pedal first, steerwheel and sensors next, drive/cruise control
      after input/status and typed GNSS speed topics exist, body/lighting after
      non-critical control contracts are defined.
- [ ] Port energy telemetry and cloud bridge after topic registry and branded
      name removal are complete.
- [ ] Update `.agents/feature_list.json`, logs, and progress after every plan
      moves from planned to implementation.

## Verification

- [ ] Command: `python3 -m json.tool .agents/feature_list.json >/dev/null`
      Result:
- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:

Per-package implementation plans must add:

- Smoke test: package starts with fake inputs, validates config, and exits
  boundedly.
- Unit tests: pure decode, encode, control, limits, stale-data, and invalid
  input behavior.
- Integration tests: real RTI Connext entities on isolated domains and fake
  or QNX/SIL CAN/DB adapters where relevant.
- QNX target evidence: required before any claim of QNX runtime behavior.

## Evidence

- `.agents/logs/2026-06-20-evsn-network-port-plans.md`

## Handoff

This is a planning artifact. Implementation must not start on any RTI/DDS node
or IDL contract until RTI Connext MCP guidance is available and recorded in the
specific work item.
