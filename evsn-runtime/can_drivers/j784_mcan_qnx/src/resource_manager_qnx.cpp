#ifndef __QNXNTO__
#error "resource_manager_qnx.cpp must only be built for QNX targets"
#endif

#ifndef _QNX_SOURCE
#define _QNX_SOURCE 1
#endif

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/mcan_message_ram.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/mcan_registers.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/qnx_can_compat.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/qnx_mmio_mapper.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/resource_manager_runtime.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/transceiver_control.hpp"

#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

#include <devctl.h>
#include <fcntl.h>
#include <hw/i2c.h>
#include <pthread.h>
#include <signal.h>
#include <sys/can_dcmd.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>
#include <sys/iomsg.h>
#include <sys/neutrino.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SETIOV
#define SETIOV(_iov, _addr, _len)                                              \
  ((_iov)->iov_base = const_cast<void *>(static_cast<const void *>(_addr)),    \
   (_iov)->iov_len = (_len))
#endif

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

static inline constexpr auto max_registered_endpoints =
    kMaxControllers *
    ((kMaxQueuesPerController * std::size_t{4U}) + std::size_t{1U});

struct skeleton_state;

struct hardware_loopback_state {
  McanMappedMmioRegion controller{};
  McanMappedMmioRegion message_ram{};
  McanMessageRamLayout layout{};
  McanControllerCapabilities capabilities{};
  McanDiagnosticsSnapshot diagnostics{};
  std::array<McanFrameTransfer, kMaxQueueCapacity> rx_queue{};
  pthread_mutex_t lock{};
  pthread_cond_t startup_cond{};
  pthread_t ist_thread{};
  skeleton_state *owner{nullptr};
  std::size_t rx_head{0U};
  std::size_t rx_tail{0U};
  std::size_t rx_count{0U};
  std::uint32_t logical_irq{0U};
  std::uint8_t instance_id{0U};
  int interrupt_id{-1};
  int ist_attach_errno{0};
  bool initialized{false};
  bool lock_initialized{false};
  bool cond_initialized{false};
  bool interrupt_mode{false};
  bool interrupts_enabled{false};
  bool internal_loopback{false};
  bool ist_thread_started{false};
  bool ist_attach_complete{false};
  bool ist_attach_failed{false};
  bool ist_stop_requested{false};
  bool tx_in_flight{false};
  bool bus_off_latched{false};
  bool error_passive_latched{false};
};

struct endpoint_attachment {
  iofunc_attr_t attr{};
  McanEndpointPath path{};
  skeleton_state *owner{nullptr};
  McanResourceManagerRuntime *runtime{nullptr};
  hardware_loopback_state *hardware{nullptr};
  EndpointKind kind{EndpointKind::ctl};
  std::uint8_t instance_id{0U};
  std::uint8_t endpoint_id{0U};
  bool qnx_can_compat{false};
  McanFrameTransfer read_reply{};
  iofunc_notify_t notify[3]{};
  pthread_mutex_t notify_lock{};
  rcvid_t pending_read_rcvid{0};
  rcvid_t pending_raw_rx_devctl_rcvid{0};
  int attach_id{-1};
  bool notify_lock_initialized{false};
  bool pending_read_active{false};
  bool pending_raw_rx_devctl_active{false};
};

static_assert(offsetof(endpoint_attachment, attr) == 0U,
              "endpoint attr must remain first for OCB lookup");

struct skeleton_state {
  dispatch_t *dispatch{nullptr};
  resmgr_attr_t resmgr_attr{};
  resmgr_connect_funcs_t connect_funcs{};
  resmgr_io_funcs_t io_funcs{};
  McanResourceManagerRuntime runtime{};
  hardware_loopback_state hardware_loopback{};
  bool hardware_loopback_enabled{false};
  bool qnx_can_compat_enabled{false};
  bool qnx_can_raw_mode_enabled{false};
  bool qnx_can_listen_only{false};
  std::array<endpoint_attachment, max_registered_endpoints> endpoints{};
  std::size_t endpoint_count{0U};
};

[[nodiscard]] McanStatus
endpoint_receive_frame(endpoint_attachment &endpoint,
                       McanFrameTransfer &frame) noexcept;
[[nodiscard]] McanStatus
endpoint_transmit_frame(endpoint_attachment &endpoint,
                        const McanFrameTransfer &frame) noexcept;
[[nodiscard]] McanStatus
endpoint_query_capabilities(endpoint_attachment &endpoint,
                            McanControllerCapabilities &capabilities) noexcept;
[[nodiscard]] McanStatus
endpoint_query_diagnostics(endpoint_attachment &endpoint,
                           McanDiagnosticsSnapshot &diagnostics) noexcept;
[[nodiscard]] McanStatus
endpoint_rx_ready_count(endpoint_attachment &endpoint,
                        std::size_t &ready_count) noexcept;
[[nodiscard]] McanStatus
endpoint_tx_room_count(endpoint_attachment &endpoint,
                       std::size_t &room_count) noexcept;
[[nodiscard]] McanStatus
write_controller_register(const McanMappedMmioRegion &controller,
                          McanCfgRegisterOffset offset,
                          std::uint32_t value) noexcept;
[[nodiscard]] McanStatus
initialize_hardware_loopback_state(hardware_loopback_state &state,
                                   const McanControllerConfig &controller,
                                   bool internal_loopback, bool interrupt_mode,
                                   skeleton_state *owner) noexcept;
[[nodiscard]] McanStatus
shutdown_hardware_loopback_state(hardware_loopback_state &state) noexcept;

struct probe_register {
  const char *name{nullptr};
  std::uint32_t offset{0U};
};

static inline constexpr auto subsystem_probe_registers =
    std::array<probe_register, 4U>{
        probe_register{"pid", offset_value(McanSsRegisterOffset::pid)},
        probe_register{"ctrl", offset_value(McanSsRegisterOffset::ctrl)},
        probe_register{"stat", offset_value(McanSsRegisterOffset::stat)},
        probe_register{"ie", offset_value(McanSsRegisterOffset::ie)}};

static inline constexpr auto controller_probe_registers =
    std::array<probe_register, 9U>{
        probe_register{"crel", offset_value(McanCfgRegisterOffset::crel)},
        probe_register{"endn", offset_value(McanCfgRegisterOffset::endn)},
        probe_register{"cccr", offset_value(McanCfgRegisterOffset::cccr)},
        probe_register{"ecr", offset_value(McanCfgRegisterOffset::ecr)},
        probe_register{"psr", offset_value(McanCfgRegisterOffset::psr)},
        probe_register{"ir", offset_value(McanCfgRegisterOffset::ir)},
        probe_register{"ie", offset_value(McanCfgRegisterOffset::ie)},
        probe_register{"ils", offset_value(McanCfgRegisterOffset::ils)},
        probe_register{"ile", offset_value(McanCfgRegisterOffset::ile)}};

static inline constexpr auto kCccrInit = std::uint32_t{1U << 0U};
static inline constexpr auto kCccrConfigChangeEnable = std::uint32_t{1U << 1U};
static inline constexpr auto kIrRxFifo0NewMessage = std::uint32_t{1U << 0U};
static inline constexpr auto kIrRxFifo0Full = std::uint32_t{1U << 2U};
static inline constexpr auto kIrRxFifo0MessageLost = std::uint32_t{1U << 3U};
static inline constexpr auto kIrTxComplete = std::uint32_t{1U << 9U};
static inline constexpr auto kIrTxCancellationFinished =
    std::uint32_t{1U << 10U};
static inline constexpr auto kIrMessageRamAccessFailure =
    std::uint32_t{1U << 17U};
static inline constexpr auto kIrErrorLoggingOverflow = std::uint32_t{1U << 22U};
static inline constexpr auto kIrErrorPassive = std::uint32_t{1U << 23U};
static inline constexpr auto kIrWarning = std::uint32_t{1U << 24U};
static inline constexpr auto kIrBusOff = std::uint32_t{1U << 25U};
static inline constexpr auto kIrProtocolArbitration = std::uint32_t{1U << 27U};
static inline constexpr auto kIrProtocolData = std::uint32_t{1U << 28U};
static inline constexpr auto kIrLoopbackErrorMask =
    kIrMessageRamAccessFailure | kIrBusOff | kIrProtocolArbitration |
    kIrProtocolData;
static inline constexpr auto kIrProductionErrorMask =
    kIrRxFifo0Full | kIrRxFifo0MessageLost | kIrMessageRamAccessFailure |
    kIrErrorLoggingOverflow | kIrErrorPassive | kIrWarning | kIrBusOff |
    kIrProtocolArbitration | kIrProtocolData;
static inline constexpr auto kRxFifo0FillLevelMask = std::uint32_t{0x7FU};
static inline constexpr auto kRxFifo0GetIndexShift = std::uint32_t{8U};
static inline constexpr auto kRxFifo0GetIndexMask = std::uint32_t{0x3FU};
static inline constexpr auto kTxBuffer0Request = std::uint32_t{1U};
static inline constexpr auto kLoopbackPollIterations = std::uint32_t{200000U};
static inline constexpr auto kLoopbackClearMessageRamWords =
    std::uint32_t{1024U};
static inline constexpr auto kResourceManagerMsgMaxSize =
    std::size_t{sizeof(io_write_t) + sizeof(McanFrameTransfer)};
static inline constexpr auto kResourceManagerDevctlMsgMaxSize =
    std::size_t{sizeof(io_devctl_t) + 128U};
static inline constexpr auto kNotifySlotCount = int{3};
static volatile sig_atomic_t g_resource_manager_shutdown_requested = 0;

[[nodiscard]] constexpr std::size_t max_size(const std::size_t lhs,
                                             const std::size_t rhs) noexcept {
  return lhs > rhs ? lhs : rhs;
}

[[nodiscard]] endpoint_attachment *
endpoint_from_ocb(RESMGR_OCB_T *const ocb) noexcept {
  if (ocb == nullptr) {
    return nullptr;
  }
  auto *const iofunc_ocb = static_cast<iofunc_ocb_t *>(ocb);
  if (iofunc_ocb->attr == nullptr) {
    return nullptr;
  }
  return reinterpret_cast<endpoint_attachment *>(iofunc_ocb->attr);
}

void request_resource_manager_shutdown(const int signo) noexcept {
  (void)signo;
  g_resource_manager_shutdown_requested = 1;
}

[[nodiscard]] McanStatus
initialize_endpoint_notify_lock(endpoint_attachment &endpoint) noexcept {
  if (::pthread_mutex_init(&endpoint.notify_lock, nullptr) != EOK) {
    return McanStatus::hardware_access_unavailable;
  }
  endpoint.notify_lock_initialized = true;
  return McanStatus::ok;
}

void destroy_endpoint_notify_lock(endpoint_attachment &endpoint) noexcept {
  if (endpoint.notify_lock_initialized) {
    (void)::pthread_mutex_destroy(&endpoint.notify_lock);
    endpoint.notify_lock_initialized = false;
  }
}

void destroy_endpoint_notify_locks(skeleton_state &state) noexcept {
  for (auto index = std::size_t{0U}; index < state.endpoint_count; ++index) {
    destroy_endpoint_notify_lock(state.endpoints[index]);
  }
}

[[nodiscard]] McanStatus
lock_endpoint_notify(endpoint_attachment &endpoint) noexcept {
  return endpoint.notify_lock_initialized &&
                 ::pthread_mutex_lock(&endpoint.notify_lock) == EOK
             ? McanStatus::ok
             : McanStatus::hardware_access_unavailable;
}

void unlock_endpoint_notify(endpoint_attachment &endpoint) noexcept {
  if (endpoint.notify_lock_initialized) {
    (void)::pthread_mutex_unlock(&endpoint.notify_lock);
  }
}

[[nodiscard]] int install_shutdown_signal_handlers() noexcept {
  g_resource_manager_shutdown_requested = 0;

  struct sigaction action{};
  action.sa_handler = request_resource_manager_shutdown;
  action.sa_flags = 0;
  if (::sigemptyset(&action.sa_mask) == -1) {
    return errno;
  }
  if (::sigaction(SIGTERM, &action, nullptr) == -1) {
    return errno;
  }
  if (::sigaction(SIGINT, &action, nullptr) == -1) {
    return errno;
  }
  if (::sigaction(SIGHUP, &action, nullptr) == -1) {
    return errno;
  }
  return EOK;
}

void trigger_endpoint_notify(endpoint_attachment &endpoint) noexcept {
  if (endpoint.kind == EndpointKind::rx) {
    auto ready_count = std::size_t{0U};
    const auto status = endpoint_rx_ready_count(endpoint, ready_count);
    if (status_ok(status) && status_ok(lock_endpoint_notify(endpoint))) {
      if (IOFUNC_NOTIFY_INPUT_CHECK(endpoint.notify,
                                    static_cast<int>(ready_count), 1) != 0) {
        iofunc_notify_trigger(endpoint.notify, static_cast<int>(ready_count),
                              IOFUNC_NOTIFY_INPUT);
      }
      unlock_endpoint_notify(endpoint);
    }
  } else if (endpoint.kind == EndpointKind::tx) {
    auto room_count = std::size_t{0U};
    const auto status = endpoint_tx_room_count(endpoint, room_count);
    if (status_ok(status) && status_ok(lock_endpoint_notify(endpoint))) {
      if (IOFUNC_NOTIFY_OUTPUT_CHECK(endpoint.notify,
                                     static_cast<int>(room_count)) != 0) {
        iofunc_notify_trigger(endpoint.notify, static_cast<int>(room_count),
                              IOFUNC_NOTIFY_OUTPUT);
      }
      unlock_endpoint_notify(endpoint);
    }
  }
}

void trigger_matching_endpoint_notify(skeleton_state &state,
                                      const EndpointKind kind,
                                      const std::uint8_t instance_id,
                                      const std::uint8_t endpoint_id) noexcept {
  for (auto index = std::size_t{0U}; index < state.endpoint_count; ++index) {
    auto &endpoint = state.endpoints[index];
    if (endpoint.kind == kind && endpoint.instance_id == instance_id &&
        endpoint.endpoint_id == endpoint_id) {
      trigger_endpoint_notify(endpoint);
    }
  }
}

[[nodiscard]] std::size_t
next_hardware_queue_index(const std::size_t index,
                          const std::size_t capacity) noexcept {
  return (index + 1U) == capacity ? 0U : index + 1U;
}

[[nodiscard]] McanStatus
initialize_hardware_thread_objects(hardware_loopback_state &state) noexcept {
  if (::pthread_mutex_init(&state.lock, nullptr) != EOK) {
    return McanStatus::hardware_access_unavailable;
  }
  state.lock_initialized = true;
  if (::pthread_cond_init(&state.startup_cond, nullptr) != EOK) {
    (void)::pthread_mutex_destroy(&state.lock);
    state.lock_initialized = false;
    return McanStatus::hardware_access_unavailable;
  }
  state.cond_initialized = true;
  return McanStatus::ok;
}

void destroy_hardware_thread_objects(hardware_loopback_state &state) noexcept {
  if (state.cond_initialized) {
    (void)::pthread_cond_destroy(&state.startup_cond);
    state.cond_initialized = false;
  }
  if (state.lock_initialized) {
    (void)::pthread_mutex_destroy(&state.lock);
    state.lock_initialized = false;
  }
}

[[nodiscard]] McanStatus
lock_hardware_state(hardware_loopback_state &state) noexcept {
  return state.lock_initialized && ::pthread_mutex_lock(&state.lock) == EOK
             ? McanStatus::ok
             : McanStatus::hardware_access_unavailable;
}

void unlock_hardware_state(hardware_loopback_state &state) noexcept {
  if (state.lock_initialized) {
    (void)::pthread_mutex_unlock(&state.lock);
  }
}

[[nodiscard]] bool
hardware_rx_queue_has_room(const hardware_loopback_state &state) noexcept {
  return state.rx_count < state.layout.rx_fifo0.count &&
         state.rx_count < state.rx_queue.size();
}

void enqueue_hardware_rx_frame_locked(hardware_loopback_state &state,
                                      const McanFrameTransfer &frame) noexcept {
  state.rx_queue[state.rx_head] = frame;
  state.rx_head =
      next_hardware_queue_index(state.rx_head, state.layout.rx_fifo0.count);
  ++state.rx_count;
  ++state.diagnostics.rx_frames;
}

