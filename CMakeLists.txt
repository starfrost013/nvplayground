cmake_minimum_required(VERSION 3.7)
project(nvplay)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99 -save-temps)

file(GLOB sources CONFIGURE_DEPENDS 

# Util
"src/util/util_logging.c"
"src/util/ini.c"

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

# Architecture: NV3
"src/architecture/nv3/nv3_class_names.c"
"src/architecture/nv3/nv3_core.c"
"src/architecture/nv3/nv3_mode_table.c"
)

add_executable(nvplay ${sources})

# Base include directories
include_directories("./src")

add_custom_target(assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
add_dependencies(nvplay assets)