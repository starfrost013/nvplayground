/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_script_commands.c: Implements the commands for the GPUScript parser
    Currently a basic command system with no real lexer or parser.
    Basically, you can't read anything because that would require somewhere to put i.e. variables = a full interpreted language. Not yet. So the stopgap is that we at least allow to print to the console (which doesn't have any side effects requiring a variable system)
    Although I might add a basic variable system to read to one of 10 variables as an intermediate step
    
    HEX notation only. Because it's easier...
*/

#include <stdio.h>


#include "architecture/nv3/nv3_ref.h"
#include "util/util.h"
#include <nvplay.h>
#include <config/config.h>
#include <stdlib.h>

// bad
char** cmd_endptr;

bool Command_WriteMMIO8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    nv_mmio_write8(offset, value);
    return true; 
}

bool Command_WriteMMIORange8()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset++)
    {
        nv_mmio_write8(offset, value);
    }

    return true; 
}

bool Command_ReadMMIOConsole8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    Logging_Write(log_level_message, "Command_ReadMMIOConsole8: %02x = %02x\n", offset, value);
    return true; 
}


bool Command_WriteMMIO32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);
 
    Logging_Write(log_level_debug, "Command_WriteMMIO32 %s:%08x %s:%08x\n", Command_Argv(1), offset, Command_Argv(2), value);

    nv_mmio_write32(offset, value);
    return true; 
}

bool Command_ReadMMIOConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = nv_mmio_read32(offset);

    Logging_Write(log_level_message, "Command_ReadMMIOConsole32: %08x = %08x\n", offset, value);
    return true; 
}

bool Command_WriteMMIORange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
    {
        nv_mmio_write32(offset, value);
    }

    return true; 
}

bool Command_WriteVRAM8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    nv_dfb_write8(offset, value);
    return true; 
}

bool Command_WriteVRAMRange8()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset++)
    {
        nv_dfb_write8(offset, value);
    }

    return true; 
}

bool Command_ReadVRAMConsole8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint8_t value = nv_dfb_read8(offset);

    Logging_Write(log_level_message, "Command_ReadVRAMConsole8: %03x = %02x\n", offset, value);
    return true; 
}

bool Command_WriteVRAM16()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    nv_dfb_write16(offset, value);
    return true; 
}

bool Command_WriteVRAMRange16()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 2)
    {
        nv_dfb_write16(offset, value);
    }
    
    return true; 
}

bool Command_ReadVRAMConsole16()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint16_t value = nv_dfb_read16(offset);

    Logging_Write(log_level_message, "Command_ReadVRAMConsole16: %04x = %04x\n", offset, value);
    return true; 
}

bool Command_WriteVRAM32()
{   
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    nv_dfb_write32(offset, value);    
    return true; 
}

bool Command_WriteVRAMRange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
    {
        nv_dfb_write32(offset, value);
    }
    
    return true; 
}

bool Command_ReadVRAMConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = nv_dfb_read32(offset);

    Logging_Write(log_level_message, "Command_ReadVRAMConsole32: %08x = %08x\n", offset, value);
    return true; 
}

bool Command_WriteRamin32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    nv_ramin_write32(offset, value);

    return true; 
}

bool Command_WriteRaminRange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
    {
        nv_ramin_write32(offset, value);
    }
     
    return true; 
}

bool Command_ReadRaminConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = nv_ramin_read32(offset);

    Logging_Write(log_level_message, "Command_ReadRaminConsole32: %08x = %08x\n", offset, value);
    return true; 
} 

bool Command_ReadCrtcConsole()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_CRTC_REGISTER_NVIDIA_END)  
    {
        Logging_Write(log_level_warning, "Command_ReadCrtcConsole: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = nv_crtc_read(index);

    Logging_Write(log_level_message, "Command_ReadCrtcConsole: CRTC[%02x] = %02x\n", index, value);
    return true; 
}

bool Command_WriteCrtc()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_CRTC_REGISTER_NVIDIA_END)  
    {
        Logging_Write(log_level_warning, "Command_ReadCrtcConsole: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    nv_crtc_write(index, value);
    return true; 
}

// Tries to hardlock the system by writing to the Mediaport registers. This doesn't seem to work in many units (Mediaport should be Video Interface Port) or
// maybe it requires a device plugged in
bool Command_NV3Explode()
{
    // Check the pci vendor 
    if (current_device.device_info.device_id == PCI_DEVICE_NV3
    || current_device.device_info.device_id == PCI_DEVICE_NV3T_ACPI)
    {
        uint32_t pme_debug_0 = nv_mmio_read32(NV3_PME_DEBUG_0);
        nv_mmio_write32(NV3_PME_INTR, 0x11111111);

        Logging_Write(log_level_debug, "Mediaport is alive (NV3Explode succeeded) NV3_PME_DEBUG_0=%08x\n", pme_debug_0);
    }

    return true; 
}

// Prints a message.
bool Command_Print()
{
    Logging_Write(log_level_message, Command_Argv(1));
    return true; 
}

// Prints a message on debug builds only.
bool Command_PrintDebug()
{
    Logging_Write(log_level_debug, Command_Argv(1));
    return true; 
}

// Prints a message on debug builds only.
bool Command_PrintWarning()
{
    Logging_Write(log_level_warning, Command_Argv(1));
    return true; 
}

// Prints a message on debug builds only.
bool Command_PrintError()
{
    Logging_Write(log_level_error, Command_Argv(1));
    return true; 
}


// Enumerates all supported commands.
gpu_script_command_t commands[] =
{    
    { "wm8", "writemmio8", Command_WriteMMIO8 },
    { "rmc8", "readmmioconsole8", Command_ReadMMIOConsole8 },
    { "wmrange8", "writemmiorange8", Command_WriteMMIORange8 },
    { "wm32", "writemmio32", Command_WriteMMIO32 },
    { "wmrange32", "writemmiorange32", Command_WriteMMIORange32 },
    { "rmc32", "readmmioconsole32", Command_ReadMMIOConsole32 },
    { "wv8", "writevram8", Command_WriteVRAM8 },
    { "rvc8", "readvramconsole8", Command_ReadVRAMConsole8 },
    { "wvrange8", "writevramrange8", Command_WriteVRAMRange8 },
    { "wv16", "writevram16", Command_WriteVRAM16 },
    { "rvc16", "readvramconsole16", Command_ReadVRAMConsole16 },
    { "wvrange16", "writevramrange16", Command_WriteVRAMRange16 },
    { "wv32", "writevram32", Command_WriteVRAM32 },
    { "rvc32", "readmmioconsole32", Command_ReadVRAMConsole32 },
    { "wvrange32", "writevramrange32", Command_WriteMMIORange32 },
    { "wr32", "writeramin32", Command_WriteRamin32 },
    { "rrc32", "readraminconsole32", Command_ReadRaminConsole32 },
    { "wrrange32", "writeraminrange32", Command_WriteRaminRange32 },
    { "rcrtcc", "readcrtcconsole", Command_ReadCrtcConsole },
    { "wcrtc", "writecrtc", Command_WriteCrtc },
    { "nv3_explode", "nv3_explode", Command_NV3Explode },
    { "print", "printmessage", Command_Print },
    { "printdebug", "printdebug", Command_PrintDebug },
    { "printwarning", "printwarning", Command_PrintWarning },
    { "printerror", "printwarning", Command_PrintError },
    
    { NULL, NULL, NULL},            // Sentinel value for end of list.
};