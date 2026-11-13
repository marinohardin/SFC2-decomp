#define STRICT
#include <windows.h>
#include <ddraw.h>
#include <d3dtypes.h>

// PORT: the compiler's own headers are DirectX 5; the DirectX 6 headers this unit was built
// against are not vendored, so the declarations it needs are written here from the documented
// DirectX 6 layout. The interfaces declare only the methods this unit calls so far.
typedef struct _D3DTRANSFORMCAPS
{
    DWORD dwSize;
    DWORD dwCaps;
} D3DTRANSFORMCAPS;

typedef struct _D3DLIGHTINGCAPS
{
    DWORD dwSize;
    DWORD dwCaps;
    DWORD dwLightingModel;
    DWORD dwNumLights;
} D3DLIGHTINGCAPS;

typedef struct _D3DPrimCaps
{
    DWORD dwSize;
    DWORD dwMiscCaps;
    DWORD dwRasterCaps;
    DWORD dwZCmpCaps;
    DWORD dwSrcBlendCaps;
    DWORD dwDestBlendCaps;
    DWORD dwAlphaCmpCaps;
    DWORD dwShadeCaps;
    DWORD dwTextureCaps;
    DWORD dwTextureFilterCaps;
    DWORD dwTextureBlendCaps;
    DWORD dwTextureAddressCaps;
    DWORD dwStippleWidth;
    DWORD dwStippleHeight;
} D3DPRIMCAPS;

// The DirectX 6 device description: the DirectX 5 one plus the fields from dwMaxTextureRepeat on.
typedef struct _D3DDeviceDesc
{
    DWORD            dwSize;
    DWORD            dwFlags;
    D3DCOLORMODEL    dcmColorModel;
    DWORD            dwDevCaps;
    D3DTRANSFORMCAPS dtcTransformCaps;
    BOOL             bClipping;
    D3DLIGHTINGCAPS  dlcLightingCaps;
    D3DPRIMCAPS      dpcLineCaps;
    D3DPRIMCAPS      dpcTriCaps;
    DWORD            dwDeviceRenderBitDepth;
    DWORD            dwDeviceZBufferBitDepth;
    DWORD            dwMaxBufferSize;
    DWORD            dwMaxVertexCount;
    DWORD            dwMinTextureWidth;
    DWORD            dwMinTextureHeight;
    DWORD            dwMaxTextureWidth;
    DWORD            dwMaxTextureHeight;
    DWORD            dwMinStippleWidth;
    DWORD            dwMaxStippleWidth;
    DWORD            dwMinStippleHeight;
    DWORD            dwMaxStippleHeight;
    DWORD            dwMaxTextureRepeat;
    DWORD            dwMaxTextureAspectRatio;
    DWORD            dwMaxAnisotropy;
    D3DVALUE         dvGuardBandLeft;
    D3DVALUE         dvGuardBandTop;
    D3DVALUE         dvGuardBandRight;
    D3DVALUE         dvGuardBandBottom;
    D3DVALUE         dvExtentsAdjust;
    DWORD            dwStencilCaps;
    DWORD            dwFVFCaps;
    DWORD            dwTextureOpCaps;
    WORD             wMaxTextureBlendStages;
    WORD             wMaxSimultaneousTextures;
} D3DDEVICEDESC;

// The DirectX 6 interfaces, declared as far as this unit calls into them: the IUnknown methods
// for every object, and IDirectDraw4 up to SetCooperativeLevel.
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

typedef struct _D3DFINDDEVICESEARCH
{
    DWORD         dwSize;
    DWORD         dwFlags;
    BOOL          bHardware;
    D3DCOLORMODEL dcmColorModel;
    GUID          guid;
    DWORD         dwCaps;
    D3DPRIMCAPS   dpcPrimCaps;
} D3DFINDDEVICESEARCH;

typedef struct _D3DFINDDEVICERESULT
{
    DWORD         dwSize;
    GUID          guid;
    D3DDEVICEDESC ddHwDesc;
    D3DDEVICEDESC ddSwDesc;
} D3DFINDDEVICERESULT;

#define D3DFDS_GUID 0x00000002L
#define DDPF_STENCILBUFFER 0x00004000L
#define D3DPRASTERCAPS_ZBUFFERLESSHSR 0x00008000L
typedef HRESULT (CALLBACK* LPD3DENUMPIXELFORMATSCALLBACK)(DDPIXELFORMAT*, LPVOID);

