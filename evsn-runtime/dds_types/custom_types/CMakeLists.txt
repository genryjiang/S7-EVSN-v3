### DO NOT TOUCH!
# Written by Ryan Wong z5417983

# Get all type files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cxx)

# Temp to shut up cmake when there r no custom types
# Will remove when there are actually custom types
if(NOT SOURCES)
    add_library(custom_types INTERFACE)
    target_include_directories(
        custom_types
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    return()
endif()

# Create lib
add_library(custom_types STATIC ${SOURCES})

target_include_directories(
    custom_types
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# link against the RTI dds libs
target_link_libraries(
    custom_types
    PUBLIC
    RTIConnextDDS::cpp2_api
)

