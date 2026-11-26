// Unit:      unsorted/fn_006B1000.cpp
// Functions: 0x006b1050 to 0x006b1ee0, 8 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include "unsorted/tSWView3D.h"

// HYPOTHESIS: the triangle rasterizer's per-frame state, set up here and read by 0x006adf80.
extern int g_00c7c504;
extern unsigned char *g_00c7c40c;
extern int g_00c7c42c;
extern int g_00c7c428;
extern unsigned short *g_00c7c41c;
extern int g_00c7c430;
extern char g_00c7c56c;

// FUNCTION: STARFLEETOP 0x006b1050
bool tSWView3D::vfunction98(int a, int b, int c, bool d)
{
	if (c < 0 || c >= 0x1000 || !field_5fb0)
		return false;

	g_00c7c504 = field_5fac;
	g_00c7c40c = field_5fb0;
	g_00c7c42c = field_1f54;
	g_00c7c428 = field_1f58;
	g_00c7c41c = field_5fb4;
	g_00c7c430 = field_5fb8 << 1;
	g_00c7c56c = field_0x1ed8;

	unsigned short *p;
	int i;
	if (!d) {
		i = 2;
		void *v0 = mVtxCache + field_0x14[0];
		void *prev = mVtxCache + field_0x14[1];
		p = &field_0x14[2];
		do {
			void *cur = mVtxCache + *p++;
			i++;
			fn_006adf80(c, v0, prev, cur);
			prev = cur;
		} while (i < a);
	} else {
		unsigned short x = field_0x14[a - 1];
		i = a - 1;
		unsigned short y = field_0x14[i - 1];
		void *v0 = mVtxCache + x;
		void *prev = mVtxCache + y;
		i--;
		i--;
		p = &field_0x14[i];
		do {
			void *cur = mVtxCache + *p--;
			i--;
			fn_006adf80(c, v0, prev, cur);
			prev = cur;
		} while (i >= 0);
	}
	return true;
}
