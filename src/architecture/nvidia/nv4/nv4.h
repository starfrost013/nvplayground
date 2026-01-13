/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4.h: Riva TNT architectural implementation and programming environment
*/

#pragma once 
#include <nvplay.h>
#include <architecture/nvidia/nv4/nv4_ref.h>

typedef struct nv4_state_s
{
    uint32_t original_pll_setting;  
} nv4_state_t; 

extern nv4_state_t nv4_state; 

bool NV4_Init();
bool NV4_DumpMFGInfo();
void NV4_Shutdown(); 

// NV4 interrupt
void NV4_InterruptService();

// NV4 PFIFO
bool NV4_InitFIFO();

// NV4 PGRAPH
bool NV4_InitGraph();
bool NV4_ResetGraph();
