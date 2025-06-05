/* 
    Filename: nv3_ref.h
    Purpose: Provides NV3 hardware defines. Simplified version of vid_nv3.h from 86Box-riva128
    (TODO: Object definitions, so that NV_User can be treated as a c struct, like in the real drivers. Everything needs to be 0x2000)
*/

#pragma once 

#include <stdbool.h>
#include <stdint.h>

#define NV3_MMIO_SIZE                                   0x1000000       // Max MMIO size

#define NV3_LFB_RAMIN_MIRROR_START                      0x400000        // Mirror of ramin (VERIFY ON HARDWARE)
#define NV3_LFB_MIRROR_START                            0x800000        // The second half of LFB(?)
#define NV3_LFB_RAMIN_START                             0xC00000        // RAMIN mapping start

#define NV3_RAMIN_SIZE                                  0x400000        // Size of RAMIN

// THere are 64 DMA channels grouped into 8 "channels" with 8 "subchannels" each. You can only use one channel at a time. An arbitrary number of 8 objects can be submitted.
// Channel 0 is always taken up by NV drivers.

#define NV3_DMA_CHANNELS                                8
#define NV3_DMA_SUBCHANNELS_PER_CHANNEL                 8

#define NV3_DMA_CHANNELS_TOTAL                          0x7F            // This is also used somewhere despite there only being 8*8 = 64 channels

#define NV3_LAST_VALID_GRAPHICS_OBJECT_ID               0x1F

// The class ids are represented with 5 bits in PGRAPH, but 7 bits in PFIFO!
// What...
#define NV3_PFIFO_FIRST_VALID_GRAPHICS_OBJECT_ID        0x40
#define NV3_PFIFO_LAST_VALID_GRAPHICS_OBJECT_ID         0x5F



#define NV3_VRAM_SIZE_1MB                               0x100000 // 1MB (commented out in drivers)
#define NV3_VRAM_SIZE_2MB                               0x200000 // 2MB
#define NV3_VRAM_SIZE_4MB                               0x400000 // 4MB
#define NV3_VRAM_SIZE_8MB                               0x800000 // NV3T only

// There is also 1mb supported by the card but it was never used

// PCI config
#define NV3_PCI_CFG_VENDOR_ID                           0x0
#define NV3_PCI_CFG_DEVICE_ID                           0x2
#define NV3_PCI_CFG_CAPABILITIES                        0x4

#define NV3_PCI_COMMAND_L_IO                            1
#define NV3_PCI_COMMAND_L_IO_ENABLED                    0x1
#define NV3_PCI_COMMAND_L_MEMORY                        2
#define NV3_PCI_COMMAND_L_MEMORY_ENABLED                0x1

#define NV3_PCI_COMMAND_H_FAST_BACK2BACK                0x01

#define NV3_PCI_STATUS_L_66MHZ_CAPABLE                  0x20
#define NV3_PCI_STATUS_H_DEVSEL_TIMING                  5
#define NV3_PCI_STATUS_H_FAST_DEVSEL_TIMING             0x00

#define NV3_PCI_CFG_REVISION                            0x8

#define NV3_PCI_CFG_REVISION_A00                        0x00 // nv3a January 1997 - engineering sample, had NV1 PAUDIO and other minor incompatibilities
#define NV3_PCI_CFG_REVISION_B00                        0x10 // nv3b September 1997
#define NV3_PCI_CFG_REVISION_C00                        0x20 // todo: verify this - nv3c (nv3t?) / RIVA 128 ZX

#define NV3_PCI_CFG_PROGRAMMING_INTERFACE               0x9
#define NV3_PCI_CFG_SUBCLASS_CODE                       0x0A
#define NV3_PCI_CFG_CLASS_CODE                          0x0B
#define NV3_PCI_CFG_CLASS_CODE_VGA                      0x03

#define NV3_PCI_CFG_CACHE_LINE_SIZE                     0x0C
#define NV3_PCI_CFG_CACHE_LINE_SIZE_DEFAULT_FROM_VBIOS  0x40

#define NV3_PCI_CFG_LATENCY_TIMER                       0x0D
#define NV3_PCI_CFG_HEADER_TYPE                         0x0E
#define NV3_PCI_CFG_BIST                                0x0F

// PCI Bars
#define NV3_PCI_CFG_BAR_PREFETCHABLE                    3
#define NV3_PCI_CFG_BAR_PREFETCHABLE_ENABLED            0x1

#define NV3_PCI_CFG_BAR0_L                              0x10
#define NV3_PCI_CFG_BAR0_BYTE1                          0x11
#define NV3_PCI_CFG_BAR0_BYTE2                          0x12
#define NV3_PCI_CFG_BAR0_BASE_ADDRESS                   0x13
#define NV3_PCI_CFG_BAR1_L                              0x14
#define NV3_PCI_CFG_BAR1_BYTE1                          0x15
#define NV3_PCI_CFG_BAR1_BYTE2                          0x16
#define NV3_PCI_CFG_BAR1_BASE_ADDRESS                   0x17
#define NV3_PCI_CFG_BAR_INVALID_START                   0x18
#define NV3_PCI_CFG_BAR_INVALID_END                     0x27
#define NV3_PCI_CFG_SUBSYSTEM_ID                        0x2C

#define NV3_PCI_CFG_ENABLE_VBIOS                        0x30
#define NV3_PCI_CFG_VBIOS_BASE                          0x32 ... 0x33
#define NV3_PCI_CFG_VBIOS_BASE_L                        0x32
#define NV3_PCI_CFG_VBIOS_BASE_H                        0x33

#define NV3_PCI_CFG_INT_LINE                            0x3C
#define NV3_PCI_CFG_INT_PIN                             0x3D

#define NV3_PCI_CFG_SUBSYSTEM_ID_MIRROR_START           0x40
#define NV3_PCI_CFG_SUBSYSTEM_ID_MIRROR_END             0x43

#define NV3_PCI_CFG_MIN_GRANT                           0x3E
#define NV3_PCI_CFG_MIN_GRANT_DEFAULT                   0x03
#define NV3_PCI_CFG_MAX_LATENCY                         0x3F
#define NV3_PCI_CFG_MAX_LATENCY_DEFAULT                 0x01

// GPU Subsystems
// These most likely correspond to functional blocks in the original design

#define NV3_PMC_START                                   0x0         // Chip Master Control Subsystem

#define NV3_PMC_BOOT                                    0x0         // Boot Configuration

