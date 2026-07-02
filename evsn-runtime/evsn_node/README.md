# EVSNNode - Generic DDS Node
 **Version: 3.0**
- **Authors: Ryan Wong | z5417983, Sebastian Moore | z5421325**

## Version Notes: v3.0
- Creating publishers now returns a typed `PublisherHandle<T>` instead of a numeric ID
- Publishing is now done via `handle.publish(data)`
- Publishing now includes compile-time type checking
- Publisher and subscriber creation have optional QoS overloads. Existing
  calls continue to use the RTI default QoS.
- Subscriber callbacks process every valid sample returned by `take()` so
  high-rate readers do not retain an older sample from a batch.
- Nodes load descriptor-based TOML parameter defaults and expose DDS-native
  parameter set/get/list/describe operations.

## Version Notes: v3.1
- `EVSNNode` constructs a process-supervision backend after node-name validation.
- Linux builds use a no-op backend with no QNX HAM headers, libraries, or
  runtime service dependency.
- QNX builds compile a HAM backend that self-attaches the process and requires
  `<ha/ham.h>` plus `libham`.
- Heartbeats are emitted from the real `spin()` dispatch path after timer
  callbacks or triggered DDS WaitSet conditions, rate-limited by the supervision
  heartbeat period.

## Version Notes: v3.2
- Legacy per-node broker bridge support has been removed from `EVSNNode`.
- Ordinary nodes are DDS-only; any cloud/app ingress belongs in dedicated bridge
  nodes with explicit mappings and reviewed authority boundaries.

## Version Notes: v3.3
- Subscriber endpoints now own their typed DataReader and WaitSet condition for
  the lifetime of the subscription.
- Loaned DDS samples are kept within the dispatch scope and copied into user
  callbacks.
- Topic validation no longer uses regex in the runtime path.
- `spin()` uses bounded WaitSet waits, explicit timer catch-up arithmetic, and
  heartbeats only after timer or DDS callback progress.
- Runtime-owned parameter DDS entities use explicit QoS/resource limits.

## Version Notes: v2.1
Thank you Seb for the great idea.
- Added static signal handler member function
    - The signal handler sets `running_` as `false`
- Changed `running_` to be static and initialised as `false` in evsn_node.cpp
- Removed `shutdown()` as this is handled by the signal handler
- Signals `SIGINT` and `SIGTERM` are now registered to call the signal handler in `spin()`

Known issue:
- Long-running user callbacks still delay shutdown until the callback returns.


## Introduction
EVSNNode is an RTI Connext Drive Professional-based DDS C++ base class designed
to simplify creation of nodes for an embedded vehicle network. Its design is
inspired by ROS2's `rclcpp::Node`, with a somewhat familiar interface and usage.

## Design Philosophy
- No DDS boilerplate for users
- User experience is DDS-agnostic
- Encapsulates execution model and event loop
- No funny threads, everything single-threaded for deterministic behaviour
- No direct memory management
- Similar to ROS2 for simplicity

## Features
As of the current version:
- Encapsulates all DDS entities and manages lifetime
- Provides APIs to create subcribers, publishers and timers with corresponding callbacks
- Provides API to publish data
- Templated functions to support any DDS type
- Manages the event loop and all callbacks/timers in a single-threaded fashion. (wow!)
- Automatically registers a signal handler to provide clean shutdown of event loop (SIGINT and SIGTERM)
- On QNX, self-attaches to QNX HAM for process supervision; on Linux, compiles
  the same EVSNNode runtime against an explicit no-op supervision backend.
- Publishes parameter state on `/evsn/parameters/state` and parameter events on
  `/evsn/parameters/event`

## API Reference
### Public Member Functions
These functions are designed to run in your main file.

```C++
void spin();
```
Starts the main event loop, registers the signal handler. 
- Run this in your `main()` function after node construction.
- It runs until `running_` is false, executing subscriber/timer callbacks when due.

