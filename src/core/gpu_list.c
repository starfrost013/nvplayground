/* 
	NVPlay
	Copyright © 2025 starfrost

	Raw GPU programming for early Nvidia GPUs
	Licensed under the MIT license (see license file)

	gpu_list.c: Enumerates all supported graphics hardware.
*/

// Architecture Includes
#include "nvplay.h"
#include <architecture/generic/nv_generic.h>
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>

// Globals
//
nv_device_info_t supported_devices[] = 
{
	// Quadros are separately identified
	// Todo: are the PCIe NV19s actually a separate ID? If so, how do we identify them?
	{ PCI_DEVICE_NV1_NV, PCI_DEVICE_NV1_NV, PCI_VENDOR_SGS, "NV1 (STG-2000 DRAM version)", &nvhal_nv1 },
	{ PCI_DEVICE_NV1_NV, PCI_DEVICE_NV1_NV, PCI_VENDOR_NV, "NV1 (NV1 VRAM version)", &nvhal_nv1 },
	{ PCI_DEVICE_NV2, PCI_DEVICE_NV2, PCI_VENDOR_NV, "NV2 (Mutara V08) (You don't have this)", &nvhal_nv2 },
	{ PCI_DEVICE_NV3, PCI_DEVICE_NV3T_ACPI, PCI_VENDOR_SGS_NV, "RIVA 128 (NV3) or RIVA 128 ZX (NV3T)", &nvhal_nv3 }, 
	{ PCI_DEVICE_NV4, PCI_DEVICE_NV4, PCI_VENDOR_NV, "RIVA TNT (NV4)", &nvhal_nv4 },
	{ PCI_DEVICE_NV5, PCI_DEVICE_NV5_CRAP, PCI_VENDOR_NV, "RIVA TNT2-based GPU (NV5) - TNT2/Pro/Ultra", &nvhal_nv4 },
	{ PCI_DEVICE_NV6, PCI_DEVICE_NV6, PCI_VENDOR_NV, "RIVA TNT2 M64 (NV6)", &nvhal_nv4 },
	{ PCI_DEVICE_NV10, PCI_DEVICE_NV10, PCI_VENDOR_NV, "GeForce 256 with SDRAM (NV10)", &nvhal_celsius },
	{ PCI_DEVICE_NV10_DDR, PCI_DEVICE_NV10_DDR, PCI_VENDOR_NV, "GeForce 256 with DDR (NV10DDR)", &nvhal_celsius },
	{ PCI_DEVICE_NV10_QUADRO, PCI_DEVICE_NV10_QUADRO, PCI_VENDOR_NV, "Quadro (NV10GL)", &nvhal_celsius },
	{ PCI_DEVICE_NV15, PCI_DEVICE_NV15_ULTRA, PCI_VENDOR_NV, "GeForce 2 (NV15)", &nvhal_celsius },
	{ PCI_DEVICE_NV15_QUADRO, PCI_DEVICE_NV15_QUADRO, PCI_VENDOR_NV, "Quadro 2 (NV15GL)", &nvhal_celsius },
	{ PCI_DEVICE_NV11_MX400, PCI_DEVICE_NV11_GO, PCI_VENDOR_NV, "GeForce 2 MX/Go (NV11)", &nvhal_celsius },
	{ PCI_DEVICE_NV11_QUADRO, PCI_DEVICE_NV11_QUADRO, PCI_VENDOR_NV, "Quadro 2 MX (NV11GL)", &nvhal_celsius },
	{ PCI_DEVICE_NV17_START, PCI_DEVICE_NV17_END, PCI_VENDOR_NV, "GeForce 4 MX-based GPU (NV17)", &nvhal_celsius },
	{ PCI_DEVICE_NV18_START, PCI_DEVICE_NV18_END, PCI_VENDOR_NV, "GeForce 4 MX-based GPU with AGP 8X support (NV18)",& nvhal_celsius },
	{ PCI_DEVICE_NV1A, PCI_DEVICE_NV1A, PCI_VENDOR_NV, "GeForce 2 IGP (NV1A)", &nvhal_celsius },
	{ PCI_DEVICE_NV1F, PCI_DEVICE_NV1F, PCI_VENDOR_NV, "GeForce 4 MX IGP (NV1AF)", &nvhal_celsius },

	{ 0, 0, 0, "", NULL }, // sentinel
};
