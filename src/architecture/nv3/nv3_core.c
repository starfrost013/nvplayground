/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv3_core.c: NV3:NV3/NV3T (RIVA 128/128ZX) core functions (bringup, shutdown, mainloop)
*/

// Filename: nv3_core.c
// Purpose: 
//
// Architecture Includes
#include <architecture/nv3/nv3.h>
#include <architecture/nv3/nv3_ref.h>

#include "nvplay.h"
#include "util/util.h"
#include <stdio.h>
#include <stdlib.h>

nv3_state_t nv3_state = {0};                    // NV3 specific state 

bool nv3_init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;
    bar1_base &= 0xFF000000;

    Logging_Write(log_level_debug, "NV3 - PCI BAR0 0x%08lX\n", bar0_base);
    Logging_Write(log_level_debug, "NV3 - PCI BAR1 0x%08lX\n", bar1_base);
    
    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};
    __dpmi_meminfo meminfo_bar1 = {0};

    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = NV3_MMIO_SIZE;
    meminfo_bar1.address = bar1_base;
    meminfo_bar1.size = NV3_MMIO_SIZE; //this will change

    current_device.bar1_dfb_start = bar1_base;
    current_device.ramin_start = bar1_base + NV3_RAMIN_START;

    __dpmi_physical_address_mapping(&meminfo_bar0);
    __dpmi_physical_address_mapping(&meminfo_bar1);
    
    Logging_Write(log_level_debug, "NV3 Init: Mapping BAR0 MMIO...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, NV3_MMIO_SIZE - 1);

    Logging_Write(log_level_debug, "NV3 Init: Mapping BAR1 (DFB / RAMIN)...\n");

    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar1.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, NV3_MMIO_SIZE - 1); // ultimately the same size

    /* store manufacture time configuratino */
    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV3_PMC_BOOT);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV3_PFB_BOOT);

    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV3_PFB_BOOT_RAM_AMOUNT) & 0x03; 
    bool ram_extension_8mb = (current_device.nv_pfb_boot_0 >> NV3_PFB_BOOT_RAM_EXTENSION) & 0x01;      // Needed for RIVA128 ZX
    
    /* Read in the amount of video memory from the NV_PFB_BOOT_0 register. Note: 0 without RAM_EXTENSION is 1MB!!! which was never even released... */
    if (!ram_amount_value && ram_extension_8mb == NV3_PFB_BOOT_RAM_EXTENSION_8MB)       // 8MB (RIVA 128 ZX)
        current_device.vram_amount = NV3_VRAM_SIZE_8MB;
    else if (ram_amount_value == NV3_PFB_BOOT_RAM_AMOUNT_4MB)                           // 4MB (Most RIVA 128s)
        current_device.vram_amount = NV3_VRAM_SIZE_4MB;
    else if (ram_amount_value == NV3_PFB_BOOT_RAM_AMOUNT_2MB)                           // 2MB (Single NEC card)
        current_device.vram_amount = NV3_VRAM_SIZE_2MB;
    else if (!ram_amount_value && ram_extension_8mb == NV3_PFB_BOOT_RAM_EXTENSION_NONE) // 1MB (never existed)
        current_device.vram_amount = NV3_VRAM_SIZE_1MB;

    current_device.straps = nv_mmio_read32(NV3_PSTRAPS);

    if ((current_device.straps >> NV3_PSTRAPS_CRYSTAL) & 0x01)
        current_device.crystal_hz = NV_CLOCK_BASE_14318180;
    else
        current_device.crystal_hz = NV_CLOCK_BASE_13500K;
        
    /* Power up all GPU subsystems */
    Logging_Write(log_level_debug, "NV3 Init: Enabling all GPU subsystems (0x11111111 -> NV3_PMC_ENABLE)...");
    nv_mmio_write32(NV3_PMC_ENABLE, 0x11111111);
    Logging_Write(log_level_debug, "Done!\n");

    /* Enable interrupts (test) */
    Logging_Write(log_level_debug, "NV3 Init: Enabling interrupts...");
    nv_mmio_write32(NV3_PMC_INTERRUPT_ENABLE, (NV3_PMC_INTERRUPT_ENABLE_HARDWARE | NV3_PMC_INTERRUPT_ENABLE_SOFTWARE));
    Logging_Write(log_level_debug, "Done!\n");
    
    Logging_Write(log_level_debug, "NV3 Init: Ensuring user-programmable pixel and memory clocks...\n");
    
    // ensure programmable pixel and memory clocks for driver & overclock testing
    uint32_t pramdac_pll_coeff_select = nv_mmio_read32(NV3_PRAMDAC_COEFF_SELECT);

    pramdac_pll_coeff_select |= (NV3_PRAMDAC_COEFF_SELECT_MPLL_SOURCE_SOFTWARE << NV3_PRAMDAC_COEFF_SELECT_MPLL_SOURCE);
    pramdac_pll_coeff_select |= (NV3_PRAMDAC_COEFF_SELECT_VPLL_SOURCE_SOFTWARE << NV3_PRAMDAC_COEFF_SELECT_VPLL_SOURCE);

    // read these into device_info structure
    // NV3 doesn't care what you set source to but NV4 does
    current_device.vpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_PIXEL);
    current_device.mpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_MEMORY);

    nv_mmio_write32(NV3_PRAMDAC_COEFF_SELECT, pramdac_pll_coeff_select);

    return true; 
}