#define NV3_PMC_INTERRUPT_STATUS                        0x100       // Interrupt Control
#define NV3_PMC_INTERRUPT_PAUDIO                        0           // Unused, NV3A only
#define NV3_PMC_INTERRUPT_PAUDIO_PENDING                0x1         // Unused, NV3A only
#define NV3_PMC_INTERRUPT_PMEDIA                        4
#define NV3_PMC_INTERRUPT_PMEDIA_PENDING                0x1
#define NV3_PMC_INTERRUPT_PFIFO                         8
#define NV3_PMC_INTERRUPT_PFIFO_PENDING                 0x1
#define NV3_PMC_INTERRUPT_PGRAPH0                       12
#define NV3_PMC_INTERRUPT_PGRAPH0_PENDING               0x1
#define NV3_PMC_INTERRUPT_PGRAPH1                       13
#define NV3_PMC_INTERRUPT_PGRAPH1_PENDING               0x1
#define NV3_PMC_INTERRUPT_PVIDEO                        16
#define NV3_PMC_INTERRUPT_PVIDEO_PENDING                0x1
#define NV3_PMC_INTERRUPT_PTIMER                        20
#define NV3_PMC_INTERRUPT_PTIMER_PENDING                0x1
#define NV3_PMC_INTERRUPT_PFB                           24
#define NV3_PMC_INTERRUPT_PFB_PENDING                   0x1
#define NV3_PMC_INTERRUPT_PBUS                          28  
#define NV3_PMC_INTERRUPT_PBUS_PENDING                  0x1
#define NV3_PMC_INTERRUPT_SOFTWARE                      31
#define NV3_PMC_INTERRUPT_SOFTWARE_PENDING              0x1
#define NV3_PMC_INTERRUPT_ENABLE                        0x140       // Controls global interrupt enable state
#define NV3_PMC_INTERRUPT_ENABLE_HARDWARE               0x1         // Determines if hardware interrupts are enabled
#define NV3_PMC_INTERRUPT_ENABLE_SOFTWARE               0x2         // Determinse if software interrupts were enabled
#define NV3_PMC_ENABLE                                  0x200       // Determines which gpu subsystems were enabled
#define NV3_PMC_ENABLE_PAUDIO                           0           // UNUSED - PAudio removed in NV3 Stepping B0 
#define NV3_PMC_ENABLE_PAUDIO_ENABLED                   0x1         // UNUSED - PAudio removed in NV3 Stepping B0
#define NV3_PMC_ENABLE_PMEDIA                           4
#define NV3_PMC_ENABLE_PMEDIA_ENABLED                   0x1
#define NV3_PMC_ENABLE_PFIFO                            8
#define NV3_PMC_ENABLE_PFIFO_ENABLED                    0x1
#define NV3_PMC_ENABLE_PGRAPH                           12          // Determines if PGRAPH is enabled.
#define NV3_PMC_ENABLE_PGRAPH_ENABLED                   0x1
#define NV3_PMC_ENABLE_PPMI                             16
#define NV3_PMC_ENABLE_PPMI_ENABLED                     0x1
#define NV3_PMC_ENABLE_PFB                              20
#define NV3_PMC_ENABLE_PFB_ENABLED                      0x1
#define NV3_PMC_ENABLE_PCRTC                            24
#define NV3_PMC_ENABLE_PCRTC_ENABLED                    0x1
#define NV3_PMC_ENABLE_PVIDEO                           28
#define NV3_PMC_ENABLE_PVIDEO_ENABLED                   0x1

#define NV3_PMC_END                                     0xfff       // overlaps with CIO
#define NV3_CIO_START                                   0x3b0       // Legacy SVGA Emulation Subsystem
#define NV3_CIO_END                                     0x3df
#define NV3_PBUS_START                                  0x1000      // Bus Control Subsystem
#define NV3_PBUS_DEBUG_0                                0x1084      // Bus Control Debug
#define NV3_PBUS_INTR                                   0x1100      // Bus Control - Interrupt Status

#define NV3_PBUS_INTR_EN                                0x1140      // Bus Control - Interrupt Enable
#define NV3_PBUS_PCI_START                              0x1800      // PCI mirror start
#define NV3_PBUS_PCI_END                                0x18FF      // PCI mirror end
#define NV3_PBUS_END                                    0x1FFF
#define NV3_PFIFO_START                                 0x2000      // FIFO for DMA Object Submission (uses hashtable to store the objects)

#define NV3_PFIFO_MINIMUM_GUARANTEED_DEPTH                             0x7C

#define NV3_PFIFO_DELAY_0                               0x2040      // PFIFO Config Register
#define NV3_PFIFO_DEBUG_0                               0x2080      // PFIFO Debug Register
#define NV3_PFIFO_CACHE0_ERROR_PENDING                  0
#define NV3_PFIFO_CACHE1_ERROR_PENDING                  4

#define NV3_PFIFO_INTR                                  0x2100      // FIFO - Interrupt Status
#define NV3_PFIFO_INTR_EN                               0x2140      // FIFO - Interrupt Enable

// PFIFO interrupts
#define NV3_PFIFO_INTR_CACHE_ERROR                      0
#define NV3_PFIFO_INTR_RUNOUT                           4
#define NV3_PFIFO_INTR_RUNOUT_OVERFLOW                  8
#define NV3_PFIFO_INTR_DMA_PUSHER                       12
#define NV3_PFIFO_INTR_DMA_PTE                          16

#define NV3_PFIFO_CONFIG_0                              0x2200
#define NV3_PFIFO_CONFIG_0_DMA_FETCH                    8

#define NV3_PFIFO_CONFIG_RAMHT                          0x2210      // Hashtable for graphics objects config
#define NV3_PFIFO_CONFIG_RAMHT_BASE_ADDRESS             12          // 15:12
#define NV3_PFIFO_CONFIG_RAMHT_BASE_ADDRESS_DEFAULT     0x0
#define NV3_PFIFO_CONFIG_RAMHT_SIZE                     16          // 17:16
#define NV3_PFIFO_CONFIG_RAMHT_SIZE_4K                  0x0
#define NV3_PFIFO_CONFIG_RAMHT_SIZE_8K                  0x1
#define NV3_PFIFO_CONFIG_RAMHT_SIZE_16K                 0x2
#define NV3_PFIFO_CONFIG_RAMHT_SIZE_32K                 0x3

#define NV3_PFIFO_CONFIG_RAMFC                          0x2214
#define NV3_PFIFO_CONFIG_RAMFC_BASE_ADDRESS             9
#define NV3_PFIFO_CONFIG_RAMFC_BASE_ADDRESS_DEFAULT     0x1C00      // Hardcoded in silicon?
#define NV3_PFIFO_CONFIG_RAMRO                          0x2218
#define NV3_PFIFO_CONFIG_RAMRO_BASE_ADDRESS             9
#define NV3_PFIFO_CONFIG_RAMRO_BASE_ADDRESS_DEFAULT     0x1E00      // Hardcoded in silicon?
#define NV3_PFIFO_CONFIG_RAMRO_SIZE                     16
#define NV3_PFIFO_CONFIG_RAMRO_SIZE_512B                0x0
#define NV3_PFIFO_CONFIG_RAMRO_SIZE_8K                  0x1 

#define NV3_PFIFO_RUNOUT_STATUS                         0x2400
#define NV3_PFIFO_RUNOUT_STATUS_RANOUT                  0           // 1 if we fucked up
#define NV3_PFIFO_RUNOUT_STATUS_EMPTY                   4           // 1 if ramro is empty
#define NV3_PFIFO_RUNOUT_STATUS_FULL                    8
#define NV3_PFIFO_RUNOUT_PUT                            0x2410
#define NV3_PFIFO_RUNOUT_PUT_ADDRESS                    3           // 9:3 if small ramfc(?) otherwise 12:3
#define NV3_PFIFO_RUNOUT_GET                            0x2420
#define NV3_PFIFO_RUNOUT_GET_ADDRESS                    3           // 13:3

#define NV3_PFIFO_RUNOUT_RAMIN_ERR                      28          // bit to or with

#define NV3_PFIFO_CACHE0_SIZE                           1           // This is for software-injected notified only!
#define NV3_PFIFO_CACHE1_SIZE_REV_AB                    32
#define NV3_PFIFO_CACHE1_SIZE_REV_C                     64
#define NV3_PFIFO_CACHE1_SIZE_MAX                       NV3_PFIFO_CACHE1_SIZE_REV_C
#define NV3_PFIFO_CACHE_REASSIGNMENT                    0x2500        

#define NV3_PFIFO_CACHE0_PUSH_ENABLED                          0x3000
#define NV3_PFIFO_CACHE0_PUSH_CHANNEL_ID                0x3004
#define NV3_PFIFO_CACHE0_PUT                            0x3010
#define NV3_PFIFO_CACHE0_STATUS                         0x3014
#define NV3_PFIFO_CACHE0_STATUS_EMPTY                   4           // 1 if ramro is empty
#define NV3_PFIFO_CACHE0_STATUS_FULL                    8
#define NV3_PFIFO_CACHE0_PUT_ADDRESS                    2           // 1 bit
#define NV3_PFIFO_CACHE0_PULL0                          0x3040
#define NV3_PFIFO_CACHE0_PULL0_ENABLED                  0
#define NV3_PFIFO_CACHE0_PULL0_HASH_FAILURE             4
#define NV3_PFIFO_CACHE0_PULL0_SOFTWARE_METHOD          8
#define NV3_PFIFO_CACHE0_PULLER_CTX_STATE               0x3050
#define NV3_PFIFO_CACHE0_PULLER_CTX_STATE_DIRTY         4           // 1=dirty 0=clean
#define NV3_PFIFO_CACHE0_GET                            0x3070
#define NV3_PFIFO_CACHE0_GET_ADDRESS                    2           // 1 bit
// Current channel context - cache1
#define NV3_PFIFO_CACHE0_CTX                            0x3080      

