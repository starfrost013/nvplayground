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

#define NV1_PCI_BAR0_SIZE                           0xFFFFFF

// 
// VRAM
//
#define NV1_VRAM_SIZE_1MB                           0x100000
#define NV1_VRAM_SIZE_2MB                           0x200000
#define NV1_VRAM_SIZE_4MB                           0x400000

//
// PMC
// cvbb v
#define NV1_PMC_BOOT_0                              0x0
#define NV1_PMC_BOOT_0_REVISION                     0

#define NV1_PMC_BOOT_0_REVISION_A                   0x0             // Prototype (1994)
#define NV1_PMC_BOOT_0_REVISION_B1                  0x0             // Prototype (early 1995)
#define NV1_PMC_BOOT_0_REVISION_B2                  0x0             // Prototype (mid 1995)
#define NV1_PMC_BOOT_0_REVISION_B3                  0x0             // Final?
#define NV1_PMC_BOOT_0_REVISION_C                   0x0             // Final?

#define NV1_PMC_BOOT_0_IMPLEMENTATION               8  
#define NV1_PMC_BOOT_0_IMPLEMENTATION_NV0           0x1             // Nvidia Hardware Simulator (1993-1994)
#define NV1_PMC_BOOT_0_IMPLEMENTATION_NV1_D32       0x2             // NV1 + DRAM + SGS-Thomson STG-1732/1764 DAC
#define NV1_PMC_BOOT_0_IMPLEMENTATION_NV1_V32       0x3             // NV1 + VRAM + SGS-Thomson STG-1732/1764 DAC
#define NV1_PMC_BOOT_0_IMPLEMENTATION_PICASSO       0x4             // NV1 + VRAM + NV 128-bit DAC


// Defines the NV architecture version (NV1/NV2/...)
#define NV1_PMC_BOOT_0_ARCHITECTURE                 16
#define NV1_PMC_BOOT_0_ARCHITECTURE_NV0             0x0             // Nvidia Hardware Simulator (1993-1994)   
#define NV1_PMC_BOOT_0_ARCHITECTURE_NV1             0x1             // NV1 (1995)
#define NV1_PMC_BOOT_0_ARCHITECTURE_NV2             0x2             // Mutara (1996, cancelled)

#define NV1_PMC_DEBUG_0                             0x80

#define NV1_PMC_INTR_0                              0x100
#define NV1_PMC_INTR_EN_0                           0x140

#define NV1_PMC_INTR_EN_0_INTA                      0
#define NV1_PMC_INTR_EN_0_INTB                      4
#define NV1_PMC_INTR_EN_0_INTC                      8
#define NV1_PMC_INTR_EN_0_INTD                      12

#define NV1_PMC_INTR_EN_0_DISABLED                  0x0
#define NV1_PMC_INTR_EN_0_HARDWARE                  0x1
#define NV1_PMC_INTR_EN_0_SOFTWARE                  0x2
#define NV1_PMC_INTR_EN_0_ALL                       0x3             // (HARDWARE | SOFTWARE)

#define NV1_PMC_INTR_READ                           0x160

//TODO: DEFINE bits
#define NV1_PMC_ENABLE                              0x200

//
// PRAMIN
//

#define NV1_RAMIN_START                            0x100000

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
// PFIFO
//

#define NV1_PFIFO_START                              0x2000
#define NV1_PFIFO_END                                0x3FFF

