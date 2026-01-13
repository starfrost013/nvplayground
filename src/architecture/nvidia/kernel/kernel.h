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
    GPU_STATE_RESET = 1,                        // GPU needs to be reset (May rename)
    // CAN THESE BE UNIFIED? SHOULD THEY?
    GPU_STATE_RENDER = 2,                       // Gpu is ready to render graphics                
    GPU_STATE_SHUTDOWN = 3,                     // GPU is shutting down
    GPU_STATE_MODE_SWITCH = 4,                  // GPU is modeswitching 
    GPU_STATE_CRASHED = 5,                      // GPU crashed
    GPU_STATE_LAST = GPU_STATE_CRASHED + 1,     // GPU last state for debug code below
} gpu_state; 

void Kernel_Main();                             // GPU driver main function
void Kernel_Fatal(const char* err);             // Triggered upon entry into an unrecoverable error condition.
void Kernel_SetState(gpu_state state);          // State transition

// State transitions
void Kernel_SetStateFifo(gpu_state state);
void Kernel_SetStateGraph(gpu_state state);

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
    } nv4_pmc;

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
    } nv4_pfifo;

    /* Graphics patch */
    struct kernel_graph_info_nv4
    {
        uint32_t debug_0;
        uint32_t debug_1;
        uint32_t debug_2;
        uint32_t debug_3;
        uint32_t ctx_control;           // how to interpret object
        uint32_t channel_id;
    } nv4_pgraph;

} kernel_instance_t;

extern kernel_instance_t* kernel_gpu;