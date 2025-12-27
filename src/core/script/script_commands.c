/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_script_commands.c: Implements the commands for the GPUScript parser
    Currently a basic command system with no real lexer or parser.
    Basically, you can't read anything because that would require somewhere to put i.e. variables = a full interpreted language. Not yet. So the stopgap is that we at least allow to print to the console (which doesn't have any side effects requiring a variable system)
    Although I might add a basic variable system to read to one of 10 variables as an intermediate step
    
    HEX notation only. Because it's easier...
*/

#include <stdio.h>

#include "architecture/nv3/nv3.h"
#include "architecture/nv3/nv3_ref.h"
#include <cmake/nvplay_version.h>
#include "script.h"
#include "util/util.h"
#include <nvplay.h>
#include <stdlib.h>

// bad
char** cmd_endptr;

bool Command_WriteMMIO8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteMMIO8(offset, value);
    return true; 
}

bool Command_WriteMMIORange8()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset++)
        NV_WriteMMIO8(offset, value);

    return true; 
}

bool Command_ReadMMIOConsole8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadMMIOConsole8: %02x = %02x\n", offset, value);
    return true; 
}


bool Command_WriteMMIO32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);
 
    Logging_Write(LOG_LEVEL_DEBUG, "Command_WriteMMIO32 %s:%08x %s:%08x\n", Command_Argv(1), offset, Command_Argv(2), value);

    NV_WriteMMIO32(offset, value);
    return true; 
}

bool Command_ReadMMIOConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = NV_ReadMMIO32(offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadMMIOConsole32: %08x = %08x\n", offset, value);
    return true; 
}

bool Command_WriteMMIORange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
        NV_WriteMMIO32(offset, value);

    return true; 
}

bool Command_WriteVRAM8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteDfb8(offset, value);
    return true; 
}

bool Command_WriteVRAMRange8()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset++)
        NV_WriteDfb8(offset, value);

    return true; 
}

bool Command_ReadVRAMConsole8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint8_t value = NV_ReadDfb8(offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadVRAMConsole8: %03x = %02x\n", offset, value);
    return true; 
}

bool Command_WriteVRAM16()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteDfb16(offset, value);
    return true; 
}

bool Command_WriteVRAMRange16()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 2)
        NV_WriteDfb16(offset, value);

    return true; 
}

bool Command_ReadVRAMConsole16()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint16_t value = NV_ReadDfb16(offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadVRAMConsole16: %04x = %04x\n", offset, value);
    return true; 
}

bool Command_WriteVRAM32()
{   
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteDfb32(offset, value);    
    return true; 
}

bool Command_WriteVRAMRange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
        NV_WriteDfb32(offset, value);

    return true; 
}

bool Command_ReadVRAMConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = NV_ReadDfb32(offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadVRAMConsole32: %08x = %08x\n", offset, value);
    return true; 
}

bool Command_WriteRamin32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteRamin32(offset, value);

    return true; 
}

bool Command_WriteRaminRange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
        NV_WriteRamin32(offset, value);

    return true; 
}

bool Command_ReadRaminConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = NV_ReadRamin32(offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadRaminConsole32: %08x = %08x\n", offset, value);
    return true; 
} 

bool Command_WritePCI8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    PCI_WriteConfig8(current_device.bus_number, current_device.function_number, offset, value);
    return true; 
}

bool Command_WritePCIRange8()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset++)
        PCI_WriteConfig8(current_device.bus_number, current_device.function_number, offset, value);

    return true; 
}

bool Command_ReadPCIConsole8()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint8_t value = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadPCIConsole8: %03x = %02x\n", offset, value);
    return true; 
}

bool Command_WritePCI16()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    PCI_WriteConfig16(current_device.bus_number, current_device.function_number, offset, value);
    return true; 
}

bool Command_WritePCIRange16()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 2)
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, offset, value);

    return true; 
}

bool Command_ReadPCIConsole16()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint16_t value = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadPCIConsole16: %04x = %04x\n", offset, value);
    return true; 
}

bool Command_WritePCI32()
{   
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    PCI_WriteConfig32(current_device.bus_number, current_device.function_number, offset, value);

    return true; 
}

bool Command_WritePCIRange32()
{
    uint32_t offset_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t offset_end = strtol(Command_Argv(2), cmd_endptr, 16);
    uint32_t value = strtol(Command_Argv(3), cmd_endptr, 16);

    for (uint32_t offset = offset_start; offset < offset_end; offset += 4)
        PCI_WriteConfig32(current_device.bus_number, current_device.function_number, offset, value);
    
    return true; 
}

bool Command_ReadPCIConsole32()
{
    uint32_t offset = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t value = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, offset);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadPCIConsole32: %08x = %08x\n", offset, value);
    return true; 
}

