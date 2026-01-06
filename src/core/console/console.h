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
void Console_PushLine(char const* buf);
// need to set this so we don't have to withdraw the screen and it's just a sliding buffer
void Console_Update();
void Console_Shutdown();


// Input

// using a char may defy the scancode differences. 

#define KEYBOARD_MOD_RIGHT_SHIFT            (1 << 1)
#define KEYBOARD_MOD_LEFT_SHIFT             (1 << 2)
#define KEYBOARD_MOD_ANY_CTRL               (1 << 3)        /* Not recommended */
#define KEYBOARD_MOD_ANY_ALT                (1 << 4)
#define KEYBOARD_MOD_SCROLL_LOCK_ACTIVE     (1 << 5)
#define KEYBOARD_MOD_NUM_LOCK_ACTIVE        (1 << 6)
#define KEYBOARD_MOD_CAPS_LOCK_ACTIVE       (1 << 7)
#define KEYBOARD_MOD_LEFT_CTRL              (1 << 8)
#define KEYBOARD_MOD_LEFT_ALT               (1 << 9)
#define KEYBOARD_MOD_RIGHT_CTRL             (1 << 10)
#define KEYBOARD_MOD_RIGHT_ALT              (1 << 11)
#define KEYBOARD_MOD_SCROLL_LOCK_PRESSED    (1 << 12)
#define KEYBOARD_MOD_NUM_LOCK_PRESSED       (1 << 13)
#define KEYBOARD_MOD_CAPS_LOCK_PRESSED      (1 << 14)
#define KEYBOARD_MOD_SYSRQ                  (1 << 15)

bool Input_GetString(char* buf, uint32_t n);
bool Input_GetStringAndChar(char* buf, uint32_t n, int32_t* ch);
bool Input_KeyDown(uint8_t scancode); 
bool Input_ModState(uint16_t mod_flags);