#define NV1_PFIFO_DELAY_0                            0x00002040 /* RW-4R */
#define NV1_PFIFO_DELAY_0_WAIT_RETRY                          7 /* RWIUF */
#define NV1_PFIFO_DELAY_0_WAIT_RETRY_0               0x00000000 /* RWI-V */
#define NV1_PFIFO_DEBUG_0                            0x00002080 /* R--4R */
#define NV1_PFIFO_DEBUG_0_CACHE_ERROR0                        0 /* R-XVF */
#define NV1_PFIFO_DEBUG_0_CACHE_ERROR0_NOT_PENDING   0x00000000 /* R---V */
#define NV1_PFIFO_DEBUG_0_CACHE_ERROR0_PENDING       0x00000001 /* R---V */
#define NV1_PFIFO_DEBUG_0_CACHE_ERROR1                             4 /* R-XVF */
#define NV1_PFIFO_DEBUG_0_CACHE_ERROR1_NOT_PENDING        0x00000000 /* R---V */
#define NV1_PFIFO_DEBUG_0_CACHE_ERROR1_PENDING            0x00000001 /* R---V */
#define NV1_PFIFO_INTR_0                                  0x00002100 /* RW-4R */
#define NV1_PFIFO_INTR_0_CACHE_ERROR                               0 /* RWXVF */
#define NV1_PFIFO_INTR_0_CACHE_ERROR_NOT_PENDING          0x00000000 /* R---V */
#define NV1_PFIFO_INTR_0_CACHE_ERROR_PENDING              0x00000001 /* R---V */
#define NV1_PFIFO_INTR_0_CACHE_ERROR_RESET                0x00000001 /* -W--V */
#define NV1_PFIFO_INTR_0_RUNOUT                                    4 /* RWXVF */
#define NV1_PFIFO_INTR_0_RUNOUT_NOT_PENDING               0x00000000 /* R---V */
#define NV1_PFIFO_INTR_0_RUNOUT_PENDING                   0x00000001 /* R---V */
#define NV1_PFIFO_INTR_0_RUNOUT_RESET                     0x00000001 /* -W--V */
#define NV1_PFIFO_INTR_0_RUNOUT_OVERFLOW                           8 /* RWXVF */
#define NV1_PFIFO_INTR_0_RUNOUT_OVERFLOW_NOT_PENDING 0x00000000 /* R---V */
#define NV1_PFIFO_INTR_0_RUNOUT_OVERFLOW_PENDING          0x00000001 /* R---V */
#define NV1_PFIFO_INTR_0_RUNOUT_OVERFLOW_RESET            0x00000001 /* -W--V */
#define NV1_PFIFO_INTR_EN_0                               0x00002140 /* RW-4R */
#define NV1_PFIFO_INTR_EN_0_CACHE_ERROR                            0 /* RWIVF */
#define NV1_PFIFO_INTR_EN_0_CACHE_ERROR_DISABLED          0x00000000 /* RWI-V */
#define NV1_PFIFO_INTR_EN_0_CACHE_ERROR_ENABLED           0x00000001 /* RW--V */
#define NV1_PFIFO_INTR_EN_0_RUNOUT                                 4 /* RWIVF */
#define NV1_PFIFO_INTR_EN_0_RUNOUT_DISABLED               0x00000000 /* RWI-V */
#define NV1_PFIFO_INTR_EN_0_RUNOUT_ENABLED                0x00000001 /* RW--V */
#define NV1_PFIFO_INTR_EN_0_RUNOUT_OVERFLOW                        8 /* RWIVF */
#define NV1_PFIFO_INTR_EN_0_RUNOUT_OVERFLOW_DISABLED      0x00000000 /* RWI-V */
#define NV1_PFIFO_INTR_EN_0_RUNOUT_OVERFLOW_ENABLED       0x00000001 /* RW--V */
#define NV1_PFIFO_CONFIG_0                                0x00002200 /* RW-4R */
#define NV1_PFIFO_CONFIG_0_FREE_LIE                                0 /* RWXVF */
#define NV1_PFIFO_CONFIG_0_FREE_LIE_DISABLED              0x00000000 /* RW--V */
#define NV1_PFIFO_CONFIG_0_FREE_LIE_252_BYTES             0x00000001 /* RW--V */
#define NV1_PFIFO_CONFIG_0_FREE_LIE_508_BYTES             0x00000002 /* RW--V */
#define NV1_PFIFO_CONFIG_0_FREE_LIE_1020_BYTES            0x00000003 /* RW--V */
#define NV1_PFIFO_CACHES                                  0x00002500 /* RW-4R */
#define NV1_PFIFO_CACHES_REASSIGN                                  0 /* RWIVF */
#define NV1_PFIFO_CACHES_REASSIGN_DISABLED                0x00000000 /* RWI-V */
#define NV1_PFIFO_CACHES_REASSIGN_ENABLED                 0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE0_PUSH0                            0x00003000 /* RW-4R */
#define NV1_PFIFO_CACHE0_PUSH0_ACCESS                              0 /* RWIVF */
#define NV1_PFIFO_CACHE0_PUSH0_ACCESS_DISABLED            0x00000000 /* RWI-V */
#define NV1_PFIFO_CACHE0_PUSH0_ACCESS_ENABLED             0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE1_PUSH0                            0x00003200 /* RW-4R */
#define NV1_PFIFO_CACHE1_PUSH0_ACCESS                              0 /* RWIVF */
#define NV1_PFIFO_CACHE1_PUSH0_ACCESS_DISABLED            0x00000000 /* RWI-V */
#define NV1_PFIFO_CACHE1_PUSH0_ACCESS_ENABLED             0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE0_PUSH1                            0x00003010 /* RW-4R */
#define NV1_PFIFO_CACHE0_PUSH1_CHID                                0 /* RWXUF */
//	Fix double-definition warning
#ifdef	NV1_PFIFO_CACHE1_PUSH1
#undef	NV1_PFIFO_CACHE1_PUSH1
#endif
#define NV1_PFIFO_CACHE1_PUSH1                            0x00003210 /* RW-4R */
#define NV1_PFIFO_CACHE1_PUSH1_CHID                                0 /* RWXUF */
#define NV1_PFIFO_CACHE0_PULL0                            0x00003040 /* RW-4R */
#define NV1_PFIFO_CACHE0_PULL0_ACCESS                              0 /* RWIVF */
#define NV1_PFIFO_CACHE0_PULL0_ACCESS_DISABLED            0x00000000 /* RWI-V */
#define NV1_PFIFO_CACHE0_PULL0_ACCESS_ENABLED             0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE1_PULL0                            0x00003240 /* R--4R */
#define NV1_PFIFO_CACHE1_PULL0_ACCESS                              0 /* RWIVF */
#define NV1_PFIFO_CACHE1_PULL0_ACCESS_DISABLED            0x00000000 /* RWI-V */
#define NV1_PFIFO_CACHE1_PULL0_ACCESS_ENABLED             0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE1_PULL0_HASH                                4 /* R-XVF */
#define NV1_PFIFO_CACHE1_PULL0_HASH_SUCCEEDED             0x00000000 /* R---V */
#define NV1_PFIFO_CACHE1_PULL0_HASH_FAILED                0x00000001 /* R---V */
#define NV1_PFIFO_CACHE1_PULL0_DEVICE                              8 /* R-XVF */
#define NV1_PFIFO_CACHE1_PULL0_DEVICE_HARDWARE            0x00000000 /* R---V */
#define NV1_PFIFO_CACHE1_PULL0_DEVICE_SOFTWARE            0x00000001 /* R---V */
#define NV1_PFIFO_CACHE0_PULL1                            0x00003050 /* RW-4R */
#define NV1_PFIFO_CACHE0_PULL1_OBJECT                              8 /* RWXVF */
#define NV1_PFIFO_CACHE0_PULL1_OBJECT_UNCHANGED           0x00000000 /* RW--V */
#define NV1_PFIFO_CACHE0_PULL1_OBJECT_CHANGED             0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE1_PULL1                            0x00003250 /* RW-4R */
#define NV1_PFIFO_CACHE1_PULL1_SUBCHANNEL                          0 /* RWXUF */
#define NV1_PFIFO_CACHE1_PULL1_CTX                                 4 /* RWXVF */
#define NV1_PFIFO_CACHE1_PULL1_CTX_CLEAN                  0x00000000 /* RW--V */
#define NV1_PFIFO_CACHE1_PULL1_CTX_DIRTY                  0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE1_PULL1_OBJECT                              8 /* RWXVF */
#define NV1_PFIFO_CACHE1_PULL1_OBJECT_UNCHANGED           0x00000000 /* RW--V */
#define NV1_PFIFO_CACHE1_PULL1_OBJECT_CHANGED             0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE0_STATUS                           0x00003020 /* R--4R */
#define NV1_PFIFO_CACHE0_STATUS_LOW_MARK                           4 /* R--VF */
#define NV1_PFIFO_CACHE0_STATUS_LOW_MARK_NOT_EMPTY        0x00000000 /* R---V */
#define NV1_PFIFO_CACHE0_STATUS_LOW_MARK_EMPTY            0x00000001 /* R---V */
#define NV1_PFIFO_CACHE0_STATUS_HIGH_MARK                          8 /* R--VF */
#define NV1_PFIFO_CACHE0_STATUS_HIGH_MARK_NOT_FULL        0x00000000 /* R---V */
#define NV1_PFIFO_CACHE0_STATUS_HIGH_MARK_FULL            0x00000001 /* R---V */
#define NV1_PFIFO_CACHE1_STATUS                           0x00003220 /* R--4R */
#define NV1_PFIFO_CACHE1_STATUS_RANOUT                             0 /* R-XVF */
#define NV1_PFIFO_CACHE1_STATUS_RANOUT_FALSE              0x00000000 /* R---V */
#define NV1_PFIFO_CACHE1_STATUS_RANOUT_TRUE               0x00000001 /* R---V */
#define NV1_PFIFO_CACHE1_STATUS_LOW_MARK                           4 /* R--VF */
#define NV1_PFIFO_CACHE1_STATUS_LOW_MARK_NOT_EMPTY        0x00000000 /* R---V */
#define NV1_PFIFO_CACHE1_STATUS_LOW_MARK_EMPTY            0x00000001 /* R---V */
#define NV1_PFIFO_CACHE1_STATUS_HIGH_MARK                          8 /* R--VF */
#define NV1_PFIFO_CACHE1_STATUS_HIGH_MARK_NOT_FULL        0x00000000 /* R---V */
#define NV1_PFIFO_CACHE1_STATUS_HIGH_MARK_FULL            0x00000001 /* R---V */
#define NV1_PFIFO_CACHE0_PUT                              0x00003030 /* RW-4R */
#define NV1_PFIFO_CACHE0_PUT_ADDRESS                               2 /* RWXUF */
#define NV1_PFIFO_CACHE1_PUT                              0x00003230 /* RW-4R */
#define NV1_PFIFO_CACHE1_PUT_ADDRESS                               2 /* RWXUF */
#define NV1_PFIFO_CACHE0_GET                              0x00003070 /* RW-4R */
#define NV1_PFIFO_CACHE0_GET_ADDRESS                               2 /* RWXUF */
#define NV1_PFIFO_CACHE1_GET                              0x00003270 /* RW-4R */
#define NV1_PFIFO_CACHE1_GET_ADDRESS                               2 /* RWXUF */
#define NV1_PFIFO_CACHE0_CTX(i)                  (0x00003080+(i)*16) /* RW-4A */
#define NV1_PFIFO_CACHE0_CTX__SIZE_1                               1 /*       */
#define NV1_PFIFO_CACHE0_CTX_INSTANCE                             15 /* RWXUF */
#define NV1_PFIFO_CACHE0_CTX_DEVICE                               22 /* RWXUF */
#define NV1_PFIFO_CACHE1_CTX(i)                  (0x00003280+(i)*16) /* RW-4A */
#define NV1_PFIFO_CACHE1_CTX__SIZE_1                               8 /*       */
#define NV1_PFIFO_CACHE1_CTX_INSTANCE                             15 /* RWXUF */
#define NV1_PFIFO_CACHE1_CTX_DEVICE                               22 /* RWXUF */
#define NV1_PFIFO_CACHE1_CTX_LIE                                  24 /* RWXVF */
#define NV1_PFIFO_CACHE1_CTX_LIE_DISABLED                 0x00000000 /* RW--V */
#define NV1_PFIFO_CACHE1_CTX_LIE_ENABLED                  0x00000001 /* RW--V */
#define NV1_PFIFO_CACHE0_METHOD(i)                (0x00003100+(i)*8) /* RW-4A */
#define NV1_PFIFO_CACHE0_METHOD__SIZE_1                            1 /*       */
#define NV1_PFIFO_CACHE0_METHOD_ADDRESS                         12:2 /* RWXUF */
#define NV1_PFIFO_CACHE0_METHOD_SUBCHANNEL                     15:13 /* RWXUF */
#define NV1_PFIFO_CACHE1_METHOD(i)                (0x00003300+(i)*8) /* RW-4A */
#define NV1_PFIFO_CACHE1_METHOD__SIZE_1                           32 /*       */
#define NV1_PFIFO_CACHE1_METHOD_ADDRESS                         12:2 /* RWXUF */
#define NV1_PFIFO_CACHE1_METHOD_SUBCHANNEL                     15:13 /* RWXUF */
#define NV1_PFIFO_CACHE0_DATA(i)                  (0x00003104+(i)*8) /* RW-4A */
#define NV1_PFIFO_CACHE0_DATA__SIZE_1                              1 /*       */
#define NV1_PFIFO_CACHE0_DATA_VALUE                             31:0 /* RWXVF */
#define NV1_PFIFO_CACHE1_DATA(i)                  (0x00003304+(i)*8) /* RW-4A */
#define NV1_PFIFO_CACHE1_DATA__SIZE_1                             32 /*       */
#define NV1_PFIFO_CACHE1_DATA_VALUE                             31:0 /* RWXVF */
#define NV1_PFIFO_DEVICE(i)                       (0x00002800+(i)*4) /* R--4A */
#define NV1_PFIFO_DEVICE__SIZE_1                                 128 /*       */
#define NV1_PFIFO_DEVICE_CHID                                      0 /* R--UF */
#define NV1_PFIFO_DEVICE_SWITCH                                   24 /* R--VF */
#define NV1_PFIFO_DEVICE_SWITCH_UNAVAILABLE               0x00000000 /* R---V */
#define NV1_PFIFO_DEVICE_SWITCH_AVAILABLE                 0x00000001 /* R---V */
#define NV1_PFIFO_DEVICE_ACCESS                                   28 /* R--VF */
#define NV1_PFIFO_DEVICE_ACCESS_DISABLED                  0x00000000 /* R---V */
#define NV1_PFIFO_DEVICE_ACCESS_ENABLED                   0x00000001 /* R---V */
#define NV1_PFIFO_RUNOUT_STATUS                           0x00002400 /* R--4R */
#define NV1_PFIFO_RUNOUT_STATUS_RANOUT                             0 /* R--VF */
#define NV1_PFIFO_RUNOUT_STATUS_RANOUT_FALSE              0x00000000 /* R---V */
#define NV1_PFIFO_RUNOUT_STATUS_RANOUT_TRUE               0x00000001 /* R---V */
#define NV1_PFIFO_RUNOUT_STATUS_LOW_MARK                           4 /* R--VF */
#define NV1_PFIFO_RUNOUT_STATUS_LOW_MARK_NOT_EMPTY        0x00000000 /* R---V */
#define NV1_PFIFO_RUNOUT_STATUS_LOW_MARK_EMPTY            0x00000001 /* R---V */
#define NV1_PFIFO_RUNOUT_STATUS_HIGH_MARK                          8 /* R--VF */
#define NV1_PFIFO_RUNOUT_STATUS_HIGH_MARK_NOT_FULL        0x00000000 /* R---V */
#define NV1_PFIFO_RUNOUT_STATUS_HIGH_MARK_FULL            0x00000001 /* R---V */
#define NV1_PFIFO_RUNOUT_PUT                              0x00002410 /* RW-4R */
#define NV1_PFIFO_RUNOUT_PUT_ADDRESS                               3 /* RWXUF */
#define NV1_PFIFO_RUNOUT_PUT_ADDRESS__SIZE_0                       3 /*       */
#define NV1_PFIFO_RUNOUT_PUT_ADDRESS__SIZE_1                       3 /*       */
#define NV1_PFIFO_RUNOUT_PUT_ADDRESS__SIZE_2                       3 /*       */
#define NV1_PFIFO_RUNOUT_PUT_ADDRESS__SIZE_3                       3 /*       */
#define NV1_PFIFO_RUNOUT_GET                              0x00002420 /* RW-4R */
#define NV1_PFIFO_RUNOUT_GET_ADDRESS                                3 /* RWXUF */


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
