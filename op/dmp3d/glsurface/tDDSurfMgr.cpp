#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "unsorted/tDDSurface.h"
// HYPOTHESIS: a nesting counter (0x00c7c608). The window handle (0x00c7c604) and the window
// procedure mWndProcStub replaces (0x00c7c610) are tDDSurfMgr's statics (SDK symbols).
extern int g_00c7c608; EXTERN_C const GUID IID_IDirectDraw4;    // the toolchain's ddraw.h predates it

// HYPOTHESIS: layout from the offsets this unit's functions use; only what they touch is named.
class tDDSurfMgr
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();

    bool fn_006b3e10();
    bool fn_006b4090(int a1, int a2, int a3, int a4, int a5, bool a6, bool a7, int a8, HWND hwnd, int a10);
    bool mSetDisplayMode(int width, int height, int bpp);
    bool mRestoreDisplayMode();
    bool mInitBuffers(int a1);
protected: HRESULT mSetupPalette(); unsigned char mPause(unsigned char a1); public:
    // (mSetupPalette is 0x006b4a50 and mPause 0x006b4b10; both are protected in the SDK.)
protected: static HWND fHWnd; static WNDPROC fOldWndProc;
    static LRESULT CALLBACK mWndProcStub(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void mDDThrow(HRESULT hr, const char* file, int line, const char* format, ...);
    unsigned char mDDCheck(HRESULT hr, const char* file, int line, const char* format, ...); public:

    int m004;
    char m_pad8[0xc - 0x8];
    char m00c[0x200];
    char m20c[0x200];
    char m40c[0x60c - 0x40c];
    unsigned long fFlag0 : 1;
    unsigned long fFlag1 : 1;
    unsigned long fFlag2 : 1;
    unsigned long fFlag3 : 1;
    unsigned long fFlag4 : 1;
    char m_pad610[0x618 - 0x610];
    LPDIRECTDRAW fDD;
    LPDIRECTDRAW2 fDD2;
    tDDSurface* m620;
    DDSURFACEDESC2 m624;
    LPDIRECTDRAWSURFACE fDDFront;
    tDDSurface* m6a4;
    DDSURFACEDESC2 m6a8;
    int m724;
    int m728;
    LPDIRECTDRAWSURFACE m72c;
    int m730;
    DWORD fCoopLevel;
    int m738;
    int m73c;
    int m740;
    int m744;
    RECT m748;
    LPDIRECTDRAWPALETTE m758;
    int m75c;
    int m760;
    int m764;
};

// HYPOTHESIS: a guard that counts a call in progress; constructed and destroyed inline.
class t00c7c608Guard
{
public:
    t00c7c608Guard(int& count) : fCount(&count) { ++*fCount; }
    inline ~t00c7c608Guard();

    int* fCount;
};

// tDDSurfMgr::mDDThrow (a cdecl member, as its varargs make it) reports a failed HRESULT with
// "Line: %5d, Module: %s" and throws it; mDDCheck records it and returns false.


// HYPOTHESIS: returns the name of a DirectDraw/Direct3D error code ("D3DERR_...").
const char* D3DErrorCodeToString(HRESULT hr);

// MATCH-ONLY: the original's earlier lines are not recovered; the blank lines below keep
// mInitInterfaces on its original line numbers, which its error reports carry. (A #line
// past the end of the file leaves the compiler's listing without line records.)
























































































































































































































































































































































































































































































































































































// FUNCTION: STARFLEETOP 0x006b3e10
bool tDDSurfMgr::fn_006b3e10()
{
    if (m738)
        mRestoreDisplayMode();

    if (fDD2)
    {
        fDD2->Release();
        fDD2 = NULL;
    }
    if (fDD)
    {
        fDD->Release();
        fDD = NULL;
    }

    try
    {
        mDDThrow(DirectDrawCreate(NULL, &fDD, NULL), __FILE__, __LINE__, "tDDSurfMgr::mInitInterfaces(): DirectDrawCreate() failed");





        mDDThrow(fDD->QueryInterface(IID_IDirectDraw4, (void**)&fDD2), __FILE__, __LINE__, "tDDSurfMgr::mInitInterfaces(): Could not obtain DirectDraw2 interface");







        DWORD coop = fFlag0 ? (DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE) : DDSCL_NORMAL;

        mDDThrow(fDD->SetCooperativeLevel(fHWnd, coop), __FILE__, __LINE__, "tDDSurfMgr::mInitInterfaces(): Could not set coop level");
        fCoopLevel = coop;
    }
    catch (...)
    {
        if (m738)
            mRestoreDisplayMode();
        if (fDD2)
        {
            fDD2->Release();
            fDD2 = NULL;
        }
        if (fDD)
        {
            fDD->Release();
            fDD = NULL;
        }
        return false;
    }
    return true;
}

