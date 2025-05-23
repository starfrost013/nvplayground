/* */
#pragma once
#include <stdbool.h>
#include <stdint.h>


/* Mode entry */
typedef struct nv3_mode_entry_s
{
    uint32_t res_horiz;
    uint32_t res_vert;
    uint32_t bpp;
    uint32_t refresh_rate; 
} nv3_mode_entry_t;

/* List of valid tables */
extern nv3_mode_entry_t mode_table[];

/* Current mode setup */
typedef struct nv3_mode_s
{
    /* Resolution Information */
    uint32_t res_horiz;
    uint32_t res_vert;
    uint32_t bpp;
    uint32_t refresh_rate; 

    /* CRTC Parameters */
    uint32_t hdisp;
    uint32_t hstart;
    uint32_t hend;
    uint32_t htotal;
    uint32_t hblankstart;
    uint32_t hblankend;
    uint32_t vdisp;
    uint32_t vstart;
    uint32_t vend;
    uint32_t vtotal; 
    uint32_t vblankstart;
    uint32_t vblankend; 
    /* NV Drivers only use this, not XFree86 */
    uint32_t hsync_polarity;
    uint32_t vsync_polarity;

    /* Util */
    bool stale;                 // Determines if the graphics hardware needs to be updated with the current state of the mode
} nv3_mode_t; 

typedef struct nv3_state_s
{
     nv3_mode_t mode;
} nv3_state_t;