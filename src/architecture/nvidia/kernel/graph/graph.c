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

void Kernel_SetStateGraph(gpu_state state)
{
    switch (state)
    {
        case GPU_STATE_INIT:
            if (!current_device.device_info.hal->graph_init)
                Kernel_Fatal("Kernel_SetStateGraph: No PGRAPH initialisation function for the current GPU");

            Logging_Write(LOG_LEVEL_DEBUG, "GPU Kernel: Initialising PGRAPH...\n");

            if (!current_device.device_info.hal->graph_init())
                Kernel_Fatal("Kernel_SetStateGraph: Failed to initialise PGRAPH");

            break; 
        case GPU_STATE_RESET:
            if (!current_device.device_info.hal->graph_reset)
                Kernel_Fatal("Kernel_SetStateGraph: No PGRAPH reset function for the current GPU");

            Logging_Write(LOG_LEVEL_DEBUG, "GPU Kernel: Resetting PGRAPH...\n");

            if (!current_device.device_info.hal->graph_reset)
                Kernel_Fatal("Kernel_SetStateGraph: Failed to reset PGRAPH");

            break; 
        default:
            break;
    }
}
