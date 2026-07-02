#ifndef __QNXNTO__
#error "resource_manager_qnx.cpp must only be built for QNX targets"
#endif

#include "evsn/can_drivers/j784_mcan_qnx/config.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/devctl_abi.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/j784_board_config.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/mcan_message_ram.hpp"
#include "evsn/can_drivers/j784_mcan_qnx/mcan_registers.hpp"

#include <array>
#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <sys/dispatch.h>
#include <sys/iofunc.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace evsn::can_drivers::j784_mcan_qnx {
namespace {

static inline constexpr auto max_registered_endpoints =
    kMaxControllers *
    ((kMaxQueuesPerController * std::size_t{2U}) + std::size_t{1U});

struct endpoint_attachment {
  iofunc_attr_t attr{};
  McanEndpointPath path{};
  int attach_id{-1};
};

struct skeleton_state {
  dispatch_t *dispatch{nullptr};
  resmgr_attr_t resmgr_attr{};
  resmgr_connect_funcs_t connect_funcs{};
  resmgr_io_funcs_t io_funcs{};
  std::array<endpoint_attachment, max_registered_endpoints> endpoints{};
  std::size_t endpoint_count{0U};
};

int unsupported_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_read_verify(ctp, msg, static_cast<iofunc_ocb_t *>(ocb), nullptr);
  if (status != EOK) {
    return status;
  }
  return ENOTSUP;
}

int unsupported_write(resmgr_context_t *ctp, io_write_t *msg,
                      RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_write_verify(ctp, msg, static_cast<iofunc_ocb_t *>(ocb), nullptr);
  if (status != EOK) {
    return status;
  }
  return ENOTSUP;
}

int unsupported_devctl(resmgr_context_t *ctp, io_devctl_t *msg,
                       RESMGR_OCB_T *ocb) {
  const auto status =
      iofunc_devctl_default(ctp, msg, static_cast<iofunc_ocb_t *>(ocb));
  if (status != _RESMGR_DEFAULT) {
    return status;
  }
  return ENOTSUP;
}

int unsupported_notify(resmgr_context_t *ctp, io_notify_t *msg,
                       RESMGR_OCB_T *ocb) {
  (void)ctp;
  (void)msg;
  (void)ocb;
  return ENOTSUP;
}

[[nodiscard]] int add_endpoint(skeleton_state &state,
                               const std::uint8_t instance_id,
                               const EndpointKind direction,
                               const std::uint8_t endpoint_id) noexcept {
  if (state.endpoint_count >= state.endpoints.size()) {
    return EOVERFLOW;
  }
  auto &endpoint = state.endpoints[state.endpoint_count];
  const auto path_result =
      make_endpoint_path(direction, instance_id, endpoint_id, endpoint.path);
  if (!status_ok(path_result)) {
    return EINVAL;
  }
  iofunc_attr_init(&endpoint.attr, S_IFCHR | 0600, nullptr, nullptr);
  endpoint.attach_id = resmgr_attach(
      state.dispatch, &state.resmgr_attr, endpoint.path.value.data(),
      _FTYPE_ANY, 0, &state.connect_funcs, &state.io_funcs, &endpoint.attr);
  if (endpoint.attach_id == -1) {
    return errno;
  }
  ++state.endpoint_count;
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
    for (auto endpoint = std::uint8_t{0U}; endpoint < instance.rx_queue_count;
         ++endpoint) {
      const auto status =
          add_endpoint(state, instance.instance_id, EndpointKind::rx, endpoint);
      if (status != EOK) {
        return status;
      }
    }
    for (auto endpoint = std::uint8_t{0U}; endpoint < instance.tx_queue_count;
         ++endpoint) {
      const auto status =
          add_endpoint(state, instance.instance_id, EndpointKind::tx, endpoint);
      if (status != EOK) {
        return status;
      }
    }
    const auto status =
        add_endpoint(state, instance.instance_id, EndpointKind::ctl, 0U);
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

[[nodiscard]] int
run_resource_manager(const McanResourceManagerConfig &config) noexcept {
  auto state = skeleton_state{};
  state.dispatch = dispatch_create();
  if (state.dispatch == nullptr) {
    return EXIT_FAILURE;
  }

  std::memset(&state.resmgr_attr, 0, sizeof(state.resmgr_attr));
  state.resmgr_attr.nparts_max = 1;
  state.resmgr_attr.msg_max_size =
      sizeof(io_devctl_t) + sizeof(McanDiagnosticsSnapshot);

  iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &state.connect_funcs,
                   _RESMGR_IO_NFUNCS, &state.io_funcs);
  state.io_funcs.read = unsupported_read;
  state.io_funcs.write = unsupported_write;
  state.io_funcs.devctl = unsupported_devctl;
  state.io_funcs.notify = unsupported_notify;

  const auto attach_status = attach_configured_endpoints(state, config);
  if (attach_status != EOK) {
    return EXIT_FAILURE;
  }

  auto *context = resmgr_context_alloc(state.dispatch);
  if (context == nullptr) {
    return EXIT_FAILURE;
  }
  for (;;) {
    context = resmgr_block(context);
    if (context == nullptr) {
      if (errno == EINTR) {
        continue;
      }
      return EXIT_FAILURE;
    }
    resmgr_handler(context);
  }
}

[[nodiscard]] int
run_attach_smoke(const McanResourceManagerConfig &config) noexcept {
  auto state = skeleton_state{};
  state.dispatch = dispatch_create();
  if (state.dispatch == nullptr) {
    return EXIT_FAILURE;
  }
  std::memset(&state.resmgr_attr, 0, sizeof(state.resmgr_attr));
  state.resmgr_attr.nparts_max = 1;
  state.resmgr_attr.msg_max_size =
      sizeof(io_devctl_t) + sizeof(McanDiagnosticsSnapshot);
  iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &state.connect_funcs,
                   _RESMGR_IO_NFUNCS, &state.io_funcs);
  state.io_funcs.read = unsupported_read;
  state.io_funcs.write = unsupported_write;
  state.io_funcs.devctl = unsupported_devctl;
  state.io_funcs.notify = unsupported_notify;
  return attach_configured_endpoints(state, config) == EOK ? EXIT_SUCCESS
                                                           : EXIT_FAILURE;
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
    return evsn::can_drivers::j784_mcan_qnx::run_attach_smoke(config);
  }
  if (argc > 1 && std::strcmp(argv[1], "--dry-run-hardware-config") == 0) {
    return evsn::can_drivers::j784_mcan_qnx::run_dry_hardware_config(config);
  }
  return evsn::can_drivers::j784_mcan_qnx::run_resource_manager(config);
}