---

### Protected Member Functions
These functions are to be run in your derived class' own member functions/constructor.

```C++
explicit EVSNNode(
    const std::string& name,
    int32_t domain_id = 0,
    const std::string& parameter_file = {});
```
Constructor for base node.
- Initialises DDS DomainParticipant, Publisher, Subscriber, WaitSet and name
- `name`: Name used for node (used for logging and node management WIP)
- `domain_id`: DDS Domain id, default 0
- `parameter_file`: optional explicit TOML defaults path for tests and
  specialised launchers. If omitted, `EVSN_PARAM_FILE`, `EVSN_PARAM_DIR`, and
  conventional `param/<node>_parameters.toml` locations are checked.
- Instantiate this base class in your dervied class's constructor.

---

```C++
virtual ~EVSNNode() = 0;
```
Pure virtual destructor.
- Please override this in your dervied class to clean up your resources

---

```C++
const std::string& get_name() const;
```
Returns the name of the node

---

```C++
std::size_t parameter_count() const;
std::string parameter_value_as_string(const std::string& parameter_name) const;
```
Inspection helpers for tests and diagnostics. Parameter mutation is performed
through the DDS request/reply service, not through node-local broker or direct
setter APIs.

---

```C++
template<typename T>
void create_subscriber(const std::string& topic_name, std::function<void(T)> callback);

template<typename T>
void create_subscriber(
    const std::string& topic_name,
    std::function<void(T)> callback,
    const dds::sub::qos::DataReaderQos& qos);
```
Creates a "subscriber" (DDS DataReader) entity and stores it internally. This should NOT be called DURING another callback! All subscribers and publishers should be created at construction time.
- `T`: DDS message type
- `topic_name`: Name of topic your subscriber subs to. Must be in this format: "/domain1/domain2/my_topic...". Any number of domains, but mus start with a /
- `callback`: Function called when a new valid message arrives. It must have a `void(T)` signature 
- Throws `std::invalid_argument` if `topic_name` isn't formatted properly

---

```C++
template<typename T>
PublisherHandle<T> create_publisher(const std::string& topic_name);

template<typename T>
PublisherHandle<T> create_publisher(
    const std::string& topic_name,
    const dds::pub::qos::DataWriterQos& qos);
```
Creates a "publisher" (DDS DataWriter) entity and stores it internally. This should NOT be called DURING another callback! All subscribers and publishers should be created at construction time.
- `T`: DDS type corresponding to your topic
-  `topic_name`: name of your topic your publisher will 
publish to.
- returns: typed publisher handle used to publish messages with `handle.publish(data)`
- Throws `std::invalid_argument` if `topic_name` isn't formatted properly

---

```C++
template<typename T>
class PublisherHandle {
public:
    template<typename U, typename std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, T>, int> = 0>
    void publish(U&& data) const;
};
```
Publishes message of type `T` using the publisher handle returned by `create_publisher<T>()`.
- Only one type `T` can be published to a given publisher
- Mismatched publish types are rejected at compile time - will show in IntelliSense/compiler output as "function cannot be referenced -- it is a deleted function" or similar.

---

```C++
void create_timer(std::chrono::milliseconds millis, std::function<void()> callback);
```
Creates a timer that calls the provided callback periodically. This should NOT be called DURING another callback! All subscribers and publishers should be created at construction time.
- `millis`: std::chrono::milliseconds timer interval
- `callback` Function executed each interval. It must have a `void()` signature

---

## Example Usage
Examples can be added under `src/`.

## Lifecycle Model
The node lifecycle follows this sequence:

1. Node constructed
2. Publishers/subscribers/timers created in derived constructor
3. `spin()` called
    - Registers signal handlers
    - Initialises timer's trigger_time to be now + period
4. WaitSet waits for events
    - Sleep timeout = time until earliest timer trigger_time
