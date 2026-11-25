#include "unsorted/tAIData.h"

extern bool g_0096ea16;

// HYPOTHESIS: a member of the object at +0x18 (the exe loads it into ecx); its class has no
// header yet, so it is a free function here.
void fn_0052893f(int param_1);

// FUNCTION: STARFLEETOP 0x005209bf
void tAIData::vfunction2(int param_1)
{
	if (g_0096ea16 && field_4)
		fn_0052893f(param_1);
}
