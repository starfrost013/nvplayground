#pragma once
#include <nvplayground.h>


/* 
    Filename: nvcore.h
    Purpose: Shared files between all NV cards, including core PCI defines
*/

/* PCI BIOS magic */
#define PCI_BIOS_MAGIC						0x20494350

/* PCI Type-0 header. We don't care about PCI to PCI bridges (type 1) or PCI to CardBus bridges (type 2) */
#define PCI_CFG_OFFSET_VENDOR_ID			0x00
#define PCI_CFG_OFFSET_DEVICE_ID			0x02
#define PCI_CFG_OFFSET_COMMAND				0x04

#define PCI_CFG_OFFSET_COMMAND_IO_ENABLED	0x01
#define PCI_CFG_OFFSET_COMMAND_MEM_ENABLED	0x02

#define PCI_CFG_OFFSET_STATUS				0x06
#define PCI_CFG_OFFSET_REVISION				0x08
#define PCI_CFG_OFFSET_CLASS_CODE			0x09	
#define PCI_CFG_OFFSET_CLASS_CODE_HIGH 		0x09
#define PCI_CFG_OFFSET_CLASS_CODE_LOW		0x0A
#define PCI_CFG_OFFSET_CACHE_LINE_SIZE		0x0C
#define PCI_CFG_OFFSET_LATENCY_TIMER		0x0D
#define PCI_CFG_OFFSET_HEADER_TYPE			0x0E
#define PCI_CFG_OFFSET_BIST					0x0F

/* BARs. Only 0/1 matter for supported NV cards and 0/1/2 for any NV cards but for PCI dumping code we dump everything 

Well, I guess they would all be used for 64-bit mapping. But we only care about 32-bit
*/
#define PCI_CFG_OFFSET_BAR0					0x10	// Main GPU MMIO
#define PCI_CFG_OFFSET_BAR1					0x14	// VRAM + RAMIN (on nv3), otherwise just dumb framebuffer
#define PCI_CFG_OFFSET_BAR2					0x18	// NV20+
#define PCI_CFG_OFFSET_BAR3					0x1C	// For dumping purposes
#define PCI_CFG_OFFSET_BAR4					0x20	// For dumping purposes
#define PCI_CFG_OFFSET_BAR5					0x24	// For dumping purposes

#define PCI_CFG_OFFSET_CARDBUS_CIS_PTR		0x28
#define PCI_CFG_OFFSET_SUBSYSTEM_VENDOR_ID	0x2A
#define PCI_CFG_OFFSET_SUBSYSTEM_ID			0x2C
#define PCI_CFG_OFFSET_EXPANSION_ROM_BASE	0x30
#define PCI_CFG_OFFSET_CAPABILITIES_PTR 	0x34
#define PCI_CFG_OFFSET_INTERRUPT_LINE		0x39
#define PCI_CFG_OFFSET_INTERRUPT_PIN		0x3A
#define PCI_CFG_OFFSET_MINIMUM_GRANT		0x3B
#define PCI_CFG_OFFSET_MAXIMUM_LATENCY		0x3C

/* TODO: AGP SHIT! */

/* PCI Structures & Enums */
typedef enum 
{
	// Int 0x1A,AX=0xB1xx   = PCI
	PCI_FUNCTION_ID_BASE = 0xB1,

	// PCI BIOS SUBFUNCTIONS
	PCI_BIOS_IS_PRESENT = 0x01,
	PCI_FIND_DEVICE = 0x02,
	PCI_FIND_CLASS_CODE = 0x03,
	PCI_GENERATE_SPECIAL_CYCLE = 0x06,
	PCI_READ_CONFIG_BYTE = 0x08,
	PCI_READ_CONFIG_WORD = 0x09,
	PCI_READ_CONFIG_DWORD = 0x0A,
	PCI_WRITE_CONFIG_BYTE = 0x0B,
	PCI_WRITE_CONFIG_WORD = 0x0C,
	PCI_WRITE_CONFIG_DWORD = 0x0D,
	PCI_GET_IRQ_ROUTING_OPTIONS = 0x0E,
	PCI_SET_PCI_IRQ = 0x0F,

	CFLAG_CARRY = 0x01,
} pci_functions_t;

typedef enum 
{
	PCI_ERROR_UNSUPPORTED_FUNCTION = 0x81,
	PCI_ERROR_BAD_VENDOR_ID = 0x83,
	PCI_ERROR_DEVICE_NOT_FOUND = 0x86,
	PCI_ERROR_BAD_PCI_REGISTER = 0x87,
} pci_errors_t; 

/* PCI Functions */
bool pci_bios_is_present(void);
bool pci_does_device_exist(uint32_t device_id, uint32_t vendor_id);

uint8_t pci_read_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint16_t pci_read_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t pci_read_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset);

bool pci_write_config_8(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint8_t value);
bool pci_write_config_16(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint16_t value);
bool pci_write_config_32(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint32_t value);

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
#define PCI_DEVICE_NV10				0x0100		// GeForce 256 (SDRAM)		1999
#define PCI_DEVICE_NV10_DDR			0x0101		// GeForce 256 (DDR1)		1999
#define PCI_DEVICE_NV10_QUADRO		0x0103		// Quadro					2000 (aka NV10GL)

