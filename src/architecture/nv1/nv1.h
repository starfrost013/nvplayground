/*
    NVPlayground
    Copyright © 2025 starfrost


    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)
    
    nv1.h: NVPlayground NV1 core
*/


#pragma once
#include <nvplayground.h>

#include "nv1_ref.h"

//
// nv1.h: NV1 Core includes
//

typedef struct nv1_state_s
{
    bool security_breach_enabled; 
    uint64_t chip_token;
} nv1_state_t;

extern nv1_state_t nv1_state;

bool nv1_init();
bool nv1_print_info();
bool nv1_print_pci_config();
bool nv1_security_breach();
bool nv1_dump_vbios();

// GPUS stuff
bool nv1_gpus_section_applies(uint32_t fourcc);
void nv1_gpus_parse_section(uint32_t fourcc);