//	tSurface.h: tSurface, a surface of pixels the visual objects draw on.
// Evidence:  class (the SDK names it; original file unknown)

#ifndef tSurface_h
#define tSurface_h 1

#include "unsorted/tVisObject.h"

// HYPOTHESIS: a surface of pixels the size of its rectangle; the layout is read from the
// constructor (0x00675080).
class tSurface : public tObjBase
{
public:
	tSurface( short x, short y, int width, int height, int a, unsigned char b, unsigned char c );
	tSurface( void *pixels, short x, short y, short width, short height, int a, unsigned char b, unsigned char c );
	virtual ~tSurface();
	virtual void vfunction8( const tRect *area );
	bool fn_00675280( c_tScrollBar_FieldA8 *image, int x, int y, const tRect *area, int a, int b );
	bool fn_00675470( int color, const tRect *rect, unsigned char frame, int a, int b );
	bool fn_006755b0( int color, const tRect *rect, const tRect *inner, unsigned char frame, int a, int b );
	// HYPOTHESIS: 0x006757f0 draws a line between two points in a colour, of a width, clipped
	// to a rectangle when one is given.
	bool fn_006757f0( int color, int x1, int y1, int x2, int y2, int a, const tRect *clip );

	void *field_60;
	tRect field_64;
	tPoint field_6C;
	int field_70;
	unsigned char field_74;
	unsigned char field_75;
	unsigned char field_76;
};

#endif	// tSurface_h
