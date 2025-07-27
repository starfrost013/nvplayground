/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv1_core.c: NV1 core functions (bringup, main loop, shutdown)
*/

#include <architecture/nv1/nv1.h>
#include <architecture/nv1/nv1_ref.h>
#include "core/nvcore.h"
#include "dpmi.h"
#include "nv1_ref.h"
#include "util/util.h"
#include <stdlib.h>

nv1_state_t nv1_state = {0};                    // NV1 specific state 

bool nv1_print_info()
{
    /* TODO: Read our Dumb Framebuffer */
    Logging_Write(log_level_message, "NV1 Manufacture-Time Configuration: \n");
    Logging_Write(log_level_message, "NV_PMC_BOOT_0           = %08lX\n", current_device.nv_pmc_boot_0);
    Logging_Write(log_level_message, "NV_PFB_BOOT_0           = %08lX\n", current_device.nv_pfb_boot_0);
    /* 
        Determine the amount of Video RAM 
        In theory this could be a shared function between all nv gpus, with the actual amount based on the gpu-specific values, but in reality i'm not so sure
    */

    Logging_Write(log_level_message, "Video RAM Size          = %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read in the straps */
    Logging_Write(log_level_message, "Straps                  = %08lX\n", current_device.straps);

    //TODO: Write code to interface with SGS-1732/1764/Nvidia Picasso DAC
    return true; 
}

bool nv1_print_pci_config()
{
    // read out the device id
    uint16_t vendor_id = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint16_t device_id = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint16_t command = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND);
    uint16_t status = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_STATUS);
    uint8_t revision = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_REVISION);
    uint8_t class_id_high = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CLASS_CODE_HIGH);
    uint16_t class_id_low = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CLASS_CODE_LOW);
    uint8_t cache_line_size = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CACHE_LINE_SIZE);
    uint8_t latency_timer = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_LATENCY_TIMER);
    uint8_t header_type = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_HEADER_TYPE);
    uint8_t bist = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BIST);
    uint32_t bar0 = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1 = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);
    uint32_t bar2 = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR2);
    uint32_t bar3 = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR3);
    uint32_t bar4 = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR4);
    uint32_t bar5 = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR5);
    uint32_t cardbus_cis_ptr = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CARDBUS_CIS_PTR);
    uint16_t subsystem_vendor_id = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_SUBSYSTEM_VENDOR_ID);
    uint16_t subsystem_id = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_SUBSYSTEM_ID);
    uint32_t rom_bar = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_EXPANSION_ROM_BASE);
    uint8_t capabilities_ptr = pci_read_config_8(current_device.bus_number, current_device.function_number,  PCI_CFG_OFFSET_CAPABILITIES_PTR);
    uint8_t interrupt_line = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_INTERRUPT_LINE);
    uint8_t interrupt_pin = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_INTERRUPT_PIN);
    uint8_t minimum_grant = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_MINIMUM_GRANT);
    uint8_t maximum_latency = pci_read_config_8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_MAXIMUM_LATENCY);

    Logging_Write(log_level_message, "[PCI CFG] PCI ID %04x:%04x\n", vendor_id, device_id);
    Logging_Write(log_level_message, "[PCI CFG] Command Register %04x\n", command);
    Logging_Write(log_level_message, "[PCI CFG] Status Register %04x\n", status);
    Logging_Write(log_level_message, "[PCI CFG] Revision %02x\n", revision);
    Logging_Write(log_level_message, "[PCI CFG] Class ID: %06x\n", (class_id_high << 16) | class_id_low);
    Logging_Write(log_level_message, "[PCI CFG] Cache Line Size %02x\n", cache_line_size);
    Logging_Write(log_level_message, "[PCI CFG] Latency Timer %02x\n", latency_timer);
    Logging_Write(log_level_message, "[PCI CFG] Header Type %02x (should be 0)\n", header_type);
    Logging_Write(log_level_message, "[PCI CFG] BIST %02x\n", bist);
    Logging_Write(log_level_message, "[PCI CFG] BAR0 %08x\n", bar0);
    Logging_Write(log_level_message, "[PCI CFG] BAR1 %08x\n", bar1);
    Logging_Write(log_level_message, "[PCI CFG] BAR2 %08x\n", bar2);
    Logging_Write(log_level_message, "[PCI CFG] BAR3 %08x\n", bar3);
    Logging_Write(log_level_message, "[PCI CFG] BAR4 %08x\n", bar4);
    Logging_Write(log_level_message, "[PCI CFG] BAR5 %08x\n", bar5);
    Logging_Write(log_level_message, "[PCI CFG] CardBus CIS Pointer %04x\n", cardbus_cis_ptr);
    Logging_Write(log_level_message, "[PCI CFG] Subsystem ID %04x:%04x\n", subsystem_vendor_id, subsystem_id);
    Logging_Write(log_level_message, "[PCI CFG] ROM BAR %04x\n", rom_bar);
    Logging_Write(log_level_message, "[PCI CFG] Capabilities Pointer %02x\n", capabilities_ptr);
    Logging_Write(log_level_message, "[PCI CFG] Interrupt Line %02x\n", interrupt_line);
    Logging_Write(log_level_message, "[PCI CFG] Interrupt Pin %02x\n", interrupt_pin);
    Logging_Write(log_level_message, "[PCI CFG] Minimum Grant %02x\n", minimum_grant);
    Logging_Write(log_level_message, "[PCI CFG] Maximum Latency %02x\n", maximum_latency);

    return true; 
}

