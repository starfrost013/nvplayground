/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_io.h: Headers for GPU I/O 
*/

#pragma once
#include <nvplay.h>

//
// READ/WRITE functions for GPU memory areas
//

/* Generic definition for all tests, used to prevent expanding the nv_test static initialisers */
#define PCI_VENDOR_GENERIC			0x0000
#define PCI_DEVICE_GENERIC			0x0000		

/* Device definitions */
#define PCI_VENDOR_SGS              0x104A      // Used for NV1, STG-2000 variant
#define PCI_VENDOR_SGS_NV           0x12D2      // Used for NV3/NV3T
#define PCI_VENDOR_NV               0x10DE      // Used for NV1, NV1 variant, and NV4+


#define PCI_DEVICE_NV1_VGA          0x0008      // NV1 VGA component    	1995
#define PCI_DEVICE_NV1_NV           0x0009      // NV1 NV component     	1995
#define PCI_DEVICE_NV2              0x0010      // Mutara               	1995-1996 (cancelled)
#define PCI_DEVICE_NV3              0x0018      // RIVA 128, RIVA 128 ZX	1997-1998
#define PCI_DEVICE_NV3T_ACPI		0x0019		// RIVA 128 ZX, with ACPI	1998
#define PCI_DEVICE_NV4				0x0020		// RIVA TNT					1998
#define PCI_DEVICE_NV5				0x0028		// RIVA TNT2 / TNT2 Pro		1999
#define PCI_DEVICE_NV5_ULTRA		0x0029		// RIVA TNT2 Ultra			1999
#define PCI_DEVICE_NV5_CRAP			0x002C		// Vanta					2000
/* Yes this is considered "NV6" for some reason */
#define PCI_DEVICE_NV6				0x002D		// RIVA TNT2 M64			1999

// Celsius (NV1x; TnL)
#define PCI_DEVICE_NV10				0x0100		// GeForce 256 (SDRAM)		1999
#define PCI_DEVICE_NV10_DDR			0x0101		// GeForce 256 (DDR1)		1999
#define PCI_DEVICE_NV10_QUADRO		0x0103		// Quadro					2000 (aka NV10GL)
#define PCI_DEVICE_NV11_MX400		0x0110		// GeForce 2 MX400			2001 (has some stuff NV15 doesn't!)
#define PCI_DEVICE_NV11_MX200		0x0111		// GeForce 2 MX100/200		2000 (has some stuff NV15 doesn't!)
#define PCI_DEVICE_NV11_GO			0x0112		// GeForce 2 Go				2000 (has some stuff NV15 doesn't!)
#define PCI_DEVICE_NV11_QUADRO		0x0113		// Quadro 2 MX				2000 (has some stuff NV15 doesn't!)
#define PCI_DEVICE_NV15				0x0150		// GeForce 2				2000
#define PCI_DEVICE_NV15_TI			0x0151		// GeForce 2 Ti				2000
#define PCI_DEVICE_NV15_ULTRA		0x0152		// GeForce 2 Ultra			2001
#define PCI_DEVICE_NV15_QUADRO		0x0153		// Quadro2					2000

// NV17/18 (Many IDs)
#define PCI_DEVICE_NV17_START		0x0170		// GeForce 4 MX				2002
#define PCI_DEVICE_NV17_END			0x017F		// GeForce 4 MX				2002
#define PCI_DEVICE_NV18_START		0x0180		// GeForce 4 MX AGP-8X		2002-2003
#define PCI_DEVICE_NV18_END			0x018D		// GeForce 4 MX AGP-8X		2002-2003

#define PCI_DEVICE_NV1A				0x01A0		// GeForce 2 IGP			~2000
#define PCI_DEVICE_NV1F				0x01F0		// GeForce 4 MX IGP			2002

// Test to see how plausible non-NVidia hardware is
#define PCI_VENDOR_S3               0x5333      // S3 Graphics, Inc.
#define PCI_DEVICE_VIRGE_325        0x5631      // ViRGE                    1996

#define PCI_VENDOR_CIRRUS           0x1013      // Cirrus Logic 
#define PCI_DEVICE_CIRRUS_5446      0x00B8      // GD-5446                  1995(?)

/* 
    NV_PMC_BOOT values 
    There's two ways to identify an NV GPU: By reading PCI config registers and by reading the NV_PMC_BOOT register
    We read PCI config registers to determine the overall model and hten read the pfb_boot register to get the stepping
*/

