/*
    NVPlay
    Copyright © 2025 starfrost


    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)
    
    nv3_tests.c: NV3 test suite implementation
*/

#include <stdio.h>
#include <stdlib.h>

// Architecture Includes
#include <architecture/nv3/nv3.h>

#include <architecture/nv3/nv3_ref.h>

#include "nvplayground.h"
#include "util/util.h"

//
// GARBAGE TEST 
//

#define NV3_GARBAGE_MMIO_AREA_01                0x610
#define NV3_GARBAGE_MMIO_AREA_02                0x344D90
#define NV3_GARBAGE_MMIO_AREA_03                0x7E0C00

#define NV3_GARBAGE_MMIO_READ_NUM_ITERATIONS    20000

// Reads garbage MMIO so that we can see what reading unmapped mmio does

bool nv3_garbage_mmio_read()
{
    uint32_t last_area01_value = 0, current_area01_value = 0;
    uint32_t last_area02_value = 0, current_area02_value = 0;
    uint32_t last_area03_value = 0, current_area03_value = 0;

    Logging_Write(log_level_message, "NV3_GarbageMMIORead: Iterations = %d\n", NV3_GARBAGE_MMIO_READ_NUM_ITERATIONS);

    for (uint32_t i = 0; i < NV3_GARBAGE_MMIO_READ_NUM_ITERATIONS; i++)
    {
        current_area01_value = nv_mmio_read32(NV3_GARBAGE_MMIO_AREA_01);
        current_area02_value = nv_mmio_read32(NV3_GARBAGE_MMIO_AREA_02);
        current_area03_value = nv_mmio_read32(NV3_GARBAGE_MMIO_AREA_03);

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
                NV3_GARBAGE_MMIO_AREA_01, last_area01_value, current_area01_value, i, NV3_GARBAGE_MMIO_READ_NUM_ITERATIONS);
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
                NV3_GARBAGE_MMIO_AREA_02, last_area02_value, current_area02_value, i, NV3_GARBAGE_MMIO_READ_NUM_ITERATIONS);
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
                NV3_GARBAGE_MMIO_AREA_03, last_area03_value, current_area03_value, i, NV3_GARBAGE_MMIO_READ_NUM_ITERATIONS);
            }

            last_area03_value = current_area03_value;
        }
    }    

    return true; 
}


/* TEMPORARY test  function to test certain hardcoded overclocks */
bool nv3_test_overclock()
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
        double megahertz = nv_clock_mnp_to_mhz(current_device.crystal_hz, final_clock);

        Logging_Write(log_level_message, "Trying MCLK = %.2f Mhz (NV_PRAMDAC_MPLL_COEFF = %08lx)...\n", megahertz, final_clock);

        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, final_clock);

        uclock_t this_clock = uclock();

        // Sit in a spinloop until it's time to wake up
        while (this_clock - start_clock < (UCLOCKS_PER_SEC * NV3_TEST_OVERCLOCK_TIME_BETWEEN_RECLOCKS))
            this_clock = uclock();
    }

    Logging_Write(log_level_message, "We survived. Returning to 100Mhz...\n");

    /* restore original clock */
    if (current_device.crystal_hz == NV_CLOCK_BASE_14318180)
        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TEST_OVERCLOCK_BASE_14318);
    else
        nv_mmio_write32(NV3_PRAMDAC_CLOCK_MEMORY, NV3_TEST_OVERCLOCK_BASE_13500);

    return true; 
}

