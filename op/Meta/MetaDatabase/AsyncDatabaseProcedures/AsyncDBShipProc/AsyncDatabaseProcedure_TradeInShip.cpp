#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBShipProc/AsyncDatabaseProcedure_TradeInShip.h"
#include "Common/Exceptions/Validate.h"
#include "Common/DataStore/DataStoreInput.h"
#include "unsorted/c_00546027.h"
#include "unsorted/fn_005430ad_owner.h"
#include "unsorted/nAsyncDatabaseProcedure__tAsyncDatabase.h"
#include "unsorted/nStoredProcedureArguments__tGetCharactersRequest.h"
#include "unsorted/nStoredProcedureArguments__tUpdateCharactersRequest.h"
#include "unsorted/nSwitch__tAddress.h"
#include "Switch/Switch.h"
#include "unsorted/tNotifierAgent.h"
#include "unsorted/tShip.h"

using namespace nAsyncDatabaseProcedure;

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
inline nDataStore::tInput::~tInput()
{
	Close( fCleanUpBuffer );
}

// HYPOTHESIS: the callback for the database's answer: reads the character, finds the ship to
// trade in among the character's ship caches, prices it, removes it from the character and the
// database and writes the character back.
// FUNCTION: STARFLEETOP 0x0056964b
void tTradeInShip::fn_0056964b( const nDataStore::tBuffer* pBuffer )
{
	nDataStore::tInput input( pBuffer );
	nStoredProcedureArguments::tGetCharactersRequest::tGetCharactersResponse charactersResponse( 0 );
	input >> charactersResponse;

	fn_00719050();
	std::vector<tServCharacter>& characters = charactersResponse.fn_0058c2e1();
#line 91
	validate( characters.size() == 1 );
	// HYPOTHESIS: fn_005430ad_owner is the placeholder class unsorted/fn_00543000.cpp gives the
	// character record.
	fn_005430ad_owner* character = (fn_005430ad_owner*)&characters[0];

	fn_0042c709_ptr shipCaches = character->fn_005434f7();
	nDatabase::tID shipID;
	nDatabase::tID otherID;
	if( (*shipCaches).size() > 1 )
	{
		int count = 0;
		for( std::vector< fn_005419e4_entry >::const_iterator it = (*shipCaches).begin(); it != (*shipCaches).end(); ++it )
		{
			if( !it->fn_005419e4() )
			{
				otherID = it->fn_005419ed();
				count++;
			}
			bool found = it->fn_005419ed() == fRequest.fn_0055e613();
			if( found )
				shipID = it->fn_005419ed();
		}
		if( shipID.IsValid() && ( count > 1 || otherID != shipID ) )
		{
			std::vector<tShip> ships;
			std::vector<nDatabase::tID> shipIDs;
			shipIDs.push_back( shipID );
			fpAsyncDatabase->vfunction9( shipIDs, &ships, 0 );
			if( ships.size() == 1 && ships[0].GetID() == shipID )
			{
				std::vector<tShip>::iterator ship = ships.begin();
				m_224 = ship->GetID();
				// HYPOTHESIS: the three rate tables live in the request; the claimed rate
				// functions take the ship's ID through the key types their unit guessed.
				double tradeInRate = ( (tTradeInRates*)&fRequest )->fGetRate( (const std::string&)m_224 );
				double repairRate = ( (tShipRepairRates*)&fRequest )->fGetRate( (fShipRepairRateNode*)&m_224 );
				// HYPOTHESIS: the original copies the item rates the third table returns into a
				// local c_0045139e here and hands it, with the two rates, to fn_0054606f, which
				// returns the price:
				//	c_0045139e itemRates = *(c_0045139e*)( (tShipItemRates*)&fRequest )->fGetRateAddress( (fShipRepairRateNode*)&m_224 );
				//	int price = ( (c_00546027*)&*ship )->fn_0054606f( tradeInRate, repairRate, itemRates );
				// c_0045139e is declared only inside unsorted/fn_00546000.cpp, so those lines wait
				// for it to move to a header.
				int price = 0;
				character->fn_0054a8c8();
				character->fn_005437a5( m_224 );
				character->fn_005430d3( -price );
				fpAsyncDatabase->vfunction52( ship );
				tNotifierAgent notifier;
				notifier.fn_005cba6e( m_224 );
			}
		}
	}
	shipCaches.Done();
	m_c4 = *(tServCharacter*)character;

	nStoredProcedureArguments::tUpdateCharactersRequest update( fn_00719490( fn_00719580( 0xe ) ) );
	update.fn_00553ba5( characters );
	m_1c->fn_00714020( m_48, 0xa, &update );
}
