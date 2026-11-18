#ifndef UNSORTED_TDDSURFACE_H
#define UNSORTED_TDDSURFACE_H

#include <ddraw.h>
#include "dmp3d/dx6/ddraw6.h"

// The name is the RTTI's. HYPOTHESIS: a wrapper around one DirectDraw surface; 0x74 bytes
// (tDDSurfMgr::mInitBuffers allocates that much). Only the members its callers use are declared.
class tDDSurface
{
public:
    tDDSurface();
    // HYPOTHESIS: attaches the surface and its description.
    void fn_006a7760(LPDIRECTDRAWSURFACE surface, DDSURFACEDESC2* desc, int a3);

    char m000[0x74];
};

#endif
