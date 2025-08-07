/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_io.c: Implements I/O functions that are shared across all pieces of graphics hardware.
*/

#include <nvplayground.h>
#include "architecture/nv3/nv3_ref.h"
#include "pc.h"
#include "sys/farptr.h"
#include <stdint.h>

//
// MMIO Functinos
//

/* Read 8-bit value from the MMIO */
uint8_t nv_mmio_read8(uint32_t offset)
{
    return _farpeekb(current_device.bar0_selector, offset);
}

/* Read 32-bit value from the MMIO */
uint32_t nv_mmio_read32(uint32_t offset)
{
    return _farpeekl(current_device.bar0_selector, offset);
}

void nv_mmio_write8(uint32_t offset, uint8_t val)
{
    _farpokeb(current_device.bar0_selector, offset, val);
}

void nv_mmio_write32(uint32_t offset, uint32_t val)
{
    _farpokel(current_device.bar0_selector, offset, val);
}

//
// DFB Functions
//

/* Read 8-bit value from the DFB */
uint8_t nv_dfb_read8(uint32_t offset)
{
    return _farpeekb(current_device.bar1_selector, offset);
}

/* Read 16-bit value from the DFB */
uint16_t nv_dfb_read16(uint32_t offset)
{
    return _farpeekw(current_device.bar1_selector, offset);
}

/* Read 32-bit value from the DFB */
uint32_t nv_dfb_read32(uint32_t offset)
{
    return _farpeekl(current_device.bar1_selector, offset);
}

/* Write 8-bit value to the DFB */
void nv_dfb_write8(uint32_t offset, uint8_t val)
{
    _farpokeb(current_device.bar1_selector, offset, val);
}

/* Write 16-bit value to the DFB */
void nv_dfb_write16(uint32_t offset, uint16_t val)
{
    _farpokew(current_device.bar1_selector, offset, val);
}

void nv_dfb_write32(uint32_t offset, uint32_t val)
{
    _farpokel(current_device.bar1_selector, offset, val);
}

/* Read 32-bit value from RAMIN */
uint32_t nv_ramin_read32(uint32_t offset)
{
    return _farpeekl(current_device.bar1_selector, NV3_RAMIN_START + offset);
}

void nv_ramin_write32(uint32_t offset, uint32_t val)
{
    _farpokel(current_device.bar1_selector, NV3_RAMIN_START + offset, val);
}

/* Accelerated nVIDIA VGA functions */

void nv_crtc_lock_extended_registers()
{
    // To do: Does this need to be moved to NV3
    nv_mmio_write32(NV3_PRMVIO_SR_INDEX, NV3_PRMVIO_SR_INDEX_LOCK);
    nv_mmio_write32(NV3_PRMVIO_SR, NV3_PRMVIO_SR_INDEX_LOCK_LOCKED);

}

void nv_crtc_unlock_extended_registers()
{
    nv_mmio_write32(NV3_PRMVIO_SR_INDEX, NV3_PRMVIO_SR_INDEX_LOCK);
    nv_mmio_write32(NV3_PRMVIO_SR, NV3_PRMVIO_SR_INDEX_LOCK_UNLOCKED);
}

uint8_t nv_crtc_read(uint8_t index)
{
    nv_mmio_write32(NV3_PRMCIO_CRTC_REGISTER_INDEX_COLOR, index);
    return nv_mmio_read32(NV3_PRMCIO_CRTC_REGISTER_COLOR);
}

uint8_t nv_gdc_read(uint8_t index)
{

}

uint8_t nv_sequencer_read(uint8_t index)
{

}

void nv_crtc_write(uint8_t index, uint8_t value)
{
    nv_mmio_write32(NV3_PRMCIO_CRTC_REGISTER_INDEX_COLOR, index);
    nv_mmio_write32(NV3_PRMCIO_CRTC_REGISTER_COLOR,value);
}



void nv_gdc_write(uint8_t index, uint8_t value)
{

}

void nv_sequencer_write(uint8_t index, uint8_t value)
{

}

//
// Universal VGA functions
//

uint8_t vga_crtc_read(uint8_t index)
{
    uint8_t miscout = inportb(VGA_PORT_MISCOUT);

    if (!(miscout & 1))
        return inportb(VGA_PORT_MONO_CRTC_INDEX);
    else
        return inportb(VGA_PORT_COLOR_CRTC_INDEX);
}

uint8_t vga_gdc_read(uint8_t index)
{
    outportb(VGA_PORT_GRAPHICS_INDEX, index);

    return inportb(VGA_PORT_GRAPHICS);
}

// Read a byte from the VGA sequencer register with index index.
uint8_t vga_sequencer_read(uint8_t index)
{
    outportb(VGA_PORT_SEQUENCER_INDEX, index);

    return inportb(VGA_PORT_SEQUENCER);
}

// Read a VGA attribute register.
uint8_t vga_attribute_read(uint8_t index)
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

void vga_crtc_write(uint8_t index, uint8_t value)
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

void vga_gdc_write(uint8_t index, uint8_t value)
{
    outportb(VGA_PORT_GRAPHICS_INDEX, index);
    outportb(VGA_PORT_GRAPHICS, value);
}

void vga_sequencer_write(uint8_t index, uint8_t value)
{
    outportb(VGA_PORT_SEQUENCER_INDEX, index);
    outportb(VGA_PORT_SEQUENCER, value);
}

void vga_attribute_write(uint8_t index, uint8_t value)
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