#define NV3_PFIFO_CACHE0_METHOD_START                   0x3100
#define NV3_PFIFO_CACHE0_METHOD_END                     0x3200
#define NV3_PFIFO_CACHE0_METHOD_ADDRESS                 2           // 12:2
#define NV3_PFIFO_CACHE0_METHOD_SUBCHANNEL              13          // 15:13
#define NV3_PFIFO_CACHE1_PUSH_ENABLED                          0x3200
#define NV3_PFIFO_CACHE1_PUSH_CHANNEL_ID                0x3204
#define NV3_PFIFO_CACHE1_PUT                            0x3210
#define NV3_PFIFO_CACHE1_PUT_ADDRESS                    2           // 6:2
#define NV3_PFIFO_CACHE1_STATUS                         0x3214
#define NV3_PFIFO_CACHE1_STATUS_RANOUT                  0           // 1 if we fucked up
#define NV3_PFIFO_CACHE1_STATUS_EMPTY                   4           // 1 if ramro is empty
#define NV3_PFIFO_CACHE1_STATUS_FULL                    8
#define NV3_PFIFO_CACHE1_DMA_STATUS                     0x3218
#define NV3_PFIFO_CACHE1_DMA_CONFIG_0                   0x3220
#define NV3_PFIFO_CACHE1_DMA_CONFIG_1                   0x3224
#define NV3_PFIFO_CACHE1_DMA_CONFIG_2                   0x3228
#define NV3_PFIFO_CACHE1_DMA_CONFIG_3                   0x322C
#define NV3_PFIFO_CACHE1_DMA_CONFIG_3_TARGET_NODE       0           // The type of bus we are sending over
#define NV3_PFIFO_CACHE1_DMA_CONFIG_3_TARGET_NODE_PCI   0x02        // The type of bus we are sending over
#define NV3_PFIFO_CACHE1_DMA_CONFIG_3_TARGET_NODE_AGP   0x03        // The type of bus we are sending over

// Why does a gpu need its own translation lookaside buffer and pagetable format. Are they crazy
#define NV3_PFIFO_CACHE1_DMA_TLB_TAG                    0x3230
#define NV3_PFIFO_CACHE1_DMA_TLB_PTE                    0x3234      // Base of pagetableor DMA
#define NV3_PFIFO_CACHE1_DMA_TLB_PT_BASE                0x3238      // Base of pagetable for DMA
#define NV3_PFIFO_CACHE1_PULL0                          0x3240
//todo: merge stuff
#define NV3_PFIFO_CACHE1_PULL0_ENABLED                  0
#define NV3_PFIFO_CACHE1_PULL0_HASH_FAILURE             4   
#define NV3_PFIFO_CACHE1_PULL0_SOFTWARE_METHOD          8           // 0=software, 1=hardware
#define NV3_PFIFO_CACHE1_PULLER_CTX_STATE               0x3250
#define NV3_PFIFO_CACHE1_PULLER_CTX_STATE_DIRTY         4
#define NV3_PFIFO_CACHE1_GET                            0x3270
#define NV3_PFIFO_CACHE1_GET_ADDRESS                    2           // 6:2

// Current channel context - cache1
#define NV3_PFIFO_CACHE1_CTX_START                      0x3280      
#define NV3_PFIFO_CACHE1_CTX_END                        0x32F0

#define NV3_PFIFO_CACHE1_METHOD_START                   0x3300
#define NV3_PFIFO_CACHE1_METHOD_END                     0x3400
#define NV3_PFIFO_CACHE1_METHOD_ADDRESS                 2           // 12:2
#define NV3_PFIFO_CACHE1_METHOD_SUBCHANNEL              13          // 15:13


#define NV3_PFIFO_END                                   0x3FFF
#define NV3_PRM_START                                   0x4000      // Real-Mode Device Support Subsystem
#define NV3_PRM_INTR                                    0x4100
#define NV3_PRM_INTR_EN                                 0x4140
#define NV3_PRM_END                                     0x4FFF
#define NV3_PRAM_START                                  0x6000      // Local ram/cache?
#define NV3_PRAM_END                                    0x6FFF
#define NV3_PRMIO_START                                 0x7000      // Real-Mode I/O Subsystem
#define NV3_PRMIO_END                                   0x7FFF
#define NV3_PTIMER_START                                0x9000      // Programmable Interval Timer
#define NV3_PTIMER_INTR                                 0x9100
#define NV3_PTIMER_INTR_ALARM                           0           // Alarm interrupt
#define NV3_PTIMER_INTR_EN                              0x9140
#define NV3_PTIMER_NUMERATOR                            0x9200
#define NV3_PTIMER_DENOMINATOR                          0x9210
#define NV3_PTIMER_TIME_0_NSEC                          0x9400      // nanoseconds [31:5]
#define NV3_PTIMER_TIME_1_NSEC                          0x9410      // nanoseconds [28:0]
#define NV3_PTIMER_ALARM_NSEC                           0x9420      // nanoseconds [31:5]
#define NV3_PTIMER_END                                  0x9FFF
#define NV3_VGA_VRAM_START                              0xA0000     // VGA Emulation VRAM
#define NV3_VGA_VRAM_END                                0xBFFFF
#define NV3_VGA_START                                   0xC0000     // VGA Emulation Registers
#define NV3_VGA_END                                     0xC7FFF
#define NV3_PRMVIO_START                                NV3_VGA_START // VGA stuff written from main GPU
#define NV3_PRMVIO_END                                  0xC0400
#define NV3_PFB_START                                   0x100000    // GPU Interface to VRAM
#define NV3_PFB_BOOT                                    0x100000    // Boot registration 
#define NV3_PFB_BOOT_RAM_AMOUNT                         0           // The amount of ram
#define NV3_PFB_BOOT_RAM_AMOUNT_8MB                     0x0         // 1mb in NV3A
#define NV3_PFB_BOOT_RAM_AMOUNT_2MB                     0x1
#define NV3_PFB_BOOT_RAM_AMOUNT_4MB                     0x2
#define NV3_PFB_BOOT_RAM_AMOUNT_UNDEFINED               0x3         // i assume this is used for debug
#define NV3_PFB_BOOT_RAM_WIDTH                          2           // the bus width of the gpu's vram
#define NV3_PFB_BOOT_RAM_WIDTH_64                       0x0         // 64bit
#define NV3_PFB_BOOT_RAM_WIDTH_128                      0x1         // 128bit
#define NV3_PFB_BOOT_RAM_BANKS                          3           // the number of banks
#define NV3_PFB_BOOT_RAM_BANKS_2                        0x0         // 2 banks (seems to be used for 2mb)
#define NV3_PFB_BOOT_RAM_BANKS_4                        0x1         // 4 banks (seems to be used for 4mb)
#define NV3_PFB_BOOT_RAM_DATA_TWIDDLE                   4
#define NV3_PFB_BOOT_RAM_DATA_TWIDDLE_OFF               0x0
#define NV3_PFB_BOOT_RAM_DATA_TWIDDLE_ON                0x1
#define NV3_PFB_BOOT_RAM_EXTENSION                      5
#define NV3_PFB_BOOT_RAM_EXTENSION_NONE                 0x0
#define NV3_PFB_BOOT_RAM_EXTENSION_8MB                  0x1

#define NV3_PFB_DELAY                                   0x100044
#define NV3_PFB_DEBUG_0                                 0x100080    // Debug register for pfb
#define NV3_PFB_GREEN_0                                 0x1000C0

#define NV3_PFB_CONFIG_0                                0x100200    // Framebuffer interface config register 0

// What is this lol
// ??? Part of the memory timings
#define NV3_PFB_RTL                                     0x100300
#define NV3_PFB_CONFIG_0_RESOLUTION                     0
// 1=40 horiz. resolution
// i assume it can be divided by some kind of divisor to produce the vertical resolution (e.g. 3/2 or multiply by 2/3) to get the final 
// horiz is 32*value
// theoretically it should support resolutions from 40-2560 horiz

