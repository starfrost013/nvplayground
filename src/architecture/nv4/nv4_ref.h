/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4_ref.h: Nv4 hardware defines. A lot of this is shared with NV5, so NV5 defs will layer on top of NV4 defs. When writing nv5 code, use nv5 def if both exist.
*/


#pragma once

//
// General
//
#define NV4_VRAM_SIZE_2MB                               0x200000 // 2MB (never used; NV4 only)
#define NV4_VRAM_SIZE_4MB                               0x400000 // 4MB (never used)
#define NV4_VRAM_SIZE_8MB                               0x800000 // 8MB
#define NV4_VRAM_SIZE_16MB                              0x1000000 // 16MB
#define NV5_VRAM_SIZE_32MB                              0x2000000 // NV5 only

#define NV4_MMIO_SIZE                                   0x1000000 // not sure. May be larger!!!!

//
// PMC
//

#define NV4_PMC_START                                   0x0

#define NV4_PMC_INTR                                    0x100
#define NV4_PMC_INTR_PMEDIA_PENDING                     4
#define NV4_PMC_INTR_PFIFO_PENDING                      8
#define NV4_PMC_INTR_PGRAPH_PENDING                     12
#define NV4_PMC_INTR_PVIDEO_PENDING                     16
#define NV4_PMC_INTR_PTIMER_PENDING                     20
#define NV4_PMC_INTR_PCRTC_PENDING                      24
#define NV4_PMC_INTR_PBUS_PENDING                       28
#define NV4_PMC_INTR_SOFTWARE_PENDING                   31

#define NV4_PMC_INTR_EN                                 0x140
#define NV4_PMC_INTR_EN_DISABLED                        0x0
#define NV4_PMC_INTR_EN_SOFTWARE                        0x1
#define NV4_PMC_INTR_EN_HARDWARE                        0x2
#define NV4_PMC_INTR_EN_ALL                             0x3

#define NV4_PMC_BOOT                                    0x0
#define NV4_PMC_ENABLE                                  0x200
#define NV4_PMC_ENABLE_PMEDIA                           4               // Enable mediaport external MPEG decoder engine
#define NV4_PMC_ENABLE_PFIFO                            8               // Enable FIFO
#define NV4_PMC_ENABLE_PGRAPH                           12
#define NV4_PMC_ENABLE_PPMI                             16
#define NV4_PMC_ENABLE_PFB                              20
#define NV4_PMC_ENABLE_PCRTC                            24
#define NV4_PMC_ENABLE_PVIDEO                           28

//
// PFB
//

#define NV4_PFB_START                                   0x100000
#define NV4_PFB_BOOT                                    0x100000
#define NV4_PFB_BOOT_RAM_AMOUNT                         0
#define NV4_PFB_BOOT_RAM_AMOUNT_2MB                     0x0
#define NV4_PFB_BOOT_RAM_AMOUNT_4MB                     0x1
#define NV4_PFB_BOOT_RAM_AMOUNT_8MB                     0x2
#define NV4_PFB_BOOT_RAM_AMOUNT_16MB                    0x3
#define NV5_PFB_BOOT_RAM_AMOUNT_32MB                    0x0

#define NV4_PSTRAPS                                     0x101000

//
// PRAMDAC
//

#define NV4_PRAMDAC_START                               0x680300
#define NV4_PRAMDAC_CURSOR_START_POSITION               0x680300

#define NV4_PRAMDAC_CURSOR_SIZE_X                       32
#define NV4_PRAMDAC_CURSOR_SIZE_Y                       32

// Same for all 3 clocks
#define NV4_PRAMDAC_CLOCK_VDIV                          0
#define NV4_PRAMDAC_CLOCK_NDIV                          8
#define NV4_PRAMDAC_CLOCK_PDIV                          16

#define NV4_PRAMDAC_CLOCK_CORE                          0x680500        // NVPLL
#define NV4_PRAMDAC_CLOCK_MEMORY                        0x680504
#define NV4_PRAMDAC_CLOCK_PIXEL                         0x680508
#define NV4_PRAMDAC_COEFF_SELECT                        0x68050C
#define NV4_PRAMDAC_COEFF_SELECT_VPLL_SOURCE            0
#define NV4_PRAMDAC_COEFF_SELECT_VPLL_SOURCE_XTAL       0x0
#define NV4_PRAMDAC_COEFF_SELECT_VPLL_SOURCE_VIP        0x1
#define NV4_PRAMDAC_COEFF_SELECT_SOURCE                 8               // Bit not set = hardware, otherwise software
#define NV4_PRAMDAC_COEFF_SELECT_MPLL_IS_SOFTWARE       0x1
#define NV4_PRAMDAC_COEFF_SELECT_VPLL_IS_SOFTWARE       0x2
#define NV4_PRAMDAC_COEFF_SELECT_NVPLL_IS_SOFTWARE      0x4
#define NV4_PRAMDAC_COEFF_SELECT_ALL_SOFTWARE           0x7
#define NV4_PRAMDAC_COEFF_SELECT_VS_PCLK_TV             16
#define NV4_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_NONE        0x0 
#define NV4_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_VSCLK       0x1 
#define NV4_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_PCLK        0x2 
#define NV4_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_BOTH        0x3 
#define NV4_PRAMDAC_COEFF_SELECT_TVCLK_SOURCE           20  
#define NV4_PRAMDAC_COEFF_SELECT_TVCLK_SOURCE_EXT       0x0
#define NV4_PRAMDAC_COEFF_SELECT_TVCLK_SOURCE_VIP       0x1             // VIP = Video Interface Port / Mediaport
#define NV4_PRAMDAC_COEFF_SELECT_TVCLK_RATIO            24 
#define NV4_PRAMDAC_COEFF_SELECT_TVCLK_RATIO_DB1        0x0
#define NV4_PRAMDAC_COEFF_SELECT_TVCLK_RATIO_DB2        0x1 
#define NV4_PRAMDAC_COEFF_SELECT_VCLK_RATIO             28 
#define NV4_PRAMDAC_COEFF_SELECT_VCLK_RATIO_DB1         0x0 
#define NV4_PRAMDAC_COEFF_SELECT_VCLK_RATIO_DB2         0x1 

#define NV4_PRAMDAC_GENERAL_CONTROL                     0x680600
#define NV4_PRAMDAC_GENERAL_CONTROL_ALT_MODE            12


#define NV4_RAMIN_START                                0x700000