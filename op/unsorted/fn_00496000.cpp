#include <stdlib.h>
#include "Tiger/tstring.h"

// HYPOTHESIS: `ret 8` with ecx unused; may be a member of an unknown class that ignores this.
// FUNCTION: STARFLEETOP 0x004961be
tString __stdcall fn_004961be(long val)
{
	tString s;
	return s.mAppendInt32s(val, 1);
}

// HYPOTHESIS: `ret 0x28` with ecx unused, like 0x004961be; may be a member that ignores this.
// FUNCTION: STARFLEETOP 0x00496214
int __stdcall fn_00496214(tString s)
{
	return atoi(s.mCharStr());
}
