/* tests.h: Defines tests */

#pragma once
#include <nvplayground.h>
#include <core/nvcore.h>

/* Defines a single test (for the global list of tests) */
typedef struct nv_test_s 
{
    uint32_t required_device_id;
    uint32_t required_vendor_id;
    const char* name;
    const char* name_friendly;                          // Name presented to the user.
    bool (*test_function)(); 
} nv_test_t; 

extern nv_test_t nv_tests[];

bool Test_IsAvailableForGPU(const char* test_name);
nv_test_t* Test_Get(const char* test_name);             // Get a test