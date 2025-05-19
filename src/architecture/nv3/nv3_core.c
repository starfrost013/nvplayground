//
// Filename: nv3_core.c
// Purpose: NV3/NV3T (Riva 128/128ZX) core functions (bringup, shutdown, mainloop)
//
#include "architecture/nv3/nv3_ref.h"
#include "core/nvcore.h"
#include "dpmi.h"
#include "go32.h"
#include "nv3.h"
#include "sys/farptr.h"
#include "time.h"

#include <stdio.h>
#include <stdlib.h>

/* TEMPORARY test  function to test certain hardcoded overclocks */
bool nv3_test_overclock()
{
    /* print out some helpful messages */
    printf("Basic clockspeed test (text mode: best case scenario)\n");
    printf("The GPU will try to run for 60 seconds at each clock setting, gradually going from an underclock to an overclock. If it crashes, reboot, and the default 100Mhz MCLK will be restored.\n");
    printf("Note: Some NVIDIA RIVA 128 ZX cards manufactured by TSMC run at 90Mhz and will have less overclocking potential!\n");

    /* read the straps to find our base clock value */
    uint32_t straps = nv_mmio_read32(NV3_PSTRAPS);

    /* 
        there are two possible clock bases here: 13.5 and 14.318 Mhz 
        we need two different values for our start menu
    */

    float clock_base = 13500000.0f;
    bool is_14318mhz_clock = false;     

    if ((straps >> NV3_PSTRAPS_CRYSTAL) & 0x01)
    {
        clock_base = 14318180.0f;
        is_14318mhz_clock = true; 
    }
        
    /* We vary the n-parameter of the MCLK to fine-tune the GPU clock speed. M can be used for large steps and P param can be used for very big steps */

    uint32_t clock_n_start = 0xA3; 
    uint32_t clock_m = 0x0B, clock_p = 0x01;

    if (is_14318mhz_clock)
    {
        clock_m = 0x0E;
        //base clock_n is 0xC4, so the gpu will be biased more towards underclocking
        clock_n_start = 0xC4;
    }

    for (int32_t clock_n = clock_n_start; clock_n <= 0xFF; clock_n++)
    {
        uclock_t start_clock = uclock();

        uint32_t final_clock = (clock_p << 16) 
        | (clock_n << 8)
        | clock_m;

        //not speed critical, use a double
        double megahertz = (clock_base * clock_n) / (clock_m << clock_p) / 1000000.0f;

        printf("Trying MCLK = %.2f Mhz (NV_PRAMDAC_MPLL_COEFF = %08lx)...\n", megahertz, final_clock);

        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, final_clock);

        uclock_t this_clock = uclock();

        // Sit in a spinloop until it's time to wake up
        while (this_clock - start_clock < (UCLOCKS_PER_SEC * NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS))
            this_clock = uclock();
    }

    printf("We survived. Returning to 100Mhz...\n");
    /* restore original clock */
    if (is_14318mhz_clock)
        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TEST_OVERCLOCK_BASE_14318);
    else
        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TEST_OVERCLOCK_BASE_13500);

    return true; 
}

bool nv3_dump_vbios()
{
    printf("Dumping Video BIOS...");

    FILE* vbios = fopen("nv3bios.bin", "wb");

    //uint32_t rom_location = NV3_PROM_START;
    uint32_t vbios_bin[8192];

    for (int32_t i = 0; i < 8192; i++)
    {
        vbios_bin[i] = _farpeekl(_dos_ds, 0xC0000 + i*4);
        //rom_location += 4; //dword 
    }   

    fwrite(vbios_bin, sizeof(vbios_bin), 1, vbios);

    fclose(vbios);
    printf("Done!\n");

    return true; 
}

