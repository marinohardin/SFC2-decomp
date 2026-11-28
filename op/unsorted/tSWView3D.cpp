// Unit:      unsorted/tSWView3D.cpp
// Functions: 0x0069f480 to 0x0069fdc0, 25 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include <string.h>
#include "unsorted/tSWView3D.h"
#include "Tiger/tinifile.h"

class tBasicFile;
tBasicFile *_NewBasicFileCRT();

// HYPOTHESIS: field_1f68 is an object whose slot +0x2c returns an object with a slot +0x48 that
// takes the 16-bit value at field_1f5c and the clipped rectangle.
class fn_0069fb90_Fill {
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void v3();
	virtual void v4();
	virtual void v5();
	virtual void v6();
	virtual void v7();
	virtual void v8();
	virtual void v9();
	virtual void v10();
	virtual void v11();
	virtual void v12();
	virtual void v13();
	virtual void v14();
	virtual void v15();
	virtual void v16();
	virtual void v17();
	virtual void Fill(unsigned short, int *);
};

class fn_0069fb90_Surface {
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void v3();
	virtual void v4();
	virtual void v5();
	virtual void v6();
	virtual void v7();
	virtual void v8();
	virtual void v9();
	virtual void v10();
	virtual fn_0069fb90_Fill *Get();
};

// HYPOTHESIS: the device built by 0x006b3260 or 0x006b4fe0; only the slots 0x0069f510 calls are
// named (slot 5 takes a callback and a value, slots 9 and 2 are called when this function made it).
class fn_0069f510_Device {
public:
	virtual void v0();
	virtual void v1();
	virtual void *v2();
	virtual void v3();
	virtual void v4();
	virtual bool v5(void (__stdcall *)(void *, int), void *);
	virtual void v6();
	virtual void v7();
	virtual void v8();
	virtual void v9();
	virtual void v10();
	virtual void *v11();
};

extern char *g_008846b4;
extern char *g_008846b8;
extern int g_00c7c10c;
void __stdcall fn_0069f5d0(void *, int);
tGLSurfMgr *dmpCreateGDISurfMgr();
tGLSurfMgr *dmpCreateDDSurfMgr();

// FUNCTION: STARFLEETOP 0x0069f3b0
tSWView3D::tSWView3D()
{
	field_1f5c = 0;
	field_1f60 = 0;
	field_1f64 = 0;
	field_1f68 = 0;
	memset(field_1f6c, 0, sizeof(field_1f6c));
	field_5fc5 = true;
	field_5fc6 = true;
	field_5fa8 = 0;
	field_5fac = 0;
	field_5fb0 = 0;
	field_5fb4 = 0;
	field_5fb8 = 0;
	field_5fbc = 0;
	field_5fc0 = 0;
	field_5fc4 = false;
	field_0x1ed8 = 0;
}

// FUNCTION: STARFLEETOP 0x0069f480
tGLSurfMgr *tSWView3D::mGetSurfMgr()
{
	return (tGLSurfMgr *)field_1f68;
}

// FUNCTION: STARFLEETOP 0x0069f490
// FUNCTION: STARFLEETOP 0x0069f4b0
tSWView3D::~tSWView3D()
{
	tSWView3D::mDone();
	// HYPOTHESIS: the inlined ~tGLPixFmt of the member at +0x5f6c resets its vtable (0x007959d8).
	// MATCH-ONLY: stored as a raw address, the class is not in a shared header.
	*(unsigned long *)field_5f6c = 0x007959d8;
}

// FUNCTION: STARFLEETOP 0x0069f510
unsigned char tSWView3D::mEnumDispModes(unsigned char (*a)(int, int, void *), void *b)
{
	fn_0069f510_Device *device = (fn_0069f510_Device *)field_1f68;
	bool created = false;
	if (!device) {
		bool debug = false;
		created = true;
		tINIFile *ini = tINIFile::mNewINIFile(g_008846b4, _NewBasicFileCRT);
		if (ini) {
			tINISection *section = ini->mGetSection(g_008846b8, 0);
			if (section) {
				unsigned long value = section->mGetInt32u("debug", 0xffffffff);
				if (value != 0xffffffff && value > 0)
					debug = true;
			}
			ini->mRelease();
		}
		void *made = 0;
		if (!(ini && debug))
			made = dmpCreateDDSurfMgr();
		device = (fn_0069f510_Device *)(made ? made : dmpCreateGDISurfMgr());
		if (!device)
			return false;
	}
	g_00c7c10c = (int)a;
	bool result = device->v5(fn_0069f5d0, b);
	if (created) {
		device->v9();
		device->v2();
	}
	return result;
}

// HYPOTHESIS: the texture slots hold objects with a virtual destructor, called with the delete flag.
class tTexInfo {
public:
	virtual ~tTexInfo();
};

// FUNCTION: STARFLEETOP 0x0069f850
unsigned char tSWView3D::mInitFullscreen(void *a, void *b, int c, int d, const tGLRectI *rect, unsigned char f)
{
	// HYPOTHESIS: tGLRectI is only forward-declared here, so the rectangle is four ints.
	int local[4];
	if (!rect) {
		rect = (const tGLRectI *)local;
		local[0] = 0;
		local[1] = 0;
		local[2] = c;
		local[3] = d;
	}
	return mInit(a, b, c, d, rect, f, true);
}

