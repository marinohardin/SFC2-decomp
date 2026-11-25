#include "unsorted/fn_0090bed4_Object.h"

// FUNCTION: STARFLEETOP 0x00522679
void __fastcall fn_00522679(char* self)
{
	*(char*)(self + 0x16b4) = 0;
}

// HYPOTHESIS: the objects the table at +0x10 names.
class tObject_00524883 {
public:
	void fn_00524883();
};

// HYPOTHESIS: a time in seconds, read as a double (tAICaptain.cpp uses it the same way).
extern double g_0090bef0;

// 0.05 in the exe's data.
extern double g_007b0250;

// FUNCTION: STARFLEETOP 0x00522681
// HYPOTHESIS: the table at +0x10 (16-byte entries: object, time double at +8) and its count at
// +0x210 are not in the header yet, so they are read by offset.
void fn_0090bed4_Object::fn_00522681()
{
	char *self = (char *) this;
	double now = g_0090bef0;
	for (int i = 0; i < *(int *) (self + 0x210); i++) {
		double *when = (double *) (self + 0x18 + i * 16);
		if (now >= *when) {
			(*(tObject_00524883 **) (self + 0x10 + i * 16))->fn_00524883();
			*when = *when + g_007b0250;
		}
	}
	if (m_16d4 != 0) {
		float *last = (float *) (self + 0x16cc);
		if ((float) g_0090bef0 - *last > m_16d0) {
			*last = (float) g_0090bef0;
			fn_00523445(m_16d4,m_16d5);
		}
	}
}
