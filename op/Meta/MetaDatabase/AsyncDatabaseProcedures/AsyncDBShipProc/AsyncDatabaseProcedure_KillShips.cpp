#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBShipProc/AsyncDatabaseProcedure_KillShips.h"
#include "Common/Exceptions/Validate.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/fn_005430ad_owner.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nStoredProcedureArguments__tGetCharactersRequest.h"
#include "unsorted/nStoredProcedureArguments__tUpdateCharactersRequest.h"
#include "unsorted/nSwitch__tAddress.h"
#include "Switch/Switch.h"
#include "unsorted/tNotifierAgent.h"
#include "unsorted/tObject_005d27f1.h"
#include "unsorted/tShip.h"

using namespace nAsyncDatabaseProcedure;

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
// (DataStoreInput.h defines it.)




// HYPOTHESIS: the callback for the database's answer: reads the one character the ships belong
// to, kills each ship, answers with the ships' IDs and writes the character back.
// FUNCTION: STARFLEETOP 0x005682b2
void tKillShips::fn_005682b2( const nDataStore::tBuffer* pBuffer )
{
	tNotifierAgent notifier;
	nDataStore::tInput input( pBuffer );
	nStoredProcedureArguments::tGetCharactersRequest::tGetCharactersResponse charactersResponse( 0 );
	input >> charactersResponse;

	fn_00719050();
	std::vector<tServCharacter>& characters = charactersResponse.fn_0058c2e1();
#line 89
	validate( characters.size() == 1 );
	// HYPOTHESIS: fn_005430ad_owner is the placeholder class unsorted/fn_00543000.cpp gives the
	// character record.
	fn_005430ad_owner* character = (fn_005430ad_owner*)&characters[0];

	std::vector<tShip> ships;
	fpAsyncDatabase->vfunction9( fRequest.fn_005596e9(), &ships, 0 );

	nStoredProcedureArguments::tKillShipsRequest::tKillShipsResponse response( &fRequest );
	response.fn_00716310( 0 );
	for( std::vector<tShip>::iterator ship = ships.begin(); ship != ships.end(); ++ship )
	{
		nDatabase::tID id = ship->GetID();
		character->fn_005437a5( id );
		response.fn_0058c2e1().push_back( id );
		fpAsyncDatabase->vfunction52( ship );
		notifier.fn_005cba6e( id );
	}
	m_80.fn_00568538( response );

	nStoredProcedureArguments::tUpdateCharactersRequest update( fn_00719490( fn_00719580( 0xd ) ) );
	update.fn_00553ba5( characters );
	field_1c->fn_00714020( fn_005d27f1()->fn_005d2f20().Get(), 0xa, &update );
}
