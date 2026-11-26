// Unit:      unsorted/fn_004AF706.cpp
// Functions: 0x004af706 to 0x004aff50, 19 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Tiger/tvectormath.h"
#include "unsorted/tValueBase.h"
#include "unsorted/fn_004D3143.h"
#include <string.h>

// HYPOTHESIS: builds a look-at (view) matrix from an eye position, a target and an up vector.
// FUNCTION: STARFLEETOP 0x004af706
void fn_004af706(const tPoint3 &eye, const tPoint3 &target, const tPoint3 &up, tMatrix &m)
{
	tPoint3 axisZ = eye - target;
	axisZ.mNormalize();
	tPoint3 axisX = up % axisZ;
	axisX.mNormalize();
	tPoint3 axisY = axisZ % axisX;

	tMtx4x4 &mtx = m.mGetMtx4x4();
	mtx[0][0] = axisX.x;
	mtx[0][1] = axisX.y;
	mtx[0][2] = axisX.z;
	mtx[0][3] = 0.0f;
	mtx[1][0] = axisY.x;
	mtx[1][1] = axisY.y;
	mtx[1][2] = axisY.z;
	mtx[1][3] = 0.0f;
	mtx[2][0] = axisZ.x;
	mtx[2][1] = axisZ.y;
	mtx[2][2] = axisZ.z;
	mtx[2][3] = 0.0f;
	mtx[3][0] = 0.0f;
	mtx[3][1] = 0.0f;
	mtx[3][2] = 0.0f;
	mtx[3][3] = 1.0f;
	m.mTranslate(-eye.x, -eye.y, -eye.z);
}

// HYPOTHESIS: a table of control names (every other word, up to 0x008515cc), one value name,
// and two zero-terminated tables of value names, all unbound when the tactical state ends.
extern const char *g_00850fec[];
extern const char g_00852424[];
extern const char *g_0084c540[];
extern const char *g_0084d1c8[];
void fn_004bf8d6();

// FUNCTION: STARFLEETOP 0x004af94b
void fn_004af94b()
{
	const char **p;
	for (p = g_00850fec; (int)p < (int)&g_00850fec[0x178]; p += 2)
		fn_004d35fa( *p );
	tValueBase *v = fn_004d35bf( g_00852424, 0 );
	if (v != 0)
		delete v;
	int i;
	for (i = 0; g_0084c540[i] != 0; i++)
	{
		v = fn_004d35bf( g_0084c540[i], 0 );
		if (v != 0)
			delete v;
	}
	for (i = 0; g_0084d1c8[i] != 0; i++)
	{
		v = fn_004d35bf( g_0084d1c8[i], 0 );
		if (v != 0)
			delete v;
	}
	fn_004bf8d6();
}

// HYPOTHESIS: constructor of one of tState_Tactical's growable arrays {data, count, capacity}
// with 0x1c-byte elements (the arrays at +0x410 and +0x41c); the capacity is at least 10.
// The class is not in a header, so it is written as a free function on the array's words.
// FUNCTION: STARFLEETOP 0x004afa1c
void *__fastcall fn_004afa1c(void *self, int, unsigned capacity)
{
	if (capacity < 10)
		capacity = 10;
	((unsigned *)self)[2] = capacity;
	((void **)self)[0] = operator new( capacity * 0x1c );
	return self;
}

// HYPOTHESIS: destructor of the same growable array; frees the element storage.
// FUNCTION: STARFLEETOP 0x004afa40
void __fastcall fn_004afa40(void *self)
{
	if (((void **)self)[0] != 0)
		operator delete( ((void **)self)[0] );
}

// HYPOTHESIS: appends one element to the same growable array, growing it by ten when full; the
// element is four words copied from the first argument followed by three more words. The first
// argument's type is unknown; a 16-byte struct copy (tPoint4) gives the original's block copy.
// FUNCTION: STARFLEETOP 0x004afa4e
void __fastcall fn_004afa4e(void *self, int, const tPoint4 *head, int a, int b, int c)
{
	int *array = (int *)self;
	if (array[1] + 1 >= array[2])
	{
		int capacity = array[2] + 10;
		int *data = (int *)operator new( capacity * 0x1c );
		if (data == 0)
			return;
		if (array[0] != 0)
		{
			memcpy( data, (void *)array[0], array[1] * 0x1c );
			operator delete( (void *)array[0] );
		}
		array[2] = capacity;
		array[0] = (int)data;
	}
	int *e = (int *)(array[1] * 0x1c + array[0]);
	array[1]++;
	*(tPoint4 *)e = *head;
	e[4] = a;
	e[5] = b;
	e[6] = c;
}

