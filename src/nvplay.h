/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nvplay.h: Central include that everything uses and core definitions
*/

#pragma once
#include <bios.h>
#include <conio.h>
#include <ctype.h>
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

// #pragma once my beloved
#include <core/gpu/gpu.h>
#include <core/gpu/gpu_repl.h>
#include <core/pci/pci.h>
#include <core/script/script.h>
#include <core/tests/tests.h>
#include <util/util.h>
#include <util/console/console.h>

/* Strings */

extern const char* msg_help; 
extern const char* msg_help_script;


#define STRING_EMPTY ""
#define MSDOS_PATH_LENGTH					64			// maximum ms-dos path size is 64
#define MAX_STR								260 

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
	NVPLAY_MODE_TESTS = 1,			// Run tests from INI
	NVPLAY_MODE_SCRIPT = 2,			// Run script file
	NVPLAY_MODE_REPLAY = 3,			// Run "GPU replay" file [FUTURE]
	NVPLAY_MODE_BOOTGPU = 4,		// Initialise graphics hardware and exit
	// Should help be a mode?
	// Dry run is not a mode - it's a variant of TESTS mode, same for all tests
} nvplay_run_mode;

// globals
// TODO: move current_device here
typedef struct nvplay_state_s
{
	nvplay_os_state os_level; 
	nvplay_run_mode run_mode; 
	bool run_all_tests;             // [Test mode] Override test ini and run all tests
    bool dry_run;                   // don't run tests, but confirm the INI settings
    bool show_help;                 // Show a help message
    char reg_script_file[MAX_STR];  // The registry script file to use
    char savestate_file[MAX_STR];   // The savestate file to use
    char replay_file[MAX_STR];      // The replay file to use
} nvplay_state_t;

extern nvplay_state_t nvplay_state;

void NVPlay_ShowHelpAndExit();
void NVPlay_Shutdown(uint32_t exit_code);
void NVPlay_DetectWindows();

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
