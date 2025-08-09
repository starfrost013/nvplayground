/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    tests.c: Enumerates all available tests in test mode
*/

#include <string.h>

#include <core/tests/tests.h>

// Architecture includes
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>

nv_test_t nv_tests[] = 
{
    // NV1 has two vendor ids
    { PCI_VENDOR_SGS, PCI_DEVICE_NV1_NV, "NV1_DumpPCICfgSpace", "NV1 Dump PCI Config Space", nv1_print_pci_config},
    { PCI_VENDOR_NV, PCI_DEVICE_NV1_NV, "NV1_DumpPCICfgSpace", "NV1 Dump PCI Config Space", nv1_print_pci_config},
    { PCI_VENDOR_SGS, PCI_DEVICE_NV1_NV, "NV1_PrintMfgInfo", "NV1 Print Manufacturing Info", nv1_print_info},
    { PCI_VENDOR_NV, PCI_DEVICE_NV1_NV, "NV1_PrintMfgInfo", "NV1 Print Manufacturing Info", nv1_print_info},
    { PCI_VENDOR_SGS, PCI_DEVICE_NV1_NV, "NV1_SecurityBreach", "NV1 DRM Enable", nv1_security_breach},
    { PCI_VENDOR_NV, PCI_DEVICE_NV1_NV, "NV1_SecurityBreach", "NV1 DRM Enable", nv1_security_breach},
    { PCI_VENDOR_SGS, PCI_DEVICE_NV1_NV, "NV1_DumpVBIOS", "NV1 Dump Video BIOS (Real Mode)", nv1_dump_vbios},
    { PCI_VENDOR_NV, PCI_DEVICE_NV1_NV, "NV1_DumpVBIOS", "NV1 Dump Video BIOS (Real Mode)", nv1_dump_vbios},

    // NV3 tests
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_PrintMfgInfo", "NV3 Print Manufacturing Info", nv3_dump_mfg_info},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_PrintMfgInfo", "NV3 Print Manufacturing Info", nv3_dump_mfg_info},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_SetOverclock", "NV3 Overclock Torture", nv3_test_overclock},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_SetOverclock", "NV3 Overclock Torture", nv3_test_overclock},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_DumpVBIOS", "NV3 Dump Video BIOS (Real Mode)", nv3_dump_vbios},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_DumpVBIOS", "NV3 Dump Video BIOS (Real Mode)", nv3_dump_vbios},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_DumpMMIO", "NV3 Full BAR0/BAR1 Dump", nv3_dump_mmio},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_DumpMMIO", "NV3 Full BAR0/BAR1 Dump", nv3_dump_mmio},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_GarbageMMIORead", "NV3 Garbage Test", nv3_garbage_mmio_read},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_GarbageMMIORead", "NV3 Garbage Test", nv3_garbage_mmio_read},

    // NV4 tests
    { PCI_VENDOR_NV, PCI_DEVICE_NV4, "NV4_DumpVBIOS", "NV3 Dump Video BIOS (Real Mode)", nv4_dump_vbios},
    { PCI_VENDOR_NV, PCI_DEVICE_NV4, "NV4_PrintMfgInfo", "NV3 Print Manufacturing Info", nv4_dump_mfg_info},
    { PCI_VENDOR_NV, PCI_DEVICE_NV4, "NV4_DumpMMIO", "NV3 Full BAR0/BAR1 Dump", nv4_dump_mmio},

    { 0x0000, 0x0000, "", "", NULL}, // Sentinel value, do not remove
};

/* Checks if a given test (represented by the test_name parameter) is available for the detected graphics hardware*/
bool Test_IsAvailableForGPU(const char* test_name)
{
    // do we need to sanity check these?
    uint32_t vendor_id = current_device.device_info.vendor_id;
    uint32_t device_id = current_device.device_info.device_id;

    uint32_t test_number = 0; 

    nv_test_t nv_test = nv_tests[test_number];

    // iterate through each test
    while (nv_test.test_function != NULL)
    {
        if (!strcmp(nv_test.name, test_name)
        && vendor_id == nv_test.required_vendor_id
        && device_id == nv_test.required_device_id)
            return true; 

        test_number++;
        nv_test = nv_tests[test_number];
    }

    return false; 
}

/* Acquires the test with the name test_name */
nv_test_t* Test_Get(const char* test_name)
{
    uint32_t test_number = 0; 
    nv_test_t nv_test = nv_tests[test_number];

    // iterate through each test
    while (nv_test.test_function != NULL)
    {
        if (!strcmp(nv_test.name, test_name))
            return &nv_tests[test_number];

        test_number++;
        nv_test = nv_tests[test_number];

    }

    return NULL; 
}