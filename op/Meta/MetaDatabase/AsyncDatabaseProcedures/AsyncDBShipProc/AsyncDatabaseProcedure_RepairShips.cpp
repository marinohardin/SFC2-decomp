#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBShipProc/AsyncDatabaseProcedure_RepairShips.h"
#include "Common/MemberCallback.h"
#include "Common/Exceptions/Validate.h"
#include "Common/Log/LogStream.h"
// HYPOTHESIS: a record reader keyed on a kind (fKind) and an offset (fOffset); the class is not
// known, so the members are named by their offsets.
class fn_005686b8_owner
{
public:
	int fKind;
	int m_04;
	char m_08;
	char m_09[3];
	int fOffset;
	int fn_005686b8( int base, int value );
	int fn_0056872c( int8* curState, int8 kind, unsigned int value );
	int fn_0056882f( int base, char a, char b, int value );
};

// FUNCTION: STARFLEETOP 0x005686b8
int fn_005686b8_owner::fn_005686b8( int base, int value )
{
	switch( fKind )
	{
	case 0:
		return fn_0056872c( (int8*)( fOffset + base + 0x25 ), *(int8*)( fOffset + base + 0xc ), value );
	case 1:
		return fn_0056882f( base, 10, 0x18, value );
	case 2:
		return fn_0056882f( base, 0, 9, value );
	}
#line 83
	validate( false );
	return value;
}

#line 38
// FUNCTION: STARFLEETOP 0x0056872c
int fn_005686b8_owner::fn_0056872c( int8* curState, int8 kind, unsigned int value )
{
	if( *curState >= 0 && kind >= 0 && *curState <= kind )
	{
		int8 repair;
		switch( m_04 )
		{
		case 0:
			repair = (char)( ( m_08 * 0.01f ) * kind + 0.5f - *curState );
			break;
		case 1:
			if( m_08 < kind )
				repair = m_08 - *curState;
			else
				repair = kind - *curState;
			break;
		default:
#line 133
			validate( false );
			return value;
		}
		if( repair > 0 )
		{
			if( (unsigned int)repair > value )
				repair = (char)value;
			*curState += repair;
			value -= repair;
			if( *curState > kind )
				*curState = kind;
		}
	}
	else
	{
		nLog::error << "Problem repairing ship, CurState = " << *curState << " MaxState = " << kind << nLog::report;
	}
	return value;
}

#line 77
// FUNCTION: STARFLEETOP 0x0056882f
int fn_005686b8_owner::fn_0056882f( int base, char a, char b, int value )
{
	int i = a;
	while( i < b && (unsigned int)value > 0 )
	{
		value = fn_0056872c( (int8*)( i + 0x57 + base ), *(int8*)( i + 0x3e + base ), value );
		i++;
	}
	return value;
}

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
// FUNCTION: STARFLEETOP 0x00568a71
tRepairShips::tRepairShips( tAsyncDatabase* pDB ) : tAsyncDatabaseProcedure( pDB )
{
#line 235
	validate( pDB );
	fn_00718f10( 9, new tMemberCallback<tRepairShips,const nDataStore::tBuffer*>( this, fn_00568b2e ) );
	fn_00719050();
}

#line 115
// FUNCTION: STARFLEETOP 0x00568b07
// FUNCTION: STARFLEETOP 0x00568b23
tRepairShips::~tRepairShips()
{
}

// The SDK's inline destructor of the reader, expanded into 0x00568b2e as in the original;
// Close itself is called out of line (0x0056027f).
inline nDataStore::tInput::~tInput()
{
	Close( fCleanUpBuffer );
}

// HYPOTHESIS: the callback for the database's answer: reads the character and the ships it
// owns, pays for each repair from the character and writes the ship back.
// FUNCTION: STARFLEETOP 0x00568b2e
void tRepairShips::fn_00568b2e( const nDataStore::tBuffer* pBuffer )
{
	nDataStore::tInput input( pBuffer );
	nStoredProcedureArguments::tRepairShipsRequest request;
	input >> request;

	fn_00719050();
	tNotifierAgent notifier;
	bool repaired = false;
	std::vector<nDatabase::tID> characterIDs;
	std::vector<nDatabase::tID> shipIDs;
	characterIDs.push_back( request.fn_0055e877() );
	std::vector<tServCharacter> characters;
	std::vector<tShip> ships;
	fpAsyncDatabase->vfunction3( characterIDs, &characters, 0 );
#line 279
	validate( characters.size() == 1 );
	// HYPOTHESIS: fn_005430ad_owner is the placeholder class unsorted/fn_00543000.cpp gives the
	// character record.
	fn_005430ad_owner* character = (fn_005430ad_owner*)&characters[0];
	validate( character->GetID() == request.fn_0055e877() );

	for( std::vector<nDatabase::tID>::const_iterator id = request.fn_005596e9().begin(); id != request.fn_005596e9().end(); ++id )
	{
		if( character->fn_00543944( *id ) )
			shipIDs.push_back( *id );
	}
	if( shipIDs.size() == 0 )
	{
		vfunction3();
		return;
	}
	fpAsyncDatabase->vfunction9( shipIDs, &ships, 0 );

	try
	{
		for( std::vector<tShip>::iterator ship = ships.begin(); ship != ships.end(); ++ship )
		{
			// HYPOTHESIS: tRepairRates and c_00546027 are the placeholder classes the callees'
			// units give the request and the ship; 0x005596ed takes the ship's ID.
			double rate = ( (tRepairRates*)&request )->GetRepairRate( (fRepairRateNode*)&ship->GetID() );
			int cost = ( (c_00546027*)&*ship )->fn_0054603d( rate );
			if( cost != 0 )
			{
				int paid = cost;
				if( character->fn_0054a8c8() < cost )
					paid = character->fn_0054a8c8();
				int maxState;
				int curState;
				( (c_00546027*)&*ship )->fn_00546027( (int)&maxState, (int)&curState );
				fn_00568f29( ship, (int)( (double)paid / cost * ( maxState - curState ) ) );
				character->fn_005430d3( paid );
				character->fn_0054385d( (fn_0054385d_source*)&*ship );
				repaired = true;
				fpAsyncDatabase->vfunction44( *ship );
				notifier.fn_005cba1f( ship->GetID() );
			}
		}
	}
	catch( ... )
	{
	}

	vfunction3();
}
