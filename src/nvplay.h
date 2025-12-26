/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nvplay.h: Central include that everything uses.
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

/* Core includes here */
/* Strings */

extern const char* msg_help; 
extern const char* msg_help_script;

void NVPlay_ShowHelpAndExit();
void NVPlay_Shutdown(uint32_t exit_code);
void NVPlay_DetectWindows();

#define STRING_EMPTY ""
#define MSDOS_PATH_LENGTH					64			// maximum ms-dos path size is 64

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
