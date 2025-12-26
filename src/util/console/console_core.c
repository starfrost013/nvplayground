/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    console_core.c: NVPlay2 Console system to allow a scrollable console and TUI implementation 
*/

#include "conio.h"
#include "dpmi.h"
#include "nvplay.h"
#include "util/util.h"
#include <util/console/console.h>

console_t console;

void Console_Init(size_t console_buf_size)
{
    console.size = console_buf_size;
    console.buf = calloc(1, console.size);

    // flush 1/8th of console by defualt 
    if (console.flush_amount > console.size)
        console.flush_amount = console.size - (console.size >> 3);

    Logging_Write(log_level_debug, "Console initialised: Ring buffer size %d", console_buf_size);
}

void Console_Clear()
{
    // Set video mode 
    // If we need this again turn it into its own function and library (LegacyVideo)
    __dpmi_regs regs = {0};

    regs.h.ah = INT_VIDEO_SET_MODE;
    regs.h.al = UNACCEL_VIDEO_TEXT_80_COLOR;

    __dpmi_int(INT_VIDEO, &regs);
}

void Console_Flush()
{
    // copy [flush_amount...end of buf] to [0...(end of buf-flush amount)]
    uint32_t initial = console.write_ptr;
    console.write_ptr -= console.flush_amount;
    uint32_t old_data_size = (initial - console.flush_amount);

    uint32_t diff = initial - old_data_size;

    // Try to not do this often.
    memcpy(&console.buf[0], &console.buf[console.flush_amount], old_data_size);

    // nuke leftover data 
    memset(&console.buf[old_data_size], 0x00, diff);

    if (console.read_ptr > old_data_size)
        console.read_ptr -= console.flush_amount;
    
}

void Console_PushLine(char* buf)
{
    // let's hope all the data passed into here is null terminated
    // use some heuristics
    size_t size = strlen(buf);

    if (size > MAX_REASONABLE_LOG_LENGTH)
    {
        // TODO: Do a bit of a hack so that we don't explode.
        uint32_t orig = log_settings.destination;
        log_settings.destination &= ~(log_dest_console);

        Logging_Write(log_level_error, "Stopped logging an unreasonable amount of stuff (Likely memory corruption!) Size was %d\n", size);

        log_settings.destination = orig; 
        return;
    }

    if (console.write_ptr > (console.size - size))
        Console_Flush();

    memcpy(&console.buf[console.write_ptr], buf, size);; 
    
    console.write_ptr += size; 
    
    puts(buf);
}

void Console_Shutdown()
{
    free(console.buf);
    
    // Prevent any weirdness caused by displaying a freed buffer.
    console.size = console.flush_amount = console.read_ptr = console.write_ptr = 0;
    Console_Clear();
    console.buf = NULL;

}