// FUNCTION: STARFLEETOP 0x006b3f80
void tDDSurfMgr::mDDThrow(HRESULT hr, const char* file, int line, const char* format, ...)
{
    if (hr == DD_OK)
        return;

    // MATCH-ONLY: the original loads the format into a register before the first sprintf.
    const char* fmt = format;
    sprintf(m00c, "Line: %5d, Module: %s", line, file);
    va_list args;
    va_start(args, format);
    vsprintf(m20c, fmt, args);
    va_end(args);
    strcpy(m40c, D3DErrorCodeToString(hr));
    throw hr;
}

// FUNCTION: STARFLEETOP 0x006b4010
// The non-throwing twin of mDDThrow; records the failure and returns false.
unsigned char tDDSurfMgr::mDDCheck(HRESULT hr, const char* file, int line, const char* format, ...)
{
    if (hr == DD_OK)
        return true;

    // MATCH-ONLY: the original loads the format into a register before the first sprintf.
    const char* fmt = format;
    sprintf(m00c, "Line: %5d, Module: %s", line, file);
    va_list args;
    va_start(args, format);
    vsprintf(m20c, fmt, args);
    va_end(args);
    strcpy(m40c, D3DErrorCodeToString(hr));
    return false;
}

// FUNCTION: STARFLEETOP 0x006b4090
bool tDDSurfMgr::fn_006b4090(int a1, int a2, int a3, int a4, int a5, bool a6, bool a7, int a8, HWND hwnd, int a10)
{
    vfunction9();

    if (!IsWindow(hwnd))
    {
        m004 = 0xc;
        return false;
    }

    RECT r;
    if (!GetWindowRect(hwnd, &r))
        return false;

    m760 = r.right - r.left;
    m764 = r.bottom - r.top;
    m724 = a2;
    m728 = a3;
    m73c = a1;
    m740 = a4;
    m744 = a5;
    fFlag0 = a6;
    fFlag3 = a7;

    fHWnd = hwnd;
    fOldWndProc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
    SetWindowLong(fHWnd, GWL_WNDPROC, (LONG)mWndProcStub);

    t00c7c608Guard guard(g_00c7c608);

    if (!fn_006b3e10())
    {
        vfunction9();
        return false;
    }

    POINT pt;
    pt.x = 0;
    pt.y = 0;
    GetClientRect(fHWnd, &m748);
    ClientToScreen(fHWnd, &pt);
    OffsetRect(&m748, pt.x, pt.y);

    HWND desktop = GetDesktopWindow();
    HDC dc = GetDC(desktop);
    int width = GetDeviceCaps(dc, HORZRES);
    int height = GetDeviceCaps(dc, VERTRES);
    int bpp = GetDeviceCaps(dc, BITSPIXEL);
    ReleaseDC(desktop, dc);

    if (bpp != a1 || width != a4 || height != a5)
    {
        if (!mSetDisplayMode(a4, a5, a1))
        {
            vfunction9();
            return false;
        }
        m738 = 1;
    }

    if (!mInitBuffers(a8))
    {
        vfunction9();
        return false;
    }

    m004 = 0;
    fFlag4 = 1;
    mPause(0);
    return true;
}

// The out-of-line copy is the one the unwind funclet of fn_006b4090 calls.
// FUNCTION: STARFLEETOP 0x006b4350
inline t00c7c608Guard::~t00c7c608Guard()
{
    --*fCount;
}

// MATCH-ONLY: the blank lines keep mSetDisplayMode's error reports on their original lines.
























































































