// Read a single CRTC register and print it to the console.
bool Command_ReadCrtcConsole()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_CRTC_REGISTER_NVIDIA_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_ReadCrtcConsole: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = NV_ReadCRTC(index);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadCrtcConsole: CRTC[%02x] = %02x\n", index, value);
    return true; 
}

// Write a CRTC register.
bool Command_WriteCrtc()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_CRTC_REGISTER_NVIDIA_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_WriteCrtc: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteCRTC(index, value);
    return true; 
}

// Write a range of CRTC registers.
bool Command_WriteCrtcRange()
{
    uint32_t index_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t index_end = strtol(Command_Argv(2), cmd_endptr, 16);

    if (index_start > NV3_CRTC_REGISTER_NVIDIA_END
        || index_end > NV3_CRTC_REGISTER_NVIDIA_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_WriteCrtcRange: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    for (uint32_t index = index_start; index < index_end; index++)
        NV_WriteCRTC(index, value);

    return true; 
}

// Read a single GDC register and print it to the console.
bool Command_ReadGdcConsole()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_PRMVIO_GR_INDEX_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_ReadGdcConsole: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = NV_ReadGDC(index);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadGdcConsole: GR[%02x] = %02x\n", index, value);
    return true; 
}

// Write a CRTC register.
bool Command_WriteGdc()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_PRMVIO_GR_INDEX_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_WriteGdc: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteGDC(index, value);
    return true; 
}

// Write a range of CRTC registers.
bool Command_WriteGdcRange()
{
    uint32_t index_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t index_end = strtol(Command_Argv(2), cmd_endptr, 16);

    // same for all NVs
    if (index_start > NV3_PRMVIO_GR_INDEX_END
        || index_end > NV3_PRMVIO_GR_INDEX_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_WriteGdcRange: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    for (uint32_t index = index_start; index < index_end; index++)
        NV_WriteGDC(index, value);

    return true; 
}

// Read a single CRTC register and print it to the console.
bool Command_ReadSRConsole()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_PRMVIO_SR_INDEX_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_ReadSRConsole: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = NV_ReadSequencer(index);

    Logging_Write(LOG_LEVEL_MESSAGE, "Command_ReadSRConsole: SR[%02x] = %02x\n", index, value);
    return true; 
}

