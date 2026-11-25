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

// HYPOTHESIS: signature and calling convention of the callees are unknown.
void __stdcall fn_00488e97(int a, int b);

struct fn_00521db5_owner {
	void fn_00521db5(int param_1, int param_2);
	void fn_00521e22(int a, int b, int c);
	int field_0;
	int field_4;
	int field_8;
};

// FUNCTION: STARFLEETOP 0x00521db5
void fn_00521db5_owner::fn_00521db5(int param_1, int param_2)
{
	fn_00488e97(field_4, field_8);
	fn_00521e22(field_4,param_1, param_2);
}

struct fn_00521dd9_owner {
	void fn_00521dd9();
	void fn_005220d5(int* a, int* b);
	int field_0;
	int* field_4;
	int field_8;
};

void __cdecl operator delete(void*);

// FUNCTION: STARFLEETOP 0x00521dd9
void fn_00521dd9_owner::fn_00521dd9()
{
	fn_00521dd9_owner* local = this;
	int* head = field_4;
	int* node = (int*)*head;
	while (node != head) {
		int* next = (int*)*node;
		fn_005220d5((int*)&local, node);
		node = next;
	}
	operator delete(field_4);
	field_4 = 0;
	field_8 = 0;
}
