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
