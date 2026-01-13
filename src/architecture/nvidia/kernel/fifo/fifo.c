/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    kernel_core.c: Resource Manager - GPU Core implementation (based on NVIDIA NV3/NV4 Resource Manager)
*/

#include <architecture/nvidia/kernel/kernel.h>
#include "core/gpu/gpu.h"
#include "nvplay.h"
#include "util/util.h"

void Kernel_SetStateFifo(gpu_state state)
{
    switch (state)
    {
        case GPU_STATE_INIT:
            if (!current_device.device_info.hal->fifo_init)
                Kernel_Fatal("KernelSetStateFifo: No FIFO initialisation function for the current GPU");
           
            Logging_Write(LOG_LEVEL_DEBUG, "GPU Kernel: Initialising PFIFO...\n");

            if (!current_device.device_info.hal->fifo_init())
                Kernel_Fatal("KernelSetStateFifo: Failed to initialise PFIFO");
            
            break; 
        default:
            break;
    }
}
