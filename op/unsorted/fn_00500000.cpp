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






































#include "unsorted/fn_008cc620_object.h"










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

void fn_00500b46( tMatrix *, tPoint3, float );
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

extern "C" double fn_007381c4( double );
extern "C" double fn_00737fa4( double );

// HYPOTHESIS: builds the rotation matrix of an angle (radians) about an axis, in tMatrix's layout.
// FUNCTION: STARFLEETOP 0x00500b46
void fn_00500b46( tMatrix *m, tPoint3 axis, float angle )
{
	float c = (float)fn_007381c4( angle );
	float s = (float)fn_00737fa4( angle );
	axis.mNormalize();
	float t = 1.0f - c;
	float *f = (float *)m;
	*((unsigned char *)m + 0x40) = 0;
	f[0] = axis.x * axis.x * t + c;
	float xy = axis.y * axis.x * t;
	f[1] = xy - axis.z * s;
	float xz = axis.z * axis.x * t;
	f[2] = axis.y * s + xz;
	f[4] = axis.z * s + xy;
	f[5] = axis.y * axis.y * t + c;
	float yz = axis.z * axis.y * t;
	f[6] = yz - axis.x * s;
	f[8] = xz - axis.y * s;
	f[9] = axis.x * s + yz;
	f[10] = axis.z * axis.z * t + c;
	f[11] = 0.0f;
	f[7] = 0.0f;
	f[3] = 0.0f;
	f[14] = 0.0f;
	f[13] = 0.0f;
	f[12] = 0.0f;
	f[15] = 1.0f;
}