int GetWindowWidth(void *);
int GetWindowHeight(void *);

// FUNCTION: STARFLEETOP 0x0069f8a0
unsigned char tSWView3D::mInitWindowed(void *a, void *b, const tGLRectI *rect, unsigned char f)
{
	int w = GetWindowWidth(a);
	int h = GetWindowHeight(a);
	int local[4];
	if (!rect) {
		rect = (const tGLRectI *)local;
		local[0] = 0;
		local[1] = 0;
		local[2] = w;
		local[3] = h;
	}
	return mInit(a, b, w, h, rect, f, false);
}

// FUNCTION: STARFLEETOP 0x0069f900
unsigned char tSWView3D::mDone()
{
	tView3DExt::mDone();
	if (field_5fb4) {
		delete field_5fb4;
		field_5fb4 = 0;
		field_5fb8 = 0;
		field_5fbc = 0;
		field_5fc0 = 0;
	}
	for (int i = 0; i < 0x1000; i++) {
		if (field_1f6c[i]) {
			delete field_1f6c[i];
			field_1f6c[i] = 0;
		}
	}
	if (field_1f68) {
		((fn_0069f510_Device *)field_1f68)->v9();
		field_1f68 = ((fn_0069f510_Device *)field_1f68)->v2();
	}
	field_5fa8 = 0;
	field_5fac = 0;
	field_5fb0 = 0;
	return true;
}

// HYPOTHESIS: the surface objects behind field_5fa8; only the slots 0x0069f990 calls are named.
class fn_0069f990_Surface {
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void v3();
	virtual void v4();
	virtual void v5();
	virtual void v6();
	virtual bool v7();
	virtual bool v8();
	virtual void *v9();
	virtual void *v10();
	virtual unsigned char *v11();
};

