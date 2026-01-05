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
#include <string.h>
#include <util/console/console.h>


void Console_Init()
{
    if (nvplay_state.config.dumb_console)
        return;

    // Initialise PDcurses
    initscr();

    // Non-blocking colour console
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    timeout(0);
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
    clear();
    refresh();
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
        log_settings.destination &= ~(LOG_DEST_CONSOLE);

        Logging_Write(LOG_LEVEL_ERROR, "Stopped logging an unreasonable amount of stuff (Likely memory corruption!) Size was %d\n", size);

        log_settings.destination = orig; 
        return;
    }

    if (nvplay_state.config.dumb_console)
        fputs(buf, stdout);
    else
        printw(buf);  
    
    wrefresh(stdscr);
}


void Console_Update()
{
    // don't refresh the console unless needed
    bool refresh_needed = false; 

    bool scroll_up = (Input_KeyDown(SCANCODE_CHAR_UPARROW));
    bool scroll_down = (Input_KeyDown(SCANCODE_CHAR_DOWNARROW));
 
    refresh_needed = (scroll_up || scroll_down);

    if (scroll_up)
        scrl(1);
    
    if (scroll_down)
        scrl(-1);

    if (refresh_needed)
        wrefresh(stdscr);
}


void Console_Shutdown()
{
    Console_Clear();
}