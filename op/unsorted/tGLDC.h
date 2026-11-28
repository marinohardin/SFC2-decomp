//	tGLDC.h: the drawing context class built on the stack by the frame-rate overlay at 0x0046aa9a.
#ifndef tGLDC_h
#define tGLDC_h 1

#include "unsorted/tGLRgbQuad.h"

class tGLSurface;

// HYPOTHESIS: only the members this overlay calls are declared, by their SDK symbols; the
// object is 0x9c bytes (stack slots of the caller) with the vtable pointer first. The overlay
// calls the virtuals on the object itself, so the calls are direct.
class tGLDC
{
public:
	tGLDC();
	virtual ~tGLDC();
	virtual tGLSurface *mSetSurface(tGLSurface *surface);
	virtual tGLRgbQuad mSetFgColor(const tGLRgbQuad &color);
	virtual tGLRgbQuad mSetBgColor(const tGLRgbQuad &color);
	virtual unsigned char mEnableOpaqueBkgnd(unsigned char enable);
	virtual unsigned long mSetTextAlign(unsigned long align);
	virtual unsigned char mTextOut(int x, int y, const char *text, int a, int b);

	char m_pad[0x98];
};

#endif // tGLDC_h
