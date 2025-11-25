/* 
    NVPlay
    Copyright © 2025 starfrost

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
"printversion: Print NVPlay version\n"
"\n"
"---IO---\n\n"
"rmc[8/32] readmmioconsole[8/16/32] offset: Read 8 or 32-bit MMIO (16-bit MMIO registers are not used) registers starting at the BAR0 offset corresponding to the offset parameter and print it to the console.\n"
"wm[8/32], writemmio[8/16/32] offset value: Write8 or 32-bit MMIO (16-bit MMIO registers are not used) registers starting at the BAR0 offset corresponding to the offset parameter to the graphics hardware.\n"
"wmr[8], writemmiorange8 offset_start offset_end value value: Write a single value to a range of 8 or 32-bit MMIO (16-bit MMIO registers are not used) registers from BAR0 offset_start to offset_end to the graphics hardware.\n"
"rvc[8/16/32] readvramconsole[8/16/32] offset: Read 8, 16, or 32-bit VRAM starting at the BAR1 offset corresponding to the offset parameter and print it to the console.\n"
"wv[8/16/32], writevram[8/16/32] offset value: Write 8, 16, or 32-bit VRAM at the BAR1 offset corresponding to the offset parameter to the graphics hardware.\n"
"wvr[8/16/32], writevramrange[8/16/32] offset_start offset_end value: Write a single value to a range of 8, 16, or 32-bit VRAM from BAR1 offset_start to offset_end to the graphics hardware.\n"
"rpc[8/16/32] readpciconsole[8/16/32] offset: Read 8, 16, or 32-bit PCI config register offset corresponding to the offset parameter and print it to the console.\n"
"wp[8/16/32], writepci[8/16/32] offset value: Write 8, 16, or 32-bit PCI config register corresponding to the offset parameter to the graphics hardware.\n"
"wpr[8], writepcirange[8/16/32] offset_start offset_end value: Write a single value to a range of 8, 16, or 32-bit PCI config registers from offset_start to offset_end to the graphics hardware.\n"
"rrc32, readramin32 offset: Read Nvidia instance memory starting at the offset corresponding to the offset parameter and print it to the console.\n"
"wr32, writeramin32 offset value: Write Nvidia instance memory starting at the offset corresponding to the offset parameter to the graphics hardware.\n"
"wrr32, writeraminrange32 offset_start offset_end value: Write a single value to Nvidia instance memory within the range from offset_start to offset_end to the graphics hardware.\n"
"rcrtcc, readcrtc offset: Read a VGA CRTC register starting at the index corresponding to the offset parameter and print it to the console.\n"
"wcrtc, writecrtc offset value: Write a VGA CRTC register starting at the index corresponding to the offset parameter to the graphics hardware.\n"
"wcrtcrange, writecrtcrange offset_start offset_end value: Write a single value to VGA CRTC registers within the range from index_start to index_end to the graphics hardware.\n"
"rgdc, readgdcconsole offset: Read a VGA graphics data register starting at the index corresponding to the offset parameter and print it to the console.\n"
"wgdc, writegdc offset value: Write a VGA graphics data register starting at the index corresponding to the offset parameter to the graphics hardware.\n"
"wgdcrange, writegdcrange offset_start offset_end value: Write a single value to VGA graphics data registers within the range from index_start to index_end to the graphics hardware.\n"
"rsrc, readsrconsole offset: Read a VGA sequencer register starting at the offset corresponding to the offset parameter and print it to the console.\n"
"wsr, writesr offset value: Write a VGA sequencer register starting at the offset corresponding to the offset parameter to the graphics hardware.\n"
"wsrrange, writesrrange offset_start offset_end value: Write a single value to VGA sequencer registers within the range from index_start to index_end to the graphics hardware.\n"

"\n"
"---MISC---\n\n"
"nv3_explode: NV3 Mediaport test. ***MAY CRASH GPU OR SYSTEM***\n"
;
