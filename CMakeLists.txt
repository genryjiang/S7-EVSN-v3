cmake_minimum_required(VERSION 3.20)
project(S7_EVSN LANGUAGES C CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "$ENV{NDDSHOME}/resource/cmake")
include(EVSNNodeHelpers)

if(BUILD_TESTING AND EVSN_TARGET_LINUX)
    include(FetchContent)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif()

find_package(
    RTIConnextDDS "7.0.0" REQUIRED COMPONENTS
    core
    messaging_api
)

add_subdirectory(evsn-runtime)