// Type 0 (32-bit)
// Bit 3 = 1, for prefetchable (Only Bits 32:24 matter for BAR, it must begin on)
#define NV1_MMIO_SPACE_TEST     0x84000008

bool nv1_init()
{
    // only top 8 bits actually matter
    uint32_t bar0_base = pci_read_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);

    /* According to the datasheet only the top 8 bits matter */
    bar0_base &= 0xFF000000;

    /* 
        Enable MMIO and I/O and relocate the PCI BARs if they aren't already programmed e.g. on dos 
        todo: TURN THIS OFF IN SHUTDOWN FUNCTION! BUT ONLY IF WE'RE NOT UNDER WINDOWS...
    */
    if (!bar0_base)
    {
        Logging_Write(log_level_message, "NV1: Chip is not enabled. Enabling I/O + Memory Space + BAR0...\n");

        uint16_t command = pci_read_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND);

        // Value 0x07 seen in DOS box on rev.C 2MB unit (ChipToken 0xE5219C6FA99E625E)
        command |= (PCI_CFG_OFFSET_COMMAND_BUS_MASTER | PCI_CFG_OFFSET_COMMAND_MEM_ENABLED | PCI_CFG_OFFSET_COMMAND_IO_ENABLED);
        
        pci_write_config_16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, command);
        Logging_Write(log_level_debug, "NV1: Programming Base Address Register 0 to hopefully-free value...%08x (prefetchable)\n", NV1_MMIO_SPACE_TEST & 0xFF000000);
        pci_write_config_32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0, NV1_MMIO_SPACE_TEST);
    
    }

    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};

    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = NV1_PCI_BAR0_SIZE;
    // NV1 does not use BAR1

    __dpmi_physical_address_mapping(&meminfo_bar0);
    
    Logging_Write(log_level_debug, "GPU Init: Mapping BAR0 MMIO...\n");

    /* Set up two LDTs, we don't need one for ramin, because, it's just a part of bar1 we map differently */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, NV1_PCI_BAR0_SIZE);

    /* store manufacture time configuratino */
    current_device.nv_pmc_boot_0 = nv_mmio_read32(NV1_PMC_BOOT_0);
    current_device.nv_pfb_boot_0 = nv_mmio_read32(NV1_PFB_BOOT_0);

    uint32_t ram_amount_value = (current_device.nv_pfb_boot_0 >> NV1_PFB_BOOT_0_RAM_AMOUNT) & 0x03; 

    if (ram_amount_value == NV1_PFB_BOOT_0_RAM_AMOUNT_4MB)                           // 4MB
        current_device.vram_amount = NV1_VRAM_SIZE_4MB;
    else if (ram_amount_value == NV1_PFB_BOOT_0_RAM_AMOUNT_2MB)                      // 2MB
        current_device.vram_amount = NV1_VRAM_SIZE_2MB;
    else if (ram_amount_value == NV1_PFB_BOOT_0_RAM_AMOUNT_1MB)                      // 1MB
        current_device.vram_amount = NV1_VRAM_SIZE_1MB;

    current_device.straps = nv_mmio_read32(NV1_STRAPS);

    /* Power up all GPU subsystems */
    Logging_Write(log_level_debug, "Enabling all GPU subsystems (0x11111111 -> NV1_PMC_ENABLE)...");
    nv_mmio_write32(NV1_PMC_ENABLE, 0x11111111);
    Logging_Write(log_level_debug, "Done!\n");

    /* Enable interrupts (test) */
    Logging_Write(log_level_debug, "Enabling interrupts - INTA, INTB, INTC and INTD...");
    uint32_t enable_value = (NV1_PMC_INTR_EN_0_ALL >> NV1_PMC_INTR_EN_0_INTD
    | (NV1_PMC_INTR_EN_0_ALL >> NV1_PMC_INTR_EN_0_INTC)
    | (NV1_PMC_INTR_EN_0_ALL >> NV1_PMC_INTR_EN_0_INTB)
    | (NV1_PMC_INTR_EN_0_ALL >> NV1_PMC_INTR_EN_0_INTA));

    nv_mmio_write32(NV1_PMC_INTR_EN_0, enable_value);

    Logging_Write(log_level_debug, "Done!\n");

    // Read the chip token out
    uint32_t chip_token_0 = (uint32_t)nv_mmio_read32(NV1_PAUTH_CHIP_TOKEN_0);
    uint32_t chip_token_1 = (uint32_t)nv_mmio_read32(NV1_PAUTH_CHIP_TOKEN_1);

    nv1_state.chip_token = ((uint64_t)chip_token_0 << 32) | (uint64_t)chip_token_1;

    Logging_Write(log_level_message, "NV1 DRM: ChipToken (Unique ID for NV1 chip): = %08x%08x\n", chip_token_0, chip_token_1);

    return true; 
}