struct IDirectDrawSurface4;
struct IDirect3DDevice3;
struct IDirect3DViewport3;
typedef HRESULT (FAR PASCAL *LPDDENUMMODESCALLBACK2)(struct _DDSURFACEDESC2 FAR*, LPVOID);
typedef HRESULT (FAR PASCAL *LPDDENUMSURFACESCALLBACK2)(IDirectDrawSurface4 FAR*,
                                                         struct _DDSURFACEDESC2 FAR*, LPVOID);

struct IDirectDraw4 : public IUnknown
{
    STDMETHOD(Compact)(THIS) PURE;
    STDMETHOD(CreateClipper)(THIS_ DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR*) PURE;
    STDMETHOD(CreatePalette)(THIS_ DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*,
                             IUnknown FAR*) PURE;
    STDMETHOD(CreateSurface)(THIS_ struct _DDSURFACEDESC2 FAR*, IDirectDrawSurface4 FAR* FAR*,
                             IUnknown FAR*) PURE;
    STDMETHOD(DuplicateSurface)(THIS_ IDirectDrawSurface4 FAR*,
                                IDirectDrawSurface4 FAR* FAR*) PURE;
    STDMETHOD(EnumDisplayModes)(THIS_ DWORD, struct _DDSURFACEDESC2 FAR*, LPVOID,
                                LPDDENUMMODESCALLBACK2) PURE;
    STDMETHOD(EnumSurfaces)(THIS_ DWORD, struct _DDSURFACEDESC2 FAR*, LPVOID,
                            LPDDENUMSURFACESCALLBACK2) PURE;
    STDMETHOD(FlipToGDISurface)(THIS) PURE;
    STDMETHOD(GetCaps)(THIS_ LPDDCAPS, LPDDCAPS) PURE;
    STDMETHOD(GetDisplayMode)(THIS_ struct _DDSURFACEDESC2 FAR*) PURE;
    STDMETHOD(GetFourCCCodes)(THIS_ LPDWORD, LPDWORD) PURE;
    STDMETHOD(GetGDISurface)(THIS_ IDirectDrawSurface4 FAR* FAR*) PURE;
    STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD) PURE;
    STDMETHOD(GetScanLine)(THIS_ LPDWORD) PURE;
    STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL) PURE;
    STDMETHOD(Initialize)(THIS_ GUID FAR*) PURE;
    STDMETHOD(RestoreDisplayMode)(THIS) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD) PURE;
    STDMETHOD(SetDisplayMode)(THIS_ DWORD, DWORD, DWORD, DWORD, DWORD) PURE;
};

struct IDirectDrawSurface4 : public IUnknown
{
    STDMETHOD(AddAttachedSurface)(THIS_ IDirectDrawSurface4*) PURE;
    STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT) PURE;
    STDMETHOD(Blt)(THIS_ LPRECT, IDirectDrawSurface4*, LPRECT, DWORD, LPDDBLTFX) PURE;
    STDMETHOD(BltBatch)(THIS_ LPDDBLTBATCH, DWORD, DWORD) PURE;
    STDMETHOD(BltFast)(THIS_ DWORD, DWORD, IDirectDrawSurface4*, LPRECT, DWORD) PURE;
    STDMETHOD(DeleteAttachedSurface)(THIS_ DWORD, IDirectDrawSurface4*) PURE;
    STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID, LPDDENUMSURFACESCALLBACK2) PURE;
    STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD, LPVOID, LPDDENUMSURFACESCALLBACK2) PURE;
    STDMETHOD(Flip)(THIS_ IDirectDrawSurface4*, DWORD) PURE;
    STDMETHOD(GetAttachedSurface)(THIS_ DDSCAPS2*, IDirectDrawSurface4**) PURE;
    STDMETHOD(GetBltStatus)(THIS_ DWORD) PURE;
    STDMETHOD(GetCaps)(THIS_ DDSCAPS2*) PURE;
    STDMETHOD(GetClipper)(THIS_ LPDIRECTDRAWCLIPPER*) PURE;
    STDMETHOD(GetColorKey)(THIS_ DWORD, LPDDCOLORKEY) PURE;
    STDMETHOD(GetDC)(THIS_ HDC*) PURE;
    STDMETHOD(GetFlipStatus)(THIS_ DWORD) PURE;
    STDMETHOD(GetOverlayPosition)(THIS_ LPLONG, LPLONG) PURE;
    STDMETHOD(GetPalette)(THIS_ LPDIRECTDRAWPALETTE*) PURE;
    STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT) PURE;
    STDMETHOD(GetSurfaceDesc)(THIS_ struct _DDSURFACEDESC2*) PURE;
    STDMETHOD(Initialize)(THIS_ LPDIRECTDRAW, struct _DDSURFACEDESC2*) PURE;
    STDMETHOD(IsLost)(THIS) PURE;
    STDMETHOD(Lock)(THIS_ LPRECT, struct _DDSURFACEDESC2*, DWORD, HANDLE) PURE;
    STDMETHOD(ReleaseDC)(THIS_ HDC) PURE;
    STDMETHOD(Restore)(THIS) PURE;
    STDMETHOD(SetClipper)(THIS_ LPDIRECTDRAWCLIPPER) PURE;
};

