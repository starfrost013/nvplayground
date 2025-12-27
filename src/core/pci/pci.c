/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    pci.c: Implements wrappers around PCI BIOS functions
*/

#include "dos.h"
#include "dpmi.h"
#include "nvplay.h"
#include "util/util.h"

#include <stdint.h>

/* Discover the PCI BIOS */
bool PCI_BiosIsPresent(void) 
{ 
  __dpmi_regs regs = {0};

  regs.h.ah = PCI_FUNCTION_ID_BASE;
  regs.h.al = PCI_BIOS_PRESENT;

  __dpmi_int(INT_PCI_BIOS, &regs);

  if (regs.d.edx != PCI_BIOS_MAGIC) // "PCI "
  {
    Logging_Write(LOG_LEVEL_ERROR, "PCI BIOS not found, or PCI BIOS specification was below version 2.0c\n");
    return false;
  }

  Logging_Write(LOG_LEVEL_MESSAGE, "Found PCI BIOS, specification version %x.%x\n", regs.h.bh, regs.h.bl); // %x as a cheap way of printing it as BCD
  return true; 
}

bool PCI_DevicePresent(uint32_t device_id, uint32_t vendor_id)
{
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_FIND_DEVICE;
    regs.x.cx = device_id;
    regs.x.dx = vendor_id;

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (regs.h.ah) // non-zero = error
    {
        switch (regs.h.ah)
        {
            case PCI_ERROR_UNSUPPORTED_FUNCTION:
                Logging_Write(LOG_LEVEL_ERROR, "PCI BIOS was not specification level 2.0c or higher compatible after all\n");
                break;
            case PCI_ERROR_BAD_VENDOR_ID:
                Logging_Write(LOG_LEVEL_ERROR, "[BUG] BAD vendor id %08lX", vendor_id);
                break; 
        }

        return false; 
    }

    // store the returned bus and function number
    current_device.bus_number = regs.h.bh;
    current_device.function_number = regs.h.bl;

    return true; 
}

uint8_t PCI_ReadConfig8(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_READ_CONFIG_BYTE;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (!regs.h.ah)
        return regs.h.cl;
    else 
    {
        //todo fatal error code
        Logging_Write(LOG_LEVEL_ERROR, "FAILED to read PCI bus %lu function %lu offset %08lX info (8bit)\n", bus_number, function_number, offset);
        return 0x00;
    }
}

uint16_t PCI_ReadConfig16(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    /* Offset must be dword aligned */
    if (offset % 0x02)
    {
        Logging_Write(LOG_LEVEL_ERROR, "BUG: PCI_ReadConfig16 called with unaligned address");
        return 0x00; // it's not happening (TODO: error code)
    }
        
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_READ_CONFIG_WORD;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (!regs.h.ah)
        return regs.x.cx;
    else 
    {
        Logging_Write(LOG_LEVEL_ERROR, "FAILED to read PCI bus %lu function %lu offset %08lX info (16bit)\n", bus_number, function_number, offset);
        return 0x00;
    }
}

/* Read the config dword for the current device */
uint32_t PCI_ReadConfig32(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    /* Offset must be dword aligned. AND fucks up with 0x10 so just use mod */
    if (offset % 0x04)
    {
        Logging_Write(LOG_LEVEL_ERROR, "BUG: PCI_ReadConfig32 called with unaligned address");
        return 0x00; // it's not happening (TODO: error code)
    }
        
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_READ_CONFIG_DWORD;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (!regs.h.ah)
        return regs.d.ecx;
    else 
    {
        Logging_Write(LOG_LEVEL_ERROR, "FAILED to read PCI bus %lu function %lu offset %08lX info (32bit)\n", bus_number, function_number, offset);
        return 0x00;
    } 
}

bool PCI_WriteConfig8(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint8_t value)
{
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_WRITE_CONFIG_BYTE;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.h.cl = value;
    regs.x.di = offset;

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (!regs.h.ah)
        return false;
    else 
    {
        //todo fatal error code
        Logging_Write(LOG_LEVEL_ERROR, "FAILED to write PCI bus %lu function %lu offset %08lX info (8bit)\n", bus_number, function_number, offset);
        return true;
    }
    
    return false; // failsafe, should never happen

}

bool PCI_WriteConfig16(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint16_t value)
{
    /* Offset must be dword aligned */
    if (offset % 0x02)
    {
        Logging_Write(LOG_LEVEL_ERROR, "BUG: PCI_WriteConfig16 called with unaligned address!\n");
        return 0x00; // it's not happening (TODO: error code)
    }
        
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_WRITE_CONFIG_WORD;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.cx = value;

    regs.x.di = offset;

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (!regs.h.ah)
        return false; 
    else 
    {
        Logging_Write(LOG_LEVEL_ERROR, "FAILED to write PCI bus %lu function %lu offset %08lX info (16bit)\n", bus_number, function_number, offset);
        return true;
    }

    return false; // failsafe, should never happen
}

/* Read the config dword for the current device */
bool PCI_WriteConfig32(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint32_t value)
{
    /* Offset must be dword aligned. AND fucks up with 0x10 so just use mod */
    if (offset % 0x04)
    {
        Logging_Write(LOG_LEVEL_ERROR, "BUG: PCI_WriteConfig32 called with unaligned address!\n");
        return 0x00; // it's not happening (TODO: error code)
    }
        
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_WRITE_CONFIG_DWORD;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;
    regs.d.ecx = value; 

    __dpmi_int(INT_PCI_BIOS, &regs);

    if (!regs.h.ah)
        return false;
    else 
    {
        Logging_Write(LOG_LEVEL_ERROR, "FAILED to write PCI bus %lu function %lu offset %08lX info (32bit)\n", bus_number, function_number, offset);
        return true;
    }

    return false; // failsafe, should never happen
}