/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4.h: Riva TNT architectural implementation and programming environment
*/

#pragma once 
#include <nvplay.h>
#include <architecture/nv4/nv4_ref.h>

typedef struct nv4_state_s
{
    uint32_t original_pll_setting;  
} nv4_state_t; 

extern nv4_state_t nv4_state; 

bool nv4_init();

bool nv4_dump_mfg_info();
bool nv4_dump_mmio();
bool nv4_dump_vbios(); 

void nv4_shutdown(); 