struct IDirect3D3 : public IUnknown
{
    STDMETHOD(EnumDevices)(THIS_ LPVOID, LPVOID) PURE;
    STDMETHOD(CreateLight)(THIS_ LPVOID, IUnknown*) PURE;
    STDMETHOD(CreateMaterial)(THIS_ LPVOID, IUnknown*) PURE;
    STDMETHOD(CreateViewport)(THIS_ IDirect3DViewport3**, IUnknown*) PURE;
    STDMETHOD(FindDevice)(THIS_ D3DFINDDEVICESEARCH*, D3DFINDDEVICERESULT*) PURE;
    STDMETHOD(CreateDevice)(THIS_ REFCLSID, IDirectDrawSurface4*, IDirect3DDevice3**,
                            IUnknown*) PURE;
    STDMETHOD(CreateVertexBuffer)(THIS_ LPVOID, LPVOID*, DWORD, IUnknown*) PURE;
    STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID, LPD3DENUMPIXELFORMATSCALLBACK, LPVOID) PURE;
};

struct IDirect3DDevice3 : public IUnknown
{
    STDMETHOD(GetCaps)(THIS_ D3DDEVICEDESC*, D3DDEVICEDESC*) PURE;
    STDMETHOD(GetStats)(THIS_ LPVOID) PURE;
    STDMETHOD(AddViewport)(THIS_ IDirect3DViewport3*) PURE;
    STDMETHOD(DeleteViewport)(THIS_ IDirect3DViewport3*) PURE;
    STDMETHOD(NextViewport)(THIS_ IDirect3DViewport3*, IDirect3DViewport3**, DWORD) PURE;
    STDMETHOD(EnumTextureFormats)(THIS_ LPVOID, LPVOID) PURE;
    STDMETHOD(BeginScene)(THIS) PURE;
    STDMETHOD(EndScene)(THIS) PURE;
    STDMETHOD(GetDirect3D)(THIS_ IDirect3D3**) PURE;
    STDMETHOD(SetCurrentViewport)(THIS_ IDirect3DViewport3*) PURE;
};

struct IDirect3DViewport3 : public IUnknown
{
    STDMETHOD(Initialize)(THIS_ LPVOID) PURE;
    STDMETHOD(GetViewport)(THIS_ LPD3DVIEWPORT) PURE;
    STDMETHOD(SetViewport)(THIS_ LPD3DVIEWPORT) PURE;
    STDMETHOD(TransformVertices)(THIS_ DWORD, LPD3DTRANSFORMDATA, DWORD, LPDWORD) PURE;
    STDMETHOD(LightElements)(THIS_ DWORD, LPD3DLIGHTDATA) PURE;
    STDMETHOD(SetBackground)(THIS_ D3DMATERIALHANDLE) PURE;
    STDMETHOD(GetBackground)(THIS_ LPD3DMATERIALHANDLE, LPBOOL) PURE;
    STDMETHOD(SetBackgroundDepth)(THIS_ LPDIRECTDRAWSURFACE) PURE;
    STDMETHOD(GetBackgroundDepth)(THIS_ LPDIRECTDRAWSURFACE*, LPBOOL) PURE;
    STDMETHOD(Clear)(THIS_ DWORD, LPD3DRECT, DWORD) PURE;
    STDMETHOD(AddLight)(THIS_ IUnknown*) PURE;
    STDMETHOD(DeleteLight)(THIS_ IUnknown*) PURE;
    STDMETHOD(NextLight)(THIS_ IUnknown*, IUnknown**, DWORD) PURE;
    STDMETHOD(GetViewport2)(THIS_ LPD3DVIEWPORT2) PURE;
    STDMETHOD(SetViewport2)(THIS_ LPD3DVIEWPORT2) PURE;
};

