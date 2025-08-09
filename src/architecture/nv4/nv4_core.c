#include "architecture/nv3/nv3_ref.h"
#include "nv4_ref.h"
#include "nvplayground.h"
#include <architecture/nv4/nv4.h>

// Globals
nv4_state_t nv4_state = {0};

bool nv4_init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

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
    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV4_PMC_BOOT);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV4_PFB_BOOT);

    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV4_PFB_BOOT_RAM_AMOUNT) & 0x03; 
    
    /* Read in the amount of video memory from the NV_PFB_BOOT_0 register. Note: 0 without RAM_EXTENSION is 1MB!!! which was never even released... */
    if (!ram_amount_value)                                                              // 8MB (RIVA 128 ZX)
        current_device.vram_amount = NV4_VRAM_SIZE_2MB;
    else if (ram_amount_value == NV4_PFB_BOOT_RAM_AMOUNT_4MB)                           // 4MB (Never released)
        current_device.vram_amount = NV4_VRAM_SIZE_4MB;
    else if (ram_amount_value == NV4_PFB_BOOT_RAM_AMOUNT_2MB)                           // 8MB
        current_device.vram_amount = NV4_VRAM_SIZE_8MB;
    else if (ram_amount_value == NV4_PFB_BOOT_RAM_AMOUNT_16MB)                          // 16Mb
        current_device.vram_amount = NV4_VRAM_SIZE_16MB;

    current_device.straps = nv_mmio_read32(NV4_PSTRAPS);

    /* Power up all GPU subsystems */
    Logging_Write(log_level_debug, "NV4 Init: Enabling all GPU subsystems (0x11111111 -> NV4_PMC_ENABLE)...");
    nv_mmio_write32(NV4_PMC_ENABLE, 0x11111111);
    Logging_Write(log_level_debug, "Done!\n");

    /* Enable interrupts (test) */
    Logging_Write(log_level_debug, "NV4 Init: Enabling interrupts...");
    nv_mmio_write32(NV4_PMC_INTR_EN, (NV4_PMC_INTR_EN_HARDWARE | NV4_PMC_INTR_EN_SOFTWARE));
    Logging_Write(log_level_debug, "Done!\n");
    
    Logging_Write(log_level_debug, "NV4 Init: Ensuring user-programmable pixel, core and and memory clocks...\n");
    
    // ensure programmable pixel and memory clocks for driver & overclock testing
    // we also need to actually *program* the clocks on NV4 if you set the PLL to programmable omde it seems. It really doesn't like it if you don't
    // so store the old values

    current_device.vpll = nv_mmio_read32(NV4_PRAMDAC_CLOCK_PIXEL);
    current_device.mpll = nv_mmio_read32(NV4_PRAMDAC_CLOCK_MEMORY);
    current_device.nvpll = nv_mmio_read32(NV4_PRAMDAC_CLOCK_CORE);

    uint32_t pramdac_pll_coeff_select = nv_mmio_read32(NV4_PRAMDAC_COEFF_SELECT);

    // Save original NV4 PLL setting. 
    nv4_state.original_pll_setting = pramdac_pll_coeff_select;

    pramdac_pll_coeff_select |= (NV4_PRAMDAC_COEFF_SELECT_ALL_SOFTWARE << NV4_PRAMDAC_COEFF_SELECT_SOURCE);

    nv_mmio_write32(NV4_PRAMDAC_COEFF_SELECT, pramdac_pll_coeff_select);

    //go
    nv_mmio_write32(NV4_PRAMDAC_CLOCK_CORE, current_device.nvpll);
    nv_mmio_write32(NV4_PRAMDAC_CLOCK_MEMORY, current_device.mpll);
    nv_mmio_write32(NV4_PRAMDAC_CLOCK_PIXEL, current_device.vpll);


    return true; 
}

