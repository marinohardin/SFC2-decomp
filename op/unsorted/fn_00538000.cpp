#include "unsorted/tBitmapFontEx.h"

// FUNCTION: STARFLEETOP 0x0053805e
void tBitmapFontEx::vfunction10()
{
	field_20++;
}

void __fastcall fn_00537bfd(int* self);

// FUNCTION: STARFLEETOP 0x00538062
void tBitmapFontEx::vfunction11()
{
	if (field_20 != 0) {
		field_20--;
		if (field_20 == 0) {
			fn_00537bfd((int*)this);
			delete this;
		}
	} else {
		field_20 = 0;
	}
}