// Default values for the boot information register.
// Depends on the chip

// One of the only registers that existed from NV1 on
#define NV_PMC_BOOT					0x00000000

// NV1 (1994-1995)

#define NV_PMC_BOOT_NV1_A01     	0x00010100      // NV1 Stepping A0 (Prototype) 				1994
#define NV_PMC_BOOT_NV1_B01     	0x00010101      // NV1 Stepping B0 (Prototype) 				1995
#define NV_PMC_BOOT_NV1_B02     	0x00010102      // NV1 Stepping B1 (Prototype) 				1995
#define NV_PMC_BOOT_NV1_B03     	0x00010103      // NV1 Stepping B2 (Prototype)				1995
#define NV_PMC_BOOT_NV1_C01     	0x00010104      // NV1 Stepping C0 (Final)	   				1995

// NV2 (1996)

#define NV_PMC_BOOT_NV2_A01			0x10020400		// NV2 Stepping A0 (Prototype) 				1995/1996 (Helios Semiconductor) 

// RIVA 128 (1997)

#define NV_PMC_BOOT_NV3_A00         0x00030100      // NV3 Stepping A0 (Prototype) 				April 1997
#define NV_PMC_BOOT_NV3_B00         0x00030110		// NV3 Stepping B0 (Final)					July(?) 1997
#define NV_PMC_BOOT_NV3T_A01_ST     0x00030120		// NV3 Stepping C0 / NV3T Stepping A1		Early 1998		(STMicro-manufactured)
#define NV_PMC_BOOT_NV3T_A02_ST     0x00030121		// NV3 Stepping C1 / NV3T Stepping A2		Early 1998		(STMicro-manufactured)
#define NV_PMC_BOOT_NV3T_A03_ST 	0x00030122		// NV3 Stepping C2/C3 / NV3 Stepping A3/A4	1998/9			(STMicro-manufactured)
#define NV_PMC_BOOT_NV3T_A01_TSMC   0x20030120		// NV3 Stepping C0 / NV3T Stepping A1		Early 1998 		(TSMC-manufactured)
#define NV_PMC_BOOT_NV3T_A02_TSMC   0x20030121		// NV3 Stepping C1 / NV3T Stepping A2		Early 1998 		(TSMC-manufactured)
#define NV_PMC_BOOT_NV3T_A03_TSMC 	0x20030122		// NV3 Stepping C2/C3 / NV3 Stepping A3/A4	1998/9			(TSMC-manufactured)

/* Temp stuff */
#define NV3_TESTOVERCLOCK_TIME_BETWEEN_RECLOCKS        60
#define NV3_TESTOVERCLOCK_BASE_13500                   0x1A30B
#define NV3_TESTOVERCLOCK_BASE_14318					0x1C40E

// RIVA TNT1 (1998)
#define NV_PMC_BOOT_NV4_A01			0x20004000		// NV4 Stepping A1/A2/A3					?December 1997? (TSMC-manufactured)
#define NV_PMC_BOOT_NV4_A04			0x20034001		// NV4 Stepping A4							?August 1998?
#define NV_PMC_BOOT_NV4_A05			0x20044001		// NV4 Stepping A5							?Late 1998>/

#define NV_PMC_BOOT_NV5_A01			0x20104000		// NV5/NV6 Stepping A1						?Late 1998?
#define NV_PMC_BOOT_NV5_A02			0x20114000		// NV5/NV6 Stepping A2						?Early 1999?
#define NV_PMC_BOOT_NV5_A03			0x20124000		// NV5/NV6 Stepping A3						?Early 1999?
#define NV_PMC_BOOT_NV5_B01			0x20204000		// NV5/NV6 Stepping B1						?Early 1999?
#define NV_PMC_BOOT_NV5_B02			0x20214000		// NV5/NV6 Stepping B2						?Early 1999?
#define NV_PMC_BOOT_NV5_B03			0x20224000		// NV5/NV6 Stepping B3						?Early 1999?

// NV10+ uses multiple device ids per gpu stepping and a hex representation of the stepping
#define NV_PMC_BOOT_NV10_BASE		0x01000000

