# VSN-0036 - QNX GPS/GNSS CAN Driver Port

Branch: feature/evsn-can-abstraction-layer
Owner: main-agent
Status: planned; implementation blocked on RTI MCP for final DDS contracts

## Scope

Port the legacy `can_broker/gps_driver` logic into the EVSN QNX/CAN runtime in
the format that fits the new broker architecture.

The old code lived inside ROS2 `can_broker`, but the new EVSN broker/gateway is
raw-frame only. Therefore the GPS decoder becomes a CAN-ID-owning GNSS driver
package that consumes raw frames from the EVSN CAN gateway and publishes typed
navigation state.

Legacy source:

- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_broker/include/can_broker/gps_driver.hpp`
- `../Sunswift/EMBD-v2-HighLevel/src/vehicle_interface/can_broker/src/gps_driver.cpp`

Legacy CAN surface:

- Standard classic CAN frames only.
- Base ID: `0x700`.
- Frame IDs: `0x700`, `0x701`, `0x702`, `0x703`, `0x704`.
- All production decode paths must validate DLC before reading 8 data bytes.

## Non-Goals

- Do not keep GPS payload decode inside the generic EVSN CAN gateway.
- Do not port the retired `centre_display` package.
- Do not preserve ROS2 scalar topics such as `/gps/lat`, `/gps/long`,
  `/gps/status`, or `/car/speed/gps` as public runtime contracts.
- Do not give GPS/GNSS code any CAN0 TX path.
- Do not use cloud telemetry as an input to drive, cruise, or motor control.
- Do not implement NMEA, serial GNSS, RTK, or vendor SDK support in this work
  item. This plan covers the legacy CAN GPS frame set only.

## Context Loaded

- `AGENTS.md`
- `.agents/feature_list.json`
- `.agents/progress.md`
- `.agents/session-handoff.md`
- `.agents/agent-context.md`
- `.agents/exec_plans/evsn-network-port-roadmap.md`
- `.agents/exec_plans/evsn-can-broker-adaptation.md`
- `.agents/exec_plans/evsn-can-abstraction-layer.md`
- `.agents/exec_plans/evsn-network-contracts-idl.md`
- `.agents/exec_plans/evsn-drive-controller-port.md`
- `.agents/exec_plans/evsn-network-regression-test-suite.md`
- `../Sunswift/EMBD-v2-HighLevel/launch/system_launch.xml`
- `.agents/exec_plans/evsn-network-port-roadmap.md` hardware-equivalence
  matrix
- `evsn-runtime/dds_types/`
- Legacy `can_broker/gps_driver.hpp`
- Legacy `can_broker/gps_driver.cpp`

Skills and references applied:

- `harness-creator`
- `qnx-cpp-rtos`
- `rti-connext-dds`
- QNX Neutrino RTOS and advanced QNX references
- DDS middleware reference

## External Guidance

### QNX Documentation

QNX online documentation was consulted for this QNX-facing plan:

- QNX Product Documentation index:
  https://www.qnx.com/developers/docs/index.html
- QNX SDP 8.0 documentation:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.qnxsdp.nav/topic/bookset.html
- CAN `devctl()` command index:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can.html
- `CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_rx_frame_raw_noblock.html
- `CAN_DEVCTL_TX_FRAME_RAW`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.devctl/topic/can/can_devctl_tx_frame_raw.html
- `clock_nanosleep()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/c/clock_nanosleep.html
- `pthread_setschedparam()`:
  https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setschedparam.html

QNX design consequences:

- GPS CAN I/O uses `VSN-0022` raw CAN gateway and `VSN-0019` CAN HAL
  backends; GPS decode code must not directly include QNX CAN headers.
- QNX raw CAN `devctl()` behavior stays inside the HAL. The GPS package sees
  only EVSN raw CAN frames or HAL test frames.
- GPS/GNSS runs below motor-control and CAN0 telemetry priorities. It is useful
  to drive/cruise/telemetry and may later feed the deferred `VSN-0037` display
  snapshot, but it is not the final motor-command owner.
- Bounded polling, bounded queues, explicit stale data, and checked shutdown
  behavior are required for QNX/SIL and target evidence.

### RTI Connext

RTI MCP guidance is not available in this session. The plan may define package
boundaries, legacy mapping, pure decode behavior, test vectors, and required
evidence, but final DDS topic/type/QoS/generated-code implementation remains
blocked until RTI Connext MCP guidance is recorded.

Pure CAN decode tests and fake frame integration may proceed without RTI MCP
because they do not create or change DDS participants, readers, writers, QoS,
IDL, listeners, WaitSets, or generated-code integration.

## Legacy Mapping To Preserve And Verify

All fields below come from the legacy `gps_driver.cpp` implementation. Each
mapping must be verified against the physical GPS/GNSS device manual or bench
captures before target deployment.

Frame `0x700`, packet 1:

- Bytes `0..3`: signed big-endian latitude raw, scale `1e-7 deg`.
- Bytes `4..7`: signed big-endian longitude raw, scale `1e-7 deg`.

Frame `0x701`, packet 2:

- Bytes `0..1`: signed big-endian speed raw, legacy scale `raw * 36 / 1000`
  to kph.
- Bytes `2..3`: signed big-endian height raw, legacy unit appears to be metres.
- Byte `4`: noise raw.
- Byte `5`: satellite count.
- Byte `7` bits `0..2`: GNSS status:
  - `1`: no fix
  - `2`: IMU-only position
  - `3`: GPS 2D
  - `4`: GPS 3D
  - `5`: GPS plus IMU
- Byte `7` bits `3..5`: IMU status:
  - `0`: initialization
  - `1`: fusion
  - `2`: suspended
  - `3`: disabled error
  - `4`: disabled user

Frame `0x702`, packet 3:

- Bytes `0..1`: signed big-endian motion heading raw.
- Bytes `2..3`: signed big-endian vehicle heading raw.
- Bytes `4..5`: signed big-endian angular velocity X raw, legacy scale
  `raw / 100`.
- Bytes `6..7`: signed big-endian angular velocity Y raw, legacy scale
  `raw / 100`.

Frame `0x703`, packet 4:

- Bytes `0..1`: signed big-endian angular velocity Z raw, legacy scale
  `raw / 100`.
- Bytes `2..3`: signed big-endian acceleration X raw, legacy scale
  `raw / 100`.
- Bytes `4..5`: signed big-endian acceleration Y raw, legacy scale
  `raw / 100`.
- Bytes `6..7`: signed big-endian acceleration Z raw, legacy scale
  `raw / 100`.

Frame `0x704`, packet 5:

- Byte `0`: year offset from 2000.
- Byte `1`: month.
- Byte `2`: day.
- Byte `3`: hour.
- Byte `4`: minute.
- Byte `5`: second.
- Bytes `6..7`: unsigned big-endian subsecond field. The old code treated this
  as milliseconds and converted by multiplying by `1,000,000 ns`; the comment
  is ambiguous and must be verified.

UTC conversion must be rewritten. The legacy code used `mktime()`, which
interprets local time and is not appropriate for a deterministic UTC GNSS
timestamp. The EVSN implementation should use an explicit bounded UTC calendar
conversion with validation.

## Proposed Architecture

### Package

Create a dedicated package:

- `evsn-runtime/gnss_can_driver/CMakeLists.txt`
- `evsn-runtime/gnss_can_driver/README.md`
- `evsn-runtime/gnss_can_driver/include/evsn/gnss_can_driver/*.hpp`
- `evsn-runtime/gnss_can_driver/src/*.cpp`
- `evsn-runtime/gnss_can_driver/test/*.cpp`
- `evsn-runtime/gnss_can_driver/test/data/*.yml`

### Runtime Boundary

Preferred process: `evsn_gnss_can_node`

Data path:

- Preserve the old CAN broker setup for unchanged hardware: GPS/GNSS base ID
  `0x700` is on CAN0 through the old `can_broker` default `/can0/from_can_bus`
  path.
- Subscribe to the CAN0 read-only mirror/fanout from `evsn_can0_drive_bus_rt`.
  Do not consume CAN0 through a generic CAN gateway TX/RX path unless a later
  approved hardware configuration moves the GPS bus.
- Filter standard IDs `base_id..base_id+4`; default base ID `0x700`.
- Decode packets in pure logic.
- Maintain a latest coherent GNSS/navigation state with per-packet freshness.
- Publish typed EVSN navigation/GNSS state after RTI MCP guidance clears.

The GPS package may be collocated in the same deployment group as a gateway for
latency or process-count reasons, but the code boundary remains separate:
gateway owns raw bus access; `evsn_gnss_can_driver` owns IDs `0x700..0x704`.

### CAN Broker/Gateway Integration

Update `VSN-0022` so the CAN broker/gateway config can describe the GPS CAN
source without decoding it:

- bus name or index for the GPS/GNSS source,
- default base ID `0x700`,
- standard-ID-only filter,
- optional mailbox or software filter for `0x700..0x704`,
- raw RX topic consumed by `evsn_gnss_can_node`,
- no GPS TX route unless a future verified device protocol requires it.

The generic gateway must still reject final CAN0 motor-command TX and must not
special-case GPS payload fields.

### EVSN Type Contract

Add navigation/GNSS IDL under `VSN-0021` after RTI guidance. Candidate type:
`EVSNGnssState`.

Required fields:

- validity flags:
  - fix valid
  - motion valid
  - inertial valid
  - UTC valid
  - input fresh
- timestamps:
  - monotonic receive time
  - input age
  - UTC epoch seconds and nanoseconds, when valid
- source:
  - bus ID
  - base CAN ID
  - last source CAN ID
  - received packet bitmask
  - sample counter
- position:
  - latitude deg
  - longitude deg
  - altitude m
- motion:
  - speed kph
  - motion heading raw or deg after scale confirmation
  - vehicle heading raw or deg after scale confirmation
- quality:
  - satellite count
  - noise
  - GNSS status enum
  - IMU status enum
- inertial:
  - angular velocity X/Y/Z with confirmed units
  - acceleration X/Y/Z with confirmed units
- diagnostics:
  - stale packet bitmask
  - malformed frame count
  - range error count
  - status/fix mismatch bits

Do not publish a forest of one-field scalar topics. If drive control needs only
speed, it should consume the typed GNSS/navigation state or a reviewed derived
navigation-speed contract created under `VSN-0021`.

## Subagents and Skills

Use bounded subagents:

- `researcher`: inspect the legacy GPS driver, old drive/cloud consumers, any
  deferred display snapshot needs, and any available GNSS device documentation.
  Produce CAN ID, field, unit, range, and old topic mapping. No file mutation.
- `implementer`: edit only `evsn-runtime/gnss_can_driver/`, assigned IDL/test
  files after RTI gate, and package docs.
- `integration-tester`: build fake raw CAN stream tests and QNX/SIL gateway
  smoke commands.
- `reviewer`: verify QNX docs evidence, RTI MCP evidence, endian/scaling,
  stale behavior, UTC handling, no CAN0 command path, and test completeness.

Required skills:

- `rti-connext-dds`
- `qnx-cpp-rtos`
- `qnx-cpp-performance`
- `middleware-testing`
- `cpp-unit-testing`
- `cpp-integration-testing`
- `test-coverage-evidence`
- `safety-critical-cpp`

## Strict Verification Gate

This work item must link evidence to `VSN-0033`. Do not mark it complete on
happy-path decode unit tests alone.

Minimum evidence:

- Smoke tests:
  - valid config for bus/base ID/topic and bounded startup/shutdown
  - fake raw CAN input stream can produce one coherent GNSS state
  - no CAN0 transmit route exists
- Unit tests:
  - standard-ID filtering for `0x700..0x704`
  - extended-frame rejection
  - DLC validation before payload access
  - big-endian signed and unsigned decode helpers
  - latitude/longitude scaling and range checks
  - speed scaling and negative/reverse or invalid-speed policy
  - status and IMU bitfield extraction
  - height, heading, angular velocity, and acceleration decode
  - UTC calendar validation and UTC epoch conversion without local timezone
  - stale packet bitmask, partial-update behavior, sample counter, diagnostics
  - malformed payload and boundary values
- Integration tests:
  - fake gateway/raw-CAN topic to typed GNSS state after RTI gate clears
  - fake HAL or QNX/SIL raw CAN smoke with `dev-can-linux -L` when target/QEMU
    exists
  - drive-controller integration consumes typed GNSS speed or explicitly
    ignores stale GNSS input
- Regression tests:
  - no production dependency on ROS2 `can_broker/gps_driver`
  - no legacy `/gps/...` or `/car/speed/gps` public topics
  - hardware-equivalence regression proves IDs `0x700..0x704` are consumed
    from the CAN0 drive-control bus owner mirror and produce the same legacy
    scalar values used by drive/cruise paths, with future display consumption
    deferred to `VSN-0037`
  - no branded cloud GPS topic names
  - no final CAN0 motor command TX from GPS, gateway, telemetry, or cloud paths

Every evidence record must include command, platform, target/backend,
middleware domain/transport when DDS is used, CAN test vectors, logs, exit
status, residual risks, and known gaps.

## Implementation Steps

- [ ] Preserve the old CAN0 GPS/GNSS assignment unless new hardware evidence
      moves it to another bus.
- [ ] Confirm device manual or bench vectors for base ID, scales, units,
      heading units, angular velocity units, acceleration units, and UTC
      subsecond interpretation.
- [ ] Add `EVSNGnssState` or equivalent navigation IDL and topic registry under
      `VSN-0021` after RTI MCP guidance.
- [ ] Add broker/gateway config entries under `VSN-0022` for raw GPS CAN RX
      routing and optional filtering, with no payload decode in the gateway.
- [ ] Implement pure decode helpers and `GnssCanStateBuilder`.
- [ ] Add fake frame smoke and unit tests.
- [ ] Add DDS adapter and isolated-domain integration tests after RTI guidance.
- [ ] Update `VSN-0028` drive-controller plan to consume typed GNSS speed, not
      legacy `/car/speed/gps`.
- [ ] Wire package evidence into `VSN-0033`.

## Verification

- [ ] Command: GNSS pure unit tests
      Result:
- [ ] Command: fake raw CAN stream smoke
      Result:
- [ ] Command: hardware-equivalence GNSS CAN0 `0x700..0x704` regression
      Result:
- [ ] Command: DDS isolated-domain GNSS integration test after RTI gate
      Result:
- [ ] Command: QNX/SIL raw CAN smoke when target/QEMU exists
      Result:
- [ ] Command: `./init.sh check`
      Result:
- [ ] Command: `git diff --check`
      Result:

## Blockers

- RTI Connext MCP guidance is required before final GNSS DDS type/topic/QoS and
  generated-code implementation.
- Physical GPS/GNSS device documentation or bench vectors are needed to confirm
  ambiguous units and the UTC subsecond field.
- GPS/GNSS bus assignment must be confirmed. The legacy physical architecture
  places GPS on CAN0, which means target integration should consume the
  read-only CAN0 drive-control mirror rather than generic CAN0 TX routing.
- QNX/SIL or target evidence requires a configured `dev-can-linux -L`, QEMU, or
  physical CAN target contract.

## Handoff

This work item extends the CAN broker migration without putting decode logic
back into the generic broker. `VSN-0022` owns raw bus transport; `VSN-0036`
owns GPS/GNSS CAN IDs, decode, typed navigation state, and package-level tests.