[[nodiscard]] McanStatus
dequeue_hardware_rx_frame_locked(hardware_loopback_state &state,
                                 McanFrameTransfer &frame) noexcept {
  frame = McanFrameTransfer{};
  if (state.rx_count == 0U) {
    return McanStatus::no_frame;
  }
  frame = state.rx_queue[state.rx_tail];
  state.rx_queue[state.rx_tail] = McanFrameTransfer{};
  state.rx_tail =
      next_hardware_queue_index(state.rx_tail, state.layout.rx_fifo0.count);
  --state.rx_count;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus hardware_interrupt_mask(std::uint32_t &mask) noexcept {
  mask = mcan_rx_tx_error_interrupt_mask() | kIrTxCancellationFinished;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
enable_hardware_interrupts(hardware_loopback_state &state) noexcept {
  auto mask = std::uint32_t{0U};
  auto status = hardware_interrupt_mask(mask);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(state.controller,
                                     McanCfgRegisterOffset::ir, 0xFFFFFFFFU);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(state.controller,
                                     McanCfgRegisterOffset::ils, 0U);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(state.controller,
                                     McanCfgRegisterOffset::ie, mask);
  if (!status_ok(status)) {
    return status;
  }
  return write_controller_register(state.controller, McanCfgRegisterOffset::ile,
                                   1U);
}

[[nodiscard]] McanStatus
disable_hardware_interrupts(hardware_loopback_state &state) noexcept {
  auto first_failure = McanStatus::ok;
  auto status = write_controller_register(state.controller,
                                          McanCfgRegisterOffset::ile, 0U);
  if (!status_ok(status)) {
    first_failure = status;
  }
  status = write_controller_register(state.controller,
                                     McanCfgRegisterOffset::ie, 0U);
  if (!status_ok(status) && status_ok(first_failure)) {
    first_failure = status;
  }
  status = write_controller_register(state.controller,
                                     McanCfgRegisterOffset::ir, 0xFFFFFFFFU);
  if (!status_ok(status) && status_ok(first_failure)) {
    first_failure = status;
  }
  state.interrupts_enabled = false;
  return first_failure;
}

void qnx_can_message_from_native(const struct can_msg &native,
                                 QnxCanCompatCanMessage &message) noexcept {
  message = QnxCanCompatCanMessage{};
  message.length = native.len;
  message.message_id = native.mid;
  message.timestamp = native.ext.timestamp;
  message.extended_id = native.ext.is_extended_mid != 0U;
  message.remote_frame = native.ext.is_remote_frame != 0U;
  for (auto index = std::size_t{0U}; index < message.data.size(); ++index) {
    message.data[index] = native.dat[index];
  }
}

void qnx_can_message_to_native(const QnxCanCompatCanMessage &message,
                               struct can_msg &native) noexcept {
  std::memset(&native, 0, sizeof(native));
  native.len = message.length;
  native.mid = message.message_id;
  native.ext.timestamp = message.timestamp;
  native.ext.is_extended_mid = message.extended_id ? 1U : 0U;
  native.ext.is_remote_frame = message.remote_frame ? 1U : 0U;
  for (auto index = std::size_t{0U}; index < message.data.size(); ++index) {
    native.dat[index] = message.data[index];
  }
}

[[nodiscard]] bool
endpoint_current_nominal_timing(const endpoint_attachment &endpoint,
                                McanBitTiming &timing) noexcept {
  timing = McanBitTiming{};
  if (endpoint.hardware != nullptr) {
    timing = make_classic_500k_timing();
    return true;
  }
  if (endpoint.runtime == nullptr || !endpoint.runtime->initialized) {
    return false;
  }
  for (auto index = std::size_t{0U}; index < endpoint.runtime->controller_count;
       ++index) {
    if (endpoint.runtime->config.controllers[index].instance_id ==
        endpoint.instance_id) {
      timing = endpoint.runtime->cores[index].config.nominal_timing;
      return true;
    }
  }
  return false;
}

[[nodiscard]] bool qnx_can_timing_matches_current(
    const endpoint_attachment &endpoint,
    const struct can_devctl_timing &requested) noexcept {
  auto current = McanBitTiming{};
  if (!endpoint_current_nominal_timing(endpoint, current)) {
    return false;
  }
  if (requested.ref_clock_freq != 0U &&
      requested.ref_clock_freq != current.source_clock_hz) {
    return false;
  }
  return requested.bit_rate_prescaler == current.prescaler &&
         requested.sync_jump_width == current.sync_jump_width &&
         requested.time_segment_1 == current.time_segment_before_sample &&
         requested.time_segment_2 == current.time_segment_after_sample;
}

struct qnx_can_raw_rx_devctl_reply {
  struct _io_devctl_reply header{};
  struct can_msg frame{};
};

static_assert(offsetof(qnx_can_raw_rx_devctl_reply, frame) ==
                  sizeof(struct _io_devctl_reply),
              "raw CAN devctl payload must immediately follow header");

[[nodiscard]] bool endpoint_uses_hardware_interrupts(
    const endpoint_attachment &endpoint) noexcept {
  return endpoint.hardware != nullptr && endpoint.hardware->interrupt_mode;
}

[[nodiscard]] bool ocb_is_nonblocking(RESMGR_OCB_T *const ocb) noexcept {
  if (ocb == nullptr) {
    return false;
  }
  const auto *const iofunc_ocb = static_cast<iofunc_ocb_t *>(ocb);
  return (iofunc_ocb->ioflag & O_NONBLOCK) != 0;
}

[[nodiscard]] int
native_can_message_from_frame(const McanFrameTransfer &frame,
                              struct can_msg &native) noexcept {
  auto compat_message = QnxCanCompatCanMessage{};
  const auto status = qnx_can_compat_message_from_frame(frame, compat_message);
  if (!status_ok(status)) {
    return resource_manager_status_to_errno(status);
  }
  qnx_can_message_to_native(compat_message, native);
  return EOK;
}

void reply_pending_read(const rcvid_t rcvid,
                        const McanFrameTransfer &frame) noexcept {
  struct iovec reply_iov{};
  reply_iov.iov_base = const_cast<McanFrameTransfer *>(&frame);
  reply_iov.iov_len = sizeof(frame);
  (void)MsgReplyv(rcvid, static_cast<long>(sizeof(frame)), &reply_iov, 1U);
}

void reply_pending_raw_rx_devctl(const rcvid_t rcvid,
                                 const McanFrameTransfer &frame) noexcept {
  auto reply = qnx_can_raw_rx_devctl_reply{};
  const auto status = native_can_message_from_frame(frame, reply.frame);
  if (status != EOK) {
    (void)MsgError(rcvid, status);
    return;
  }
  reply.header.ret_val = EOK;
  reply.header.nbytes = sizeof(reply.frame);
  (void)MsgReply(rcvid, EOK, &reply, sizeof(reply));
}

void cancel_endpoint_waiters(endpoint_attachment &endpoint,
                             const int error_code) noexcept {
  auto read_rcvid = rcvid_t{0};
  auto raw_rx_rcvid = rcvid_t{0};
  if (endpoint_uses_hardware_interrupts(endpoint) &&
      status_ok(lock_hardware_state(*endpoint.hardware))) {
    if (endpoint.pending_read_active) {
      read_rcvid = endpoint.pending_read_rcvid;
      endpoint.pending_read_rcvid = 0;
      endpoint.pending_read_active = false;
    }
    if (endpoint.pending_raw_rx_devctl_active) {
      raw_rx_rcvid = endpoint.pending_raw_rx_devctl_rcvid;
      endpoint.pending_raw_rx_devctl_rcvid = 0;
      endpoint.pending_raw_rx_devctl_active = false;
    }
    unlock_hardware_state(*endpoint.hardware);
  }
  if (read_rcvid != 0) {
    (void)MsgError(read_rcvid, error_code);
  }
  if (raw_rx_rcvid != 0) {
    (void)MsgError(raw_rx_rcvid, error_code);
  }
}

[[nodiscard]] bool
cancel_endpoint_waiter_by_rcvid(endpoint_attachment &endpoint,
                                const rcvid_t rcvid,
                                const int error_code) noexcept {
  if (rcvid == 0 || !endpoint_uses_hardware_interrupts(endpoint)) {
    return false;
  }
  auto matched = false;
  if (status_ok(lock_hardware_state(*endpoint.hardware))) {
    if (endpoint.pending_read_active && endpoint.pending_read_rcvid == rcvid) {
      endpoint.pending_read_rcvid = 0;
      endpoint.pending_read_active = false;
      matched = true;
    }
    if (endpoint.pending_raw_rx_devctl_active &&
        endpoint.pending_raw_rx_devctl_rcvid == rcvid) {
      endpoint.pending_raw_rx_devctl_rcvid = 0;
      endpoint.pending_raw_rx_devctl_active = false;
      matched = true;
    }
    unlock_hardware_state(*endpoint.hardware);
  }
  if (matched) {
    (void)MsgError(rcvid, error_code);
  }
  return matched;
}

void cancel_all_endpoint_waiters(skeleton_state &state,
                                 const int error_code) noexcept {
  for (auto index = std::size_t{0U}; index < state.endpoint_count; ++index) {
    cancel_endpoint_waiters(state.endpoints[index], error_code);
  }
}

[[nodiscard]] int hardware_receive_or_queue_read(endpoint_attachment &endpoint,
                                                 const rcvid_t rcvid,
                                                 const bool blocking,
                                                 McanFrameTransfer &frame,
                                                 bool &queued) noexcept {
  queued = false;
  frame = McanFrameTransfer{};
  if (!endpoint_uses_hardware_interrupts(endpoint)) {
    return EINVAL;
  }
  auto &hardware = *endpoint.hardware;
  auto lock_status = lock_hardware_state(hardware);
  if (!status_ok(lock_status)) {
    return resource_manager_status_to_errno(lock_status);
  }
  const auto receive_status = dequeue_hardware_rx_frame_locked(hardware, frame);
  if (status_ok(receive_status)) {
    unlock_hardware_state(hardware);
    return EOK;
  }
  if (receive_status != McanStatus::no_frame) {
    unlock_hardware_state(hardware);
    return resource_manager_status_to_errno(receive_status);
  }
  if (!blocking) {
    unlock_hardware_state(hardware);
    return EAGAIN;
  }
  if (endpoint.pending_read_active) {
    unlock_hardware_state(hardware);
    return EBUSY;
  }
  endpoint.pending_read_rcvid = rcvid;
  endpoint.pending_read_active = true;
  queued = true;
  unlock_hardware_state(hardware);
  return EOK;
}

[[nodiscard]] int hardware_receive_or_queue_raw_rx_devctl(
    endpoint_attachment &endpoint, const rcvid_t rcvid, const bool blocking,
    McanFrameTransfer &frame, bool &queued) noexcept {
  queued = false;
  frame = McanFrameTransfer{};
  if (!endpoint_uses_hardware_interrupts(endpoint)) {
    return EINVAL;
  }
  auto &hardware = *endpoint.hardware;
  auto lock_status = lock_hardware_state(hardware);
  if (!status_ok(lock_status)) {
    return resource_manager_status_to_errno(lock_status);
  }
  const auto receive_status = dequeue_hardware_rx_frame_locked(hardware, frame);
  if (status_ok(receive_status)) {
    unlock_hardware_state(hardware);
    return EOK;
  }
  if (receive_status != McanStatus::no_frame) {
    unlock_hardware_state(hardware);
    return resource_manager_status_to_errno(receive_status);
  }
  if (!blocking) {
    unlock_hardware_state(hardware);
    return EAGAIN;
  }
  if (endpoint.pending_raw_rx_devctl_active) {
    unlock_hardware_state(hardware);
    return EBUSY;
  }
  endpoint.pending_raw_rx_devctl_rcvid = rcvid;
  endpoint.pending_raw_rx_devctl_active = true;
  queued = true;
  unlock_hardware_state(hardware);
  return EOK;
}

void complete_endpoint_pending_rx_waiters(
    endpoint_attachment &endpoint) noexcept {
  if (!endpoint_uses_hardware_interrupts(endpoint) ||
      endpoint.kind != EndpointKind::rx) {
    return;
  }

  auto read_rcvid = rcvid_t{0};
  auto read_frame = McanFrameTransfer{};
  auto raw_rx_rcvid = rcvid_t{0};
  auto raw_rx_frame = McanFrameTransfer{};
  auto &hardware = *endpoint.hardware;
  if (status_ok(lock_hardware_state(hardware))) {
    if (endpoint.pending_read_active && hardware.rx_count > 0U) {
      if (status_ok(dequeue_hardware_rx_frame_locked(hardware, read_frame))) {
        read_rcvid = endpoint.pending_read_rcvid;
        endpoint.pending_read_rcvid = 0;
        endpoint.pending_read_active = false;
      }
    }
    if (endpoint.pending_raw_rx_devctl_active && hardware.rx_count > 0U) {
      if (status_ok(dequeue_hardware_rx_frame_locked(hardware, raw_rx_frame))) {
        raw_rx_rcvid = endpoint.pending_raw_rx_devctl_rcvid;
        endpoint.pending_raw_rx_devctl_rcvid = 0;
        endpoint.pending_raw_rx_devctl_active = false;
      }
    }
    unlock_hardware_state(hardware);
  }

  if (read_rcvid != 0) {
    reply_pending_read(read_rcvid, read_frame);
  }
  if (raw_rx_rcvid != 0) {
    reply_pending_raw_rx_devctl(raw_rx_rcvid, raw_rx_frame);
  }
}

void complete_matching_rx_waiters(skeleton_state &state,
                                  const std::uint8_t instance_id,
                                  const std::uint8_t endpoint_id) noexcept {
  for (auto index = std::size_t{0U}; index < state.endpoint_count; ++index) {
    auto &endpoint = state.endpoints[index];
    if (endpoint.kind == EndpointKind::rx &&
        endpoint.instance_id == instance_id &&
        endpoint.endpoint_id == endpoint_id) {
      complete_endpoint_pending_rx_waiters(endpoint);
    }
  }
}

int qnx_can_compat_devctl(resmgr_context_t *ctp, io_devctl_t *msg,
                          endpoint_attachment &endpoint) {
  if (endpoint.owner == nullptr || !endpoint.owner->qnx_can_compat_enabled) {
    return ENOTSUP;
  }

  const auto command = static_cast<std::uint32_t>(msg->i.dcmd);
  if (command == static_cast<std::uint32_t>(CAN_DEVCTL_SET_TIMING)) {
    if (ctp->size < sizeof(msg->i) + sizeof(struct can_devctl_timing)) {
      return EBADMSG;
    }
    const auto *const requested =
        static_cast<const struct can_devctl_timing *>(_IO_INPUT_PAYLOAD(msg));
    if (requested == nullptr) {
      return EBADMSG;
    }
    if (!qnx_can_timing_matches_current(endpoint, *requested)) {
      return ENOTSUP;
    }
    std::memset(&msg->o, 0, sizeof(msg->o));
    msg->o.ret_val = EOK;
    msg->o.nbytes = 0U;
    return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o));
  }

  if (!endpoint.owner->qnx_can_raw_mode_enabled) {
    return ENOTSUP;
  }

  if (command == static_cast<std::uint32_t>(CAN_DEVCTL_TX_FRAME_RAW)) {
    if (endpoint.kind != EndpointKind::tx) {
      return EINVAL;
    }
    if (endpoint.owner->qnx_can_listen_only) {
      return EACCES;
    }
    if (ctp->size < sizeof(msg->i) + sizeof(struct can_msg)) {
      return EBADMSG;
    }
    const auto *const native =
        static_cast<const struct can_msg *>(_IO_INPUT_PAYLOAD(msg));
    if (native == nullptr) {
      return EBADMSG;
    }
    auto compat_message = QnxCanCompatCanMessage{};
    qnx_can_message_from_native(*native, compat_message);
    auto frame = McanFrameTransfer{};
    auto operation_status =
        qnx_can_compat_frame_from_message(compat_message, frame);
    if (!status_ok(operation_status)) {
      return resource_manager_status_to_errno(operation_status);
    }
    operation_status = endpoint_transmit_frame(endpoint, frame);
    if (!status_ok(operation_status)) {
      return qnx_can_compat_raw_tx_status_to_errno(operation_status);
    }
    endpoint.attr.flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
    if (!endpoint_uses_hardware_interrupts(endpoint)) {
      trigger_endpoint_notify(endpoint);
    }
    if (endpoint.owner != nullptr &&
        !endpoint_uses_hardware_interrupts(endpoint)) {
      trigger_matching_endpoint_notify(*endpoint.owner, EndpointKind::rx,
                                       endpoint.instance_id,
                                       endpoint.endpoint_id);
    }
    std::memset(&msg->o, 0, sizeof(msg->o));
    msg->o.ret_val = EOK;
    msg->o.nbytes = 0U;
    return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o));
  }

  if (command == static_cast<std::uint32_t>(CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK) ||
      command == static_cast<std::uint32_t>(CAN_DEVCTL_RX_FRAME_RAW_BLOCK)) {
    if (endpoint.kind != EndpointKind::rx) {
      return EINVAL;
    }
    if (ctp->info.dstmsglen <
        static_cast<std::uint32_t>(sizeof(msg->o) + sizeof(struct can_msg))) {
      return EMSGSIZE;
    }
    const auto blocking =
        command == static_cast<std::uint32_t>(CAN_DEVCTL_RX_FRAME_RAW_BLOCK);
    auto frame = McanFrameTransfer{};
    if (endpoint_uses_hardware_interrupts(endpoint)) {
      auto queued = false;
      const auto queue_status = hardware_receive_or_queue_raw_rx_devctl(
          endpoint, ctp->rcvid, blocking, frame, queued);
      if (queue_status != EOK) {
        return queue_status;
      }
      if (queued) {
        return _RESMGR_NOREPLY;
      }
    } else {
      auto operation_status = endpoint_receive_frame(endpoint, frame);
      if (!status_ok(operation_status)) {
        return resource_manager_status_to_errno(operation_status);
      }
    }
    auto *const native = static_cast<struct can_msg *>(_IO_OUTPUT_PAYLOAD(msg));
    if (native == nullptr) {
      return EBADMSG;
    }
    const auto native_status = native_can_message_from_frame(frame, *native);
    if (native_status != EOK) {
      return native_status;
    }
    endpoint.attr.flags |= IOFUNC_ATTR_ATIME;
    trigger_endpoint_notify(endpoint);
    std::memset(&msg->o, 0, sizeof(msg->o));
    msg->o.ret_val = EOK;
    msg->o.nbytes = sizeof(struct can_msg);
    return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + sizeof(struct can_msg));
  }

  return ENOSYS;
}

int mcan_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_read_verify(ctp, msg, static_cast<iofunc_ocb_t *>(ocb), nullptr);
  if (status != EOK) {
    return status;
  }
  if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
    return ENOSYS;
  }
  auto *const endpoint = endpoint_from_ocb(ocb);
  if (endpoint == nullptr || endpoint->runtime == nullptr) {
    return EIO;
  }
  if (endpoint->qnx_can_compat) {
    return ENOTSUP;
  }
  if (endpoint->kind != EndpointKind::rx) {
    return EBADF;
  }
  if (_IO_READ_GET_NBYTES(msg) < sizeof(McanFrameTransfer)) {
    return EMSGSIZE;
  }
  auto frame = McanFrameTransfer{};
  if (endpoint_uses_hardware_interrupts(*endpoint)) {
    auto queued = false;
    const auto receive_status = hardware_receive_or_queue_read(
        *endpoint, ctp->rcvid, !ocb_is_nonblocking(ocb), frame, queued);
    if (receive_status != EOK) {
      return receive_status;
    }
    if (queued) {
      return _RESMGR_NOREPLY;
    }
  } else {
    const auto receive_status = endpoint_receive_frame(*endpoint, frame);
    if (!status_ok(receive_status)) {
      return resource_manager_status_to_errno(receive_status);
    }
  }
  endpoint->read_reply = frame;
  SETIOV(ctp->iov, &endpoint->read_reply, sizeof(endpoint->read_reply));
  _IO_SET_READ_NBYTES(ctp, sizeof(endpoint->read_reply));
  if (msg->i.nbytes > 0) {
    endpoint->attr.flags |= IOFUNC_ATTR_ATIME;
  }
  trigger_endpoint_notify(*endpoint);
  return _RESMGR_NPARTS(1);
}