//
// Enters the NV1 into "Security Breach" mode.
//
bool nv1_security_breach()
{
    if (current_device.nv_pmc_boot_0 == NV_PMC_BOOT_NV1_A01)
    {
        Logging_Write(log_level_warning, "NV1 DRM:\ttest skipped: Rev. A card (prototype) detected\n");
        return true;
    }

    // First, let's read the PAUTH_DEBUG register to make sure the breach hasn't already ben done.
    uint32_t pauth_debug_0 = nv_mmio_read32(NV1_PAUTH_DEBUG_0);
    bool breach_detected = (pauth_debug_0 >> NV1_PAUTH_DEBUG_0_BREACH_DETECTED) & 0x01;

    if (breach_detected)
    {
        Logging_Write(log_level_message, "NV1 DRM:\tSecurity Breach Mode ACTIVE!!!! Reset the machine and try again\n");
        return false; 
    }
    else 
        Logging_Write(log_level_message, "NV1 DRM:\tTripping the alarms...\n");

    Logging_Write(log_level_message, "NV1 DRM:\tChipToken (Unique ID for NV1 chip): 0x%08X\n", nv1_state.chip_token);

    Logging_Write(log_level_debug, "Let's try PAUTH_PASSWORD\n");

    uint32_t random_value = 0;

    // First write directly into PAUTH password register.
    for (uint32_t j = 0; j < 128; j++)
    {
        Logging_Write(log_level_debug, "Write to index 0x%X (addr range=%06X-%06X)\n", j, NV1_PAUTH_PASSWORD_0(j), NV1_PAUTH_PASSWORD_3(j));

        // Write 128 bits of garbage data at a time into the PAUTH password registers
        random_value = (rand() << 16 | rand());
        nv_mmio_write32(NV1_PAUTH_PASSWORD_0(j), random_value);
        random_value = (rand() << 16 | rand());
        nv_mmio_write32(NV1_PAUTH_PASSWORD_1(j), random_value);
        random_value = (rand() << 16 | rand());
        nv_mmio_write32(NV1_PAUTH_PASSWORD_2(j), random_value);
        random_value = (rand() << 16 | rand());
        nv_mmio_write32(NV1_PAUTH_PASSWORD_3(j), random_value);

    }
        
    // now let's see if that's enough to trip it
    pauth_debug_0 = nv_mmio_read32(NV1_PAUTH_DEBUG_0);
    breach_detected = (pauth_debug_0 >> NV1_PAUTH_DEBUG_0_BREACH_DETECTED) & 0x01;

    if (breach_detected)
    {
        Logging_Write(log_level_message, "NV1 DRM:\tAlarm tripped. PAUTH_DEBUG_0 bit 0 is set\n. Reset to remove effects.\n");
        return true; 
    }
    else
        Logging_Write(log_level_debug, "That didn't work. Let's try RAMPW\n");

    // If not, try writing garbage directly into the RAMPW area.
    // we do not have to read the ramin config because NV1 mirrors RAMPW to 606000?
    // IF THIS DOESN'T WORK: ACCESS IT VIA RAMIN ((size)-0x400)

    // try writing garbage into rampw
    for (uint32_t rampw_offset = 0; rampw_offset < NV1_RAMPW_SIZE; rampw_offset += 4)   
    {
        random_value = (rand() << 16 | rand());
        nv_mmio_write32(NV1_PRAMPW + rampw_offset, random_value);
    }

    // now let's see if that's enough to trip it
    pauth_debug_0 = nv_mmio_read32(NV1_PAUTH_DEBUG_0);
    breach_detected = (pauth_debug_0 >> NV1_PAUTH_DEBUG_0_BREACH_DETECTED) & 0x01;

    if (breach_detected)
    {
        Logging_Write(log_level_message, "NV1 DRM:\tAlarm tripped. PAUTH_DEBUG_0 bit 0 is set. Reset to remove effects.\n");
        return true; 
    }
    else
    { 
        Logging_Write(log_level_debug, "That didn't work. There's some other way to do it, or this is a Rev B02 card or earlier.\n");
        return false;
    }

    return false; 
}

// Todo: Universal vbios dump function
bool nv1_dump_vbios()
{
    Logging_Write(log_level_message, "Dumping Video BIOS...");

    FILE* vbios = fopen("nv1bios.bin", "wb");

    uint32_t vbios_bin[8192];

    for (int32_t i = 0; i < 8192; i++)
    {
        vbios_bin[i] = nv_mmio_read32(NV1_PROM + i*4);
    }   

    fwrite(vbios_bin, sizeof(vbios_bin), 1, vbios);

    fclose(vbios);
    Logging_Write(log_level_message, "Done!\n");

    return true; 
}

bool nv1_gpus_section_applies(uint32_t fourcc)
{
    return (fourcc == gpus_section_nv1e);
}