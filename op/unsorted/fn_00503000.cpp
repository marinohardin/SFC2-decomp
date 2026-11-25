#include <math.h>
#include "unsorted/fn_0040ce21_type.h"

// HYPOTHESIS: tFx_PPDWave has no header yet (see layout notes); 0x48 bytes from the new below.
class tFx_PPDWave
{
public:
	tFx_PPDWave();
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005032f7
tFx_PPDWave* fn_005032f7()
{
	static fn_0040ce21_type marker("Let's haul it!");
	return new tFx_PPDWave;
}

#include "unsorted/fn_008cc620_object.h"










extern char g_00967479;
extern long g_009672d0;
extern long g_009672d4;

// HYPOTHESIS: 64 cells of five floats from 0x00966dcc, each cell holding a, b, c, d and a
// result that sits in the first float of the next cell.
extern float g_00966dcc[325];
extern float g_007959c8;
extern float g_007a0adc;
extern float g_007a09c0;

// FUNCTION: STARFLEETOP 0x00503354
void fn_00503354()
{
	if( !g_00967479 )
	{
		g_009672d0 = g_008cc620->vfunction2( "assets/textures/ppd-wave.bmp", 1, 0, 0 );
		g_009672d4 = g_008cc620->vfunction2( "assets/textures/ppd-wave-hit.bmp", 1, 0, 0 );
		for( int i = 0; i < 8; i++ )
		{
			float y = ( (float)( i - 4 ) + g_007959c8 ) * g_007a0adc;
			for( int j = 0; j < 8; j++ )
			{
				float *p = &g_00966dcc[( i * 8 + j ) * 5];
				float x = ( (float)( j - 4 ) + g_007959c8 ) * g_007a0adc;
				p[1] = x * g_007a09c0;
				p[2] = y;
				p[3] = x;
				p[4] = y;
				if( j == 0 || i == 0 || j == 7 || i == 7 )
					p[5] = 0;
				else
					p[5] = sqrt( 1.0 - ( p[3] * p[3] + y * y ) * 5.0 );
			}
		}
	}
	g_00967479 = 1;
}
