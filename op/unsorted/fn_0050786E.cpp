#include "unsorted/tObject_0040ce21.h"

// HYPOTHESIS: tFx_Tractor is not declared in any header yet
class tFx_Tractor
{
public:
	tFx_Tractor();
	char	pad00[0x40];
};

// FUNCTION: STARFLEETOP 0x0050786e
tFx_Tractor* fn_0050786e()
{
	static tObject_0040ce21 marker("Let's haul it!");
	return new tFx_Tractor;
}
