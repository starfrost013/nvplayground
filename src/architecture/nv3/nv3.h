#include <nvplayground.h>

//
// Filename: nv3.h
// Purpose: NV3/NV3T (Riva 128/128ZX) core
//

bool nv3_init();

#define NV3_MMIO_SIZE       0x1000000

#define NV3_VRAM_SIZE_2MB   0x200000
#define NV3_VRAM_SIZE_4MB   0x400000
#define NV3_VRAM_SIZE_8MB   0x800000

#define NV3_RAMIN_SIZE      0x400000