VOID D3DUtil_InitViewport( D3DVIEWPORT2& vp, DWORD dwWidth, DWORD dwHeight );

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define D3DFW_FULLSCREEN         0x00000001
#define D3DFWERR_INITIALIZATIONFAILED 0x82000000
#define D3DFW_ZBUFFER            0x00000004
#define D3DFWERR_NODIRECTDRAW    0x82000001
#define D3DFWERR_COULDNTSETCOOPLEVEL 0x82000002
// PORT: the DirectX 5 headers have no IDirectDraw4 interface identifier.
// {9C59509A-39BD-11D1-8C4A-00C04FD930C5}
static const GUID IID_IDirectDraw4 =
    { 0x9c59509a, 0x39bd, 0x11d1, { 0x8c, 0x4a, 0x00, 0xc0, 0x4f, 0xd9, 0x30, 0xc5 } };
#define D3DFWERR_NODIRECT3D      0x82000003
#define D3DFWERR_NO3DDEVICE      0x82000004
#define D3DFWERR_NOZBUFFER       0x82000005
// HYPOTHESIS: the sample framework's flag for a zbuffer with stencil; 0x8 asks for one here.
#define D3DFW_STENCILBUFFER      0x00000008
// PORT: the DirectX 5 headers have no IDirect3D3 interface identifier.
// {BB223240-E72B-11D0-A9B4-00AA00C0993E}
static const GUID IID_IDirect3D3 =
    { 0xbb223240, 0xe72b, 0x11d0, { 0xa9, 0xb4, 0x00, 0xaa, 0x00, 0xc0, 0x99, 0x3e } };
HRESULT WINAPI EnumZBufferFormatsCallback( DDPIXELFORMAT* pddpf, VOID* pddpfDesired );
#define D3DFWERR_NOVIEWPORT      0x82000006
#define D3DFWERR_NOPRIMARY       0x82000007
#define D3DFWERR_NOCLIPPER       0x82000008
#define D3DFWERR_BADDISPLAYMODE  0x82000009
#define D3DFWERR_NOBACKBUFFER    0x8200000a
#define D3DFWERR_NONZEROREFCOUNT 0x8200000b
#define D3DFWERR_NOTINITIALIZED  0x8200000e
// HYPOTHESIS: the framework's flag for a separate backbuffer; 0x2 asks for one here.
#define D3DFW_BACKBUFFER         0x00000002
// PORT: the DirectX 5 headers have no DDSDM_STANDARDVGAMODE.
#define DDSDM_STANDARDVGAMODE    0x00000001
VOID D3DUtil_InitSurfaceDesc( DDSURFACEDESC2& ddsd, DWORD dwFlags=0, DWORD dwCaps=0 );
#define D3DFWERR_INVALIDMODE     0x8200000d
typedef IDirectDraw4*        LPDIRECTDRAW4;
typedef IDirectDrawSurface4* LPDIRECTDRAWSURFACE4;
typedef IDirect3D3*          LPDIRECT3D3;
typedef IDirect3DDevice3*    LPDIRECT3DDEVICE3;
typedef IDirect3DViewport3*  LPDIRECT3DVIEWPORT3;

// The framework class of the DirectX 6 SDK samples, as linked into the dmp3D library.
class CD3DFramework
{
    // Internal variables for the framework class
    HWND                 m_hWnd;               // The window object
    BOOL                 m_bIsFullscreen;      // Fullscreen vs. windowed
    DWORD                m_dwRenderWidth;      // Dimensions of the render target
    DWORD                m_dwRenderHeight;
    RECT                 m_rcScreenRect;       // Screen rect for window
    RECT                 m_rcViewportRect;     // Offscreen rect for VPort
    LPDIRECTDRAWSURFACE4 m_pddsFrontBuffer;    // The primary surface
    LPDIRECTDRAWSURFACE4 m_pddsBackBuffer;     // The backbuffer surface
    LPDIRECTDRAWSURFACE4 m_pddsRenderTarget;   // The render target surface
    LPDIRECTDRAWSURFACE4 m_pddsZBuffer;        // The zbuffer surface
    LPDIRECT3DDEVICE3    m_pd3dDevice;         // The D3D device
    LPDIRECT3DVIEWPORT3  m_pvViewport;         // The D3D viewport
    LPDIRECTDRAW4        m_pDD;                // The DirectDraw object
    LPDIRECT3D3          m_pD3D;               // The Direct3D object
    D3DDEVICEDESC        m_ddDeviceDesc;
    DWORD                m_dwDeviceMemType;
    DDPIXELFORMAT        m_ddpfZBuffer;

