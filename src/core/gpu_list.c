/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_list.c: Enumerates all supported graphics hardware.
*/


// Architecture Includes
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>

// Globals
//
nv_device_info_t supported_devices[] = 
{
	{ PCI_DEVICE_NV1_NV, PCI_VENDOR_SGS, "NV1 (STG-2000 DRAM version)", nv1_init, NULL, NULL, nv1_gpus_section_applies, nv1_gpus_parse_section, },
	{ PCI_DEVICE_NV1_NV, PCI_VENDOR_NV, "NV1 (NV1 VRAM version)", nv1_init, NULL, NULL, NULL, NULL, },
    { PCI_DEVICE_NV2, PCI_VENDOR_NV, "NV2 (Mutara V08) (You don't have this)", NULL, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV3, PCI_VENDOR_SGS_NV, "RIVA 128 (NV3), or RIVA 128 ZX without ACPI support (NV3T)", nv3_init, NULL, NULL, NULL, NULL, }, 
	{ PCI_DEVICE_NV3T_ACPI, PCI_VENDOR_SGS_NV, "RIVA 128 ZX with ACPI support (NV3T)", nv3_init, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV4, PCI_VENDOR_NV, "RIVA TNT (NV4)", nv4_init, NULL, NULL, nv4_shutdown, NULL, },
	{ PCI_DEVICE_NV5, PCI_VENDOR_NV, "RIVA TNT2 / TNT2 Pro (NV5)", NULL, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV5_ULTRA, PCI_VENDOR_NV, "RIVA TNT2 Ultra (NV5_ULTRA)", NULL, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV5_CRAP, PCI_VENDOR_NV, "Vanta (RIVA TNT2 derivative) (NV5_VANTA)", NULL, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV6, PCI_VENDOR_NV, "RIVA TNT2 M64 (NV6)", NULL, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV10, PCI_VENDOR_NV, "GeForce 256 with SDRAM (NV10)", NULL, NULL, NULL, NULL, NULL, },
	{ PCI_DEVICE_NV10_DDR, PCI_VENDOR_NV, "GeForce 256 with DDR (NV10)", NULL, NULL, NULL, NULL,NULL,  },
	{ PCI_DEVICE_NV10_QUADRO, PCI_VENDOR_NV, "Quadro (NV10GL)", NULL, NULL, NULL, NULL, NULL, },
	{ 0, 0, "", NULL, NULL, NULL, NULL, }, // sentinel
};
