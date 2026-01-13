/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv_generic_hal.c: Implements HAL functions (todo: C++ later)
*/

#include <architecture/nvidia/kernel/nv_generic.h>
#include <architecture/nvidia/nv1/nv1.h>
#include <architecture/nvidia/nv3/nv3.h>
#include <architecture/nvidia/nv4/nv4.h>
#include <architecture/nvidia/nv10/nv10.h>
#include <architecture/virge/virge.h>
#include <architecture/cirrus/alpine.h>

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

    // Kernel functions
    NULL,                           // Service interrupts
    NULL,                           // FIFO init
    NULL,                           // PGRAPH init
    NULL,                           // PGRAPH reset
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

    // Kernel functions
    NULL,                           // Service interrupts
    NULL,                           // FIFO init
    NULL,                           // PGRAPH init
    NULL,                           // PGRAPH reset
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

    // Kernel functions
    NV4_InterruptService,           // Service interrupts
    NV4_InitFIFO,                   // FIFO init
    NV4_InitGraph,                  // PGRAPH init
    NV4_ResetGraph,                 // PGRAPH reset
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

    // Kernel functions
    NULL,                           // Service interrupts
    NULL,                           // FIFO init
    NULL,                           // PGRAPH init
    NULL,                           // PGRAPH reset
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};

// S3 ViRGE test hal
nvhal_entry_t nvhal_virge =
{
    // Init/shutdown functions
    ViRGE_Init,                      // Init
    ViRGE_Shutdown,                  // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Kernel functions
    NULL,                           // Service interrupts
    NULL,                           // FIFO init
    NULL,                           // PGRAPH init
    NULL,                           // PGRAPH reset
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};

// Cirrus Logic "Alpine" test hal
nvhal_entry_t nvhal_alpine =
{
    // Init/shutdown functions
    Alpine_Init,                     // Init
    Alpine_Shutdown,                 // Shutdown

    // Dump functions
    NULL,                           // Dump FIFO to text file
    NULL,                           // Dump RAMHT to text file 
    NULL,                           // Dump RAMFC to text file
    NULL,                           // Dump RAMRO to text file
    NULL,                           // Dump PGRAPH_CACHE to text file

    // Kernel functions
    NULL,                           // FIFO init
    NULL,                           // PGRAPH init
    NULL,                           // PGRAPH reset
    NULL,                           // Submit object in subchannel
    NULL,                           // Submit method for existing subchannel 
};