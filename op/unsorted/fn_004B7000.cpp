#include <windows.h>
#include "Tiger/tUniFont.h"
#include "Tiger/tFontmgr.h"

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