// WHAT ARE THE TIMINGS: ARE THEY IN THE VBIOS?
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_320           0xA
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_400           0xD
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_480           0xF
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_512           0x10
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_640           0x14
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_800           0x19
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_960           0x1E
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_1024          0x20
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_1152          0x24
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_1280          0x28
#define NV3_PFB_CONFIG_0_HORIZ_RESOLUTION_1600          0x32

#define NV3_PFB_CONFIG_0_PIXEL_DEPTH                    8
#define NV3_PFB_CONFIG_0_DEPTH_8BPP                     0x1
#define NV3_PFB_CONFIG_0_DEPTH_16BPP                    0x2
#define NV3_PFB_CONFIG_0_DEPTH_32BPP                    0x3

#define NV3_PFB_CONFIG_1                                0x100204    // Framebuffer interface config register 1
#define NV3_PFB_END                                     0x100FFF
#define NV3_PEXTDEV_START                               0x101000    // External Devices
#define NV3_PSTRAPS                                     0x101000    // Straps Bits
#define NV3_PSTRAPS_BUS_SPEED                           0           // Configured bus speed
#define NV3_PSTRAPS_BUS_SPEED_33MHZ                     0x0
#define NV3_PSTRAPS_BUS_SPEED_66MHZ                     0x1
#define NV3_PSTRAPS_BIOS                                1           // Is a VBIOS present?
#define NV3_PSTRAPS_BIOS_NOT_PRESENT                    0x0
#define NV3_PSTRAPS_BIOS_PRESENT                        1
#define NV3_PSTRAPS_RAM_TYPE                            2           // Type of RAM module
#define NV3_PSTRAPS_RAM_TYPE_16MBIT                     0x0
#define NV3_PSTRAPS_RAM_TYPE_8MBIT                      0x1
#define NV3_PSTRAPS_NEC_MODE                            3           // PC98?
#define NV3_PSTRAPS_NEC_MODE_DISABLED                   0x0
#define NV3_PSTRAPS_NEC_MODE_ENABLED                    0x1
#define NV3_PSTRAPS_BUS_WIDTH                           4           // Bus width
#define NV3_PSTRAPS_BUS_WIDTH_64BIT                     0x0
#define NV3_PSTRAPS_BUS_WIDTH_128BIT                    0x0
#define NV3_PSTRAPS_BUS_TYPE                            5           // Determines if this is a PCI or AGP card
#define NV3_PSTRAPS_BUS_TYPE_PCI                        0x0
#define NV3_PSTRAPS_BUS_TYPE_AGP                        0x1
#define NV3_PSTRAPS_CRYSTAL                             6           // type of clock crystal
#define NV3_PSTRAPS_CRYSTAL_13500K                      0x0         // 13.5 Mhz
#define NV3_PSTRAPS_CRYSTAL_14318180                    0x1         // 14.318180 Mhz clock crystal
#define NV3_PSTRAPS_TVMODE                              7           // Type of TV signal to put out
#define NV3_PSTRAPS_TVMODE_SECAM                        0x0  
#define NV3_PSTRAPS_TVMODE_NTSC                         0x1
#define NV3_PSTRAPS_TVMODE_PAL                          0x2
#define NV3_PSTRAPS_TVMODE_NONE                         0x3
#define NV3_PSTRAPS_AGP2X                               9
#define NV3_PSTRAPS_AGP2X_ENABLED                       0x0
#define NV3_PSTRAPS_AGP2X_DISABLED                      0x1
#define NV3_PSTRAPS_UNUSED                              10
#define NV3_PSTRAPS_OVERWRITE                           11
#define NV3_PSTRAPS_OVERWRITE_DISABLED                  0x0
#define NV3_PSTRAPS_OVERWRITE_ENABLED                   0x1
#define NV3_PEXTDEV_END                                 0x101FFF
#define NV3_PROM_START                                  0x110000    // VBIOS?
#define NV3_PROM_END                                    0x11FFFF
#define NV3_PALT_START                                  0x120000    // ??? but it exists
#define NV3_PALT_END                                    0x12FFFF
#define NV3_PME_START                                   0x200000    // Mediaport 
#define NV3_PME_INTR                                    0x200100    // Mediaport: Interrupt Pending?
#define NV3_PME_INTR_EN                                 0x200140    // Mediaport: Interrupt Enable
#define NV3_PME_END                                     0x200FFF
#define NV3_PGRAPH_START                                0x400000    // Scene graph for 2d/3d rendering...the most important part
// PGRAPH Core

#define NV3_PGRAPH_MAX_BUFFERS                          4

// For these debug registers, 0=Disabled, 1=Enabled

// Debug 0: General
#define NV3_PGRAPH_DEBUG_0                              0x400080
#define NV3_PGRAPH_DEBUG_0_STATE_IN_RESET               0
#define NV3_PGRAPH_DEBUG_0_AP_PIPE_IN_RESET             1
#define NV3_PGRAPH_DEBUG_0_CACHE_IN_RESET               2
#define NV3_PGRAPH_DEBUG_0_3D_PIPE_IN_RESET             3
#define NV3_PGRAPH_DEBUG_0_BULK_READS                   4
#define NV3_PGRAPH_DEBUG_0_TILING                       16
#define NV3_PGRAPH_DEBUG_0_WRITE_ONLY_ROPS_2D           20
#define NV3_PGRAPH_DEBUG_0_WRITE_ONLY_ROPS_3D           21
#define NV3_PGRAPH_DEBUG_0_DRAWDIR_AUT                  24
#define NV3_PGRAPH_DEBUG_0_DRAWDIR_Y                    25
#define NV3_PGRAPH_DEBUG_0_ALPHA_ABORT                  28

// Debug 1: Registers
#define NV3_PGRAPH_DEBUG_1                              0x400084
#define NV3_PGRAPH_DEBUG_1_VOLATILE_RESET_LAST          0
#define NV3_PGRAPH_DEBUG_1_DMA_ACTIVITY_CANCEL          4
#define NV3_PGRAPH_DEBUG_1_TURBO3D_2X                   8
#define NV3_PGRAPH_DEBUG_1_TURBO3D_4X                   9
#define NV3_PGRAPH_DEBUG_1_TRIANGLE_OPS                 12
#define NV3_PGRAPH_DEBUG_1_TRIANGLE_CLIP_OPS            13
#define NV3_PGRAPH_DEBUG_1_INSTANCE                     16
#define NV3_PGRAPH_DEBUG_1_CONTEXT                      20
#define NV3_PGRAPH_DEBUG_1_CACHE_FLUSH                  24
#define NV3_PGRAPH_DEBUG_1_ZCLAMP                       28

// Debug 2: 3D Pipeline
#define NV3_PGRAPH_DEBUG_2                              0x400088
#define NV3_PGRAPH_DEBUG_2_AVOID_READMODIFYWRITE_BLEND  0
#define NV3_PGRAPH_DEBUG_2_DPWR_FIFO                    8
#define NV3_PGRAPH_DEBUG_2_BILINEAR_FILTERING_3D        12
#define NV3_PGRAPH_DEBUG_2_ANISOTROPIC_FILTERING_3D     13
#define NV3_PGRAPH_DEBUG_2_FOG                          14
#define NV3_PGRAPH_DEBUG_2_LIGHTING                     15 // Not sure what this does, maybe hardware t&l was planned
#define NV3_PGRAPH_DEBUG_2_BILINEAR_FILTERING_2D        16
#define NV3_PGRAPH_DEBUG_2_ANISOTROPIC_FILTERING_2D     17
#define NV3_PGRAPH_DEBUG_2_D3D_COALESCE                 20 // coalesce reads/writes for d3d class 0x17
#define NV3_PGRAPH_DEBUG_2_D3D_COALESCE_POINT_ZETA      22 // class 0x18 coalesce
#define NV3_PGRAPH_DEBUG_2_PREFETCH                     24
#define NV3_PGRAPH_DEBUG_2_VOLATILE_RESET               28

