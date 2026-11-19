#include "Tiger/tstring.h"
#include "unsorted/tVisObject.h"
#include "unsorted/tScrollBar.h"

// HYPOTHESIS: the exception the constructor throws when its pixels cannot be allocated; 0x00692f60
// builds it from a message and a value, and it is thrown with the throw information at 0x008273b8.
// Its size is read from the constructor's frame.
class c_00692f60_Error
{
public:
	c_00692f60_Error( const tString &text, int a );
	virtual ~c_00692f60_Error();
	unsigned char data[ 0x28 ];
};

// HYPOTHESIS: the object at 0x00c690d0; 0x006740f0 returns the text of an error id.
class c_00c690d0_Strings
{
public:
	const tString &fn_006740f0( int id );
};

extern c_00c690d0_Strings *g_00c690d0;
// HYPOTHESIS: the text the constructor's empty message starts from; the original reaches it
// at 0x0083b344, not the pooled empty literal.
extern const char g_0083b344[];

// HYPOTHESIS: 0x006741e0 looks up an error id by its name.
int fn_006741e0( const char *name );
// HYPOTHESIS: 0x00695150 allocates a count of bytes; the first value is not known.
void *fn_00695150( int a, int size );

// HYPOTHESIS: a surface of pixels the size of its rectangle; the layout is read from the
// constructor (0x00675080). The original destructor at 0x00675250 is not recovered yet.
class tSurface : public tObjBase
{
public:
	tSurface( short x, short y, int width, int height, int a, unsigned char b, unsigned char c );
	tSurface( void *pixels, short x, short y, short width, short height, int a, unsigned char b, unsigned char c );
	virtual ~tSurface();
	// HYPOTHESIS: the callers reach it as c_tVisObject_Field14::fn_00675280 (tVisObject.h).
	bool fn_00675280( c_tScrollBar_FieldA8 *image, int x, int y, const tRect *area, int a, int b );
	// HYPOTHESIS: the callers reach it as c_tVisObject_Field14::fn_00675470 (tVisObject.h).
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

// HYPOTHESIS: 0x00695180 releases pixels obtained from 0x00695150.
void fn_00695180( void *p ) throw();

// FUNCTION: STARFLEETOP 0x00675060
// FUNCTION: STARFLEETOP 0x00675250
tSurface::~tSurface()
{
	if ( field_76 && field_60 )
		fn_00695180( field_60 );
}

// FUNCTION: STARFLEETOP 0x00675080
tSurface::tSurface( short x, short y, int width, int height, int a, unsigned char b, unsigned char c )
	: field_70( a ), field_74( c ), field_75( b ), field_76( 1 )
{
	field_3C.x = x;
	field_3C.y = y;
	field_6C.x = 0;
	field_6C.y = 0;
	field_64.mSet( 0, 0, width, height );
	field_34.mSet( x, y, x + width, y + height );

	field_60 = fn_00695150( 1, width * height );
	if ( field_60 == 0 )
	{
		int id = fn_006741e0( "tQ3Error_Q3SURFACE_Alloc" );
		static tString sEmpty( g_0083b344 );
		throw c_00692f60_Error( g_00c690d0 ? g_00c690d0->fn_006740f0( id ) : sEmpty, 1 );
	}
}

// HYPOTHESIS: wraps pixels the caller owns (stored in +0x60 instead of allocated); +0x76 is
// then clear.
// FUNCTION: STARFLEETOP 0x006751c0
tSurface::tSurface( void *pixels, short x, short y, short width, short height, int a, unsigned char b, unsigned char c )
	: field_60( pixels ), field_70( a ), field_74( c ), field_75( b ), field_76( 0 )
{
	field_3C.x = x;
	field_3C.y = y;
	field_6C.x = 0;
	field_6C.y = 0;
	field_64.mSet( 0, 0, width, height );
	field_34 = tRect( x, y, x + width, y + height );
}

// HYPOTHESIS: 0x006951e0 copies the part of an image inside a rectangle to pixels; the values
// after the destination's pixels are not known.
void fn_006951e0( c_tScrollBar_FieldA8 *image, int x, int y, int left, int top, int right, int bottom,
	void *pixels, int a, int height, unsigned char b, int c, int d );

// HYPOTHESIS: draws an image with its hot spot at a point, clipped to the area given (in the
// owner's coordinates) and to the surface; returns whether anything was drawn.
// FUNCTION: STARFLEETOP 0x00675280
bool tSurface::fn_00675280( c_tScrollBar_FieldA8 *image, int x, int y, const tRect *area, int a, int b )
{
	if ( field_60 == 0 )
		return false;

	tRect clip;
	if ( area != 0 )
	{
		clip = *area;
		clip.mOffset( -field_6C.x, -field_6C.y );
		clip.mClip( &field_64 );
	}
	else
		clip = field_64;

	x -= field_6C.x + image->field_04;
	y -= field_6C.y + image->field_06;
	tRect r( x, y, x + image->field_00, y + image->field_02 );

	if ( !r.mIsEnclosedBy( &clip ) )
	{
		if ( !r.mIntersects( &clip ) )
			return false;
		r.mClip( &clip );
		if ( r.mIsEmpty() )
			return false;
	}

	fn_006951e0( image, x, y, r.left, r.top, r.right, r.bottom, field_60, field_70, field_64.mHeight(), field_74, a, b );
	return true;
}

// HYPOTHESIS: 0x00696ab0 fills a rectangle of pixels in a colour; the values after the
// pixels are those 0x006951e0 takes.
void fn_00696ab0( int color, int left, int top, int right, int bottom, void *pixels, int a,
	int height, unsigned char b );

// HYPOTHESIS: fills a rectangle (in surface coordinates, clipped to the surface) in a colour and,
// when asked, draws a frame around it in another; returns whether anything was drawn.
// FUNCTION: STARFLEETOP 0x00675470
bool tSurface::fn_00675470( int color, const tRect *rect, unsigned char frame, int a, int b )
{
	if ( field_60 == 0 )
		return false;

	tRect clip;
	if ( rect != 0 )
	{
		clip = *rect;
		clip.mClip( &field_64 );
	}
	else
		clip = field_64;

	if ( clip.mIsEmpty() )
		return false;

	fn_00696ab0( color, clip.left, clip.top, clip.right, clip.bottom, field_60, field_70, field_64.mHeight(), field_74 );
	if ( frame )
	{
		fn_006757f0( b, clip.left, clip.top, clip.right, clip.top, a, 0 );
		fn_006757f0( b, clip.left, clip.bottom, clip.right, clip.bottom, a, 0 );
		fn_006757f0( b, clip.left, clip.top, clip.left, clip.bottom, a, 0 );
		fn_006757f0( b, clip.right, clip.top, clip.right, clip.bottom + 1, a, 0 );
	}
	return true;
}

// HYPOTHESIS: 0x00696db0 draws a one-pixel line clipped to a rectangle; the values after the
// pixels are those 0x00696ab0 takes.
void fn_00696db0( int color, int x1, int y1, int x2, int y2, const tRect *clip, void *pixels, int a,
	int height, unsigned char b );

// HYPOTHESIS: draws a line a given number of pixels wide, widened across its direction and
// clipped to the rectangle given and to the surface; returns whether anything was drawn.
// FUNCTION: STARFLEETOP 0x006757f0
bool tSurface::fn_006757f0( int color, int x1, int y1, int x2, int y2, int a, const tRect *clip )
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	tRect r;
	if ( field_60 == 0 )
		return false;

