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

// HYPOTHESIS: a member of the N = 12 instance (row stride 12 cells); the wrap reads field_04.
// Returns the position of cell (row, column); the height lookup wraps indices past N.
// FUNCTION: STARFLEETOP 0x004fa526
template<>
tPoint3 tHypermass<12>::fn_004fa526(unsigned int param_2, unsigned int param_3)
{
	int n = field_04;
	tPoint3 p;
	p.x = ((int) param_3 - n * 0.5f) * field_10;
	p.y = ((int) param_2 - n * 0.5f) * field_10;
	if (param_2 >= n)
		param_2 -= n;
	if (param_3 >= n)
		param_3 -= n;
	p.z = -field_1c[param_2 * 12 + param_3][3] - 2.0f;
	return p;
}
