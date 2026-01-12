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

kernel_instance_t* kernel_gpu = NULL;

#ifdef DEBUG
/* debug functionality */
const char* debug_state_names[GPU_STATE_LAST] = 
{
    "GPU_STATE_INIT",
    "GPU_STATE_RENDER",
    "GPU_STATE_SHUTDOWN",
    "GPU_STATE_MODE_SWITCH",
    "GPU_STATE_CRASHED",
};
#endif

void KernelSetStateGpu(gpu_state state)
{
#ifdef DEBUG
    Logging_Write(LOG_LEVEL_DEBUG, "GPU Kernel/RM is transitioning to state %s", debug_state_names[state]);
#endif

    switch (state)
    {
        case GPU_STATE_INIT:
            kernel_gpu = calloc(1, sizeof(kernel_instance_t));

            current_device.device_info.hal->fifo_init();
            break;
        default:
            Logging_Write(LOG_LEVEL_ERROR, "Kernel: INVALID gpu state %d\n", state);
            return; // skip
    }

    KernelSetStateFifo(state);
    
}

void KernelMain()
{

}