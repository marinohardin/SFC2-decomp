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

// HYPOTHESIS: tFx_Probe is the 0x84-byte effect class built by its constructor at 0x0050408d.
class tFx_Probe
{
public:
	tFx_Probe();
	char pad00[0x84];
};

// HYPOTHESIS: a one-byte registration object whose constructor (0x0040ce21) takes a string.
class fn_0040ce21_holder
{
public:
	fn_0040ce21_holder(const char* s);
};

extern const char g_00857bb0[];

// FUNCTION: STARFLEETOP 0x00504339
void fn_00504339()
{
	static fn_0040ce21_holder holder(g_00857bb0);
	tFx_Probe* probe = 0;
	probe = new tFx_Probe;
}

float fn_005050e2(float aValue);

// HYPOTHESIS: the original bytes are the tail of a larger function (the call ends the range); this is the closest standalone form.
// FUNCTION: STARFLEETOP 0x00504433
void fn_00504433()
{
	fn_005050e2(64.0f);
}
