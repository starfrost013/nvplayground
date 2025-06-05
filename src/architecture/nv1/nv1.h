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
bool nv1_security_breach();