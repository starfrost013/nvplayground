/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4_ref.h: Nv4 hardware defines. A lot of this is shared with NV5, so NV5 defs will layer on top of NV4 defs. When writing nv5 code, use nv5 def if both exist.
*/


#pragma once

//
// PMC
//

#define NV4_PMC_START                   0x0

#define NV4_PMC_BOOT_0                  0x0
#define NV4_PMC_ENABLE                  0x200

#define NV4_PRAMIN_START                0x700000