#define tvectormath_spark_members
#include <math.h>
#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x0069f190
unsigned char tView3DExt::mLightingEnabled()
{
	return field_0x1eb8;
}

// FUNCTION: STARFLEETOP 0x0069fda0
unsigned char tView3DExt::mEnableGlobalBlend(unsigned char flag)
{
	unsigned char old = field_0x1ed8;
	field_0x1ed8 = flag ? true : false;    // HYPOTHESIS: the original tests the argument again (it may have been a BOOL-like type)
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9620
unsigned int tView3DExt::mSetWinding(unsigned int mode)
{
	unsigned int old = field_0x1ebc;
	switch (mode) {
	case 6:
		field_0x1ebc = 6;
		field_0x1ec0 = 1;
		break;
	case 7:
		field_0x1ebc = 7;
		field_0x1ec0 = 0;
		break;
	}
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9660
unsigned int tView3DExt::mGetWinding()
{
	return field_0x1ebc;
}

// FUNCTION: STARFLEETOP 0x006a9670
unsigned int tView3DExt::mSetCullMode(unsigned int mode)
{
	unsigned int old = field_0x1ec4;
	switch (mode) {
	case 0:
		field_0x1ec4 = 0;
		field_0x1ec8 = 0;
		break;
	case 1:
		field_0x1ec4 = 1;
		field_0x1ec8 = 1;
		break;
	case 2:
		field_0x1ec4 = 2;
		field_0x1ec8 = 2;
		break;
	}
	return old;
}

// FUNCTION: STARFLEETOP 0x006a96d0
unsigned int tView3DExt::mGetCullMode()
{
	return field_0x1ec4;
}

// FUNCTION: STARFLEETOP 0x006a96e0
unsigned int tView3DExt::mSetShadeMode(unsigned int mode)
{
	unsigned int old = field_0x1ecc;
	if (mode >= 3 && mode <= 5) {
		field_0x1ecc = mode;
	}
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9700
unsigned int tView3DExt::mGetShadeMode()
{
	return field_0x1ecc;
}

// HYPOTHESIS: the original returns the previous level as a float (it loads field_0x1ed4 first and
// leaves it on the FPU stack; 0x006a9540 pops the result); tView3D declares the slot void.
// FUNCTION: STARFLEETOP 0x006a9710
float tView3DExt::mSetGlobalBlend(float level)
{
	float old = field_0x1ed4;
	if (level < 0.0f) {
		field_0x1ed4 = 0.0f;
	} else if (level > 1.0f) {
		field_0x1ed4 = 1.0f;
	} else {
		field_0x1ed4 = level;
	}
	// The byte copy is rounded by adding 1.5 * 2^23 and keeping the low mantissa byte.
	level = field_0x1ed4 * 255.0f + 12582912.0f;
	field_0x1ed9 = *(unsigned char *)&level;
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9780
float tView3DExt::mGetGlobalBlend()
{
	return field_0x1ed4;
}

// FUNCTION: STARFLEETOP 0x006a9aa0
unsigned long tView3DExt::mGetMaxNumLights() const
{
	return 0x40;
}

// FUNCTION: STARFLEETOP 0x006a9ab0
void tView3DExt::mSetLightDescription(unsigned long index, const tGLLightDesc &light)
{
	if (index < 0x40) {
		const int *src = (const int *)&light;
		*(int *)&field_0x778[index][0x00] = src[0];
		// HYPOTHESIS: the light record holds the direction (+4) and position (+0x10) points that
		// mSetModelMtx transforms; copying them as points gives the original's store addressing.
		*(tPoint3 *)&field_0x778[index][0x04] = *(const tPoint3 *)&src[1];
		*(tPoint3 *)&field_0x778[index][0x10] = *(const tPoint3 *)&src[4];
		*(int *)&field_0x778[index][0x1c] = src[7];
		*(int *)&field_0x778[index][0x20] = src[8];
		*(int *)&field_0x778[index][0x24] = src[9];
		*(int *)&field_0x778[index][0x28] = src[10];
		*(int *)&field_0x778[index][0x2c] = src[11];
		*(int *)&field_0x778[index][0x30] = src[12];
		*(int *)&field_0x778[index][0x34] = src[13];
		*(int *)&field_0x778[index][0x38] = src[14];
		*(int *)&field_0x778[index][0x3c] = src[15];
		int type = src[0];
		if ((type == 1 || (type > 4 && type <= 6)) && ((float *)field_0x778[index])[10] != 0.0f)
			field_0x1db8[index] = 1.0f / ((float *)field_0x778[index])[10];
	}
}

// FUNCTION: STARFLEETOP 0x006a9ba0
void tView3DExt::mGetLightDescription(unsigned long index, tGLLightDesc &light) const
{
	if (index < 0x40) {
		const int *src = (const int *)field_0x778[index];
		int *dst = (int *)&light;
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = src[3];
		dst[4] = src[4];
		dst[5] = src[5];
		dst[6] = src[6];
		dst[7] = src[7];
		dst[8] = src[8];
		dst[9] = src[9];
		dst[10] = src[10];
		dst[11] = src[11];
		dst[12] = src[12];
		dst[13] = src[13];
		dst[14] = src[14];
		dst[15] = src[15];
	}
}

// FUNCTION: STARFLEETOP 0x006a9c20
unsigned char tView3DExt::mEnableLighting(unsigned char flag)
{
	unsigned char old = field_0x1eb8;
	field_0x1eb8 = flag;
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9c40
unsigned char tView3DExt::mEnableLight(unsigned long index, unsigned char enable)
{
	if (index < 0x40)
	{
		field_0x1778[index] = enable;
		return true;
	}
	return false;
}

// HYPOTHESIS: the cache is freed twice; the original's bytes repeat the same block, perhaps from
// an inlined helper the source called in two places.
// FUNCTION: STARFLEETOP 0x006a9e80
// FUNCTION: STARFLEETOP 0x006a9eb0
tView3DExt::~tView3DExt()
{
	if ( mVtxCache )
	{
		delete [] mVtxCache;
		mVtxCache = NULL;
		mVtxCacheSize = 0;
	}
	if ( mVtxCache )
	{
		delete [] mVtxCache;
		mVtxCache = NULL;
		mVtxCacheSize = 0;
	}
}

// FUNCTION: STARFLEETOP 0x006a9f20
fn_006a9f20_Callback tView3DExt::mSetBeginCallback(fn_006a9f20_Callback callback, void *data)
{
	field_0x1ee0 = data;
	fn_006a9f20_Callback old = field_0x1edc;
	field_0x1edc = callback;
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9f40
fn_006a9f20_Callback tView3DExt::mGetBeginCallback()
{
	return field_0x1edc;
}

// FUNCTION: STARFLEETOP 0x006a9f50
fn_006a9f20_Callback tView3DExt::mSetEndCallback(fn_006a9f20_Callback callback, void *data)
{
	field_0x1ee8 = data;
	fn_006a9f20_Callback old = field_0x1ee4;
	field_0x1ee4 = callback;
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9f70
fn_006a9f20_Callback tView3DExt::mGetEndCallback()
{
	return field_0x1ee4;
}

// FUNCTION: STARFLEETOP 0x006a9f80
void tView3DExt::mSetLightPosition(const tPoint3 &v)
{
	float len = v.z * v.z + v.y * v.y + v.x * v.x;
	if (len >= 1.0f || len < 0.9999f)
		len = (float)sqrt(len);
	if (len >= 1e-07f) {
		field_0x738[0] = v.x / len;
		field_0x738[1] = v.y / len;
		field_0x738[2] = v.z / len;
	}
}

// FUNCTION: STARFLEETOP 0x006a9ff0
const tPoint3 &tView3DExt::mGetLightPosition()
{
	return *(tPoint3 *)field_0x738;
}

// HYPOTHESIS: the float constant 0.5 at 0x007959c8.
extern float g_007959c8;
// The clip rectangle is tView3DExt::fClipMin (0x00c7c250) and fClipMax (0x50 further).

// FUNCTION: STARFLEETOP 0x006aa000
void tView3DExt::mSetViewport(int a, int b, int c, int d)
{
	field_0x75c = (float)c;
	field_0x760 = (float)d;
	field_0x764 = (float)a;
	field_0x768 = (float)b;
	field_0x76c = (float)a * g_007959c8;
	field_0x770 = (float)b * g_007959c8;
	fClipMin.x = field_0x75c;
	fClipMax.x = field_0x764 + field_0x75c;
	fClipMin.y = field_0x760;
	fClipMax.y = field_0x768 + field_0x760;
}

// FUNCTION: STARFLEETOP 0x006aa080
void tView3DExt::mGetViewport(int *a, int *b, int *c, int *d)
{
	if (a)
		*a = (int)field_0x764;
	if (b)
		*b = (int)field_0x768;
	if (c)
		*c = (int)field_0x75c;
	if (d)
		*d = (int)field_0x760;
}

// The three factors of the field-of-view angle are separate constants in the original.
extern double g_007a1648;    // pi
extern double g_007a13a8;    // 1/180
extern double g_007977c8;    // 0.5

// FUNCTION: STARFLEETOP 0x006aa0e0
void tView3DExt::mCreatePerspective(tMatrix &m, float fov, float zNear, float zFar)
{
	float top = (float)tan(fov * g_007977c8 * g_007a13a8 * g_007a1648) * zNear;
	float bottom = -top;
	float right = field_0x764 / field_0x768 * top;
	float left = -right;
	tMtx4x4 &mtx = m.mGetMtx4x4();
	float depth = zFar - zNear;
	float width = right - left;
	float height = top - bottom;
	float twoNear = zNear + zNear;
	mtx[0][0] = twoNear / width;
	mtx[0][1] = 0.0f;
	mtx[0][2] = (left + right) / width;
	mtx[0][3] = 0.0f;
	mtx[1][0] = 0.0f;
	mtx[1][1] = twoNear / height;
	mtx[1][2] = (bottom + top) / height;
	mtx[1][3] = 0.0f;
	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[2][2] = -((zNear + zFar) / depth);
	mtx[2][3] = -((zNear * zFar + zNear * zFar) / depth);
	mtx[3][0] = 0.0f;
	mtx[3][1] = 0.0f;
	mtx[3][2] = -1.0f;
	mtx[3][3] = 0.0f;
	// HYPOTHESIS: the original ends with m->mSet3x4Flag(kFalse) (the byte at +0x41 cleared last,
	// m kept live to the end); tMatrix in Tiger/tvectormath.h does not declare that SDK member yet.
}

// FUNCTION: STARFLEETOP 0x006aa1c0
void tView3DExt::mSetPerspective(float fov, float zNear, float zFar)
{
	tMatrix m;
	mCreatePerspective(m, fov, zNear, zFar);
	mSetProjMtx(m);
}

// FUNCTION: STARFLEETOP 0x006aa210
void tView3DExt::mSetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	tMatrix m;
	mCreateOrtho(m, left, right, bottom, top, zNear, zFar);
	mSetProjMtx(m);
}

// FUNCTION: STARFLEETOP 0x006aa270
void tView3DExt::mSetOrthoIdentity(float zNear, float zFar)
{
	tMatrix m;
	mCreateOrthoIdentity(m, zNear, zFar);
	mSetProjMtx(m);
}

// FUNCTION: STARFLEETOP 0x006aa2c0
void tView3DExt::mCreateOrtho(tMatrix &m, float left, float right, float bottom, float top, float zNear, float zFar)
{
	// HYPOTHESIS: the original builds m with tMatrix's 12-float (3x4) constructor, which also sets the
	// 3x4 flag (the byte at +0x41); tMatrix in Tiger/tvectormath.h does not declare that constructor yet.
	// MATCH-ONLY: the constant cells come first, as the inlined constructor's stores do.
	tMtx4x4 &mtx = m.mGetMtx4x4();
	mtx[0][1] = 0.0f;
	mtx[0][2] = 0.0f;
	mtx[1][0] = 0.0f;
	mtx[1][2] = 0.0f;
	mtx[2][0] = 0.0f;
	mtx[2][1] = 0.0f;
	mtx[3][0] = 0.0f;
	mtx[3][1] = 0.0f;
	mtx[3][2] = 0.0f;
	mtx[3][3] = 1.0f;
	mtx[0][3] = -((right + left) / (right - left));
	mtx[1][1] = 2.0f / (top - bottom);
	mtx[1][3] = -((top + bottom) / (top - bottom));
	mtx[2][2] = -2.0f / (zFar - zNear);
	mtx[0][0] = 2.0f / (right - left);
	mtx[2][3] = -((zFar + zNear) / (zFar - zNear));
}

// HYPOTHESIS: the bytes pass x, y, x + w, y + h; mCreateOrtho's parameter names (left, right, bottom,
// top) may then be in the wrong order.
// FUNCTION: STARFLEETOP 0x006aa390
void tView3DExt::mCreateOrthoIdentity(tMatrix &m, float zNear, float zFar)
{
	float y = field_0x760;
	float x = field_0x75c;
	mCreateOrtho(m, x, y, x + field_0x764, y + field_0x768, zNear, zFar);
}

// FUNCTION: STARFLEETOP 0x006aa490
const tMatrix &tView3DExt::mGetProjMtx()
{
	return field_0x55c;
}

// FUNCTION: STARFLEETOP 0x006aa4a0
void tView3DExt::mSetCameraMtx(const tMatrix &m)
{
	field_0x628 = m;
	field_0x5a0.mMult(field_0x5e4, field_0x628);
}

// FUNCTION: STARFLEETOP 0x006aa4d0
const tMatrix &tView3DExt::mGetCameraMtx()
{
	return field_0x628;
}

// FUNCTION: STARFLEETOP 0x006aa4e0
void tView3DExt::mGetCameraPosition(tPoint3 &p)
{
	static tPoint3 origin(0.0f, 0.0f, 0.0f);
	field_0x628.mInverseTransform(p, origin);
}

// HYPOTHESIS: the original calls 0x0069cc40 (another tMatrix member taking an output point and an
// input point) where this body calls mInverseTransform three times; tMatrix declares no member for it yet.
// FUNCTION: STARFLEETOP 0x006aa550
void tView3DExt::mSetModelMtx(const tMatrix &m)
{
	field_0x6f4 = m;
	field_0x628.mMult(field_0x66c, m);
	field_0x5e4.mMult(field_0x6b0, m);
	tMatrix product(tMatrix::kUninitialized);
	field_0x628.mMult(product, m);
	static tPoint3 origin(0.0f, 0.0f, 0.0f);
	product.mInverseTransform(*(tPoint3 *)field_0x744, origin);
	m.mInverseTransform(*(tPoint3 *)field_0x750, *(tPoint3 *)field_0x738);
	for (int i = 0; i < 0x40; i++) {
		if (field_0x1778[i] == 0)
			return;
		char *light = field_0x778[i];
		tPoint3 *pos = (tPoint3 *)(light + 0x10);
		tPoint3 *dir = (tPoint3 *)(light + 4);
		switch (*(int *)light) {
		case 1:
		case 3:
		case 5:
			m.mInverseTransform(*(tPoint3 *)field_0x17b8[i], *pos);
			break;
		case 2:
		case 4:
		case 6:
			m.mInverseTransform(*(tPoint3 *)field_0x1ab8[i], *dir);
			m.mInverseTransform(*(tPoint3 *)field_0x17b8[i], *pos);
			break;
		default:
			m.mInverseTransform(*(tPoint3 *)field_0x1ab8[i], *dir);
		}
	}
}

// FUNCTION: STARFLEETOP 0x006aa6c0
tMatrix *tView3DExt::vfunction59()
{
	return &field_0x6f4;
}

// FUNCTION: STARFLEETOP 0x006aa6d0
void tView3DExt::mSetDriver(int value)
{
	field_0x774 = value;
}

// FUNCTION: STARFLEETOP 0x006aa6e0
void tView3DExt::mResetCounts()
{
	field_0x1f2c[4] = 0;
	field_0x1f2c[3] = 0;
	field_0x1f2c[2] = 0;
	field_0x1f2c[1] = 0;
	field_0x1f2c[0] = 0;
	field_0x1f2c[7] = 0;
	field_0x1f2c[6] = 0;
	field_0x1f2c[5] = 0;
}

// FUNCTION: STARFLEETOP 0x006aa720
unsigned long tView3DExt::mGetRawCountPoints()
{
	return field_0x1f2c[0];
}

// FUNCTION: STARFLEETOP 0x006aa730
unsigned long tView3DExt::mGetRawCountLines()
{
	return field_0x1f2c[1];
}

// FUNCTION: STARFLEETOP 0x006aa740
unsigned long tView3DExt::mGetRawCountTris()
{
	return field_0x1f2c[2];
}

// FUNCTION: STARFLEETOP 0x006aa750
unsigned long tView3DExt::mGetRawCountQuads()
{
	return field_0x1f2c[3];
}

// FUNCTION: STARFLEETOP 0x006aa760
unsigned long tView3DExt::mGetRawCountNGons()
{
	return field_0x1f2c[4];
}

// FUNCTION: STARFLEETOP 0x006aa770
unsigned long tView3DExt::mGetTrueCountPoints()
{
	return field_0x1f2c[5];
}

// FUNCTION: STARFLEETOP 0x006aa780
unsigned long tView3DExt::mGetTrueCountLines()
{
	return field_0x1f2c[6];
}

// FUNCTION: STARFLEETOP 0x006aa790
unsigned long tView3DExt::mGetTrueCountTris()
{
	return field_0x1f2c[7];
}

// HYPOTHESIS: tView3D declares vfunction42/53/54 without the arguments this function passes
// (HEADER REQUEST pending); the member-pointer casts give those calls their real shapes, and the
typedef bool (tView3D::*tVFn41)(bool);    // one for mBlendEnable keeps the shape these bytes match
typedef void (tView3D::*tVFn42)(int, int);
typedef bool (tView3D::*tVFn53)();
typedef void (tView3D::*tVFn54)(int);

// FUNCTION: STARFLEETOP 0x006ac5a0
void tView3DExt::vfunction81(int a, void **b, bool c)
{
	tVFn41 f41 = (tVFn41)&tView3D::mBlendEnable;
	tVFn42 f42 = (tVFn42)&tView3D::vfunction42;
	tVFn53 f53 = (tVFn53)&tView3D::vfunction53;
	tVFn54 f54 = (tVFn54)&tView3D::vfunction54;
	int saved = vfunction96();
	unsigned char old = mAlphaTestEnable(false);
	mAlphaTestEnable(old);
	if ((mGetCaps() & 0x20) == 0) {
		(this->*f54)(0);
		vfunction97(1, a, b, c);
		return;
	}
	if ((this->*f53)()) {
		vfunction97(0, a, b, c);
		return;
	}
	unsigned char flags;
	if (old ? (flags & 2) == 0 : (flags & 1) == 0) {
		(this->*f54)(0);
		vfunction97(1, a, b, c);
		return;
	}
	(this->*f54)(2);
	vfunction97(2, a, b, c);
	unsigned char lit = mEnableLighting(false);
	bool prev = (this->*f41)(true);
	(this->*f42)(1, 4);
	(this->*f54)(1);
	vfunction97(1, a, b, c);
	(this->*f41)(prev);
	mEnableLighting(lit);
	(this->*f54)(0);
}
