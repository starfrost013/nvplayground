#basic definitions
cmake_minimum_required(VERSION 3.26)

set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../src/cmake/nvplay_version.h")
set(CACHE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../src/cmake/nvplay_build.txt")

#Reading data from file + incrementation
IF(EXISTS ${CACHE_FILE})
    file(READ ${CACHE_FILE} BUILD_NUMBER)
    math(EXPR BUILD_NUMBER "${BUILD_NUMBER}+1")
ELSE()
    set(BUILD_NUMBER "1")
ENDIF()

#Update the cache
file(WRITE ${CACHE_FILE} "${BUILD_NUMBER}")

#Create the header
file(WRITE ${HEADER_FILE} "/* Version information (Auto-generated; modify using cmake/VersionInfo.cmake) */

#define APP_NAME \"NVPlay\"
#define APP_DESCRIPTION \"GPU research driver & raw programming tool\\nCopyright (C) 2025-2026 starfrost\\n\"
#define APP_VERSION \"2.0.0.${BUILD_NUMBER}\"
#define APP_STATE \"pre-release\"
#define APP_COMPILE_TIME __DATE__ \" \" __TIME__

#define APP_VERSION_FULL APP_VERSION \" (\" APP_STATE \"; \" APP_COMPILE_TIME \")\\n \"APP_DESCRIPTION 

#define APP_SIGNON_STRING APP_NAME \" Version \" APP_VERSION_FULL")