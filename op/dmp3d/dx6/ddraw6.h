#ifndef DMP3D_DX6_DDRAW6_H
#define DMP3D_DX6_DDRAW6_H

#include <ddraw.h>

// The original built against the DirectX 6 headers; the pinned compiler's are DirectX 5: the DirectX 6 surface description and caps
// are not vendored, so they are written here from the documented DirectX 6 layout.
// d3dframe.cpp and d3dutil.cpp still carry their own copies.
typedef struct _DDSCAPS2
{
    DWORD dwCaps;
    DWORD dwCaps2;
    DWORD dwCaps3;
    DWORD dwCaps4;
} DDSCAPS2;

typedef struct _DDSURFACEDESC2
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwHeight;
    DWORD dwWidth;
    union
    {
        LONG  lPitch;
        DWORD dwLinearSize;
    };
    DWORD dwBackBufferCount;
    union
    {
        DWORD dwMipMapCount;
        DWORD dwRefreshRate;
    };
    DWORD         dwAlphaBitDepth;
    DWORD         dwReserved;
    LPVOID        lpSurface;
    DDCOLORKEY    ddckCKDestOverlay;
    DDCOLORKEY    ddckCKDestBlt;
    DDCOLORKEY    ddckCKSrcOverlay;
    DDCOLORKEY    ddckCKSrcBlt;
    DDPIXELFORMAT ddpfPixelFormat;
    DDSCAPS2      ddsCaps;
    DWORD         dwTextureStage;
} DDSURFACEDESC2;

#endif