/* 
    NV_PFB_BOOT values 
    There's two ways to identify NV GPU: By reading PCI config registers and by reading the NV_PFB_BOOT register
    We read PCI config registers to determine the overall model and hten read the pfb_boot register to get the stepping
*/
#define NV_PMC_BOOT_NV1_A01     	0x00010100      // NV1 Stepping A0 (Prototype) 				1994
#define NV_PMC_BOOT_NV1_B01     	0x00010101      // NV1 Stepping B0 (Prototype) 				1995
#define NV_PMC_BOOT_NV1_B02     	0x00010102      // NV1 Stepping B1 (Prototype) 				1995
#define NV_PMC_BOOT_NV1_B03     	0x00010103      // NV1 Stepping B2 (Prototype)				1995
#define NV_PMC_BOOT_NV1_C01     	0x00010104      // NV1 Stepping C0 (Final)	   				1995
#define NV_PMC_BOOT_NV2_A01			0x10020400		// NV2 Stepping A0 (Prototype) 				1995/1996 (Helios Semiconductor) 
// Default value for the boot information register.
// Depends on the chip
#define NV_PMC_BOOT_NV3_A00         0x00030100      // NV3 Stepping A0 (Prototype) 				April 1997
#define NV_PMC_BOOT_NV3_B00         0x00030110		// NV3 Stepping B0 (Final)					July(?) 1997
#define NV_PMC_BOOT_NV3T_A01_ST     0x00030120		// NV3 Stepping C0 / NV3T Stepping A1		Early 1998		(STMicro-manufactured)
#define NV_PMC_BOOT_NV3T_A02_ST     0x00030121		// NV3 Stepping C1 / NV3T Stepping A2		Early 1998		(STMicro-manufactured)
#define NV_PMC_BOOT_NV3T_A03_ST 	0x00030122		// NV3 Stepping C2/C3 / NV3 Stepping A3/A4	1998/9			(STMicro-manufactured)
#define NV_PMC_BOOT_NV3T_A01_TSMC   0x20030120		// NV3 Stepping C0 / NV3T Stepping A1		Early 1998 		(TSMC-manufactured)
#define NV_PMC_BOOT_NV3T_A02_TSMC   0x20030121		// NV3 Stepping C1 / NV3T Stepping A2		Early 1998 		(TSMC-manufactured)
#define NV_PMC_BOOT_NV3T_A03_TSMC 	0x20030122		// NV3 Stepping C2/C3 / NV3 Stepping A3/A4	1998/9			(TSMC-manufactured)


/* NVidia Device Definition */
typedef struct nv_device_info_s
{
	uint32_t device_id; 		// Device ID of the GPU
	uint32_t vendor_id;			// Vendor ID of the GPU
	const char* name; 			// Friendly name of the GPU
	bool (*init_function)();	// Function to call on entry point

	void (*main_function)();	// Function to call on entry point
	
	void (*shutdown_function)();	// Function to call on shutdown
	void (*submit_function)();	// Function to call for graphics object submission
} nv_device_info_t; 

/* List of supported devices */
extern nv_device_info_t supported_devices[]; 

/* Full NV Device Struct (shared across all devices) */
typedef struct nv_device_s
{
	nv_device_info_t device_info;
	uint32_t bus_number;			// PCI bus number
	uint32_t function_number; 		// PCI function number
	void* bar0;						// PCI BAR0 mapping - gpu stuff
	void* bar1;						// PCI BAR1 mapping for DFB
	int32_t bar0_selector;			// MUST BE USED FOR ACCESS TO BAR0
	int32_t bar1_selector;			// MUST BE USED FOR ACCESS TO BAR1
	uint32_t bar1_dfb_start;		// DFB start address
	uint32_t bar1_ramin_start; 		// RAMIN start address

	uint32_t vram_amount;			// Amount of Video RAM

	/* Some registers shared between all gpus */
	uint32_t nv_pfb_boot_0;			// nv_pfb_boot_0 register read at boot
	uint32_t nv_pmc_boot_0;			// nv_pmc_boot_0 register read at boot
	uint32_t straps;				// Straps for oem-specific config
} nv_device_t;

extern nv_device_t current_device;

// Detection functions
bool nv_detect(); 

// Read/write functions
// only 8 and 32 bit are really needed
uint8_t nv_mmio_read8(uint32_t offset); 
uint32_t nv_mmio_read32(uint32_t offset); 
/* Requires some special dispensations if the bus size is 64-bit and there is only 2 MB of VRAM */
uint8_t nv_dfb_read8(uint32_t offset); 
uint16_t nv_dfb_read16(uint32_t offset); 
uint32_t nv_dfb_read32(uint32_t offset); 
// RAMIN is always read as 32bit
uint32_t nv_ramin_read32(uint32_t offset); 

void nv_mmio_write8(uint32_t offset, uint8_t val);
void nv_mmio_write32(uint32_t offset, uint32_t val);
void nv_dfb_write8(uint32_t offset, uint8_t val);
void nv_dfb_write16(uint32_t offset, uint16_t val);
void nv_dfb_write32(uint32_t offset, uint32_t val);
void nv_ramin_write32(uint32_t offset, uint32_t val);

/* Temp stuff */
#define NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS        60
#define NV3_TEST_OVERCLOCK_BASE_13500                   0x1A30B
#define NV3_TEST_OVERCLOCK_BASE_14318					0x1C40E