/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    kernel.h: NVPlay Resource Manager Kernel
*/

#pragma once
#include <nvplay.h>

//
// Resource Manager 
//

// DETERMINE WHEN STUFF WAS MADE OBSOLETE AND ALSO REMOVE ALL NON-HW CLASSES
// would a hashmap be better??

typedef struct nvrm_class_s
{
    uint32_t classid;
    // the fastest way to represent this data?
    uint32_t generation_minimum;                // below nv4 doesn't count but kept practicall
    uint32_t generation_maximum;                // does not mean it is *unusable* just obsolete
    const char* name;
} nvrm_class_t;

typedef enum gpu_state_e
{
    GPU_STATE_INIT = 0,                         // Gpu state is initialising
    // CAN THESE BE UNIFIED? SHOULD THEY?
    GPU_STATE_RENDER = 1,                       // Gpu is ready to render  graphics                
    GPU_STATE_SHUTDOWN = 2,                     // GPU is shutting down
    GPU_STATE_MODE_SWITCH = 3,                  // GPU is modeswitching 
    GPU_STATE_CRASHED = 4,                      // GPU crashed
    GPU_STATE_LAST = GPU_STATE_CRASHED + 1,     // GPU last state for debug code below
} gpu_state; 


void KernelSetStateGpu(gpu_state state);
void KernelSetStateFifo(gpu_state state);
void KernelMain();

extern nvrm_class_t nvrm_class[];

/* 
    Kernel - GPU Instance 
    This is a separate structure because I want you to be able to run the REPL separately with none of the overhead of the RM
*/
typedef struct kernel_instance_s
{
    // Cursor enable state
    bool cursor_enable;
    gpu_state state; 
    
    struct kernel_mc_info_nv4
    {
        uint32_t enable;
        uint32_t intr;
        uint32_t intr_en;

    };
    /* may have to move this out */
    struct kernel_fifo_info_nv4
    {
        uint32_t intr_en;
        uint32_t intr_en_0;
        uint32_t ramht_addr;
        uint32_t ramht_size;
        uint32_t ramro_addr;
        uint32_t ramro_size;
        uint32_t ramfc_addr;
        uint32_t ramfc_size;
    };

    struct kernel_fifo_info_nv4 nv4_fifo;
} kernel_instance_t;

extern kernel_instance_t* kernel_gpu ;