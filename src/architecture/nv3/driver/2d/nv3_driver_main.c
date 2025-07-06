//
// nv3_driver_main.c: Core NV3 driver functions - initialisation, modeset, shutdown
//

#include "architecture/nv3/nv3_ref.h"
#include "core/nvcore.h"
#include "time.h"
#include "util/util.h"
#include <architecture/nv3/driver/2d/nv3_driver.h>

//
// Globals
//
nv3_driver_t nv3_driver = {0};

void nv3driver_main()
{
    Logging_Write(log_level_message, "NVIDIA NV3 Open-Source Graphics Driver %s\n", APP_SIGNON_STRING);

    Logging_Write(log_level_message, "Initialising graphics hardware...\n");
    nv3driver_enter_graphics_mode();
}

/* 
    Determines if the driver has a valid mode 
    Currently it just checks that the resolution and refresh rate are valid but it should probably also check the CRTC settings are good
*/
bool nv3driver_is_mode_valid()
{
    uint32_t mode_table_entry = 0;
    nv3_mode_entry_t* mode_entry = &mode_table[mode_table_entry];

    while (mode_entry)
    {
        mode_entry = &mode_table[mode_table_entry];

        // set the mode entry ptr of the nv3driver mode and return true if a valid mode was found
        if (nv3_driver.mode.bpp == mode_entry->bpp
        && nv3_driver.mode.refresh_rate == mode_entry->refresh_rate
        && nv3_driver.mode.res_horiz == mode_entry->res_horiz
        && nv3_driver.mode.res_vert == mode_entry->res_vert)
        {
            nv3_driver.mode.mode_entry_ptr = mode_entry;
            return true;
        }

        mode_table_entry++;
    }

    return false; 
}

