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

void NV4_InterruptService()
{
    uint32_t intr = NV_ReadMMIO32(NV4_PMC_INTR_0);
    uint32_t intr_en = NV_ReadMMIO32(NV4_PMC_INTR_EN_0);

    if (!intr_en)
        return;

    // send the interrupt to the subsystem 
    
    if (intr & (1 << NV4_PMC_INTR_0_PFIFO_PENDING)
        && intr_en & (1 << NV4_PMC_INTR_0_PFIFO_PENDING))
    {
        NV4_InterruptFIFO();
    }

    if (intr & (1 << NV4_PMC_INTR_0_PGRAPH_PENDING)
        && intr_en & (1 << NV4_PMC_INTR_0_PGRAPH_PENDING))
    {
        NV4_InterruptGraph();

    }
}