int mcan_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_write_verify(ctp, msg, static_cast<iofunc_ocb_t *>(ocb), nullptr);
  if (status != EOK) {
    return status;
  }
  if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE) {
    return ENOSYS;
  }
  auto *const endpoint = endpoint_from_ocb(ocb);
  if (endpoint == nullptr || endpoint->runtime == nullptr) {
    return EIO;
  }
  if (endpoint->qnx_can_compat) {
    return ENOTSUP;
  }
  if (endpoint->kind != EndpointKind::tx) {
    return EBADF;
  }
  if (_IO_WRITE_GET_NBYTES(msg) != sizeof(McanFrameTransfer)) {
    return EMSGSIZE;
  }
  auto frame = McanFrameTransfer{};
  const auto read_count =
      resmgr_msgread(ctp, &frame, sizeof(frame), sizeof(msg->i));
  if (read_count == -1) {
    return errno;
  }
  if (read_count != static_cast<ssize_t>(sizeof(frame))) {
    return EBADMSG;
  }
  const auto transmit_status = endpoint_transmit_frame(*endpoint, frame);
  if (!status_ok(transmit_status)) {
    return resource_manager_status_to_errno(transmit_status);
  }
  _IO_SET_WRITE_NBYTES(ctp, sizeof(frame));
  endpoint->attr.flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
  if (!endpoint_uses_hardware_interrupts(*endpoint)) {
    trigger_endpoint_notify(*endpoint);
  }
  if (endpoint->owner != nullptr &&
      !endpoint_uses_hardware_interrupts(*endpoint)) {
    trigger_matching_endpoint_notify(*endpoint->owner, EndpointKind::rx,
                                     endpoint->instance_id,
                                     endpoint->endpoint_id);
  }
  return _RESMGR_NPARTS(0);
}

int mcan_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_devctl_default(ctp, msg, static_cast<iofunc_ocb_t *>(ocb));
  if (status != _RESMGR_DEFAULT) {
    return status;
  }
  auto *const endpoint = endpoint_from_ocb(ocb);
  if (endpoint == nullptr || endpoint->runtime == nullptr) {
    return EIO;
  }
  if (endpoint->qnx_can_compat) {
    return qnx_can_compat_devctl(ctp, msg, *endpoint);
  }
  if (endpoint->kind != EndpointKind::ctl) {
    return EBADF;
  }
  if (ctp->rcvid == 0 || ctp->size < sizeof(msg->i)) {
    return EBADMSG;
  }

  auto reply_bytes = std::size_t{0U};
  auto operation_status = McanStatus::ok;
  const auto command = static_cast<std::uint32_t>(msg->i.dcmd);
  if (command == kMcanDevctlQueryCapabilities) {
    if (ctp->info.dstmsglen <
        static_cast<std::uint32_t>(sizeof(msg->o) +
                                   sizeof(McanControllerCapabilities))) {
      return EMSGSIZE;
    }
    auto *const capabilities =
        static_cast<McanControllerCapabilities *>(_IO_OUTPUT_PAYLOAD(msg));
    operation_status = endpoint_query_capabilities(*endpoint, *capabilities);
    reply_bytes = sizeof(*capabilities);
  } else if (command == kMcanDevctlQueryDiagnostics) {
    if (ctp->info.dstmsglen <
        static_cast<std::uint32_t>(sizeof(msg->o) +
                                   sizeof(McanDiagnosticsSnapshot))) {
      return EMSGSIZE;
    }
    auto *const diagnostics =
        static_cast<McanDiagnosticsSnapshot *>(_IO_OUTPUT_PAYLOAD(msg));
    operation_status = endpoint_query_diagnostics(*endpoint, *diagnostics);
    reply_bytes = sizeof(*diagnostics);
  } else {
    return ENOSYS;
  }
  if (!status_ok(operation_status)) {
    return resource_manager_status_to_errno(operation_status);
  }
  std::memset(&msg->o, 0, sizeof(msg->o));
  msg->o.ret_val = EOK;
  msg->o.nbytes = static_cast<std::uint32_t>(reply_bytes);
  return _RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + reply_bytes);
}

int mcan_notify(resmgr_context_t *ctp, io_notify_t *msg, RESMGR_OCB_T *ocb) {
  auto *const endpoint = endpoint_from_ocb(ocb);
  if (endpoint == nullptr || endpoint->runtime == nullptr) {
    return EIO;
  }
  if (endpoint->kind == EndpointKind::ctl) {
    return ENOTSUP;
  }

  auto trigger = 0;
  if (endpoint->kind == EndpointKind::rx) {
    auto ready_count = std::size_t{0U};
    const auto status = endpoint_rx_ready_count(*endpoint, ready_count);
    if (!status_ok(status)) {
      return resource_manager_status_to_errno(status);
    }
    if (ready_count > 0U) {
      trigger |= _NOTIFY_COND_INPUT;
    }
  } else if (endpoint->kind == EndpointKind::tx) {
    auto room_count = std::size_t{0U};
    const auto status = endpoint_tx_room_count(*endpoint, room_count);
    if (!status_ok(status)) {
      return resource_manager_status_to_errno(status);
    }
    if (room_count > 0U) {
      trigger |= _NOTIFY_COND_OUTPUT;
    }
  }
  const auto lock_status = lock_endpoint_notify(*endpoint);
  if (!status_ok(lock_status)) {
    return resource_manager_status_to_errno(lock_status);
  }
  const auto result =
      iofunc_notify(ctp, msg, endpoint->notify, trigger, nullptr, nullptr);
  unlock_endpoint_notify(*endpoint);
  return result;
}

int mcan_close_dup(resmgr_context_t *ctp, io_close_t *msg, RESMGR_OCB_T *ocb) {
  auto *const endpoint = endpoint_from_ocb(ocb);
  if (endpoint != nullptr) {
    cancel_endpoint_waiters(*endpoint, EINTR);
    if (status_ok(lock_endpoint_notify(*endpoint))) {
      iofunc_notify_remove_strict(ctp, endpoint->notify, kNotifySlotCount);
      unlock_endpoint_notify(*endpoint);
    }
  }
  return iofunc_close_dup_default(ctp, msg, static_cast<iofunc_ocb_t *>(ocb));
}

int mcan_unblock(resmgr_context_t *ctp, io_pulse_t *msg, RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_unblock_default(ctp, msg, static_cast<iofunc_ocb_t *>(ocb));
  if (status != _RESMGR_DEFAULT) {
    return status;
  }
  auto *const endpoint = endpoint_from_ocb(ocb);
  if (endpoint == nullptr || msg == nullptr) {
    return _RESMGR_DEFAULT;
  }
  const auto rcvid = static_cast<rcvid_t>(msg->value.sival_int);
  return cancel_endpoint_waiter_by_rcvid(*endpoint, rcvid, EINTR)
             ? _RESMGR_NOREPLY
             : _RESMGR_DEFAULT;
}

[[nodiscard]] int
attach_configured_endpoints(skeleton_state &state,
                            const McanResourceManagerConfig &config) noexcept;

[[nodiscard]] int add_endpoint(skeleton_state &state,
                               const std::uint8_t instance_id,
                               const EndpointKind direction,
                               const std::uint8_t endpoint_id,
                               const bool qnx_can_compat) noexcept {
  if (state.endpoint_count >= state.endpoints.size()) {
    return EOVERFLOW;
  }
  auto &endpoint = state.endpoints[state.endpoint_count];
  const auto path_result =
      qnx_can_compat ? make_qnx_can_compat_endpoint_path(
                           direction, instance_id, endpoint_id, endpoint.path)
                     : make_endpoint_path(direction, instance_id, endpoint_id,
                                          endpoint.path);
  if (!status_ok(path_result)) {
    return EINVAL;
  }
  if (qnx_can_compat) {
    if (::access(endpoint.path.value.data(), F_OK) == 0) {
      return EEXIST;
    }
    if (errno != ENOENT) {
      return errno;
    }
  }
  iofunc_attr_init(&endpoint.attr, S_IFCHR | 0600, nullptr, nullptr);
  IOFUNC_NOTIFY_INIT(endpoint.notify);
  const auto notify_lock_status = initialize_endpoint_notify_lock(endpoint);
  if (!status_ok(notify_lock_status)) {
    return resource_manager_status_to_errno(notify_lock_status);
  }
  endpoint.owner = &state;
  endpoint.runtime = &state.runtime;
  endpoint.hardware = state.hardware_loopback_enabled &&
                              state.hardware_loopback.instance_id == instance_id
                          ? &state.hardware_loopback
                          : nullptr;
  endpoint.kind = direction;
  endpoint.instance_id = instance_id;
  endpoint.endpoint_id = endpoint_id;
  endpoint.qnx_can_compat = qnx_can_compat;
  endpoint.read_reply = McanFrameTransfer{};
  endpoint.attach_id = resmgr_attach(
      state.dispatch, &state.resmgr_attr, endpoint.path.value.data(),
      _FTYPE_ANY, 0, &state.connect_funcs, &state.io_funcs, &endpoint.attr);
  if (endpoint.attach_id == -1) {
    destroy_endpoint_notify_lock(endpoint);
    return errno;
  }
  ++state.endpoint_count;
  return EOK;
}

[[nodiscard]] int initialize_skeleton_state(
    skeleton_state &state, const McanResourceManagerConfig &config,
    const McanResourceManagerRuntimeOptions &runtime_options,
    const bool hardware_loopback_enabled, const bool qnx_can_compat_enabled,
    const bool qnx_can_raw_mode_enabled,
    const bool qnx_can_listen_only) noexcept {
  state = skeleton_state{};
  state.dispatch = dispatch_create();
  if (state.dispatch == nullptr) {
    return ENOMEM;
  }
  state.qnx_can_compat_enabled = qnx_can_compat_enabled;
  state.qnx_can_raw_mode_enabled = qnx_can_raw_mode_enabled;
  state.qnx_can_listen_only = qnx_can_listen_only;

  std::memset(&state.resmgr_attr, 0, sizeof(state.resmgr_attr));
  state.resmgr_attr.nparts_max = 1;
  state.resmgr_attr.msg_max_size =
      max_size(kResourceManagerMsgMaxSize, kResourceManagerDevctlMsgMaxSize);

  iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &state.connect_funcs,
                   _RESMGR_IO_NFUNCS, &state.io_funcs);
  state.io_funcs.read = mcan_read;
  state.io_funcs.write = mcan_write;
  state.io_funcs.devctl = mcan_devctl;
  state.io_funcs.notify = mcan_notify;
  state.io_funcs.close_dup = mcan_close_dup;
  state.io_funcs.unblock = mcan_unblock;

  auto status = initialize_resource_manager_runtime(state.runtime, config,
                                                    runtime_options);
  if (!status_ok(status)) {
    return resource_manager_status_to_errno(status);
  }
  if (hardware_loopback_enabled) {
    if (config.controller_count != 1U) {
      return ENOTSUP;
    }
    const auto hardware_internal_loopback =
        runtime_options.interrupts_enabled ? runtime_options.internal_loopback
                                           : true;
    status = initialize_hardware_loopback_state(
        state.hardware_loopback, config.controllers[0],
        hardware_internal_loopback, runtime_options.interrupts_enabled, &state);
    if (!status_ok(status)) {
      return resource_manager_status_to_errno(status);
    }
    state.hardware_loopback_enabled = true;
  }
  const auto attach_status = attach_configured_endpoints(state, config);
  if (attach_status != EOK) {
    if (state.hardware_loopback_enabled) {
      (void)shutdown_hardware_loopback_state(state.hardware_loopback);
      state.hardware_loopback_enabled = false;
    }
    destroy_endpoint_notify_locks(state);
    return attach_status;
  }
  return EOK;
}

[[nodiscard]] int
qnx_can_compat_namespace_available(const std::uint8_t instance_id) noexcept {
  auto namespace_path = McanEndpointPath{};
  const auto status =
      make_qnx_can_compat_namespace_path(instance_id, namespace_path);
  if (!status_ok(status)) {
    return resource_manager_status_to_errno(status);
  }
  if (::access(namespace_path.value.data(), F_OK) == 0) {
    return EEXIST;
  }
  const auto access_errno = errno;
  if (access_errno != ENOENT) {
    return access_errno;
  }
  return EOK;
}

[[nodiscard]] int
attach_configured_endpoints(skeleton_state &state,
                            const McanResourceManagerConfig &config) noexcept {
  const auto result = validate_resource_manager_skeleton(config);
  if (!status_ok(result)) {
    return EINVAL;
  }
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    const auto &instance = config.controllers[index];
    if (state.qnx_can_compat_enabled) {
      const auto namespace_status =
          qnx_can_compat_namespace_available(instance.instance_id);
      if (namespace_status != EOK) {
        return namespace_status;
      }
    }
    for (auto endpoint = std::uint8_t{0U}; endpoint < instance.rx_queue_count;
         ++endpoint) {
      if (state.qnx_can_compat_enabled) {
        const auto compat_status = add_endpoint(
            state, instance.instance_id, EndpointKind::rx, endpoint, true);
        if (compat_status != EOK) {
          return compat_status;
        }
      }
      const auto status = add_endpoint(state, instance.instance_id,
                                       EndpointKind::rx, endpoint, false);
      if (status != EOK) {
        return status;
      }
    }
    for (auto endpoint = std::uint8_t{0U}; endpoint < instance.tx_queue_count;
         ++endpoint) {
      if (state.qnx_can_compat_enabled) {
        const auto compat_status = add_endpoint(
            state, instance.instance_id, EndpointKind::tx, endpoint, true);
        if (compat_status != EOK) {
          return compat_status;
        }
      }
      const auto status = add_endpoint(state, instance.instance_id,
                                       EndpointKind::tx, endpoint, false);
      if (status != EOK) {
        return status;
      }
    }
    const auto status =
        add_endpoint(state, instance.instance_id, EndpointKind::ctl, 0U, false);
    if (status != EOK) {
      return status;
    }
  }
  return EOK;
}

[[nodiscard]] McanResourceManagerConfig skeleton_default_config() noexcept {
  auto config = McanResourceManagerConfig{};
  config.controller_count = 1U;
  config.controllers[0].instance_id = 0U;
  config.controllers[0].mode = McanMode::classic;
  config.controllers[0].arbitration_bitrate_valid = true;
  config.controllers[0].arbitration_bitrate = 500'000U;
  config.controllers[0].rx_queue_capacity = 16U;
  config.controllers[0].tx_queue_capacity = 16U;
  config.controllers[0].rx_queue_count = 1U;
  config.controllers[0].tx_queue_count = 1U;
  (void)apply_j784_mcan_board_mapping(config.controllers[0],
                                      J784McanPhysicalInstance::mcu_mcan0);
  return config;
}

[[nodiscard]] McanResourceManagerConfig single_controller_config_for_endpoint(
    const std::uint8_t endpoint_instance_id) noexcept {
  auto config = skeleton_default_config();
  const auto *const instance =
      find_j784_mcan_board_instance_by_endpoint(endpoint_instance_id);
  if (instance == nullptr) {
    config.controller_count = 0U;
    return config;
  }
  config.controllers[0].instance_id = instance->default_endpoint_instance_id;
  config.controllers[0].hardware = instance->hardware;
  config.controllers[0].board_mapping_evidence_confirmed = true;
  return config;
}

[[nodiscard]] McanResourceManagerConfig
single_controller_irq_config_for_endpoint(
    const std::uint8_t endpoint_instance_id,
    const std::uint32_t logical_irq) noexcept {
  auto config = single_controller_config_for_endpoint(endpoint_instance_id);
  if (config.controller_count == 0U) {
    return config;
  }
  config.controllers[0].hardware.qnx_logical_irq = logical_irq;
  config.controllers[0].qnx_irq_routing_evidence_confirmed = logical_irq != 0U;
  return config;
}

[[nodiscard]] bool
external_endpoint_irq_routing_confirmed(const std::uint8_t endpoint_instance_id,
                                        const std::uint32_t logical_irq) noexcept {
  return endpoint_instance_id == 1U && logical_irq == 867U;
}

[[nodiscard]] McanResourceManagerConfig
single_controller_external_irq_config_for_endpoint(
    const std::uint8_t endpoint_instance_id,
    const std::uint32_t logical_irq) noexcept {
  auto config = single_controller_config_for_endpoint(endpoint_instance_id);
  if (config.controller_count == 0U) {
    return config;
  }
  config.controllers[0].hardware.qnx_logical_irq = logical_irq;
  config.controllers[0].qnx_irq_routing_evidence_confirmed =
      external_endpoint_irq_routing_confirmed(endpoint_instance_id, logical_irq);
  return config;
}

template <std::size_t RegisterCount>
[[nodiscard]] int probe_read_only_registers(
    const char *const label, const McanHardwareRegion &region,
    const std::array<probe_register, RegisterCount> &registers) noexcept {
  auto mapped = McanMappedMmioRegion{};
  std::printf("  %s base=0x%llx size=0x%x\n", label,
              static_cast<unsigned long long>(region.base_address),
              static_cast<unsigned int>(region.size_bytes));
  auto status = map_qnx_mmio_region_read_only(region, mapped);
  if (!status_ok(status)) {
    std::printf("  %s map_status=%s\n", label, status_name(status));
    return EXIT_FAILURE;
  }
  auto result = EXIT_SUCCESS;
  for (const auto &reg : registers) {
    auto value = std::uint32_t{0U};
    status = read_qnx_mmio32(mapped, reg.offset, value);
    if (!status_ok(status)) {
      std::printf("    %s read_status=%s\n", reg.name, status_name(status));
      result = EXIT_FAILURE;
      break;
    }
    std::printf("    %s=0x%08x\n", reg.name, static_cast<unsigned int>(value));
  }
  status = unmap_qnx_mmio_region(mapped);
  if (!status_ok(status)) {
    std::printf("  %s unmap_status=%s\n", label, status_name(status));
    result = EXIT_FAILURE;
  }
  return result;
}

