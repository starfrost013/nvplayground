/*
    NVPlay
    Copyright © 2025-2026 starfrost


    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)
    
    nv3_tests.c: NV3 test suite implementation
*/

#include "architecture/nv1/nv1_ref.h"
#include "architecture/nv3/nv3_ref.h"
#include "nvplay.h"
#include "util/util.h"
#include <architecture/generic/nv_generic.h>
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>

// Architecture Includes
bool NVGeneric_DumpPCISpace()
{
    // read out the device id
    uint16_t vendor_id = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint16_t device_id = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint16_t command = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_COMMAND);
    uint16_t status = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_STATUS);
    uint8_t revision = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_REVISION);
    uint8_t class_id_high = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_CLASS_CODE_HIGH);
    uint16_t class_id_low = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_CLASS_CODE_LOW);
    uint8_t cache_line_size = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_CACHE_LINE_SIZE);
    uint8_t latency_timer = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_LATENCY_TIMER);
    uint8_t header_type = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_HEADER_TYPE);
    uint8_t bist = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BIST);
    uint32_t bar0 = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1 = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR1);
    uint32_t bar2 = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR2);
    uint32_t bar3 = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR3);
    uint32_t bar4 = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR4);
    uint32_t bar5 = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR5);
    uint32_t cardbus_cis_ptr = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_CARDBUS_CIS_PTR);
    uint16_t subsystem_vendor_id = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_SUBSYSTEM_VENDOR_ID);
    uint16_t subsystem_id = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_SUBSYSTEM_ID);
    uint32_t rom_bar = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_EXPANSION_ROM_BASE);
    uint8_t capabilities_ptr = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number,  PCI_CFG_OFFSET_CAPABILITIES_PTR);
    uint8_t interrupt_line = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_INTERRUPT_LINE);
    uint8_t interrupt_pin = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_INTERRUPT_PIN);
    uint8_t minimum_grant = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_MINIMUM_GRANT);
    uint8_t maximum_latency = PCI_ReadConfig8(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_MAXIMUM_LATENCY);

    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] PCI ID %04x:%04x\n", vendor_id, device_id);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Command Register %04x\n", command);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Status Register %04x\n", status);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Revision %02x\n", revision);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Class ID: %06x\n", (class_id_high << 16) | class_id_low);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Cache Line Size %02x\n", cache_line_size);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Latency Timer %02x\n", latency_timer);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Header Type %02x (should be 0)\n", header_type);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BIST %02x\n", bist);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BAR0 %08x\n", bar0);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BAR1 %08x\n", bar1);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BAR2 %08x\n", bar2);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BAR3 %08x\n", bar3);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BAR4 %08x\n", bar4);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] BAR5 %08x\n", bar5);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] CardBus CIS Pointer %04x\n", cardbus_cis_ptr);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Subsystem ID %04x:%04x\n", subsystem_vendor_id, subsystem_id);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] ROM BAR %04x\n", rom_bar);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Capabilities Pointer %02x\n", capabilities_ptr);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Interrupt Line %02x\n", interrupt_line);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Interrupt Pin %02x\n", interrupt_pin);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Minimum Grant %02x\n", minimum_grant);
    Logging_Write(LOG_LEVEL_MESSAGE, "[PCI CFG] Maximum Latency %02x\n", maximum_latency);

    return true; 
}

bool NVGeneric_DumpMMIO_NV1()
{
    // nv1 has a different setup
    Logging_Write(LOG_LEVEL_MESSAGE, "Dumping GPU PCI BAR0...\n");

    FILE* mmio_bar0 = fopen("nv1bar0.bin", "wb");

    if (!mmio_bar0)
        return false;

    uint32_t* mmio_dump_bar_buf = (uint32_t*)calloc(1, NV_MMIO_SIZE);

    if (!mmio_dump_bar_buf)
        return false; 

    /* 
        Dump all known memory regions except write-only ones and ones that crash
        We don't use nv_mmio_* because those will account for other things in the future
    */

    for (int32_t bar0_pos = 0; bar0_pos <= NV1_PCI_BAR0_SIZE; bar0_pos += 4)
    {
        // subtract nv3_flush_frequency to start at 0
        if (bar0_pos % NV_MMIO_DUMP_FLUSH_FREQUENCY == 0 
            && bar0_pos > 0) // i'm lazy
        {
            Logging_Write(LOG_LEVEL_DEBUG, "Dumped BAR0 up to: %08lX\n", bar0_pos);
            fwrite(&mmio_dump_bar_buf[(bar0_pos - NV_MMIO_DUMP_FLUSH_FREQUENCY) >> 2], NV_MMIO_DUMP_FLUSH_FREQUENCY, 1, mmio_bar0);
            fflush(mmio_bar0);

            // don't try and read out of bounds
            if (bar0_pos == NV_MMIO_SIZE)
                break;
        }

        mmio_dump_bar_buf[bar0_pos >> 2] = _farpeekl(current_device.bus_info.bar0_selector, bar0_pos);
    }

    fclose(mmio_bar0);
    free(mmio_dump_bar_buf);

    Logging_Write(LOG_LEVEL_MESSAGE, "Done!\n");

    return true; 
}

