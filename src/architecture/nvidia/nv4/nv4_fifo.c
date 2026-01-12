/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4_core.c: NV4/NV5 (TNT series) initialisation and shutdown
*/

#include "nv4_ref.h"
#include "nvplay.h"

#include <architecture/nvidia/kernel/kernel.h>
#include <architecture/nvidia/nv4/nv4.h>

/* Init FIFO for rendering - NV4 */
void NV4_InitFIFO()
{
    kernel_gpu->nv4_fifo.intr_en = NV_ReadMMIO32(NV4_PFIFO_INTR_EN_0);
}