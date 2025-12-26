/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    console.h: NVPlay2 Console system to allow a scrollable console and TUI
*/

// Default amount of scrollback

#pragma once 
#include <nvplay.h>

typedef struct console_s
{
    size_t size;                                // size of the console bufer
    size_t flush_amount;                        // flush on overflow
    uint8_t* buf;                               // console buffer

    size_t read_ptr;                            // read pointer (scrollback)
    size_t write_ptr;                           // write pointer (write)
} console_t;

#define DEFAULT_CONSOLE_SIZE        65536       // 64 KB
#define DEFAULT_CONSOLE_COLUMNS     80          // 80 columns

#define MAX_REASONABLE_LOG_LENGTH   1024

extern console_t console; 

void Console_Init(size_t console_buf_size);
void Console_Clear();
void Console_PushLine(char* buf);
// need to set this so we don't have to withdraw the screen and it's just a sliding buffer
void Console_OnKey();
void Console_Shutdown();