/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    NV10_core.c: NV1x (GeForce 256, 2, 2MX, 4MX) / Celsius-based GPU architecture
*/

#include "nvplay.h"
#include <architecture/nv10/nv10.h>
#include <config/config.h>

// Globals
nv10_state_t nv10_state = {0};

bool nv10_init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;
    bar1_base &= 0xFF000000;

    Logging_Write(log_level_debug, "Celsius - PCI BAR0 0x%08lX\n", bar0_base);
    Logging_Write(log_level_debug, "Celsius - PCI BAR1 0x%08lX\n", bar1_base);
    
    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};
    __dpmi_meminfo meminfo_bar1 = {0};

    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = NV10_MMIO_SIZE;
    meminfo_bar1.address = bar1_base;
    meminfo_bar1.size = NV10_MMIO_SIZE; //this will change

    current_device.bar1_dfb_start = bar1_base;
    current_device.ramin_start = bar0_base + NV10_RAMIN_START;

    __dpmi_physical_address_mapping(&meminfo_bar0);
    __dpmi_physical_address_mapping(&meminfo_bar1);
    
    Logging_Write(log_level_debug, "Celsius Init: Mapping BAR0 (MMIO + RAMIN)...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, NV10_MMIO_SIZE - 1);

    Logging_Write(log_level_debug, "Celsius Init: Mapping BAR1 (VRAM aperture)...\n");

    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar1.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, NV10_MMIO_SIZE - 1); // ultimately the same size

    /* store manufacture time configuratino */
    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV10_PMC_BOOT);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV10_PFB_CSTATUS);
    current_device.nv10_pfb_cfg = nv_mmio_read32(NV10_PFB_CFG);

    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV10_PFB_CSTATUS_VRAM) & 0xFF; 
    current_device.vram_amount = 0x100000 * ram_amount_value;

    // we can't discover the vram amount to limit how much gets mapped
    // (I'm worried about the effect of mapping 128m on other devices)

    uint32_t limit = current_device.vram_amount - 1;
    
    // map 32mb to preserve address space. always map max amount
    if (config.nv10_always_map_128m)
        limit = 0x7FFFFFF;
    else
    {
        if (current_device.vram_amount < 0x1000000)
            limit = NV10_MMIO_SIZE - 1;
        else
            limit = current_device.vram_amount;
    }

    // always map 32mb
    if (current_device.vram_amount <= 0x1000000)
        limit = NV10_MMIO_SIZE - 1;

    __dpmi_set_segment_limit(current_device.bar1_selector, limit); // ultimately the same size

    current_device.straps = nv_mmio_read32(NV10_PSTRAPS);

    if ((current_device.straps >> NV10_PSTRAPS_CRYSTAL) & 0x01)
        current_device.crystal_hz = NV_CLOCK_BASE_14318180;
    else
        current_device.crystal_hz = NV_CLOCK_BASE_13500K;

    /* Power up all GPU subsystems */
    Logging_Write(log_level_debug, "Celsius Init: Enabling all GPU subsystems (0x11111111 -> NV10_PMC_ENABLE)...");
    nv_mmio_write32(NV10_PMC_ENABLE, 0x11111111);
    Logging_Write(log_level_debug, "Done!\n");

    /* Enable interrupts (test) */
    Logging_Write(log_level_debug, "Celsius Init: Enabling interrupts...");
    nv_mmio_write32(NV10_PMC_INTR_EN, (NV10_PMC_INTR_EN_HARDWARE | NV10_PMC_INTR_EN_SOFTWARE));
    Logging_Write(log_level_debug, "Done!\n");
    
    Logging_Write(log_level_debug, "NV4 Init: Ensuring user-programmable pixel, core and memory clocks...\n");
    
    // ensure programmable pixel and memory clocks for driver & overclock testing
    // we also need to actually *program* the clocks on NV4 if you set the PLL to programmable omde it seems. It really doesn't like it if you don't
    // so store the old values

    current_device.vpll = nv_mmio_read32(NV10_PRAMDAC_CLOCK_PIXEL);
    current_device.mpll = nv_mmio_read32(NV10_PRAMDAC_CLOCK_MEMORY);
    current_device.nvpll = nv_mmio_read32(NV10_PRAMDAC_CLOCK_CORE);

    uint32_t pramdac_pll_coeff_select = nv_mmio_read32(NV10_PRAMDAC_COEFF_SELECT);

    // Save original NV4 PLL setting. 
    nv10_state.original_pll_setting = pramdac_pll_coeff_select;
    pramdac_pll_coeff_select |= (NV10_PRAMDAC_COEFF_SELECT_ALL_SOFTWARE << NV10_PRAMDAC_COEFF_SELECT_SOURCE);

    nv_mmio_write32(NV10_PRAMDAC_COEFF_SELECT, pramdac_pll_coeff_select);

    //go
    nv_mmio_write32(NV10_PRAMDAC_CLOCK_CORE, current_device.nvpll);
    nv_mmio_write32(NV10_PRAMDAC_CLOCK_MEMORY, current_device.mpll);
    nv_mmio_write32(NV10_PRAMDAC_CLOCK_PIXEL, current_device.vpll);


    return true; 
}

bool nv10_dump_mfg_info()
{
        /* TODO: Read our Dumb Framebuffer */
    Logging_Write(log_level_message, "Nvidia Celsius-based Graphics Hardware (NV1x) Manufacture-Time Configuration: \n");
    Logging_Write(log_level_message, "NV_PMC_BOOT_0           = %08lX\n", current_device.nv_pmc_boot_0);
    Logging_Write(log_level_message, "NV_PFB_CFG              = %08lX\n", current_device.nv_pfb_boot_0);
    Logging_Write(log_level_message, "NV_PFB_CSTATUS          = %08lX\n", current_device.nv10_pfb_cfg);
    /* 
        Determine the amount of Video RAM 
        In theory this could be a shared function between all nv gpus, but in reality i'm not so sure
    */

    Logging_Write(log_level_message, "Video RAM Size          = %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read in the straps */
    Logging_Write(log_level_message, "Straps                  = %08lX\n", current_device.straps);

    // We store these but read the current values
    uint32_t vpll = nv_mmio_read32(NV10_PRAMDAC_CLOCK_PIXEL);
    uint32_t nvpll = nv_mmio_read32(NV10_PRAMDAC_CLOCK_CORE);
    uint32_t mpll = nv_mmio_read32(NV10_PRAMDAC_CLOCK_MEMORY);

    double vpll_mhz = nv_clock_mnp_to_mhz(current_device.crystal_hz, vpll);
    double nvpll_mhz = nv_clock_mnp_to_mhz(current_device.crystal_hz, nvpll);
    double mpll_mhz = nv_clock_mnp_to_mhz(current_device.crystal_hz, mpll);

    //todo: convert to MHz
    Logging_Write(log_level_message, "Pixel Clock Coefficient = %08lX (%.2f MHz)\n", vpll, vpll_mhz);
    Logging_Write(log_level_message, "Core Clock Coefficient  = %08lX (%.2f MHz)\n", nvpll, nvpll_mhz);
    Logging_Write(log_level_message, "VRAM Clock Coefficient  = %08lX (%.2f MHz)\n", mpll, mpll_mhz);


    return true; 
}

void nv10_shutdown()
{
    // Restore original nonprogrammable VPLL/MPLL/NVPLL
    nv_mmio_write32(NV10_PRAMDAC_COEFF_SELECT, nv10_state.original_pll_setting);
    return; 
}