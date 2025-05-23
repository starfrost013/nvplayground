#include "nvcore.h"
#include "sys/farptr.h"

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
    Logging_Write(log_level_warning, "nv_ramin_read32 NOT YET IMPLEMENTED");
    return 0x00;
}

void nv_ramin_write32(uint32_t offset, uint32_t val)
{
     Logging_Write(log_level_warning, "nv_ramin_write32 NOT YET IMPLEMENTED");
    return;
}

