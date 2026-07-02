### DO NOT TOUCH!
# Written by Ryan Wong z5417983

set(EVSN_NODE_SOURCES
    evsn_node.cpp
    parameter_service.cpp
    node_supervisor.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    include(CheckIncludeFileCXX)
    check_include_file_cxx("ha/ham.h" EVSN_NODE_HAS_QNX_HAM_HEADER)
    if(NOT EVSN_NODE_HAS_QNX_HAM_HEADER)
        message(FATAL_ERROR "QNX HAM header <ha/ham.h> is required for QNX evsn_node builds")
    endif()

    find_library(EVSN_NODE_QNX_HAM_LIBRARY NAMES ham)
    if(NOT EVSN_NODE_QNX_HAM_LIBRARY)
        message(FATAL_ERROR "QNX HAM library libham is required for QNX evsn_node builds")
    endif()

    list(APPEND EVSN_NODE_SOURCES qnx_ham_node_supervisor.cpp)
else()
    list(APPEND EVSN_NODE_SOURCES noop_node_supervisor.cpp)
endif()

add_library(evsn_node STATIC ${EVSN_NODE_SOURCES})

target_include_directories(evsn_node PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    evsn_node
    PUBLIC
    RTIConnextDDS::cpp2_api
    RTIConnextDDS::messaging_cpp2_api
    custom_types
)

if(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    target_link_libraries(evsn_node PRIVATE ${EVSN_NODE_QNX_HAM_LIBRARY})

    add_executable(qnx_evsn_node_ham_smoke
        test/qnx_evsn_node_ham_smoke.cpp
    )
    target_link_libraries(qnx_evsn_node_ham_smoke PRIVATE evsn_node)
    evsn_install_node(qnx_evsn_node_ham_smoke "")
endif()

if(BUILD_TESTING AND EVSN_TARGET_LINUX)
    add_executable(evsn_parameter_service_test
        test/parameter_service_test.cpp
    )
    target_link_libraries(evsn_parameter_service_test PRIVATE evsn_node GTest::gtest_main)
    add_test(
        NAME evsn_parameter_service_test
        COMMAND
            ${CMAKE_SOURCE_DIR}/scripts/test-parameter-service
            $<TARGET_FILE:evsn_parameter_service_test>
    )

    add_test(
        NAME evsn_node_no_mqtt_runtime
        COMMAND ${CMAKE_SOURCE_DIR}/scripts/test-no-evsnnode-mqtt
    )

    add_executable(evsn_node_supervision_test
        test/evsn_node_supervision_test.cpp
    )
    target_link_libraries(evsn_node_supervision_test PRIVATE evsn_node GTest::gtest_main)
    add_test(
        NAME evsn_node_supervision_test
        COMMAND
            ${CMAKE_SOURCE_DIR}/scripts/test-parameter-service
            $<TARGET_FILE:evsn_node_supervision_test>
    )
    set_tests_properties(
        evsn_parameter_service_test
        evsn_node_no_mqtt_runtime
        evsn_node_supervision_test
        PROPERTIES TIMEOUT 20
    )
endif()