bool nv3driver_set_mode()
{
    Logging_Write(log_level_message, "Entering %dx%d %dbpp %dhz\n", nv3_driver.mode.res_horiz, nv3_driver.mode.res_vert, nv3_driver.mode.bpp, nv3_driver.mode.refresh_rate);

    //todo allow overwrite
    
    Logging_Write(log_level_message, "Entering into graphics mode in 5 seconds. This procedure is not yet thoroughly tested.\n\nIF YOU SEE AN IRREGULAR VIDEO SIGNAL IN ANY WAY, POWER DOWN IMMEDIATELY.\n");

    uclock_t start_clock = uclock();

    //. wait 5 seconds
    while (!((uclock() - start_clock) > (5 * UCLOCKS_PER_SEC)));

    // check for a valid mode
    if (!nv3driver_is_mode_valid())
        return false; 

    // let's go

    // enable locked crtc
    nv_crtc_unlock_extended_registers();

    // write CRTC information, starting at reg 0x01 like linux drivers and nv drivers
    nv_crtc_write(NV3_CRTC_REGISTER_HTOTAL, nv3_driver.mode.crtc.htotal);
    nv_crtc_write(NV3_CRTC_REGISTER_HDISPEND, nv3_driver.mode.crtc.hdisp);
    nv_crtc_write(NV3_CRTC_REGISTER_HBLANKSTART, nv3_driver.mode.crtc.hblankstart);
    nv_crtc_write(NV3_CRTC_REGISTER_HBLANKEND, nv3_driver.mode.crtc.hblankend);

    // Bit 7 = HBE5
    uint8_t hretraceend_value = (((nv3_driver.mode.crtc.hblankend >> 5) & 0x01) << 7) | (nv3_driver.mode.crtc.hretraceend & 0x1F);

    nv_crtc_write(NV3_CRTC_REGISTER_HRETRACESTART, hretraceend_value);
    nv_crtc_write(NV3_CRTC_REGISTER_HRETRACEEND, nv3_driver.mode.crtc.hretraceend);

    /* Would a #define help here?*/
    uint8_t overflow_value = 0x00;

    overflow_value |= (((nv3_driver.mode.crtc.vretracestart >> 9) & 0x01) << 7);    //bit7 Vretrace bit 9
    overflow_value |= (((nv3_driver.mode.crtc.vdisp >> 9) & 0x01) << 6);            //bit6 Vdisp end bit 9
    overflow_value |= (((nv3_driver.mode.crtc.vtotal >> 9) & 0x01) << 5);           //bit5 Vtotal bit 9
    overflow_value |= (((nv3_driver.mode.crtc.vblankstart >> 8) & 0x01) << 3);      //bit3 Vblank start bit 8
    overflow_value |= (((nv3_driver.mode.crtc.vretracestart >> 8) & 0x01) << 2);    //bit2 Vretrace start bit 8
    overflow_value |= (((nv3_driver.mode.crtc.vdisp >> 8) & 0x01) << 1);            //bit1 Vdisp end bit 8
    overflow_value |= ((nv3_driver.mode.crtc.vtotal >> 8) & 0x01);                  //bit0 Vtotal bit 8

    nv_crtc_write(NV3_CRTC_REGISTER_OVERFLOW, overflow_value);

    // Linux drivers don't set 0x8 (NV3_CRTC_REGISTER_PRESETROWSCAN)

    //todo: figure out (below 640x480? don't require linedouble)
    nv_crtc_write(NV3_CRTC_REGISTER_MAXSCAN, (((nv3_driver.mode.crtc.vblankstart >> 9) & 0x01) << 5) | (1 << NV3_CRTC_REGISTER_MAXSCAN_LINEDOUBLE));
    nv_crtc_write(NV3_CRTC_REGISTER_CURSOR_START, nv3_driver.mode.crtc.cursor_start);
    nv_crtc_write(NV3_CRTC_REGISTER_CURSOR_END, nv3_driver.mode.crtc.cursor_end);

    // start addr not needed?
    nv_crtc_write(NV3_CRTC_REGISTER_VRETRACESTART, nv3_driver.mode.crtc.vretracestart);
    nv_crtc_write(NV3_CRTC_REGISTER_VRETRACEEND , nv3_driver.mode.crtc.vretraceend);
    
    nv_crtc_write(NV3_CRTC_REGISTER_VTOTAL, nv3_driver.mode.crtc.vtotal);
    nv_crtc_write(NV3_CRTC_REGISTER_VDISPEND, nv3_driver.mode.crtc.vdisp);
    nv_crtc_write(NV3_CRTC_REGISTER_STARTVBLANK, nv3_driver.mode.crtc.vblankstart);
    nv_crtc_write(NV3_CRTC_REGISTER_ENDVBLANK, nv3_driver.mode.crtc.vblankend);

    //large screen bit
    if (nv3_driver.mode.res_horiz >= 1280)
        nv_crtc_write(NV3_CRTC_REGISTER_REPAINT_1, (1 << NV3_CRTC_REGISTER_REPAINT_1_LARGE_SCREEN));
    else
        nv_crtc_write(NV3_CRTC_REGISTER_REPAINT_1, 0); // has no other purpose


    nv_crtc_lock_extended_registers();
    
    // enter into mode
    Logging_Write(log_level_debug, "Can you hear me now?\n");

}

/* 
    NV3: Try to get into a graphics mode.

    We do this via the following ways:
    1. Enable cache reassignment
    2. Enable PFIFO caches
    3. Determine if a valid mode was set
    4. Setup CRTC registers according to mode information
    5. Initialise RAMIN: RAMRO, RAMHT, RAMFC, etc.
    6. Setup 2d driver gfx initflags and test function
    7. Insert objects for test functionality into RAMIN + RAMHT
    8. Run NV2D driver by executing object methods
*/
bool nv3driver_enter_graphics_mode()
{
    Logging_Write(log_level_debug, "Enabling cache reassignment...");

    // Ensure cache reassignment is enabled
    // We don't do context switching yet but we still enable it since it seems 3D stuff requires it
    nv_mmio_write32(NV3_PFIFO_CACHE_REASSIGNMENT, 1);

    Logging_Write(log_level_debug, "Enabling PFIFO caches...");

    // Enable PFIFO caches
    nv_mmio_write32(NV3_PFIFO_CACHE0_PUSH_ENABLED, 1);
    nv_mmio_write32(NV3_PFIFO_CACHE1_PUSH_ENABLED, 1);

    nv_mmio_write32(NV3_PFIFO_CACHE0_PULL0_ENABLED, 1);
    nv_mmio_write32(NV3_PFIFO_CACHE1_PULL0_ENABLED, 1);

    // Try and set the mode
    if (!nv3driver_set_mode())
    {
        // immediately reset the graphics hardware before exiting
    }

    return true; 
}

bool nv3driver_leave_graphics_mode()
{

}
