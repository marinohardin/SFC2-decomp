#include <string>
#include "Common/Thread/Optex.h"
#include "unsorted/tMessengerRelayCreator.h"

// MATCH-ONLY: the original's guard release is a bare Leave, with no flag test.
inline tOptex::tGuard::~tGuard()
{
	fOptex->Leave();
}

// HYPOTHESIS: the owner is a MetaClient object with an optex at +0xb8 whose real name is unknown;
// this unit names it after its one method.
class fn_00601747_class
{
public:
	void fn_00601747( tMetaClientCreator** result );

	char		pad00[0xb8];
	tOptex		fOptex;
};

// FUNCTION: STARFLEETOP 0x00601747
void fn_00601747_class::fn_00601747( tMetaClientCreator** result )
{
	tOptex::tGuard guard( &fOptex );
	*result = new tMessengerRelayCreator;
}

// HYPOTHESIS: a third global std::string built from the same literal as g_00c39fe0 and g_00c39ff0; its dynamic initializer is this function.
// FUNCTION: STARFLEETOP 0x006017d1
std::string g_00c3a000 = "MessengerRelayNameC";
