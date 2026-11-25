#include "unsorted/tObject_0040ce21.h"

// HYPOTHESIS: tFx_Shockwave has no shared header yet; declared here pending a HEADER REQUEST.
class tFx_Shockwave
{
public:
	tFx_Shockwave();

	char	pad00[0x30];
};

// FUNCTION: STARFLEETOP 0x00505d0a
tFx_Shockwave* fn_00505d0a()
{
	static tObject_0040ce21 report("Oh my, what will we do");
	return new tFx_Shockwave();
}
