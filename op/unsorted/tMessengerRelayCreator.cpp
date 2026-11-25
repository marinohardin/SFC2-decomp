#include "unsorted/tMessengerRelayCreator.h"

// MATCH-ONLY: the destructor is empty; its 11 bytes are the vtable store and the call to the base
// destructor that VC6 generates around an empty body.
// FUNCTION: STARFLEETOP 0x00601834
// FUNCTION: STARFLEETOP 0x00601850
tMessengerRelayCreator::~tMessengerRelayCreator()
{
}

// FUNCTION: STARFLEETOP 0x0060185b
// HYPOTHESIS: the original returns the new object; the base declares the slot as void.
void tMessengerRelayCreator::vfunction3()
{
    new tMessengerRelayC;
}
