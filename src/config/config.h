/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    config.h: Config subsystem
*/

#pragma once 

#include <nvplay.h>
#include <core/tests/tests.h>
#include <util/util.h>

#define INI_FILE_NAME "nvplay.ini"
#define MAX_TEST_NAME_BUFFER_LEN    64

// This is a linked list.
// We can make a lot of simplifications by making some assumptions about or design; 
// e.g. we don't need to ever remove these since the enabled tests are enumerated at init.

// Holds information about tests that need to be run.
typedef struct nv_config_test_entry_s
{
    char name[MAX_TEST_NAME_BUFFER_LEN]; 
    struct nv_config_test_entry_s* prev; 
    struct nv_config_test_entry_s* next; 
    bool (*test_function)(); 

} nv_config_test_entry_t; 


// Main config struct 
typedef struct nv_config_s
{
    bool loaded;                                    // Determines if the configuration file has been loaded.
    ini_t ini_file;
    nv_config_test_entry_t* test_list_head;         // The first test entry.
    nv_config_test_entry_t* test_list_tail;         // The last test entry.
    uint32_t num_tests_enabled;                     // The number of enabled tests.

    // Debug settings
    bool nv10_always_map_128m;                      // NV1x: Always map 128MB
} nv_config_t;

extern nv_config_t config; 

bool Config_Load();