/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    config.c: Config system (nvplay.ini) implementation
*/

#include "config.h"
#include "core/tests/tests.h"
#include "nvplay.h"
#include <util/util.h>
#include <config/config.h>

// Globals
nv_config_t config = {0}; 

// Functions
bool Config_Load()
{
    config.ini_file = ini_read(INI_FILE_NAME);

    if (!config.ini_file)
        return false; 

    Logging_Write(log_level_message, "Loaded nvplay.ini\n");

    // load debug settings
    ini_section_t section_debug = ini_find_section(config.ini_file, "Debug");

    if (section_debug)
        config.nv10_always_map_128m = ini_section_get_int(section_debug, "NV10_AlwaysMapFullBAR1", false);

    ini_section_t section_tests = ini_find_section(config.ini_file, "Tests");

    if (section_tests)
    {
        uint32_t test_id = 0;
        nv_test_t* current_test = &nv_tests[test_id];

        while (current_test->test_function)
        {
            // see if the tests specified in the ini file are for the GPU we have installed

            if (ini_has_entry(section_tests, current_test->name))
            {
                // call ini_section_get_int so we don't have to iterate through the INI sections again
                int32_t enabled = ini_section_get_int(section_tests, current_test->name, 0);

                // check if it's enabled. if it's not just skip
                if (!enabled)
                    Logging_Write(log_level_debug, "Test %s disabled (2)\n", current_test->name);
                else
                {
                    // Check the PCI device ID range of the test
                    bool test_is_available = (current_device.device_info.vendor_id == current_test->required_vendor_id
                    && (current_device.device_info.device_id_start >= current_test->required_device_id
                    || current_device.device_info.device_id_end <= current_test->required_device_id));

                    // Allow a generic test to be used anywhere
                    if (current_test->required_device_id == PCI_DEVICE_GENERIC
                    && current_test->required_vendor_id == PCI_VENDOR_GENERIC)
                        test_is_available = true; 

                    // apply the run all tests cmd line option
                    if (command_line.run_all_tests)
                        test_is_available = true; 

                    if (!current_test->test_function)
                    {
                        Logging_Write(log_level_warning, "The test %s (%s) does not have defined test function!\n", current_test->name, current_test->name_friendly);
                        test_is_available = false;
                    }

                    if (test_is_available)
                    {
                        nv_config_test_entry_t* new_test_entry = calloc(1, sizeof(nv_config_test_entry_t));
                        new_test_entry->test = current_test;

                        /* 
                            set up the test list and add the new test to the test list
                            NOTE: we don't need to move things
                        */

                        config.num_tests_enabled++;

                        if (!config.test_list_head)
                        {
                            config.test_list_head = config.test_list_tail = new_test_entry;
                            config.test_list_head->prev = config.test_list_head->next = NULL;
                        }
                        else 
                        {
                            /* CASE: >= 1 element */
                            config.test_list_tail->next = new_test_entry;
                            new_test_entry->prev = config.test_list_tail;
                            new_test_entry->next = NULL;
                            config.test_list_tail = new_test_entry;
                        }
                    }
                }
            }    

            // check if the test is available

            test_id++;
            current_test = &nv_tests[test_id];
        }
    }
    
    config.loaded = true; 
    return true; 
};