/*
    NVPlay
    Copyright © 2025-2026 starfrost


    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)
    
    nv3_tests.c: NV3 test suite implementation
*/

#include <stdio.h>
#include <stdlib.h>

// Architecture Includes
#include <architecture/nv3/nv3.h>

#include <architecture/nv3/nv3_ref.h>

#include "nvplay.h"
#include "util/util.h"

//
// GARBAGE TEST 
//

#define NV3_GARBAGE_MMIO_AREA_01                0x610
#define NV3_GARBAGE_MMIO_AREA_02                0x344D90
#define NV3_GARBAGE_MMIO_AREA_03                0x7E0C00

#define NV3_ReadGarbageMMIO_NUM_ITERATIONS    20000

// Reads garbage MMIO so that we can see what reading unmapped mmio does

bool NV3_ReadGarbageMMIO()
{
    uint32_t last_area01_value = 0, current_area01_value = 0;
    uint32_t last_area02_value = 0, current_area02_value = 0;
    uint32_t last_area03_value = 0, current_area03_value = 0;

    Logging_Write(log_level_message, "NV3_GarbageMMIORead: Iterations = %d\n", NV3_ReadGarbageMMIO_NUM_ITERATIONS);

    for (uint32_t i = 0; i < NV3_ReadGarbageMMIO_NUM_ITERATIONS; i++)
    {
        current_area01_value = NV_ReadMMIO32(NV3_GARBAGE_MMIO_AREA_01);
        current_area02_value = NV_ReadMMIO32(NV3_GARBAGE_MMIO_AREA_02);
        current_area03_value = NV_ReadMMIO32(NV3_GARBAGE_MMIO_AREA_03);

        /* check if any values changed */
        if (current_area01_value != last_area01_value)
        {
            if (i == 0)
            {
                Logging_Write(log_level_message, "NV3_GarbageMMIORead: Unmapped Test Area 1 (%08x) initial value: %08x\n",
                NV3_GARBAGE_MMIO_AREA_01, current_area01_value);
            }
            else
            {
                Logging_Write(log_level_message, "NV3_GarbageMMIORead: Unmapped Test Area 1 (%08x) changed: %08x -> %08x (iter %d/%d)\n",
                NV3_GARBAGE_MMIO_AREA_01, last_area01_value, current_area01_value, i, NV3_ReadGarbageMMIO_NUM_ITERATIONS);
            }

            last_area01_value = current_area01_value;
        }
                
        /* check if any values changed */
        if (current_area02_value != last_area02_value)
        {
            if (i == 0)
            {
                Logging_Write(log_level_message, "NV3_GarbageMMIORead: Unmapped Test Area 2 (%08x) initial value: %08x\n",
                NV3_GARBAGE_MMIO_AREA_02, current_area02_value);
            }
            else
            {
                Logging_Write(log_level_message, "NV3_GarbageMMIORead: Unmapped Test Area 2 (%08x) changed: %08x -> %08x (iter %d/%d)\n",
                NV3_GARBAGE_MMIO_AREA_02, last_area02_value, current_area02_value, i, NV3_ReadGarbageMMIO_NUM_ITERATIONS);
            }

            last_area02_value = current_area02_value;
        }        
        
        /* check if any values changed */
        if (current_area03_value != last_area03_value)
        {
            if (i == 0)
            {
                Logging_Write(log_level_message, "NV3_GarbageMMIORead: Unmapped Test Area 3 (%08x) initial value: %08x\n",
                NV3_GARBAGE_MMIO_AREA_03, current_area03_value);
            }
            else
            {
                Logging_Write(log_level_message, "NV3_GarbageMMIORead: Unmapped Test Area 3 (%08x) changed: %08x -> %08x (iter %d/%d)\n",
                NV3_GARBAGE_MMIO_AREA_03, last_area03_value, current_area03_value, i, NV3_ReadGarbageMMIO_NUM_ITERATIONS);
            }

            last_area03_value = current_area03_value;
        }
    }    

    return true; 
}


