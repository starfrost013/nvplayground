/* 
    NVPlayground
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
#include <architecture/nv3/nv3_api.h>
#include <architecture/nv3/nv3_ref.h>

#include "util/util.h"
#include <stdio.h>
#include <stdlib.h>

nv3_state_t nv3_state = {0};                    // NV3 specific state 


bool nv3_init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

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
    current_device.bar1_ramin_start = bar1_base + 0xC00000;

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

    nv_mmio_write32(NV3_PRAMDAC_COEFF_SELECT, pramdac_pll_coeff_select);

    return true; 
}
