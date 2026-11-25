#include "Switch/AsyncProcedure.h"
#include "Common/Log/LogStream.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/nStoredProcedureArguments__tNotifyEvent.h"

// HYPOTHESIS: the player list panel; the real class (tMetaClientPlayerListPanel, named by its
// creator and by tRemoveItem) has no header yet. Only the member this function calls is declared.
class tMetaClientPlayerListPanel
{
public:
	void fn_00603bfb( int32u id );
};

namespace nAsyncClientPlayerListProcedures
{

	// Name from the exe's RTTI (.?AVtCharacterLoggedOff@nAsyncClientPlayerListProcedures@@).
	class tCharacterLoggedOff : public nSwitch::tAsyncProcedure
	{
	public:
		// HYPOTHESIS: the callback for the character logged off message.
		void fn_00601983( const nDataStore::tBuffer* buffer );

	protected:
		tMetaClientPlayerListPanel* field_44;
	};

};

using namespace nAsyncClientPlayerListProcedures;
using namespace nStoredProcedureArguments;

// FUNCTION: STARFLEETOP 0x00601983
void tCharacterLoggedOff::fn_00601983( const nDataStore::tBuffer* buffer )
{
	nLog::info << "MetaClient: kCharacterLoggedOff" << nLog::report;

	nDataStore::tInput input( buffer );
	tNotifyEvent event;
	input >> event;

	fn_00719050();

	field_44->fn_00603bfb( event.fn_005e8f25() );
}
