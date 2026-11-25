#include "unsorted/fn_008cc620_object.h"










extern char g_00967978;
extern long g_00967724;
extern long g_00967728;
extern long g_0096772c;

// FUNCTION: STARFLEETOP 0x0050678f
void fn_0050678f()
{
	if( !g_00967978 )
	{
		g_00967724 = g_008cc620->vfunction2( "assets/textures/expl_00-15.bmp", 1, 0, 0 );
		g_00967728 = g_008cc620->vfunction2( "assets/textures/expl_16-31.bmp", 1, 0, 0 );
		g_0096772c = g_008cc620->vfunction2( "assets/textures/expl_ring16f.bmp", 1, 0, 0 );
	}
	g_00967978 = 1;
}
