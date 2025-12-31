/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    main_help.c: Implements help message
*/

#include <nvplay.h>

const char* msg_help = "nvPlay help:\n\n"
"---COMMAND LINE OPTIONS---\n\n"
"By default (without any command-line options) nvPlay will enter into a REPL loop that lets you perform raw level I/O with a supported GPU.\n"
"\x1b[1;32m-d, -dry.\x1b[1;00m: Dry run. Initialise the graphics hardware but do not run any tests. This mode can be used to revive cards with a dead Video BIOS under Windows 9x\n"
"\x1b[1;32m-a, -all.\x1b[1;00m: Run all tests regardless of if your graphics hardware supports them. THIS IS EXTREMELY INADVISABLE. *DO NOT* DO THIS UNLESS YOU KNOW WHAT YOU ARE DOING!\n"
"\x1b[1;32m-s, -script <file>.\x1b[1;00m: Run a .NVS script file.\n"
"\x1b[1;32m-t, -test.\x1b[1;00m: Enter into test mode. If supported graphics hardware is detected, nvplay.ini will be parsed and tests that are enabled will be run.\n"
"\x1b[1;32m-boot, -bootonly.\x1b[1;00m: Boot the GPU and exit. This can be used to initialise and run NVIDIA GPUs that have broken VBIOSes (at least under DOS and Windows 9x using autoexec)\n"
"\x1b[1;32m-?, -help.\x1b[1;00m: Show this text and exit\n\n"
"\x1b[1;32m---SUPPORTED GRAPHICS CARDS---\x1b[1;00m\n\n"
"The following graphics cards are supported by nvPlay:\n"
"\x1b[1;32mBest support (a wide range of tests and understanding available):\n\n"
"nVIDIA RIVA 128 (NV3)\n"
"nVIDIA RIVA 128 ZX (NV3T)\n\n"
"\x1b[1;32mSome support (some tests and understanding)\x1b[1;00m:\n"
"nVIDIA NV1\n"
"nVIDIA RIVA TNT (NV4)\n"
"nVIDIA RIVA TNT2 (NV5) & M64 (NV6)\n\n"
"\x1b[1;32mUntested, but should have basic support\x1b[1;00m:\n"
"nVIDIA GeForce 256 (NV10) & GeForce DDR (NV10DDR)\n"
"nVIDIA Quadro (NV10GL)\n"
"nVIDIA GeForce 2 & Quadro 2 (NV15)\n"
"nVIDIA GeForce 2 MX & Quadro 2 MX (NV11)\n"
"nVIDIA GeForce 4 MX (NV17) incl. AGP 8X (NV18) and PCIe (NV19)\n"
"Any other graphics hardware is \x1b[1;31mnot supported at this time.\x1b[1;00m"
;