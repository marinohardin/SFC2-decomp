#include "Switch/Switch.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/nStoredProcedureArguments__tNotifyEvent.h"

// Name from the exe's RTTI (.?AVtMetaClientNewsPanel@@, derived from tMetaClientPanel).
// HYPOTHESIS: only the member this function reads is placed; fMyAddress is named by the
// assertion text at line 83, and its type follows tSwitch::fn_00714020's destination.
class tMetaClientNewsPanel
{
public:
	void fn_005df230();

protected:
	int8u field_00[0xac];
	nSwitch::tAddress* fMyAddress;
};

using namespace nStoredProcedureArguments;




























































// FUNCTION: STARFLEETOP 0x005df230
void tMetaClientNewsPanel::fn_005df230()
{
	tNotifyEvent event( 3, 0, -1 );
	validate( fMyAddress );
	nSwitch::tAddress* address = fMyAddress;
	nSwitch::tSwitch::fn_00713b20()->fn_00714020( address, 3, &event );
}
