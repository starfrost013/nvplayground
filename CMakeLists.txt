cmake_minimum_required(VERSION 3.7)
project(nvplay)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99 -save-temps)

file(GLOB sources CONFIGURE_DEPENDS 

# Util
"src/util/ini.c"
"src/util/util_cmdline.c"
"src/util/util_logging.c"
"src/util/util_string.c"

# Main
"src/main.c"

# PCI
"src/core/pci/pci.c"

# Config
"src/config/config.c"

# NVCore
"src/core/gpu_list.c"
"src/core/gpu_detect.c"
"src/core/gpu_io.c"

# NVCore: Tests
"src/core/tests/tests.c"

# NVCore: Script Engine
"src/core/script/gpu_script_commands.c"
"src/core/script/gpu_script_parser.c"

# NVCore: GPU Savestate format
"src/core/formats/formats_gpus.c"

# Architecture: NV1
"src/architecture/nv1/nv1_core.c"

# Architecture: NV3
"src/architecture/nv3/nv3_class_names.c"
"src/architecture/nv3/nv3_mode_table.c"
"src/architecture/nv3/nv3_core.c"
"src/architecture/nv3/tests/nv3_tests.c"
"src/architecture/nv3/driver/2d/nv3_driver_main.c"

)

add_executable(nvplay ${sources})

# Base include directories
include_directories("./src")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  target_compile_definitions(nvplay PRIVATE DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_compile_definitions(nvplay PRIVATE RELEASE)
endif()

add_custom_target(assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
add_dependencies(nvplay assets)