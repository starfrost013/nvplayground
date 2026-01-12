/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    kernel_core.c: Resource Manager - GPU Core implementation (based on NVIDIA NV3/NV4 Resource Manager)
*/

#include <architecture/nvidia/kernel/kernel.h>
#include "core/gpu/gpu.h"
#include "util/util.h"

void KernelSetStateFifo(gpu_state state)
{
    switch (state)
    {
        case GPU_STATE_INIT:
            kernel_gpu = calloc(1, sizeof(kernel_instance_t));

            
            Logging_Write(LOG_LEVEL_DEBUG, "GPU Kernel: Initialising PFIFO...\n");
            break; 
        default:
            break;
    }
}