bool nv4_dump_mfg_info()
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
    uint32_t vpll = nv_mmio_read32(NV4_PRAMDAC_CLOCK_PIXEL);
    uint32_t nvpll = nv_mmio_read32(NV4_PRAMDAC_CLOCK_CORE);
    uint32_t mpll = nv_mmio_read32(NV4_PRAMDAC_CLOCK_MEMORY);
    
    //todo: convert to MHz
    Logging_Write(log_level_message, "Pixel Clock Coefficient = %08lX\n", vpll);
    Logging_Write(log_level_message, "Core Clock Coefficient  = %08lX\n", nvpll);
    Logging_Write(log_level_message, "VRAM Clock Coefficient  = %08lX\n", mpll);

    return true; 
}

#define NV4_FLUSH_FREQUENCY     65536

bool nv4_dump_mmio()
{
    Logging_Write(log_level_message, "Dumping GPU PCI BARs (BAR0 = MMIO, BAR1 = VRAM/RAMIN)...\n");

    FILE* vbios_bar0 = fopen("nv4bar0.bin", "wb");
    FILE* vbios_bar1 = fopen("nv4bar1.bin", "wb");

    if (!vbios_bar0
    || !vbios_bar1)
        return false;

    uint32_t* mmio_dump_bar_buf = (uint32_t*)calloc(1, NV4_MMIO_SIZE);

    if (!mmio_dump_bar_buf)
        return false; 
    
    // use ldt to read out BAR0 and BAR1 MMIO 
    // Flush every 64kb, because the real NV3 hardware may crash at some point?!

    /* 
        Dump all known memory regions except write-only ones
        We don't use nv_mmio_* because those will account for other things in the future
    */

    for (int32_t bar0_pos = 0; bar0_pos <= NV4_MMIO_SIZE; bar0_pos += 4)
    {
        // subtract nv4_flush_frequency to start at 0
        if (bar0_pos % NV4_FLUSH_FREQUENCY == 0 
            && bar0_pos > 0) // i'm lazy
        {
            Logging_Write(log_level_debug, "Dumped BAR0 up to: %08lX\n", bar0_pos);
            fwrite(&mmio_dump_bar_buf[(bar0_pos - NV4_FLUSH_FREQUENCY) >> 2], NV4_FLUSH_FREQUENCY, 1, vbios_bar0);
            fflush(vbios_bar0);

            // don't try and read out of bounds
            if (bar0_pos == NV4_MMIO_SIZE)
                break;
        }

        // Yay. NV4 does not crash reading register
        mmio_dump_bar_buf[bar0_pos >> 2] = _farpeekl(current_device.bar0_selector, bar0_pos);

    }

    fclose(vbios_bar0);

    for (int32_t bar1_pos = 0; bar1_pos <= NV4_MMIO_SIZE; bar1_pos += 4)
    {
        if ((bar1_pos % NV4_FLUSH_FREQUENCY == 0 
            && bar1_pos > 0))
        {
            Logging_Write(log_level_debug, "Dumped BAR1 up to: %08lX\n", bar1_pos);
            fwrite(&mmio_dump_bar_buf[(bar1_pos - NV4_FLUSH_FREQUENCY) >> 2], NV4_FLUSH_FREQUENCY, 1, vbios_bar1);
            fflush(vbios_bar1);

            // don't try and read out of bounds
            if (bar1_pos == NV4_MMIO_SIZE)
                break;
        }

        // no excluded areas needed
        mmio_dump_bar_buf[bar1_pos >> 2] = _farpeekl(current_device.bar1_selector, bar1_pos);

    }

    fclose(vbios_bar1);
    
    free(mmio_dump_bar_buf);

    Logging_Write(log_level_message, "Done!\n");

    return true; 
}

bool nv4_shutdown()
{
    // Restore original nonprogrammable VPLL/MPLL/NVPLL
    nv_mmio_write32(NV4_PRAMDAC_COEFF_SELECT, nv4_state.original_pll_setting);
    return true; 
}