    HRESULT CreateEnvironment( GUID*, GUID*, DDSURFACEDESC2*, DWORD );
    HRESULT CreateDirectDraw( GUID*, DWORD );
    HRESULT CreateDirect3D( GUID*, DWORD );
    HRESULT CreateBuffers( DDSURFACEDESC2*, DWORD );
    HRESULT CreateZBuffer();

public:
    CD3DFramework();
    HRESULT DestroyObjects();
    HRESULT Initialize( HWND, GUID*, GUID*, DDSURFACEDESC2*, DWORD );
    HRESULT ShowFrame();
    HRESULT FlipToGDISurface( BOOL bDrawFrame );
    HRESULT RestoreSurfaces();
    VOID    Move( INT x, INT y );
};

// FUNCTION: STARFLEETOP 0x006a7c20
CD3DFramework::CD3DFramework()
{
    m_hWnd             = NULL;
    m_bIsFullscreen    = FALSE;
    m_dwRenderWidth    = 0L;
    m_dwRenderHeight   = 0L;
    m_pddsFrontBuffer  = NULL;
    m_pddsBackBuffer   = NULL;
    m_pddsRenderTarget = NULL;
    m_pddsZBuffer      = NULL;
    m_pd3dDevice       = NULL;
    m_pvViewport       = NULL;
    m_pDD              = NULL;
    m_pD3D             = NULL;
    m_dwDeviceMemType  = NULL;
}

