// Unit:      unsorted/fn_00521D73.cpp
// Functions: 0x00521d73 to 0x00521e22, 5 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/fn_0090bed4_Object.h"

// HYPOTHESIS: signature and calling convention of the callee are unknown.
void fn_0051cb7d(int a, int b);

// FUNCTION: STARFLEETOP 0x00521d73
void tObject_00521d73::fn_00521d73(int param_1)
{
	if (this != (tObject_00521d73*)param_1) {
		*(int*)((char*)this + 4) = *(int*)(param_1 + 4);
		int* p = (int*)(param_1 + 0xc);
		int n = 2;
		do {
			fn_0051cb7d(p[0], p[1]);
			p += 4;
			n--;
		} while (n != 0);
	}
}
