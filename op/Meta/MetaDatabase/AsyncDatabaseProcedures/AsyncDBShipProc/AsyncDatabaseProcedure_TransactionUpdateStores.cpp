#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBShipProc/AsyncDatabaseProcedure_TransactionUpdateStores.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"


















// The SDK's template bodies, which Common/Callback.h and Common/MemberCallback.h leave to the
// units that instantiate them.
template< typename tnResult >
tCallback<tnResult>::~tCallback()
{}

template< typename tnMemberClass, typename tnResult >
tMemberCallback<tnMemberClass,tnResult>::tMemberCallback<tnMemberClass,tnResult>( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

using namespace nAsyncDatabaseProcedure;

// The analysis names this address as the destructor; the body is the constructor.
// FUNCTION: STARFLEETOP 0x00569bc9
tTransactionUpdateStores::tTransactionUpdateStores( tAsyncDatabase* db ) : tAsyncDatabaseProcedure( db )
{
#line 36
	validate( db );
	fn_00718f10( 10, new tMemberCallback<tTransactionUpdateStores,const nDataStore::tBuffer*>( this, fn_00569c83 ) );
	fn_00719050();
}

// FUNCTION: STARFLEETOP 0x00569c5c
// FUNCTION: STARFLEETOP 0x00569c78
#line 48
tTransactionUpdateStores::~tTransactionUpdateStores()
{
}

// The SDK's inline destructor of the reader, expanded into the function below as in the
// original; Close itself is called out of line (0x0056027f).
inline nDataStore::tInput::~tInput()
{
	Close( fCleanUpBuffer );
}

// HYPOTHESIS: the callback for the database's answer: reads the character and the ships it
// owns, names the ships, buys the stores the request asks for as far as the character can pay,
// writes ships and character back and sends the updated ships in the response. The stores
// fields are named by their offsets in c_00427db2: +0xf1/+0xf2 a maximum and a count, the
// bytes at +0x10c a maximum and at +0x112 a count, four slots at +0x17a to +0x18c.
// FUNCTION: STARFLEETOP 0x00569c83
void tTransactionUpdateStores::fn_00569c83( const nDataStore::tBuffer* pBuffer )
{
	nDataStore::tInput input( pBuffer );
	nStoredProcedureArguments::tTransactionUpdateStoresRequest request;
	input >> request;

	fn_00719050();
	std::vector<nDatabase::tID> characterIDs;
	std::vector<nDatabase::tID> shipIDs;
	characterIDs.push_back( request.GetCharacterID() );
	std::vector<tServCharacter> characters;
	std::vector<tShip> ships;
	fpAsyncDatabase->vfunction3( characterIDs, &characters, 0 );
#line 72
	validate( characters.size() == 1 );
	// HYPOTHESIS: fn_005430ad_owner is the placeholder class unsorted/fn_00543000.cpp gives the
	// character record.
	fn_005430ad_owner& character = *(fn_005430ad_owner*)&characters[0];
#line 75
	validate( character.GetID() == request.GetCharacterID() );

	for( std::vector<nDatabase::tID>::const_iterator id = request.fn_005596e9().begin(); id != request.fn_005596e9().end(); ++id )
	{
		if( character.fn_00543944( *id ) )
			shipIDs.push_back( *id );
	}
	std::vector<tShip> updatedShips;
	fpAsyncDatabase->vfunction9( shipIDs, &ships, 0 );

	std::vector<tShip>::iterator ship;
	for( ship = ships.begin(); ship != ships.end(); ++ship )
	{
		// HYPOTHESIS: the request's tables are keyed by the ship's ID; the claimed getters'
		// placeholder parameter types force the casts.
		std::string name = ( (tShipNames*)&request )->fn_0055b087( (const std::string&)ship->GetID() );
		if( !name.empty() )
			( (c_00546296*)&*ship )->fn_005467b0( name );
		c_00427db2 oldStores( ( (c_00546027*)&*ship )->fn_005462e5() );
		c_00427db2 newStores( *(c_00427db2*)( (tStoresStates*)&request )->fn_0055b183( (fStoresStateNode*)&ship->GetID() ) );
		c_0045139e* rates = (c_0045139e*)( (tStoresItemRates*)&request )->fGetRateAddress( (fStoresRateNode*)&ship->GetID() );

		int shuttles = max( 0, min( oldStores.field_f1, newStores.field_f2 ) ) - oldStores.field_f2;
		c_004280f1* oldMissiles = (c_004280f1*)oldStores.fn_0055e60f();
		c_004280f1 missiles( *(c_004280f1*)newStores.fn_0055e60f() );
		missiles.field_c[3] = min( missiles.field_c[3], request.fn_0058b243()->fn_005c7f7c() );
		int missileCount = missiles.fn_0045193a( missiles.fn_00451a8b() );
		int count2 = max( 0, min( oldStores.field_10c[2], newStores.field_112[2] ) ) - oldStores.field_112[2];
		int count0 = max( 0, min( oldStores.field_10c[0], newStores.field_112[0] ) ) - oldStores.field_112[0];
		int count1 = max( 0, min( oldStores.field_10c[1], newStores.field_112[1] ) ) - oldStores.field_112[1];

		double price = rates->fn_0045139e( 3 );
		int affordable = max( 0, character.fn_0054a8c8() / price );
		int bought = min( shuttles, affordable );
		character.fn_005430d3( max( 0, bought * price ) );

		double oldPrice = oldMissiles->fn_00451aa6( rates );
		int oldValue = oldMissiles->fn_00451a8b() * oldPrice;
		double missilePrice = missiles.fn_00451aa6( rates );
		int affordableMissiles;
		if( missilePrice != 0.0 )
			affordableMissiles = max( 0, character.fn_0054a8c8() / missilePrice );
		else
			affordableMissiles = missileCount;
		int boughtMissiles = min( missileCount, affordableMissiles );
		character.fn_005430d3( max( 0, boughtMissiles * missilePrice - oldValue ) );

		int counts[4];
		int i;
		for( i = 0; i < 4; i++ )
		{
			long size;
			rates->fn_0045147a( oldStores.field_18c[i], &size );
			int value = oldStores.field_182[i] * size;
			if( oldStores.field_17a[i] && rates->fn_0045147a( newStores.field_18c[i], &size ) )
			{
				int canBuy = ( max( 0, character.fn_0054a8c8() ) + value ) / size;
				counts[i] = min( newStores.field_182[i], canBuy );
				character.fn_005430d3( max( 0, counts[i] * size - value ) );
				newStores.field_182[i] = counts[i];
			}
			else
			{
				newStores.field_17a[i] = oldStores.field_17a[i];
				newStores.field_17e[i] = oldStores.field_17e[i];
				newStores.field_182[i] = oldStores.field_182[i];
				newStores.field_186[i] = oldStores.field_186[i];
				newStores.field_18c[i] = oldStores.field_18c[i];
			}
		}

		double price2 = rates->fn_004513a9( 2 );
		int affordable2 = max( 0, character.fn_0054a8c8() / price2 );
		int bought2 = min( count2, affordable2 );
		character.fn_005430d3( max( 0, bought2 * price2 ) );
		double price0 = rates->fn_004513a9( 0 );
		int affordable0 = max( 0, character.fn_0054a8c8() / price0 );
		int bought0 = min( count0, affordable0 );
		character.fn_005430d3( max( 0, bought0 * price0 ) );
		double price1 = rates->fn_004513a9( 1 );
		int affordable1 = max( 0, character.fn_0054a8c8() / price1 );
		int bought1 = min( count1, affordable1 );
		character.fn_005430d3( max( 0, bought1 * price1 ) );

		oldStores.field_f2 += bought;
		oldMissiles->fn_0045154e( &missiles );
		oldMissiles->fn_0045193a( boughtMissiles );
		for( i = 0; i < 4; i++ )
		{
			oldStores.field_182[i] = newStores.field_182[i];
			oldStores.field_186[i] = newStores.field_186[i];
			oldStores.field_18c[i] = newStores.field_18c[i];
		}
		oldStores.field_112[2] += bought2;
		oldStores.field_112[0] += bought0;
		oldStores.field_112[1] += bought1;
		( (c_00546027*)&*ship )->fn_00546492( oldStores );
	}

	tNotifierAgent notifier;
	for( ship = ships.begin(); ship != ships.end(); ++ship )
	{
		character.fn_0054385d( (fn_0054385d_source*)&*ship );
		fpAsyncDatabase->vfunction44( *ship );
		updatedShips.push_back( *ship );
		notifier.fn_005cba1f( ship->GetID() );
	}
	fpAsyncDatabase->vfunction42( (tServCharacter&)character );
	notifier.fn_005cbc48( character.GetID() );
	nStoredProcedureArguments::tTransactionUpdateStoresRequest::tTransactionUpdateStoresResponse response( request, updatedShips );
	m_1c->fn_00714020( request.vfunction8().Get(), request.vfunction9(), &response );
}
