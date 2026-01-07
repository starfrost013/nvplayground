/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    virge.h: S3 ViRGE definitions
*/

#pragma once
#include <nvplay.h>


// Type 0 (32-bit)
// Bit 3 = 0, for not prefetchable (Only Bits 31:23 matter for BAR, it must begin on)
#define VIRGE_MMIO_SPACE_TEST       0x84000000

bool ViRGE_Init();
void ViRGE_Shutdown();

#define S3VIRGE_BAR0_SIZE           0x4000000   // Lower half is big endian (we don't care about it)

#define S3VIRGE_SUBSYSTEM_STATUS    0x1008504   // S3D Subsystem Status