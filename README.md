# NVPlay

## GPU research driver & raw programming tool

This is a test suite intended to allow for hardware testing and emulation of early NVidia graphics processing units so that they can be emulated as part of the [86Box-nv](https://github.com/starfrost013/) project. It runs under [DJGPP](https://www.delorie.com/djgpp/) in order to obtain exclusive use of the graphics processor's resources without
a Windows or Linux driver interfering while still benefiting from a 32-bit protected mode environment.

The program is currently capable of performing several modes of operation:

* Running a user-defined "main" function (not actually implemented in practice)
* Running any of the tests defined in the `nvplay.ini` file
* Running a ".nvp" script (GPUScript will be documented when a real interpreter exists for it)
* Running a repl loop that allows you to do raw GPU I/O

All of this functionality is fairly early and is subject to change at any time. Feel free to submit a pull request to contribute to the project or report issues.

## Supported hardware 

### Well-supported

**Nvidia RIVA 128 (NV3)** / **Nvidia RIVA 128 ZX (NV3T)**: This has been extensively tested across several units of NV3 and NV3T devices. If you have one, please run this program under it and contribute to the testing effort by submitting your nvplay.log to me (currently, just contact me on discord @ thefrozenstar_, but a place to submit runs will be worked out eventually).

**Nvidia RIVA TNT (NV4)**: Should have almost the same level of support as NV3. Help is also encouraged for this GPU.

### Some support

**Nvidia NV1**: Tested and working but with only basic compatibility. I now one one of these thanks to a very generous donation, so support will be coming soon

**Nvidia RIVA TNT2 / TNT2 Ultra / Vanta / Vanta LT (NV5 / NV5ULTRA / NV5VANTA / NV6)**: Should mostly work with NV4 support, but untetsed.

**Nvidia "Celsius" based cards (NV1x) (GeForce 256, 2, 2 MX, Quadro 1 & 2, and 4 MX)**: Very basic and untested support. **NOTE:** Regular GeForce 4 is a much different architecture and is **NOT** supported. See below!

### Not supported, but will be supported in the future 


**ATI Rage series (3D Rage/3D Rage II/Rage Pro/Rage128...)**: Low priority. Some background research is happening on supporting these

### Not supported, and is not intended to be supported in the future

**Anything after this**: Any later microarchitecture is largely covered by the Xbox or other work (NV20) or too complex to emulate or maintain support for reasonably.  

**3dfx cards**: These seem to be well-supported by internal tools that are used to repair these cards.

### Unknown

**Other cards (Matrox, ...)**: Research required.

**Other PCI devices**: This project could hypothetically be extended to any PCI/AGP/PCIe device of interest, not just graphics hardware. Most likely, supporrt for these devices
will only be added if someone wishes to add it, merged into main at my, as the maintainer's discretion. 

## Supported software environments

### Well-supported

**MS-DOS under DJGPP**: This is the intended and gold standard environment for running NVPlay.

### Some support

**Windows 9x DOS box**: Should work, but be wary of running tests, especially graphics tests, while the drivers are running. Nvidia's drivers, especially their old ones, are infamously temperamental and may not like being interfered with.

### Not supported

**Windows NT (NTVDM)**: The PCI BIOS is not emulated, so, until if/when the PCI bus is manually enumerated, NTVDM will not work.