#include "unsorted/fn_0040ce21_type.h"

// HYPOTHESIS: tFx_PPDWave has no header yet (see layout notes); 0x48 bytes from the new below.
class tFx_PPDWave
{
public:
	tFx_PPDWave();
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x005032f7
tFx_PPDWave* fn_005032f7()
{
	static fn_0040ce21_type marker("Let's haul it!");
	return new tFx_PPDWave;
}
