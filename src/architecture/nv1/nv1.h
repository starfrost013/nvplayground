/*
    NVPlayground
    Copyright © 2025 starfrost


    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)
    
    nv1.h: NVPlayground NV1 core
*/


#pragma once
#include <nvplayground.h>
#include <core/nvcore.h>
#include "nv1_ref.h"

//
// nv1.h: NV1 Core includes
//

typedef struct nv1_state_s
{
    bool security_breach_enabled; 
} nv1_state_t;

bool nv1_init();
bool nv1_print_info();
bool nv1_print_pci_config();
bool nv1_security_breach();