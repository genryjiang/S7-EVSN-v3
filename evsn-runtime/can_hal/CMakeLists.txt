set(EVSN_CAN_BACKEND "" CACHE STRING "Selected EVSN CAN HAL backend")
set_property(CACHE EVSN_CAN_BACKEND PROPERTY STRINGS fake qnx-devctl qnx-dev-can-linux tda4-fdcan)

if(EVSN_CAN_BACKEND STREQUAL "")
    if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
        set(EVSN_CAN_BACKEND "qnx-devctl" CACHE STRING "Selected EVSN CAN HAL backend" FORCE)
    else()
        set(EVSN_CAN_BACKEND "fake" CACHE STRING "Selected EVSN CAN HAL backend" FORCE)
    endif()
endif()

set(EVSN_CAN_HAL_SOURCES
    src/common/can_channel_common.cpp
    src/common/can_frame.cpp
)

if(EVSN_CAN_BACKEND STREQUAL "fake")
    list(APPEND EVSN_CAN_HAL_SOURCES src/fake/fake_can_channel.cpp)
elseif(EVSN_CAN_BACKEND STREQUAL "qnx-devctl")
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "QNX")
        message(FATAL_ERROR "EVSN_CAN_BACKEND=qnx-devctl requires a QNX CMake system")
    endif()
    include(CheckIncludeFileCXX)
    check_include_file_cxx("sys/can_dcmd.h" EVSN_CAN_HAL_HAS_QNX_CAN_DCMD)
    if(NOT EVSN_CAN_HAL_HAS_QNX_CAN_DCMD)
        message(FATAL_ERROR "EVSN_CAN_BACKEND=qnx-devctl requires QNX header <sys/can_dcmd.h>")
    endif()
    list(APPEND EVSN_CAN_HAL_SOURCES src/qnx/qnx_devctl_can_channel.cpp)
elseif(EVSN_CAN_BACKEND STREQUAL "qnx-dev-can-linux")
    message(FATAL_ERROR "EVSN_CAN_BACKEND=qnx-dev-can-linux is blocked until GPL helper usage and SIL-only extension scope are approved")
elseif(EVSN_CAN_BACKEND STREQUAL "tda4-fdcan")
    foreach(required_var
            EVSN_TDA4_CAN_INCLUDE_DIR
            EVSN_TDA4_CAN_LIBRARY
            EVSN_TDA4_CAN_DRIVER_NAME
            EVSN_TDA4_CAN_EXPECTED_ARCH)
        if(NOT DEFINED ${required_var} OR "${${required_var}}" STREQUAL "")
            message(FATAL_ERROR "EVSN_CAN_BACKEND=tda4-fdcan requires ${required_var} and target bench evidence before it can be enabled")
        endif()
    endforeach()
    message(FATAL_ERROR "EVSN_CAN_BACKEND=tda4-fdcan is not implemented until the TDA4 FD-CAN driver ABI is reviewed and approved")
else()
    message(FATAL_ERROR "Unsupported EVSN_CAN_BACKEND: ${EVSN_CAN_BACKEND}")
endif()

add_library(evsn_can_hal STATIC ${EVSN_CAN_HAL_SOURCES})
target_include_directories(
    evsn_can_hal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(evsn_can_hal PUBLIC cxx_std_17)
target_compile_definitions(
    evsn_can_hal
    PUBLIC
        EVSN_CAN_BACKEND_NAME="${EVSN_CAN_BACKEND}"
)

if(CMAKE_SYSTEM_NAME STREQUAL "QNX" AND EVSN_CAN_BACKEND STREQUAL "qnx-devctl")
    add_executable(qnx_can_hal_vcan_smoke test/qnx_can_hal_vcan_smoke.cpp)
    target_link_libraries(qnx_can_hal_vcan_smoke PRIVATE evsn_can_hal)
    target_compile_definitions(qnx_can_hal_vcan_smoke PRIVATE _POSIX_C_SOURCE=200112L)
    install(
        TARGETS qnx_can_hal_vcan_smoke
        RUNTIME DESTINATION bin
        COMPONENT qnx_can_hal_vcan_smoke
    )
endif()

if(BUILD_TESTING AND EVSN_TARGET_LINUX)
    if(NOT EVSN_CAN_BACKEND STREQUAL "fake")
        message(FATAL_ERROR "Linux CAN HAL tests require EVSN_CAN_BACKEND=fake")
    endif()

    add_executable(can_hal_validation_test test/can_hal_validation_test.cpp)
    target_link_libraries(can_hal_validation_test PRIVATE evsn_can_hal GTest::gtest_main)
    add_test(NAME can_hal_validation_test COMMAND can_hal_validation_test)

    add_executable(can_hal_fake_backend_test test/can_hal_fake_backend_test.cpp)
    target_link_libraries(can_hal_fake_backend_test PRIVATE evsn_can_hal GTest::gtest_main)
    add_test(NAME can_hal_fake_backend_test COMMAND can_hal_fake_backend_test)

    add_test(
        NAME can_hal_public_boundary_test
        COMMAND
            bash
            ${CMAKE_CURRENT_SOURCE_DIR}/test/check_public_boundary.sh
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set_tests_properties(
        can_hal_validation_test
        can_hal_fake_backend_test
        can_hal_public_boundary_test
        PROPERTIES TIMEOUT 20
    )
endif()
