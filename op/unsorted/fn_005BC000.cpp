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
