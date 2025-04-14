cmake_minimum_required(VERSION 3.7)
project(nvplay)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99 -save-temps)

file(GLOB sources CONFIGURE_DEPENDS 

# Main
"src/main.c"

# PCI
"src/core/pci/pci.c"

# NVCore
"src/core/nvcore_detect.c"
"src/core/nvcore_io.c"

# Architecture: NV3
"src/architecture/nv3/nv3_core.c"
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