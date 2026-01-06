/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nvplay.h: Central include that everything uses and core definitions
*/

#pragma once
#include <bios.h>
#include <ctype.h>
#include <curses.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/nearptr.h>
#include <sys/farptr.h>


/* Strings */

extern const char* msg_help; 
extern const char* msg_help_script;

#define STRING_EMPTY ""
#define MSDOS_PATH_LENGTH					64			// maximum ms-dos path size is 64
#define MAX_STR								260 

// #pragma once my beloved
#include <core/gpu/gpu.h>
#include <core/gpu/gpu_repl.h>
#include <core/pci/pci.h>
#include <core/script/script.h>
#include <util/util.h>
#include <util/util_ini.h>
#include <core/console/console.h>

// Operating systems that nvplay runs on.
// (Windows/386 could theoretically run DJGPP, but it is not DPMI aware)
typedef enum nvplay_os_state_e
{
	NVPLAY_OS_DOS = 0,				// MS-DOS box
	NVPLAY_OS_WIN30 = 1,			// Windows 3.0 enhanced mode
	NVPLAY_OS_WIN31 = 2,			// Windows 3.1 enhanced mode
	NVPLAY_OS_WIN95 = 3,			// Windows 95 (4.00 = 950, 4.03 = OSR2)
	NVPLAY_OS_WIN98 = 4,			// Windows 98 (4.10)
	NVPLAY_OS_WINME = 5,			// Windows ME (4.90)
	// Failsafe - you can turn off 2F/1600 which means you can't detect the Windows version. 
	// We have to assume as late as possible except NT (you can't do it on NT)
	NVPLAY_OS_WINUNKNOWN = 6,			
	// TODO: How to detect NTVDM version? It must be possible
	NVPLAY_OS_NT = 7,				// Windows NT NTVDM
} nvplay_os_state;

// Run mode. What should we do?
typedef enum nvplay_run_mode_e
{
	NVPLAY_MODE_REPL = 0,			// Enter REPL loop
	NVPLAY_MODE_SCRIPT = 1,			// Run script file
	NVPLAY_MODE_REPLAY = 2,			// Run "GPU replay" file [FUTURE]
	NVPLAY_MODE_BOOTGPU = 3,		// Initialise graphics hardware and exit
	NVPLAY_MODE_HELP = 4,			// Print help and exit
	// Should help be a mode?
	// Dry run is not a mode - it's a variant of TESTS mode, same for all tests
} nvplay_run_mode;

/* Configuration stuff */

#define INI_FILE_NAME               "nvplay.ini"
#define MAX_TEST_NAME_BUFFER_LEN    64

/* Defines a single test (for the global list of tests) */
typedef struct nv_test_s 
{
    uint32_t required_vendor_id;
    uint32_t required_device_id;
    const char* name;
    const char* name_friendly;                          // Name presented to the user.
    bool (*test_function)(); 
} nv_test_t; 

extern nv_test_t nv_tests[];

// This is a linked list.
// We can make a lot of simplifications by making some assumptions about or design; 
// e.g. we don't need to ever remove these since the enabled tests are enumerated at init.

// Holds information about tests that need to be run.
typedef struct nv_config_test_entry_s
{
    nv_test_t* test; 
    struct nv_config_test_entry_s* prev; 
    struct nv_config_test_entry_s* next; 
} nv_config_test_entry_t; 

//
// FUNCTIONS
//

bool Test_IsAvailableForGPU(const char* test_name);
nv_config_test_entry_t* Test_Get(const char* test_name);             // Get a test

bool Test_Run(nv_config_test_entry_t* test_entry);


// Main config struct (Should this be moved to nvplay.h?) 
typedef struct nv_config_s
{
    bool loaded;                                    // Determines if the configuration file has been loaded.
    ini_t ini_file;
    nv_config_test_entry_t* test_list_head;         // The first test entry.
    nv_config_test_entry_t* test_list_tail;         // The last test entry.
    uint32_t num_tests_enabled;                     // The number of enabled tests.

    // Debug settings
    bool nv10_always_map_128m;                      // NV1x: Always map 128MB
	bool dumb_console;								// Use dumb console
} nv_config_t;

bool Config_Load();

// globals
// TODO: move current_device here
typedef struct nvplay_state_s
{
	nvplay_os_state os_level; 
	nvplay_run_mode run_mode; 
    char reg_script_file[MAX_STR];  // The registry script file to use
    char savestate_file[MAX_STR];   // The savestate file to use
    char replay_file[MAX_STR];      // The replay file to use
	nv_config_t config;				// The configuration information loaded frromt he INI file
    WINDOW* window;                 // Curses window

} nvplay_state_t;

extern nvplay_state_t nvplay_state;

void NVPlay_ShowHelpAndExit();
void NVPlay_Shutdown(uint32_t exit_code);
void NVPlay_DetectOS();

//cannot use int32_t because it's defined as long
bool NVPlay_ParseCmdline(int argc, char** argv);

/* Exit codes (8 bit  only for MS-DOS )*/ 
#define NVPLAY_EXIT_CODE_SUCCESS			0			// Normal exit	
#define NVPLAY_EXIT_CODE_NO_PCI				1			// Computer too old - No PCI BIOS 2.0
#define NVPLAY_EXIT_CODE_UNSUPPORTED_GPU	2			// Unsupported GPU
#define NVPLAY_EXIT_CODE_CONFIG_LOAD_FAIL	3			// Configuration load failure
#define NVPLAY_EXIT_CODE_UNIMPLEMENTED_GPU	4			// GPU will be supported in the future
#define NVPLAY_EXIT_CODE_NO_GPU_INIT		5			// No GPU initialisation function
#define NVPLAY_EXIT_CODE_NO_TESTS			6			// Nothing to do
#define NVPLAY_EXIT_CODE_LOG_INIT_FAIL		7			// Failed to initialise logging engine
#define NVPLAY_EXIT_CODE_HELP_MENU			8			// Help menu	
#define NVPLAY_EXIT_CODE_UNSUPPORTED_OS		9			// Operating system not supported
#define NVPLAY_EXIT_CODE_UNKNOWN_FATAL		0x67		// 6-7

/* Video & System BIOS */

#define INT_VIDEO							0x10		// VBIOS Services
#define INT_VIDEO_SET_MODE					0x00

#define INT_PCI_BIOS        				0x1A		// PCI BIOS interrupt 
#define INT_DOS								0x21		// MS-DOS API