// Debug 3: Zeta & Alpha Buffer 
#define NV3_PGRAPH_DEBUG_3                              0x40008C
#define NV3_PGRAPH_DEBUG_3_CULLING                      0
#define NV3_PGRAPH_DEBUG_3_FAST_DATA_D3D                4
#define NV3_PGRAPH_DEBUG_3_FAST_DATA_STRETCH            5
#define NV3_PGRAPH_DEBUG_3_ZFLUSH                       7
#define NV3_PGRAPH_DEBUG_3_AUTOZFLUSH_POINT_ZETA        8
#define NV3_PGRAPH_DEBUG_3_AUTOZFLUSH_D3D               9
#define NV3_PGRAPH_DEBUG_3_SLOT_CONFLICT_POINT_ZETA     10          // Slot conflict handling for POINT_ZETA (class 0x18)
#define NV3_PGRAPH_DEBUG_3_SLOT_CONFLICT_D3D            11          // Slot conflict handling for D3D5_TRI (class 0x17)
#define NV3_PGRAPH_DEBUG_3_EARLY_ZABORT                 12
#define NV3_PGRAPH_DEBUG_3_TRIANGLE_END_FLUSH           13
#define NV3_PGRAPH_DEBUG_3_ZFIFO_NOOP                   14          // ???
#define NV3_PGRAPH_DEBUG_3_DITHER                       15
#define NV3_PGRAPH_DEBUG_3_FORCE_COLOR_BUFFER_READ      16
#define NV3_PGRAPH_DEBUG_3_FORCE_ZETA_BUFFER_READ       17
#define NV3_PGRAPH_DEBUG_3_DATA_CHECK                   20
#define NV3_PGRAPH_DEBUG_3_DATA_CHECK_FAIL              21
#define NV3_PGRAPH_DEBUG_3_FORMAT_CHECK                 22
#define NV3_PGRAPH_DEBUG_3_ALPHA_CHECK                  24

// Interrupt stuff
#define NV3_PGRAPH_INTR_0                               0x400100
#define NV3_PGRAPH_INTR_0_VBLANK                        8           // Fired every frame
#define NV3_PGRAPH_INTR_0_VBLANK_ENABLED                0x1         // Is the vblank interrupt enabled?
#define NV3_PGRAPH_INTR_0_SOFTWARE_NOTIFY               28          // Fired on software notification

#define NV3_PGRAPH_INTR_1                               0x400104
#define NV3_PGRAPH_INTR_1_SOFTWARE_METHOD_PENDING       0           // Software or invalid method
#define NV3_PGRAPH_INTR_1_INVALID_DATA                  4           // Invalid data. Not sure when this would be triggered.
#define NV3_PGRAPH_INTR_1_DOUBLE_NOTIFY                 12          // Tried to notify while a notify was pending.
#define NV3_PGRAPH_INTR_1_CTXSW_NOTIFY                  16          // Notify fired for software context

#define NV3_PGRAPH_INTR_EN_0                            0x400140    // Interrupt Control for PGRAPH #1
//todo: add what this does
#define NV3_PGRAPH_INTR_EN_1                            0x400144    // Interrupt Control for PGRAPH #2 (it can receive two at onc)
#define NV3_PGRAPH_CONTEXT_SWITCH                       0x400180    // Holds the current PGRAPH context, switched by context switching

/* Contextual information for pgraph */
#define NV3_PGRAPH_CONTEXT_SWITCH_COLOR_FORMAT          2           // Holds the current color format used for drawing operations.
#define NV3_PGRAPH_CONTEXT_SWITCH_ALPHA                 3           // Holds a boolean if alpha transparency is currently enabled in drawing operations.
#define NV3_PGRAPH_CONTEXT_SWITCH_MONO_FORMAT           8           // Holds the current color format used for monochome drawing operations.  
#define NV3_PGRAPH_CONTEXT_SWITCH_DAC_BYPASS            9           // Holds if PRAMDAC should be bypassed, and an external DAC drawn.
#define NV3_PGRAPH_CONTEXT_SWITCH_Z_WRITE               12          // Holds if we should write back to the zbuffer.
#define NV3_PGRAPH_CONTEXT_SWITCH_CHROMA_KEY            13          // Holds the current chroma mask used for drawing operations.
#define NV3_PGRAPH_CONTEXT_SWITCH_PLANE_MASK            14          // Holds the current plane mask used for drawing operations.
#define NV3_PGRAPH_CONTEXT_SWITCH_USER_CLIP             15          // Holds the user-specified clipping information used for drawing operations.
#define NV3_PGRAPH_CONTEXT_SWITCH_SRC_BUFFER            16          // Holds the buffer ID used for drawing operation (i.e. which bpixel/bpitch/boffset index to use)
#define NV3_PGRAPH_CONTEXT_SWITCH_DST_BUFFER0_ENABLED   20          // Holds a boolean indicating if buffer 0 can be used as the destination for a drawing operation.
#define NV3_PGRAPH_CONTEXT_SWITCH_DST_BUFFER1_ENABLED   21          // Holds a boolean indicating if buffer 1 can be used as the destination for a drawing operation.
#define NV3_PGRAPH_CONTEXT_SWITCH_DST_BUFFER2_ENABLED   22          // Holds a boolean indicating if buffer 2 can be used as the destination for a drawing operation.
#define NV3_PGRAPH_CONTEXT_SWITCH_DST_BUFFER3_ENABLED   23          // Holds a boolean indicating if buffer 3 can be used as the destination for a drawing operation.
#define NV3_PGRAPH_CONTEXT_SWITCH_PATCH_CONFIG          24          // Something to do with an operation to do during a patchcord?
#define NV3_PGRAPH_CONTEXT_SWITCH_VOLATILE              31          // HUH

#define NV3_PGRAPH_CONTEXT_CONTROL                      0x400190    // DMA context control
#define NV3_PGRAPH_CONTEXT_USER                         0x400194    // Current DMA context state, may rename
#define NV3_PGRAPH_CONTEXT_CACHE(i)                     0x4001A0+(i*4)  // Context Cache
#define NV3_PGRAPH_CONTEXT_CACHE_SIZE                   8
// TODO: CLIP0/CLIP1 (8 clips min/max in 32bits)
#define NV3_PGRAPH_ABS_UCLIP_XMIN                       0x40053C    // Clip X minimum
#define NV3_PGRAPH_ABS_UCLIP_XMAX                       0x400540    // Clip X maximum
#define NV3_PGRAPH_ABS_UCLIP_YMIN                       0x400544    // Clip Y minimum
#define NV3_PGRAPH_ABS_UCLIP_YMAX                       0x400548    // Clip Y maximum
#define NV3_PGRAPH_SRC_CANVAS_MIN                       0x400550    // Minimum Source Canvas for Blit, Y=30:16, X=10:0
#define NV3_PGRAPH_SRC_CANVAS_MAX                       0x400554    // Maximum Source Canvas for Blit, Y=30:16, X=10:0
#define NV3_PGRAPH_DST_CANVAS_MIN                       0x400558    // Minimum Destination Canvas for Blit, Y=30:16, X=10:0
#define NV3_PGRAPH_DST_CANVAS_MAX                       0x40055C    // Maximum Destination Canvas for Blit, Y=30:16, X=10:0
#define NV3_PGRAPH_PATTERN_COLOR_0_RGB                  0x400600    
#define NV3_PGRAPH_PATTERN_COLOR_0_ALPHA                0x400604
#define NV3_PGRAPH_PATTERN_COLOR_1_RGB                  0x400608    
#define NV3_PGRAPH_PATTERN_COLOR_1_ALPHA                0x40060C    // pattern color 
#define NV3_PGRAPH_PATTERN_BITMAP_HIGH                  0x400610    // pattern bitmap [31:0]
#define NV3_PGRAPH_PATTERN_BITMAP_LOW                   0x400614    // pattern bitmap [63:32]
#define NV3_PGRAPH_PATTERN_SHAPE                        0x400618
#define NV3_PGRAPH_ROP3                                 0x400624    // ROP3      
#define NV3_PGRAPH_PLANE_MASK                           0x400628
#define NV3_PGRAPH_CHROMA_KEY                           0x40062C
#define NV3_PGRAPH_BETA                                 0x400640    // Beta factor (30:23 fractional, 22:0 before fraction)
#define NV3_PGRAPH_DMA                                  0x400680
#define NV3_PGRAPH_INSTANCE                             0x400688    // Current instance (?)

