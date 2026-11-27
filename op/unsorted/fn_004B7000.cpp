#include <windows.h>
#include "Tiger/tUniFont.h"
#include "Tiger/tFontmgr.h"
#include "unsorted/tState_Tactical.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/tEntity.h"

using namespace tState_Namespace_Tactical;

// HYPOTHESIS: a 16-bit pixel surface; slot 10 (+0x28) returns the row pitch in bytes and
// slot 11 (+0x2c) the pixels, as tSWView3D::vfunction85 (0x006b23c0) reads them. Probably
// tBmpSurf, whose header declares no virtuals yet.
class fn_004b70ac_Surface {
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual int vfunction11();
	virtual unsigned char *vfunction12();
};

// Draws the one-pixel outline of a rectangle in a 16-bit colour. The colour is loaded as a
// dword, so the parameter is int-sized.
// FUNCTION: STARFLEETOP 0x004b70ac
void fn_004b70ac(fn_004b70ac_Surface *surface, int color, RECT *rect)
{
	int height = rect->bottom - rect->top >= 0 ? rect->bottom - rect->top : 0;
	int width = rect->right - rect->left >= 0 ? rect->right - rect->left : 0;
	int pitch = surface->vfunction11();
	unsigned char *bits = surface->vfunction12();
	if (bits) {
		unsigned char *row = bits + rect->top * pitch;
		unsigned short *top = (unsigned short *)(row + rect->left * 2);
		unsigned short *bottom = (unsigned short *)(bits + (rect->bottom - 1) * pitch + rect->left * 2);
		for (; width > 0; width--) {
			*top++ = (unsigned short)color;
			*bottom++ = (unsigned short)color;
		}
		unsigned short *left = (unsigned short *)row + rect->left;
		unsigned short *right = (unsigned short *)row + rect->right - 1;
		for (; height > 0; height--) {
			*left = (unsigned short)color;
			left = (unsigned short *)((unsigned char *)left + pitch);
			*right = (unsigned short)color;
			right = (unsigned short *)((unsigned char *)right + pitch);
		}
	}
}

// HYPOTHESIS: the font object at 0x00919f60 has no recovered name.
extern tUniFont g_00919f60;

// Measures a string in that font: zero origin, summed glyph widths, one line high.
// FUNCTION: STARFLEETOP 0x004b7161
// The original returns with ret 8: the function is __stdcall.
void __stdcall fn_004b7161(const char *text, RECT *rect)
{
	rect->top = 0;
	rect->bottom = g_00919f60.mGetFontHeight(*text);
	rect->left = 0;
	rect->right = 0;
	unsigned short ch;
	while ((ch = (unsigned char)*text++) != 0) {
		const tCharInfo *info = g_00919f60.mGetCharInfo(ch);
		if (info) {
			rect->right += info->field_4;
		}
	}
}

// Shifts a rectangle (left, top, right, bottom) so that it lies inside field_1f0, keeping its
// size: first against the top-left corner, then against the bottom-right one.
// HYPOTHESIS: a member of tState_Tactical; the class owns the rectangle at +0x1f0, and no
// caller is known.
// FUNCTION: STARFLEETOP 0x004b71af
void tState_Tactical::fn_004b71af(int *rect)
{
	int d = field_1f0[0] - rect[0];
	if (d > 0) {
		rect[0] += d;
		rect[2] += d;
	}
	d = field_1f0[1] - rect[1];
	if (d > 0) {
		rect[1] += d;
		rect[3] += d;
	}
	d = field_1f0[2] - rect[2];
	if (d < 0) {
		rect[0] += d;
		rect[2] += d;
	}
	d = field_1f0[3] - rect[3];
	if (d < 0) {
		rect[1] += d;
		rect[3] += d;
	}
}

extern tEntityMgr g_008e1060;

// Keeps field_484 on an entity: the player's own one, else the entity field_484 names.
// HYPOTHESIS: field_484 holds an entity's tID; the entity is returned, since the original keeps
// eax untouched up to the ret.
// FUNCTION: STARFLEETOP 0x004b720d
tEntity *tState_Tactical::fn_004b720d()
{
	tEntity *entity = g_008e1060.mGetPlayersControledEntity();
	if (entity != 0 || (entity = g_008e1060.fn_00424a37(field_484)) != 0)
		field_484 = entity->fID;
	return entity;
}

// HYPOTHESIS: an int rectangle (left, top, right, bottom) with methods and no RTTI; the exe
// names neither it nor this method. tState_Tactical::field_1f0 has the same shape.
struct fn_004b7f9e_Rect {
	int left;
	int top;
	int right;
	int bottom;
	void fn_004b7f9e(const fn_004b7f9e_Rect *other);
};

// Grows this rectangle to cover the one given.
// FUNCTION: STARFLEETOP 0x004b7f9e
void fn_004b7f9e_Rect::fn_004b7f9e(const fn_004b7f9e_Rect *other)
{
	if (other->left < left)
		left = other->left;
	if (other->top < top)
		top = other->top;
	if (other->right > right)
		right = other->right;
	if (other->bottom > bottom)
		bottom = other->bottom;
}

// Clips the edge from b to a against the plane where component [axis] equals k times w,
// leaving the intersection in b.
// FUNCTION: STARFLEETOP 0x004b7fce
void fn_004b7fce(float *a, float *b, int axis, float k)
{
	float db = b[axis] - k * b[3];
	float t = db / (db - (a[axis] - k * a[3]));
	b[0] = (a[0] - b[0]) * t + b[0];
	b[1] = (a[1] - b[1]) * t + b[1];
	b[2] = (a[2] - b[2]) * t + b[2];
	b[3] = (a[3] - b[3]) * t + b[3];
}
