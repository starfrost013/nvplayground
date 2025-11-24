/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv_generic_hal.c: Implements HAL functions (todo: C++ later)
*/

#include <architecture/generic/nv_generic.h>
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>
#include <architecture/nv10/nv10.h>

nvhal_entry_t nvhal_entries[] =
{
    // NV1
    { 
        nv1_init,
        NULL,
    },

    // NV2
    {
        NULL,
        NULL, 
    },

    // NV3
    {

    },

    // NV4
    {

    },

    // NV5
    {

    },

    // NV10
    {

    },
};