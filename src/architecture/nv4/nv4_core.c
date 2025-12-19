/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4_core.c: NV4/NV5 (TNT series) initialisation and shutdown
*/

#include "nv4_ref.h"
#include "nvplay.h"

#include <architecture/nv4/nv4.h>

// Globals
nv4_state_t nv4_state = {0};

bool NV4_Init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;
    bar1_base &= 0xFF000000;

    Logging_Write(log_level_debug, "NV4 - PCI BAR0 0x%08lX\n", bar0_base);
    Logging_Write(log_level_debug, "NV4 - PCI BAR1 0x%08lX\n", bar1_base);
    
    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};
    __dpmi_meminfo meminfo_bar1 = {0};

    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = NV4_MMIO_SIZE;
    meminfo_bar1.address = bar1_base;
    meminfo_bar1.size = NV4_MMIO_SIZE; //this will change

    current_device.bar1_dfb_start = bar1_base;
    current_device.ramin_start = bar0_base + NV4_RAMIN_START;

    __dpmi_physical_address_mapping(&meminfo_bar0);
    __dpmi_physical_address_mapping(&meminfo_bar1);
    
    Logging_Write(log_level_debug, "NV4 Init: Mapping BAR0 (MMIO + RAMIN)...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, NV4_MMIO_SIZE - 1);

    Logging_Write(log_level_debug, "NV4 Init: Mapping BAR1 (VRAM aperture)...\n");

    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar1.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, NV4_MMIO_SIZE - 1); // ultimately the same size

    /* store manufacture time configuratino */
    current_device.nv_pmc_boot_0 = NV_ReadMMIO32(NV4_PMC_BOOT);
    current_device.nv_pfb_boot_0 = NV_ReadMMIO32(NV4_PFB_BOOT);

    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV4_PFB_BOOT_RAM_AMOUNT) & 0x03; 
    
    /* Read in the amount of video memory from the NV_PFB_BOOT_0 register. Note: 0 without RAM_EXTENSION is 1MB!!! which was never even released... */
    if (!ram_amount_value)                                                              // 8MB (RIVA 128 ZX)
        current_device.vram_amount = NV4_VRAM_SIZE_2MB;
    else if (ram_amount_value == NV4_PFB_BOOT_RAM_AMOUNT_4MB)                           // 4MB (Never released)
        current_device.vram_amount = NV4_VRAM_SIZE_4MB;
    else if (ram_amount_value == NV4_PFB_BOOT_RAM_AMOUNT_8MB)                           // 8MB
        current_device.vram_amount = NV4_VRAM_SIZE_8MB;
    else if (ram_amount_value == NV4_PFB_BOOT_RAM_AMOUNT_16MB)                          // 16Mb
        current_device.vram_amount = NV4_VRAM_SIZE_16MB;

    current_device.straps = NV_ReadMMIO32(NV4_PSTRAPS);

    if ((current_device.straps >> NV4_PSTRAPS_CRYSTAL) & 0x01)
        current_device.crystal_hz = NV_CLOCK_BASE_14318180;
    else
        current_device.crystal_hz = NV_CLOCK_BASE_13500K;

    /* Power up all GPU subsystems */
    Logging_Write(log_level_debug, "NV4 Init: Enabling all GPU subsystems (0x11111111 -> NV4_PMC_ENABLE)...");
    NV_WriteMMIO32(NV4_PMC_ENABLE, 0x11111111);
    Logging_Write(log_level_debug, "Done!\n");

    /* Enable interrupts (test) */
    Logging_Write(log_level_debug, "NV4 Init: Enabling interrupts...");
    NV_WriteMMIO32(NV4_PMC_INTR_EN, (NV4_PMC_INTR_EN_HARDWARE | NV4_PMC_INTR_EN_SOFTWARE));
    Logging_Write(log_level_debug, "Done!\n");
    
    Logging_Write(log_level_debug, "NV4 Init: Ensuring user-programmable pixel, core and memory clocks...\n");
    
    // ensure programmable pixel and memory clocks for driver & overclock testing
    // we also need to actually *program* the clocks on NV4 if you set the PLL to programmable omde it seems. It really doesn't like it if you don't
    // so store the old values

    current_device.vpll = NV_ReadMMIO32(NV4_PRAMDAC_CLOCK_PIXEL);
    current_device.mpll = NV_ReadMMIO32(NV4_PRAMDAC_CLOCK_MEMORY);
    current_device.nvpll = NV_ReadMMIO32(NV4_PRAMDAC_CLOCK_CORE);

    uint32_t pramdac_pll_coeff_select = NV_ReadMMIO32(NV4_PRAMDAC_COEFF_SELECT);

    // Save original NV4 PLL setting. 
    nv4_state.original_pll_setting = pramdac_pll_coeff_select;

    pramdac_pll_coeff_select |= (NV4_PRAMDAC_COEFF_SELECT_ALL_SOFTWARE << NV4_PRAMDAC_COEFF_SELECT_SOURCE);

    NV_WriteMMIO32(NV4_PRAMDAC_COEFF_SELECT, pramdac_pll_coeff_select);

    //go
    NV_WriteMMIO32(NV4_PRAMDAC_CLOCK_CORE, current_device.nvpll);
    NV_WriteMMIO32(NV4_PRAMDAC_CLOCK_MEMORY, current_device.mpll);
    NV_WriteMMIO32(NV4_PRAMDAC_CLOCK_PIXEL, current_device.vpll);


    return true; 
}

bool NV4_DumpMFGInfo()
{
        /* TODO: Read our Dumb Framebuffer */
    Logging_Write(log_level_message, "NV4 Manufacture-Time Configuration: \n");
    Logging_Write(log_level_message, "NV_PMC_BOOT_0           = %08lX\n", current_device.nv_pmc_boot_0);
    Logging_Write(log_level_message, "NV_PFB_BOOT_0           = %08lX\n", current_device.nv_pfb_boot_0);
    /* 
        Determine the amount of Video RAM 
        In theory this could be a shared function between all nv gpus, but in reality i'm not so sure
    */

    Logging_Write(log_level_message, "Video RAM Size          = %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read in the straps */
    Logging_Write(log_level_message, "Straps                  = %08lX\n", current_device.straps);

    // We store these but read the current values
    uint32_t vpll = NV_ReadMMIO32(NV4_PRAMDAC_CLOCK_PIXEL);
    uint32_t nvpll = NV_ReadMMIO32(NV4_PRAMDAC_CLOCK_CORE);
    uint32_t mpll = NV_ReadMMIO32(NV4_PRAMDAC_CLOCK_MEMORY);

    double vpll_mhz = NV_ClockMNPToMhz(current_device.crystal_hz, vpll);
    double nvpll_mhz = NV_ClockMNPToMhz(current_device.crystal_hz, nvpll);
    double mpll_mhz = NV_ClockMNPToMhz(current_device.crystal_hz, mpll);

    //todo: convert to MHz
    Logging_Write(log_level_message, "Pixel Clock Coefficient = %08lX (%.2f MHz)\n", vpll, vpll_mhz);
    Logging_Write(log_level_message, "Core Clock Coefficient  = %08lX (%.2f MHz)\n", nvpll, nvpll_mhz);
    Logging_Write(log_level_message, "VRAM Clock Coefficient  = %08lX (%.2f MHz)\n", mpll, mpll_mhz);

    return true; 
}

void NV4_Shutdown()
{
    // Restore original nonprogrammable VPLL/MPLL/NVPLL
    NV_WriteMMIO32(NV4_PRAMDAC_COEFF_SELECT, nv4_state.original_pll_setting);
    return; 
}