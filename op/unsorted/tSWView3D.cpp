// Unit:      unsorted/tSWView3D.cpp
// Functions: 0x0069f480 to 0x0069fdc0, 25 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include <string.h>
#include "unsorted/tSWView3D.h"

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

// FUNCTION: STARFLEETOP 0x0069fb90
bool tSWView3D::vfunction29(int a, int b, int c, int d, bool e, bool f)
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
bool tSWView3D::vfunction85(int handle, tBmpSurf *surf, char c1, int c2)
{
	fn_006b23c0_Surf *s = (fn_006b23c0_Surf *)surf;
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
				t->flag64 = c1;
				t->flag65 = (char)c2;
				return true;
			}
		}
	}
	return false;
}
