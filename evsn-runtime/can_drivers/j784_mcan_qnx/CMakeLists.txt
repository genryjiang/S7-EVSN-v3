add_library(
    evsn_j784_mcan_qnx
    STATIC
        src/config.cpp
        src/devctl_abi.cpp
        src/j784_board_config.cpp
        src/mcan_core.cpp
        src/mcan_message_ram.cpp
        src/mcan_registers.cpp
        src/qnx_mmio_mapper.cpp
)

target_include_directories(
    evsn_j784_mcan_qnx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(evsn_j784_mcan_qnx PUBLIC cxx_std_17)

if(BUILD_TESTING AND EVSN_TARGET_LINUX)
    add_executable(
        j784_mcan_qnx_board_config_test
        test/j784_mcan_qnx_board_config_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_board_config_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_board_config_test COMMAND j784_mcan_qnx_board_config_test)

    add_executable(
        j784_mcan_qnx_config_test
        test/j784_mcan_qnx_config_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_config_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_config_test COMMAND j784_mcan_qnx_config_test)

    add_executable(
        j784_mcan_qnx_devctl_abi_test
        test/j784_mcan_qnx_devctl_abi_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_devctl_abi_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_devctl_abi_test COMMAND j784_mcan_qnx_devctl_abi_test)

    add_executable(
        j784_mcan_qnx_core_test
        test/j784_mcan_qnx_core_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_core_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_core_test COMMAND j784_mcan_qnx_core_test)

    add_executable(
        j784_mcan_qnx_message_ram_test
        test/j784_mcan_qnx_message_ram_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_message_ram_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_message_ram_test COMMAND j784_mcan_qnx_message_ram_test)

    add_executable(
        j784_mcan_qnx_mmio_mapper_test
        test/j784_mcan_qnx_mmio_mapper_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_mmio_mapper_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_mmio_mapper_test COMMAND j784_mcan_qnx_mmio_mapper_test)

    add_executable(
        j784_mcan_qnx_registers_test
        test/j784_mcan_qnx_registers_test.cpp
    )
    target_link_libraries(j784_mcan_qnx_registers_test PRIVATE evsn_j784_mcan_qnx GTest::gtest_main)
    add_test(NAME j784_mcan_qnx_registers_test COMMAND j784_mcan_qnx_registers_test)

    add_test(
        NAME j784_mcan_qnx_private_boundary_test
        COMMAND
            bash
            ${CMAKE_CURRENT_SOURCE_DIR}/test/check_private_boundary.sh
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_test(
        NAME j784_mcan_qnx_no_ipc_path_test
        COMMAND
            bash
            ${CMAKE_CURRENT_SOURCE_DIR}/test/check_no_ipc_path.sh
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_test(
        NAME j784_mcan_qnx_no_gpl_source_copy_test
        COMMAND
            bash
            ${CMAKE_CURRENT_SOURCE_DIR}/test/check_no_gpl_source_copy.sh
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set_tests_properties(
        j784_mcan_qnx_board_config_test
        j784_mcan_qnx_config_test
        j784_mcan_qnx_core_test
        j784_mcan_qnx_devctl_abi_test
        j784_mcan_qnx_message_ram_test
        j784_mcan_qnx_mmio_mapper_test
        j784_mcan_qnx_no_gpl_source_copy_test
        j784_mcan_qnx_no_ipc_path_test
        j784_mcan_qnx_private_boundary_test
        j784_mcan_qnx_registers_test
        PROPERTIES TIMEOUT 20
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    add_executable(dev-evsn-mcan-j784 src/resource_manager_qnx.cpp)
    target_link_libraries(dev-evsn-mcan-j784 PRIVATE evsn_j784_mcan_qnx)
    evsn_install_node(dev-evsn-mcan-j784 "")
endif()
