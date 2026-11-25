#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tBmpSurf.h"

// HYPOTHESIS: tFx_Plasma has no header yet; this is the shape the constructor call and
// the 0x1fa0 allocation show. It belongs in a shared header once one exists.
class tFx_Plasma
{
public:
	tFx_Plasma( tBmpSurf *bmp );

	char	pad00[0x1fa0];
};

// FUNCTION: STARFLEETOP 0x005023f9
tFx_Plasma *fn_005023f9( tBmpSurf *arg )
{
	static tObject_0040ce21 marker("Toot Toot");
	return new tFx_Plasma( arg );
}

// HYPOTHESIS: the object held by the global at 0x008cc620; its third virtual loads a texture
// by file name and returns a handle. No header declares it yet; a shared one should replace this.
class fn_008cc620_object_00502b0a
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2( const char *name, long a, long b, long c );
};

extern fn_008cc620_object_00502b0a *g_008cc620;
extern char g_00967478;
extern long g_009672dc;

// FUNCTION: STARFLEETOP 0x00502b0a
void fn_00502b0a()
{
	if( !g_00967478 )
	{
		g_009672dc = g_008cc620->vfunction2( "assets/textures/ppd-beam.bmp", 1, 0, 0 );
	}
	g_00967478 = 1;
}
