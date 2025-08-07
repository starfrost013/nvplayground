/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv1_ref.h: Provides NV1 hardware defines. Simplified version of vid_nv3.h from 86Box-riva128
*/

#pragma once 

#include <stdbool.h>
#include <stdint.h>

//
// PCI Bus Information
//

#define NV1_PCI_BAR0_SIZE                       0xFFFFFF

// 
// VRAM
//
#define NV1_VRAM_SIZE_1MB                       0x100000
#define NV1_VRAM_SIZE_2MB                       0x200000
#define NV1_VRAM_SIZE_4MB                       0x400000

//
// PMC
//
#define NV1_PMC_BOOT_0                          0x0
#define NV1_PMC_BOOT_0_REVISION                 0

#define NV1_PMC_BOOT_0_REVISION_A               0x0             // Prototype (1994)
#define NV1_PMC_BOOT_0_REVISION_B1              0x0             // Prototype (early 1995)
#define NV1_PMC_BOOT_0_REVISION_B2              0x0             // Prototype (mid 1995)
#define NV1_PMC_BOOT_0_REVISION_B3              0x0             // Final?
#define NV1_PMC_BOOT_0_REVISION_C               0x0             // Final?

#define NV1_PMC_BOOT_0_IMPLEMENTATION           8  
#define NV1_PMC_BOOT_0_IMPLEMENTATION_NV0       1               // Nvidia Hardware Simulator (1993-1994)
#define NV1_PMC_BOOT_0_IMPLEMENTATION_NV1_D32   2               // NV1 + DRAM + SGS-Thomson STG-1732/1764 DAC
#define NV1_PMC_BOOT_0_IMPLEMENTATION_NV1_V32   3               // NV1 + VRAM + SGS-Thomson STG-1732/1764 DAC
#define NV1_PMC_BOOT_0_IMPLEMENTATION_PICASSO   4               // NV1 + VRAM + NV 128-bit DAC


// Defines the NV architecture version (NV1/NV2/...)
#define NV1_PMC_BOOT_0_ARCHITECTURE             16
#define NV1_PMC_BOOT_0_ARCHITECTURE_NV0         0               // Nvidia Hardware Simulator (1993-1994)   
#define NV1_PMC_BOOT_0_ARCHITECTURE_NV1         1               // NV1 (1995)
#define NV1_PMC_BOOT_0_ARCHITECTURE_NV2         2               // Mutara (1996, cancelled)

#define NV1_PMC_DEBUG_0                         0x80

#define NV1_PMC_INTR_0                          0x100
#define NV1_PMC_INTR_EN_0                       0x140

#define NV1_PMC_INTR_EN_0_INTA                  0
#define NV1_PMC_INTR_EN_0_INTB                  4
#define NV1_PMC_INTR_EN_0_INTC                  8
#define NV1_PMC_INTR_EN_0_INTD                  12

#define NV1_PMC_INTR_EN_0_DISABLED              0x0
#define NV1_PMC_INTR_EN_0_HARDWARE              0x1
#define NV1_PMC_INTR_EN_0_SOFTWARE              0x2
#define NV1_PMC_INTR_EN_0_ALL                   0x3             // (HARDWARE | SOFTWARE)

#define NV1_PMC_INTR_READ                       0x160

//TODO: DEFINE bits
#define NV1_PMC_ENABLE                          0x200

//
// PAUTH
// Scary nvidia mode
//

// Read only
#define NV1_PAUTH_DEBUG_0                           0x605080
#define NV1_PAUTH_DEBUG_0_BREACH_DETECTED           0
#define NV1_PAUTH_DEBUG_0_EEPROM_INVALID            4

#define NV1_PAUTH_CHIP_TOKEN_0                      0x605400
#define NV1_PAUTH_CHIP_TOKEN_1                      0x605404
#define NV1_PAUTH_PASSWORD_0(i)                     0x605800+(i*16)
#define NV1_PAUTH_PASSWORD_1(i)                     0x605804+(i*16)
#define NV1_PAUTH_PASSWORD_2(i)                     0x605808+(i*16)
#define NV1_PAUTH_PASSWORD_3(i)                     0x60580C+(i*16)

#define NV1_PAUTH_PASSWORD_SIZE                     128

// 
// PFB
//

#define NV1_PFB_BOOT_0                              0x600000

#define NV1_PFB_BOOT_0_RAM_AMOUNT                   0
#define NV1_PFB_BOOT_0_RAM_AMOUNT_1MB               0x0
#define NV1_PFB_BOOT_0_RAM_AMOUNT_2MB               0x1
#define NV1_PFB_BOOT_0_RAM_AMOUNT_4MB               0x2

//
// PEXTDEV
//

#define NV1_STRAPS                                  0x608000
#define NV1_STRAPS_STRAP_VENDOR                     0

//
// PRAM+RAMIN
//

#define NV1_PRAM_CONFIG                             0x602200
#define NV1_PRAM_CONFIG_SIZE                        0
#define NV1_PRAM_CONFIG_12KB                        0
#define NV1_PRAM_CONFIG_20KB                        1
#define NV1_PRAM_CONFIG_36KB                        2
#define NV1_PRAM_CONFIG_68KB                        3

// Position of RAMPW in RAMIN
#define NV1_RAMPW_POSITION_CONFIG0                  0x2c00
#define NV1_RAMPW_POSITION_CONFIG1                  0x4c00
#define NV1_RAMPW_POSITION_CONFIG2                  0x8c00
#define NV1_RAMPW_POSITION_CONFIG3                  0x10c00

// Static RAMPW mirror
#define NV1_PRAMPW                                  0x606000
#define NV1_RAMPW_SIZE                              0x400

//
// PROM
//
#define NV1_PROM                                    0x601000
#define NV1_PROM_SIZE                               32768
