#include "Common/Database/DatabaseID.h"
#include "unsorted/tHexPoint.h"

// FUNCTION: STARFLEETOP 0x005bc012
int __fastcall FUN_005bc012(int *param_1)
{
	return *param_1 - 0x2c;
}

// HYPOTHESIS: a 24-byte element whose destructor is 0x005bb6fc.
struct fn_005bb6fc_elem
{
	char m00[0x18];

	void FUN_005bb6fc();
};

// HYPOTHESIS: an STL vector of 24-byte elements (allocator byte at +0,
// first/last/end at +4/+8/+0xc); this is its _Tidy.
struct fn_005bc018_vector
{
	char m00;
	fn_005bb6fc_elem *_First;
	fn_005bb6fc_elem *_Last;
	fn_005bb6fc_elem *_End;

	void FUN_005bc018();
};

// FUNCTION: STARFLEETOP 0x005bc018
void fn_005bc018_vector::FUN_005bc018()
{
	fn_005bb6fc_elem *l = _Last;
	for (fn_005bb6fc_elem *p = _First; p != l; p++)
		p->FUN_005bb6fc();
	operator delete(_First);
	_First = 0, _Last = 0, _End = 0;
}

// HYPOTHESIS: takes the owner's +8 field, a flag and a value; the callee
// pops its own arguments.
void __stdcall FUN_005bce21(int param_1, int param_2, int param_3);

// HYPOTHESIS: owner object holding a handle at +8.
struct fn_005bc04b_owner
{
	int m00;
	int m04;
	int m08;

	void FUN_005bc04b(int param_1);
};

// FUNCTION: STARFLEETOP 0x005bc04b
void fn_005bc04b_owner::FUN_005bc04b(int param_1)
{
	FUN_005bce21(m08, 1, param_1);
}

// HYPOTHESIS: a record holding a database id and a hex position; the
// destructor stores no vtable, so the id is a member, not a base.
struct fn_005bc05c_record
{
	nDatabase::tID m00;
	tHexPoint m10;

	~fn_005bc05c_record();
};

// FUNCTION: STARFLEETOP 0x005bc05c
fn_005bc05c_record::~fn_005bc05c_record()
{
}
