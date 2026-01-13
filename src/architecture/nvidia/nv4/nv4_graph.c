/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    nv4_graph.c: NV4 pgraph kernel

    Notes:
    - We do context switch but not as much as Nvidia does.
    - We don't bother with any DX3/DX5 compatibility levels. Only native rendering level (DX6) is used.
    - We directly work with RAMHT instead of submitting graphics patches.
*/

/* Default value on reset */
#define CTX_CONTROL_DEFAULT_UNLOADED (NV4_PGRAPH_CTX_CONTROL_MINIMUM_TIME_33US << NV4_PGRAPH_CTX_CONTROL_MINIMUM_TIME) \
| (NV4_PGRAPH_CTX_CONTROL_TIME_EXPIRED << NV4_PGRAPH_CTX_CONTROL_TIME) \
| (NV4_PGRAPH_CTX_CONTROL_CHID_INVALID << NV4_PGRAPH_CTX_CONTROL_CHID) \
| (NV4_PGRAPH_CTX_CONTROL_SWITCHING_IDLE << NV4_PGRAPH_CTX_CONTROL_SWITCHING) \
| (NV4_PGRAPH_CTX_CONTROL_DEVICE_ENABLED << NV4_PGRAPH_CTX_CONTROL_DEVICE)


#include "core/gpu/gpu.h"
#include "nv4_ref.h"
#include "nvplay.h"
#include "util/util.h"

#include <architecture/nvidia/kernel/kernel.h>
#include <architecture/nvidia/nv4/nv4.h>

