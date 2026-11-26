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

#include "unsorted/tHypermass.h"
#include <math.h>

// FUNCTION: STARFLEETOP 0x004f857c
template<>
tHypermass<12>::tHypermass()
	: field_04(12), field_08(1.0f / 12), field_0c((float) sqrt(288.0)),
	  field_10(field_08 * 64.0f), field_14(32.0f), field_18(0.03125f)
{
	fn_004fa6ec();
}

// FUNCTION: STARFLEETOP 0x004f8a02
template<>
tHypermass<16>::tHypermass()
	: field_04(16), field_08(1.0f / 16), field_0c((float) sqrt(512.0)),
	  field_10(field_08 * 64.0f), field_14(32.0f), field_18(0.03125f)
{
	fn_004fa6ec();
}
