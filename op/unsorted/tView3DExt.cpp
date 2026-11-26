#define tvectormath_spark_members
#include "unsorted/tView3DExt.h"

// HYPOTHESIS: the original calls 0x0069cc40 (another tMatrix member taking an output point and an
// input point) where this body calls fn_0069cbc0 three times; tMatrix declares no member for it yet.
// FUNCTION: STARFLEETOP 0x006aa550
void tView3DExt::vfunction58(tMatrix *m)
{
	field_0x6f4 = *m;
	field_0x628.mMult(field_0x66c, *m);
	field_0x5e4.mMult(field_0x6b0, *m);
	tMatrix product(tMatrix::kUninitialized);
	field_0x628.mMult(product, *m);
	static tPoint3 origin(0.0f, 0.0f, 0.0f);
	product.fn_0069cbc0(*(tPoint3 *)field_0x744, origin);
	m->fn_0069cbc0(*(tPoint3 *)field_0x750, *(tPoint3 *)field_0x738);
	for (int i = 0; i < 0x40; i++) {
		if (field_0x1778[i] == 0)
			return;
		char *light = field_0x778[i];
		tPoint3 *pos = (tPoint3 *)(light + 0x10);
		tPoint3 *dir = (tPoint3 *)(light + 4);
		switch (*(int *)light) {
		case 1:
		case 3:
		case 5:
			m->fn_0069cbc0(*(tPoint3 *)field_0x17b8[i], *pos);
			break;
		case 2:
		case 4:
		case 6:
			m->fn_0069cbc0(*(tPoint3 *)field_0x1ab8[i], *dir);
			m->fn_0069cbc0(*(tPoint3 *)field_0x17b8[i], *pos);
			break;
		default:
			m->fn_0069cbc0(*(tPoint3 *)field_0x1ab8[i], *dir);
		}
	}
}
