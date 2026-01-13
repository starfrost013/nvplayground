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

kernel_instance_t* kernel_gpu = NULL;

#ifdef DEBUG
/* debug functionality */
const char* debug_state_names[GPU_STATE_LAST] = 
{
    "GPU_STATE_INIT",
    "GPU_STATE_RESET",
    "GPU_STATE_RENDER",
    "GPU_STATE_SHUTDOWN",
    "GPU_STATE_MODE_SWITCH",
    "GPU_STATE_CRASHED",
};
#endif

void Kernel_SetState(gpu_state state)
{
#ifdef DEBUG
    Logging_Write(LOG_LEVEL_DEBUG, "GPU Kernel/RM is transitioning to state %s", debug_state_names[state]);
#endif

    switch (state)
    {
        case GPU_STATE_INIT:
            kernel_gpu = calloc(1, sizeof(kernel_instance_t));
            break;
        case GPU_STATE_RESET:
        case GPU_STATE_SHUTDOWN:
            break; 
        default:
            Logging_Write(LOG_LEVEL_ERROR, "Kernel: INVALID GPU state %d\n", state);
            return; // skip
    }

    Kernel_SetStateFifo(state);
    Kernel_SetStateGraph(state);

    /* We have to init kerenl_gpu before subsystem init free kernel_gpu after all subsytems have shut down */

    if (state == GPU_STATE_SHUTDOWN)
        free(kernel_gpu);
}

void Kernel_Interrupt()
{
    if (!current_device.device_info.hal->interrupt_service)
        Kernel_Fatal("Kernel: No interrupt service for current GPU");

    current_device.device_info.hal->interrupt_service();
}

void Kernel_Main()
{
    Kernel_SetState(GPU_STATE_INIT);

    /* If we are initialising we have to reset */
    Kernel_SetState(GPU_STATE_RESET);

    /* todo: script */
    while (kernel_gpu->running)
    {
        Kernel_Interrupt();
    }
}

/* Triggered upon entry into an unrecoverable error condition. */
void Kernel_Fatal(const char* err)
{
    Logging_Write(LOG_LEVEL_ERROR, "******* GPU Driver Kernel reported FATAL ERROR: *******\n");
    Logging_Write(LOG_LEVEL_ERROR, "%s\n", err);
    Logging_Write(LOG_LEVEL_ERROR, "******* A recovery will be attempted. However, success cannot be guaranteed."
        "It is recommended to reboot your system. *******\n");

    Kernel_SetState(GPU_STATE_SHUTDOWN);
    NVPlay_Shutdown(NVPLAY_EXIT_CODE_KERNEL_ENTERED_CRASHED_STATE);
}