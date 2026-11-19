#include "Switch/AsyncProcedure.h"
#include "Switch/Switch.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Thread/SingleAccessWrapper.h"
#include "unsorted/tAddressMgr.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/nStoredProcedureArguments__tGetClientCharacterRequest.h"
#include "unsorted/fn_0042a480_object.h"

namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtRequestLogonInformation@nAsyncClientMapProcedures@@).
	class tRequestLogonInformation : public nSwitch::tAsyncProcedure
	{
	public:
		// HYPOTHESIS: the procedure's start, slot 5 as in the map initialize procedure.
		virtual void vfunction5();
	};

};

using namespace nAsyncClientMapProcedures;
using namespace nStoredProcedureArguments;

// FUNCTION: STARFLEETOP 0x005e13d8
void tRequestLogonInformation::vfunction5()
{
	tGetClientCharacterRequest request( fn_00719490( fn_00719580( 6 ) ), ( *gObject_0092deb4->fn_004d16b2( 0 ) ).FUN_0042a9ba() );

	// The original's validate text gives the line (192) and the getter's name.
#line 192
	validate( tAddressMgr::Instance()->GetCharacterRelayAddress() );
	field_1c->fn_00714020( tAddressMgr::Instance()->GetCharacterRelayAddress(), 8, &request );
}
