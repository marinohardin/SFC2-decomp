#include "unsorted/tHypermass.h"
#include <math.h>

// FUNCTION: STARFLEETOP 0x004f930e
template<>
tHypermass<24>::tHypermass()
	: field_04(24), field_08(1.0f / 24), field_0c((float) sqrt(1152.0)),
	  field_10(field_08 * 64.0f), field_14(32.0f), field_18(0.03125f)
{
	fn_004fa6ec();
}

// FUNCTION: STARFLEETOP 0x004f9794
template<>
tHypermass<28>::tHypermass()
	: field_04(28), field_08(1.0f / 28), field_0c((float) sqrt(1568.0)),
	  field_10(field_08 * 64.0f), field_14(32.0f), field_18(0.03125f)
{
	fn_004fa6ec();
}