// FUNCTION: STARFLEETOP 0x006b4360
bool tDDSurfMgr::mSetDisplayMode(int width, int height, int bpp)
{
    int restore = !(fCoopLevel & DDSCL_EXCLUSIVE);
    if (restore)
    {
        HRESULT hr = fDD->SetCooperativeLevel(fHWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
        if (hr != DD_OK)
        {
            mDDCheck(hr, __FILE__, __LINE__, "tDDSurfMgr::mSetDisplayMode(): Could not set coop level");
            m004 = 2;
            return false;
        }
    }








    bool result = true;
    HRESULT hr = fDD->SetDisplayMode(width, height, bpp);
    if (hr != DD_OK)
    {
        mDDCheck(hr, __FILE__, __LINE__, "tDDSurfMgr::mSetDisplayMode(): Unable to set display mode to %dx%dx%dx", width, height, bpp);
        m004 = 2;
        result = false;
    }







    if (restore)
    {
        hr = fDD->SetCooperativeLevel(fHWnd, fCoopLevel);
        if (hr != DD_OK)
        {
            mDDCheck(hr, __FILE__, __LINE__, "tDDSurfMgr::mSetDisplayMode(): Could not restore coop level");
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x006b4460
bool tDDSurfMgr::mRestoreDisplayMode()
{
    if (!m738)
        return true;

    m738 = 0;
    int restore = !(fCoopLevel & DDSCL_EXCLUSIVE);
    if (restore)
    {
        HRESULT hr = fDD->SetCooperativeLevel(fHWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
        if (hr != DD_OK)
        {












            mDDCheck(hr, __FILE__, __LINE__, "tDDSurfMgr::mRestoreDisplayMode(): Could not set coop level");
            m004 = 2;
            return false;
        }
    }


    bool result = true;
    HRESULT hr = fDD->RestoreDisplayMode();
    if (hr != DD_OK)
    {

        mDDCheck(hr, __FILE__, __LINE__, "tDDSurfMgr::mRestoreDisplayMode(): Could not restore display mode");
        result = false;
    }





    if (restore)
    {
        hr = fDD->SetCooperativeLevel(fHWnd, fCoopLevel);
        if (hr != DD_OK)
        {
            mDDCheck(hr, __FILE__, __LINE__, "tDDSurfMgr::mSetDisplayMode(): Could not restore coop level");
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x006b4540
bool tDDSurfMgr::mInitBuffers(int a1)
{
    m004 = 2;
    LPDIRECTDRAWCLIPPER clipper = NULL;
    try
    {
        DDSURFACEDESC2 ddsd;
        if (fFlag0 && fFlag3)
        {
            memset(&ddsd, 0, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);
            ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY;
            ddsd.dwBackBufferCount = a1 ? a1 : 1;



















            HRESULT hr;
            while (ddsd.dwBackBufferCount > 1)
            {
                // HYPOTHESIS: the DirectDraw2 interface is handed the DirectX 6 description.
                hr = fDD2->CreateSurface((LPDDSURFACEDESC)&ddsd, &fDDFront, NULL);
                if (hr == DD_OK)
                    break;
                ddsd.dwBackBufferCount--;
            }
            m730 = ddsd.dwBackBufferCount;
            if (hr != DD_OK)
                mDDThrow(hr, __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Failed to create primary surface\n");








            DDSCAPS2 caps;
            memset(&caps, 0, sizeof(caps));
            caps.dwCaps = DDSCAPS_BACKBUFFER;
            mDDThrow(fDDFront->GetAttachedSurface((LPDDSCAPS)&caps, &m72c), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Could not obtain back buffer surface");
        }
        else
        {








            memset(&ddsd, 0, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);
            ddsd.dwFlags = DDSD_CAPS;
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
            mDDThrow(fDD2->CreateSurface((LPDDSURFACEDESC)&ddsd, &fDDFront, NULL), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Failed to create front surface");










            mDDThrow(DirectDrawCreateClipper(0, &clipper, NULL), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Failed to create clipper");





            mDDThrow(clipper->SetHWnd(0, fHWnd), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Clipper->SetHWnd() failed");





            mDDThrow(fDDFront->SetClipper(clipper), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): fDDFront->SetClipper() failed");
            clipper->Release();
            clipper = NULL;


















            memset(&ddsd, 0, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);
            ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
            ddsd.dwWidth = m724;
            ddsd.dwHeight = m728;
            ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
            HRESULT hr = fDD2->CreateSurface((LPDDSURFACEDESC)&ddsd, &m72c, NULL);
            if (hr != DD_OK)
            {
                mDDCheck(hr, __FILE__, __LINE__, "tD3DMgr::mInitD3DDevice(): Could not create back buffer\nin video memory");










                ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
                mDDThrow(fDD2->CreateSurface((LPDDSURFACEDESC)&ddsd, &m72c, NULL), __FILE__, __LINE__, "tD3DMgr::mInitD3DDevice(): Could not create back buffer\nin system memory");
            }
        }








        memset(&m624, 0, sizeof(m624));
        m624.dwSize = sizeof(m624);
        mDDThrow(fDDFront->GetSurfaceDesc((LPDDSURFACEDESC)&m624), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Could not obtain front surface description");





        memset(&m6a8, 0, sizeof(m6a8));
        m6a8.dwSize = sizeof(m6a8);
        mDDThrow(m72c->GetSurfaceDesc((LPDDSURFACEDESC)&m6a8), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Could not obtain back surface description");










        if (m624.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
        {
            HRESULT hr = mSetupPalette();
            if (hr != DD_OK)
                mDDThrow(hr, __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): Can't make palette\n");








            mDDThrow(fDDFront->SetPalette(m758), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): SetPalette() failed");




            mDDThrow(m72c->SetPalette(m758), __FILE__, __LINE__, "tDDSurfMgr::mInitBuffers(): SetPalette() failed");
        }
        m620 = new tDDSurface;
        if (!m620)
        {
            m004 = 4;
            throw E_OUTOFMEMORY;
        }
        m620->fn_006a7760(fDDFront, &m624, 0);
        m6a4 = new tDDSurface;
        if (!m6a4)
        {
            m004 = 4;
            throw E_OUTOFMEMORY;
        }
        m6a4->fn_006a7760(m72c, &m6a8, 0);
    }
    catch (...)
    {
        if (clipper)
            clipper->Release();
        if (m758)
        {
            m758->Release();
            m758 = NULL;
        }
        if (m72c)
        {
            m72c->Release();
            m72c = NULL;
        }
        if (fDDFront)
        {
            fDDFront->Release();
            fDDFront = NULL;
        }
        return false;
    }
    m004 = 0;
    return true;
}