// FUNCTION: STARFLEETOP 0x006a7c60
HRESULT CD3DFramework::DestroyObjects()
{
    LONG nDD  = 0L;
    LONG nD3D = 0L;

    SAFE_RELEASE( m_pvViewport );

    if( m_pd3dDevice )
        nD3D = m_pd3dDevice->Release();
    m_pd3dDevice = NULL;

    // In windowed mode, release the explicitly created backbuffer.
    if( FALSE == m_bIsFullscreen )
        SAFE_RELEASE( m_pddsBackBuffer );
    SAFE_RELEASE( m_pddsRenderTarget );
    SAFE_RELEASE( m_pddsZBuffer );
    SAFE_RELEASE( m_pddsFrontBuffer );
    SAFE_RELEASE( m_pD3D );

    if( m_pDD )
    {
        m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
        nDD = m_pDD->Release();
    }
    m_pDD = NULL;

    // Both reference counts must reach zero.
    if( nDD || nD3D )
        return D3DFWERR_NONZEROREFCOUNT;

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a7d20
HRESULT CD3DFramework::Initialize( HWND hWnd, GUID* pDriverGUID, GUID* pDeviceGUID,
                                   DDSURFACEDESC2* pMode, DWORD dwFlags )
{
    HRESULT hr;

    // Check params. A NULL mode is valid for windowed modes only.
    if( ( NULL==hWnd ) || ( NULL==pMode && (dwFlags&D3DFW_FULLSCREEN) ) )
        return E_INVALIDARG;

    // Setup state for windowed/fullscreen mode
    m_hWnd          = hWnd;
    m_bIsFullscreen = ( dwFlags & D3DFW_FULLSCREEN ) ? TRUE : FALSE;

    // Create the D3D objects
    if( FAILED( hr = CreateEnvironment( pDriverGUID, pDeviceGUID, pMode, dwFlags ) ) )
    {
        DestroyObjects();
        if( E_FAIL == hr )
            hr = D3DFWERR_INITIALIZATIONFAILED;
    }

    return hr;
}

// FUNCTION: STARFLEETOP 0x006a7d90
HRESULT CD3DFramework::CreateEnvironment( GUID* pDriverGUID, GUID* pDeviceGUID,
                                          DDSURFACEDESC2* pMode, DWORD dwFlags )
{
    HRESULT hr;

    // Create the DDraw object
    if( FAILED( hr = CreateDirectDraw( pDriverGUID, dwFlags ) ) )
        return hr;

    // Create the Direct3D object
    if( pDeviceGUID )
        if( FAILED( hr = CreateDirect3D( pDeviceGUID, dwFlags ) ) )
            return hr;

    // Create the front and back buffers, and attach a clipper
    if( FAILED( hr = CreateBuffers( pMode, dwFlags ) ) )
        return hr;

    // Without a device GUID the app wants 2D only
    if( pDeviceGUID )
    {
        // Create and attach the zbuffer
        if( dwFlags & D3DFW_ZBUFFER )
            if( FAILED( hr = CreateZBuffer() ) )
                return hr;

        // A palettized display mode is not supported
        DDSURFACEDESC2 ddsd;
        ddsd.dwSize = sizeof(DDSURFACEDESC2);
        m_pDD->GetDisplayMode( &ddsd );
        if( ddsd.ddpfPixelFormat.dwRGBBitCount <= 8 )
            return D3DFWERR_INVALIDMODE;

        // Create the 3D device
        if( FAILED( m_pD3D->CreateDevice( *pDeviceGUID, m_pddsRenderTarget,
                                          &m_pd3dDevice, NULL ) ) )
            return D3DFWERR_NO3DDEVICE;

        // Create the viewport and attach it to the device
        D3DVIEWPORT2 vp;
        D3DUtil_InitViewport( vp, m_dwRenderWidth, m_dwRenderHeight );

        if( FAILED( m_pD3D->CreateViewport( &m_pvViewport, NULL ) ) ||
            FAILED( m_pd3dDevice->AddViewport( m_pvViewport ) ) ||
            FAILED( m_pvViewport->SetViewport2( &vp ) ) ||
            FAILED( m_pd3dDevice->SetCurrentViewport( m_pvViewport ) ) )
            return D3DFWERR_NOVIEWPORT;
    }

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a7ee0
HRESULT CD3DFramework::CreateDirectDraw( GUID* pDriverGUID, DWORD dwFlags )
{
    // Create the DirectDraw interface, and query for the DD4 interface
    LPDIRECTDRAW pDD;
    if( FAILED( DirectDrawCreate( pDriverGUID, &pDD, NULL ) ) )
        return D3DFWERR_NODIRECTDRAW;

    if( FAILED( pDD->QueryInterface( IID_IDirectDraw4, (VOID**)&m_pDD ) ) )
    {
        pDD->Release();
        return D3DFWERR_NODIRECTDRAW;
    }
    pDD->Release();

    // Set the Windows cooperative level
    DWORD dwCoopFlags = DDSCL_NORMAL;
    if( m_bIsFullscreen )
        dwCoopFlags = DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN;

    // HYPOTHESIS: 0x20 and 0x10 are framework flags added for this game (no name in the exe or
    // the SDK); 0x20 keeps the window untouched, and unless 0x10 is set the cooperative level
    // gets 0x800, which takes the place of the sample's DDSCL_FPUSETUP.
    if( dwFlags & 0x00000020 )
        dwCoopFlags |= DDSCL_NOWINDOWCHANGES;

    if( 0L == ( dwFlags & 0x00000010 ) )
        dwCoopFlags |= 0x00000800L;

    if( FAILED( m_pDD->SetCooperativeLevel( m_hWnd, dwCoopFlags ) ) )
        return D3DFWERR_COULDNTSETCOOPLEVEL;

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a7f80
HRESULT CD3DFramework::CreateDirect3D( GUID* pDeviceGUID, DWORD dwFlags )
{
    // Query DirectDraw for access to Direct3D
    if( FAILED( m_pDD->QueryInterface( IID_IDirect3D3, (VOID**)&m_pD3D ) ) )
        return D3DFWERR_NODIRECT3D;

    // Use the FindDevice call to get the device description
    D3DFINDDEVICESEARCH search;
    D3DFINDDEVICERESULT result;
    ZeroMemory( &result, sizeof(D3DFINDDEVICERESULT) );
    ZeroMemory( &search, sizeof(D3DFINDDEVICESEARCH) );
    result.dwSize  = sizeof(D3DFINDDEVICERESULT);
    search.dwSize  = sizeof(D3DFINDDEVICESEARCH);
    search.dwFlags = D3DFDS_GUID;
    search.guid    = *pDeviceGUID;

    if( FAILED( m_pD3D->FindDevice( &search, &result ) ) )
        return D3DFWERR_NODIRECT3D;

    // Keep the device description, and note where the device's surfaces must live
    if( 0L == result.ddHwDesc.dwFlags )
    {
        m_dwDeviceMemType = DDSCAPS_SYSTEMMEMORY;
        memcpy( &m_ddDeviceDesc, &result.ddSwDesc, sizeof(D3DDEVICEDESC) );
    }
    else
    {
        m_dwDeviceMemType = DDSCAPS_VIDEOMEMORY;
        memcpy( &m_ddDeviceDesc, &result.ddHwDesc, sizeof(D3DDEVICEDESC) );
    }

    // Find a zbuffer pixel format for the device, with a stencil buffer if one is wanted
    ZeroMemory( &m_ddpfZBuffer, sizeof(DDPIXELFORMAT) );
    if( dwFlags & D3DFW_STENCILBUFFER )
        m_ddpfZBuffer.dwFlags = DDPF_ZBUFFER|DDPF_STENCILBUFFER;
    else
        m_ddpfZBuffer.dwFlags = DDPF_ZBUFFER;

    m_pD3D->EnumZBufferFormats( *pDeviceGUID, EnumZBufferFormatsCallback,
                                (VOID*)&m_ddpfZBuffer );

    if( sizeof(DDPIXELFORMAT) != m_ddpfZBuffer.dwSize )
        return D3DFWERR_NOZBUFFER;

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a8110
HRESULT CD3DFramework::CreateBuffers( DDSURFACEDESC2* pddsd, DWORD dwFlags )
{
    HRESULT hr;
    DDSURFACEDESC2 ddsd;

    if( dwFlags & D3DFW_FULLSCREEN )
    {
        // The render target covers the whole screen
        SetRect( &m_rcViewportRect, 0, 0, pddsd->dwWidth, pddsd->dwHeight );
        m_rcScreenRect   = m_rcViewportRect;
        m_dwRenderWidth  = m_rcViewportRect.right;
        m_dwRenderHeight = m_rcViewportRect.bottom;

        // 320x200x8 is the standard VGA mode 13h
        DWORD dwModeFlags = 0;
        if( 320 == m_dwRenderWidth && 200 == m_dwRenderHeight &&
            8 == pddsd->ddpfPixelFormat.dwRGBBitCount )
            dwModeFlags = DDSDM_STANDARDVGAMODE;

        // Set the display mode
        if( FAILED( m_pDD->SetDisplayMode( m_dwRenderWidth, m_dwRenderHeight,
                                           pddsd->ddpfPixelFormat.dwRGBBitCount,
                                           pddsd->dwRefreshRate, dwModeFlags ) ) )
            return D3DFWERR_BADDISPLAYMODE;

        // Setup to create the primary surface, with a backbuffer if one is wanted
        D3DUtil_InitSurfaceDesc( ddsd, DDSD_CAPS );
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_3DDEVICE;
        if( dwFlags & D3DFW_BACKBUFFER )
        {
            ddsd.dwFlags          |= DDSD_BACKBUFFERCOUNT;
            ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE|DDSCAPS_3DDEVICE|
                                     DDSCAPS_FLIP|DDSCAPS_COMPLEX;
            ddsd.dwBackBufferCount = 1;
        }

        // Create the primary surface
        if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
        {
            if( hr != DDERR_OUTOFVIDEOMEMORY )
                return D3DFWERR_NOPRIMARY;
            return hr;
        }

        // Get the backbuffer, which was created along with the primary
        if( dwFlags & D3DFW_BACKBUFFER )
        {
            DDSCAPS2 ddscaps;
            ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
            if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps,
                                                                    &m_pddsBackBuffer ) ) )
            {
                if( hr != DDERR_OUTOFVIDEOMEMORY )
                    return D3DFWERR_NOBACKBUFFER;
                return hr;
            }
        }
    }
    else
    {
        // Get the dimensions of the viewport and screen bounds
        GetClientRect( m_hWnd, &m_rcViewportRect );
        GetClientRect( m_hWnd, &m_rcScreenRect );
        ClientToScreen( m_hWnd, (POINT*)&m_rcScreenRect.left );
        ClientToScreen( m_hWnd, (POINT*)&m_rcScreenRect.right );
        m_dwRenderWidth  = m_rcViewportRect.right;
        m_dwRenderHeight = m_rcViewportRect.bottom;

        // Create the primary surface
        D3DUtil_InitSurfaceDesc( ddsd, DDSD_CAPS );
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        if( 0L == ( dwFlags & D3DFW_BACKBUFFER ) )
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_3DDEVICE;

        if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
        {
            if( hr != DDERR_OUTOFVIDEOMEMORY )
                return D3DFWERR_NOPRIMARY;
            return hr;
        }

        // Create a clipper object and attach it to the primary surface
        LPDIRECTDRAWCLIPPER pcClipper;
        if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
            return D3DFWERR_NOCLIPPER;

        pcClipper->SetHWnd( 0, m_hWnd );
        m_pddsFrontBuffer->SetClipper( pcClipper );
        SAFE_RELEASE( pcClipper );

        if( dwFlags & D3DFW_BACKBUFFER )
        {
            // Create a backbuffer to render into
            ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
            ddsd.dwWidth        = m_dwRenderWidth;
            ddsd.dwHeight       = m_dwRenderHeight;
            ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_3DDEVICE;

            if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) )
            {
                if( hr != DDERR_OUTOFVIDEOMEMORY )
                    return D3DFWERR_NOBACKBUFFER;
                return hr;
            }
        }
        else
        {
            // Without a backbuffer the viewport is the window on the screen
            ClientToScreen( m_hWnd, (POINT*)&m_rcViewportRect.left );
            ClientToScreen( m_hWnd, (POINT*)&m_rcViewportRect.right );
        }
    }

    // Render into the backbuffer when there is one, otherwise into the primary
    if( dwFlags & D3DFW_BACKBUFFER )
        m_pddsRenderTarget = m_pddsBackBuffer;
    else
        m_pddsRenderTarget = m_pddsFrontBuffer;
    m_pddsRenderTarget->AddRef();

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a83f0
HRESULT CD3DFramework::CreateZBuffer()
{
    HRESULT hr;

    // Devices with hidden surface removal of their own need no zbuffer
    if( m_ddDeviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
        return S_OK;

    // Setup the surface desc for the zbuffer, the size of the render target
    DDSURFACEDESC2 ddsd;
    D3DUtil_InitSurfaceDesc( ddsd, DDSD_WIDTH|DDSD_HEIGHT|DDSD_CAPS|DDSD_PIXELFORMAT );
    ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | m_dwDeviceMemType;
    ddsd.dwWidth        = m_dwRenderWidth;
    ddsd.dwHeight       = m_dwRenderHeight;
    memcpy( &ddsd.ddpfPixelFormat, &m_ddpfZBuffer, sizeof(DDPIXELFORMAT) );

    // Create and attach a z-buffer
    if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsZBuffer, NULL ) ) )
    {
        if( hr != DDERR_OUTOFVIDEOMEMORY )
            return D3DFWERR_NOZBUFFER;
        return hr;
    }

    if( FAILED( m_pddsRenderTarget->AddAttachedSurface( m_pddsZBuffer ) ) )
        return D3DFWERR_NOZBUFFER;

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a84b0
HRESULT CD3DFramework::ShowFrame()
{
    if( NULL == m_pddsFrontBuffer )
        return D3DFWERR_NOTINITIALIZED;

    // Without a backbuffer there is nothing to present; report whether the
    // primary surface has been lost
    if( NULL == m_pddsBackBuffer )
        return m_pddsFrontBuffer->IsLost();

    // In fullscreen mode, flip the backbuffer to the front
    if( m_bIsFullscreen )
        return m_pddsFrontBuffer->Flip( NULL, DDFLIP_WAIT );

    // In windowed mode, blit the backbuffer to the window on the screen
    return m_pddsFrontBuffer->Blt( &m_rcScreenRect, m_pddsBackBuffer,
                                   &m_rcViewportRect, DDBLT_WAIT, NULL );
}

