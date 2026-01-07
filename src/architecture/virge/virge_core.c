/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    virge.h: S3 ViRGE definitions
*/

#include "virge.h"

bool ViRGE_Init()
{
     // only top 8 bits actually matter
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;

    /* 
        Enable MMIO and I/O and relocate the PCI BARs if they aren't already programmed e.g. on dos 
        todo: TURN THIS OFF IN SHUTDOWN FUNCTION! BUT ONLY IF WE'RE NOT UNDER WINDOWS...
    */
    if (!bar0_base)
    {
        Logging_Write(LOG_LEVEL_MESSAGE, "ViRGE: Chip is not enabled. Enabling I/O + Memory Space + BAR0...\n");

        uint16_t command = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND);

        command |= (PCI_CFG_OFFSET_COMMAND_BUS_MASTER | PCI_CFG_OFFSET_COMMAND_MEM_ENABLED | PCI_CFG_OFFSET_COMMAND_IO_ENABLED);
        
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, command);
        Logging_Write(LOG_LEVEL_DEBUG, "ViRGE: Programming Base Address Register 0 to hopefully-free value...%08x (prefetchable)\n", VIRGE_MMIO_SPACE_TEST & 0xFC000000); // virge has to be on 64M boundary
        PCI_WriteConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0, VIRGE_MMIO_SPACE_TEST);
    
    }

    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};

    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = S3VIRGE_BAR0_SIZE;
    // NV1 does not use BAR1

    __dpmi_physical_address_mapping(&meminfo_bar0);
    
    Logging_Write(LOG_LEVEL_DEBUG, "GPU Init: Mapping BAR0 MMIO...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, S3VIRGE_BAR0_SIZE);

    /* store manufacture time configuratino */
    uint32_t subsystem_status = S3VIRGE_SUBSYSTEM_STATUS;

    Logging_Write(LOG_LEVEL_MESSAGE, "ViRGE Subsystem Status = %08x\n", subsystem_status);

    return true; 
}

void ViRGE_Shutdown()
{

}