# 
#   NVPlay
#   Copyright © 2025-2026 starfrost
#
#   Raw GPU programming for early Nvidia GPUs
#   Licensed under the MIT license (see license file)
#
#   CMakeLists.txt: Core buildscript
#

cmake_minimum_required(VERSION 3.26)
project(nvplay)

set(CMAKE_GENERATOR Ninja)

set(NVPLAY_OUTPUT_FILE_NAME "${CMAKE_PROJECT_NAME}.exe")
set(NVPLAY_CONFIG_FILE_NAME "${CMAKE_PROJECT_NAME}.ini")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99 -save-temps)

file(GLOB sources CONFIGURE_DEPENDS 

# Util
"src/util/util_ini.c"
"src/util/util_logging.c"
"src/util/util_string.c"


# Main
"src/main.c"
"src/main_help.c"

# TUI engine (The layer between nvplay <--> pdcurses)
"src/core/console/console_core.c"
"src/core/console/console_input.c"

# PCI
"src/core/pci/pci.c"

# Config
"src/config/config.c"

# Core
"src/core/core_cmdline.c"
"src/core/core_detect.c"

"src/core/gpu/gpu_list.c"
"src/core/gpu/gpu_io.c"
"src/core/gpu/gpu_repl.c"

# Core: Tests
"src/core/tests/tests.c"

# Core: Script Engine
"src/core/script/script_commands.c"
"src/core/script/script_help.c"
"src/core/script/script_parser.c"

# Architecture: kernel/Shared

# Resource Manager
"src/architecture/nvidia/kernel/core/kernel.c"
"src/architecture/nvidia/kernel/fifo/fifo.c"
"src/architecture/nvidia/kernel/graph/graph.c"

"src/architecture/nvidia/kernel/nv_generic_hal.c"
"src/architecture/nvidia/kernel/nv_generic_tests.c"
"src/architecture/nvidia/kernel/nvrm_class_names.c"

# Architecture: NV1
"src/architecture/nvidia/nv1/nv1_core.c"
"src/architecture/nvidia/nv1/nv1_class_names.c"

# Architecture: NV3
"src/architecture/nvidia/nv3/nv3_class_names.c"
"src/architecture/nvidia/nv3/nv3_mode_table.c"
"src/architecture/nvidia/nv3/nv3_core.c"
"src/architecture/nvidia/nv3/tests/nv3_tests.c"

# Architecture: NV4/5  
"src/architecture/nvidia/nv4/nv4_core.c"
"src/architecture/nvidia/nv4/nv4_fifo.c"
"src/architecture/nvidia/nv4/nv4_graph.c"

# Architecture: Celsius
"src/architecture/nvidia/nv10/nv10_core.c"

# Architecture: S3 ViRGE (Test)
"src/architecture/virge/virge_core.c"

# Architecture: Cirrus GD5446 (Test)
"src/architecture/cirrus/alpine_core.c"
)

add_executable(nvplay ${sources})

# Base include directories
include_directories("./src")
include_directories("./external/pdcurses")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  target_compile_definitions(nvplay PRIVATE DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_compile_definitions(nvplay PRIVATE RELEASE)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
endif()

# create required folders
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out/assets)

# increment the build number
add_custom_command(TARGET nvplay PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -P "${PROJECT_SOURCE_DIR}/src/cmake/VersionInfo.cmake"
)

# Libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/pdcurses)

# copy assets
add_custom_command(TARGET nvplay POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/${NVPLAY_OUTPUT_FILE_NAME}
  ${CMAKE_BINARY_DIR}/out/${NVPLAY_OUTPUT_FILE_NAME}
)

add_custom_command(TARGET nvplay POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/out/assets
)

add_custom_command(TARGET nvplay POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/out/${NVPLAY_CONFIG_FILE_NAME}
  ${CMAKE_BINARY_DIR}/${NVPLAY_CONFIG_FILE_NAME}
)

# delete duplicate nvplay.ini (it would be very stupid to move it to the root folder)
add_custom_command(TARGET nvplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/assets/${NVPLAY_CONFIG_FILE_NAME})

target_link_libraries(nvplay PRIVATE PDCurses)