// FUNCTION: STARFLEETOP 0x006a8500
HRESULT CD3DFramework::FlipToGDISurface( BOOL bDrawFrame )
{
    if( m_pDD && m_bIsFullscreen )
    {
        m_pDD->FlipToGDISurface();

        if( bDrawFrame )
        {
            DrawMenuBar( m_hWnd );
            RedrawWindow( m_hWnd, NULL, NULL, RDW_FRAME );
        }
    }

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a8540
HRESULT CD3DFramework::RestoreSurfaces()
{
    // Restore the primary surface
    if( m_pddsFrontBuffer )
        if( m_pddsFrontBuffer->IsLost() )
            m_pddsFrontBuffer->Restore();

    // Restore the back buffer surface
    if( m_pddsBackBuffer )
        if( m_pddsBackBuffer->IsLost() )
            m_pddsBackBuffer->Restore();

    // Restore the z-buffer surface
    if( m_pddsZBuffer )
        if( m_pddsZBuffer->IsLost() )
            m_pddsZBuffer->Restore();

    return S_OK;
}

// FUNCTION: STARFLEETOP 0x006a85a0
VOID CD3DFramework::Move( INT x, INT y )
{
    if( FALSE == m_bIsFullscreen )
    {
        SetRect( &m_rcScreenRect, x, y, x + m_dwRenderWidth, y + m_dwRenderHeight );

        // Without a back buffer, render straight to the window's screen rect
        if( NULL == m_pddsBackBuffer )
            m_rcViewportRect = m_rcScreenRect;
    }
}