bool nv3_gpus_section_applies(uint32_t fourcc)
{
    return (fourcc == gpus_section_mmio
        || fourcc == gpus_section_bar1
        || fourcc == gpus_section_cache);
}

bool nv3_gpus_parse_section(uint32_t fourcc, FILE* stream)
{
    // We only allocate one of these at once & try to avoid allocating when we don't need to
    uint32_t* mmio_base = NULL;
    uint32_t* bar1_base = NULL;
    bool section_mmio_parsed = false;
    bool section_bar1_parsed = false;

    bool valid_file = true;

    // Let's start with a basic implmenetation
    switch (fourcc)
    {
        case gpus_section_mmio:
            Logging_Write(log_level_debug, "NV3 GPUS Parser: Reading MMIO section\n");
            mmio_base = calloc(1, NV3_MMIO_SIZE);
            
            fread((void*)mmio_base, NV3_MMIO_SIZE, 1, stream);
            section_mmio_parsed = true; 
            break;
        case gpus_section_bar1:
            Logging_Write(log_level_debug, "NV3 GPUS Parser: Reading BAR1 section\n");

            bar1_base = calloc(1, NV3_MMIO_SIZE);

            // we already verified the size
            fread((void*)bar1_base, NV3_MMIO_SIZE, 1, stream);

            section_bar1_parsed = true; 
            break;
        case gpus_section_cache:
            Logging_Write(log_level_warning, "PGRAPH_CACHE!\n");
            break; 
        default:
            break;
    }

    // return if the file is not valid
    if (!valid_file)
    {
        if (mmio_base)
            free(mmio_base);

        if (bar1_base)
            free(bar1_base);

        Logging_Write(log_level_error, "Invalid file!");
        return false; 
    }

    Logging_Write(log_level_message, "NV3 GPUS Parser: Valid file. Committing writes to graphics hardware.\n"
        "WARNING: Your graphics hardware may produce an invalid signal, glitch or crash your system.\n");

    // TODO: How to disable rendering?

    // Commit to GPU
    // What does invalid MMIO WRITE do?

    if (section_mmio_parsed)
    {
        // Submit BAR1. This submits RAMIN
        Logging_Write(log_level_message, "NV3 GPUS Parser: Submitting MMIO & RAMIN\n");

        for (uint32_t addr = 0; addr < NV3_MMIO_SIZE; addr += 4)
        {
            if (addr % 0x10000 == 0)
                Logging_Write(log_level_debug, "Submitted MMIO/RAMIN up to %08x\n", addr);

            if (!nv3_mmio_area_is_excluded(addr))
                nv_mmio_write32(addr,  mmio_base[addr >> 2]);
        }
    }

    if (section_bar1_parsed)
    {
        Logging_Write(log_level_message, "NV3 GPUS Parser: Submitting BAR1\n");

        // Then MMIO    

        // We don't need to write 800000-ffffff due to the fact that this is PIO-mode submission and the state of the FIFO is knowable
        for (uint32_t addr = 0; addr < NV3_USER_START; addr += 4)
        {            
            if (addr % 0x10000 == 0)
                Logging_Write(log_level_debug, "Submitted BAR1 up to %08x\n", addr);

            nv_dfb_write32(addr, bar1_base[addr >> 2]);
        }
    }

    // if we get here
    return true; 
}