// Current notification object for pgraph
#define NV3_PGRAPH_NOTIFY                               0x400684    // Notifier for PGRAPH    
#define NV3_PGRAPH_NOTIFY_INSTANCE                      0
#define NV3_PGRAPH_NOTIFY_REQUEST_PENDING               16
#define NV3_PGRAPH_NOTIFY_REQUEST_TYPE                  20
#define NV3_PGRAPH_NOTIFY_REQUEST_TYPE_HARDWARE         0x0         // anything else is software

#define NV3_PGRAPH_CLIP0_MIN                            0x400690    // Clip for Blitting 0 Min
#define NV3_PGRAPH_CLIP0_MAX                            0x400694    // Clip for Blitting 0 Max
#define NV3_PGRAPH_CLIP1_MIN                            0x400698    // Clip for Blitting 1 Min
#define NV3_PGRAPH_CLIP1_MAX                            0x40069C    // Clip for Blitting 1 Max
#define NV3_PGRAPH_CLIP_MISC                            0x4006A0    // Regions/Render/Complex mode
#define NV3_PGRAPH_FIFO_ACCESS                          0x4006A4    // Is PGRAPH enabled?
#define NV3_PGRAPH_FIFO_ACCESS_DISABLED                 0x0
#define NV3_PGRAPH_FIFO_ACCESS_ENABLED                  0x1
#define NV3_PGRAPH_CLIP_MISC                            0x4006A0    // Miscellaneous clipping information
#define NV3_PGRAPH_STATUS                               0x4006B0    // Current PGRAPH status
#define NV3_PGRAPH_TRAPPED_ADDRESS                      0x4006B4
#define NV3_PGRAPH_TRAPPED_DATA                         0x4006B8
#define NV3_PGRAPH_TRAPPED_INSTANCE                     0x4006BC

#define NV3_PGRAPH_DMA_INTR_0                           0x401100    // PGRAPH DMA Interrupt Status
#define NV3_PGRAPH_DMA_INTR_INSTANCE                    0
#define NV3_PGRAPH_DMA_INTR_PRESENT                     4
#define NV3_PGRAPH_DMA_INTR_PROTECTION                  8
#define NV3_PGRAPH_DMA_INTR_LINEAR                      12
#define NV3_PGRAPH_DMA_INTR_NOTIFY                      16
#define NV3_PGRAPH_DMA_INTR_EN_0                        0x401140    // PGRAPH DMA Interrupt Enable 0

// Write-only register
#define NV3_PGRAPH_CLASSES_START                        0x410000
#define NV3_PGRAPH_CLASSES_END                          0x5FFFFF

// not sure about the class ids
// these are NOT what each class is, just uSed to manipulate it (there isn't a one to one class->reg mapping anyway)
#define NV3_PGRAPH_CLASS01_BETA_START                   0x410000    // Beta blending factor
#define NV3_PGRAPH_CLASS01_BETA_END                     0x411FFF  
#define NV3_PGRAPH_CLASS02_ROP_START                    0x420000    // Blending render operation used at final pixel/fragment generation stage
#define NV3_PGRAPH_CLASS02_ROP_END                      0x421FFF
#define NV3_PGRAPH_CLASS03_COLORKEY_START               0x430000    // Color key for image
#define NV3_PGRAPH_CLASS03_COLORKEY_END                 0x431FFF      
#define NV3_PGRAPH_CLASS04_PLANEMASK_START              0x440000    // Plane mask (for clipping?)
#define NV3_PGRAPH_CLASS04_PLANEMASK_END                0x441FFF
#define NV3_PGRAPH_CLASS05_CLIP_START                   0x450000    // clipping, probably class 23
#define NV3_PGRAPH_CLASS05_CLIP_END                     0x451FFF
#define NV3_PGRAPH_CLASS06_PATTERN_START                0x460000    // presumably a blend pattern 
#define NV3_PGRAPH_CLASS06_PATTERN_END                  0x461FFF
#define NV3_PGRAPH_CLASS07_RECTANGLE_START              0x470000    // also class 25 - that's black [NV1]
#define NV3_PGRAPH_CLASS07_RECTANGLE_END                0x471FFF    // also class 25 - that's black [NV1]
#define NV3_PGRAPH_CLASS08_POINT_START                  0x480000    // A single point
#define NV3_PGRAPH_CLASS08_POINT_END                    0x481FFF
#define NV3_PGRAPH_CLASS09_LINE_START                   0x490000    // A line
#define NV3_PGRAPH_CLASS09_LINE_END                     0x491FFF       
#define NV3_PGRAPH_CLASS0A_LIN_START                    0x4A0000    // A lin - a line without its starting or ending pixels
#define NV3_PGRAPH_CLASS0A_LIN_END                      0x4A1FFF
#define NV3_PGRAPH_CLASS0B_TRIANGLE_START               0x4B0000    // A triangle [NV1 variant] - in NV1 this was converted to a quad patch
#define NV3_PGRAPH_CLASS0B_TRIANGLE_END                 0x4B1FFF   
#define NV3_PGRAPH_CLASS0C_GDITEXT_START                0x4C0000    // Windows 95/NT GDI text acceleration
#define NV3_PGRAPH_CLASS0C_GDITEXT_END                  0x4C1FFF    

#define NV3_PGRAPH_CLASS0D_MEM2MEM_XFER_START           0x4D0000    // memory to memory transfer (not sure about which class this is)
#define NV3_PGRAPH_CLASS0D_MEM2MEM_XFER_END             0x4D1FFF    
#define NV3_PGRAPH_CLASS0E_IMAGE2MEM_XFER_SCALED_START  0x4E0000    // class 55, 56
#define NV3_PGRAPH_CLASS0F_IMAGE2MEM_XFER_SCALED_END    0x4E1FFF

#define NV3_PGRAPH_CLASS10_BLIT_START                   0x500000    // Blit 2d image from memory
#define NV3_PGRAPH_CLASS10_BLIT_END                     0x501FFF      

#define NV3_PGRAPH_CLASS11_CPU2MEM_IMAGE_START          0x510000    // Used for class 33, 34, 54
#define NV3_PGRAPH_CLASS11_CPU2MEM_IMAGE_END            0x511FFF    
#define NV3_PGRAPH_CLASS12_CPU2MEM_BITMAP_START         0x520000    // not sure, might depend on format
#define NV3_PGRAPH_CLASS12_CPU2MEM_BITMAP_END           0x521FFF    

#define NV3_PGRAPH_CLASS14_IMAGE2MEM_XFER_START         0x540000    // send image to vram, not sure what class 
#define NV3_PGRAPH_CLASS14_IMAGE2MEM_XFER_END           0x541FFF    
#define NV3_PGRAPH_CLASS15_CPU2MEM_STRETCHED_START      0x550000    // stretched cpu->vram transfer, 54
#define NV3_PGRAPH_CLASS15_CPU2MEM_STRETCHED_END        0x551FFF  

#define NV3_PGRAPH_CLASS17_D3D5TRI_ZETA_START           0x570000    // [NV3] Copy a direct3d 5.0 accelerated triangle to the zeta buffer
#define NV3_PGRAPH_CLASS17_D3D5TRI_ZETA_END             0x571FFF    
#define NV3_PGRAPH_CLASS18_POINTZETA_START              0x580000    // possibly class 69
#define NV3_PGRAPH_CLASS18_POINTZETA_END                0x581FFF

#define NV3_PGRAPH_CLASS1C_MEM2IMAGE_START              0x5C0000    // class 55, 56, 62, 63?
#define NV3_PGRAPH_CLASS1C_MEM2IMAGE_END                0x5C1FFF    


