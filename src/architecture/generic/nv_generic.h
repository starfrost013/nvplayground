/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv_generic.h: Implements generic NV tests that apply to all or most GPUs
*/

#pragma once
#include <nvplay.h>

#define NV_MMIO_DUMP_FLUSH_FREQUENCY     65536
#define NV_MMIO_SIZE                     0x1000000       // Max MMIO size
#define NV5_MAX_VRAM_SIZE                0x2000000

bool NVGeneric_DumpPCISpace();
bool NVGeneric_DumpMMIO();
bool NVGeneric_DumpVBIOS();
bool NVGeneric_DumpFIFO();
bool NVGeneric_DumpRAMHT();                         // Dump all currently loaded objects in the current channel
bool NVGeneric_DumpRAMFC();                         // Dump all channels that are not context switched to
bool NVGeneric_DumpRAMRO();                         // Dump any errors that may have occurred 
bool NVGeneric_DumpPGRAPHCache();

// Hardware Abstraction Layer entry
// All hardware-specific stuff
typedef struct nvhal_entry_s
{
    bool (*init_function)();							// Function to call on entry point	
	void (*shutdown_function)();						// Function to call on shutdown

    // TEST functions
    void (*dump_fifo_to_text_file)();
    void (*dump_ramht_to_text_file)();
    void (*dump_ramfc_to_text_file)();
    void (*dump_ramro_to_text_file)();

    // UTILISATION functions
    void (*submit_object)(uint32_t name, uint32_t context);
    void (*submit_method)(uint32_t method, uint32_t param);
} nvhal_entry_t;   