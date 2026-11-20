#include "Switch/AsyncProcedure.h"
#include "Switch/Switch.h"
#include "Common/Log/LogStream.h"
#include "Common/Exceptions/Validate.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/tAddressMgr.h"
#include "unsorted/tCompressedPosition.h"
#include "unsorted/nStoredProcedureArguments__tNotifyEvent.h"
#include "Meta/MetaDatabase/StoredProcedures/MapProcedures/MetaMapDataRequest.h"

namespace nAsyncClientMapProcedures
{

	// Name from the exe's RTTI (.?AVtMapChange@nAsyncClientMapProcedures@@).
	class tMapChange : public nSwitch::tAsyncProcedure
	{
	public:
		// HYPOTHESIS: the callback registered for the map change message (the RTTI names a
		// tMemberCallback<tMapChange,const nDataStore::tBuffer*>).
		void fn_005e009a( const nDataStore::tBuffer* buffer );
	};

};

using namespace nAsyncClientMapProcedures;
using namespace nStoredProcedureArguments;










































// FUNCTION: STARFLEETOP 0x005e009a
void tMapChange::fn_005e009a( const nDataStore::tBuffer* buffer )
{
	nLog::info << "MetaClient: kMapChange" << nLog::report;

	tNotifyEvent event;
	nDataStore::tInput input( buffer );
	input >> event;

	fn_00719050();

	tCompressedPosition position( event.fn_005e8f25() );
	tMetaMapDataRequest request( field_3c, 2, position.fn_0055f269(), position.fn_005e8f21() );

	validate( tAddressMgr::Instance()->GetMapRelayAddress() );
	field_1c->fn_00714020( tAddressMgr::Instance()->GetMapRelayAddress(), 3, &request );
}
