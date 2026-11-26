#include "unsorted/tObject_0040ce21.h"

// HYPOTHESIS: the effect class built here; its constructor is 0x004f81e2 and it is 0x3c bytes.
class tFx_Hypermass
{
public:
	tFx_Hypermass();

	char	pad00[0x3c];
};

// FUNCTION: STARFLEETOP 0x004f851f
// HYPOTHESIS: a factory that builds a hypermass effect and, once, a report object.
tFx_Hypermass *fn_004f851f()
{
	static tObject_0040ce21 report("Holy Space Anomolies Batman");
	return new tFx_Hypermass;
}
