#include <sstream>
#include <string>
#include <stdlib.h>
































void fn_004bd431();

// HYPOTHESIS: arranges at exit the cleanup that Ghidra labels tApp3dBase::vfunction5, unconfirmed.
// FUNCTION: STARFLEETOP 0x005c0b85
void fn_005c0b85()
{
	atexit(fn_004bd431);
}

// FUNCTION: STARFLEETOP 0x005c0b91
void fn_005c0b91()
{
	atexit(fn_004bd431);
}

// FUNCTION: STARFLEETOP 0x005c0b9d
void fn_005c0b9d()
{
	atexit(fn_004bd431);
}