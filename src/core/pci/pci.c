
#include "dos.h"
#include "dpmi.h"
#include "nvplayground.h"
#include "util/util.h"
#include <core/nvcore.h>

/* Discover the PCI BIOS */
bool pci_bios_is_present(void) 
{ 
  __dpmi_regs regs = {0};

  regs.h.ah = PCI_FUNCTION_ID_BASE;
  regs.h.al = PCI_BIOS_IS_PRESENT;

  __dpmi_int(INT_1A, &regs);

  if (regs.d.edx != PCI_BIOS_MAGIC) // "PCI "
  {
    Logging_Write(log_level_error, "PCI BIOS not found, or PCI BIOS specification was below version 2.0c\n");
    return false;
  }

  Logging_Write(log_level_message, "Found PCI BIOS, specification version %x.%x\n", regs.h.bh, regs.h.bl); // %x as a cheap way of printing it as BCD
  return true; 
}

bool pci_does_device_exist(uint32_t device_id, uint32_t vendor_id)
{
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_FIND_DEVICE;
    regs.x.cx = device_id;
    regs.x.dx = vendor_id;

    __dpmi_int(INT_1A, &regs);

    if (regs.h.ah) // non-zero = error
    {
        switch (regs.h.ah)
        {
            case PCI_ERROR_UNSUPPORTED_FUNCTION:
                Logging_Write(log_level_error, "PCI BIOS was not specification level 2.0c or higher compatible after all\n");
                break;
            case PCI_ERROR_BAD_VENDOR_ID:
                Logging_Write(log_level_error, "[BUG] BAD vendor id %08lX", vendor_id);
                break; 
        }

        return false; 
    }

    // store the returned bus and function number
    current_device.bus_number = regs.h.bh;
    current_device.function_number = regs.h.bl;

    return true; 
}

uint32_t pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_READ_CONFIG_BYTE;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;

    __dpmi_int(INT_1A, &regs);

    if (!regs.h.ah)
        return regs.h.cl;
    else 
    {
        //todo fatal error code
        Logging_Write(log_level_error, "FAILED to get PCI bus %lu function %lu offset %08lX info (8bit)", bus_number, function_number, offset);
        return 0x00;
    }
}

uint32_t pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    /* Offset must be dword aligned */
    if (offset % 0x02)
    {
        Logging_Write(log_level_error, "BUG: pci_read_config_16 called with unaligned address");
        return 0x00; // it's not happening (TODO: error code)
    }
        
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_READ_CONFIG_WORD;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;

    __dpmi_int(INT_1A, &regs);

    if (!regs.h.ah)
        return regs.x.cx;
    else 
    {
        Logging_Write(log_level_error, "FAILED to get PCI bus %lu function %lu offset %08lX info (16bit)", bus_number, function_number, offset);
        return 0x00;
    }
}

/* Read the config dword for the current device */
uint32_t pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset)
{
    /* Offset must be dword aligned. AND fucks up with 0x10 so just use mod */
    if (offset % 0x04)
    {
        Logging_Write(log_level_error, "BUG: pci_read_config_32 called with unaligned address");
        return 0x00; // it's not happening (TODO: error code)
    }
        
    __dpmi_regs regs = {0};

    regs.h.ah = PCI_FUNCTION_ID_BASE;
    regs.h.al = PCI_READ_CONFIG_DWORD;
    regs.h.bh = bus_number;
    regs.h.bl = function_number;
    regs.x.di = offset;

    __dpmi_int(INT_1A, &regs);

    if (!regs.h.ah)
        return regs.d.ecx;
    else 
    {
        Logging_Write(log_level_error, "FAILED to get PCI bus %lu function %lu offset %08lX info (32bit)", bus_number, function_number, offset);
        return 0x00;
    }
        
}