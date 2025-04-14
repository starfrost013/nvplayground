//
// Filename: nv3_core.c
// Purpose: NV3/NV3T (Riva 128/128ZX) core functions (bringup, shutdown, mainloop)
//
#include "architecture/nv3/nv3_ref.h"
#include "core/nvcore.h"
#include "dpmi.h"
#include "nv3.h"
#include "sys/farptr.h"

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
    __dpmi_meminfo meminfo_bar1_ramin = {0};
    __dpmi_meminfo meminfo_bar1_dfb = {0};

    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = NV3_MMIO_SIZE;
    meminfo_bar1_ramin.address = bar1_base + 0xC00000;
    meminfo_bar1_ramin.size = NV3_RAMIN_SIZE;
    meminfo_bar1_dfb.address = bar1_base;
    meminfo_bar1_dfb.size = NV3_VRAM_SIZE_4MB; //this will change

    __dpmi_physical_address_mapping(&meminfo_bar0);
    __dpmi_physical_address_mapping(&meminfo_bar1_ramin);
    __dpmi_physical_address_mapping(&meminfo_bar1_dfb);

    
    printf("GPU Init: Mapping BAR0 MMIO...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, NV3_MMIO_SIZE - 1);

    printf("GPU Init: Mapping BAR1 (DFB / RAMIN)...\n");

    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar1_dfb.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, NV3_MMIO_SIZE - 1); // ultimately the same size

    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV3_PMC_BOOT);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV3_PFB_BOOT);

    /* TODO: Read our Dumb Framebuffer */
    printf("I'm a Riva 128! Information: \n");
    printf("NV_PMC_BOOT_0           = %08lX\n", current_device.nv_pmc_boot_0);
    printf("NV_PFB_BOOT_0           = %08lX\n", current_device.nv_pfb_boot_0);

    /* 
        Determine the amount of Video RAM 
        In theory this could be a shared function between all nv gpus, but in reality i'm not so sure
    */
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

    printf("Video RAM Size          = %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read in the straps */
    current_device.straps = nv_mmio_read32(NV3_PSTRAPS);
    printf("Straps                  = %08lX\n", current_device.straps);

    uint32_t vpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_PIXEL);
    uint32_t mpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_MEMORY);

    //todo: MHz
    printf("Pixel Clock Coefficient = %08lX", vpll);
    printf("Memory Clock Coefficient= %08lX", mpll);

    /* Power up all GPU subsystems */
    printf("Enabling all GPU subsystems (NV3_PMC_ENABLE -> 0x11111111)...");
    nv_mmio_write32(NV3_PMC_ENABLE, 0x11111111);
    printf("Done!\n");

    /* Enable interrupts (test) */
    printf("Enabling interrupts...");
    nv_mmio_write32(NV3_PMC_INTERRUPT_ENABLE, (NV3_PMC_INTERRUPT_ENABLE_HARDWARE | NV3_PMC_INTERRUPT_ENABLE_SOFTWARE));
    printf("Done!\n");

    printf("Turning them off again for now...");
    nv_mmio_write32(NV3_PMC_INTERRUPT_ENABLE, 0);
    printf("Done!\n");

    return true; 
}