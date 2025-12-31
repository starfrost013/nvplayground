/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_io.c: Implements I/O functions that are shared across all graphics hardware.
*/

#include <nvplay.h>
#include <architecture/nv1/nv1.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv4/nv4.h>
#include "architecture/nv3/nv3_ref.h"
#include "architecture/nv4/nv4_ref.h"
#include "pc.h"
#include "sys/farptr.h"
#include "util/util.h"
#include <stdint.h>
#include <time.h>

//
// MMIO Functions
//

/* Read 8-bit value from the MMIO */
uint8_t NV_ReadMMIO8(uint32_t offset)
{
    return _farpeekb(current_device.bar0_selector, offset);
}

/* Read 32-bit value from the MMIO */
uint32_t NV_ReadMMIO32(uint32_t offset)
{
    return _farpeekl(current_device.bar0_selector, offset);
}

void NV_WriteMMIO8(uint32_t offset, uint8_t val)
{
    _farpokeb(current_device.bar0_selector, offset, val);
}

void NV_WriteMMIO32(uint32_t offset, uint32_t val)
{
    _farpokel(current_device.bar0_selector, offset, val);
}

//
// DFB Functions
//

/* Read 8-bit value from the DFB */
uint8_t NV_ReadDfb8(uint32_t offset)
{
    return _farpeekb(current_device.bar1_selector, offset);
}

/* Read 16-bit value from the DFB */
uint16_t NV_ReadDfb16(uint32_t offset)
{
    return _farpeekw(current_device.bar1_selector, offset);
}

/* Read 32-bit value from the DFB */
uint32_t NV_ReadDfb32(uint32_t offset)
{
    return _farpeekl(current_device.bar1_selector, offset);
}

/* Write 8-bit value to the DFB */
void NV_WriteDfb8(uint32_t offset, uint8_t val)
{
    _farpokeb(current_device.bar1_selector, offset, val);
}

/* Write 16-bit value to the DFB */
void NV_WriteDfb16(uint32_t offset, uint16_t val)
{
    _farpokew(current_device.bar1_selector, offset, val);
}

void NV_WriteDfb32(uint32_t offset, uint32_t val)
{
    _farpokel(current_device.bar1_selector, offset, val);
}

/* Read 32-bit value from RAMIN */
uint32_t NV_ReadRamin32(uint32_t offset)
{
    // I considered having this be a GPU-specific function, but RAMIN mapping did not change much after NV4 until NV40
    // and direct RAMIN writes are fairly rare.
    //
    // If it turns out to be too slow we can change it

    switch (current_device.real_device_id) // temp
    {
        // RAMIN not usable on NV1 with CONFIG=2 due to hardware errata, see envytools
        case PCI_DEVICE_NV1_NV:
            return _farpeekl(current_device.bar0_selector, NV1_RAMIN_START + offset);
        case PCI_DEVICE_NV3:
        case PCI_DEVICE_NV3T_ACPI:
            return _farpeekl(current_device.bar1_selector, NV3_RAMIN_START + offset);
        // WARNING! WARNING! WARNING!
        // 
        // NV4 MMIO dumps show *PROM* (VBIOS mirror) at 0x700000 unlike 0x300000 as indicated by NV drivers. 
        // RAMIN RAMFC RAMHT RAMRO structures always start at 0x10000 so NVIDIA never had to deal with this issue (for all practical purposes RAMIN starts at 710000)
        // Therefore, it may not be possible, due to hardware errata, to write to RAMIN address below 0x10000!
        case PCI_DEVICE_NV4 ... PCI_DEVICE_NV1F: // all NV1xsa re the same
            return _farpeekl(current_device.bar0_selector, NV4_RAMIN_START + offset);
    }

    Logging_Write(log_level_error, "NV_ReadRamin32: Somehow reached here with an unsupported gpu\n");
    return 0x00;
}

