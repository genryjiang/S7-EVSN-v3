function(evsn_install_node target_name param_file)
    install(
        TARGETS ${target_name}
        RUNTIME DESTINATION bin
        COMPONENT ${target_name}
    )

    if(param_file)
        install(
            FILES ${param_file}
            DESTINATION param
            COMPONENT ${target_name}
        )
    endif()
endfunction()

function(evsn_add_gtest test_name)
    if(NOT TARGET GTest::gtest_main)
        message(FATAL_ERROR "evsn_add_gtest requires GTest::gtest_main")
    endif()

    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name} PRIVATE GTest::gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()