5. Callbacks executed
    - DataReader conditions wake the owner thread and dispatch valid samples
      through owned subscription records
    - The internal parameter-service timer polls DDS request/reply and publishes
      accepted state/event updates on the same spin thread.
    - Each loop turn reassesses timers and caps WaitSet sleep for bounded
      signal-driven shutdown latency.
6. If SIGTERM or SIGINT reaches process, signal_handler is executed, atomic bool running_ is cleared
7. `spin()` exits
8. Node destructed

## QNX HAM Supervision (Henry Jiang z5416365)

The supervision backend is selected at build time:

- Linux and other non-QNX builds compile `noop_node_supervisor.cpp`; heartbeat
  and shutdown calls are no-ops and do not mask QNX compile/link failures.
- QNX builds compile `qnx_ham_node_supervisor.cpp`. CMake fails if
  `<ha/ham.h>` or `libham` is unavailable.

The QNX backend self-attaches with a sanitised entity name derived from the node
name, using the form `evsn_node_<node>`. Its default policy is:

- heartbeat period: 1 s, override with `EVSN_HAM_HEARTBEAT_MS`; QNX requires at
  least `HAMHBEATMIN`
- low missed-heartbeat threshold: 3, override with
  `EVSN_HAM_MISSED_HEARTBEAT_LOW`
- high missed-heartbeat threshold: 5, override with
  `EVSN_HAM_MISSED_HEARTBEAT_HIGH`
- death condition: log and restart
- restart command: `EVSN_HAM_RESTART_COMMAND`, or the executable path read from
  `/proc/self/exefile` when unset
- low/high missed heartbeat: log
- optional high missed-heartbeat escalation:
  `EVSN_HAM_HEARTBEAT_HIGH_COMMAND`
- normal shutdown: `ham_detach_self()`

`EVSNNode::spin()` does not run a separate watchdog thread. It calls the
supervisor heartbeat only after timer callbacks have executed or after
triggered DDS WaitSet conditions have been dispatched. Production nodes must
therefore have expected timer or DDS activity within the configured heartbeat
window, or their QNX HAM heartbeat thresholds must be configured accordingly.

HAM restart is process supervision. RTI Connext participant rediscovery,
volatile command-topic behavior, and stale-sample handling remain DDS QoS and
application-contract responsibilities.

## DDS-Native Parameters

Each node may declare safe startup defaults in
`param/<node>_parameters.toml`. The file contains a schema version, node name,
fallback policy, and a list of parameter descriptors:

```toml
schema_version = 1
node = "example_node"
fallback = "Use declared safe defaults."

[[parameters]]
name = "publish_period_ms"
type = "int64"
default = 1000
mutable = true
min = 100
max = 10000
unit = "ms"
description = "Example publish period."
fallback = "Publish once per second."
```

Supported types are `bool`, `int64`, `float64`, and `string`. Numeric
parameters may declare `min` and `max`. Only parameters marked `mutable = true`
accept runtime set requests; immutable values are still visible through get,
list, describe, and state topics.

`EVSNParameterService` uses RTI request/reply for set/get/list/describe, publishes
current state on `/evsn/parameters/state`, and publishes accepted or rejected set
attempts on `/evsn/parameters/event`. The state topic is keyed by node name and
parameter name with reliable transient-local keep-last(1) QoS for late joiners.
The event topic is reliable keep-all while the writer is alive.

## Dependencies
- RTI Connext DDS 7.0.0 Core libraries or newer
- If using LM libraries, a valid RTI Connext Drive licese
- Properly installed and sourced environment (`NDDSHOME`)
- QNX builds of `evsn_node` require QNX HAM development files: `<ha/ham.h>` and
  `libham`

RTI Connext DDS environment must be sourced before building:

```bash
source $NDDSHOME/resource/scripts/rtisetenv_x64Linux4gcc7.3.0.bash
```

## Maintainers
`Ryan Wong | z5417983`
