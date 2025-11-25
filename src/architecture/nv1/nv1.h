/*
    NVPlay
    Copyright © 2025 starfrost


    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)
    
    nv1.h: NVPlay NV1 core
*/


#pragma once
#include <nvplay.h>

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
