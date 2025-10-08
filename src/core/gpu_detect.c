/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_detect.c: Code for detecting graphics hardware
*/

#include <nvplay.h>

// The selected device after detection is done. 
nv_device_t current_device = {0}; 

bool GPU_Detect()
{
    nv_device_info_t current_device_info = supported_devices[0]; 
    int32_t i = 0; 

    // iterate through all the known device info sets
    while (current_device_info.vendor_id != 0x00)
    {
        current_device_info = supported_devices[i];
        
        Logging_Write(log_level_debug, "Trying to find GPU: %s\n", current_device_info.name);

        if (PCI_DevicePresent(current_device_info.device_id, current_device_info.vendor_id))
        {
            Logging_Write(log_level_message, "Detected GPU: %s\n", current_device_info.name);

            // set up current info
            current_device.device_info = current_device_info;
            return true; 
        }

        i++;
    }

    Logging_Write(log_level_error, "No supported Nvidia GPU found\n");
    return false; 

}

extern inline bool GPU_IsNV1()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV1_A01
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV1_C01);
}

extern inline bool GPU_IsNV2()
{
    return (current_device.nv_pmc_boot_0 == NV_PMC_BOOT_NV2_A01);
}

// Checks for NV3 *AND* NV3T
extern inline bool GPU_IsNV3()
{
    // NV3 has multiple manufacturing fabs, but also shares the architecture with certain NV4 steppings , so we have to be slightly clever and check for 0x30
    return ((current_device.nv_pmc_boot_0 & 0xFFFFF) >> 12) == 0x30;
}

// Checks for NV3 only
extern bool GPU_IsNV3AorB()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV3_A00 
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV3_B00);
}

extern bool GPU_IsNV3T()
{
    return ((current_device.nv_pmc_boot_0 & 0xFFFFF) >> 12) == 0x30
    && (current_device.nv_pmc_boot_0 & 0xFF) >= 0x20; // Revision C
}

extern bool GPU_IsNV4()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV4_A01
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV4_A05);
}

extern bool GPU_IsNV4orBetter()
{
    // Special-case NV4 a1 (even though it was probably only ever an ES) since its fabless ID number is less(!) than NV1/2/3!
    return (current_device.nv_pmc_boot_0 == NV_PMC_BOOT_NV4_A01
    || (current_device.nv_pmc_boot_0 & 0xFFFFFFF) >= 0x34001);
}

extern inline bool GPU_IsNV5()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV5_A01
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV5_B03);
}

extern inline bool GPU_IsNV10()
{
    // No pre-NV10 gpu has >=0x1000000 if fab removed
    return (current_device.nv_pmc_boot_0 & 0xFFFFFFF) >= NV_PMC_BOOT_NV10_BASE;
}