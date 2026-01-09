/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    virge.h: S3 ViRGE definitions
*/

#include "core/gpu/gpu.h"
#include "dpmi.h"
#include <architecture/cirrus/alpine.h>

bool Alpine_Init()
{
     // only top 8 bits actually matter
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR0);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;

    /* 
        Enable MMIO and I/O and relocate the PCI BARs if they aren't already programmed e.g. on dos 
        todo: TURN THIS OFF IN SHUTDOWN FUNCTION! BUT ONLY IF WE'RE NOT UNDER WINDOWS...
    */
    if (!bar0_base)
    {
        Logging_Write(LOG_LEVEL_MESSAGE, "Alpine: Chip is not enabled. Enabling I/O + Memory Space + BAR0...\n");

        uint16_t command = PCI_ReadConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_COMMAND);

        command |= (PCI_CFG_OFFSET_COMMAND_BUS_MASTER | PCI_CFG_OFFSET_COMMAND_MEM_ENABLED | PCI_CFG_OFFSET_COMMAND_IO_ENABLED);
        
        PCI_WriteConfig16(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_COMMAND, command);
        Logging_Write(LOG_LEVEL_DEBUG, "Alpine: Programming Base Address Register 0 to hopefully-free value...%08x (prefetchable)\n", CIRRUS_MMIO_SPACE_TEST & 0xFE000000); // virge has to be on 64M boundary
        PCI_WriteConfig32(current_device.bus_info.bus_number, current_device.bus_info.function_number, PCI_CFG_OFFSET_BAR0, CIRRUS_MMIO_SPACE_TEST);
    
    }

    /* Enable LFB */
    VGA_WriteSequencer(CIRRUS_ALPINE_SR_APERTURE_ENABLE, (1 << CIRRUS_ALPINE_SR_APERTURE_ENABLE));

    /* Check video BIOS for chip revision*/
    __dpmi_regs regs = {0};

    regs.h.ah = CIRRUS_INT_INQUIRE;
    regs.h.bl = CIRRUS_INT_INQUIRE_CHIP_REVISION;

    __dpmi_int(INT_VIDEO, &regs);

    if (regs.h.al == 0)
        current_device.revision = CIRRUS_5446_REV_A;
    else    
        current_device.revision = CIRRUS_5446_REV_B;
    
    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};

    meminfo_bar0.address = bar0_base;

    // 5446 maps differently
    if (current_device.revision == CIRRUS_5446_REV_A)
        meminfo_bar0.size = CIRRUS_MMIO_SIZE_REV_A;
    else
        meminfo_bar0.size = CIRRUS_MMIO_SIZE_REV_B;
    // NV1 does not use BAR1

    __dpmi_physical_address_mapping(&meminfo_bar0);
    
    Logging_Write(LOG_LEVEL_DEBUG, "GPU Init: Mapping BAR0 MMIO...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bus_info.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bus_info.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bus_info.bar0_selector, meminfo_bar0.size);

    return true; 
}

void Alpine_Shutdown()
{

}