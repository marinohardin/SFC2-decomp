#include "unsorted/fn_008cc620_object.h"










extern char g_00966d78;
extern long g_00966acc;
extern long g_00966ad0;
extern long g_00966ad4;
extern long g_00966ad8;
extern long g_00966adc;
extern long g_00966ae0;
extern long g_00966ae4;

// FUNCTION: STARFLEETOP 0x00501b0e
void fn_00501b0e()
{
	if( !g_00966d78 )
	{
		g_00966acc = g_008cc620->vfunction2( "assets/textures/red-plasma.bmp", 1, 0, 0 );
		g_00966ad0 = g_008cc620->vfunction2( "assets/textures/orange-plasma.bmp", 1, 0, 0 );
		g_00966ad4 = g_008cc620->vfunction2( "assets/textures/yellow-plasma.bmp", 1, 0, 0 );
		g_00966ad8 = g_008cc620->vfunction2( "assets/textures/green-plasma.bmp", 1, 0, 0 );
		g_00966adc = g_008cc620->vfunction2( "assets/textures/violet-plasma.bmp", 1, 0, 0 );
		g_00966ae0 = g_008cc620->vfunction2( "assets/textures/FastPlasma.bmp", 1, 0, 0 );
		g_00966ae4 = g_008cc620->vfunction2( "assets/textures/PlasmaSnare.bmp", 1, 0, 0 );
		g_00966d78 = 1;
	}
}
