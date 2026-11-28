#include "unsorted/tAICaptain.h"

// 0x00517f6e writes this flag at offset 0x1c.
// FUNCTION: STARFLEETOP 0x0051819f
bool tAIHETOfficer::fn_0051819f()
{
	return *(const bool *)((const char *)this + 0x1c);
}
