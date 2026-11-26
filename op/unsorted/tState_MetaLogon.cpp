#include "unsorted/tState_MetaLogon.h"

// HYPOTHESIS: the original also calls 0x0045e574 (the constructor of the 0x64-byte object at
// +0x124) just before the store below; that class has no constructor declared in its header
// yet, so this attempt is partial.
// FUNCTION: STARFLEETOP 0x0046d0b0
tState_MetaLogon::tState_MetaLogon()
{
	field_188 = 0;
}