bool NVGeneric_DumpMMIO_NV3AndLater()
{
    Logging_Write(LOG_LEVEL_MESSAGE, "Dumping GPU PCI BARs (BAR0 = MMIO, BAR1 = VRAM/RAMIN)...\n");

    FILE* mmio_bar0 = fopen("nvbar0.bin", "wb");
    FILE* mmio_bar1 = fopen("nvbar1.bin", "wb");

    if (!mmio_bar0
    || !mmio_bar1)
        return false;

    // later devices have above 16mb of vram
    uint32_t vram_dump_size = NV_MMIO_SIZE;

    if (GPU_IsNV5() || GPU_IsNV10())
        vram_dump_size = NV5_MAX_VRAM_SIZE;

    uint32_t* mmio_dump_bar_buf = (uint32_t*)calloc(1, NV_MMIO_SIZE);

    if (!mmio_dump_bar_buf)
        return false; 
    
    // use ldt to read out BAR0 and BAR1 MMIO 
    // Flush every 64kb, because the real NV3 hardware may crash at some point?!

    // yep!
    bool piece_of_crap_cant_even_read_registers_without_crashing = GPU_IsNV3();

    /* 
        Dump all known memory regions except write-only ones and ones that crash
        We don't use nv_mmio_* because those will account for other things in the future
    */

    for (int32_t bar0_pos = 0; bar0_pos <= NV_MMIO_SIZE; bar0_pos += 4)
    {
        // subtract nv3_flush_frequency to start at 0
        if (bar0_pos % NV_MMIO_DUMP_FLUSH_FREQUENCY == 0 
            && bar0_pos > 0) // i'm lazy
        {
            Logging_Write(LOG_LEVEL_DEBUG, "Dumped BAR0 up to: %08lX\n", bar0_pos);
            fwrite(&mmio_dump_bar_buf[(bar0_pos - NV_MMIO_DUMP_FLUSH_FREQUENCY) >> 2], NV_MMIO_DUMP_FLUSH_FREQUENCY, 1, mmio_bar0);
            fflush(mmio_bar0);

            // don't try and read out of bounds
            if (bar0_pos == NV_MMIO_SIZE)
                break;
        }

        // skip the address if it will crash 
        if (piece_of_crap_cant_even_read_registers_without_crashing && NV3_MMIOAreaIsExcluded(bar0_pos))
        {
            mmio_dump_bar_buf[bar0_pos >> 2] = 0x4E4F4E45; // 'NONE'
        }
        else
            mmio_dump_bar_buf[bar0_pos >> 2] = _farpeekl(current_device.bus_info.bar0_selector, bar0_pos);

    }

    fclose(mmio_bar0);

    for (int32_t bar1_pos = 0; bar1_pos <= vram_dump_size; bar1_pos += 4)
    {
        if ((bar1_pos % NV_MMIO_DUMP_FLUSH_FREQUENCY == 0 
            && bar1_pos > 0))
        {
            Logging_Write(LOG_LEVEL_DEBUG, "Dumped BAR1 up to: %08lX\n", bar1_pos);
            fwrite(&mmio_dump_bar_buf[(bar1_pos - NV_MMIO_DUMP_FLUSH_FREQUENCY) >> 2], NV_MMIO_DUMP_FLUSH_FREQUENCY, 1, mmio_bar1);
            fflush(mmio_bar1);

            // don't try and read out of bounds
            if (bar1_pos == vram_dump_size)
                break;
        }

        // no excluded areas needed
        mmio_dump_bar_buf[bar1_pos >> 2] = _farpeekl(current_device.bus_info.bar1_selector, bar1_pos);

    }

    fclose(mmio_bar1);
    
    free(mmio_dump_bar_buf);

    Logging_Write(LOG_LEVEL_MESSAGE, "Done!\n");

    return true; 
}

bool NVGeneric_DumpMMIO()
{
    if (GPU_IsNV1())
        return NVGeneric_DumpMMIO_NV1();
    else
        return NVGeneric_DumpMMIO_NV3AndLater();
}

#define VBIOS_LENGTH    8192
bool NVGeneric_DumpVBIOS()
{
    Logging_Write(LOG_LEVEL_MESSAGE, "Dumping Video BIOS...");

    FILE* vbios = fopen("nvbios.bin", "wb");

    uint32_t vbios_bin[VBIOS_LENGTH];

    uint32_t generation = GPU_NV_GetGeneration();
    uint32_t base_location = 0;

    switch (generation)
    {
        case 1:
            base_location = NV1_PROM;
            break;
        case 3:
            base_location = NV3_PROM_START;
            break;
        case 4 ... 0x10:
            base_location = NV4_RAMIN_START;
            break;
    }

    for (int32_t i = 0; i < VBIOS_LENGTH; i++)
        vbios_bin[i] = NV_ReadMMIO32(base_location + i*4);

    fwrite(vbios_bin, sizeof(vbios_bin), 1, vbios);

    fclose(vbios);
    Logging_Write(LOG_LEVEL_MESSAGE, "Done!\n");

    return true; 
}


