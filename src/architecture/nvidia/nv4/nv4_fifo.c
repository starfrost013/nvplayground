/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4_core.c: NV4/NV5 (TNT series) initialisation and shutdown
*/

#include "core/gpu/gpu.h"
#include "nv4_ref.h"
#include "nvplay.h"

#include <architecture/nvidia/kernel/kernel.h>
#include <architecture/nvidia/nv4/nv4.h>

/* Init FIFO for rendering - NV4 */
bool NV4_InitFIFO()
{

    // Enable PFIFO interrupts
    uint32_t fifo_intr_en = (1 << NV4_PFIFO_INTR_EN_0_CACHE_ERROR
    | (1 << NV4_PFIFO_INTR_EN_0_DMA_PUSHER)             // DMA pusher
    | (1 << NV4_PFIFO_INTR_EN_0_DMA_PT)
    | (1 << NV4_PFIFO_INTR_EN_0_RUNOUT)
    | (1 << NV4_PFIFO_INTR_EN_0_RUNOUT_OVERFLOW)        // very bad
    );

    kernel_gpu->nv4_pfifo.intr_en = fifo_intr_en;

    NV_WriteMMIO32(NV4_PFIFO_INTR_EN_0, fifo_intr_en);

    // Reset FIFO state
    NV_WriteMMIO32(NV4_PFIFO_CACHE1_GET, 0);
    NV_WriteMMIO32(NV4_PFIFO_CACHE1_PUT, 0);
    NV_WriteMMIO32(NV4_PFIFO_CACHE1_DMA_GET, 0);
    NV_WriteMMIO32(NV4_PFIFO_CACHE1_DMA_PUT, 0);
    NV_WriteMMIO32(NV4_PFIFO_CACHE0_HASH, 0);
    NV_WriteMMIO32(NV4_PFIFO_CACHE1_HASH, 0);

    NV_WriteMMIO32(NV4_PFIFO_MODE, 0x0);
    NV_WriteMMIO32(NV4_PFIFO_DMA, 0x0);
    NV_WriteMMIO32(NV4_PFIFO_SIZE, 0x0);

    // Reset RAMRO
    NV_WriteMMIO32(NV4_PFIFO_RUNOUT_GET, 0x0);
    NV_WriteMMIO32(NV4_PFIFO_RUNOUT_PUT, 0x0);


    return true; 
}