void NV_WriteRamin32(uint32_t offset, uint32_t val)
{
    // I considered having this be a GPU-specific function, but RAMIN mapping did not change much after NV4 until NV40
    // and direct RAMIN writes are fairly rare.
    //
    // If it turns out to be too slow we can change it

    switch (current_device.real_device_id)
    {
        // RAMIN not usable on NV1 with CONFIG=2 due to hardware errata, see envytools
        case PCI_DEVICE_NV1_NV:
            _farpokel(current_device.bar0_selector, NV1_RAMIN_START + offset, val);
            break;
        case PCI_DEVICE_NV3:
        case PCI_DEVICE_NV3T_ACPI:
            _farpokel(current_device.bar1_selector, NV3_RAMIN_START + offset, val);
             break;
        // See "WARNING" above for NV4 RAMIN writes!
        case PCI_DEVICE_NV4:
        case PCI_DEVICE_NV10 ... PCI_DEVICE_NV18_END:       // NV1x uses 0x700000 BAR0 start too
            _farpokel(current_device.bar0_selector, NV4_RAMIN_START + offset, val);    
             break;
        default:
            Logging_Write(log_level_error, "NV_WriteRamin32: Somehow reached here with an unsupported GPU\n");
            break;
    }

}

/* Accelerated nVIDIA VGA functions */

void NV_CRTCLockExtendedRegisters()
{
    if (GPU_IsNV1())
        return; 

    if (GPU_IsNV3())
    {
        // To do: Does this need to be moved to NV3
        NV_WriteMMIO32(NV3_PRMVIO_SR_INDEX, NV3_PRMVIO_SR_INDEX_LOCK);
        NV_WriteMMIO32(NV3_PRMVIO_SR, NV3_PRMVIO_SR_INDEX_LOCK_LOCKED);
    }
    else 
    {
        NV_WriteMMIO32(NV4_PRMCIO_CRX_COLOR, NV4_CIO_SR_LOCK_INDEX);
        NV_WriteMMIO32(NV4_PRMCIO_CR_COLOR, NV4_CIO_SR_LOCK_VALUE);
    }

}

void NV_CRTCUnlockExtendedRegisters()
{
    if (GPU_IsNV1())
        return; 

    if (GPU_IsNV3())
    {
        NV_WriteMMIO32(NV3_PRMVIO_SR_INDEX, NV3_PRMVIO_SR_INDEX_LOCK);
        NV_WriteMMIO32(NV3_PRMVIO_SR, NV3_PRMVIO_SR_INDEX_LOCK_LOCKED);
    }
    else 
    {
        NV_WriteMMIO32(NV4_PRMCIO_CRX_COLOR, NV4_CIO_SR_LOCK_INDEX);
        NV_WriteMMIO32(NV4_PRMCIO_CR_COLOR, NV4_CIO_SR_UNLOCK_RW_VALUE);
    }
}

uint8_t NV_ReadCRTC(uint8_t index)
{
    NV_WriteMMIO32(NV3_PRMCIO_CRTC_REGISTER_INDEX_COLOR, index);
    return NV_ReadMMIO32(NV3_PRMCIO_CRTC_REGISTER_COLOR);
}

// TODO
uint8_t NV_ReadGDC(uint8_t index)
{
    NV_WriteMMIO32(NV3_PRMVIO_GR_INDEX, index);
    return NV_ReadMMIO32(NV3_PRMVIO_GR);
}

// TODO
uint8_t NV_ReadSequencer(uint8_t index)
{
    NV_CRTCUnlockExtendedRegisters();
    NV_WriteMMIO32(NV3_PRMVIO_SR_INDEX, index);
    NV_CRTCLockExtendedRegisters();
    
    return NV_ReadMMIO32(NV3_PRMVIO_SR);
}

void NV_WriteCRTC(uint8_t index, uint8_t value)
{
    NV_WriteMMIO32(NV3_PRMCIO_CRTC_REGISTER_INDEX_COLOR, index);
    NV_WriteMMIO32(NV3_PRMCIO_CRTC_REGISTER_COLOR,value);
}

void NV_WriteGDC(uint8_t index, uint8_t value)
{
    NV_WriteMMIO32(NV3_PRMVIO_GR_INDEX, index);
    NV_WriteMMIO32(NV3_PRMVIO_GR,value);
}

