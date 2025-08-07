/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv3.h: NV3/NV3T (RIVA 128/128ZX) core - NVPLAYGROUND ONLY, NOT API
*/

#pragma once
#include <nvplayground.h>
#include "nv3_ref.h"

//
// Structures
//

/* Certain areas of MMIO are not safe to read/write and crash the gpu , so exclude them. */
typedef struct nv3_dump_excluded_areas_s
{
    uint32_t start;
    uint32_t end; 
} nv3_dump_excluded_areas_t; 

//
// Functions
//

bool nv3_init();

/* NV3 Tests */
bool nv3_dump_vbios();
bool nv3_dump_mmio();
bool nv3_garbage_mmio_read();
bool nv3_print_info();
bool nv3_test_overclock(); 


extern nv3_dump_excluded_areas_t excluded_areas[];

bool nv3_mmio_area_is_excluded(uint32_t addr);

// GPUS stuff
bool nv3_gpus_section_applies(uint32_t fourcc);
void nv3_gpus_parse_section(uint32_t fourcc);