/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    alpine.h: Cirrus Logic CL-GD543X/4X "Alpine" VisualMedia Accelerator
*/

#pragma once
#include <nvplay.h>


// Type 0 (32-bit)
// Bit 3 = 0, for not prefetchable (Only Bits 31:23 matter for BAR, it must begin on)
#define CIRRUS_MMIO_SPACE_TEST                  0x84000000

bool Alpine_Init();
void Alpine_Shutdown();

#define CIRRUS_MMIO_SIZE_REV_A                  0x1000000   // Lower half is big endian (we don't care about it)
#define CIRRUS_MMIO_SIZE_REV_B                  0x2000000   // Lower half is big endian (we don't care about it)

#define CIRRUS_ALPINE_SR_APERTURE               0x7
#define CIRRUS_ALPINE_SR_APERTURE_ENABLE        4

#define CIRRUS_INT_INQUIRE                      0x12
#define CIRRUS_INT_INQUIRE_CHIP_REVISION        0x82

#define CIRRUS_5446_REV_A                       0x00
#define CIRRUS_5446_REV_B                       0x01