bool NVGeneric_DumpFIFO()
{
    // open a file
    char file_name[MSDOS_PATH_LENGTH] = {0};
    snprintf(file_name, MSDOS_PATH_LENGTH, "nv%lxfifo.txt", GPU_NV_GetGeneration());
    FILE* stream = fopen(file_name, "r+");

    if (!stream)
    {
        Logging_Write(LOG_LEVEL_ERROR, "Failed to open FIFO dump file %s!\n", file_name);
        return false; 
    }

    // call NVHAL function to dump hal
    if (current_device.device_info.hal->dump_fifo_to_text_file)
        current_device.device_info.hal->dump_fifo_to_text_file(stream);
    else
        Logging_Write(LOG_LEVEL_ERROR, "HAL Failure: No dump_fifo_to_text_file function for GPU %s\n", current_device.device_info.name);

    fclose(stream);

    return true; 
}

// Dump all currently loaded objects in the current channel
bool NVGeneric_DumpRAMHT()
{
    // open a file

    char file_name[MSDOS_PATH_LENGTH] = {0};
    snprintf(file_name, MSDOS_PATH_LENGTH, "nv%lxramht.txt", GPU_NV_GetGeneration());
    FILE* stream = fopen(file_name, "r+");

    if (!stream)
    {
        Logging_Write(LOG_LEVEL_ERROR, "Failed to open RAMHT dump file %s!\n", file_name);
        return false; 
    }

    // call NVHAL function to dump ramht
    if (current_device.device_info.hal->dump_ramht_to_text_file)
        current_device.device_info.hal->dump_ramht_to_text_file(stream);
    else
        Logging_Write(LOG_LEVEL_ERROR, "HAL Failure: No dump_ramht_to_text_file function for GPU %s\n", current_device.device_info.name);


    fclose(stream);

    return true; 
}

// Dump all channels that are not the current
bool NVGeneric_DumpRAMFC()
{    
    char file_name[MSDOS_PATH_LENGTH] = {0};
    snprintf(file_name, MSDOS_PATH_LENGTH, "nv%lxramfc.txt", GPU_NV_GetGeneration());
    FILE* stream = fopen(file_name, "r+");

    if (!stream)
    {
        Logging_Write(LOG_LEVEL_ERROR, "Failed to open RAMFC dump file %s!\n", file_name);
        return false; 
    }

    if (current_device.device_info.hal->dump_ramfc_to_text_file)
        current_device.device_info.hal->dump_ramfc_to_text_file(stream);
    else
        Logging_Write(LOG_LEVEL_ERROR, "HAL Failure: No dump_ramfc_to_text_file function for GPU %s\n", current_device.device_info.name);


    fclose(stream);

    return true; 
}

// Dump any errors that may have occurred 
bool NVGeneric_DumpRAMRO()
{
    char file_name[MSDOS_PATH_LENGTH] = {0};
    snprintf(file_name, MSDOS_PATH_LENGTH, "nv%lxramro.txt", GPU_NV_GetGeneration());
    FILE* stream = fopen(file_name, "r+");

    if (!stream)
    {
        Logging_Write(LOG_LEVEL_ERROR, "Failed to open RAMRO dump file %s!\n", file_name);
        return false; 
    }

    if (current_device.device_info.hal->dump_ramro_to_text_file)
        current_device.device_info.hal->dump_ramro_to_text_file(stream);
    else
        Logging_Write(LOG_LEVEL_ERROR, "HAL Failure: No dump_ramro_to_text_file function for GPU %s\n", current_device.device_info.name);

    fclose(stream);

    return true; 
}              

// The GPU really hates this test and explodes rendering and will probably also hardlock unless you are very careful
bool NVGeneric_DumpPGRAPHCache()
{
    if (GPU_IsNV10())
    {
        Logging_Write(LOG_LEVEL_ERROR, "DumpPGRAPHCache is not yet supported on NV10 because NV10 has a much more complicated and larger cache\n");
        return false; 
    }

    if (GPU_IsNV1())
    {
        Logging_Write(LOG_LEVEL_ERROR, "NV1 doesn't have on-die cache, failing\n");
        return false;
    }   
    
    char file_name[MSDOS_PATH_LENGTH] = {0};
    snprintf(file_name, MSDOS_PATH_LENGTH, "nv%lxcache.bin", GPU_NV_GetGeneration());

    FILE* stream = fopen(file_name, "rb+");

        if (!stream)
    {
        Logging_Write(LOG_LEVEL_ERROR, "Failed to open PGRAPH_CACHE dump file %s!\n", file_name);
        return false; 
    }

    if (current_device.device_info.hal->dump_cache_to_text_file)
        current_device.device_info.hal->dump_cache_to_text_file(stream);
    else
        Logging_Write(LOG_LEVEL_ERROR, "HAL Failure: No dump_cache_to_text_file function for GPU %s\n", current_device.device_info.name);

    fclose(stream);

    return true; 
}

