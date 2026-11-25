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

__declspec(noreturn) float fn_005050e2(float aValue);

// HYPOTHESIS: the original bytes are the tail of a larger function (the call ends the range); this is the closest standalone form.
// FUNCTION: STARFLEETOP 0x00504bff
void fn_00504bff()
{
	// MATCH-ONLY: the original call leaves no result to pop; call through a void-returning type.
	((void (*)(float))fn_005050e2)(0.0f);
}