// HYPOTHESIS: appends to the same growable array; the element's first four words and then its
// next two words come from the first argument, the last word is the second argument.
// FUNCTION: STARFLEETOP 0x004afac9
void __fastcall fn_004afac9(void *self, int, const tPoint4 *head, int c)
{
	int *array = (int *)self;
	if (array[1] + 1 >= array[2])
	{
		int capacity = array[2] + 10;
		int *data = (int *)operator new( capacity * 0x1c );
		if (data == 0)
			return;
		if (array[0] != 0)
		{
			memcpy( data, (void *)array[0], array[1] * 0x1c );
			operator delete( (void *)array[0] );
		}
		array[2] = capacity;
		array[0] = (int)data;
	}
	int *e = (int *)(array[1] * 0x1c + array[0]);
	array[1]++;
	*(tPoint4 *)e = *head;
	e[4] = ((const int *)head)[0];
	e[5] = ((const int *)head)[1];
	e[6] = c;
}

// HYPOTHESIS: constructor of another growable array {data, count, capacity} with 4-byte
// elements; the capacity is at least 10. Written on the array's words like 0x004afa1c.
// FUNCTION: STARFLEETOP 0x004afb43
void *__fastcall fn_004afb43(void *self, int, unsigned capacity)
{
	if (capacity < 10)
		capacity = 10;
	((unsigned *)self)[2] = capacity;
	((void **)self)[0] = operator new( capacity * 4 );
	return self;
}

// HYPOTHESIS: appends one word to the 4-byte growable array of 0x004afb43, growing it by ten
// when full.
// FUNCTION: STARFLEETOP 0x004afb67
void __fastcall fn_004afb67(void *self, int, int value)
{
	int *array = (int *)self;
	if (array[1] + 1 >= array[2])
	{
		int capacity = array[2] + 10;
		int *data = (int *)operator new( capacity * 4 );
		if (data == 0)
			return;
		if (array[0] != 0)
		{
			memcpy( data, (void *)array[0], array[1] * 4 );
			operator delete( (void *)array[0] );
		}
		array[2] = capacity;
		array[0] = (int)data;
	}
	((int *)array[0])[array[1]] = value;
	array[1]++;
}

// HYPOTHESIS: constructor of a growable array {data, count, capacity} with 20-byte elements;
// the capacity is at least 10, like 0x004afb43.
// FUNCTION: STARFLEETOP 0x004afbc4
void *__fastcall fn_004afbc4(void *self, int, unsigned capacity)
{
	if (capacity < 10)
		capacity = 10;
	((unsigned *)self)[2] = capacity;
	((void **)self)[0] = operator new( capacity * 0x14 );
	return self;
}

// HYPOTHESIS: appends to the 20-byte growable array of 0x004afbc4, growing it by ten when full;
// the element is one word followed by four words copied from the second argument.
// FUNCTION: STARFLEETOP 0x004afbeb
void __fastcall fn_004afbeb(void *self, int, int key, const tPoint4 *value)
{
	int *array = (int *)self;
	if (array[1] + 1 >= array[2])
	{
		int capacity = array[2] + 10;
		int *data = (int *)operator new( capacity * 0x14 );
		if (data == 0)
			return;
		if (array[0] != 0)
		{
			memcpy( data, (void *)array[0], array[1] * 0x14 );
			operator delete( (void *)array[0] );
		}
		array[2] = capacity;
		array[0] = (int)data;
	}
	((int *)array[0])[array[1] * 5] = key;
	*(tPoint4 *)((int *)array[0] + array[1] * 5 + 1) = *value;
	array[1]++;
}

// HYPOTHESIS: constructor of a growable array {data, count, capacity} with 8-byte elements;
// the capacity is at least 10, like 0x004afbc4.
// FUNCTION: STARFLEETOP 0x004afc63
void *__fastcall fn_004afc63(void *self, int, unsigned capacity)
{
	if (capacity < 10)
		capacity = 10;
	((unsigned *)self)[2] = capacity;
	((void **)self)[0] = operator new( capacity * 8 );
	return self;
}

// HYPOTHESIS: appends a pair of words to the 8-byte growable array of 0x004afc63, growing it by
// ten when full.
// FUNCTION: STARFLEETOP 0x004afc87
void __fastcall fn_004afc87(void *self, int, int first, int second)
{
	int *array = (int *)self;
	if (array[1] + 1 >= array[2])
	{
		int capacity = array[2] + 10;
		int *data = (int *)operator new( capacity * 8 );
		if (data == 0)
			return;
		if (array[0] != 0)
		{
			memcpy( data, (void *)array[0], array[1] * 8 );
			operator delete( (void *)array[0] );
		}
		array[2] = capacity;
		array[0] = (int)data;
	}
	((int *)array[0])[array[1] * 2] = first;
	((int *)array[0])[array[1] * 2 + 1] = second;
	array[1]++;
}