[[nodiscard]] int
probe_message_ram_mapping(const McanHardwareRegion &region) noexcept {
  auto mapped = McanMappedMmioRegion{};
  std::printf("  message_ram base=0x%llx size=0x%x\n",
              static_cast<unsigned long long>(region.base_address),
              static_cast<unsigned int>(region.size_bytes));
  auto status = map_qnx_mmio_region_read_only(region, mapped);
  if (!status_ok(status)) {
    std::printf("  message_ram map_status=%s\n", status_name(status));
    return EXIT_FAILURE;
  }
  std::printf("  message_ram map_status=ok read=skipped\n");
  status = unmap_qnx_mmio_region(mapped);
  if (!status_ok(status)) {
    std::printf("  message_ram unmap_status=%s\n", status_name(status));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

[[nodiscard]] const char *
evidence_text_or_not_observed(const char *const value) noexcept {
  if (value == nullptr) {
    return "not-observed";
  }
  return value;
}

[[nodiscard]] int run_read_only_hardware_probe_for_instance(
    const J784McanBoardInstance &instance) noexcept {
  auto controller = McanControllerConfig{};
  controller.instance_id = instance.default_endpoint_instance_id;
  controller.arbitration_bitrate_valid = true;
  controller.arbitration_bitrate = 500'000U;
  controller.hardware = instance.hardware;
  const auto status = validate_controller_hardware_mapping(controller);
  std::printf("instance=%s endpoint=%u tisci_device_id=%u mapping_status=%s\n",
              instance.name,
              static_cast<unsigned int>(instance.default_endpoint_instance_id),
              static_cast<unsigned int>(instance.hardware.tisci_device_id),
              status_name(status));
  std::printf(
      "  ti_reference_gic_spi_line0=%u ti_reference_gic_spi_line1=%u "
      "mcan_clock_select_register_offset=0x%x\n",
      static_cast<unsigned int>(instance.ti_reference_evidence.gic_spi_line0),
      static_cast<unsigned int>(instance.ti_reference_evidence.gic_spi_line1),
      static_cast<unsigned int>(
          instance.ti_reference_evidence.mcan_clock_select_register_offset));
  std::printf("  ti_boot_device_tree_status=%s "
              "ti_boot_device_tree_label=%s\n",
              instance.ti_reference_evidence.ti_boot_device_tree_status_is_okay
                  ? "okay"
                  : "not-observed",
              evidence_text_or_not_observed(
                  instance.ti_reference_evidence.ti_boot_device_tree_label));
  std::printf(
      "  pinmux=%s transceiver=%s standby=%s\n",
      evidence_text_or_not_observed(
          instance.ti_reference_evidence.ti_boot_pinmux_state_name),
      evidence_text_or_not_observed(
          instance.ti_reference_evidence.ti_boot_transceiver_node_name),
      evidence_text_or_not_observed(
          instance.ti_reference_evidence.ti_boot_standby_control_description));
  std::printf(
      "  linux_reference_device_tree_node=%s status=%s "
      "linux_reference_can_interface=%s board_connector=%s "
      "ethernet_firmware_conflict=%s\n",
      evidence_text_or_not_observed(
          instance.board_port_evidence.linux_reference_device_tree_node_name),
      evidence_text_or_not_observed(
          instance.board_port_evidence.linux_reference_device_tree_status),
      evidence_text_or_not_observed(
          instance.board_port_evidence.linux_reference_can_interface_name),
      evidence_text_or_not_observed(
          instance.board_port_evidence.board_connector_name),
      instance.board_port_evidence.ethernet_firmware_overlay_disables_path
          ? "requires-explicit-enable"
          : "clear");
  if (!status_ok(status)) {
    return EXIT_FAILURE;
  }
  auto result = probe_read_only_registers(
      "subsystem", instance.hardware.subsystem, subsystem_probe_registers);
  if (probe_read_only_registers("controller", instance.hardware.controller,
                                controller_probe_registers) != EXIT_SUCCESS) {
    result = EXIT_FAILURE;
  }
  if (probe_message_ram_mapping(instance.hardware.message_ram) !=
      EXIT_SUCCESS) {
    result = EXIT_FAILURE;
  }
  return result;
}

[[nodiscard]] int
run_read_only_hardware_probe(const McanResourceManagerConfig &config,
                             const bool probe_all) noexcept {
  const auto privilege_status = request_qnx_io_privileges();
  if (!status_ok(privilege_status)) {
    std::printf("qnx_io_privilege_status=%s\n", status_name(privilege_status));
    return EXIT_FAILURE;
  }
  std::printf("qnx_io_privilege_status=ok\n");
  if (probe_all) {
    auto result = EXIT_SUCCESS;
    for (auto index = std::size_t{0U}; index < j784_mcan_board_instance_count();
         ++index) {
      const auto *const instance = j784_mcan_board_instance_at(index);
      if (instance == nullptr || run_read_only_hardware_probe_for_instance(
                                     *instance) != EXIT_SUCCESS) {
        result = EXIT_FAILURE;
      }
    }
    return result;
  }
  const auto skeleton_status = validate_resource_manager_skeleton(config);
  if (!status_ok(skeleton_status)) {
    std::printf("config_status=%s\n", status_name(skeleton_status));
    return EXIT_FAILURE;
  }
  const auto *const instance = find_j784_mcan_board_instance_by_endpoint(
      config.controllers[0].instance_id);
  if (instance == nullptr) {
    std::printf("default_instance_status=invalid_hardware_mapping\n");
    return EXIT_FAILURE;
  }
  return run_read_only_hardware_probe_for_instance(*instance);
}

struct tca6424_read_one_packet {
  i2c_sendrecv_t header{};
  std::uint8_t buffer[1]{};
};

struct tca6424_write_one_packet {
  i2c_send_t header{};
  std::uint8_t buffer[2]{};
};

static_assert(offsetof(tca6424_read_one_packet, buffer) ==
                  sizeof(i2c_sendrecv_t),
              "QNX I2C sendrecv buffer must directly follow the header");
static_assert(offsetof(tca6424_write_one_packet, buffer) == sizeof(i2c_send_t),
              "QNX I2C send buffer must directly follow the header");

[[nodiscard]] McanStatus tca6424_read_register(const int fd,
                                               const std::uint8_t i2c_address,
                                               const std::uint8_t reg,
                                               std::uint8_t &value) noexcept {
  auto packet = tca6424_read_one_packet{};
  packet.header.slave.addr = i2c_address;
  packet.header.slave.fmt = I2C_ADDRFMT_7BIT;
  packet.header.send_len = 1U;
  packet.header.recv_len = 1U;
  packet.header.stop = 1U;
  packet.buffer[0] = reg;

  const auto rc =
      ::devctl(fd, DCMD_I2C_SENDRECV, &packet,
               sizeof(packet.header) + sizeof(packet.buffer), nullptr);
  if (rc != EOK) {
    return McanStatus::hardware_access_unavailable;
  }
  value = packet.buffer[0];
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
tca6424_write_register(const int fd, const std::uint8_t i2c_address,
                       const std::uint8_t reg,
                       const std::uint8_t value) noexcept {
  auto packet = tca6424_write_one_packet{};
  packet.header.slave.addr = i2c_address;
  packet.header.slave.fmt = I2C_ADDRFMT_7BIT;
  packet.header.len = 2U;
  packet.header.stop = 1U;
  packet.buffer[0] = reg;
  packet.buffer[1] = value;

  const auto rc =
      ::devctl(fd, DCMD_I2C_SEND, &packet,
               sizeof(packet.header) + sizeof(packet.buffer), nullptr);
  return rc == EOK ? McanStatus::ok : McanStatus::hardware_access_unavailable;
}

[[nodiscard]] McanStatus apply_wkup_gpio_transceiver_action(
    const J784TransceiverAction &action) noexcept {
  const auto validation_status = validate_j784_transceiver_action(action);
  if (!status_ok(validation_status) ||
      action.backend != J784TransceiverBackend::wkup_gpio_mmio) {
    return McanStatus::invalid_argument;
  }

  auto mapped = McanMappedMmioRegion{};
  auto status = map_qnx_mmio_region(action.wkup_gpio.region, mapped);
  if (!status_ok(status)) {
    return status;
  }

  const auto mask = std::uint32_t{1U << action.wkup_gpio.bit};
  status = write_qnx_mmio32(mapped,
                            action.wkup_gpio.output_high
                                ? action.wkup_gpio.set_offset
                                : action.wkup_gpio.clear_offset,
                            mask);
  if (status_ok(status)) {
    auto direction = std::uint32_t{0U};
    status =
        read_qnx_mmio32(mapped, action.wkup_gpio.direction_offset, direction);
    if (status_ok(status)) {
      direction &= ~mask;
      status = write_qnx_mmio32(mapped, action.wkup_gpio.direction_offset,
                                direction);
    }
  }

  const auto unmap_status = unmap_qnx_mmio_region(mapped);
  if (!status_ok(status)) {
    return status;
  }
  return unmap_status;
}

[[nodiscard]] McanStatus
apply_tca6424_transceiver_action(const int fd,
                                 const J784TransceiverAction &action) noexcept {
  const auto validation_status = validate_j784_transceiver_action(action);
  if (!status_ok(validation_status) ||
      action.backend != J784TransceiverBackend::tca6424_i2c) {
    return McanStatus::invalid_argument;
  }

  auto value = std::uint8_t{0U};
  auto status = tca6424_read_register(fd, action.tca6424.i2c_address,
                                      action.tca6424.output_register, value);
  if (!status_ok(status)) {
    return status;
  }
  const auto mask = static_cast<std::uint8_t>(1U << action.tca6424.pin);
  if (action.tca6424.output_high) {
    value = static_cast<std::uint8_t>(value | mask);
  } else {
    value = static_cast<std::uint8_t>(value & ~mask);
  }
  status = tca6424_write_register(fd, action.tca6424.i2c_address,
                                  action.tca6424.output_register, value);
  if (!status_ok(status)) {
    return status;
  }

  status = tca6424_read_register(fd, action.tca6424.i2c_address,
                                 action.tca6424.config_register, value);
  if (!status_ok(status)) {
    return status;
  }
  value = static_cast<std::uint8_t>(value & ~mask);
  return tca6424_write_register(fd, action.tca6424.i2c_address,
                                action.tca6424.config_register, value);
}

[[nodiscard]] McanStatus apply_transceiver_action(
    const J784TransceiverAction &action,
    const J784TransceiverRuntimeOptions &options) noexcept {
  switch (action.backend) {
  case J784TransceiverBackend::none:
    return McanStatus::ok;
  case J784TransceiverBackend::wkup_gpio_mmio:
    return apply_wkup_gpio_transceiver_action(action);
  case J784TransceiverBackend::tca6424_i2c: {
    if (!options.allow_main_mcan4_ethfw_conflict_override) {
      return McanStatus::missing_hardware_evidence;
    }
    if (options.main_mcan4_tca6424_i2c_device_path == nullptr ||
        options.main_mcan4_tca6424_i2c_device_path[0] == '\0') {
      return McanStatus::hardware_access_unavailable;
    }
    const auto fd = ::open(options.main_mcan4_tca6424_i2c_device_path, O_RDWR);
    if (fd == -1) {
      return McanStatus::hardware_access_unavailable;
    }
    const auto status = apply_tca6424_transceiver_action(fd, action);
    const auto close_rc = ::close(fd);
    if (!status_ok(status)) {
      return status;
    }
    return close_rc == 0 ? McanStatus::ok
                         : McanStatus::hardware_access_unavailable;
  }
  }
  return McanStatus::invalid_argument;
}

[[nodiscard]] int run_transceiver_control(
    const bool apply_writes,
    const bool allow_main_mcan4_ethfw_conflict_override,
    const char *const main_mcan4_tca6424_i2c_device_path) noexcept {
  auto result = EXIT_SUCCESS;
  auto options = J784TransceiverRuntimeOptions{
      apply_writes, allow_main_mcan4_ethfw_conflict_override,
      main_mcan4_tca6424_i2c_device_path};

  if (apply_writes) {
    const auto privilege_status = request_qnx_io_privileges();
    std::printf("qnx_io_privilege_status=%s\n", status_name(privilege_status));
    if (!status_ok(privilege_status)) {
      return EXIT_FAILURE;
    }
  }

  const auto mode = apply_writes ? "apply" : "probe";
  std::printf("transceiver_control board=%s mode=%s "
              "main_mcan4_tca6424_i2c_device_path=%s\n",
              kJ784EvmBoardRevision, mode,
              main_mcan4_tca6424_i2c_device_path == nullptr
                  ? "not-configured"
                  : main_mcan4_tca6424_i2c_device_path);

  for (auto index = std::size_t{0U}; index < j784_mcan_board_instance_count();
       ++index) {
    const auto *const instance = j784_mcan_board_instance_at(index);
    if (instance == nullptr) {
      result = EXIT_FAILURE;
      continue;
    }

    auto plan = J784TransceiverActionPlan{};
    auto status =
        make_j784_transceiver_action_plan(instance->physical_instance, plan);
    if (!status_ok(status)) {
      std::printf("instance=%s plan_status=%s\n", instance->name,
                  status_name(status));
      result = EXIT_FAILURE;
      continue;
    }

    std::printf("instance=%s board_connector=%s "
                "linux_reference_device_tree_node=%s "
                "has_actions_requiring_explicit_enable=%s\n",
                plan.instance_name, plan.board_connector_name,
                plan.linux_reference_device_tree_node_name,
                plan.has_actions_requiring_explicit_enable ? "yes" : "no");
    if (plan.ethernet_firmware_conflict_note != nullptr) {
      std::printf("  explicit_enable_note=%s\n",
                  plan.ethernet_firmware_conflict_note);
    }

    status = validate_j784_transceiver_runtime_options(plan, options);
    if (apply_writes && plan.has_actions_requiring_explicit_enable &&
        !allow_main_mcan4_ethfw_conflict_override) {
      std::printf("  apply_status=%s action=skipped\n", status_name(status));
      continue;
    }
    if (!status_ok(status)) {
      std::printf("  apply_status=%s\n", status_name(status));
      result = EXIT_FAILURE;
      continue;
    }

    for (auto action_index = std::size_t{0U}; action_index < plan.action_count;
         ++action_index) {
      const auto &action = plan.actions[action_index];
      char action_text[512]{};
      const auto format_rc = format_j784_transceiver_action(
          action, action_text, sizeof(action_text));
      if (format_rc < 0) {
        std::printf("  action=%u format_status=invalid_argument\n",
                    static_cast<unsigned int>(action_index));
        result = EXIT_FAILURE;
        continue;
      }
      std::printf("  action=%u %s write=%s\n",
                  static_cast<unsigned int>(action_index), action_text,
                  apply_writes && action.writes_hardware ? "yes" : "no");

      if (apply_writes && action.writes_hardware) {
        status = apply_transceiver_action(action, options);
        std::printf("    write_status=%s\n", status_name(status));
        if (!status_ok(status)) {
          result = EXIT_FAILURE;
        }
      }
    }
  }
  return result;
}

[[nodiscard]] McanControllerCapabilities
make_hardware_capabilities(const McanControllerConfig &controller,
                           const bool internal_loopback) noexcept {
  auto capabilities = McanControllerCapabilities{};
  capabilities.flags = capability_value(McanCapabilityFlag::classic_frame) |
                       capability_value(McanCapabilityFlag::extended_id);
  if (controller.mode == McanMode::fd) {
    capabilities.flags |= capability_value(McanCapabilityFlag::fd_frame) |
                          capability_value(McanCapabilityFlag::esi);
  }
  if (controller.brs_enabled) {
    capabilities.flags |= capability_value(McanCapabilityFlag::brs);
  }
  if (internal_loopback) {
    capabilities.flags |=
        capability_value(McanCapabilityFlag::internal_loopback);
  }
  capabilities.max_rx_queues = controller.rx_queue_count;
  capabilities.max_tx_queues = controller.tx_queue_count;
  capabilities.max_rx_queue_capacity = controller.rx_queue_capacity;
  capabilities.max_tx_queue_capacity = controller.tx_queue_capacity;
  return capabilities;
}

[[nodiscard]] McanControllerCapabilities
make_loopback_capabilities(const McanControllerConfig &controller) noexcept {
  return make_hardware_capabilities(controller, true);
}

[[nodiscard]] McanFrameTransfer make_loopback_frame() noexcept {
  auto frame = McanFrameTransfer{};
  frame.can_id = 0x123U;
  frame.dlc = 8U;
  frame.length = 8U;
  for (auto index = std::size_t{0U}; index < frame.length; ++index) {
    frame.payload[index] = static_cast<std::uint8_t>(0x30U + index);
  }
  return frame;
}

[[nodiscard]] McanMessageRamLayout
make_internal_loopback_smoke_message_ram_layout() noexcept {
  auto layout = McanMessageRamLayout{};
  layout.standard_filters = McanMessageRamSection{0U, 1U, 1U};
  layout.extended_filters = McanMessageRamSection{12U, 1U, 2U};
  layout.tx_event_fifo = McanMessageRamSection{25U, 5U, 2U};
  layout.tx_buffers = McanMessageRamSection{37U, 5U, 18U};
  layout.rx_fifo0 = McanMessageRamSection{137U, 5U, 18U};
  layout.rx_fifo1 = McanMessageRamSection{0U, 0U, 0U};
  layout.rx_buffers = McanMessageRamSection{237U, 0U, 0U};
  return layout;
}

[[nodiscard]] bool frames_match(const McanFrameTransfer &expected,
                                const McanFrameTransfer &actual) noexcept {
  if (expected.can_id != actual.can_id || expected.flags != actual.flags ||
      expected.dlc != actual.dlc || expected.length != actual.length) {
    return false;
  }
  for (auto index = std::size_t{0U}; index < expected.length; ++index) {
    if (expected.payload[index] != actual.payload[index]) {
      return false;
    }
  }
  return true;
}

[[nodiscard]] std::uint32_t
message_ram_byte_offset(const std::uint32_t word_offset) noexcept {
  return word_offset * kMcanMessageRamWordBytes;
}

[[nodiscard]] McanStatus
write_controller_register(const McanMappedMmioRegion &controller,
                          const McanCfgRegisterOffset offset,
                          const std::uint32_t value) noexcept {
  return write_qnx_mmio32(controller, offset_value(offset), value);
}

[[nodiscard]] McanStatus
read_controller_register(const McanMappedMmioRegion &controller,
                         const McanCfgRegisterOffset offset,
                         std::uint32_t &value) noexcept {
  return read_qnx_mmio32(controller, offset_value(offset), value);
}

[[nodiscard]] McanStatus poll_controller_mask(
    const McanMappedMmioRegion &controller, const McanCfgRegisterOffset offset,
    const std::uint32_t mask, const std::uint32_t expected) noexcept {
  for (auto iteration = std::uint32_t{0U}; iteration < kLoopbackPollIterations;
       ++iteration) {
    auto value = std::uint32_t{0U};
    const auto status = read_controller_register(controller, offset, value);
    if (!status_ok(status)) {
      return status;
    }
    if ((value & mask) == expected) {
      return McanStatus::ok;
    }
  }
  return McanStatus::invalid_state;
}

[[nodiscard]] McanStatus
write_message_ram_word(const McanMappedMmioRegion &message_ram,
                       const std::uint32_t word_offset,
                       const std::uint32_t value) noexcept {
  return write_qnx_mmio32(message_ram, message_ram_byte_offset(word_offset),
                          value);
}

[[nodiscard]] McanStatus
read_message_ram_word(const McanMappedMmioRegion &message_ram,
                      const std::uint32_t word_offset,
                      std::uint32_t &value) noexcept {
  return read_qnx_mmio32(message_ram, message_ram_byte_offset(word_offset),
                         value);
}

[[nodiscard]] McanStatus
clear_loopback_message_ram(const McanMappedMmioRegion &message_ram) noexcept {
  for (auto word = std::uint32_t{0U}; word < kLoopbackClearMessageRamWords;
       ++word) {
    const auto status = write_message_ram_word(message_ram, word, 0U);
    if (!status_ok(status)) {
      return status;
    }
  }
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
write_tx_buffer_element(const McanMappedMmioRegion &message_ram,
                        const McanMessageRamLayout &layout,
                        const McanMessageRamElement &element) noexcept {
  auto word = static_cast<std::uint32_t>(layout.tx_buffers.start_word);
  for (auto index = std::size_t{0U}; index < element.words.size(); ++index) {
    const auto status =
        write_message_ram_word(message_ram, word, element.words[index]);
    if (!status_ok(status)) {
      return status;
    }
    ++word;
  }
  return McanStatus::ok;
}

[[nodiscard]] McanStatus read_rx_fifo0_element(
    const McanMappedMmioRegion &message_ram, const McanMessageRamLayout &layout,
    const std::uint32_t rx_index, McanMessageRamElement &element) noexcept {
  element = McanMessageRamElement{};
  if (rx_index >= layout.rx_fifo0.count) {
    return McanStatus::invalid_state;
  }
  auto word = static_cast<std::uint32_t>(layout.rx_fifo0.start_word) +
              (rx_index * layout.rx_fifo0.element_words);
  for (auto index = std::size_t{0U}; index < element.words.size(); ++index) {
    const auto status =
        read_message_ram_word(message_ram, word, element.words[index]);
    if (!status_ok(status)) {
      return status;
    }
    ++word;
  }
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
drain_hardware_rx_fifo0_locked(hardware_loopback_state &state,
                               bool &rx_ready) noexcept {
  rx_ready = false;
  auto overflow_seen = false;
  for (;;) {
    auto rxf0s = std::uint32_t{0U};
    auto status = read_controller_register(state.controller,
                                           McanCfgRegisterOffset::rxf0s, rxf0s);
    if (!status_ok(status)) {
      return status;
    }
    const auto rx_fill = rxf0s & kRxFifo0FillLevelMask;
    if (rx_fill == 0U) {
      return overflow_seen ? McanStatus::rx_queue_full : McanStatus::ok;
    }
    const auto rx_index =
        (rxf0s >> kRxFifo0GetIndexShift) & kRxFifo0GetIndexMask;
    if (hardware_rx_queue_has_room(state)) {
      auto element = McanMessageRamElement{};
      status = read_rx_fifo0_element(state.message_ram, state.layout, rx_index,
                                     element);
      auto frame = McanFrameTransfer{};
      if (status_ok(status)) {
        status =
            decode_rx_message_ram_element(element, state.capabilities, frame);
      }
      if (status_ok(status)) {
        enqueue_hardware_rx_frame_locked(state, frame);
        rx_ready = true;
      } else {
        ++state.diagnostics.invalid_frame_rejects;
      }
    } else {
      overflow_seen = true;
      ++state.diagnostics.rx_queue_full;
    }
    const auto acknowledge_status = write_controller_register(
        state.controller, McanCfgRegisterOffset::rxf0a, rx_index);
    if (!status_ok(acknowledge_status)) {
      return acknowledge_status;
    }
  }
  return overflow_seen ? McanStatus::rx_queue_full : McanStatus::ok;
}

void record_hardware_interrupt_errors_locked(hardware_loopback_state &state,
                                             const std::uint32_t ir) noexcept {
  if ((ir & (kIrRxFifo0Full | kIrRxFifo0MessageLost)) != 0U) {
    ++state.diagnostics.rx_queue_full;
  }
  if ((ir & kIrBusOff) != 0U) {
    ++state.diagnostics.bus_off_events;
    state.tx_in_flight = false;
    state.bus_off_latched = true;
  }
  if ((ir & kIrErrorPassive) != 0U) {
    state.error_passive_latched = true;
  }
  if ((ir &
       (kIrMessageRamAccessFailure | kIrErrorLoggingOverflow | kIrErrorPassive |
        kIrWarning | kIrProtocolArbitration | kIrProtocolData)) != 0U) {
    state.diagnostics.last_native_error = ir;
  }
}

[[nodiscard]] McanStatus
process_hardware_interrupt(hardware_loopback_state &state, bool &rx_ready,
                           bool &tx_ready, bool &error_ready) noexcept {
  rx_ready = false;
  tx_ready = false;
  error_ready = false;
  auto status = lock_hardware_state(state);
  if (!status_ok(status)) {
    return status;
  }
  auto ir = std::uint32_t{0U};
  status =
      read_controller_register(state.controller, McanCfgRegisterOffset::ir, ir);
  if (!status_ok(status)) {
    unlock_hardware_state(state);
    return status;
  }
  auto handled_mask = std::uint32_t{0U};
  auto mask_status = hardware_interrupt_mask(handled_mask);
  if (!status_ok(mask_status)) {
    unlock_hardware_state(state);
    return mask_status;
  }
  const auto handled_ir = ir & handled_mask;
  if (handled_ir == 0U) {
    unlock_hardware_state(state);
    return McanStatus::ok;
  }
  ++state.diagnostics.interrupt_count;
  state.diagnostics.last_native_error = handled_ir;
  record_hardware_interrupt_errors_locked(state, handled_ir);
  if ((handled_ir & kIrBusOff) != 0U) {
    error_ready = true;
  }
  auto txbto = std::uint32_t{0U};
  const auto txbto_status = read_controller_register(
      state.controller, McanCfgRegisterOffset::txbto, txbto);
  const auto tx_buffer_completed =
      status_ok(txbto_status) && ((txbto & kTxBuffer0Request) != 0U);
  if ((handled_ir & (kIrTxComplete | kIrTxCancellationFinished |
                     kIrProductionErrorMask)) != 0U ||
      tx_buffer_completed) {
    if (((handled_ir & kIrTxComplete) != 0U || tx_buffer_completed) &&
        state.tx_in_flight) {
      ++state.diagnostics.tx_frames;
    }
    state.tx_in_flight = false;
    tx_ready = true;
  }
  if ((handled_ir &
       (kIrRxFifo0NewMessage | kIrRxFifo0Full | kIrRxFifo0MessageLost)) != 0U) {
    auto local_rx_ready = false;
    status = drain_hardware_rx_fifo0_locked(state, local_rx_ready);
    rx_ready = local_rx_ready;
    if (status == McanStatus::rx_queue_full) {
      status = McanStatus::ok;
    }
  }
  const auto clear_status = write_controller_register(
      state.controller, McanCfgRegisterOffset::ir, handled_ir);
  if (!status_ok(clear_status) && status_ok(status)) {
    status = clear_status;
  }
  unlock_hardware_state(state);
  return status;
}

void notify_hardware_interrupt_waiters(hardware_loopback_state &state,
                                       const bool rx_ready, const bool tx_ready,
                                       const bool error_ready) noexcept {
  if (state.owner == nullptr) {
    return;
  }
  if (error_ready) {
    cancel_all_endpoint_waiters(*state.owner, EIO);
  }
  if (rx_ready) {
    complete_matching_rx_waiters(*state.owner, state.instance_id, 0U);
    trigger_matching_endpoint_notify(*state.owner, EndpointKind::rx,
                                     state.instance_id, 0U);
  }
  if (tx_ready) {
    trigger_matching_endpoint_notify(*state.owner, EndpointKind::tx,
                                     state.instance_id, 0U);
  }
}

[[nodiscard]] bool
hardware_ist_stop_requested(hardware_loopback_state &state) noexcept {
  auto stop = true;
  if (status_ok(lock_hardware_state(state))) {
    stop = state.ist_stop_requested;
    unlock_hardware_state(state);
  }
  return stop;
}

void publish_ist_attach_result(hardware_loopback_state &state,
                               const int interrupt_id,
                               const int attach_errno) noexcept {
  if (!status_ok(lock_hardware_state(state))) {
    return;
  }
  state.interrupt_id = interrupt_id;
  state.ist_attach_errno = attach_errno;
  state.ist_attach_complete = interrupt_id != -1;
  state.ist_attach_failed = interrupt_id == -1;
  (void)::pthread_cond_signal(&state.startup_cond);
  unlock_hardware_state(state);
}

void *hardware_interrupt_thread(void *arg) {
  auto *const state = static_cast<hardware_loopback_state *>(arg);
  if (state == nullptr) {
    return nullptr;
  }
  const auto privilege_status = request_qnx_io_privileges();
  if (!status_ok(privilege_status)) {
    publish_ist_attach_result(*state, -1, EACCES);
    return nullptr;
  }
  const auto interrupt_id = InterruptAttachThread(
      static_cast<int>(state->logical_irq), _NTO_INTR_FLAGS_EXCLUSIVE);
  if (interrupt_id == -1) {
    publish_ist_attach_result(*state, -1, errno);
    return nullptr;
  }
  publish_ist_attach_result(*state, interrupt_id, EOK);
  (void)InterruptUnmask(0, interrupt_id);
  while (!hardware_ist_stop_requested(*state)) {
    const auto wait_status = InterruptWait(0, nullptr);
    if (wait_status == -1) {
      if (errno == EINTR) {
        continue;
      }
      if (hardware_ist_stop_requested(*state)) {
        break;
      }
      continue;
    }
    if (hardware_ist_stop_requested(*state)) {
      break;
    }
    auto rx_ready = false;
    auto tx_ready = false;
    auto error_ready = false;
    const auto process_status =
        process_hardware_interrupt(*state, rx_ready, tx_ready, error_ready);
    if (!status_ok(process_status)) {
      if (status_ok(lock_hardware_state(*state))) {
        state->diagnostics.last_native_error =
            static_cast<std::uint32_t>(process_status);
        unlock_hardware_state(*state);
      }
    }
    notify_hardware_interrupt_waiters(*state, rx_ready, tx_ready, error_ready);
    (void)InterruptUnmask(0, interrupt_id);
  }
  (void)InterruptDetach(interrupt_id);
  if (status_ok(lock_hardware_state(*state))) {
    state->interrupt_id = -1;
    unlock_hardware_state(*state);
  }
  return nullptr;
}

[[nodiscard]] McanStatus
start_hardware_interrupt_thread(hardware_loopback_state &state) noexcept {
  if (state.logical_irq == 0U) {
    return McanStatus::missing_hardware_evidence;
  }
  if (::pthread_create(&state.ist_thread, nullptr, hardware_interrupt_thread,
                       &state) != EOK) {
    return McanStatus::hardware_access_unavailable;
  }
  state.ist_thread_started = true;
  auto status = lock_hardware_state(state);
  if (!status_ok(status)) {
    return status;
  }
  while (!state.ist_attach_complete && !state.ist_attach_failed) {
    (void)::pthread_cond_wait(&state.startup_cond, &state.lock);
  }
  const auto attach_failed = state.ist_attach_failed;
  const auto attach_errno = state.ist_attach_errno;
  unlock_hardware_state(state);
  if (attach_failed) {
    (void)::pthread_join(state.ist_thread, nullptr);
    state.ist_thread_started = false;
    state.ist_attach_errno = attach_errno;
    return McanStatus::hardware_access_unavailable;
  }
  return McanStatus::ok;
}

void request_hardware_ist_stop(hardware_loopback_state &state) noexcept {
  auto interrupt_id = -1;
  if (status_ok(lock_hardware_state(state))) {
    state.ist_stop_requested = true;
    interrupt_id = state.interrupt_id;
    unlock_hardware_state(state);
  }
  if (interrupt_id != -1) {
    (void)InterruptUnblock(interrupt_id);
  }
  if (state.ist_thread_started) {
    (void)::pthread_join(state.ist_thread, nullptr);
    state.ist_thread_started = false;
  }
}

[[nodiscard]] McanStatus program_loopback_message_ram(
    const McanMappedMmioRegion &message_ram, const McanMessageRamLayout &layout,
    const McanFrameTransfer &frame,
    const McanControllerCapabilities &capabilities) noexcept {
  auto status = clear_loopback_message_ram(message_ram);
  if (!status_ok(status)) {
    return status;
  }
  auto element = McanMessageRamElement{};
  status = encode_tx_message_ram_element(frame, capabilities, element);
  if (!status_ok(status)) {
    return status;
  }
  return write_tx_buffer_element(message_ram, layout, element);
}

[[nodiscard]] McanStatus
program_loopback_controller(const McanMappedMmioRegion &controller,
                            const McanCoreConfig &core_config,
                            const McanMessageRamRegisters &message_ram,
                            McanRegisterProgramming &programming) noexcept {
  auto status = make_mcan_register_programming(core_config, programming);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::cccr,
                                     kCccrInit);
  if (!status_ok(status)) {
    return status;
  }
  status = poll_controller_mask(controller, McanCfgRegisterOffset::cccr,
                                kCccrInit, kCccrInit);
  if (!status_ok(status)) {
    return status;
  }
  const auto cccr_config = programming.cccr_config | kCccrConfigChangeEnable;
  status = write_controller_register(controller, McanCfgRegisterOffset::cccr,
                                     cccr_config);
  if (!status_ok(status)) {
    return status;
  }
  status = poll_controller_mask(controller, McanCfgRegisterOffset::cccr,
                                kCccrInit | kCccrConfigChangeEnable,
                                kCccrInit | kCccrConfigChangeEnable);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::test,
                                     programming.test);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::nbtp,
                                     programming.nominal_bit_timing);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::gfc,
                                     programming.global_filter_config);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::sidfc,
                                     message_ram.sidfc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::xidfc,
                                     message_ram.xidfc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::xidam,
                                     programming.extended_id_mask);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::rxf0c,
                                     message_ram.rxf0c);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::rxf1c,
                                     message_ram.rxf1c);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::rxbc,
                                     message_ram.rxbc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::rxesc,
                                     message_ram.rxesc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::txbc,
                                     message_ram.txbc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::txefc,
                                     message_ram.txefc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::txesc,
                                     message_ram.txesc);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::ir,
                                     0xFFFFFFFFU);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::ie, 0U);
  if (!status_ok(status)) {
    return status;
  }
  status =
      write_controller_register(controller, McanCfgRegisterOffset::ile, 0U);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::cccr,
                                     programming.cccr_run);
  if (!status_ok(status)) {
    return status;
  }
  return write_controller_register(controller, McanCfgRegisterOffset::test,
                                   programming.test);
}

