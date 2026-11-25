#include "unsorted/fn_0040ce21_type.h"

// HYPOTHESIS: tFx_PPDPulse has no header yet; 0x48 bytes from the new below.
class tFx_PPDPulse
{
public:
	tFx_PPDPulse();
	char pad00[0x48];
};

// FUNCTION: STARFLEETOP 0x00503dac
tFx_PPDPulse* fn_00503dac()
{
	static fn_0040ce21_type marker("Let's Paul it!");
	return new tFx_PPDPulse;
}
