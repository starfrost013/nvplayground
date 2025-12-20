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
"\x1b[1;32mrmc[8/32] readmmioconsole[8/16/32]offset\x1b[39m: Read the 8/32-bit MMIO register (there are no 16-bit MMIO registers) at the address \"offset\" and print it to the console.\n"
"\x1b[1;32mwm[8/32], writemmio[8/16/32] offset value\x1b[39m: Write the value \"value\" to an 8/32-bit MMIO (there are no 16-bit MMIO registers) register at the address \"offset\".\n"
"\x1b[1;32mwmr[8], writemmiorange8 offset_start offset_end value\x1b[39m: Write a single value to 8/32-bit MMIO (there are no 16-bit MMIO registers) registers from BAR0 offset_start to offset_end.\n"
"\x1b[1;32mrvc[8/16/32] readvramconsole[8/16/32] offset\x1b[39m: Read an 8/16/32-bit VRAM starting at the BAR1 address \"offset\" and print it to the console.\n"
"\x1b[1;32mrwv[8/16/32], writevram[8/16/32] offset value\x1b[39m: Write an 8/16/32-bit VRAM at the BAR1 address \"offset\".\n"
"\x1b[1;32mrwvr[8/16/32], writevramrange[8/16/32] offset_start offset_end value\x1b[39m: Write a single value to all of the 8/16/32-bit VRAM/BAR1 from offset_start to offset_end.\n"
"\x1b[1;32mrrpc[8/16/32] readpciconsole[8/16/32] offset\x1b[39m: Read an 8/16/32-bit PCI config register at the address \"offset\" and print it to the console.\n"
"\x1b[1;32mrwp[8/16/32], writepci[8/16/32] offset value\x1b[39m: Write an 8/16/32-bit PCI config register at the address \"offset\". \n"
"\x1b[1;32mrwpr[8], writepcirange[8/16/32] offset_start offset_end value\x1b[39m: Write a single value to all of the 8/16/32-bit PCI config registers from offset_start to offset_end.\n"
"\x1b[1;32mrrrc32, readramin32 offset\x1b[39m: Read Nvidia instance memory at the 32-bit address \"offset\" and print it to the console.\n"
"\x1b[1;32mrwr32, writeramin32 offset value\x1b[39m: Write Nvidia instance memory at the 32-bit address \"offset\".\n"
"\x1b[1;32mrwrr32, writeraminrange32 offset_start offset_end value\x1b[39m: Write a single value to all of Nvidia instance memory from offset_start to offset_end.\n"
"\x1b[1;32mrrcrtcc, readcrtc offset\x1b[39m: Read a VGA CRTC register at the address \"offset\" and print it to the console.\n"
"\x1b[1;32mrwcrtc, writecrtc offset value\x1b[39m: Write a new value to the VGA CRTC register at the address \"offset\".\n"
"\x1b[1;32mrwcrtcrange, writecrtcrange offset_start offset_end value\x1b[39m: Write a single value to all of the VGA CRTC registers from index_start to index_end.\n"
"\x1b[1;32mrrgdc, readgdcconsole offset\x1b[39m: Read the VGA graphics data register at the address \"offset\" and print it to the console.\n"
"\x1b[1;32mrwgdc, writegdc offset value\x1b[39m: Write the VGA graphics data register at the address \"offset\" offset parameter.\n"
"\x1b[1;32mrwgdcrange, writegdcrange offset_start offset_end value\x1b[39m: Write a single value to all of the VGA graphics data registers from index_start to index_end.\n"
"\x1b[1;32mrrsrc, readsrconsole offset\x1b[39m: Read the VGA sequencer register at the address \"offset\" and print it to the console.\n"
"\x1b[1;32mrwsr, writesr offset value\x1b[39m: Write the VGA sequencer register starting at the address \"offset\".\n"
"\x1b[1;32mrwsrrange, writesrrange offset_start offset_end value\x1b[39m: Write a single value to all of the VGA sequencer registers from index_start to index_end.\n"

".\n"
"---MISC---\n\n"
"\x1b[1;32mrnv3_explode\x1b[39m: NV3 Mediaport test. ***MAY CRASH GPU OR SYSTEM***\n"
;
