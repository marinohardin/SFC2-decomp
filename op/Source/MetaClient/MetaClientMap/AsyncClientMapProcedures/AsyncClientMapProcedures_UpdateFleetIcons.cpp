#include "Switch/AsyncProcedure.h"
#include "Switch/Switch.h"
#include "Common/Log/LogStream.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Thread/SingleAccessWrapper.h"
#include "unsorted/tAddressMgr.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/nStoredProcedureArguments__tGetFleetDataRequest.h"
#include "unsorted/fn_0042a480_object.h"

namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtUpdateFleetIcons@nAsyncClientMapProcedures@@).
	class tUpdateFleetIcons : public nSwitch::tAsyncProcedure
	{
	public:
		// HYPOTHESIS: the callback registered for the fleet icon message (the RTTI names a
		// tMemberCallback<tUpdateFleetIcons,const nDataStore::tBuffer*>).
		void fn_005e189d( const nDataStore::tBuffer* buffer );
	};

};

using namespace nAsyncClientMapProcedures;
using namespace nStoredProcedureArguments;











































// FUNCTION: STARFLEETOP 0x005e189d
void tUpdateFleetIcons::fn_005e189d( const nDataStore::tBuffer* buffer )
{
	nLog::info << "MetaClient: kUpdateFleetIcons" << nLog::report;

	fn_00719050();

	// HYPOTHESIS: the original pushes only the result pointer for 0x004d16b2 here, where the
	// logon information procedure also pushes 0; with the argument kept, the rest of the body
	// matched once the call was written without it.
	tGetFleetDataRequest request( fn_00719490( fn_00719580( 7 ) ), ( *gObject_0092deb4->fn_004d16b2( 0 ) ).FUN_0042a9ba() );

	validate( tAddressMgr::Instance()->GetCharacterRelayAddress() );
	field_1c->fn_00714020( tAddressMgr::Instance()->GetCharacterRelayAddress(), 15, &request );
}
