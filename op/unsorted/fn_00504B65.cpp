#include "unsorted/tObject_0040ce21.h"

// HYPOTHESIS: a factory for tFx_ProximityDetonation (size 0x30); the class is not declared in a header yet.
class tFx_ProximityDetonation
{
public:
	tFx_ProximityDetonation();
	char	pad00[0x30];
};

// FUNCTION: STARFLEETOP 0x00504b65
tFx_ProximityDetonation* fn_00504b65()
{
	static tObject_0040ce21 marker("That was close!");
	return new tFx_ProximityDetonation;
}
