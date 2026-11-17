// Unit:      Source/entity/team.cpp
// Evidence:  assert-path (the original's assertion strings name this file)
// Profile:   game-release (harness/units.json)

#include "unsorted/tMemoryPart.h"

// HYPOTHESIS: one (a, b) pair of the list below; the meaning of either word is unknown.
struct tObject_00426ca8_entry
{
	int field_0x0;
	int field_0x4;
};

// HYPOTHESIS: a team object of this unit; only the growable pair list at +0xc4 (array,
// count, capacity) is known.
class tObject_00426ca8
{
public:
	void fn_00426da7();
	void fn_00426ca8(int a, int b);

	char pad_0x0[0xc4];
	tObject_00426ca8_entry *field_0xc4;
	int field_0xc8;
	int field_0xcc;
};

// FUNCTION: STARFLEETOP 0x00426ca8
void tObject_00426ca8::fn_00426ca8(int a, int b)
{
	fn_00426da7();
	if (field_0xcc == 0)
	{
		field_0xc4 = (tObject_00426ca8_entry *)tMemoryPart::Search_tMemoryPart()->mAllocate(8 * 8, 243, __FILE__);
		field_0xcc = 8;
	}
	else if (field_0xc8 == field_0xcc)
	{
		field_0xcc += 8;
		field_0xc4 = (tObject_00426ca8_entry *)tMemoryPart::Search_tMemoryPart()->vfunction8(field_0xc4, field_0xcc * 8);
	}
	field_0xc4[field_0xc8].field_0x0 = a;
	field_0xc4[field_0xc8].field_0x4 = b;
	field_0xc8++;
}
