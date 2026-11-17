#include <windows.h>
#include <ddraw.h>

// The original built against the DirectX 6 headers; the pinned compiler's are DirectX 5: they have no IDirectDrawSurface4. Only its
// IUnknown methods are used here.
typedef IUnknown* LPDIRECTDRAWSURFACE4;
DEFINE_GUID( IID_IDirectDrawSurface4, 0x0B2B8630,0xAD35,0x11D0,0x8E,0xA6,0x00,0x60,0x97,0x97,0xEA,0x5B );

typedef HRESULT(WINAPI * DIRECTDRAWCREATE)( GUID*, LPDIRECTDRAW*, IUnknown* );
// HYPOTHESIS: DirectInputCreate is only looked up, never called, so its
// interface pointer type is left opaque.
typedef HRESULT(WINAPI * DIRECTINPUTCREATE)( HINSTANCE, DWORD, VOID**, IUnknown* );

// FUNCTION: STARFLEETOP 0x006ad410
VOID GetDXVersion( DWORD* pdwDXVersion, DWORD* pdwDXPlatform )
{
    HRESULT              hr;
    HINSTANCE            DDHinst = 0;
    HINSTANCE            DIHinst = 0;
    LPDIRECTDRAW         pDDraw  = 0;
    LPDIRECTDRAW2        pDDraw2 = 0;
    DIRECTDRAWCREATE     DirectDrawCreate   = 0;
    DIRECTINPUTCREATE    DirectInputCreate  = 0;
    OSVERSIONINFO        osVer;
    LPDIRECTDRAWSURFACE  pSurf  = 0;
    LPDIRECTDRAWSURFACE3 pSurf3 = 0;
    LPDIRECTDRAWSURFACE4 pSurf4 = 0;

    // First get the windows platform
    osVer.dwOSVersionInfoSize = sizeof(osVer);
    if( !GetVersionEx( &osVer ) )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        return;
    }

    if( osVer.dwPlatformId == VER_PLATFORM_WIN32_NT )
    {
        (*pdwDXPlatform) = VER_PLATFORM_WIN32_NT;

        if( osVer.dwMajorVersion < 4 )
        {
            (*pdwDXPlatform) = 0;
            return;
        }

        if( osVer.dwMajorVersion == 4 )
        {
            (*pdwDXVersion) = 0x200;

            DIHinst = LoadLibrary( "DINPUT.DLL" );
            if( DIHinst == 0 )
            {
                OutputDebugString( "Couldn't LoadLibrary DInput\r\n" );
                return;
            }

            DirectInputCreate = (DIRECTINPUTCREATE)GetProcAddress( DIHinst,
                                                                 "DirectInputCreateA" );
            FreeLibrary( DIHinst );

            if( DirectInputCreate == 0 )
            {
                OutputDebugString( "Couldn't GetProcAddress DInputCreate\r\n" );
                return;
            }

            (*pdwDXVersion) = 0x300;
            return;
        }
    }
    else
    {
        (*pdwDXPlatform) = VER_PLATFORM_WIN32_WINDOWS;
    }

    DDHinst = LoadLibrary( "DDRAW.DLL" );
    if( DDHinst == 0 )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        FreeLibrary( DDHinst );
        return;
    }

    DirectDrawCreate = (DIRECTDRAWCREATE)GetProcAddress( DDHinst, "DirectDrawCreate" );
    if( DirectDrawCreate == 0 )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        FreeLibrary( DDHinst );
        OutputDebugString( "Couldn't LoadLibrary DDraw\r\n" );
        return;
    }

    hr = DirectDrawCreate( NULL, &pDDraw, NULL );
    if( FAILED(hr) )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        FreeLibrary( DDHinst );
        OutputDebugString( "Couldn't create DDraw\r\n" );
        return;
    }

    (*pdwDXVersion) = 0x100;

    hr = pDDraw->QueryInterface( IID_IDirectDraw2, (VOID**)&pDDraw2 );
    if( FAILED(hr) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        OutputDebugString( "Couldn't QI DDraw2\r\n" );
        return;
    }

    pDDraw2->Release();
    (*pdwDXVersion) = 0x200;

    DIHinst = LoadLibrary( "DINPUT.DLL" );
    if( DIHinst == 0 )
    {
        OutputDebugString( "Couldn't LoadLibrary DInput\r\n" );
        pDDraw->Release();
        FreeLibrary( DDHinst );
        return;
    }

    DirectInputCreate = (DIRECTINPUTCREATE)GetProcAddress( DIHinst,
                                                        "DirectInputCreateA" );
    FreeLibrary( DIHinst );
    if( DirectInputCreate == 0 )
    {
        FreeLibrary( DDHinst );
        pDDraw->Release();
        OutputDebugString( "Couldn't GetProcAddress DInputCreate\r\n" );
        return;
    }

    (*pdwDXVersion) = 0x300;

    DDSURFACEDESC ddsd;
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    hr = pDDraw->SetCooperativeLevel( NULL, DDSCL_NORMAL );
    if( FAILED(hr) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        (*pdwDXVersion) = 0;
        OutputDebugString( "Couldn't Set coop level\r\n" );
        return;
    }

    hr = pDDraw->CreateSurface( &ddsd, &pSurf, NULL );
    if( FAILED(hr) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        *pdwDXVersion = 0;
        OutputDebugString( "Couldn't CreateSurface\r\n" );
        return;
    }

    if( FAILED( pSurf->QueryInterface( IID_IDirectDrawSurface3,
                                       (VOID**)&pSurf3 ) ) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        return;
    }

    (*pdwDXVersion) = 0x500;

    if( FAILED( pSurf->QueryInterface( IID_IDirectDrawSurface4,
                                       (VOID**)&pSurf4 ) ) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        return;
    }

    (*pdwDXVersion) = 0x600;
    pSurf->Release();
    pDDraw->Release();
    FreeLibrary( DDHinst );
    return;
}
