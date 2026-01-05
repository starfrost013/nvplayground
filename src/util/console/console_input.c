/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    console_input.c: Input library and keyboard functions
*/

#include <bios.h>
#include <curses.h>
#include <util/util.h>
#include <util/console/console.h>

/* 
    I am lazy as hell 
    This code is non-portable, requires switching to real mode and basically ignores all non-english keyboard layouts (Sorry!)
*/

#define BIOSKEY_GET_STATE           0x10
#define BIOSKEY_GET_MODIFIERS       0x12

#define PREFIX_START                0xE0 

// keypad mode doesn't work on DOS
#define REAL_KEY_BACKSPACE          0x08

// this is a terrible idea (NOT thread-safe)
// if the last return was successful, clear the screen
bool last_return_was_successful = false; 

// Get input from the user (non-blocking)
bool Input_GetString(char* buf, uint32_t n)
{
    //error 
    if (!n)
        return false; 

    // detect if our GetString function was called using a new buf
    if (last_return_was_successful)
    {
        memset(buf, 0x00, n);
        last_return_was_successful = false; 
    }

    int32_t c = getch();

    // -1 = no key pressed
    if (c == -1)
        return false; 

    uint32_t len = strlen(buf);
    
    // we are out of space
    if (len >= (n - 1))
        return false;  

    // back space
    if (c == REAL_KEY_BACKSPACE)
    {
        if (len > 0)
        {
            buf[len - 1] = '\0';
            Console_PopChar();
        }

        return false;
    }
    else 
        Console_PushChar(c); // put everything in a centralised place

    // return the character
    buf[len] = c;

    bool ret = (c == '\n'
    || c == '\r');

    last_return_was_successful = ret; 

    return ret;
}

bool Input_KeyDown(uint8_t scancode)
{
    int32_t read_key = bioskey(BIOSKEY_GET_STATE);

    uint8_t real_scancode = (read_key >> 8) & 0xFF;

    // do some data munging
    // we use 8-bit scancodes. E0 prefix is considered to be | 0x80 by scancodes.h. Let's munge it to have the right format  
    if ((read_key & 0xFF) == PREFIX_START)
        real_scancode |= 0x80;

    return (scancode == real_scancode);
}

bool Input_ModState(uint16_t mod_flags)
{
    // guaranteed to be 16bit
    int16_t key = (int16_t)bioskey(BIOSKEY_GET_MODIFIERS);
    
    // check
    return ((key & mod_flags) != 0);
}