/* TEMPORARY test  function to test certain hardcoded overclocks */
bool NV3_TestOverclock()
{
    /* print out some helpful messages */
    Logging_Write(log_level_message, "Basic clockspeed test (text mode: best case scenario)\n");
    Logging_Write(log_level_message, "The GPU will try to run for 60 seconds at each clock setting, gradually going from an underclock to an overclock. If it crashes, reboot, and the default 100Mhz MCLK will be restored.\n");
    Logging_Write(log_level_message, "Note: Some NVIDIA RIVA 128 ZX cards manufactured by TSMC run at 90Mhz and will have less overclocking potential!\n");

    /* We vary the n-parameter of the MCLK to fine-tune the GPU clock speed. M can be used for large steps and P param can be used for very big steps */

    uint32_t clock_n_start = 0xA3; 
    uint32_t clock_m = 0x0B, clock_p = 0x01;
    /*
        there are two possible clock bases here: 13.5 and 14.318 Mhz 
        we need two different values for our start menu
    */

    if (current_device.crystal_hz == NV_CLOCK_BASE_14318180)
    {
        clock_m = 0x0E;
        //base clock_n is 0xC4, so the gpu will be biased more towards underclocking
        clock_n_start = 0xC4;
    }

    for (int32_t clock_n = clock_n_start; clock_n <= 0xFF; clock_n++)
    {
        uclock_t start_clock = uclock();

        uint32_t final_clock = (clock_p << 16) 
        | (clock_n << 8)
        | clock_m;

        //not speed critical, use a double
        double megahertz = NV_ClockMNPToMhz(current_device.crystal_hz, final_clock);

        Logging_Write(log_level_message, "Trying MCLK = %.2f Mhz (NV_PRAMDAC_MPLL_COEFF = %08lx)...\n", megahertz, final_clock);

        NV_WriteMMIO32(NV3_PRAMDAC_CLOCK_MEMORY, final_clock);

        uclock_t this_clock = uclock();

        // Sit in a spinloop until it's time to wake up
        while (this_clock - start_clock < (UCLOCKS_PER_SEC * NV3_TestOverclock_TIME_BETWEEN_RECLOCKS))
            this_clock = uclock();
    }

    Logging_Write(log_level_message, "We survived. Returning to 100Mhz...\n");

    /* restore original clock */
    if (current_device.crystal_hz == NV_CLOCK_BASE_14318180)
        NV_WriteMMIO32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TestOverclock_BASE_14318);
    else
        NV_WriteMMIO32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TestOverclock_BASE_13500);

    return true; 
}

// Areas of the GPU space that may crash the system. These are to be ignored while dumping with the EXCLUDED string written.
nv3_dump_excluded_areas_t excluded_areas[] =
{
    { NV3_PME_START, NV3_PME_END },                             // At least one RIVA crashed when this area was accessed.
    { NV3_PGRAPH_CLASSES_START, NV3_PGRAPH_CLASSES_END, },      // Write-only area
    { 0x602000, 0x67FFFF },
    { 0, 0 },                                                   // Sentinel value
};

// see nv_generic.c
// Determines if an MMIO area is excluded.
bool NV3_MMIOAreaIsExcluded(uint32_t addr)
{
    nv3_dump_excluded_areas_t excluded_area = excluded_areas[0];

    uint32_t exclusion_number = 0;

    while (excluded_area.start != 0)
    {
        // if the address is excluded, return
        if (addr >= excluded_area.start
        && addr <= excluded_area.end)
        {
            return true; 
        }

        exclusion_number++;
        excluded_area = excluded_areas[exclusion_number];
    }

    return false;
}

bool NV3_DumpMFGInfo()
{
    /* TODO: Read our Dumb Framebuffer */
    Logging_Write(log_level_message, "NV3 Manufacture-Time Configuration: \n");
    Logging_Write(log_level_message, "NV_PMC_BOOT_0             = %08lX\n", current_device.nv_pmc_boot_0);
    Logging_Write(log_level_message, "NV_PFB_BOOT_0             = %08lX\n", current_device.nv_pfb_boot_0);
    
    /* 
        Determine the amount of Video RAM 
        In theory this could be a shared function between all nv gpus, but in reality i'm not so sure
    */

    Logging_Write(log_level_message, "Video RAM Size            = %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read in the straps */
    Logging_Write(log_level_message, "Straps                    = %08lX\n", current_device.straps);

    uint32_t vpll = NV_ReadMMIO32(NV3_PRAMDAC_CLOCK_PIXEL);
    uint32_t mpll = NV_ReadMMIO32(NV3_PRAMDAC_CLOCK_MEMORY);
    
    //todo: convert to MHz

    double vpll_mhz = NV_ClockMNPToMhz(current_device.crystal_hz, vpll);
    double mpll_mhz = NV_ClockMNPToMhz(current_device.crystal_hz, mpll);

    Logging_Write(log_level_message, "Pixel Clock Coefficient   = %08lX (%.2f MHz)\n", vpll, vpll_mhz);
    Logging_Write(log_level_message, "Core/Mem Clock Coefficient= %08lX (%.2f MHz)\n", mpll, mpll_mhz);

    return true; 
}