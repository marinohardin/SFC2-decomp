// Unit:      unsorted/fn_00504316.cpp
// Functions: 0x00504316 to 0x005044d2, 17 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: the free list of 0x54-byte effect objects; the next link sits at +0x34.
extern char* g_00967484;

// FUNCTION: STARFLEETOP 0x00504316
void fn_00504316()
{
	char* p = g_00967484;
	while (p) {
		char* next = *(char**)(p + 0x34);
		operator delete(p);
		p = next;
	}
	g_00967484 = 0;
}
