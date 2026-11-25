#include <string>
#include "unsorted/tPlayerRelayCCreator.h"

// HYPOTHESIS: the original passes this string to the base constructor, which needs a string
// parameter in unsorted/tMetaClientCreator.h (HEADER REQUEST); the literal at 0x0087a4fc is not read yet.
// FUNCTION: STARFLEETOP 0x00600114
tPlayerRelayCCreator::tPlayerRelayCCreator()
{
    std::string name;
    name.assign("PlayerRelayC");
}

// MATCH-ONLY: the class has no members; the destructor body is only the base destructor call (11 bytes, exact)
// FUNCTION: STARFLEETOP 0x00600181
// FUNCTION: STARFLEETOP 0x0060019d
tPlayerRelayCCreator::~tPlayerRelayCCreator()
{
}

// FUNCTION: STARFLEETOP 0x006001a8
void tPlayerRelayCCreator::vfunction3()
{
    // HYPOTHESIS: the original discards the new object
    new tPlayerRelayC;
}