// Hardware Abstraction Layer entry
// All hardware-specific stuff
typedef struct nvhal_entry_s
{
    bool (*init_function)();							// Function to call on entry point	
	void (*shutdown_function)();						// Function to call on shutdown

    // TEST functions
    void (*dump_fifo_to_text_file)(FILE* stream);
    void (*dump_ramht_to_text_file)(FILE* stream);
    void (*dump_ramfc_to_text_file)(FILE* stream);
    void (*dump_ramro_to_text_file)(FILE* stream);
    void (*dump_cache_to_text_file)(FILE* stream);

    // KERNEL functions
    bool (*fifo_init)();

    // RENDERING functions
    void (*submit_object)(uint32_t name, uint32_t context);
    void (*submit_method)(uint32_t method, uint32_t param);
} nvhal_entry_t;   

/* Graphics Device Definition */
typedef struct nv_device_info_s
{
	uint32_t device_id_start;							// First device ID of the GPU
	uint32_t device_id_end;								// Last device ID of the GPU
	uint32_t vendor_id;									// Vendor ID of the GPU
	const char* name; 									// Friendly name of the GPU
	nvhal_entry_t* hal;									// HAL entry to use for the GPU
} nv_device_info_t; 

/* List of supported devices */
extern nv_device_info_t supported_devices[]; 

/* Device information for PCI/AGP/PCIe bus */
typedef struct nv_device_pci_s
{
	uint32_t bus_number;			// PCI bus number
	uint32_t function_number; 		// PCI function number
    void* bar0;						// PCI BAR0 mapping - gpu stuff
	void* bar1;						// PCI BAR1 mapping for DFB
	int32_t bar0_selector;			// MUST BE USED FOR ACCESS TO BAR0
	int32_t bar1_selector;			// MUST BE USED FOR ACCESS TO BAR1

} nv_device_bus_t;

/* Full NV Device Struct (shared across all devices) */
typedef struct nv_device_s
{
	nv_device_info_t device_info;
    nv_device_bus_t bus_info;

	uint32_t real_device_id;		// real device id
	uint32_t ramin_start; 			// RAMIN start address
	uint32_t vram_amount;			// Amount of Video RAM
	/* Some registers shared between all gpus */
	uint32_t nv_pfb_boot_0;			// nv_pfb_boot_0 register read at boot
	uint32_t nv_pmc_boot_0;			// nv_pmc_boot_0 register read at boot
	uint32_t nv10_pfb_cfg;			// [NV10+] nv10 pfb_cfg
	uint32_t straps;				// Straps for oem-specific config
	double crystal_hz;				// Clock crystal base (TODO: fully refactor so this is not needed)

	uint32_t mpll;					// [NV1+] Core Clock [NV4+] Memory Clock
	uint32_t vpll;					// [NV1+] Video Clock
	uint32_t nvpll;					// [NV4+] Core Clock

    uint32_t revision;              // GPU specific implementation

	bool initialised;				// Initialsied
} nv_device_t;

extern nv_device_t current_device;

// Detection functions
bool GPU_Detect(); 

// Allow easy range checks for generic tests 
// Don't use #defines because some gpus have multiple ID "sets" and I don't like complex macros
// NV1orBetter not needed

// Generic function checking if the installed GPU is an NV1
static inline bool GPU_IsNV1()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV1_A01
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV1_C01);
}

// Generic function checking if the installed GPU is an NV2
static inline bool GPU_IsNV2()
{
    return (current_device.nv_pmc_boot_0 == NV_PMC_BOOT_NV2_A01);
}

// Generic function checking if the installed GPU is an NV3 series card. Checks for NV3 *AND* NV3T
static inline bool GPU_IsNV3()
{
    // NV3 has multiple manufacturing fabs, but also shares the architecture with certain NV4 steppings , so we have to be slightly clever and check for 0x30
    return ((current_device.nv_pmc_boot_0 & 0xFFFFF) >> 12) == 0x30;
}

// Generic function checking if the installed GPU is an NV3 revision A or B (2/4MB)
static inline bool GPU_IsNV3AorB()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV3_A00 
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV3_B00);
}

// Generic function checking if the installed GPU is an NV3T (Turbo) (RIVA 128 ZX)
static inline bool GPU_IsNV3T()
{
    return ((current_device.nv_pmc_boot_0 & 0xFFFFF) >> 12) == 0x30
    && (current_device.nv_pmc_boot_0 & 0xFF) >= 0x20; // Revision C
}

static inline bool GPU_IsNV4()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV4_A01
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV4_A05);
}

