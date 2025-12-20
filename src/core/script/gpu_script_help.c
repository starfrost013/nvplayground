/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    main_help.c: Implements help message
*/

#include <nvplay.h>

const char* msg_help_script = "nvPlay Commands help:\n\n"
"---GENERAL---\n\n"
"rt, runtest testname: Run test\n"
"print text: Print message\n"
"printdebug text: Print message (Debug verbosity)\n"
"printwarning text: Print message (Warning verbosity)\n"
"printerror text: Print message (Error verbosity)\n"
"printversion: Print nvPlay version\n"
".\n"
"---IO---\n\n"
"rmc[8/32] readmmioconsole[8/16/32] offset: Read the 8/32-bit MMIO register (there are no 16-bit MMIO registers) at the address \"offset\" and print it to the console.\n"
"wm[8/32], writemmio[8/16/32] offset value: Write the value \"value\" to an 8/32-bit MMIO (there are no 16-bit MMIO registers) register at the address \"offset\".\n"
"wmr[8], writemmiorange8 offset_start offset_end value: Write a single value to 8/32-bit MMIO (there are no 16-bit MMIO registers) registers from BAR0 offset_start to offset_end.\n"
"rvc[8/16/32] readvramconsole[8/16/32] offset: Read an 8/16/32-bit VRAM starting at the BAR1 address \"offset\" and print it to the console.\n"
"wv[8/16/32], writevram[8/16/32] offset value: Write an 8/16/32-bit VRAM at the BAR1 address \"offset\".\n"
"wvr[8/16/32], writevramrange[8/16/32] offset_start offset_end value: Write a single value to all of the 8/16/32-bit VRAM/BAR1 from offset_start to offset_end.\n"
"rpc[8/16/32] readpciconsole[8/16/32] offset: Read an 8/16/32-bit PCI config register at the address \"offset\" and print it to the console.\n"
"wp[8/16/32], writepci[8/16/32] offset value: Write an 8/16/32-bit PCI config register at the address \"offset\". \n"
"wpr[8], writepcirange[8/16/32] offset_start offset_end value: Write a single value to all of the 8/16/32-bit PCI config registers from offset_start to offset_end.\n"
"rrc32, readramin32 offset: Read Nvidia instance memory at the 32-bit address \"offset\" and print it to the console.\n"
"wr32, writeramin32 offset value: Write Nvidia instance memory at the 32-bit address \"offset\".\n"
"wrr32, writeraminrange32 offset_start offset_end value: Write a single value to all of Nvidia instance memory from offset_start to offset_end.\n"
"rcrtcc, readcrtc offset: Read a VGA CRTC register at the address \"offset\" and print it to the console.\n"
"wcrtc, writecrtc offset value: Write a new value to the VGA CRTC register at the address \"offset\".\n"
"wcrtcrange, writecrtcrange offset_start offset_end value: Write a single value to all of the VGA CRTC registers from index_start to index_end.\n"
"rgdc, readgdcconsole offset: Read the VGA graphics data register at the address \"offset\" and print it to the console.\n"
"wgdc, writegdc offset value: Write the VGA graphics data register at the address \"offset\" offset parameter.\n"
"wgdcrange, writegdcrange offset_start offset_end value: Write a single value to all of the VGA graphics data registers from index_start to index_end.\n"
"rsrc, readsrconsole offset: Read the VGA sequencer register at the address \"offset\" and print it to the console.\n"
"wsr, writesr offset value: Write the VGA sequencer register starting at the address \"offset\".\n"
"wsrrange, writesrrange offset_start offset_end value: Write a single value to all of the VGA sequencer registers from index_start to index_end.\n"

".\n"
"---MISC---\n\n"
"nv3_explode: NV3 Mediaport test. ***MAY CRASH GPU OR SYSTEM***\n"
;
