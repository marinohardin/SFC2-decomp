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