[[nodiscard]] McanStatus wait_for_loopback_completion(
    const McanMappedMmioRegion &controller, std::uint32_t &final_ir,
    std::uint32_t &final_rxf0s, std::uint32_t &final_txbto) noexcept {
  final_ir = 0U;
  final_rxf0s = 0U;
  final_txbto = 0U;
  for (auto iteration = std::uint32_t{0U}; iteration < kLoopbackPollIterations;
       ++iteration) {
    auto status = read_controller_register(controller,
                                           McanCfgRegisterOffset::ir, final_ir);
    if (!status_ok(status)) {
      return status;
    }
    status = read_controller_register(controller, McanCfgRegisterOffset::rxf0s,
                                      final_rxf0s);
    if (!status_ok(status)) {
      return status;
    }
    status = read_controller_register(controller, McanCfgRegisterOffset::txbto,
                                      final_txbto);
    if (!status_ok(status)) {
      return status;
    }
    if ((final_ir & kIrLoopbackErrorMask) != 0U) {
      return McanStatus::invalid_state;
    }
    const auto rx_fill_level = final_rxf0s & kRxFifo0FillLevelMask;
    const auto tx_done = ((final_ir & kIrTxComplete) != 0U) ||
                         ((final_txbto & kTxBuffer0Request) != 0U);
    if (((final_ir & kIrRxFifo0NewMessage) != 0U) && tx_done &&
        rx_fill_level > 0U) {
      return McanStatus::ok;
    }
  }
  return McanStatus::invalid_state;
}

[[nodiscard]] McanStatus wait_for_hardware_loopback_completion(
    const McanMappedMmioRegion &controller, const std::uint32_t initial_rx_fill,
    std::uint32_t &final_ir, std::uint32_t &final_rxf0s,
    std::uint32_t &final_txbto) noexcept {
  final_ir = 0U;
  final_rxf0s = 0U;
  final_txbto = 0U;
  for (auto iteration = std::uint32_t{0U}; iteration < kLoopbackPollIterations;
       ++iteration) {
    auto status = read_controller_register(controller,
                                           McanCfgRegisterOffset::ir, final_ir);
    if (!status_ok(status)) {
      return status;
    }
    status = read_controller_register(controller, McanCfgRegisterOffset::rxf0s,
                                      final_rxf0s);
    if (!status_ok(status)) {
      return status;
    }
    status = read_controller_register(controller, McanCfgRegisterOffset::txbto,
                                      final_txbto);
    if (!status_ok(status)) {
      return status;
    }
    if ((final_ir & kIrLoopbackErrorMask) != 0U) {
      return McanStatus::invalid_state;
    }
    const auto rx_fill_level = final_rxf0s & kRxFifo0FillLevelMask;
    const auto tx_done = ((final_ir & kIrTxComplete) != 0U) ||
                         ((final_txbto & kTxBuffer0Request) != 0U);
    if (tx_done && rx_fill_level > initial_rx_fill) {
      return McanStatus::ok;
    }
  }
  return McanStatus::invalid_state;
}