// Write a CRTC register.
bool Command_WriteSR()
{
    uint32_t index = strtol(Command_Argv(1), cmd_endptr, 16);

    if (index > NV3_PRMVIO_SR_INDEX_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_WriteSR: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    NV_WriteSequencer(index, value);
    return true; 
}

// Write a range of CRTC registers.
bool Command_WriteSRRange()
{
    uint32_t index_start = strtol(Command_Argv(1), cmd_endptr, 16);
    uint32_t index_end = strtol(Command_Argv(2), cmd_endptr, 16);

    // uint, can't be below 0
    if (index_start > NV3_PRMVIO_SR_INDEX_END
        || index_end > NV3_PRMVIO_SR_INDEX_END)  
    {
        Logging_Write(LOG_LEVEL_WARNING, "Command_WriteSRRange: Ignoring invalid index %02x\n", index);
        return false; 
    }

    uint32_t value = strtol(Command_Argv(2), cmd_endptr, 16);

    for (uint32_t index = index_start; index < index_end; index++)
        NV_WriteSequencer(index, value);

    return true; 
}

// Tries to hardlock the system by writing to the Mediaport registers. This doesn't seem to work in many units (Mediaport should be Video Interface Port) or
// maybe it requires a device plugged in
bool Command_NV3Explode()
{
    // Check the pci vendor 
    if (current_device.real_device_id == PCI_DEVICE_NV3
    || current_device.real_device_id == PCI_DEVICE_NV3T_ACPI)
    {
        uint32_t pme_debug_0 = NV_ReadMMIO32(NV3_PME_DEBUG_0);
        NV_WriteMMIO32(NV3_PME_INTR, 0x11111111);

        Logging_Write(LOG_LEVEL_DEBUG, "Mediaport is alive (NV3Explode succeeded) NV3_PME_DEBUG_0=%08x\n", pme_debug_0);
    }

    return true; 
}

bool Command_RunTest()
{
    const char* test_name = Command_Argv(1);

    nv_config_test_entry_t* test_entry = Test_Get(test_name);

    if (test_entry)
        return test_entry->test->test_function();
    else
    {
        Logging_Write(LOG_LEVEL_MESSAGE, "Tried to run invalid test %s!", test_name);
        return false;
    }

    return false; //shutup compiler even though this line cannot be reached under any circumstances
}

bool Command_RunScript()
{
    NVPlay_RunScript(Command_Argv(1));
    return true; 
}

// Prints a message.
bool Command_Print()
{
    Logging_Write(LOG_LEVEL_MESSAGE, Command_Argv(1));
    return true; 
}

// Prints a message on debug builds only.
bool Command_PrintDebug()
{
    Logging_Write(LOG_LEVEL_DEBUG, Command_Argv(1));
    return true; 
}

// Prints a message on debug builds only.
bool Command_PrintWarning()
{
    Logging_Write(LOG_LEVEL_WARNING, Command_Argv(1));
    return true; 
}

// Prints a message on debug builds only.
bool Command_PrintError()
{
    Logging_Write(LOG_LEVEL_ERROR, Command_Argv(1));
    return true; 
}

bool Command_PrintVersion()
{
   	Logging_Write(LOG_LEVEL_MESSAGE, APP_SIGNON_STRING);
    return true; 
}

// Enumerates all supported commands.
gpu_script_command_t commands[] =
{    
    { "wm8", "writemmio8", Command_WriteMMIO8, 2 },
    { "rmc8", "readmmioconsole8", Command_ReadMMIOConsole8, 3 },
    { "wmrange8", "writemmiorange8", Command_WriteMMIORange8, 2 },
    { "wm32", "writemmio32", Command_WriteMMIO32, 2},
    { "wmrange32", "writemmiorange32", Command_WriteMMIORange32, 3 },
    { "rmc32", "readmmioconsole32", Command_ReadMMIOConsole32, 1 },
    { "wv8", "writevram8", Command_WriteVRAM8, 2 },
    { "rvc8", "readvramconsole8", Command_ReadVRAMConsole8, 1 },
    { "wvrange8", "writevramrange8", Command_WriteVRAMRange8, 3 },
    { "wv16", "writevram16", Command_WriteVRAM16, 2 },
    { "rvc16", "readvramconsole16", Command_ReadVRAMConsole16, 1 },
    { "wvrange16", "writevramrange16", Command_WriteVRAMRange16, 3 },
    { "wv32", "writevram32", Command_WriteVRAM32, 2 },
    { "rvc32", "readvramconsole32", Command_ReadVRAMConsole32, 1 },
    { "wvrange32", "writevramrange32", Command_WriteMMIORange32, 3 },
    { "wp8", "writepci8", Command_WritePCI8, 2 },
    { "rpc8", "readpciconsole8", Command_ReadPCIConsole8, 1 },
    { "wprange8", "writepcirange8", Command_WritePCIRange8, 3 },
    { "wp16", "writepci16", Command_WritePCI16, 2 },
    { "rpc16", "readpciconsole16", Command_ReadPCIConsole16, 1 },
    { "wprange16", "writepcirange16", Command_WritePCIRange16, 3 },
    { "wp32", "writepci32", Command_WritePCI32, 2 },
    { "rpc32", "readpciconsole32", Command_ReadPCIConsole32, 1 },
    { "wprange32", "writepcirange32", Command_WritePCIRange32, 3 },
    { "wr32", "writeramin32", Command_WriteRamin32, 2 },
    { "rrc32", "readraminconsole32", Command_ReadRaminConsole32 },
    { "wrrange32", "writeraminrange32", Command_WriteRaminRange32, 3 },
    { "rcrtcc", "readcrtcconsole", Command_ReadCrtcConsole, 1 },
    { "wcrtc", "writecrtc", Command_WriteCrtc, 2 },
    { "wcrtcrange", "writecrtcrange", Command_WriteCrtcRange, 3},
    { "rgdcc", "readgdcconsole", Command_ReadGdcConsole, 1 },
    { "wgdcc", "writegdc", Command_WriteGdc, 2 },
    { "wgdcrange", "writegdcrange", Command_WriteGdcRange, 3},
    { "rsrc", "readsrconsole", Command_ReadSRConsole, 1 },
    { "wsr", "writesr", Command_WriteSR, 2 },
    { "wsrrange", "writesrrange", Command_WriteSRRange, 3},
    { "nv3_explode", "nv3_explode", Command_NV3Explode, 0 },
    { "rt", "runtest", Command_RunTest, 1},
    { "rs", "runscript", Command_RunScript, 1},
    { "print", "printmessage", Command_Print, 1 },
    { "printdebug", "printdebug", Command_PrintDebug, 1 },
    { "printwarning", "printwarning", Command_PrintWarning, 1 },
    { "printerror", "printerror", Command_PrintError, 1 },
    { "printversion", "printversion", Command_PrintVersion, 0 },
    
    { NULL, NULL, NULL, 0 },            // Sentinel value for end of list.
};