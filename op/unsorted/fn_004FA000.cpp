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

extern float g_0092f61c;
extern "C" double fn_00737fa4(double);
// Four floats the original reads from this unit's constant data (values by Ghidra); the two
// equal scales sit at separate addresses, so they are named constants, not pooled literals.
// HYPOTHESIS: file-scope const floats in this order.
const float g_007aa550 = 0.25f;
const float g_007aa554 = 0.2f;
const float g_007aa558 = 0.046875f;
const float g_007aa55c = 0.046875f;
// 2.0, read by 0x004fa662 from the slot after the four above.
const float g_007aa560 = 2.0f;

// HYPOTHESIS: a member of the N = 12 instance, like 0x004fa526; returns cell (row, column)
// turned by an angle from its weight and the clock.
// FUNCTION: STARFLEETOP 0x004fa58f
template<>
tPoint2 tHypermass<12>::fn_004fa58f(unsigned int param_2, unsigned int param_3)
{
	int n = field_04;
	float x = ((int) param_3 - n * 0.5f) * field_10;
	x *= g_007aa558;
	float y = ((int) param_2 - n * 0.5f) * field_10;
	y *= g_007aa55c;
	if (param_2 >= n)
		param_2 -= n;
	if (param_3 >= n)
		param_3 -= n;
	float s = (float) sin(field_1c[param_2 * 12 + param_3][3] * g_007aa554 + g_0092f61c * g_007aa550);
	double c = fn_00737fa4(field_1c[param_2 * 12 + param_3][3] * g_007aa554 + g_0092f61c * g_007aa550);
	tPoint2 p;
	p.x = x * s - y * c;
	p.y = y * s + x * c;
	return p;
}

// HYPOTHESIS: one point (a colour) in the data segment; the original scales its three members
// last first, as the arguments of an inlined setter.
extern tPoint3 g_00946158;
// A 1.0 the original reads from a shared constant slot (other units read it too), while its
// first comparison uses a pooled 1.0.
extern float g_007959cc;

// HYPOTHESIS: a member of the N = 12 instance, like 0x004fa526; writes a grey or tinted
// brightness from a distance, clamped to [0, 1].
// FUNCTION: STARFLEETOP 0x004fa662
template<>
void tHypermass<12>::fn_004fa662(tPoint3 *param_1, float param_2, char param_3)
{
	float d = param_2 - g_007aa560;
	if (d >= 1.0f)
		param_2 = g_007959cc - d * field_18;
	else
		param_2 = d;
	if (param_2 < 0.0f)
		param_2 = 0.0f;
	else if (param_2 > g_007959cc)
		param_2 = 1.0f;
	if (param_3)
		param_1->mSet(g_00946158.x * param_2, g_00946158.y * param_2, g_00946158.z * param_2);
	else
	{
		param_1->x = param_2;
		param_1->y = param_2;
		param_1->z = param_2;
	}
}

// The N = 16 instance of 0x004fa526 (row stride 16 cells).
// FUNCTION: STARFLEETOP 0x004fa9bc
template<>
tPoint3 tHypermass<16>::fn_004fa526(unsigned int param_2, unsigned int param_3)
{
	int n = field_04;
	tPoint3 p;
	p.x = ((int) param_3 - n * 0.5f) * field_10;
	p.y = ((int) param_2 - n * 0.5f) * field_10;
	if (param_2 >= n)
		param_2 -= n;
	if (param_3 >= n)
		param_3 -= n;
	p.z = -field_1c[param_2 * 16 + param_3][3] - 2.0f;
	return p;
}