#define NV3_PGRAPH_REGISTER_END                         0x401FFF    // end of pgraph registers
#define NV3_PGRAPH_REAL_END                             0x5C1FFF

// PRMCIO is redirected to SVGA subsystem
#define NV3_PRMCIO_START                                0x601000

#define NV3_PRMCIO_CRTC_REGISTER_CUR_INDEX_MONO         0x6013B4    // Current CRTC Register Index - Monochrome
#define NV3_PRMCIO_CRTC_REGISTER_CUR_MONO               0x6013B5    // Currently Selected CRTC Register - Monochrome
#define NV3_PRMCIO_CRTC_REGISTER_CUR_INDEX_COLOR        0x6013D4    // Current CRTC Register Index - Colour
#define NV3_PRMCIO_CRTC_REGISTER_CUR_COLOR              0x6013D5    
#define NV3_PRMCIO_END                                  0x601FFF

#define NV3_PDAC_START                                  0x680000    // OPTIONAL external DAC
#define NV3_PVIDEO_START                                0x680000    // Video Generation / overlay configuration
#define NV3_PVIDEO_INTR                                 0x680100
#define NV3_PVIDEO_INTR_EN                              0x680140
#define NV3_PVIDEO_FIFO_THRESHOLD                       0x680238
#define NV3_PVIDEO_FIFO_BURST_LENGTH                    0x68023C
#define NV3_PVIDEO_OVERLAY                              0x680244
#define NV3_PVIDEO_OVERLAY_VIDEO_IS_ON                  0
#define NV3_PVIDEO_OVERLAY_KEY_ENABLED                  4
#define NV3_PVIDEO_OVERLAY_FORMAT                       8           // 0 = CCIR, 1 = YUY2

#define NV3_PVIDEO_END                                  0x6802FF

#define NV3_PRAMDAC_START                               0x680300
#define NV3_PRAMDAC_CURSOR_START_POSITION               0x680300

#define NV3_PRAMDAC_CURSOR_SIZE_X                       32
#define NV3_PRAMDAC_CURSOR_SIZE_Y                       32

#define NV3_PRAMDAC_CLOCK_MEMORY                        0x680504
#define NV3_PRAMDAC_CLOCK_MEMORY_VDIV                   7:0
#define NV3_PRAMDAC_CLOCK_MEMORY_NDIV                   15:8
#define NV3_PRAMDAC_CLOCK_MEMORY_PDIV                   18:16
#define NV3_PRAMDAC_CLOCK_PIXEL                         0x680508
#define NV3_PRAMDAC_COEFF_SELECT                        0x68050C

#define NV3_PRAMDAC_GENERAL_CONTROL                     0x680600
#define NV3_PRAMDAC_GENERAL_CONTROL_565_MODE            12

// These are all 10-bit values, but aligned to 32bits
// so treating them as 32bit should be fine
#define NV3_PRAMDAC_VSERR_WIDTH                         0x680700
#define NV3_PRAMDAC_VEQU_END                            0x680704
#define NV3_PRAMDAC_VBBLANK_END                         0x680708
#define NV3_PRAMDAC_VBLANK_END                          0x68070C
#define NV3_PRAMDAC_VBLANK_START                        0x680710
#define NV3_PRAMDAC_VBBLANK_START                       0x680714
#define NV3_PRAMDAC_VEQU_START                          0x680718
#define NV3_PRAMDAC_VTOTAL                              0x68071C
#define NV3_PRAMDAC_HSYNC_WIDTH                         0x680720
#define NV3_PRAMDAC_HBURST_START                        0x680724
#define NV3_PRAMDAC_HBURST_END                          0x680728
#define NV3_PRAMDAC_HBLANK_START                        0x68072C
#define NV3_PRAMDAC_HBLANK_END                          0x680730
#define NV3_PRAMDAC_HTOTAL                              0x680734
#define NV3_PRAMDAC_HEQU_WIDTH                          0x680738
#define NV3_PRAMDAC_HSERR_WIDTH                         0x68073C

#define NV3_PRAMDAC_END                                 0x680FFF
#define NV3_PDAC_END                                    0x680FFF    // OPTIONAL external DAC


#define NV3_USER_DAC_START                              0x681200
#define NV3_USER_DAC_PALETTE_START                      0x6813C6
#define NV3_USER_DAC_PIXEL_MASK                         0x6813C6
#define NV3_USER_DAC_READ_MODE_ADDRESS                  0x6813C7    //bit0=read/write mode?
#define NV3_USER_DAC_WRITE_MODE_ADDRESS                 0x6813C8
#define NV3_USER_DAC_PALETTE_DATA                       0x6813C9
#define NV3_USER_DAC_PALETTE_SIZE                       768
#define NV3_USER_DAC_PALETTE_END                        0x6813C9
#define NV3_USER_DAC_END                                0x681FFF

#define NV3_USER_START                                  0x800000    // Mapping for the area where objects are submitted into the FIFO (up to 0x880000?)
#define NV3_USER_END                                    0xFFFFFF

// easier name
#define NV3_OBJECT_SUBMIT_START                         NV3_USER_START
#define NV3_OBJECT_SUBMIT_SUBCHANNEL                    13
#define NV3_OBJECT_SUBMIT_CHANNEL                       16
#define NV3_OBJECT_SUBMIT_END                           NV3_USER_END

// also PDFB (Debug Framebuffer?)
#define NV3_PNVM_START                                  0x1000000   // VRAM access (max 8MB)
#define NV3_PNVM_END                                    0x17FFFFF   

// to be less confusing - NVM = "NV Memory"
#define NV3_VRAM_START                                  NV3_PNVM_START
#define NV3_VRAM_END                                    NV3_PNVM_END

// control structures for dma'd in graphics objects from pfifo
// these all have configurable sizes, define them here
#define NV3_RAMIN_START                                0x1C00000

#define NV3_RAMIN_RAMHT_START                          0x1C00000   // Hashtable for storing submitted objects
#define NV3_RAMIN_RAMHT_END                            0x1C00FFF
#define NV3_RAMIN_RAMHT_SIZE_0                         0xFFF
#define NV3_RAMIN_RAMHT_SIZE_1                         0x1FFF
#define NV3_RAMIN_RAMHT_SIZE_2                         0x3FFF
#define NV3_RAMIN_RAMHT_SIZE_3                         0x7FFF

/* OBSOLETE AREA for AUDIO probably. DO NOT USE! */
#define NV3_RAMIN_RAMAU_START                          0x1C01000   
#define NV3_RAMIN_RAMAU_END                            0x1C01BFF
#define NV3_RAMIN_RAMFC_START                          0x1C01C00   // context for unused PFIFO DMA channels
#define NV3_RAMIN_RAMFC_END                            0x1C01DFF
#define NV3_RAMIN_RAMFC_SIZE_0                         0x1FF
#define NV3_RAMIN_RAMFC_SIZE_1                         0xFFF
#define NV3_RAMIN_RAMRO_START                          0x1C01E00   // Runout area for invalid submissions
#define NV3_RAMIN_RAMRO_SIZE_0                         0x1FF
#define NV3_RAMIN_RAMRO_SIZE_1                         0x1FFF
#define NV3_RAMIN_RAMRO_END                            0x1C01FFF
#define NV3_RAMIN_RAMRM_START                          0x1C02000
#define NV3_RAMIN_RAMRM_END                            0x1C02FFF

#define NV3_RAMIN_END                                  0x1FFFFFF

// not done

// CRTC/CIO (0x3b0-0x3df)

#define NV3_CRTC_DATA_OUT                               0x3C0
#define NV3_CRTC_MISCOUT                                0x3C2

#define NV3_RMA_REGISTER_START                          0x3D0
#define NV3_RMA_REGISTER_END                            0x3D3

#define NV3_CRTC_REGISTER_INDEX                         0x3D4 
#define NV3_CRTC_REGISTER_CURRENT                       0x3D5

