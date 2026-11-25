#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tBmpSurf.h"

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