/* Init rendering - NV4 */
bool NV4_InitGraph()
{
    /* 
    Initialise PGRAPH debug registers based on 6.xx drivers. These are required for GPU functionality
    Let's explicitly turn off some options just in case
    */
    kernel_gpu->nv4_pgraph.debug_0 = (1 << NV4_PGRAPH_DEBUG_0_LIMIT_CHECK)  // Enable limit checking
    | (1 << NV4_PGRAPH_DEBUG_0_LIMIT_INT)
    | (0 << NV4_PGRAPH_DEBUG_0_OVRFLW_INT)
    | (1 << NV4_PGRAPH_DEBUG_0_WRITE_ONLY_ROPS_2D)                          // Write only ROPS (2D)
    | (1 << NV4_PGRAPH_DEBUG_0_WRITE_ONLY_ROPS_3D)
    | (0 << NV4_PGRAPH_DEBUG_0_DRAWDIR_AUTO)                                // Manually set draw dir
    | (NV4_PGRAPH_DEBUG_0_DRAWDIR_Y_INCR << NV4_PGRAPH_DEBUG_0_DRAWDIR_Y)  
    | (1 << NV4_PGRAPH_DEBUG_0_ALPHA_ABORT)
    | (1 << NV4_PGRAPH_DEBUG_0_MINUSD5)         /* tf */
    | (1 << NV4_PGRAPH_DEBUG_0_BLIT_DST_LIMIT); 

    // Nvidia drivers set to last on reset then not last
    kernel_gpu->nv4_pgraph.debug_1 = (NV4_PGRAPH_DEBUG_1_VOLATILE_RESET_LAST << NV4_PGRAPH_DEBUG_1_VOLATILE_RESET)
    | (1 << NV4_PGRAPH_DEBUG_1_PATCH_INV)                                   // Patch inversion?
    | (1 << NV4_PGRAPH_DEBUG_1_TRI_OPTS)                                    // Enable triangle operations
    | (1 << NV4_PGRAPH_DEBUG_1_TRICLIP_OPTS)                                // Enable triangle clipping
    | (1 << NV4_PGRAPH_DEBUG_1_INSTANCE)                                    // Enable objects
    | (1 << NV4_PGRAPH_DEBUG_1_CTX)                                         // Enable objects [Context]
    | (NV4_PGRAPH_DEBUG_1_CACHE_IGNORE << NV4_PGRAPH_DEBUG_1_CACHE)         // ?
    | (1 << NV4_PGRAPH_DEBUG_1_CACHE_MODE)                                  // Enable PGRAPH_CACHE
    | (1 << NV4_PGRAPH_DEBUG_1_ZCLAMP)                                      // Clamp Z coordinates
    | (1 << NV4_PGRAPH_DEBUG_1_UCLAMP)                                      // Clamp U texture coordinates
    | (1 << NV4_PGRAPH_DEBUG_1_RCLAMP)                                      // Clamp R(?) coordinates
    | (1 << NV4_PGRAPH_DEBUG_1_DX6_2PIXMODE);                               // NVidia drivers only enable this on NV5. Check if it causes errata
    
    kernel_gpu->nv4_pgraph.debug_2 = (1 << NV4_PGRAPH_DEBUG_2_PINGPONG)     // ?????????????????????????????????
    | (NV4_PGRAPH_DEBUG_2_ZBUF_SEQ_AUTO << NV4_PGRAPH_DEBUG_2_ZBUF_SEQ)     // Z buffer order
    | (1 << NV4_PGRAPH_DEBUG_2_FAST_VERTEX_LOAD)                            // Better than slow I guess
    | (1 << NV4_PGRAPH_DEBUG_2_BILINEAR_3D_ENABLED)                         // Enable bilinear texture filtering
    | (1 << NV4_PGRAPH_DEBUG_2_ANISOTROPIC_3D_ENABLED)                      // Enable anisotropic texture filtering
    | (1 << NV4_PGRAPH_DEBUG_2_FOG_3D_ENABLED)                              // Enable fog (idk if it is vertex or table)
    | (1 << NV4_PGRAPH_DEBUG_2_SPECULAR_3D_ENABLED)                         // Enable specular highlight
    | (1 << NV4_PGRAPH_DEBUG_2_ALPHA_3D_ENABLED)                            // Enable alpha blending (disabled is S3 ViRGE mode)
    | (1 << NV4_PGRAPH_DEBUG_2_COELESCE_2D_ENABLED)                         // Write coalescing enabled [2D]
    | (1 << NV4_PGRAPH_DEBUG_2_COELESCE_D3D_ENABLED)                        // Write coalescing enabled [3D]
    | (1 << NV4_PGRAPH_DEBUG_2_BLIT_MULTILINE_ENABLED)                      // Blit multiple lines at once
    | (1 << NV4_PGRAPH_DEBUG_2_VOLATILE_RESET_ENABLED)                      // Reset volatile state (non-RAMFC)
    // Todo: do we need these?. These get enabled
    | (1 << NV4_PGRAPH_DEBUG_2_SPARE1)
    | (1 << NV4_PGRAPH_DEBUG_2_SPARE2)
    | (1 << NV4_PGRAPH_DEBUG_2_SPARE3);

    // DX3 culling mode is used by nvidia. So if it causes errors change it back - but we try to avoid compatibility modes
    kernel_gpu->nv4_pgraph.debug_3 = (NV4_PGRAPH_DEBUG_3_CULLING_TYPE_DX5 << NV4_PGRAPH_DEBUG_3_CULLING_TYPE)
    | (1 << NV4_PGRAPH_DEBUG_3_CULLING)                                     // Enable culling
    | (1 << NV4_PGRAPH_DEBUG_3_FAST_DATA_STRTCH)                            // Enable fast stretched image handling
    | (1 << NV4_PGRAPH_DEBUG_3_FAST_DATA_D3D)                               // Enable fast 3D
    | (NV4_PGRAPH_DEBUG_3_ZFLUSH_IGNORE << NV4_PGRAPH_DEBUG_3_ZFLUSH)       // Don't (manually?) flush Z-buffer (see if we need to change this)
    | (1 << NV4_PGRAPH_DEBUG_3_AUTOZFLUSH_PTZ)                              // Point with zeta?
    | (1 << NV4_PGRAPH_DEBUG_3_AUTOZFLUSH_D3D)                              // Point with zeta?
    | (1 << NV4_PGRAPH_DEBUG_3_POSTDITHER_2D)                               // Enable post-dithering operations [2D, <=16bpp]
    | (1 << NV4_PGRAPH_DEBUG_3_POSTDITHER_3D)                               // Enable post-dithering operations [3D, <=16bpp]
    // Nvidia for some reason define this as both on and off so i assume it is on
    | (1 << NV4_PGRAPH_DEBUG_3_PREDITHER_2D)                                // Enable pre-dithering operations [2D, <=16bpp]
    | (1 << NV4_PGRAPH_DEBUG_3_PREDITHER_3D)                                // Enable pre-dithering operations [3D, <=16bpp]
    | (1 << NV4_PGRAPH_DEBUG_3_SLOT_CONFLICT_PTZ)                           // RAMHT slot conflict behaviour?
    | (1 << NV4_PGRAPH_DEBUG_3_SLOT_CONFLICT_D3D)                           // RAMHT slot conflict behaviour?
    | (1 << NV4_PGRAPH_DEBUG_3_EARLYZ_ABORT)                                // Early Z/stencil rejection enabled (Optimisation) https://therealmjp.github.io/posts/to-earlyz-or-not-to-earlyz/
    | (0 << NV4_PGRAPH_DEBUG_3_FORCE_CREAD)                                 // Don't force colour buffer reads  
    | (0 << NV4_PGRAPH_DEBUG_3_FORCE_ZREAD)                                 // Don't force Z buffer reads  
    | (1 << NV4_PGRAPH_DEBUG_3_FORMAT_CHECK)                                // Check format
    | (0 << NV4_PGRAPH_DEBUG_3_TRIEND_FLUSH)                                // Don't flush triangles at end of rendering
    | (1 << NV4_PGRAPH_DEBUG_3_DMA_CHECK)                                   // Check DMA pushbuffer is valid
    | (1 << NV4_PGRAPH_DEBUG_3_DATA_CHECK)                                  // Check DMA pushbuffer data is valid
    | (0 << NV4_PGRAPH_DEBUG_3_DATA_CHECK_FAIL)                             // ?
    | (0 << NV4_PGRAPH_DEBUG_3_RAMREADBACK)                                 // Don't read back RAM (optimisation?)
    | (1 << NV4_PGRAPH_DEBUG_3_STATE_CHECK)                                 // Check state is valid
    ;

    // NV4 driver initialises in DX3 mode.
    Logging_Write(LOG_LEVEL_DEBUG, "NV4_InitGraph is initialising graphics hardware...\n");

    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_0, kernel_gpu->nv4_pgraph.debug_0);
    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_1, kernel_gpu->nv4_pgraph.debug_1);
    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_2, kernel_gpu->nv4_pgraph.debug_2);
    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_3, kernel_gpu->nv4_pgraph.debug_3);


    return true; 
}