// These are standard (0-18h)
#define NV3_CRTC_REGISTER_HTOTAL                        0x00
#define NV3_CRTC_REGISTER_HDISPEND                      0x01
#define NV3_CRTC_REGISTER_HBLANKSTART                   0x02
#define NV3_CRTC_REGISTER_HBLANKEND                     0x03
#define NV3_CRTC_REGISTER_HRETRACESTART                 0x04
#define NV3_CRTC_REGISTER_HRETRACEEND                   0x05
#define NV3_CRTC_REGISTER_CURSOR_START_DISABLED         5

#define NV3_CRTC_REGISTER_VTOTAL                        0x06
#define NV3_CRTC_REGISTER_OVERFLOW                      0x07
#define NV3_CRTC_REGISTER_PRESETROWSCAN                 0x08
#define NV3_CRTC_REGISTER_MAXSCAN                       0x09
#define NV3_CRTC_REGISTER_CURSOR_START                  0x0A
#define NV3_CRTC_REGISTER_CURSOR_END                    0x0B
#define NV3_CRTC_REGISTER_STARTADDR_HIGH                0x0C
#define NV3_CRTC_REGISTER_STARTADDR_LOW                 0x0D
#define NV3_CRTC_REGISTER_CURSORLOCATION_HIGH           0x0E
#define NV3_CRTC_REGISTER_CURSORLOCATION_LOW            0x0F
#define NV3_CRTC_REGISTER_VRETRACESTART                 0x10
#define NV3_CRTC_REGISTER_VRETRACEEND                   0x11
#define NV3_CRTC_REGISTER_VDISPEND                      0x12
#define NV3_CRTC_REGISTER_OFFSET                        0x13
#define NV3_CRTC_REGISTER_UNDERLINELOCATION             0x14
#define NV3_CRTC_REGISTER_STARTVBLANK                   0x15
#define NV3_CRTC_REGISTER_ENDVBLANK                     0x16
#define NV3_CRTC_REGISTER_CRTCCONTROL                   0x17
#define NV3_CRTC_REGISTER_LINECOMP                      0x18
#define NV3_CRTC_REGISTER_STANDARDVGA_END               0x18


// These are nvidia, licensed from weitek (25-63)
#define NV3_CRTC_REGISTER_RPC0                          0x19        // What does this mean?
#define NV3_CRTC_REGISTER_RPC1                          0x1A        // What does this mean?
#define NV3_CRTC_REGISTER_READ_BANK                     0x1D
#define NV3_CRTC_REGISTER_WRITE_BANK                    0x1E
#define NV3_CRTC_REGISTER_FORMAT                        0x25
#define NV3_CRTC_REGISTER_FORMAT_VDT10                  0           // Use 10 bit vtotal value instead of 8 bit
#define NV3_CRTC_REGISTER_FORMAT_VDE10                  1           // Use 10 bit dispend value instead of 8 bit
#define NV3_CRTC_REGISTER_FORMAT_VRS10                  2           // Use 10 bit vblank start value instead of 8 bit
#define NV3_CRTC_REGISTER_FORMAT_VBS10                  3           // Use 10 bit vsync start value instead of 8 bit
#define NV3_CRTC_REGISTER_FORMAT_HBE6                   4           // Use 6 bit hsync start value
#define NV3_CRTC_REGISTER_PIXELMODE                     0x28

#define NV3_CRTC_REGISTER_HEB                           0x2D        // HRS most significant bit

#define NV3_CRTC_REGISTER_CURSOR_ADDR0                  0x30        // Cursor high 
#define NV3_CRTC_REGISTER_CURSOR_ADDR1                  0x31        // Cursor low (1:0 = enable)

#define NV3_CRTC_REGISTER_PIXELMODE_VGA                 0x00        // vga textmode
#define NV3_CRTC_REGISTER_PIXELMODE_8BPP                0x01
#define NV3_CRTC_REGISTER_PIXELMODE_16BPP               0x02
#define NV3_CRTC_REGISTER_PIXELMODE_32BPP               0x03 

#define NV3_CRTC_REGISTER_RL0                           0x34
#define NV3_CRTC_REGISTER_RL1                           0x35
#define NV3_CRTC_REGISTER_RMA                           0x38        // REAL MODE ACCESS!
#define NV3_CRTC_REGISTER_I2C                           0x3E    
#define NV3_CRTC_REGISTER_I2C_GPIO                      0x3F

// where the fuck is GDC?
#define NV3_CRTC_BANKED_128K_A0000                      0x00
#define NV3_CRTC_BANKED_64K_A0000                       0x04
#define NV3_CRTC_BANKED_32K_B0000                       0x08
#define NV3_CRTC_BANKED_32K_B8000                       0x0C

#define NV3_CRTC_REGISTER_NVIDIA_END                    0x3F

// for 86box 8bit addressing
// get rid of this asap, replace with 32->8 macros
#define NV3_RMA_SIGNATURE_MSB                           0x65
#define NV3_RMA_SIGNATURE_BYTE2                         0xD0
#define NV3_RMA_SIGNATURE_BYTE1                         0x16
#define NV3_RMA_SIGNATURE_LSB                           0x2B

#define NV3_CRTC_REGISTER_RMA_MODE_MAX                  0x0F


/* 
    STRUCTURES FOR THE GPU START HERE
    OBJECT CLASS & RENDERING RELATED STUFF IS IN VID_NV3_CLASSES.H
*/

//todo: pixel format

// Master Control
typedef struct nv3_pmc_s
{
    /* 
    Holds chip manufacturing information at bootup.
    Current specification (may change later): pre-packed for convenience

    Revision A: 
        FIB Revision 1, Mask Revision B0, Implementation 1 [NV3], Architecture 3 [NV3], Manufacturer Nvidia, Foundry SGS (seems to have been the most common?)
        31:28=0000, 27:24=0000, 23:16=0003, 15:8=0001, 7:4=0001, 3:0=0001
        little endian 00000000 00000011 00000001 00010001   = 0x00300111#
    Revision B/c:
        write this later
    */
    int32_t boot;   
    int32_t interrupt_status;   // Determines if interrupts are pending for specific subsystems.
    int32_t interrupt_enable;   // Determines if interrupts are actually enabled.
    int32_t enable;             // Determines which subsystems are enabled.

} nv3_pmc_t;

// DMA & Notifier Engine
// All singing all dancing all happy happy bullshit to send dma transfers literally anywhere in your computer
//

// Not a notification status, because it's a 16-bit enum
// C23 fixes this
#define NV3_NOTIFICATION_STATUS_DONE_OK         0x0
#define NV3_NOTIFICATION_STATUS_IN_PROGRESS     0xFF
#define NV3_NOTIFICATION_STATUS_ERROR           0x100

#define NV3_NOTIFICATION_INFO_ADJUST            0       // Wut
#define NV3_NOTIFICATION_PT_PRESENT             16      // Determines if the pagetable exists.
#define NV3_NOTIFICATION_TARGET                 24      // Determines where this notification goes
#define NV3_NOTIFICATION_TARGET_NVM             0       // VRAM target for DMA
#define NV3_NOTIFICATION_TARGET_CART            1       // "Cartridge" target for dma, only mentioned in a few places, !!! NV2 LEFTOVER !!!
#define NV3_NOTIFICATION_TARGET_PCI             2       // Send the data to the host system over PCI
#define NV3_NOTIFICATION_TARGET_AGP             3       // Send the data to the host system over AGP

#define NV3_NOTIFICATION_PAGE_IS_PRESENT        0       // Determines if the page really exists
#define NV3_NOTIFICATION_PAGE_ACCESS            1       // Determines the page access type
#define NV3_NOTIFICATION_PAGE_ACCESS_READ       0x0     // If this is set, the page is read only
#define NV3_NOTIFICATION_PAGE_ACCESS_READ_WRITE 0x1     // If this is set, the page is read/write
#define NV3_NOTIFICATION_PAGE_FRAME_ADDRESS     12      // The pageframe to use 


// Core notification structure
typedef struct nv3_notification_s
{
    uint64_t nanoseconds;
    uint32_t info32;
    uint16_t info16;
    uint16_t status;
} nv3_notification_t;
