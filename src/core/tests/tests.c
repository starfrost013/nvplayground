/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    tests.c: Enumerates all available tests in test mode
*/

#include "nvplay.h"
#include <string.h>

#include <core/tests/tests.h>
#include <config/config.h>

// Architecture includes
#include <architecture/generic/nv_generic.h>
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>
#include <architecture/nv10/nv10.h>

nv_test_t nv_tests[] = 
{
    // Generic tests
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpPCI", "NV Generic - Dump PCI", NVGeneric_DumpPCISpace},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpMMIO", "NV Generic - Dump MMIO", NVGeneric_DumpMMIO},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpVBIOS", "NV Generic - Dump VBIOS", NVGeneric_DumpVBIOS},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpFIFOa", "NV Generic - Dump FIFO State", NVGeneric_DumpVBIOS},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpRAMHT", "NV Generic - Dump RAMHT", NVGeneric_DumpRAMHT},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpRAMFC", "NV Generic - Dump RAMFC", NVGeneric_DumpRAMFC},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpRAMRO", "NV Generic - Dump RAMRO", NVGeneric_DumpRAMRO},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpCACHE", "NV Generic - Dump on-die cache", NVGeneric_DumpPGRAPHCache},

    // NV1 has two vendor ids
    { PCI_VENDOR_SGS, PCI_DEVICE_NV1_NV, "NV1_PrintMfgInfo", "NV1 Print Manufacturing Info", nv1_print_info},
    { PCI_VENDOR_NV, PCI_DEVICE_NV1_NV, "NV1_PrintMfgInfo", "NV1 Print Manufacturing Info", nv1_print_info},
    { PCI_VENDOR_SGS, PCI_DEVICE_NV1_NV, "NV1_SecurityBreach", "NV1 DRM Enable", nv1_security_breach},
    { PCI_VENDOR_NV, PCI_DEVICE_NV1_NV, "NV1_SecurityBreach", "NV1 DRM Enable", nv1_security_breach},

    // NV3 tests
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_PrintMfgInfo", "NV3 Print Manufacturing Info", nv3_dump_mfg_info},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_PrintMfgInfo", "NV3 Print Manufacturing Info", nv3_dump_mfg_info},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_SetOverclock", "NV3 Overclock Torture", nv3_test_overclock},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_SetOverclock", "NV3 Overclock Torture", nv3_test_overclock},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3, "NV3_GarbageMMIORead", "NV3 Garbage Test", nv3_garbage_mmio_read},
    { PCI_VENDOR_SGS_NV, PCI_DEVICE_NV3T_ACPI, "NV3_GarbageMMIORead", "NV3 Garbage Test", nv3_garbage_mmio_read},

    // NV4 tests

    { 0x0000, 0x0000, "", "", NULL}, // Sentinel value, do not remove
};

/* Checks if a given test (represented by the test_name parameter) is available for the detected graphics hardware*/
bool Test_IsAvailableForGPU(const char* test_name)
{
    // do we need to sanity check these?
    uint32_t vendor_id = current_device.device_info.vendor_id;
    uint32_t device_id = current_device.real_device_id;

    uint32_t test_number = 0; 
    nv_test_t nv_test = nv_tests[test_number];

    //make this code a bit faster
    bool need_inner_loop = true;

    if (current_device.device_info.device_id_start == current_device.device_info.device_id_end)
        need_inner_loop = false;

    // iterate through each test
    while (nv_test.test_function != NULL)
    {
        bool right_test = !strcmp(nv_test.name, test_name)
        && vendor_id == nv_test.required_vendor_id;

        if (right_test)
        {
            if (!need_inner_loop
            && device_id == nv_test.required_device_id)
                return true; 
            else 
            {
                for (uint32_t device_id = current_device.device_info.device_id_start; device_id <= current_device.device_info.device_id_end; device_id++)
                {
                    if (device_id == nv_test.required_device_id)
                        return true; 
                }
            }
        }


        test_number++;
        nv_test = nv_tests[test_number];
    }

    return false; 
}

/* Acquires the test with the name test_name. The test must be loaded and supported */
nv_config_test_entry_t* Test_Get(const char* test_name)
{
    nv_config_test_entry_t* test_entry = config.test_list_head;

    // iterate through each test
    while (test_entry)
    {
        if (!strcmp(test_entry->name, test_name))
            return test_entry;

        test_entry = test_entry->next;
    }

    return NULL; 
}

bool Test_Run(nv_config_test_entry_t* test)
{
	/* 
		TODO: Ini setting to disable this print in the case of graphical tests.
		Otherwise we'll have to switch back to test mode every test.

		Also, test logging. (after util_logging.c is done)
	*/
	if (test->test_function)
	{
		bool success = test->test_function();	

		if (success)
			Logging_Write(log_level_message, "Test %s succeeded\n", test->name);
		else
			Logging_Write(log_level_message, "Test %s failed! :(\n", test->name);
	
        return success; 
    }
    
    //should never happen because we should explicitly check for this
    return false; 
}