void print_loopback_register_snapshot(const McanMappedMmioRegion &controller,
                                      const char *const label) noexcept {
  auto cccr = std::uint32_t{0U};
  auto test = std::uint32_t{0U};
  auto nbtp = std::uint32_t{0U};
  auto txbc = std::uint32_t{0U};
  auto txesc = std::uint32_t{0U};
  auto txbar = std::uint32_t{0U};
  auto txbrp = std::uint32_t{0U};
  auto txbto = std::uint32_t{0U};
  auto rxf0c = std::uint32_t{0U};
  auto rxf0s = std::uint32_t{0U};
  auto ir = std::uint32_t{0U};
  auto status =
      read_controller_register(controller, McanCfgRegisterOffset::cccr, cccr);
  if (status_ok(status)) {
    status =
        read_controller_register(controller, McanCfgRegisterOffset::test, test);
  }
  if (status_ok(status)) {
    status =
        read_controller_register(controller, McanCfgRegisterOffset::nbtp, nbtp);
  }
  if (status_ok(status)) {
    status =
        read_controller_register(controller, McanCfgRegisterOffset::txbc, txbc);
  }
  if (status_ok(status)) {
    status = read_controller_register(controller, McanCfgRegisterOffset::txesc,
                                      txesc);
  }
  if (status_ok(status)) {
    status = read_controller_register(controller, McanCfgRegisterOffset::txbar,
                                      txbar);
  }
  if (status_ok(status)) {
    status = read_controller_register(controller, McanCfgRegisterOffset::txbrp,
                                      txbrp);
  }
  if (status_ok(status)) {
    status = read_controller_register(controller, McanCfgRegisterOffset::txbto,
                                      txbto);
  }
  if (status_ok(status)) {
    status = read_controller_register(controller, McanCfgRegisterOffset::rxf0c,
                                      rxf0c);
  }
  if (status_ok(status)) {
    status = read_controller_register(controller, McanCfgRegisterOffset::rxf0s,
                                      rxf0s);
  }
  if (status_ok(status)) {
    status =
        read_controller_register(controller, McanCfgRegisterOffset::ir, ir);
  }
  if (!status_ok(status)) {
    std::printf("loopback snapshot=%s status=%s\n", label, status_name(status));
    return;
  }
  std::printf(
      "loopback snapshot=%s cccr=0x%08x test=0x%08x "
      "nbtp=0x%08x txbc=0x%08x txesc=0x%08x txbar=0x%08x "
      "txbrp=0x%08x txbto=0x%08x rxf0c=0x%08x rxf0s=0x%08x "
      "ir=0x%08x\n",
      label, static_cast<unsigned int>(cccr), static_cast<unsigned int>(test),
      static_cast<unsigned int>(nbtp), static_cast<unsigned int>(txbc),
      static_cast<unsigned int>(txesc), static_cast<unsigned int>(txbar),
      static_cast<unsigned int>(txbrp), static_cast<unsigned int>(txbto),
      static_cast<unsigned int>(rxf0c), static_cast<unsigned int>(rxf0s),
      static_cast<unsigned int>(ir));
}

[[nodiscard]] McanStatus
stop_loopback_controller(const McanMappedMmioRegion &controller) noexcept {
  auto first_failure = McanStatus::ok;
  auto status =
      write_controller_register(controller, McanCfgRegisterOffset::cccr,
                                kCccrInit | kCccrConfigChangeEnable);
  if (!status_ok(status)) {
    first_failure = status;
  }
  status =
      write_controller_register(controller, McanCfgRegisterOffset::test, 0U);
  if (!status_ok(status) && status_ok(first_failure)) {
    first_failure = status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::ie, 0U);
  if (!status_ok(status) && status_ok(first_failure)) {
    first_failure = status;
  }
  status =
      write_controller_register(controller, McanCfgRegisterOffset::ile, 0U);
  if (!status_ok(status) && status_ok(first_failure)) {
    first_failure = status;
  }
  status = write_controller_register(controller, McanCfgRegisterOffset::ir,
                                     0xFFFFFFFFU);
  if (!status_ok(status) && status_ok(first_failure)) {
    first_failure = status;
  }
  return first_failure;
}

