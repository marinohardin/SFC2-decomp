#define STRICT
#include <windows.h>
#include <ddraw.h>
#include <d3dtypes.h>

// PORT: the compiler's own headers are DirectX 5; the DirectX 6 surface description this unit
// fills is not vendored, so it is written here from the documented DirectX 6 layout.
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

// FUNCTION: STARFLEETOP 0x006a94a0
VOID D3DUtil_InitSurfaceDesc( DDSURFACEDESC2& ddsd, DWORD dwFlags, DWORD dwCaps )
{
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize                 = sizeof(ddsd);
    ddsd.dwFlags                = dwFlags;
    ddsd.ddsCaps.dwCaps         = dwCaps;
    ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
}

// FUNCTION: STARFLEETOP 0x006a94d0
VOID D3DUtil_InitViewport( D3DVIEWPORT2& vp, DWORD dwWidth, DWORD dwHeight )
{
    ZeroMemory( &vp, sizeof(D3DVIEWPORT2) );
    vp.dwSize       = sizeof(D3DVIEWPORT2);
    vp.dwWidth      = dwWidth;
    vp.dwHeight     = dwHeight;
    vp.dvMaxZ       = 1.0f;

    vp.dvClipX      = -1.0f;
    vp.dvClipWidth  = 2.0f;
    vp.dvClipY      = 1.0f;
    vp.dvClipHeight = 2.0f;
}