static inline bool GPU_IsNV4orBetter()
{
    // Special-case NV4 a1 (even though it was probably only ever an ES) since its fabless ID number is less(!) than NV1/2/3!
    return (current_device.nv_pmc_boot_0 == NV_PMC_BOOT_NV4_A01
    || (current_device.nv_pmc_boot_0 & 0xFFFFFFF) >= 0x34001);
}

static inline bool GPU_IsNV5()
{
    return (current_device.nv_pmc_boot_0 >= NV_PMC_BOOT_NV5_A01
    && current_device.nv_pmc_boot_0 <= NV_PMC_BOOT_NV5_B03);
}

static inline bool GPU_IsNV10()
{
    // No pre-NV10 gpu has >=0x1000000 if fab removed
    return (current_device.nv_pmc_boot_0 & 0xFFFFFFF) >= NV_PMC_BOOT_NV10_BASE;
}

// Obtain the generation of Nvidia-based graphics hardware
static inline __attribute__((always_inline)) uint32_t GPU_NV_GetGeneration()
{
	// We don't support any thing above Nv10 rn
	
	// NV10 is 0x0100 and doesn't use the nv1-5 era device id format
	if (GPU_IsNV10())
		return 0x10;
	else
		return (current_device.real_device_id & ~8) >> 3;
}

// only 8 and 32 bit are really needed
uint8_t NV_ReadMMIO8(uint32_t offset); 
uint32_t NV_ReadMMIO32(uint32_t offset); 
void NV_WriteMMIO8(uint32_t offset, uint8_t val);
void NV_WriteMMIO32(uint32_t offset, uint32_t val);

/* Requires some special dispensations if the bus size is 64-bit and there is only 2 MB of VRAM */
uint8_t NV_ReadDfb8(uint32_t offset); 
uint16_t NV_ReadDfb16(uint32_t offset); 
uint32_t NV_ReadDfb32(uint32_t offset); 
void NV_WriteDfb8(uint32_t offset, uint8_t val);
void NV_WriteDfb16(uint32_t offset, uint16_t val);
void NV_WriteDfb32(uint32_t offset, uint32_t val);

// RAMIN is always read as 32bit
uint32_t NV_ReadRamin32(uint32_t offset); 
void NV_WriteRamin32(uint32_t offset, uint32_t val);

// NV-VGA
void NV_CRTCLockExtendedRegisters();
void NV_CRTCUnlockExtendedRegisters();

// Clock

#define NV_CLOCK_BASE_13500K				13500000.0
#define NV_CLOCK_BASE_14318180				14318180.0

double NV_ClockMNPToMhz(uint32_t clock_base, uint32_t mnp);

//
// VGA common stuff
// (Can be used on all gpus theoretically)
//

#define VGA_PORT_ATTRIBUTE_REGISTER				0x3C0		// This doesn't use index
#define VGA_PORT_ATTRIBUTE_DATA_WRITE			0x3C1
#define VGA_PORT_MISCOUT						0x3C2
#define VGA_PORT_SEQUENCER_INDEX				0x3C4
#define VGA_PORT_SEQUENCER						0x3C5
#define VGA_PORT_GRAPHICS_INDEX					0x3CE
#define VGA_PORT_GRAPHICS						0x3CF
#define VGA_PORT_MONO_CRTC_INDEX				0x3B4
#define VGA_PORT_MONO_CRTC						0x3B5
#define VGA_PORT_COLOR_CRTC_INDEX				0x3D4
#define VGA_PORT_COLOR_CRTC						0x3D5
#define VGA_PORT_INPUT0_MONO					0x3BA
#define VGA_PORT_INPUT0_COLOR					0x3DA

#define VGA_REALMODE_VBIOS_LOCATION				0xC0000

uint8_t VGA_ReadCRTC(uint8_t index);
uint8_t VGA_ReadSequencer(uint8_t index);
uint8_t VGA_ReadAttribute(uint8_t index);
uint8_t VGA_ReadGraphics(uint8_t index);
void VGA_WriteCRTC(uint8_t index, uint8_t value);
void VGA_WriteGDC(uint8_t index, uint8_t value);
void VGA_WriteSequencer(uint8_t index, uint8_t value);
void VGA_WriteAttribute(uint8_t index, uint8_t value);
void VGA_WriteGraphics(uint8_t index, uint8_t value);
