#pragma once
#include <nvplayground.h>
#include "nv3_ref.h"

//
// Filename: nv3.h
// Purpose: NV3/NV3T (RIVA 128/128ZX) core - NVPLAYGROUND ONLY, NOT API
//

bool nv3_init();

/* NV3 Tests */
bool nv3_enter_graphics_mode();
bool nv3_dump_vbios();
bool nv3_dump_mmio();
bool nv3_garbage_mmio_read();
bool nv3_print_info();
bool nv3_test_overclock(); 

/* Certain areas of MMIO are not safe to read/write, so exclude them. */
typedef struct nv3_dump_excluded_areas_s
{
    uint32_t start;
    uint32_t end; 
} nv3_dump_excluded_areas_t; 

extern nv3_dump_excluded_areas_t excluded_areas[];

bool nv3_mmio_area_is_excluded(uint32_t addr);