bool nv3_dump_vbios()
{
    Logging_Write(log_level_message, "Dumping Video BIOS...");

    FILE* vbios = fopen("nv3bios.bin", "wb");

    //uint32_t rom_location = NV3_PROM_START;
    uint32_t vbios_bin[8192];

    for (int32_t i = 0; i < 8192; i++)
    {
        vbios_bin[i] = _farpeekl(_dos_ds, 0xC0000 + i*4);
        //rom_location += 4; //dword 
    }   

    fwrite(vbios_bin, sizeof(vbios_bin), 1, vbios);

    fclose(vbios);
    Logging_Write(log_level_message, "Done!\n");

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

// Determines if an MMIO area is excluded.
bool nv3_mmio_area_is_excluded(uint32_t addr)
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

#define NV3_FLUSH_FREQUENCY     65536

bool nv3_dump_mmio()
{
    Logging_Write(log_level_message, "Dumping GPU PCI BARs (BAR0 = MMIO, BAR1 = VRAM/RAMIN)...\n");

    FILE* mmio_bar0 = fopen("nv3bar0.bin", "wb");
    FILE* mmio_bar1 = fopen("nv3bar1.bin", "wb");

    if (!mmio_bar0
    || !mmio_bar1)
        return false;

    uint32_t* mmio_dump_bar_buf = (uint32_t*)calloc(1, NV3_MMIO_SIZE);

    if (!mmio_dump_bar_buf)
        return false; 
    
    // use ldt to read out BAR0 and BAR1 MMIO 
    // Flush every 64kb, because the real NV3 hardware may crash at some point?!

    /* 
        Dump all known memory regions except write-only ones and ones that crash
        We don't use nv_mmio_* because those will account for other things in the future
    */

    for (int32_t bar0_pos = 0; bar0_pos <= NV3_MMIO_SIZE; bar0_pos += 4)
    {
        // subtract nv3_flush_frequency to start at 0
        if (bar0_pos % NV3_FLUSH_FREQUENCY == 0 
            && bar0_pos > 0) // i'm lazy
        {
            Logging_Write(log_level_debug, "Dumped BAR0 up to: %08lX\n", bar0_pos);
            fwrite(&mmio_dump_bar_buf[(bar0_pos - NV3_FLUSH_FREQUENCY) >> 2], NV3_FLUSH_FREQUENCY, 1, mmio_bar0);
            fflush(mmio_bar0);

            // don't try and read out of bounds
            if (bar0_pos == NV3_MMIO_SIZE)
                break;
        }

        // skip the address if it will crash 
        if (nv3_mmio_area_is_excluded(bar0_pos))
        {
            mmio_dump_bar_buf[bar0_pos >> 2] = 0x4E4F4E45; // 'NONE'
        }
        else
            mmio_dump_bar_buf[bar0_pos >> 2] = _farpeekl(current_device.bar0_selector, bar0_pos);

    }

    fclose(mmio_bar0);

    for (int32_t bar1_pos = 0; bar1_pos <= NV3_MMIO_SIZE; bar1_pos += 4)
    {
        if ((bar1_pos % NV3_FLUSH_FREQUENCY == 0 
            && bar1_pos > 0))
        {
            Logging_Write(log_level_debug, "Dumped BAR1 up to: %08lX\n", bar1_pos);
            fwrite(&mmio_dump_bar_buf[(bar1_pos - NV3_FLUSH_FREQUENCY) >> 2], NV3_FLUSH_FREQUENCY, 1, mmio_bar1);
            fflush(mmio_bar1);

            // don't try and read out of bounds
            if (bar1_pos == NV3_MMIO_SIZE)
                break;
        }

        // no excluded areas needed
        mmio_dump_bar_buf[bar1_pos >> 2] = _farpeekl(current_device.bar1_selector, bar1_pos);

    }

    fclose(mmio_bar1);
    
    free(mmio_dump_bar_buf);

    Logging_Write(log_level_message, "Done!\n");

    return true; 
}

bool nv3_dump_mfg_info()
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

    uint32_t vpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_PIXEL);
    uint32_t mpll = nv_mmio_read32(NV3_PRAMDAC_CLOCK_MEMORY);
    
    //todo: convert to MHz

    double vpll_mhz = nv_clock_mnp_to_mhz(current_device.crystal_hz, vpll);
    double mpll_mhz = nv_clock_mnp_to_mhz(current_device.crystal_hz, mpll);

    Logging_Write(log_level_message, "Pixel Clock Coefficient   = %08lX (%.2f MHz)\n", vpll, vpll_mhz);
    Logging_Write(log_level_message, "Core/Mem Clock Coefficient= %08lX (%.2f MHz)\n", mpll, mpll_mhz);
    return true; 

}