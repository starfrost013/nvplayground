/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    pci.h: PCI headers
*/

#pragma once
#include <nvplay.h>

// Unaccelerated video modes
#define UNACCEL_VIDEO_TEXT_40_MONO			0x00		// 40x25 text 16 shades	of grey	[CGA+]
#define UNACCEL_VIDEO_TEXT_40_COLOR			0x01		// 40x25 text 16 colours		[CGA+]
#define UNACCEL_VIDEO_TEXT_80_MONO			0x02		// 80x25 text 16 shades	of grey	[CGA+]
#define UNACCEL_VIDEO_TEXT_80_COLOR			0x03		// 80x25 text 16 colours		[CGA+]
#define UNACCEL_VIDEO_320X200_2BPP			0x04		// 320x200 4 colours			[CGA+]
#define UNACCEL_VIDEO_320X200_2BPP_MONO		0x05		// 320x200 4 shades of grey		[CGA+]
#define UNACCEL_VIDEO_640X200_1BPP			0x06		// 320x200 2 colour mono		[CGA+]
#define UNACCEL_VIDEO_TEXT_MDA				0x07		// "Mono" 80x25 text			[MDA+, 16 colours unused]
#define UNACCEL_VIDEO_160X200_4BPP			0x08		// 160x200 16 colours 			[PCjr+]
#define UNACCEL_VIDEO_320X200_4BPP			0x09		// 320x200 16 colours			[PCjr+]
#define UNACCEL_VIDEO_640X200_2BPP			0x0A		// 320x200 4 colours			[PCjr+]
// 0B, 0C reserved (EGA Internal)
#define UNACCEL_VIDEO_320X200_4BPP_EGA		0x0D		// 320x200 16 colours			[EGA+]
#define UNACCEL_VIDEO_640X200_4BPP			0x0E		// 640x200 16 colours			[EGA+]
#define UNACCEL_VIDEO_640X350_1BPP			0x0F		// 640x350 2 colour mono		[VGA+]
#define UNACCEL_VIDEO_640X350_4BPP			0x10		// 640x350 16 colours			[EGA+, 4 colours if 64KB VRAM]
#define UNACCEL_VIDEO_640X480_1BPP			0x11		// 640x480 2 colour mono		[VGA+]
#define UNACCEL_VIDEO_640X480_4BPP			0x12		// 640x480 16 colours			[VGA+]
#define UNACCEL_VIDEO_320X200_8BPP			0x13		// 320x200 256 colours			[VGA+]

// VESA (VBE1.2) goes here...VBE2/3 require querying Video BIOS

/* PCI */

/* PCI BIOS magic */
#define PCI_BIOS_MAGIC						0x20494350

/* PCI Type-0 header. We don't care about PCI to PCI bridges (type 1) or PCI to CardBus bridges (type 2) */
#define PCI_CFG_OFFSET_VENDOR_ID			0x00
#define PCI_CFG_OFFSET_DEVICE_ID			0x02
#define PCI_CFG_OFFSET_COMMAND				0x04

#define PCI_CFG_OFFSET_COMMAND_IO_ENABLED	0x01
#define PCI_CFG_OFFSET_COMMAND_MEM_ENABLED	0x02
#define PCI_CFG_OFFSET_COMMAND_BUS_MASTER	0x04	// Allow bus mastering

#define PCI_CFG_OFFSET_STATUS				0x06
#define PCI_CFG_OFFSET_REVISION				0x08
#define PCI_CFG_OFFSET_CLASS_CODE			0x09	
#define PCI_CFG_OFFSET_CLASS_CODE_HIGH 		0x09
#define PCI_CFG_OFFSET_CLASS_CODE_LOW		0x0A
#define PCI_CFG_OFFSET_CACHE_LINE_SIZE		0x0C
#define PCI_CFG_OFFSET_LATENCY_TIMER		0x0D
#define PCI_CFG_OFFSET_HEADER_TYPE			0x0E
#define PCI_CFG_OFFSET_BIST					0x0F

/* BARs. Only 0/1 matter for supported NV cards and 0/1/2 for NV20+ but for PCI dumping code we dump everything 

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
	PCI_BIOS_PRESENT = 0x01,
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
bool PCI_BiosIsPresent(void);		// Try and find a PCI 2.1 BIOS
bool PCI_DevicePresent(uint32_t device_id, uint32_t vendor_id);

uint8_t PCI_ReadConfig8(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint16_t PCI_ReadConfig16(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t PCI_ReadConfig32(uint32_t bus_number, uint32_t function_number, uint32_t offset);

bool PCI_WriteConfig8(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint8_t value);
bool PCI_WriteConfig16(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint16_t value);
bool PCI_WriteConfig32(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint32_t value);