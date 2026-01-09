/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv1_class_names.c: NV1 friendly class names
*/

/* These are the object classes AS RECOGNISED BY THE GRAPHICS HARDWARE. */
/* The drivers implement a COMPLETELY DIFFERENT SET OF CLASSES. */

/* THERE CAN ONLY BE 32 CLASSES IN NV1 BECAUSE THE CLASS ID PART OF THE CONTEXT OF A GRAPHICS OBJECT IN PFIFO RAM HASH TABLE IS ONLY 5 BITS LONG! */

const char* nv1_class_names[] = 
{
    "NV1 INVALID class 0x00",
    "NV1 class 0x01: Beta factor",
    "NV1 class 0x02: Render operation",
    "NV1 class 0x03: Chroma key",
    "NV1 class 0x04: Plane mask",
    "NV1 class 0x05: Clipping rectangle",
    "NV1 class 0x06: Pattern",
    "NV1 INVALID class 0x07",
    "NV1 class 0x08: Point",
    "NV1 class 0x09: Line",
    "NV1 class 0x0A: Lin (line without starting or ending pixel)",
    "NV1 class 0x0B: Triangle",
    "NV1 class 0x0C: Rectangle",
    "NV1 class 0x0D: Bilinear texture mapped quad",
    "NV1 class 0x0E: Quadratic texture mapped quad",
    "NV1 INVALID class 0x0F",
    "NV1 class 0x10: Blit",
    "NV1 class 0x11: Image from CPU",
    "NV1 class 0x12: Bitmap",
    "NV1 class 0x13: Image from Memory",
    "NV1 class 0x14: Image to Memory",
    "NV1 class 0x15: Stretched image from CPU",
    "NV1 INVALID class 0x16",
    "NV1 INVALID class 0x17",
    "NV1 INVALID class 0x18",
    "NV1 INVALID class 0x19",
    "NV1 INVALID class 0x1A",
    "NV1 INVALID class 0x1B",
    "NV1 INVALID class 0x1C",
    "NV1 class 0x1D: Bilinear texture mapped quad w/ beta factor",
    "NV1 class 0x1E: Bilinear texture mapped quad w/ beta factor",
    "NV1 INVALID class 0x1F",
};