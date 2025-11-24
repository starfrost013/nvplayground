/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv10.h: Nvidia "Celsius" architecture:
        NV10        GeForce 256 (1999)
        NV11        GeForce 2 MX (2000)
        NV15        GeForce 2 (2000)
        NV17        GeForce 4 MX (2002)
        NV18        GeForce 4 MX (AGP 8X) (2002?)
        NV19        GeForce 4 MX (PCIe bridge chip) (2004)
        NV1A        Crush-11 (GeForce2IGP, Xbox prototype) (2001)

    The last and the greatest of the fixed function era. Full TnL & DirectX 7 capable. Only basic support is provided because these are very complex beasts
    (No 097 mode for you)
*/

#include <nvplay.h>

typedef struct nv10_state_s
{
    uint32_t original_pll_setting;  
} nv10_state_t; 

extern nv10_state_t nv10_state;

// For Celsius, a *LOT* of VRAM is supported.
// Let's only map what is required for this one. Sometimes we may need to check unmapped memory, so we can force 128 MB to be mapped via a command-line (or config? option)

#define NV10_PMC_BOOT                   0x0
#define NV10_PMC_INTR                   0x100
#define NV10_PMC_INTR_EN                0x140
#define NV10_PMC_INTR_EN_SOFTWARE       0x1
#define NV10_PMC_INTR_EN_HARDWARE       0x2

#define NV10_PMC_ENABLE                 0x200

#define NV10_PFB_CFG                    0x100200
#define NV10_PFB_CSTATUS                0x10020C
#define NV10_PSTRAPS                    0x101000

// (1mb*value)
#define NV10_PFB_CSTATUS_VRAM           20
#define NV10_PFB_CSTATUS_VRAM_2MB       0x2
#define NV10_PFB_CSTATUS_VRAM_4MB       0x4
#define NV10_PFB_CSTATUS_VRAM_8MB       0x8
#define NV10_PFB_CSTATUS_VRAM_16MB      0x10
#define NV10_PFB_CSTATUS_VRAM_32MB      0x20
#define NV10_PFB_CSTATUS_VRAM_64MB      0x40
#define NV10_PFB_CSTATUS_VRAM_128MB     0x80

#define NV10_PRAMDAC_CLOCK_CORE                          0x680500        // NVPLL
#define NV10_PRAMDAC_CLOCK_MEMORY                        0x680504
#define NV10_PRAMDAC_CLOCK_PIXEL                         0x680508
#define NV10_PRAMDAC_COEFF_SELECT                        0x68050C
#define NV10_PRAMDAC_COEFF_SELECT_VPLL_SOURCE            0
#define NV10_PRAMDAC_COEFF_SELECT_VPLL_SOURCE_XTAL       0x0
#define NV10_PRAMDAC_COEFF_SELECT_VPLL_SOURCE_VIP        0x1
#define NV10_PRAMDAC_COEFF_SELECT_SOURCE                 8               // Bit not set = hardware, otherwise software
#define NV10_PRAMDAC_COEFF_SELECT_MPLL_IS_SOFTWARE       0x1
#define NV10_PRAMDAC_COEFF_SELECT_VPLL_IS_SOFTWARE       0x2
#define NV10_PRAMDAC_COEFF_SELECT_NVPLL_IS_SOFTWARE      0x4
#define NV10_PRAMDAC_COEFF_SELECT_ALL_SOFTWARE           0x7
#define NV10_PRAMDAC_COEFF_SELECT_VS_PCLK_TV             16
#define NV10_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_NONE        0x0 
#define NV10_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_VSCLK       0x1 
#define NV10_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_PCLK        0x2 
#define NV10_PRAMDAC_COEFF_SELECT_VS_PCLK_TV_BOTH        0x3 
#define NV10_PRAMDAC_COEFF_SELECT_TVCLK_SOURCE           20  
#define NV10_PRAMDAC_COEFF_SELECT_TVCLK_SOURCE_EXT       0x0
#define NV10_PRAMDAC_COEFF_SELECT_TVCLK_SOURCE_VIP       0x1             // VIP = Video Interface Port / Mediaport
#define NV10_PRAMDAC_COEFF_SELECT_TVCLK_RATIO            24 
#define NV10_PRAMDAC_COEFF_SELECT_TVCLK_RATIO_DB1        0x0
#define NV10_PRAMDAC_COEFF_SELECT_TVCLK_RATIO_DB2        0x1 
#define NV10_PRAMDAC_COEFF_SELECT_VCLK_RATIO             28 
#define NV10_PRAMDAC_COEFF_SELECT_VCLK_RATIO_DB1         0x0 
#define NV10_PRAMDAC_COEFF_SELECT_VCLK_RATIO_DB2         0x1 

#define NV10_MMIO_SIZE                                   0x2000000
#define NV10_RAMIN_START                                 0x700000

#define NV10_PSTRAPS_CRYSTAL                             6
#define NV10_PSTRAPS_CRYSTAL_13500K                      0x0
#define NV10_PSTRAPS_CRYSTAL_14318180                    0x1

bool nv10_init();
void nv10_shutdown(); 