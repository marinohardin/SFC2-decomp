#include "unsorted/tHypermass.h"
#include <math.h>

// FUNCTION: STARFLEETOP 0x004fa0a0
template<>
tHypermass<36>::tHypermass()
	: field_04(36), field_08(1.0f / 36), field_0c((float) sqrt(2592.0)),
	  field_10(field_08 * 64.0f), field_14(32.0f), field_18(0.03125f)
{
	fn_004fa6ec();
}
