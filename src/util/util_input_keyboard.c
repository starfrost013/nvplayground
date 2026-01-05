#include <bios.h>
#include <curses.h>
#include <util/util.h>

/* 
    I am lazy as hell 
    This code is non-portable, requires switching to real mode and basically ignores all non-english keyboard layouts (Sorry!)
*/

#define BIOSKEY_GET_STATE           0x10
#define BIOSKEY_GET_MODIFIERS       0x12

#define PREFIX_START                0xE0 

// Get input from the user (non-blocking)
bool Input_GetString(char* buf, uint32_t n)
{
    //error 
    if (!n)
        return false; 

    if (!kbhit())
        return false; 

    char c = getch();

    uint32_t len = strlen(buf);
    
    // we are out of space
    if (len >= (n - 1))
        return false;  

    buf[n] = c;

    return (c == '\n'
    || c == '\r');
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