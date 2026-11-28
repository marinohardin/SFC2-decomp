//	tApp3dBase.h: tApp3dBase, the 3D application base, and the view and view settings it uses.
// Evidence:  class (original file unknown)

#ifndef tApp3dBase_h
#define tApp3dBase_h 1

#include "unsorted/tApplication.h"

#include "unsorted/tGLRgbQuad.h"

class tBmpSurf;
class tGLLightDesc;
class tGLSurface;
class tTexInfo;
struct tGLRectI;
class tMatrix;
struct tPoint2;
struct tPoint3;
struct tagRECT;
typedef tagRECT RECT;

// The primitive kinds tView3D::mBeginPrimitive takes (0 to 9 are accepted); the enumerators
// are not known, so none is declared.
enum ePrimitiveType {
};

// HYPOTHESIS: a three-float vector built through float arguments (the copy goes through the FPU).
struct fn_0040105e_Vector {
	fn_0040105e_Vector() {}
	fn_0040105e_Vector(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
	float m_x;
	float m_y;
	float m_z;
};

// The view tApp3dBase::vfunction11 creates (tSWView3D or a Direct3D view, both tView3D). The
// slots that carry a name take it, with its signature, from the SDK's symbols for the classes that
// implement them (tSWView3D, tView3DExt and tD3DViewMgr agree slot by slot); the others keep the
// placeholder names and the signatures the units use. Overloads of one name are declared in
// reverse slot order: VC6 lays out the virtual overloads of a name in reverse declaration order.
class tView3D {
public:
	// +0x00 is the virtual destructor (tSWView3D 0x0069f490, tView3DExt 0x006a9e80).
	virtual ~tView3D();
	virtual unsigned char mEnumDispModes(unsigned char (*callback)(int, int, void *), void *data);    // +0x04
	// tApp3dBase::vfunction7 opens the view full screen or windowed; the trailing flag lets the
	// view try its best setting first.
	virtual unsigned char mInitFullscreen(void *, void *, int, int, const tGLRectI *, unsigned char);    // +0x08
	virtual unsigned char mInitWindowed(void *, void *, const tGLRectI *, unsigned char);    // +0x0c
	// HYPOTHESIS: +0x10 takes the "3D" "driver" setting.
	virtual void mSetDriver(int);    // +0x10
	virtual unsigned char mDone();    // +0x14, closes the view (tSWView3D 0x0069f900 returns true)
	virtual void vfunction7();
	// HYPOTHESIS: +0x1c reports whether the view can draw this frame (tSfcApp3d::vfunction4
	// tests it).
	virtual bool vfunction8();    // +0x1c
	virtual void vfunction9();    // +0x20
	virtual unsigned long mGetCaps();
	virtual unsigned char mBeginScene();
	virtual unsigned char mEndScene();
	virtual void (*mSetBeginCallback(void (*)(tGLSurface *, void *), void *))(tGLSurface *, void *);    // +0x30
	virtual void (*mGetBeginCallback())(tGLSurface *, void *);
	virtual void (*mSetEndCallback(void (*)(tGLSurface *, void *), void *))(tGLSurface *, void *);    // +0x38
	virtual void (*mGetEndCallback())(tGLSurface *, void *);
	virtual void mSetViewport(int, int, int, int);
	virtual void mGetViewport(int *, int *, int *, int *);
	virtual unsigned int mSetCullMode(unsigned int);
	virtual unsigned int mGetCullMode();
	virtual unsigned int mSetShadeMode(unsigned int);
	virtual unsigned int mGetShadeMode();
	virtual unsigned int mSetWinding(unsigned int);
	virtual unsigned int mGetWinding();
	virtual unsigned int mSetPolygonMode(unsigned int);
	virtual unsigned int mGetPolygonMode();
	virtual unsigned char mSetClearColor(tGLRgbQuad);    // +0x6c
	virtual unsigned char mSetClearColor(unsigned char, unsigned char, unsigned char);    // +0x68
	virtual unsigned char mClear(unsigned char, unsigned char);    // +0x74
	virtual unsigned char mClear(int, int, int, int, unsigned char, unsigned char);    // +0x70
	virtual unsigned char mFlip();
	virtual unsigned long mGetMaxNumLights() const;
	virtual void mSetLightDescription(unsigned long, const tGLLightDesc &);
	virtual void mGetLightDescription(unsigned long, tGLLightDesc &) const;
	virtual void mSetLightPosition(const tPoint3 &);    // +0x88
	virtual const tPoint3 &mGetLightPosition();
	virtual unsigned char mEnableLighting(unsigned char);
	virtual unsigned char mLightingEnabled();
	virtual unsigned char mEnableLight(unsigned long, unsigned char);
	virtual unsigned char mIsBlendEnabled();
	virtual unsigned char mBlendEnable(unsigned char);
	virtual void vfunction42();
	virtual unsigned long mGetSrcBlendCaps();
	virtual void vfunction44();
	virtual float mSetGlobalBlend(float);
	virtual float mGetGlobalBlend();
	virtual unsigned char mEnableGlobalBlend(unsigned char);
	virtual unsigned char mAlphaTestEnable(unsigned char);
	virtual unsigned char mEnableZWriteAndTest(unsigned char);
	virtual unsigned char mEnableZWrite(unsigned char);
	virtual unsigned char mEnableZTest(unsigned char);
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void mSetCameraMtx(const tMatrix &);
	virtual const tMatrix &mGetCameraMtx();
	virtual void mGetCameraPosition(tPoint3 &);
	virtual void mSetModelMtx(const tMatrix &);
	virtual tMatrix *vfunction59();
	virtual void mSetProjMtx(const tMatrix &);
	virtual const tMatrix &mGetProjMtx();
	virtual void mSetPerspective(float, float, float);    // +0xf4
	virtual void mSetOrtho(float, float, float, float, float, float);
	virtual void mSetOrthoIdentity(float, float);    // +0xfc
	virtual void mCreatePerspective(tMatrix &, float, float, float);
	virtual void mCreateOrthoIdentity(tMatrix &, float, float);
	virtual void mCreateOrtho(tMatrix &, float, float, float, float, float, float);
	virtual unsigned char mBeginPrimitive(ePrimitiveType);
	virtual unsigned char mEndPrimitive();
	virtual void mVtxCoord(const tPoint3 &);    // +0x118
	virtual void mVtxCoord(float, float, float);    // +0x114
	virtual void mVtxNormal(const tPoint3 &);    // +0x120
	virtual void mVtxNormal(float, float, float);    // +0x11c
	virtual void mVtxTexCoord(const tPoint2 &);    // +0x128
	virtual void mVtxTexCoord(float, float);    // +0x124
	virtual void mVtxColor(float, float, float);    // +0x130
	virtual void mVtxColor(tGLRgbQuad);    // +0x12c
	virtual void mPrimitiveTexID(int);
	virtual void mSubmitVertices(int, const tPoint3 *);
	virtual void vfunction80(void *);
	virtual void vfunction81(int, void **, bool);
	virtual bool vfunction82(void (__stdcall *)(void *, int), int);
	virtual tTexInfo *vfunction83(int, void *, int, int, int);
	virtual tTexInfo *vfunction84(int);
	// +0x150 takes a texture handle and its surface (tSFCTexMgrEx::vfunction4).
	virtual unsigned char mLoadTexture(int, tGLSurface &, unsigned char, unsigned long);    // +0x150
	// HYPOTHESIS: tSfcApp3d::vfunction21 calls +0x154 with the two "Mouse" settings (async,
	// hwblit) and passes its result to the mouse at 0x00898c24.
	virtual int vfunction86(int, int);    // +0x154
};

// HYPOTHESIS: the view settings tApp3dBase::vfunction11 reads; +0x24..+0x30 behave as a RECT.
// The constructor is 0x0040b080.
struct fn_0040105e_Params {
	fn_0040105e_Params();
	int m_field00;    // +0x00, from "3D" "windowed"; 1 or 2 tries the windowed open first, 2 only it
	int m_field04;    // +0x04, from "3D" "ZBuffer"; retry setting for the windowed open
	int m_field08;    // +0x08, from "3D" "ZBuffer"; retry setting for the full-screen open
	char m_field0C;    // +0x0c, from "3D" "wireframe"; selects which view is created
	char m_field0D;    // +0x0d
	char m_pad0E[2];
	float m_field10;
	float m_field14;
	float m_field18;
	int m_field1C;    // +0x1c
	int m_field20;    // +0x20
	int m_left;      // +0x24
	int m_top;       // +0x28
	int m_right;     // +0x2c
	int m_bottom;    // +0x30
	fn_0040105e_Vector m_field34;    // +0x34
};

// HYPOTHESIS: field names and types are unknown; offsets are from the constructor only.
class tApp3dBase {
public:
	tApp3dBase();
	// HYPOTHESIS: the empty method taking one argument (it ends in ret 4) sits at +0x00,
	// where vfunction20 calls it with the view; the destructor then takes +0x04.
	virtual void vfunction1(tView3D *);    // +0x00
	virtual ~tApp3dBase();    // +0x04
	// HYPOTHESIS: slots are named after their vtable position; vfunction13 (+0x30) and
	// vfunction20 (+0x4c) are evidenced.
	virtual void vfunction3(tView3D *);    // +0x08
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual unsigned char vfunction7(tView3D *, int, int, fn_0040105e_Params *);    // +0x18, returns what the view's mInitFullscreen returns
	virtual void vfunction8(tView3D *);    // +0x1c
	virtual void vfunction9(tView3D *);    // +0x20
	virtual void vfunction10();
	virtual bool vfunction11(tApplication *param_1, fn_0040105e_Params *param_2);
	virtual void vfunction12();    // +0x2c
	// HYPOTHESIS: returns bool, as tSfcApp3d's function of the same name does.
	virtual bool vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual bool vfunction20();

	char m_field4;    // +0x04, byte-sized store
	int m_field8;     // +0x08, counted up by vfunction20
	// HYPOTHESIS: the application object; tSfcApp3d::vfunction9 calls 0x0069a9ad on it.
	tApplication *m_fieldC;    // +0x0c
	tView3D *m_field10;    // +0x10
	char m_field14;    // +0x14
};

// HYPOTHESIS: the application singleton pointer (0x0089309c), set by the constructor.
extern tApp3dBase *g_pApp3dBase;

#endif	// tApp3dBase_h