bool NV4_ResetGraph()
{
    /* Now reset. */
    kernel_gpu->nv4_pgraph.debug_0 |= (1 << NV4_PGRAPH_DEBUG_0_STATE_RESET);
    kernel_gpu->nv4_pgraph.debug_1 |= (1 << NV4_PGRAPH_DEBUG_1_DMA_ACTIVITY_CANCEL);
    kernel_gpu->nv4_pgraph.debug_1 |= (1 << NV4_PGRAPH_DEBUG_1_VOLATILE_RESET);

    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_0, kernel_gpu->nv4_pgraph.debug_0);
    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_1, kernel_gpu->nv4_pgraph.debug_1);

    kernel_gpu->nv4_pgraph.debug_0 &= ~(1 << NV4_PGRAPH_DEBUG_0_STATE_RESET);
    kernel_gpu->nv4_pgraph.debug_1 &= ~(1 << NV4_PGRAPH_DEBUG_1_DMA_ACTIVITY_CANCEL);
    kernel_gpu->nv4_pgraph.debug_1 &= ~(1 << NV4_PGRAPH_DEBUG_1_VOLATILE_RESET);

    /* Leave reset */
    
    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_0, kernel_gpu->nv4_pgraph.debug_0);
    NV_WriteMMIO32(NV4_PGRAPH_DEBUG_1, kernel_gpu->nv4_pgraph.debug_1);

    /* Reset current grobj */

    NV_WriteMMIO32(NV4_PGRAPH_CTX_SWITCH1, 0x0);
    NV_WriteMMIO32(NV4_PGRAPH_CTX_SWITCH2, 0x0);
    NV_WriteMMIO32(NV4_PGRAPH_CTX_SWITCH3, 0x0);
    NV_WriteMMIO32(NV4_PGRAPH_CTX_SWITCH4, 0x0);

    NV_WriteMMIO32(NV4_PGRAPH_CTX_CONTROL, CTX_CONTROL_DEFAULT_UNLOADED);

    /* Enable FIFO access */
    NV_WriteMMIO32(NV4_PGRAPH_FIFO, NV4_PGRAPH_FIFO_ACCESS_ENABLED);
    /* Load current channel here */
}