[[nodiscard]] McanStatus
read_hardware_rx_fill(const hardware_loopback_state &state,
                      std::uint32_t &rx_fill) noexcept {
  rx_fill = 0U;
  if (!state.initialized) {
    return McanStatus::invalid_state;
  }
  auto rxf0s = std::uint32_t{0U};
  const auto status = read_controller_register(
      state.controller, McanCfgRegisterOffset::rxf0s, rxf0s);
  if (!status_ok(status)) {
    return status;
  }
  rx_fill = rxf0s & kRxFifo0FillLevelMask;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
hardware_tx_room_available(const hardware_loopback_state &state,
                           bool &available) noexcept {
  available = false;
  auto rx_fill = std::uint32_t{0U};
  auto status = read_hardware_rx_fill(state, rx_fill);
  if (!status_ok(status)) {
    return status;
  }
  if (rx_fill >= state.layout.rx_fifo0.count) {
    return McanStatus::ok;
  }
  auto txbrp = std::uint32_t{0U};
  status = read_controller_register(state.controller,
                                    McanCfgRegisterOffset::txbrp, txbrp);
  if (!status_ok(status)) {
    return status;
  }
  available = (txbrp & kTxBuffer0Request) == 0U;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus initialize_hardware_loopback_state(
    hardware_loopback_state &state,
    const McanControllerConfig &controller_config, const bool internal_loopback,
    const bool interrupt_mode, skeleton_state *const owner) noexcept {
  state = hardware_loopback_state{};
  auto status = validate_controller_hardware_mapping(controller_config);
  if (!status_ok(status)) {
    return status;
  }
  if (interrupt_mode &&
      (!controller_config.qnx_irq_routing_evidence_confirmed ||
       controller_config.hardware.qnx_logical_irq == 0U)) {
    return McanStatus::missing_hardware_evidence;
  }
  status = request_qnx_io_privileges();
  if (!status_ok(status)) {
    return status;
  }
  status = initialize_hardware_thread_objects(state);
  if (!status_ok(status)) {
    return status;
  }
  status = map_qnx_mmio_region(controller_config.hardware.controller,
                               state.controller);
  if (!status_ok(status)) {
    destroy_hardware_thread_objects(state);
    return status;
  }
  status = map_qnx_mmio_region(controller_config.hardware.message_ram,
                               state.message_ram);
  if (!status_ok(status)) {
    (void)shutdown_hardware_loopback_state(state);
    return status;
  }

  state.owner = owner;
  state.instance_id = controller_config.instance_id;
  state.logical_irq = controller_config.hardware.qnx_logical_irq;
  state.interrupt_mode = interrupt_mode;
  state.internal_loopback = internal_loopback;
  state.layout = make_internal_loopback_smoke_message_ram_layout();
  state.capabilities =
      make_hardware_capabilities(controller_config, internal_loopback);
  state.capabilities.max_rx_queue_capacity = state.layout.rx_fifo0.count;
  state.capabilities.max_tx_queue_capacity = 1U;

  auto message_ram_registers = McanMessageRamRegisters{};
  status = encode_message_ram_registers(state.layout, message_ram_registers);
  if (status_ok(status)) {
    status = clear_loopback_message_ram(state.message_ram);
  }

  auto controller_touched = false;
  auto core_config = McanCoreConfig{};
  core_config.controller = controller_config;
  core_config.nominal_timing = make_classic_500k_timing();
  core_config.internal_loopback = internal_loopback;
  core_config.interrupts_enabled = interrupt_mode;
  auto programming = McanRegisterProgramming{};
  if (status_ok(status)) {
    controller_touched = true;
    status = program_loopback_controller(state.controller, core_config,
                                         message_ram_registers, programming);
  }
  if (status_ok(status)) {
    status = poll_controller_mask(state.controller, McanCfgRegisterOffset::cccr,
                                  kCccrInit, 0U);
  }
  if (!status_ok(status)) {
    if (controller_touched) {
      (void)stop_loopback_controller(state.controller);
    }
    (void)shutdown_hardware_loopback_state(state);
    return status;
  }
  if (interrupt_mode) {
    status = start_hardware_interrupt_thread(state);
    if (status_ok(status)) {
      status = enable_hardware_interrupts(state);
    }
    if (!status_ok(status)) {
      (void)shutdown_hardware_loopback_state(state);
      return status;
    }
    state.interrupts_enabled = true;
  }
  state.initialized = true;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
shutdown_hardware_loopback_state(hardware_loopback_state &state) noexcept {
  auto first_failure = McanStatus::ok;
  if (state.interrupts_enabled && state.controller.address != nullptr) {
    const auto status = disable_hardware_interrupts(state);
    if (!status_ok(status)) {
      first_failure = status;
    }
  }
  if (state.ist_thread_started) {
    request_hardware_ist_stop(state);
  }
  if (state.initialized && state.controller.address != nullptr) {
    const auto status = stop_loopback_controller(state.controller);
    if (!status_ok(status)) {
      first_failure = status;
    }
  }
  if (state.message_ram.address != nullptr) {
    const auto status = unmap_qnx_mmio_region(state.message_ram);
    if (!status_ok(status) && status_ok(first_failure)) {
      first_failure = status;
    }
  }
  if (state.controller.address != nullptr) {
    const auto status = unmap_qnx_mmio_region(state.controller);
    if (!status_ok(status) && status_ok(first_failure)) {
      first_failure = status;
    }
  }
  state.initialized = false;
  destroy_hardware_thread_objects(state);
  return first_failure;
}

[[nodiscard]] McanStatus
hardware_loopback_transmit_frame(hardware_loopback_state &state,
                                 const std::uint8_t queue_id,
                                 const McanFrameTransfer &frame) noexcept {
  if (!state.initialized) {
    return McanStatus::invalid_state;
  }
  if (queue_id != 0U) {
    return McanStatus::invalid_queue;
  }
  if (state.interrupt_mode) {
    auto status = lock_hardware_state(state);
    if (!status_ok(status)) {
      return status;
    }
    if (state.tx_in_flight) {
      ++state.diagnostics.tx_queue_full;
      unlock_hardware_state(state);
      return McanStatus::tx_queue_full;
    }
    if (state.bus_off_latched) {
      unlock_hardware_state(state);
      return McanStatus::invalid_state;
    }
    if (state.internal_loopback && !hardware_rx_queue_has_room(state)) {
      ++state.diagnostics.rx_queue_full;
      unlock_hardware_state(state);
      return McanStatus::rx_queue_full;
    }
    auto element = McanMessageRamElement{};
    status = encode_tx_message_ram_element(frame, state.capabilities, element);
    if (!status_ok(status)) {
      ++state.diagnostics.invalid_frame_rejects;
      unlock_hardware_state(state);
      return status;
    }
    status = write_tx_buffer_element(state.message_ram, state.layout, element);
    if (status_ok(status)) {
      status = write_controller_register(
          state.controller, McanCfgRegisterOffset::ir,
          kIrRxFifo0NewMessage | kIrRxFifo0Full | kIrRxFifo0MessageLost |
              kIrTxComplete | kIrTxCancellationFinished |
              kIrProductionErrorMask);
    }
    if (status_ok(status)) {
      state.tx_in_flight = true;
      status = write_controller_register(
          state.controller, McanCfgRegisterOffset::txbar, kTxBuffer0Request);
    }
    if (!status_ok(status)) {
      state.tx_in_flight = false;
    }
    unlock_hardware_state(state);
    return status;
  }
  auto initial_rx_fill = std::uint32_t{0U};
  auto status = read_hardware_rx_fill(state, initial_rx_fill);
  if (!status_ok(status)) {
    return status;
  }
  if (initial_rx_fill >= state.layout.rx_fifo0.count) {
    ++state.diagnostics.rx_queue_full;
    return McanStatus::rx_queue_full;
  }
  auto room_available = false;
  status = hardware_tx_room_available(state, room_available);
  if (!status_ok(status)) {
    return status;
  }
  if (!room_available) {
    ++state.diagnostics.tx_queue_full;
    return McanStatus::tx_queue_full;
  }

  auto element = McanMessageRamElement{};
  status = encode_tx_message_ram_element(frame, state.capabilities, element);
  if (!status_ok(status)) {
    ++state.diagnostics.invalid_frame_rejects;
    return status;
  }
  status = write_tx_buffer_element(state.message_ram, state.layout, element);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(
      state.controller, McanCfgRegisterOffset::ir,
      kIrRxFifo0NewMessage | kIrTxComplete | kIrLoopbackErrorMask);
  if (!status_ok(status)) {
    return status;
  }
  status = write_controller_register(
      state.controller, McanCfgRegisterOffset::txbar, kTxBuffer0Request);
  if (!status_ok(status)) {
    return status;
  }
  auto final_ir = std::uint32_t{0U};
  auto final_rxf0s = std::uint32_t{0U};
  auto final_txbto = std::uint32_t{0U};
  status = wait_for_hardware_loopback_completion(
      state.controller, initial_rx_fill, final_ir, final_rxf0s, final_txbto);
  if (!status_ok(status)) {
    state.diagnostics.last_native_error = final_ir;
    return status;
  }
  ++state.diagnostics.tx_frames;
  state.diagnostics.last_native_error = final_ir;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
hardware_loopback_receive_frame(hardware_loopback_state &state,
                                const std::uint8_t queue_id,
                                McanFrameTransfer &frame) noexcept {
  frame = McanFrameTransfer{};
  if (!state.initialized) {
    return McanStatus::invalid_state;
  }
  if (queue_id != 0U) {
    return McanStatus::invalid_queue;
  }
  if (state.interrupt_mode) {
    const auto status = lock_hardware_state(state);
    if (!status_ok(status)) {
      return status;
    }
    const auto receive_status = dequeue_hardware_rx_frame_locked(state, frame);
    unlock_hardware_state(state);
    return receive_status;
  }
  auto rxf0s = std::uint32_t{0U};
  auto status = read_controller_register(state.controller,
                                         McanCfgRegisterOffset::rxf0s, rxf0s);
  if (!status_ok(status)) {
    return status;
  }
  const auto rx_fill = rxf0s & kRxFifo0FillLevelMask;
  if (rx_fill == 0U) {
    return McanStatus::no_frame;
  }
  const auto rx_index = (rxf0s >> kRxFifo0GetIndexShift) & kRxFifo0GetIndexMask;
  auto element = McanMessageRamElement{};
  status =
      read_rx_fifo0_element(state.message_ram, state.layout, rx_index, element);
  if (status_ok(status)) {
    status = decode_rx_message_ram_element(element, state.capabilities, frame);
  }
  if (!status_ok(status)) {
    ++state.diagnostics.invalid_frame_rejects;
    return status;
  }
  status = write_controller_register(state.controller,
                                     McanCfgRegisterOffset::rxf0a, rx_index);
  if (!status_ok(status)) {
    return status;
  }
  ++state.diagnostics.rx_frames;
  return McanStatus::ok;
}

[[nodiscard]] McanStatus
endpoint_receive_frame(endpoint_attachment &endpoint,
                       McanFrameTransfer &frame) noexcept {
  if (endpoint.hardware != nullptr) {
    return hardware_loopback_receive_frame(*endpoint.hardware,
                                           endpoint.endpoint_id, frame);
  }
  if (endpoint.runtime == nullptr) {
    return McanStatus::invalid_state;
  }
  return resource_manager_receive_frame(*endpoint.runtime, endpoint.kind,
                                        endpoint.instance_id,
                                        endpoint.endpoint_id, frame);
}

[[nodiscard]] McanStatus
endpoint_transmit_frame(endpoint_attachment &endpoint,
                        const McanFrameTransfer &frame) noexcept {
  if (endpoint.hardware != nullptr) {
    return hardware_loopback_transmit_frame(*endpoint.hardware,
                                            endpoint.endpoint_id, frame);
  }
  if (endpoint.runtime == nullptr) {
    return McanStatus::invalid_state;
  }
  return resource_manager_transmit_frame(*endpoint.runtime, endpoint.kind,
                                         endpoint.instance_id,
                                         endpoint.endpoint_id, frame);
}

[[nodiscard]] McanStatus
endpoint_query_capabilities(endpoint_attachment &endpoint,
                            McanControllerCapabilities &capabilities) noexcept {
  if (endpoint.hardware != nullptr) {
    capabilities = endpoint.hardware->capabilities;
    return endpoint.hardware->initialized ? McanStatus::ok
                                          : McanStatus::invalid_state;
  }
  if (endpoint.runtime == nullptr) {
    return McanStatus::invalid_state;
  }
  return resource_manager_query_capabilities(
      *endpoint.runtime, endpoint.kind, endpoint.instance_id, capabilities);
}

[[nodiscard]] McanStatus
endpoint_query_diagnostics(endpoint_attachment &endpoint,
                           McanDiagnosticsSnapshot &diagnostics) noexcept {
  if (endpoint.hardware != nullptr) {
    if (endpoint.hardware->interrupt_mode) {
      const auto lock_status = lock_hardware_state(*endpoint.hardware);
      if (!status_ok(lock_status)) {
        return lock_status;
      }
      diagnostics = endpoint.hardware->diagnostics;
      const auto initialized = endpoint.hardware->initialized;
      unlock_hardware_state(*endpoint.hardware);
      return initialized ? McanStatus::ok : McanStatus::invalid_state;
    }
    diagnostics = endpoint.hardware->diagnostics;
    return endpoint.hardware->initialized ? McanStatus::ok
                                          : McanStatus::invalid_state;
  }
  if (endpoint.runtime == nullptr) {
    return McanStatus::invalid_state;
  }
  return resource_manager_query_diagnostics(*endpoint.runtime, endpoint.kind,
                                            endpoint.instance_id, diagnostics);
}

[[nodiscard]] McanStatus
endpoint_rx_ready_count(endpoint_attachment &endpoint,
                        std::size_t &ready_count) noexcept {
  ready_count = 0U;
  if (endpoint.hardware != nullptr) {
    if (endpoint.hardware->interrupt_mode) {
      const auto lock_status = lock_hardware_state(*endpoint.hardware);
      if (!status_ok(lock_status)) {
        return lock_status;
      }
      ready_count = endpoint.hardware->rx_count;
      unlock_hardware_state(*endpoint.hardware);
      return McanStatus::ok;
    }
    auto rx_fill = std::uint32_t{0U};
    const auto status = read_hardware_rx_fill(*endpoint.hardware, rx_fill);
    if (!status_ok(status)) {
      return status;
    }
    ready_count = rx_fill;
    return McanStatus::ok;
  }
  if (endpoint.runtime == nullptr) {
    return McanStatus::invalid_state;
  }
  return resource_manager_rx_ready_count(*endpoint.runtime, endpoint.kind,
                                         endpoint.instance_id,
                                         endpoint.endpoint_id, ready_count);
}

[[nodiscard]] McanStatus
endpoint_tx_room_count(endpoint_attachment &endpoint,
                       std::size_t &room_count) noexcept {
  room_count = 0U;
  if (endpoint.hardware != nullptr) {
    if (endpoint.hardware->interrupt_mode) {
      const auto lock_status = lock_hardware_state(*endpoint.hardware);
      if (!status_ok(lock_status)) {
        return lock_status;
      }
      room_count = endpoint.hardware->tx_in_flight ? 0U : 1U;
      unlock_hardware_state(*endpoint.hardware);
      return McanStatus::ok;
    }
    auto room_available = false;
    const auto status =
        hardware_tx_room_available(*endpoint.hardware, room_available);
    if (!status_ok(status)) {
      return status;
    }
    room_count = room_available ? 1U : 0U;
    return McanStatus::ok;
  }
  if (endpoint.runtime == nullptr) {
    return McanStatus::invalid_state;
  }
  return resource_manager_tx_room_count(*endpoint.runtime, endpoint.kind,
                                        endpoint.instance_id,
                                        endpoint.endpoint_id, room_count);
}

[[nodiscard]] int run_internal_loopback_smoke_for_instance(
    const J784McanBoardInstance &instance) noexcept {
  auto controller_config = McanControllerConfig{};
  controller_config.instance_id = instance.default_endpoint_instance_id;
  controller_config.mode = McanMode::classic;
  controller_config.arbitration_bitrate_valid = true;
  controller_config.arbitration_bitrate = 500'000U;
  controller_config.rx_queue_capacity = 16U;
  controller_config.tx_queue_capacity = 16U;
  controller_config.rx_queue_count = 1U;
  controller_config.tx_queue_count = 1U;
  controller_config.hardware = instance.hardware;
  auto status = validate_controller_hardware_mapping(controller_config);
  if (!status_ok(status)) {
    std::printf("loopback mapping_status=%s\n", status_name(status));
    return EXIT_FAILURE;
  }
  auto controller = McanMappedMmioRegion{};
  status = map_qnx_mmio_region(instance.hardware.controller, controller);
  if (!status_ok(status)) {
    std::printf("loopback controller_map_status=%s\n", status_name(status));
    return EXIT_FAILURE;
  }
  auto message_ram = McanMappedMmioRegion{};
  status = map_qnx_mmio_region(instance.hardware.message_ram, message_ram);
  if (!status_ok(status)) {
    std::printf("loopback message_ram_map_status=%s\n", status_name(status));
    (void)unmap_qnx_mmio_region(controller);
    return EXIT_FAILURE;
  }
  auto result = EXIT_SUCCESS;
  const auto layout = make_internal_loopback_smoke_message_ram_layout();
  auto message_ram_registers = McanMessageRamRegisters{};
  status = encode_message_ram_registers(layout, message_ram_registers);
  if (!status_ok(status)) {
    std::printf("loopback message_ram_register_status=%s\n",
                status_name(status));
    result = EXIT_FAILURE;
  }
  auto core_config = McanCoreConfig{};
  core_config.controller = controller_config;
  core_config.nominal_timing = make_classic_500k_timing();
  core_config.internal_loopback = true;
  core_config.interrupts_enabled = false;
  const auto capabilities = make_loopback_capabilities(controller_config);
  const auto tx_frame = make_loopback_frame();
  if (result == EXIT_SUCCESS) {
    status = program_loopback_message_ram(message_ram, layout, tx_frame,
                                          capabilities);
    if (!status_ok(status)) {
      std::printf("loopback message_ram_program_status=%s\n",
                  status_name(status));
      result = EXIT_FAILURE;
    }
  }
  auto programming = McanRegisterProgramming{};
  if (result == EXIT_SUCCESS) {
    status = program_loopback_controller(controller, core_config,
                                         message_ram_registers, programming);
    if (!status_ok(status)) {
      std::printf("loopback controller_program_status=%s\n",
                  status_name(status));
      result = EXIT_FAILURE;
    } else {
      print_loopback_register_snapshot(controller, "programmed");
    }
  }
  if (result == EXIT_SUCCESS) {
    status = poll_controller_mask(controller, McanCfgRegisterOffset::cccr,
                                  kCccrInit, 0U);
    if (!status_ok(status)) {
      std::printf("loopback normal_mode_status=%s\n", status_name(status));
      result = EXIT_FAILURE;
    } else {
      print_loopback_register_snapshot(controller, "normal");
    }
  }
  if (result == EXIT_SUCCESS) {
    status = write_controller_register(controller, McanCfgRegisterOffset::txbar,
                                       kTxBuffer0Request);
    if (!status_ok(status)) {
      std::printf("loopback tx_request_status=%s\n", status_name(status));
      result = EXIT_FAILURE;
    } else {
      print_loopback_register_snapshot(controller, "tx_requested");
    }
  }
  auto final_ir = std::uint32_t{0U};
  auto final_rxf0s = std::uint32_t{0U};
  auto final_txbto = std::uint32_t{0U};
  if (result == EXIT_SUCCESS) {
    status = wait_for_loopback_completion(controller, final_ir, final_rxf0s,
                                          final_txbto);
    std::printf("loopback poll_status=%s ir=0x%08x rxf0s=0x%08x "
                "txbto=0x%08x\n",
                status_name(status), static_cast<unsigned int>(final_ir),
                static_cast<unsigned int>(final_rxf0s),
                static_cast<unsigned int>(final_txbto));
    if (!status_ok(status)) {
      result = EXIT_FAILURE;
    }
  }
  if (result == EXIT_SUCCESS) {
    const auto rx_index =
        (final_rxf0s >> kRxFifo0GetIndexShift) & kRxFifo0GetIndexMask;
    auto rx_element = McanMessageRamElement{};
    status = read_rx_fifo0_element(message_ram, layout, rx_index, rx_element);
    if (status_ok(status)) {
      auto rx_frame = McanFrameTransfer{};
      status =
          decode_rx_message_ram_element(rx_element, capabilities, rx_frame);
      if (status_ok(status) && frames_match(tx_frame, rx_frame)) {
        std::printf("loopback frame_status=ok can_id=0x%03x length=%u\n",
                    static_cast<unsigned int>(rx_frame.can_id),
                    static_cast<unsigned int>(rx_frame.length));
      } else {
        status = status_ok(status) ? McanStatus::invalid_state : status;
      }
    }
    if (!status_ok(status)) {
      std::printf("loopback frame_status=%s\n", status_name(status));
      result = EXIT_FAILURE;
    }
    status = write_controller_register(controller, McanCfgRegisterOffset::rxf0a,
                                       rx_index);
    if (!status_ok(status)) {
      std::printf("loopback rx_fifo_ack_status=%s\n", status_name(status));
      result = EXIT_FAILURE;
    }
  }
  status = stop_loopback_controller(controller);
  if (!status_ok(status)) {
    std::printf("loopback stop_status=%s\n", status_name(status));
    result = EXIT_FAILURE;
  }
  status = unmap_qnx_mmio_region(message_ram);
  if (!status_ok(status)) {
    std::printf("loopback message_ram_unmap_status=%s\n", status_name(status));
    result = EXIT_FAILURE;
  }
  status = unmap_qnx_mmio_region(controller);
  if (!status_ok(status)) {
    std::printf("loopback controller_unmap_status=%s\n", status_name(status));
    result = EXIT_FAILURE;
  }
  return result;
}

[[nodiscard]] int run_internal_loopback_smoke_endpoint(
    const std::uint8_t endpoint_instance_id) noexcept {
  const auto privilege_status = request_qnx_io_privileges();
  if (!status_ok(privilege_status)) {
    std::printf("qnx_io_privilege_status=%s\n", status_name(privilege_status));
    return EXIT_FAILURE;
  }
  const auto *const instance =
      find_j784_mcan_board_instance_by_endpoint(endpoint_instance_id);
  if (instance == nullptr) {
    std::printf("loopback instance_status=invalid_hardware_mapping\n");
    return EXIT_FAILURE;
  }
  std::printf("qnx_io_privilege_status=ok\n");
  std::printf(
      "loopback instance=%s endpoint=%u\n", instance->name,
      static_cast<unsigned int>(instance->default_endpoint_instance_id));
  return run_internal_loopback_smoke_for_instance(*instance);
}

[[nodiscard]] int
run_internal_loopback_smoke(const McanResourceManagerConfig &config) noexcept {
  const auto skeleton_status = validate_resource_manager_skeleton(config);
  if (!status_ok(skeleton_status)) {
    std::printf("config_status=%s\n", status_name(skeleton_status));
    return EXIT_FAILURE;
  }
  return run_internal_loopback_smoke_endpoint(
      config.controllers[0].instance_id);
}

[[nodiscard]] int
run_dry_hardware_config(const McanResourceManagerConfig &config) noexcept {
  const auto skeleton_status = validate_resource_manager_skeleton(config);
  if (!status_ok(skeleton_status)) {
    return EXIT_FAILURE;
  }
  for (auto index = std::size_t{0U}; index < config.controller_count; ++index) {
    const auto &controller = config.controllers[index];
    auto status = validate_controller_hardware_mapping(controller);
    if (!status_ok(status)) {
      return EXIT_FAILURE;
    }

    auto core_config = McanCoreConfig{};
    core_config.controller = controller;
    core_config.nominal_timing = make_classic_500k_timing();
    core_config.data_timing = make_fd_2m_timing();
    core_config.internal_loopback = true;
    core_config.interrupts_enabled = true;

    auto registers = McanRegisterProgramming{};
    status = make_mcan_register_programming(core_config, registers);
    if (!status_ok(status)) {
      return EXIT_FAILURE;
    }

    const auto layout = make_pdk_loopback_message_ram_layout();
    auto message_ram_registers = McanMessageRamRegisters{};
    status = encode_message_ram_registers(layout, message_ram_registers);
    if (!status_ok(status)) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

[[nodiscard]] int run_validate_production_ownership_scope() noexcept {
  auto production_config = McanResourceManagerConfig{};
  auto status =
      make_j784_mcan_production_resource_manager_config(production_config);
  std::printf("production_config_status=%s\n", status_name(status));
  if (!status_ok(status)) {
    return EXIT_FAILURE;
  }
  const auto policy = j784_mcan_default_production_scope_policy();
  status =
      validate_j784_mcan_production_ownership_scope(production_config, policy);
  std::printf("production_ownership_scope_status=%s\n", status_name(status));
  return status_ok(status) ? EXIT_SUCCESS : EXIT_FAILURE;
}

[[nodiscard]] int
run_resource_manager(const McanResourceManagerConfig &config,
                     const McanResourceManagerRuntimeOptions &runtime_options,
                     const bool hardware_loopback_enabled,
                     const bool qnx_can_compat_enabled,
                     const bool qnx_can_raw_mode_enabled,
                     const bool qnx_can_listen_only) noexcept {
  auto state = skeleton_state{};
  const auto init_status = initialize_skeleton_state(
      state, config, runtime_options, hardware_loopback_enabled,
      qnx_can_compat_enabled, qnx_can_raw_mode_enabled, qnx_can_listen_only);
  if (init_status != EOK) {
    std::printf("resource_manager_init_status=%d\n", init_status);
    return EXIT_FAILURE;
  }
  const auto signal_status = install_shutdown_signal_handlers();
  if (signal_status != EOK) {
    std::printf("resource_manager_signal_status=%d\n", signal_status);
    if (state.hardware_loopback_enabled) {
      (void)shutdown_hardware_loopback_state(state.hardware_loopback);
    }
    destroy_endpoint_notify_locks(state);
    return EXIT_FAILURE;
  }

  auto *context = resmgr_context_alloc(state.dispatch);
  if (context == nullptr) {
    if (state.hardware_loopback_enabled) {
      (void)shutdown_hardware_loopback_state(state.hardware_loopback);
    }
    destroy_endpoint_notify_locks(state);
    return EXIT_FAILURE;
  }
  auto exit_status = EXIT_SUCCESS;
  while (g_resource_manager_shutdown_requested == 0) {
    context = resmgr_block(context);
    if (context == nullptr) {
      if (g_resource_manager_shutdown_requested != 0) {
        break;
      }
      if (errno == EINTR) {
        continue;
      }
      cancel_all_endpoint_waiters(state, EIO);
      if (state.hardware_loopback_enabled) {
        (void)shutdown_hardware_loopback_state(state.hardware_loopback);
      }
      destroy_endpoint_notify_locks(state);
      exit_status = EXIT_FAILURE;
      break;
    }
    resmgr_handler(context);
  }
  cancel_all_endpoint_waiters(state, EIO);
  if (state.hardware_loopback_enabled) {
    const auto shutdown_status =
        shutdown_hardware_loopback_state(state.hardware_loopback);
    if (!status_ok(shutdown_status)) {
      std::printf("resource_manager_shutdown_status=%s\n",
                  status_name(shutdown_status));
      exit_status = EXIT_FAILURE;
    }
  }
  destroy_endpoint_notify_locks(state);
  return exit_status;
}

[[nodiscard]] int
run_attach_smoke(const McanResourceManagerConfig &config,
                 const McanResourceManagerRuntimeOptions &runtime_options,
                 const bool hardware_loopback_enabled,
                 const bool qnx_can_compat_enabled) noexcept {
  auto state = skeleton_state{};
  const auto status = initialize_skeleton_state(
      state, config, runtime_options, hardware_loopback_enabled,
      qnx_can_compat_enabled, qnx_can_compat_enabled, false);
  if (state.hardware_loopback_enabled) {
    (void)shutdown_hardware_loopback_state(state.hardware_loopback);
  }
  destroy_endpoint_notify_locks(state);
  return status == EOK ? EXIT_SUCCESS : EXIT_FAILURE;
}

[[nodiscard]] McanResourceManagerRuntimeOptions
normal_resource_manager_options() noexcept {
  auto options = McanResourceManagerRuntimeOptions{};
  options.frame_io_enabled = false;
  options.internal_loopback = false;
  options.interrupts_enabled = false;
  options.complete_tx_immediately = true;
  return options;
}

[[nodiscard]] McanResourceManagerRuntimeOptions
software_loopback_resource_manager_options() noexcept {
  auto options = McanResourceManagerRuntimeOptions{};
  options.frame_io_enabled = true;
  options.internal_loopback = true;
  options.interrupts_enabled = true;
  options.complete_tx_immediately = true;
  return options;
}

[[nodiscard]] McanResourceManagerRuntimeOptions
hardware_irq_resource_manager_options(const bool internal_loopback) noexcept {
  auto options = McanResourceManagerRuntimeOptions{};
  options.frame_io_enabled = false;
  options.internal_loopback = internal_loopback;
  options.interrupts_enabled = true;
  options.complete_tx_immediately = false;
  return options;
}

[[nodiscard]] int open_path(const McanEndpointPath &path,
                            const int open_flags) noexcept {
  return ::open(path.value.data(), open_flags);
}

[[nodiscard]] bool parse_endpoint_argument(const char *const argument,
                                           std::uint8_t &endpoint) noexcept {
  if (argument == nullptr) {
    return false;
  }
  char *end = nullptr;
  errno = 0;
  const auto parsed = std::strtoul(argument, &end, 10);
  if (errno != 0 || end == argument || *end != '\0' ||
      parsed >= kMaxControllers) {
    return false;
  }
  endpoint = static_cast<std::uint8_t>(parsed);
  return true;
}

[[nodiscard]] bool parse_irq_argument(const char *const argument,
                                      std::uint32_t &logical_irq) noexcept {
  logical_irq = 0U;
  if (argument == nullptr) {
    return false;
  }
  char *end = nullptr;
  errno = 0;
  const auto parsed = std::strtoul(argument, &end, 0);
  if (errno != 0 || end == argument || *end != '\0' || parsed == 0UL ||
      parsed > static_cast<unsigned long>(
                   std::numeric_limits<std::uint32_t>::max())) {
    return false;
  }
  logical_irq = static_cast<std::uint32_t>(parsed);
  return true;
}

[[nodiscard]] int run_resource_manager_client_smoke(
    const std::uint8_t endpoint_instance_id) noexcept {
  auto rx_path = McanEndpointPath{};
  auto tx_path = McanEndpointPath{};
  auto ctl_path = McanEndpointPath{};
  if (!status_ok(make_endpoint_path(EndpointKind::rx, endpoint_instance_id, 0U,
                                    rx_path)) ||
      !status_ok(make_endpoint_path(EndpointKind::tx, endpoint_instance_id, 0U,
                                    tx_path)) ||
      !status_ok(make_endpoint_path(EndpointKind::ctl, endpoint_instance_id, 0U,
                                    ctl_path))) {
    return EXIT_FAILURE;
  }

  const auto ctl_fd = open_path(ctl_path, O_RDONLY);
  if (ctl_fd == -1) {
    std::printf("client_smoke ctl_open_errno=%d\n", errno);
    return EXIT_FAILURE;
  }
  auto capabilities = McanControllerCapabilities{};
  auto devctl_status =
      ::devctl(ctl_fd, static_cast<int>(kMcanDevctlQueryCapabilities),
               &capabilities, sizeof(capabilities), nullptr);
  if (devctl_status != EOK) {
    std::printf("client_smoke capabilities_status=%d errno=%d\n", devctl_status,
                errno);
    (void)::close(ctl_fd);
    return EXIT_FAILURE;
  }
  std::printf("client_smoke capabilities_status=ok flags=0x%08x\n",
              static_cast<unsigned int>(capabilities.flags));

  const auto rx_fd = open_path(rx_path, O_RDONLY | O_NONBLOCK);
  if (rx_fd == -1) {
    std::printf("client_smoke rx_open_errno=%d\n", errno);
    (void)::close(ctl_fd);
    return EXIT_FAILURE;
  }
  const auto tx_fd = open_path(tx_path, O_WRONLY);
  if (tx_fd == -1) {
    std::printf("client_smoke tx_open_errno=%d\n", errno);
    (void)::close(rx_fd);
    (void)::close(ctl_fd);
    return EXIT_FAILURE;
  }

  const auto tx_frame = make_loopback_frame();
  const auto write_count = ::write(tx_fd, &tx_frame, sizeof(tx_frame));
  if (write_count != static_cast<ssize_t>(sizeof(tx_frame))) {
    std::printf("client_smoke write_count=%d errno=%d\n",
                static_cast<int>(write_count), errno);
    (void)::close(tx_fd);
    (void)::close(rx_fd);
    (void)::close(ctl_fd);
    return EXIT_FAILURE;
  }
  auto rx_frame = McanFrameTransfer{};
  auto read_count = ssize_t{-1};
  auto read_errno = int{0};
  for (auto attempt = std::uint32_t{0U}; attempt < 100U; ++attempt) {
    errno = 0;
    read_count = ::read(rx_fd, &rx_frame, sizeof(rx_frame));
    read_errno = errno;
    if (read_count == static_cast<ssize_t>(sizeof(rx_frame))) {
      break;
    }
    if (read_count == -1 &&
        (read_errno == EAGAIN || read_errno == EWOULDBLOCK)) {
      (void)::usleep(10'000U);
      continue;
    }
    break;
  }
  if (read_count != static_cast<ssize_t>(sizeof(rx_frame)) ||
      !frames_match(tx_frame, rx_frame)) {
    std::printf("client_smoke read_count=%d errno=%d frame_match=%u\n",
                static_cast<int>(read_count), read_errno,
                static_cast<unsigned int>(frames_match(tx_frame, rx_frame)));
    (void)::close(tx_fd);
    (void)::close(rx_fd);
    (void)::close(ctl_fd);
    return EXIT_FAILURE;
  }
  auto diagnostics = McanDiagnosticsSnapshot{};
  devctl_status =
      ::devctl(ctl_fd, static_cast<int>(kMcanDevctlQueryDiagnostics),
               &diagnostics, sizeof(diagnostics), nullptr);
  if (devctl_status != EOK) {
    std::printf("client_smoke diagnostics_status=%d errno=%d\n", devctl_status,
                errno);
    (void)::close(tx_fd);
    (void)::close(rx_fd);
    (void)::close(ctl_fd);
    return EXIT_FAILURE;
  }
  std::printf("client_smoke frame_status=ok can_id=0x%03x length=%u "
              "tx_frames=%llu rx_frames=%llu\n",
              static_cast<unsigned int>(rx_frame.can_id),
              static_cast<unsigned int>(rx_frame.length),
              static_cast<unsigned long long>(diagnostics.tx_frames),
              static_cast<unsigned long long>(diagnostics.rx_frames));
  (void)::close(tx_fd);
  (void)::close(rx_fd);
  (void)::close(ctl_fd);
  return EXIT_SUCCESS;
}

[[nodiscard]] bool
qnx_can_messages_match(const struct can_msg &expected,
                       const struct can_msg &actual) noexcept {
  if (expected.len != actual.len || expected.mid != actual.mid ||
      expected.ext.is_extended_mid != actual.ext.is_extended_mid ||
      expected.ext.is_remote_frame != actual.ext.is_remote_frame) {
    return false;
  }
  for (auto index = std::uint8_t{0U}; index < expected.len; ++index) {
    if (expected.dat[index] != actual.dat[index]) {
      return false;
    }
  }
  return true;
}

[[nodiscard]] int run_qnx_can_compat_client_smoke(
    const std::uint8_t endpoint_instance_id) noexcept {
  auto rx_path = McanEndpointPath{};
  auto tx_path = McanEndpointPath{};
  if (!status_ok(make_qnx_can_compat_endpoint_path(
          EndpointKind::rx, endpoint_instance_id, 0U, rx_path)) ||
      !status_ok(make_qnx_can_compat_endpoint_path(
          EndpointKind::tx, endpoint_instance_id, 0U, tx_path))) {
    return EXIT_FAILURE;
  }

  const auto rx_fd = open_path(rx_path, O_RDONLY);
  if (rx_fd == -1) {
    std::printf("qnx_can_compat_smoke rx_open_errno=%d\n", errno);
    return EXIT_FAILURE;
  }
  const auto tx_fd = open_path(tx_path, O_WRONLY);
  if (tx_fd == -1) {
    std::printf("qnx_can_compat_smoke tx_open_errno=%d\n", errno);
    (void)::close(rx_fd);
    return EXIT_FAILURE;
  }

  struct can_msg tx_message{};
  std::memset(&tx_message, 0, sizeof(tx_message));
  tx_message.mid = 0x123U << 18U;
  tx_message.len = 8U;
  for (auto index = std::uint8_t{0U}; index < tx_message.len; ++index) {
    tx_message.dat[index] = static_cast<std::uint8_t>(0x30U + index);
  }

  auto devctl_status = ::devctl(tx_fd, CAN_DEVCTL_TX_FRAME_RAW, &tx_message,
                                sizeof(tx_message), nullptr);
  if (devctl_status != EOK) {
    std::printf("qnx_can_compat_smoke tx_status=%d errno=%d\n", devctl_status,
                errno);
    (void)::close(tx_fd);
    (void)::close(rx_fd);
    return EXIT_FAILURE;
  }

  struct can_msg rx_message{};
  std::memset(&rx_message, 0, sizeof(rx_message));
  auto devctl_errno = int{0};
  for (auto attempt = std::uint32_t{0U}; attempt < 100U; ++attempt) {
    errno = 0;
    devctl_status = ::devctl(rx_fd, CAN_DEVCTL_RX_FRAME_RAW_NOBLOCK,
                             &rx_message, sizeof(rx_message), nullptr);
    devctl_errno = errno;
    if (devctl_status == EOK) {
      break;
    }
    if (devctl_status == EAGAIN || devctl_errno == EAGAIN ||
        devctl_errno == EWOULDBLOCK) {
      (void)::usleep(10'000U);
      continue;
    }
    break;
  }
  if (devctl_status != EOK || !qnx_can_messages_match(tx_message, rx_message)) {
    std::printf("qnx_can_compat_smoke rx_status=%d errno=%d "
                "frame_match=%u\n",
                devctl_status, devctl_errno,
                static_cast<unsigned int>(
                    qnx_can_messages_match(tx_message, rx_message)));
    (void)::close(tx_fd);
    (void)::close(rx_fd);
    return EXIT_FAILURE;
  }
  std::printf("qnx_can_compat_smoke frame_status=ok mid=0x%08x len=%u\n",
              static_cast<unsigned int>(rx_message.mid),
              static_cast<unsigned int>(rx_message.len));
  (void)::close(tx_fd);
  (void)::close(rx_fd);
  return EXIT_SUCCESS;
}

[[nodiscard]] int
run_qnx_can_compat_startup(const int argc, const char *const argv[]) noexcept {
  auto startup = QnxCanCompatStartupConfig{};
  auto status = parse_qnx_can_compat_startup(argc, argv, startup);
  if (!status_ok(status)) {
    std::printf("qnx_can_compat_parse_status=%s\n", status_name(status));
    return EXIT_FAILURE;
  }
  auto config = McanResourceManagerConfig{};
  status = qnx_can_compat_make_resource_manager_config(startup, config);
  if (!status_ok(status)) {
    std::printf("qnx_can_compat_config_status=%s\n", status_name(status));
    return EXIT_FAILURE;
  }
  auto options = McanResourceManagerRuntimeOptions{};
  options.frame_io_enabled = startup.raw_mode;
  options.internal_loopback = startup.internal_loopback;
  options.interrupts_enabled = startup.internal_loopback;
  options.complete_tx_immediately = true;
  if (startup.irq_count > 0U && startup.irqs[0] != 0U) {
    return run_resource_manager(
        config,
        hardware_irq_resource_manager_options(startup.internal_loopback), true,
        true, startup.raw_mode, startup.listen_only);
  }
  return run_resource_manager(config, options, false, true, startup.raw_mode,
                              startup.listen_only);
}

} // namespace
} // namespace evsn::can_drivers::j784_mcan_qnx

int main(int argc, char *argv[]) {
  const auto config =
      evsn::can_drivers::j784_mcan_qnx::skeleton_default_config();
  if (argc > 1 && std::strcmp(argv[1], "--validate-config-only") == 0) {
    const auto result =
        evsn::can_drivers::j784_mcan_qnx::validate_resource_manager_skeleton(
            config);
    return evsn::can_drivers::j784_mcan_qnx::status_ok(result) ? EXIT_SUCCESS
                                                               : EXIT_FAILURE;
  }
  if (argc > 1 && std::strcmp(argv[1], "--attach-smoke") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_attach_smoke(
        config,
        evsn::can_drivers::j784_mcan_qnx::normal_resource_manager_options(),
        false, false);
  }
  if (argc > 1 && std::strcmp(argv[1], "--dry-run-hardware-config") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_dry_hardware_config(config);
  }
  if (argc > 1 &&
      std::strcmp(argv[1], "--validate-production-ownership-scope") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::
        run_validate_production_ownership_scope();
  }
  if (argc > 1 && std::strcmp(argv[1], "--probe-hardware") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_read_only_hardware_probe(
        config, false);
  }
  if (argc > 1 && std::strcmp(argv[1], "--probe-all-hardware") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_read_only_hardware_probe(
        config, true);
  }
  if (argc > 1 && std::strcmp(argv[1], "--probe-transceivers") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_transceiver_control(
        false, false, nullptr);
  }
  if (argc > 1 && std::strcmp(argv[1], "--enable-transceivers") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_transceiver_control(
        true, false, nullptr);
  }
  if (argc > 1 &&
      std::strcmp(argv[1], "--enable-transceivers-main-mcan4") == 0) {
    if (argc <= 2) {
      std::printf("missing_argument=i2c-dev\n");
      return EXIT_FAILURE;
    }
    return evsn::can_drivers::j784_mcan_qnx::run_transceiver_control(true, true,
                                                                     argv[2]);
  }
  if (argc > 2 &&
      std::strcmp(argv[1], "--internal-loopback-smoke-endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint)) {
      return EXIT_FAILURE;
    }
    return evsn::can_drivers::j784_mcan_qnx::
        run_internal_loopback_smoke_endpoint(endpoint);
  }
  if (argc > 1 && std::strcmp(argv[1], "--internal-loopback-smoke") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_internal_loopback_smoke(
        config);
  }
  if (argc > 1 &&
      std::strcmp(argv[1], "--software-loopback-resource-manager") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        config,
        evsn::can_drivers::j784_mcan_qnx::
            software_loopback_resource_manager_options(),
        false, false, false, false);
  }
  if (argc > 1 &&
      std::strcmp(argv[1],
                  "--qnx-can-compat-software-loopback-resource-manager") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        config,
        evsn::can_drivers::j784_mcan_qnx::
            software_loopback_resource_manager_options(),
        false, true, true, false);
  }
  if (argc > 2 &&
      std::strcmp(argv[1], "--hardware-loopback-resource-manager-endpoint") ==
          0) {
    auto endpoint = std::uint8_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config =
        evsn::can_drivers::j784_mcan_qnx::single_controller_config_for_endpoint(
            endpoint);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::normal_resource_manager_options(),
        true, false, false, false);
  }
  if (argc > 2 &&
      std::strcmp(argv[1],
                  "--qnx-can-compat-hardware-loopback-resource-manager-"
                  "endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config =
        evsn::can_drivers::j784_mcan_qnx::single_controller_config_for_endpoint(
            endpoint);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::normal_resource_manager_options(),
        true, true, true, false);
  }
  if (argc > 3 &&
      std::strcmp(argv[1], "--hardware-irq-resource-manager-endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    auto logical_irq = std::uint32_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint) ||
        !evsn::can_drivers::j784_mcan_qnx::parse_irq_argument(argv[3],
                                                              logical_irq)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config = evsn::can_drivers::j784_mcan_qnx::
        single_controller_irq_config_for_endpoint(endpoint, logical_irq);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::hardware_irq_resource_manager_options(
            false),
        true, false, false, false);
  }
  if (argc > 3 &&
      std::strcmp(argv[1], "--external-can-resource-manager-endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    auto logical_irq = std::uint32_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint) ||
        !evsn::can_drivers::j784_mcan_qnx::parse_irq_argument(argv[3],
                                                              logical_irq)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config = evsn::can_drivers::j784_mcan_qnx::
        single_controller_external_irq_config_for_endpoint(endpoint,
                                                           logical_irq);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::hardware_irq_resource_manager_options(
            false),
        true, false, false, false);
  }
  if (argc > 3 &&
      std::strcmp(argv[1],
                  "--hardware-irq-loopback-resource-manager-endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    auto logical_irq = std::uint32_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint) ||
        !evsn::can_drivers::j784_mcan_qnx::parse_irq_argument(argv[3],
                                                              logical_irq)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config = evsn::can_drivers::j784_mcan_qnx::
        single_controller_irq_config_for_endpoint(endpoint, logical_irq);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::hardware_irq_resource_manager_options(
            true),
        true, false, false, false);
  }
  if (argc > 3 &&
      std::strcmp(argv[1],
                  "--qnx-can-compat-hardware-irq-resource-manager-endpoint") ==
          0) {
    auto endpoint = std::uint8_t{0U};
    auto logical_irq = std::uint32_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint) ||
        !evsn::can_drivers::j784_mcan_qnx::parse_irq_argument(argv[3],
                                                              logical_irq)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config = evsn::can_drivers::j784_mcan_qnx::
        single_controller_irq_config_for_endpoint(endpoint, logical_irq);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::hardware_irq_resource_manager_options(
            false),
        true, true, true, false);
  }
  if (argc > 3 &&
      std::strcmp(argv[1],
                  "--qnx-can-compat-external-can-resource-manager-endpoint") ==
          0) {
    auto endpoint = std::uint8_t{0U};
    auto logical_irq = std::uint32_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint) ||
        !evsn::can_drivers::j784_mcan_qnx::parse_irq_argument(argv[3],
                                                              logical_irq)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config = evsn::can_drivers::j784_mcan_qnx::
        single_controller_external_irq_config_for_endpoint(endpoint,
                                                           logical_irq);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::hardware_irq_resource_manager_options(
            false),
        true, true, true, false);
  }
  if (argc > 3 &&
      std::strcmp(
          argv[1],
          "--qnx-can-compat-hardware-irq-loopback-resource-manager-endpoint") ==
          0) {
    auto endpoint = std::uint8_t{0U};
    auto logical_irq = std::uint32_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint) ||
        !evsn::can_drivers::j784_mcan_qnx::parse_irq_argument(argv[3],
                                                              logical_irq)) {
      return EXIT_FAILURE;
    }
    const auto endpoint_config = evsn::can_drivers::j784_mcan_qnx::
        single_controller_irq_config_for_endpoint(endpoint, logical_irq);
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
        endpoint_config,
        evsn::can_drivers::j784_mcan_qnx::hardware_irq_resource_manager_options(
            true),
        true, true, true, false);
  }
  if (argc > 1 &&
      std::strcmp(argv[1], "--resource-manager-client-smoke") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager_client_smoke(
        0U);
  }
  if (argc > 2 &&
      std::strcmp(argv[1], "--resource-manager-client-smoke-endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint)) {
      return EXIT_FAILURE;
    }
    return evsn::can_drivers::j784_mcan_qnx::run_resource_manager_client_smoke(
        endpoint);
  }
  if (argc > 2 &&
      std::strcmp(argv[1], "--qnx-can-compat-client-smoke-endpoint") == 0) {
    auto endpoint = std::uint8_t{0U};
    if (!evsn::can_drivers::j784_mcan_qnx::parse_endpoint_argument(argv[2],
                                                                   endpoint)) {
      return EXIT_FAILURE;
    }
    return evsn::can_drivers::j784_mcan_qnx::run_qnx_can_compat_client_smoke(
        endpoint);
  }
  if (argc > 1 && std::strcmp(argv[1], "--help") == 0) {
    std::printf("usage: dev-evsn-mcan-j784 [--validate-config-only|"
                "--attach-smoke|--dry-run-hardware-config|"
                "--validate-production-ownership-scope|"
                "--probe-hardware|--probe-all-hardware|"
                "--probe-transceivers|--enable-transceivers|"
                "--enable-transceivers-main-mcan4 <i2c-dev>|"
                "--internal-loopback-smoke|"
                "--internal-loopback-smoke-endpoint <id>|"
                "--software-loopback-resource-manager|"
                "--qnx-can-compat-software-loopback-resource-manager|"
                "--hardware-loopback-resource-manager-endpoint <id>|"
                "--qnx-can-compat-hardware-loopback-resource-manager-"
                "endpoint <id>|"
                "--hardware-irq-resource-manager-endpoint <id> <irq>|"
                "--external-can-resource-manager-endpoint <id> <irq>|"
                "--hardware-irq-loopback-resource-manager-endpoint "
                "<id> <irq>|"
                "--qnx-can-compat-hardware-irq-resource-manager-endpoint "
                "<id> <irq>|"
                "--qnx-can-compat-external-can-resource-manager-endpoint "
                "<id> <irq>|"
                "--qnx-can-compat-hardware-irq-loopback-resource-manager-"
                "endpoint <id> <irq>|"
                "--resource-manager-client-smoke|"
                "--resource-manager-client-smoke-endpoint <id>|"
                "--qnx-can-compat-client-smoke-endpoint <id>|"
                "NXP-style [-R] [-t] [-f] [-b bitrate] [-d bitrate] "
                "[canN[,irq...]]]\n");
    return EXIT_SUCCESS;
  }
  if (argc > 1) {
    constexpr auto max_startup_args = std::size_t{32U};
    std::array<const char *, max_startup_args> startup_args{};
    const auto startup_argc = argc - 1;
    if (startup_argc > static_cast<int>(startup_args.size())) {
      return EXIT_FAILURE;
    }
    for (auto index = int{0}; index < startup_argc; ++index) {
      startup_args[static_cast<std::size_t>(index)] = argv[index + 1];
    }
    if (evsn::can_drivers::j784_mcan_qnx::qnx_can_compat_startup_args_present(
            startup_argc, startup_args.data())) {
      return evsn::can_drivers::j784_mcan_qnx::run_qnx_can_compat_startup(
          startup_argc, startup_args.data());
    }
  }
  return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(
      config,
      evsn::can_drivers::j784_mcan_qnx::normal_resource_manager_options(),
      false, false, false, false);
}
