#pragma once
#include <nvplayground.h>
#include "nv3_ref.h"
#include "nv3_state.h"

//
// Filename: nv3.h
// Purpose: NV3/NV3T (Riva 128/128ZX) core
//

bool nv3_init();

/* NV3 Tests */
bool nv3_dump_vbios();
bool nv3_dump_mmio();
bool nv3_print_info();
bool nv3_test_overclock(); 