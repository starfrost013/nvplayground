#basic definitions
cmake_minimum_required(VERSION 3.26)

# Version information
set(NVPLAY_VERSION_MAJOR 1)
set(NVPLAY_VERSION_MINOR 1)
set(NVPLAY_VERSION_REVISION 0)
set(NVPLAY_VERSION_STATE "beta")

set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../src/cmake/nvplay_version.h")
set(CACHE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../src/cmake/nvplay_build.txt")

#Reading data from file + incrementation
IF(EXISTS ${CACHE_FILE})
    file(READ ${CACHE_FILE} NVPLAY_VERSION_BUILD)
    math(EXPR NVPLAY_VERSION_BUILD "${NVPLAY_VERSION_BUILD}+1")
ELSE()
    set(NVPLAY_VERSION_BUILD "1")
ENDIF()

#Update the cache
file(WRITE ${CACHE_FILE} "${NVPLAY_VERSION_BUILD}")

#Create the header
file(WRITE ${HEADER_FILE} "/* Version information (Auto-generated; modify using cmake/VersionInfo.cmake) */

#define APP_NAME \"NVPlay\"
#define APP_DESCRIPTION \"GPU research driver & raw programming tool\\nCopyright (C) 2025-2026 starfrost\\n\"
#define APP_VERSION \"${NVPLAY_VERSION_MAJOR}.${NVPLAY_VERSION_MINOR}.${NVPLAY_VERSION_REVISION}.${NVPLAY_VERSION_BUILD}\"

/* For code purposes */
#define APP_MAJOR ${NVPLAY_VERSION_MAJOR}
#define APP_MINOR ${NVPLAY_VERSION_MINOR}
#define APP_REVISION ${NVPLAY_VERSION_REVISION}
#define APP_BUILD ${NVPLAY_VERSION_BUILD}

#define APP_STATE \"${NVPLAY_VERSION_STATE}\"
#define APP_COMPILE_TIME __DATE__ \" \" __TIME__

#define APP_VERSION_FULL APP_VERSION \" (\" APP_STATE \"; \" APP_COMPILE_TIME \")\\n \"APP_DESCRIPTION 

#define APP_SIGNON_STRING APP_NAME \" Version \" APP_VERSION_FULL")