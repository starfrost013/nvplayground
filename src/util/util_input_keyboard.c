#include <bios.h>
#include <util/util.h>

/* 
    I am lazy as hell 
    This code is non-portable, requires switching to real mode and basically ignores all non-english keyboard layouts (Sorry!)
*/

#define BIOSKEY_GET_STATE           0x10
#define BIOSKEY_GET_MODIFIERS       0x12

#define PREFIX_START                0xE0 

bool Input_KeyDown(uint8_t scancode)
{
    int32_t read_key = bioskey(BIOSKEY_GET_STATE);

    /* Extended keys have E0 prefix. So we have to read another key that. */
    if (read_key != PREFIX_START)
        return ((read_key >> 8) & 0xFF) == scancode; // low 8 bits are key, upper 8 bits are scancode
    else
    {
        // read again to get the real key 
        read_key = bioskey(BIOSKEY_GET_STATE);

        return ((read_key >> 8) & 0xFF) == read_key; // low 8 bits are key, upper 8 bits are scancode
    }
}

bool Input_ModState(uint16_t mod_flags)
{
    // guaranteed to be 16bit
    int16_t key = (int16_t)bioskey(BIOSKEY_GET_MODIFIERS);
    
    // check
    return ((key & mod_flags) != 0);
}