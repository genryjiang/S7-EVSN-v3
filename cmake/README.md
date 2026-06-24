# CMake Helpers

This directory contains shared CMake modules used by the S7_EVSN build.

## Contents

- `EVSNNodeHelpers.cmake`
	- Shared helper functions used by node package `CMakeLists.txt` files.

## How It Is Loaded

The root build file adds this directory to `CMAKE_MODULE_PATH` and includes the helper module:

```cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(EVSNNodeHelpers)
```

This means helper functions defined here are available in subdirectories such as
runtime packages under `evsn-runtime/` and future node packages under `src/`.

## Provided Function

### `evsn_install_node(target_name param_file)`

Installs a node executable and (optionally) its parameter file.

Behavior:
- Always installs the executable target to `deploy/bin` via `RUNTIME DESTINATION bin`.
- Uses the target name as the install component.
- If `param_file` is non-empty, installs it to `deploy/param`.

Function definition is in `EVSNNodeHelpers.cmake`.

## Typical Usage In A Node Package

```cmake
set(TARGET_NAME my_node)

add_executable(${TARGET_NAME}
		src/main.cpp
		src/my_node.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE include)
target_link_libraries(${TARGET_NAME} PRIVATE evsn_node std_types)

evsn_install_node(${TARGET_NAME} param/${TARGET_NAME}_param.toml)
```

If your node has no param file yet or doesn't need one, pass an empty string:

```cmake
evsn_install_node(${TARGET_NAME} "")
```

### `evsn_add_gtest(test_name sources...)`

Adds a GoogleTest executable, links it against `GTest::gtest_main`, and
registers it with CTest under the same name. Use this for deterministic unit
tests in package `test/` directories.

Typical usage:

```cmake
if(BUILD_TESTING AND EVSN_TARGET_LINUX)
	evsn_add_gtest(my_node_test test/my_node_test.cpp)
	target_link_libraries(my_node_test PRIVATE my_node_library)
	set_tests_properties(my_node_test PROPERTIES TIMEOUT 20)
endif()
```

CTest remains the test runner used by `./scripts/verify`; GoogleTest is the
unit-test framework inside each test executable.

## Notes For This Repository

- `evsn-runtime/CMakeLists.txt` auto-discovers runtime modules and DDS type libraries.
- Keep per-node build logic in each package `CMakeLists.txt`; keep shared install behavior in this helper module.
