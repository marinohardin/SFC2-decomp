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
