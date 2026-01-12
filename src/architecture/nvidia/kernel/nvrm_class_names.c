/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nvrm_class_names.c: NVRM class names for NV4+. HARDWARE IMPLEMENTED ONLY

    They changed the class system to be the same as the resource manager class system in NV4. Some classes are implemented in hw and others are trapped by sw (so NVIDIA can add
    software fallbacks for unsupported graphics features in newer drivers). Same is true for methods of classes so driver sare both forward & backward compatible
*/

#include <nvplay.h>
#include <architecture/nvidia/kernel/kernel.h>
#include <architecture/nvidia/kernel/nv_generic.h>

#define CLASS_LAST                  0x7F
#define SUPPORTED_BY_ALL_GENS       5090        // RTX 5090
nvrm_class_t nvrm_class[] =
{
    { 0x12, 1, SUPPORTED_BY_ALL_GENS, "NV01ContextBeta (012)" },      
    { 0x17, 1, SUPPORTED_BY_ALL_GENS, "Nv01ColorKey (017)" },
    { 0x18, 1, SUPPORTED_BY_ALL_GENS, "Nv01ImagePattern (018)" },
    { 0x19, 1, SUPPORTED_BY_ALL_GENS, "Nv01ImageBlackRectangle (019)" },
    { 0x1C, 1, SUPPORTED_BY_ALL_GENS, "Nv01Lin (01C)" },
    { 0x1D, 1, SUPPORTED_BY_ALL_GENS, "Nv01RenderSolidTriangle (01D)" },
    { 0x1E, 1, SUPPORTED_BY_ALL_GENS, "Nv01ImageBlit (01D)" },
    { 0x21, 1, SUPPORTED_BY_ALL_GENS, "Nv01ImageFromCpu (021)" },
    { 0x30, 1, SUPPORTED_BY_ALL_GENS, "Nv01Null (030) !!!! NULL !!!!" },
    { 0x36, 3, SUPPORTED_BY_ALL_GENS, "Nv03StretchedImageFromCpu (036)" },
    { 0x37, 3, SUPPORTED_BY_ALL_GENS, "Nv03ScaledImageFromMemory (037)" },
    { 0x38, 4, SUPPORTED_BY_ALL_GENS, "Nv04DvdSubpicture (038)" },
    { 0x39, 3, SUPPORTED_BY_ALL_GENS, "Nv03MemoryToMemoryFormat (039)" },
    { 0x42, 4, SUPPORTED_BY_ALL_GENS, "Nv04ContextSurfaces2d (042)" },               // NV3 0x1C replacement
    { 0x43, 3, SUPPORTED_BY_ALL_GENS, "Nv03ContextRop (043)" },
    { 0x44, 4, SUPPORTED_BY_ALL_GENS, "Nv04ImagePattern (044)" },
    { 0x48, 3, SUPPORTED_BY_ALL_GENS, "Nv03Dx3TexturedTriangle (048)" },             // **** OBSOLETE AS OF NV4 ****
    { 0x4A, 4, SUPPORTED_BY_ALL_GENS, "Nv04GdiRectangleText (04A)" },                // **** OBSOLETE AS OF NV4 ****
    { 0x4B, 3, SUPPORTED_BY_ALL_GENS, "Nv03GdiRectangleText (04B)" },                // **** OBSOLETE AS OF NV4 ****
    { 0x52, 4, SUPPORTED_BY_ALL_GENS, "Nv04ContextSurfaceSwizzled (052)" },
    { 0x53, 4, SUPPORTED_BY_ALL_GENS, "Nv04ContextSurfacesArgbZs (053)" },
    { 0x54, 4, SUPPORTED_BY_ALL_GENS, "Nv04Dx5TexturedTriangle (054)" },             // **** KIND OF OBSOLETE (DX5 only, not full games) Replaces NV3 <id> ****
    { 0x55, 4, SUPPORTED_BY_ALL_GENS, "Nv04Dx6MultiTextureTriangle (055)" },
    //0x56 is NV10 Primitive, 0x97 is NV20 Shader
    { 0x57, 4, SUPPORTED_BY_ALL_GENS, "Nv04ContextColorKey (057)" },
    { 0x58, 3, SUPPORTED_BY_ALL_GENS, "Nv03ContextSurface0 (058)" },                 // **** OBSOLETE AS OF NV4 **** (NV3 IMAGE_IN_MEMORY Buffer0) replaced by Nv04ContextSurfaces2d
    { 0x59, 3, SUPPORTED_BY_ALL_GENS, "Nv03ContextSurface1 (059)" },                 // **** OBSOLETE AS OF NV4 **** (NV3 IMAGE_IN_MEMORY Buffer1) replaced by Nv04ContextSurfaces2d
    { 0x5A, 3, SUPPORTED_BY_ALL_GENS, "Nv03ContextSurface2 (05A)" },                 // **** OBSOLETE AS OF NV4 **** (NV3 IMAGE_IN_MEMORY Buffer2) replaced by Nv04ContextSurfaces2d
    { 0x5B, 3, SUPPORTED_BY_ALL_GENS, "Nv03ContextSurface3 (05B)" },                 // **** OBSOLETE AS OF NV4 **** (NV3 IMAGE_IN_MEMORY Buffer3) replaced by Nv04ContextSurfaces2d
    { 0x5C, 4, SUPPORTED_BY_ALL_GENS, "Nv04RenderSolidLin (05C)" },
    { 0x5D, 4, SUPPORTED_BY_ALL_GENS, "Nv04RenderSolidTriangle (05D)" },
    { 0x5E, 4, SUPPORTED_BY_ALL_GENS, "Nv04RenderSolidRectangle (05E)" },
    { 0x5F, 4, SUPPORTED_BY_ALL_GENS, "Nv04ImageBlit (05F)" },
    { 0x60, 4, SUPPORTED_BY_ALL_GENS, "Nv04IndexedImageFromCpu (060)" },
    { 0x61, 4, SUPPORTED_BY_ALL_GENS, "Nv04ImageFromCpu (061)" },
    { 0x63, 5, SUPPORTED_BY_ALL_GENS, "Nv05ScaledImageFromMemory (063)" },            // **** NV5+ ****
    { 0x64, 4, SUPPORTED_BY_ALL_GENS, "Nv05IndexedImageFromCpu (064)" },
    { 0x65, 5, SUPPORTED_BY_ALL_GENS, "Nv05ImageFromCpu (065)" },                     // **** NV5+ ****
    { 0x66, 5, SUPPORTED_BY_ALL_GENS, "Nv05StretchedImageFromCpu (066)" },            // **** NV5+ ****
    { 0x67, 10, SUPPORTED_BY_ALL_GENS, "Nv10VideoLutCursorDac (067)" },               // Exists in NV4/NV5, but may be different. **** NV10+ ****
    { 0x72, 4, SUPPORTED_BY_ALL_GENS, "Nv04ContextBeta (072)" },
    { 0x76, 4, SUPPORTED_BY_ALL_GENS, "Nv04StretchedImageFromCpu (076)" },
    { 0x77, 4, SUPPORTED_BY_ALL_GENS, "Nv04ScaledImageFromMemory (077)" },

};