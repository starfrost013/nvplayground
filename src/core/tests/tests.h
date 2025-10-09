/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    tests.h: Global tests define
*/

#pragma once
#include "config/config.h"
#include <nvplay.h>

// needed because config.h depends on us here
typedef struct nv_config_test_entry_s nv_config_test_entry_t;

/* Defines a single test (for the global list of tests) */
typedef struct nv_test_s 
{
    uint32_t required_vendor_id;
    uint32_t required_device_id;
    const char* name;
    const char* name_friendly;                          // Name presented to the user.
    bool (*test_function)(); 
} nv_test_t; 

extern nv_test_t nv_tests[];

bool Test_IsAvailableForGPU(const char* test_name);
nv_config_test_entry_t* Test_Get(const char* test_name);             // Get a test

bool Test_Run(nv_config_test_entry_t* test);
