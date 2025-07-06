#pragma once

// Includes
#include <nvplayground.h>
#include <architecture/nv3/nv3.h>
#include <architecture/nv3/nv3_api.h>
#include <architecture/nv3/nv3_ref.h>
#include <util/util.h>

// NVDriver 
// Accelerated rendering driver for test suite purposes state (0.4.0.0+)

//
// Structures
//

// Driver Escape functions called from the main application
typedef enum nv3_driver_escape_e
{
    nv3driver_escape_main = 0,
} nv3_driver_escape;

typedef struct nv3_driver_init_settings_s
{
    bool use_mode_ini;
    nv3_driver_escape escape; 
} nv3_driver_init_settings_t; 

typedef struct nv3_driver_s
{
    nv3_driver_init_settings_t settings;
    nv3_mode_t mode; 
} nv3_driver_t;

//
// Globals
//
extern nv3_driver_t nv3_driver; 

//
// Functions
//
void nv3driver_main();
bool nv3driver_enter_graphics_mode();
bool nv3driver_leave_graphics_mode();



// (0x53 ^ 0x48 ^ 0x49 ^ 0x54 ^ 0x00) & 0x7F    = 6 * (2 << RAMHT_CONFIG.SIZE) * 8 = RAMHT location of this object. USE CONTEXT INFORMATION TO LOCATE GROBJ!
// This is a RECTANGLE.
#define OBJECT_TEST_NAME        0x53484954
#define OBJECT_TEST_CHANNEL     0
#define OBJECT_TEST_SUBCHANNEL  0

// RAMIN Offset = 0x4600
// Object Type = 0x47 (Rectangle)
// Map Struct to NV_USER for method execution 
#define OBJECT_TEST_CONTEXT     0x0460C700
