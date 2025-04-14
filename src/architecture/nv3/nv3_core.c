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

    /* TODO: Read our Dumb Framebuffer */
    printf("I'm a Riva 128! Information: \n");
    printf("NV_PMC_BOOT_0           = %08lX \n", _farpeekl(current_device.bar0_selector, 0));
    printf("NV_PFB_BOOT_0           = %08lX \n", _farpeekl(current_device.bar0_selector, 0x100000));
    return true; 
}