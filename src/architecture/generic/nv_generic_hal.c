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

// NV1 NVHAL entry
nvhal_entry_t nvhal_nv1 =
{
    // Init/shutdown functions
    nv1_init,                       // Init
    NULL,                           // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel
}; 

// NV2 NVHAL entry
nvhal_entry_t nvhal_nv2 =
{
    // Init/shutdown functions
    NULL,                           // Init
    NULL,                           // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel              
};

nvhal_entry_t nvhal_nv3 =
{
    // Init/shutdown functions
    nv3_init,                       // Init
    NULL,                           // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};

// NV4-based GPU (NV4/NV5/NV6) HAL
nvhal_entry_t nvhal_nv4 =
{
    // Init/shutdown functions
    nv4_init,                       // Init
    nv4_shutdown,                   // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};

// Celsius (NV1x) HAL
nvhal_entry_t nvhal_celsius =
{
    // Init/shutdown functions
    nv10_init,                      // Init
    nv10_shutdown,                  // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};