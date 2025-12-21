/* 
    NVPlay
    Copyright © 2025-2026 starfrost

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
    NV1_Init,                       // Init
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
    NV3_Init,                       // Init
    NULL,                           // Shutdown

    // Dump functions
    NV3_DumpFIFO,                   // Dump FIFO to text file
    NV3_DumpRAMHT,                  // Dump RAMHT to text file 
    NV3_DumpRAMFC,                  // Dump RAMFC to text file
    NV3_DumpRAMRO,                  // Dump RAMRO to text file
    NV3_DumpPGRAPHCache,            // Dump PGRAPH_CACHE to text file

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};

// NV4-based GPU (NV4/NV5/NV6) HAL
nvhal_entry_t nvhal_nv4 =
{
    // Init/shutdown functions
    NV4_Init,                       // Init
    NV4_Shutdown,                   // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NV3_DumpPGRAPHCache,            // Dump PGRAPH_CACHE to text file (similar enough to nv3)

    // Rendering functions
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};

// Celsius (NV1x) HAL
nvhal_entry_t nvhal_celsius =
{
    // Init/shutdown functions
    NV10_Init,                      // Init
    NV10_Shutdown,                  // Shutdown

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