	if ( clip != 0 )
	{
		r = *clip;
		r.mClip( &field_64 );
	}
	else
		r = field_64;

	if ( r.mIsEmpty() )
		return false;

	for ( int i = 0; i < a; i++ )
	{
		if ( dx == 0 )
			fn_00696db0( color, x1 + i, y1, x1 + i, y2, &r, field_60, field_70, field_64.mHeight(), field_74 );
		else if ( dy == 0 || dy < dx )
			fn_00696db0( color, x1, y1 + i, x2, y2 + i, &r, field_60, field_70, field_64.mHeight(), field_74 );
		else
			fn_00696db0( color, x1 + i, y1, x2 + i, y2, &r, field_60, field_70, field_64.mHeight(), field_74 );
	}
	return true;
}

// HYPOTHESIS: fills the part of a second rectangle inside the first (both clipped to the
// surface) in a colour and, when asked, draws a frame of a given width along the inside of the
// first, clipped to the filled part; returns whether anything was drawn.
// FUNCTION: STARFLEETOP 0x006755b0
bool tSurface::fn_006755b0( int color, const tRect *rect, const tRect *inner, unsigned char frame, int a, int b )
{
	tRect fill;
	if ( field_60 == 0 )
		return false;

	tRect clip;
	if ( rect != 0 )
	{
		clip = *rect;
		clip.mClip( &field_64 );
	}
	else
		clip = field_64;

	if ( inner != 0 )
	{
		fill = *inner;
		fill.mClip( &clip );
	}
	else
		fill = clip;

	if ( fill.mIsEmpty() )
		return false;

	fn_00696ab0( color, fill.left, fill.top, fill.right, fill.bottom, field_60, field_70, field_64.mHeight(), field_74 );
	if ( frame )
	{
		fn_006757f0( b, clip.left, clip.top, clip.right - a, clip.top, a, &fill );
		fn_006757f0( b, clip.left, clip.bottom - a, clip.right - a, clip.bottom - a, a, &fill );
		fn_006757f0( b, clip.left, clip.top, clip.left, clip.bottom - a, a, &fill );
		fn_006757f0( b, clip.right - a, clip.top, clip.right - a, clip.bottom - a, a, &fill );
	}
	return true;
}
