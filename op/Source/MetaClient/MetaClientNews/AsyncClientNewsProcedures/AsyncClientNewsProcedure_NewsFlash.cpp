#include "Switch/AsyncProcedure.h"
#include "Switch/Switch.h"
#include "Common/Log/LogStream.h"
#include "Common/Exceptions/Validate.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/tAddressMgr.h"
#include "unsorted/nStoredProcedureArguments__tNotifyEvent.h"
#include "unsorted/nStoredProcedureArguments__tGetNewsRequest.h"

namespace nAsyncClientNewsProcedure
{

	// Name from the exe's RTTI (.?AVtNewsFlash@nAsyncClientNewsProcedure@@).
	class tNewsFlash : public nSwitch::tAsyncProcedure
	{
	public:
		// HYPOTHESIS: the callback registered for the news flash message (the RTTI names a
		// tMemberCallback<tNewsFlash,const nDataStore::tBuffer*>).
		void fn_005dc8a8( const nDataStore::tBuffer* buffer );
	};

};

using namespace nAsyncClientNewsProcedure;
using namespace nStoredProcedureArguments;































// FUNCTION: STARFLEETOP 0x005dc8a8
void tNewsFlash::fn_005dc8a8( const nDataStore::tBuffer* buffer )
{
	nLog::info << "MetaClient: kNewsFlash" << nLog::report;

	nDataStore::tInput input( buffer );
	tNotifyEvent event;
	input >> event;

	fn_00719050();

	tGetNewsRequest request( field_3c, 2, nDatabase::tID( event.fn_005e8f25() ) );

	validate( tAddressMgr::Instance()->GetNewsRelayAddress() );
	nSwitch::tSwitch::fn_00713b20()->fn_00714020( tAddressMgr::Instance()->GetNewsRelayAddress(), 2, &request );
}
