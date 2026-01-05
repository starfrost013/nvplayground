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


#define DEFAULT_CONSOLE_SIZE        65536       // 64 KB
#define DEFAULT_CONSOLE_ROWS        25          // 25 rows

#define MAX_REASONABLE_LOG_LENGTH   1024

void Console_Init();
void Console_Clear();
void Console_PushChar(char ch);
void Console_PopChar();
void Console_GetPosition(int32_t* cx, int32_t* cy);
void Console_PushLine(char* buf);
// need to set this so we don't have to withdraw the screen and it's just a sliding buffer
void Console_Update();
void Console_Shutdown();