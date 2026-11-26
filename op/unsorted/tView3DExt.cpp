#define tvectormath_spark_members
#include "unsorted/tView3DExt.h"

// FUNCTION: STARFLEETOP 0x0069f190
bool tView3DExt::vfunction38()
{
	return field_0x1eb8;
}

// FUNCTION: STARFLEETOP 0x0069fda0
bool tView3DExt::vfunction47(bool flag)
{
	bool old = field_0x1ed8;
	field_0x1ed8 = flag ? true : false;    // HYPOTHESIS: the original tests the argument again (it may have been a BOOL-like type)
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9620
int tView3DExt::vfunction23(int mode)
{
	int old = field_0x1ebc;
	switch (mode) {
	case 6:
		field_0x1ebc = 6;
		field_0x1ec0 = 1;
		break;
	case 7:
		field_0x1ebc = 7;
		field_0x1ec0 = 0;
		break;
	}
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9660
int tView3DExt::vfunction24()
{
	return field_0x1ebc;
}

// FUNCTION: STARFLEETOP 0x006a9670
int tView3DExt::vfunction19(int mode)
{
	int old = field_0x1ec4;
	switch (mode) {
	case 0:
		field_0x1ec4 = 0;
		field_0x1ec8 = 0;
		break;
	case 1:
		field_0x1ec4 = 1;
		field_0x1ec8 = 1;
		break;
	case 2:
		field_0x1ec4 = 2;
		field_0x1ec8 = 2;
		break;
	}
	return old;
}

// FUNCTION: STARFLEETOP 0x006a96d0
int tView3DExt::vfunction20()
{
	return field_0x1ec4;
}

// FUNCTION: STARFLEETOP 0x006a96e0
int tView3DExt::vfunction21(int mode)
{
	int old = field_0x1ecc;
	// HYPOTHESIS: the original compares unsigned (both tests are unsigned jumps).
	if ((unsigned int)mode >= 3 && (unsigned int)mode <= 5) {
		field_0x1ecc = mode;
	}
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9700
int tView3DExt::vfunction22()
{
	return field_0x1ecc;
}

// HYPOTHESIS: the original returns the previous level as a float (it loads field_0x1ed4 first and
// leaves it on the FPU stack; 0x006a9540 pops the result); tView3D declares the slot void.
// FUNCTION: STARFLEETOP 0x006a9710
void tView3DExt::vfunction45(float level)
{
	if (level < 0.0f) {
		field_0x1ed4 = 0.0f;
	} else if (level > 1.0f) {
		field_0x1ed4 = 1.0f;
	} else {
		field_0x1ed4 = level;
	}
	// The byte copy is rounded by adding 1.5 * 2^23 and keeping the low mantissa byte.
	level = field_0x1ed4 * 255.0f + 12582912.0f;
	field_0x1ed9 = *(unsigned char *)&level;
}

// FUNCTION: STARFLEETOP 0x006a9780
float tView3DExt::vfunction46()
{
	return field_0x1ed4;
}

// FUNCTION: STARFLEETOP 0x006a9aa0
int tView3DExt::vfunction32()
{
	return 0x40;
}

// FUNCTION: STARFLEETOP 0x006a9ab0
void tView3DExt::vfunction33(unsigned int index, const void *light)
{
	if (index < 0x40) {
		const int *src = (const int *)light;
		*(int *)&field_0x778[index][0x00] = src[0];
		// HYPOTHESIS: the light record holds the direction (+4) and position (+0x10) points that
		// vfunction58 transforms; copying them as points gives the original's store addressing.
		*(tPoint3 *)&field_0x778[index][0x04] = *(const tPoint3 *)&src[1];
		*(tPoint3 *)&field_0x778[index][0x10] = *(const tPoint3 *)&src[4];
		*(int *)&field_0x778[index][0x1c] = src[7];
		*(int *)&field_0x778[index][0x20] = src[8];
		*(int *)&field_0x778[index][0x24] = src[9];
		*(int *)&field_0x778[index][0x28] = src[10];
		*(int *)&field_0x778[index][0x2c] = src[11];
		*(int *)&field_0x778[index][0x30] = src[12];
		*(int *)&field_0x778[index][0x34] = src[13];
		*(int *)&field_0x778[index][0x38] = src[14];
		*(int *)&field_0x778[index][0x3c] = src[15];
		int type = src[0];
		if ((type == 1 || (type > 4 && type <= 6)) && ((float *)field_0x778[index])[10] != 0.0f)
			field_0x1db8[index] = 1.0f / ((float *)field_0x778[index])[10];
	}
}

// FUNCTION: STARFLEETOP 0x006a9ba0
void tView3DExt::vfunction34(unsigned int index, void *light)
{
	if (index < 0x40) {
		const int *src = (const int *)field_0x778[index];
		int *dst = (int *)light;
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = src[3];
		dst[4] = src[4];
		dst[5] = src[5];
		dst[6] = src[6];
		dst[7] = src[7];
		dst[8] = src[8];
		dst[9] = src[9];
		dst[10] = src[10];
		dst[11] = src[11];
		dst[12] = src[12];
		dst[13] = src[13];
		dst[14] = src[14];
		dst[15] = src[15];
	}
}

// FUNCTION: STARFLEETOP 0x006a9c20
bool tView3DExt::vfunction37(bool flag)
{
	bool old = field_0x1eb8;
	field_0x1eb8 = flag;
	return old;
}

// FUNCTION: STARFLEETOP 0x006a9c40
bool tView3DExt::vfunction39(unsigned int index, bool enable)
{
	if (index < 0x40)
	{
		field_0x1778[index] = enable;
		return true;
	}
	return false;
}

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

// HYPOTHESIS: tView3D declares vfunction41/42/53/54 without the arguments this function passes
// (HEADER REQUEST pending); the member-pointer casts give those calls their real shapes.
typedef bool (tView3D::*tVFn41)(bool);
typedef void (tView3D::*tVFn42)(int, int);
typedef bool (tView3D::*tVFn53)();
typedef void (tView3D::*tVFn54)(int);

// FUNCTION: STARFLEETOP 0x006ac5a0
void tView3DExt::vfunction81(int a, void **b, bool c)
{
	tVFn41 f41 = (tVFn41)&tView3D::vfunction41;
	tVFn42 f42 = (tVFn42)&tView3D::vfunction42;
	tVFn53 f53 = (tVFn53)&tView3D::vfunction53;
	tVFn54 f54 = (tVFn54)&tView3D::vfunction54;
	int saved = vfunction96();
	bool old = vfunction48(false);
	vfunction48(old);
	if ((vfunction10() & 0x20) == 0) {
		(this->*f54)(0);
		vfunction97(1, a, b, c);
		return;
	}
	if ((this->*f53)()) {
		vfunction97(0, a, b, c);
		return;
	}
	unsigned char flags;
	if (old ? (flags & 2) == 0 : (flags & 1) == 0) {
		(this->*f54)(0);
		vfunction97(1, a, b, c);
		return;
	}
	(this->*f54)(2);
	vfunction97(2, a, b, c);
	bool lit = vfunction37(false);
	bool prev = (this->*f41)(true);
	(this->*f42)(1, 4);
	(this->*f54)(1);
	vfunction97(1, a, b, c);
	(this->*f41)(prev);
	vfunction37(lit);
	(this->*f54)(0);
}
