/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    core_detect.c: Code for detecting aspects of the system environment like the operating system and graphics hardware
*/

#include "dpmi.h"
#include "util/util.h"
#include <nvplay.h>

// The selected device after detection is done. 
nv_device_t current_device = {0}; 

#define INT_MULTIPLEX                       0x2F
#define INT_MULTIPLEX_WINDOWS_IN_MEM        0x1600
#define INT_MULTIPLEX_GET_API_ENTRY_POINT   0x1602

#define INT_MULTIPLEX_WIN386_OLD            0x01
#define INT_MULTIPLEX_WIN386_NEW            0xFF
#define INT_MULTIPLEX_XMS1                  0x80

#define INT_DOS_EXTENDED_VERSION            0x3306

//
// Determines if we are running under Windows.
// It is not possible to run under Windows/386.
// It's possible to hide it so we have to use multiple ways
//
void NVPlay_DetectWindows()
{
    __dpmi_regs regs = {0};

    // First, let's check for Windows NT using the DOS5/6 API "Get True Version Number". 5.5 or 30.00 means NTVDM
    // It also bypasses SETVER and is re-entrant
    regs.x.ax = INT_DOS_EXTENDED_VERSION;

    __dpmi_int(INT_DOS, &regs);

    if ((regs.h.bh == 0x32
    && regs.h.bl == 0x05)           // DOS 5.50
    || (regs.h.bh == 0x00  
    && regs.h.bl == 0x1E))          // DOS 30.00 (early NT)
    {
        nvplay_state.os_level = NVPLAY_OS_NT;
        return;
    }

    // Legacy windows detected. Let's detect Enhanced-Mode Windows (since this is DPMI we need at least a 386)

    // https://www.minuszerodegrees.net/websitecopies/Linux.old/docs/interrupts/int-html/rb-4495.htm
    // Could use 160Ah (which is more straightforward on 3.1+) but want to cover all abses

    regs.x.ax = INT_MULTIPLEX_WINDOWS_IN_MEM;
    __dpmi_int(INT_MULTIPLEX, &regs);

    // if it is 0, make sure we are actually in dos and that the user isn't hiding it using the PIF editor
    // https://www.fysnet.net/chkwin.htm

    if (regs.h.al == 0)
    {
        memset(&regs, 0x00, sizeof(__dpmi_regs));

        regs.x.ax = INT_MULTIPLEX_GET_API_ENTRY_POINT;
        __dpmi_int(INT_MULTIPLEX, &regs);

        if (regs.x.es == 0
        && regs.x.di == 0)
            nvplay_state.os_level = NVPLAY_OS_DOS;
        else
            nvplay_state.os_level = NVPLAY_OS_WINUNKNOWN;

        return; 
    }

    // 1600 returned a non0 value
    // so we KNOW this is windows

    switch (regs.h.al)
    {
        case INT_MULTIPLEX_XMS1: // XMS1
            nvplay_state.os_level = NVPLAY_OS_DOS;
            break;
        case INT_MULTIPLEX_WIN386_OLD:
        case INT_MULTIPLEX_WIN386_NEW:
            Logging_Write(LOG_LEVEL_MESSAGE, "Someone strapped a DPMI client to Windows/386 2.x???\n");
            break;
        default: // windows version
            if (regs.h.al == 3 && regs.h.ah == 0)
                nvplay_state.os_level = NVPLAY_OS_WIN30;
            else if (regs.h.al == 3 && regs.h.ah == 1)
                nvplay_state.os_level = NVPLAY_OS_WIN31;
            else if (regs.h.al == 4 && regs.h.ah == 0)
                nvplay_state.os_level = NVPLAY_OS_WIN95;
            else if (regs.h.al == 4 && regs.h.ah == 1)
                nvplay_state.os_level = NVPLAY_OS_WIN98;
            else if (regs.h.al == 4 && regs.h.ah == 9)
                nvplay_state.os_level = NVPLAY_OS_WINME;           
            else
                nvplay_state.os_level = NVPLAY_OS_WINUNKNOWN;   // Some ME pre-alpha builds were version5 
            break; 
    }

#ifdef DEBUG
    const char* os_string = "Operating system: DOS\n";

    switch (nvplay_state.os_level)
    {
        case NVPLAY_OS_DOS:
            break; // shut up GCC, we set it above
        case NVPLAY_OS_WINUNKNOWN:
            os_string = "Operating system: unknown Windows (or something pretending to be Windows)\n";
            break;
        case NVPLAY_OS_WIN30:
            os_string = "Operating system: Windows 3.0\n";
            break;
        case NVPLAY_OS_WIN31:
            os_string = "Operating system: Windows 3.1\n";
            break;
        case NVPLAY_OS_WIN95:
            os_string = "Operating system: Windows 95\n";
            break;
        case NVPLAY_OS_WIN98:
            os_string = "Operating system: Windows 98\n";
            break;
        case NVPLAY_OS_WINME:
            os_string = "Operating system: Windows ME\n";
            break;
        case NVPLAY_OS_NT:
            os_string = "Operating system: Windows NT-based\n";
            break;
    }

    Logging_Write(LOG_LEVEL_DEBUG, os_string);
#endif
}

bool GPU_Detect()
{
    nv_device_info_t current_device_info = supported_devices[0]; 
    int32_t i = 0; 

    // iterate through all the known device info sets
    while (current_device_info.vendor_id != 0x00)
    {
        current_device_info = supported_devices[i];
        
        Logging_Write(LOG_LEVEL_DEBUG, "Trying to find GPU: %s\n", current_device_info.name);

        for (uint32_t device_id = current_device_info.device_id_start; device_id <= current_device_info.device_id_end; device_id++)
        {
            if (PCI_DevicePresent(device_id, current_device_info.vendor_id))
            {
                Logging_Write(LOG_LEVEL_MESSAGE, "Detected GPU: %s\n", current_device_info.name);

                // set up current info
                current_device.device_info = current_device_info;
                current_device.real_device_id = device_id; // since some have multiple
                return true; 
            }
        }

        i++;
    }

    Logging_Write(LOG_LEVEL_ERROR, "A supported GPU was not found.\n");
    return false; 

}