bool nv3_dump_mmio()
{
    printf("Dumping GPU MMIO...");

    FILE* vbios_bar0 = fopen("nv3bar0.bin", "wb");
    FILE* vbios_bar1 = fopen("nv3bar1.bin", "wb");

    if (!vbios_bar0
    || !vbios_bar1)
        return false;

    uint8_t* mmio_dump_bar_buf = (uint8_t*)calloc(1, NV3_MMIO_SIZE);

    if (!mmio_dump_bar_buf)
        return false; 
    
    // use ldt to read out BAR0 and BAR1 MMIO 

    for (int32_t i = 0; i < NV3_MMIO_SIZE; i++)
    {
        mmio_dump_bar_buf[i] = _farpeekl(current_device.bar0_selector, i);
    }

    fwrite(vbios_bar0, sizeof(mmio_dump_bar_buf), 1, vbios_bar0);

    for (int32_t i = 0; i < NV3_MMIO_SIZE; i++)
    {
        mmio_dump_bar_buf[i] = _farpeekl(current_device.bar1_selector, i);
    }

    fwrite(vbios_bar1, sizeof(mmio_dump_bar_buf), 1, vbios_bar1);
    
    fclose(vbios_bar0);
    fclose(vbios_bar1);
    
    free(mmio_dump_bar_buf);

    printf("Done!\n");

    return true; 
}

bool nv3_print_info()
{
    /* TODO: Read our Dumb Framebuffer */
    printf("NV3 Manufacture-Time Configuration: \n");
    printf("NV_PMC_BOOT_0           = %08lX\n", current_device.nv_pmc_boot_0);
    printf("NV_PFB_BOOT_0           = %08lX\n", current_device.nv_pfb_boot_0);
    /* 
        Determine the amount of Video RAM 
        In theory this could be a shared function between all nv gpus, but in reality i'm not so sure
    */

    printf("Video RAM Size          = %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read in the straps */
    printf("Straps                  = %08lX\n", current_device.straps);

    uint32_t vpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_PIXEL);
    uint32_t mpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_MEMORY);
    
    //todo: MHz
    printf("Pixel Clock Coefficient = %08lX\n", vpll);
    printf("Memory Clock Coefficient= %08lX\n", mpll);
    return true; 

}

bool nv3_init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;
    bar1_base &= 0xFF000000;

    printf("NV3 - PCI BAR0 0x%08lX\n", bar0_base);
    printf("NV3 - PCI BAR1 0x%08lX\n", bar1_base);
    
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
    
    printf("GPU Init: Mapping BAR0 MMIO...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, NV3_MMIO_SIZE - 1);

    printf("GPU Init: Mapping BAR1 (DFB / RAMIN)...\n");

    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar1.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, NV3_MMIO_SIZE - 1); // ultimately the same size

    /* store manufacture time configuratino */
    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV3_PMC_BOOT);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV3_PFB_BOOT);

    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV3_PFB_BOOT_RAM_AMOUNT) & 0x03; 
    bool ram_extension_8mb = (current_device.nv_pfb_boot_0 >> NV3_PFB_BOOT_RAM_EXTENSION) & 0x01;      // Needed for Riva128 ZX
    
    /* Read in the amount of video memory from the NV_PFB_BOOT_0 register. Note: 0 without RAM_EXTENSION is 1MB!!! which was never even released... */
    if (!ram_amount_value && ram_extension_8mb == NV3_PFB_BOOT_RAM_EXTENSION_8MB)       // 8MB (Riva 128 ZX)
        current_device.vram_amount = NV3_VRAM_SIZE_8MB;
    else if (ram_amount_value == NV3_PFB_BOOT_RAM_AMOUNT_4MB)                           // 4MB (Most Riva 128s)
        current_device.vram_amount = NV3_VRAM_SIZE_4MB;
    else if (ram_amount_value == NV3_PFB_BOOT_RAM_AMOUNT_2MB)                           // 2MB (Single NEC card)
        current_device.vram_amount = NV3_VRAM_SIZE_2MB;
    else if (!ram_amount_value && ram_extension_8mb == NV3_PFB_BOOT_RAM_EXTENSION_NONE) // 1MB (never existed)
        current_device.vram_amount = NV3_VRAM_SIZE_1MB;

    current_device.straps = nv_mmio_read32(NV3_PSTRAPS);

    /* Power up all GPU subsystems */
    printf("Enabling all GPU subsystems (0x11111111 -> NV3_PMC_ENABLE)...");
    nv_mmio_write32(NV3_PMC_ENABLE, 0x11111111);
    printf("Done!\n");

    /* Enable interrupts (test) */
    printf("Enabling interrupts...");
    nv_mmio_write32(NV3_PMC_INTERRUPT_ENABLE, (NV3_PMC_INTERRUPT_ENABLE_HARDWARE | NV3_PMC_INTERRUPT_ENABLE_SOFTWARE));
    printf("Done!\n");
    
    return true; 
}