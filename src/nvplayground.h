#pragma once

/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
*/

#include <bios.h>
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
#include <sys/nearptr.h>
#include <sys/farptr.h>

// #pragma once my beloved
#include <util/util.h>

#define APP_NAME "NVPlay"
#define APP_DESCRIPTION "Raw GPU programming for early Nvidia GPUs\n"
#define APP_VERSION "0.2.1"
#define APP_COMPILE_TIME __DATE__ " " __TIME__

#define APP_SIGNON_STRING APP_NAME " Version " APP_VERSION " (" APP_COMPILE_TIME ")\n" APP_DESCRIPTION

#define INT_1A        0x1A
