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
void NVGeneric_DumpPGRAPHCache_NV3(FILE* stream);   // NV3-NV4 CACHE

// I considered using an array, but we'd have to define hardcoded indicies anyway so we can just define these entries
extern nvhal_entry_t nvhal_nv1;
extern nvhal_entry_t nvhal_nv2;
extern nvhal_entry_t nvhal_nv3;
extern nvhal_entry_t nvhal_nv4;
extern nvhal_entry_t nvhal_celsius;