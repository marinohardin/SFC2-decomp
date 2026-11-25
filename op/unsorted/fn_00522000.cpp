// Unit:      unsorted/fn_00522000.cpp
// Functions: 0x0052203a to 0x005224f3, 12 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: a vector of 8-byte elements; field_8 is the end pointer, the function removes one element.
struct fn_0052203a_vector {
	int field_0;
	int* field_4;
	int* field_8;
	void fn_0052203a(int* pos);
};

// FUNCTION: STARFLEETOP 0x0052203a
void fn_0052203a_vector::fn_0052203a(int* pos)
{
	int* end = field_8;
	if (pos + 2 != end) {
		int* src = pos + 2;
		do {
			pos[0] = src[0];
			pos[1] = src[1];
			pos += 2;
			src += 2;
		} while (src != end);
	}
	field_8 -= 2;
}
