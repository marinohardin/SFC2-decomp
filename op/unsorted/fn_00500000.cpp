#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tBmpSurf.h"
#include "Source/entity/entity_physical.h"

// HYPOTHESIS: tFx_Phaser has no header yet; this is the shape the constructor call and
// the 0x4c allocation show. It belongs in a shared header once one exists.
class tFx_Phaser
{
public:
	tFx_Phaser( tBmpSurf *bmp );

	char	pad00[0x4c];
};

// FUNCTION: STARFLEETOP 0x0050037b
tFx_Phaser *fn_0050037b( tBmpSurf *arg )
{
	static tObject_0040ce21 marker("You'll need more than that");
	return new tFx_Phaser( arg );
}

float fn_005050e2(float aValue);
extern float g_009669fc;

// HYPOTHESIS: the original bytes are the tail of a larger function; this is the closest standalone form.
// FUNCTION: STARFLEETOP 0x0050048f
void fn_0050048f()
{
	g_009669fc = fn_005050e2(64.0f);
}

extern float g_00966a04;

// HYPOTHESIS: the original bytes are the tail of a larger function; this is the closest standalone form.
// FUNCTION: STARFLEETOP 0x005004b7
void fn_005004b7()
{
	g_00966a04 = fn_005050e2(127.0f);
}

extern float g_00966a08;

// HYPOTHESIS: the original bytes are the tail of a larger function; this is the closest standalone form.
// FUNCTION: STARFLEETOP 0x005004cb
void fn_005004cb()
{
	g_00966a08 = fn_005050e2(128.0f);
}

extern float g_00966ab0;

// HYPOTHESIS: the original bytes are the tail of a larger function; this is the closest standalone form.
// FUNCTION: STARFLEETOP 0x00500803
void fn_00500803()
{
	g_00966ab0 = fn_005050e2(255.0f);
}

// HYPOTHESIS: the object held by the global at 0x008cc620; its third virtual loads a texture
// by file name and returns a handle. No header declares it yet; a shared one should replace this.
class fn_008cc620_object_00500847
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2( const char *name, long a, long b, long c );
};

extern fn_008cc620_object_00500847 *g_008cc620;
extern char g_00966ac0;
extern long g_00857fc4;

// FUNCTION: STARFLEETOP 0x00500847
void fn_00500847()
{
	if( !g_00966ac0 )
	{
		g_00857fc4 = g_008cc620->vfunction2( "assets/textures/photons-2.bmp", 1, 0, 0 );
		g_00966ac0 = 1;
	}
}

// HYPOTHESIS: no header declares tFx_Base or the array element yet; minimal forms so the constructor can be written.
class tFx_Base
{
public:
	tFx_Base();
	virtual ~tFx_Base();
	char m04[0x0a];
	char m0e;
	char m0f;
	long m10;
	char m14[0x28];
	char m3c;
	char m3d[3];
};

class fn_005008f4_element
{
public:
	fn_005008f4_element();
	void fn_00500976( tObject_00413ac3_arg *renderer, float x, float y, float z );
	char m00[0x18];
};

class tFx_Photon : public tFx_Base
{
public:
	tFx_Photon();
	fn_005008f4_element m40[16];
};

extern long g_007aa8d0;

// FUNCTION: STARFLEETOP 0x00500872
tFx_Photon::tFx_Photon()
{
	m0e = 1;
	m3c = 1;
	m10 = g_007aa8d0;
}

float fn_0069b0e0();
extern double g_007aa8c8;
extern float g_0096699c;
extern float g_009669a0;

// FUNCTION: STARFLEETOP 0x005008f4
fn_005008f4_element::fn_005008f4_element()
{
	float a = fn_0069b0e0() * 2.0f - 1.0f;
	float b = fn_0069b0e0() * 2.0f - 1.0f;
	float c = fn_0069b0e0() * 2.0f - 1.0f;
	*(float *)&m00[4] = b;
	*(float *)&m00[8] = a;
	*(float *)&m00[0] = c;
	double t = fn_0069b0e0() * g_007aa8c8;
	*(float *)&m00[12] = t + t;
	*(float *)&m00[16] = (g_009669a0 - g_0096699c) * fn_0069b0e0() + g_0096699c;
	*(float *)&m00[20] = fn_0069b0e0() * 5.0f;
}

// MATCH-ONLY: stand-in for the tMatrix member at 0x0069b450, which tvectormath.h does not declare yet.
class fn_00500976_matrix : public tMatrix
{
public:
	void fn_0069b450( tMatrix *other );
};

void fn_00500b46( tMatrix *, tObject_00412f08, float );
extern float g_0092f61c;
extern float g_00966ab0;
extern float g_00966ab4;
extern float g_00966ab8;
extern float g_00966abc;

// HYPOTHESIS: the element draws itself as a quad; its point is at 0 and the floats at 0xc, 0x10, 0x14.
// FUNCTION: STARFLEETOP 0x00500976
void fn_005008f4_element::fn_00500976( tObject_00413ac3_arg *renderer, float x, float y, float z )
{
	tMatrix a;
	fn_00500b46( &a, *(tPoint3 *)&m00[0], g_0092f61c * *(float *)&m00[16] + *(float *)&m00[12] );
	fn_00500976_matrix b;
	b.mIdentity();
	b.mTranslate( x, y, z );
	b.fn_0069b450( &a );
	renderer->vfunction57( &b );
	float f = *(float *)&m00[20] * 0.1f;
	float f2 = *(float *)&m00[20] * 0.1f;
	renderer->vfunction76( 1.0f, 1.0f, 1.0f );
	renderer->vfunction67( 7 );
	renderer->vfunction73( g_00966ab0, g_00966ab4 );
	renderer->vfunction69( -f, 0.0f, 0.0f );
	renderer->vfunction73( g_00966ab8, g_00966ab4 );
	renderer->vfunction69( f, 0.0f, 0.0f );
	renderer->vfunction73( g_00966ab8, g_00966abc );
	renderer->vfunction69( f2, *(float *)&m00[20], 0.0f );
	renderer->vfunction73( g_00966ab0, g_00966abc );
	renderer->vfunction69( -f2, *(float *)&m00[20], 0.0f );
	renderer->vfunction68();
}