void NV_WriteSequencer(uint8_t index, uint8_t value)
{
    NV_CRTCUnlockExtendedRegisters();
    NV_WriteMMIO32(NV3_PRMVIO_SR_INDEX, index);
    NV_WriteMMIO32(NV3_PRMVIO_SR,value);
    NV_CRTCLockExtendedRegisters();
}

//
// Universal VGA functions
//

uint8_t VGA_ReadCRTC(uint8_t index)
{
    uint8_t miscout = inportb(VGA_PORT_MISCOUT);

    if (!(miscout & 1))
        return inportb(VGA_PORT_MONO_CRTC_INDEX);
    else
        return inportb(VGA_PORT_COLOR_CRTC_INDEX);
}

uint8_t VGA_ReadGDC(uint8_t index)
{
    outportb(VGA_PORT_GRAPHICS_INDEX, index);

    return inportb(VGA_PORT_GRAPHICS);
}

// Read a byte from the VGA sequencer register with index index.
uint8_t VGA_ReadSequencer(uint8_t index)
{
    outportb(VGA_PORT_SEQUENCER_INDEX, index);

    return inportb(VGA_PORT_SEQUENCER);
}

// Read a VGA attribute register.
uint8_t VGA_ReadAttribute(uint8_t index)
{
    // figure out if this is colour or mono
    uint8_t miscout = inportb(VGA_PORT_MISCOUT);

    // do a useless read to reset the attribute register flip-flop

    if (!(miscout & 1))
        inportb(VGA_PORT_INPUT0_MONO);
    else
        inportb(VGA_PORT_INPUT0_COLOR);

    // write to 3c0. writing to the data is 3c1, but reading is 3c0. what.
    outportb(VGA_PORT_ATTRIBUTE_REGISTER, index);
    return inportb(VGA_PORT_ATTRIBUTE_DATA_WRITE);
}

void VGA_WriteCRTC(uint8_t index, uint8_t value)
{
    uint8_t miscout = inportb(VGA_PORT_MISCOUT);

    if (!(miscout & 1))
    {
        outportb(VGA_PORT_MONO_CRTC_INDEX ,index);
        outportb(VGA_PORT_MONO_CRTC, value);
    }
    else
    {
        outportb(VGA_PORT_COLOR_CRTC_INDEX, value);
        outportb(VGA_PORT_COLOR_CRTC, value);
    }
}

void VGA_WriteGDC(uint8_t index, uint8_t value)
{
    outportb(VGA_PORT_GRAPHICS_INDEX, index);
    outportb(VGA_PORT_GRAPHICS, value);
}

void VGA_WriteSequencer(uint8_t index, uint8_t value)
{
    outportb(VGA_PORT_SEQUENCER_INDEX, index);
    outportb(VGA_PORT_SEQUENCER, value);
}

void VGA_WriteAttribute(uint8_t index, uint8_t value)
{
    // figure out if this is colour or mono
    uint8_t miscout = inportb(VGA_PORT_MISCOUT);

    // do a useless read to reset the attribute register flip-flop

    if (!(miscout & 1))
        inportb(VGA_PORT_INPUT0_MONO);
    else
        inportb(VGA_PORT_INPUT0_COLOR);

    // write to 3c0
    outportb(VGA_PORT_ATTRIBUTE_REGISTER, index);
    outportb(VGA_PORT_ATTRIBUTE_REGISTER, value);


    // figure out what is being written next
}


//not speed critical, use a double for precision
// NV3/NV4. Not sure about NV1
double NV_ClockMNPToMhz(uint32_t clock_base, uint32_t mnp)
{
    // clock_base = 13500000 or 14318180

    uint32_t clock_p, clock_n, clock_m;

    clock_p = (clock_base >> 16) & 0xFF;
    clock_n = (clock_base >> 8) & 0xFF;
    clock_m = (clock_base & 0xFF); 

    return ((double)clock_base * clock_n) / (double)(clock_m << clock_p) / 1000000.0f;
}

// TODO: nv_clock_mhz_to_mnp