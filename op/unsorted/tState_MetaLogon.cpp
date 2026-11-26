#include "unsorted/tState_MetaLogon.h"
#include "unsorted/fn_004D3143.h"
#include <sstream>

// HYPOTHESIS: the original also calls 0x0045e574 (the constructor of the 0x64-byte object at
// +0x124) just before the store below; that class has no constructor declared in its header
// yet, so this attempt is partial.
// FUNCTION: STARFLEETOP 0x0046d0b0
tState_MetaLogon::tState_MetaLogon()
{
	field_188 = 0;
}

extern char *g_008471e4;
extern char *g_0084724c;
extern char g_0084731c[];

// FUNCTION: STARFLEETOP 0x0046d2a4
char *tState_MetaLogon::vfunction13()
{
	return g_008471e4;
}

// FUNCTION: STARFLEETOP 0x0046d2aa
void tState_MetaLogon::fn_0046d2aa( const char *text )
{
	std::stringstream stream;
	stream << text << g_0084731c << field_28.mCharStr();
	field_28 = stream.str().c_str();
	fn_004d3179( g_0084724c, field_28 );
}
