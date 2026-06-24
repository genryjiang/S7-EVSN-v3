### DO NOT TOUCH!
# Written by Ryan Wong z5417983

# Get all type sources (technically not good practice to glob but...)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cxx)

# Create lib
add_library(std_types STATIC ${SOURCES})

target_include_directories(
    std_types
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# link against the RTI dds libs
target_link_libraries(
    std_types
    PUBLIC
    RTIConnextDDS::cpp2_api
)

if(BUILD_TESTING AND EVSN_TARGET_LINUX)
    evsn_add_gtest(evsn_can20_frame_test test/evsn_can20_frame_test.cpp)
    target_link_libraries(evsn_can20_frame_test PRIVATE std_types)
    set_tests_properties(evsn_can20_frame_test PROPERTIES TIMEOUT 20)
endif()