// FUNCTION: STARFLEETOP 0x0069f990
unsigned char tSWView3D::mBeginScene()
{
	field_5fa8 = ((fn_0069f510_Device *)field_1f68)->v11();
	if (field_0x1edc)
		((void (*)(void *, void *))field_0x1edc)(field_5fa8, field_0x1ee0);
	if (field_5fa8) {
		if (((fn_0069f990_Surface *)field_5fa8)->v7()) {
			field_5fac = (int)((fn_0069f990_Surface *)field_5fa8)->v10();
			field_5fb0 = ((fn_0069f990_Surface *)field_5fa8)->v11();
		} else {
			field_5fac = 0;
			field_5fb0 = 0;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0069fa10
unsigned char tSWView3D::mEndScene()
{
	void *surface = ((fn_0069f510_Device *)field_1f68)->v11();
	if (field_0x1ee4)
		((void (*)(void *, void *))field_0x1ee4)(surface, field_0x1ee8);
	if (surface)
		return ((fn_0069f990_Surface *)surface)->v8();
	return false;
}

// FUNCTION: STARFLEETOP 0x0069fa50
unsigned int tSWView3D::mSetPolygonMode(unsigned int)
{
	return 10;
}

// FUNCTION: STARFLEETOP 0x0069fa60
unsigned int tSWView3D::mGetPolygonMode()
{
	return 10;
}

// FUNCTION: STARFLEETOP 0x0069fa70
void tSWView3D::mSetViewport(int a, int b, int c, int d)
{
	tView3DExt::mSetViewport(a, b, c, d);
	field_1f50 = d;
	field_1f4c = c;
	field_1f54 = a + c;
	field_1f58 = b + d;
}

// FUNCTION: STARFLEETOP 0x0069fad0
unsigned long tSWView3D::mGetCaps()
{
	unsigned long result = 0xe;
	if (field_5fb4)
		result = 0x1f;
	return result;
}

// FUNCTION: STARFLEETOP 0x0069faf0
unsigned char tSWView3D::mSetClearColor(tGLRgbQuad color)
{
	return mSetClearColor(color.field_0x02, color.field_0x01, color.field_0x00);
}

// FUNCTION: STARFLEETOP 0x0069fb10
unsigned char tSWView3D::mSetClearColor(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned short r16 = r;
	unsigned short g16 = g;
	unsigned short b16 = b;
	field_1f5c = (((r16 & 0xfff8) << 5 | (g16 & 0xfff8)) << 2) | (unsigned short)(b16 >> 3);
	return true;
}

// FUNCTION: STARFLEETOP 0x0069fb50
unsigned char tSWView3D::mClear(unsigned char e, unsigned char f)
{
	int width = (int)field_0x764;
	int height = (int)field_0x768;
	return mClear(0, 0, width, height, e, f);
}

// FUNCTION: STARFLEETOP 0x0069fb90
unsigned char tSWView3D::mClear(int a, int b, int c, int d, unsigned char e, unsigned char f)
{
	int rect[4];
	rect[2] = field_1f4c + c;
	rect[0] = field_1f4c + a;
	rect[1] = field_1f50 + b;
	int bottom = field_1f50 + d;
	if (rect[0] < field_1f4c)
		rect[0] = field_1f4c;
	if (rect[1] < field_1f50)
		rect[1] = field_1f50;
	if (field_1f54 < rect[2])
		rect[2] = field_1f54;
	if (field_1f58 < bottom)
		bottom = field_1f58;
	rect[3] = bottom;
	if (rect[2] <= rect[0] || rect[3] <= rect[1])
		return false;

	if (e && field_1f68) {
		fn_0069fb90_Fill *fill = ((fn_0069fb90_Surface *)field_1f68)->Get();
		fill->Fill(field_1f5c, rect);
	}
	if (f && field_5fb4) {
		unsigned int pitch = field_5fc0;
		unsigned short *p = field_5fb4 + (field_5fb8 * rect[1] + rect[0]);
		int rows = rect[3] - rect[1];
		rows &= (rows < 0) - 1;
		for (; rows > 0; rows--) {
			memset(p, 0, pitch);
			p = (unsigned short *)((char *)p + pitch);
		}
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x0069fd20
tView3D *dmpGetSWView3D()
{
	if (!tSWView3D::fMaster)
		tSWView3D::fMaster = new tSWView3D;
	if (tSWView3D::fMaster)
		tSWView3D::fMaster->vfunction7();
	return tSWView3D::fMaster;
}

// FUNCTION: STARFLEETOP 0x0069fdc0
unsigned long tSWView3D::mGetSrcBlendCaps()
{
	return 2;
}

// FUNCTION: STARFLEETOP 0x0069fde0
unsigned char tSWView3D::mAlphaTestEnable(unsigned char param_1)
{
	bool old = field_5fc4;
	field_5fc4 = param_1 ? true : false;    // HYPOTHESIS: the original tests the argument again, as tView3DExt::mEnableGlobalBlend does
	return old;
}

// FUNCTION: STARFLEETOP 0x0069fe00
unsigned char tSWView3D::mEnableZWriteAndTest(unsigned char param_1)
{
	field_5fc5 = param_1 ? true : false;
	field_5fc6 = param_1 ? true : false;
	return true;
}

// FUNCTION: STARFLEETOP 0x0069fe20
unsigned char tSWView3D::mEnableZWrite(unsigned char param_1)
{
	field_5fc5 = param_1 ? true : false;
	return true;
}

// FUNCTION: STARFLEETOP 0x0069fe40
unsigned char tSWView3D::mEnableZTest(unsigned char param_1)
{
	field_5fc6 = param_1 ? true : false;
	return true;
}

// HYPOTHESIS: the surface's vtable, as 0x006b23c0 reads it (tBmpSurf declares no virtuals yet).
class fn_006b23c0_Info {
public:
	char pad[0x34];
	int mode;
};

class fn_006b23c0_Rect {
public:
	int left;
	int top;
	int right;
	int bottom;
};

class fn_006b23c0_Surf {
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual fn_006b23c0_Info *GetInfo();
	virtual fn_006b23c0_Rect *GetRect();
	virtual void v5();
	virtual void v6();
	virtual bool IsLocked();
	virtual int v8();
	virtual void v9();
	virtual int GetPitch();
	virtual unsigned char *GetBits();
	virtual void v12();
	virtual void v13();
	virtual int GetFlag();
};

// HYPOTHESIS: the tTexInfo fields this reads (+0x6c pitch, +0x74 bits, +0x64 and +0x65 flags).
class fn_006b23c0_Tex {
public:
	char pad[0x64];
	char flag64;
	char flag65;
	char pad2[6];
	int pitch;
	int pad3;
	unsigned char *bits;
};

// FUNCTION: STARFLEETOP 0x006b23c0
unsigned char tSWView3D::mLoadTexture(int handle, tGLSurface &surf, unsigned char c1, unsigned long c2)
{
	fn_006b23c0_Surf *s = (fn_006b23c0_Surf *)&surf;
	if (handle > 0 && handle <= 0x1000) {
		fn_006b23c0_Rect *r = s->GetRect();
		unsigned int w = ((r->right - r->left < 0) - 1) & (r->right - r->left);
		r = s->GetRect();
		unsigned int h = ((r->bottom - r->top < 0) - 1) & (r->bottom - r->top);
		fn_006b23c0_Info *info = s->GetInfo();
		unsigned int flag = s->GetFlag();
		if (info->mode == 1 && flag) {
			fn_006b23c0_Tex *t = (fn_006b23c0_Tex *)vfunction83(handle, info, w, h, flag);
			if (t && s->IsLocked()) {
				int srcPitch = s->GetPitch();
				int dstPitch = t->pitch;
				unsigned char *dst = t->bits + (h - 1) * dstPitch;
				unsigned char *src = s->GetBits();
				if ((int)h > 0) {
					int y = h;
					do {
						memcpy(dst, src, w);
						dst -= dstPitch;
						src += srcPitch;
						y--;
					} while (y != 0);
				}
				s->v8();
				t->flag64 = (char)c1;
				t->flag65 = (char)c2;
				return true;
			}
		}
	}
	return false;
}
