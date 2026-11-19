//	MetaMapDataRequest.cpp
//
//	The map data request: the hexes of the meta map as the client receives them.
//
#include "Meta/MetaDatabase/StoredProcedures/MapProcedures/MetaMapDataRequest.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/tMetaMapHex.h"
#include "Common/DataStore/DataStoreOutput.h"
#define DATASTORE_OUT_OF_LINE_GET	// MATCH-ONLY: the original calls the out-of-line copy of tInput::Get (0x0042a309)
#include "Common/DataStore/DataStoreInput.h"



// The blank lines below keep the original's line numbers, which the assertions carry.








namespace nStoredProcedureArguments
{

// FUNCTION: STARFLEETOP 0x0054bd73
tMetaMapDataRequest::tResponse::tClientHex::tClientHex( tMetaMapHex& hex, int32 maxEconomyValue, int32 maxVictoryValue ) :
	tStreamable( 1 ),
	// MATCH-ONLY: the original copies the map's values as integers; the value type is probably
	// not the float tMetaMapHex.h guesses.
	m_0c( *(int32*)&hex.fn_0053a2fc().find( 0 )->second ),
	m_0d( *(int32*)&hex.fn_0053a2fc().find( 1 )->second ),
	m_10( hex.fn_0053a31c() ),
	m_14( hex.fn_0053a33c() != 0 ),
	m_15( hex.fn_0053a35c() != 0 )
{
	validate( maxEconomyValue == kMaxPointValue && maxVictoryValue == kMaxPointValue );

	m_24 = hex.fn_0053a46f();

	// MATCH-ONLY: validate() written out: the assertion strings name the hex's accessors
	// GetCurrentVictoryPoints and GetCurrentEconomicPoints, which tMetaMapHex.h still calls
	// fn_0053a414 and fn_0053a39c.
	if( !(0 <= hex.fn_0053a414( 0 ) && hex.fn_0053a414( 0 ) <= (int8u)kMax8BitNum) ) { nException::tValidationFailed failed( "0 <= hex.GetCurrentVictoryPoints( nMetaMap::eEmpire ) && hex.GetCurrentVictoryPoints( nMetaMap::eEmpire ) <= (int8u)kMax8BitNum", __FILE__, __LINE__ ); failed.ReportException(); }
	if( !(0 <= hex.fn_0053a414( 1 ) && hex.fn_0053a414( 1 ) <= (int8u)kMax8BitNum) ) { nException::tValidationFailed failed( "0 <= hex.GetCurrentVictoryPoints( nMetaMap::eOrion ) && hex.GetCurrentVictoryPoints( nMetaMap::eOrion ) <= (int8u)kMax8BitNum", __FILE__, __LINE__ ); failed.ReportException(); }
	if( !(0 <= hex.fn_0053a39c() && hex.fn_0053a39c() <= (int8u)kMax8BitNum) ) { nException::tValidationFailed failed( "0 <= hex.GetCurrentEconomicPoints() && hex.GetCurrentEconomicPoints() <= (int8u)kMax8BitNum", __FILE__, __LINE__ ); failed.ReportException(); }

	m_28 = hex.fn_0053a414( 0 );
	m_29 = hex.fn_0053a414( 1 );
	m_2a = hex.fn_0053a39c();

	fn_0054c0d5();
}

// FUNCTION: STARFLEETOP 0x0054bf43
tMetaMapDataRequest::tResponse::tClientHex::tClientHex() :
	tStreamable( 1 )
{
}

// FUNCTION: STARFLEETOP 0x0054bf57
void tMetaMapDataRequest::tResponse::tClientHex::StreamOut( nDataStore::tOutput& output ) const
{
	output << m_0c << m_0d << m_10 << m_14 << m_15 << m_28 << m_29 << m_2a << (int8u)(m_24 * 100.0f);
}

// FUNCTION: STARFLEETOP 0x0054c025
void tMetaMapDataRequest::tResponse::tClientHex::StreamIn( nDataStore::tInput& input )
{
	iObjectVersion version = fObjectVersion;
	fObjectVersion = 1;

	int8u value;

	switch( version )
	{
		case 0:
			input >> m_0c >> m_10 >> m_14 >> m_15 >> m_28 >> m_29 >> m_2a >> value;
			break;

		case 1:
			input >> m_0c >> m_0d >> m_10 >> m_14 >> m_15 >> m_28 >> m_29 >> m_2a >> value;
			break;
	}

	m_24 = value * 0.01f;

	fn_0054c0d5();
}

// FUNCTION: STARFLEETOP 0x0054c0d5
void tMetaMapDataRequest::tResponse::tClientHex::fn_0054c0d5()
{
	m_18 = m_28 * 0.01f;
	m_1c = m_29 * 0.01f;
	m_20 = m_2a * 0.01f;
}

// FUNCTION: STARFLEETOP 0x0054c110
tMetaMapDataRequest::tResponse::tResponse( int32 param_1, int32 param_2, bool param_3 ) :
	nSwitch::tResponse( 0 ),
	m_20( true ),
	m_24( param_1 ),
	m_28( param_2 )
{
	if( param_3 )
	{
		m_10.resize( m_24 * m_28, tClientHex() );
	}
}

// FUNCTION: STARFLEETOP 0x0054c1b7
tMetaMapDataRequest::tResponse::~tResponse()
{
}

// FUNCTION: STARFLEETOP 0x0054c1ec
tMetaMapDataRequest::tResponse::tResponse( bool param_1 ) :
	nSwitch::tResponse( 0 ),
	m_20( false ),
	m_24( 1 ),
	m_28( 1 )
{
	if( param_1 )
	{
		m_10.resize( 1, tClientHex() );
	}
}

// FUNCTION: STARFLEETOP 0x0054c26d
void tMetaMapDataRequest::tResponse::StreamOut( nDataStore::tOutput& output ) const
{
	output << m_10 << m_20;

	if( !m_20 )
	{
		output << m_21 << m_22;
	}
}

// HYPOTHESIS: the vector reader at 0x0054c9a1, not claimed here (another lane's address range)
}

namespace nDataStore
{

template<>
tInput& operator>>( tInput& input, std::vector< nStoredProcedureArguments::tMetaMapDataRequest::tResponse::tClientHex >& value )
{
	value.clear();

	int32u size = 0;
	input.Read( (byte*)&size, sizeof( size ) );

	value.reserve( size );

	for( int i = 0; i < size; i++ )
	{
		nStoredProcedureArguments::tMetaMapDataRequest::tResponse::tClientHex type;
		input >> type;
		value.push_back( type );
	}

	return input;
}

};

namespace nStoredProcedureArguments
{

// FUNCTION: STARFLEETOP 0x0054c2d1
void tMetaMapDataRequest::tResponse::StreamIn( nDataStore::tInput& input )
{
	input >> m_10 >> m_20;

	if( !m_20 )
	{
		input >> m_21 >> m_22;
	}
}

// FUNCTION: STARFLEETOP 0x0054c311
void tMetaMapDataRequest::tResponse::fn_0054c311( tMetaMapHex& hex, int32 maxEconomyValue, int32 maxVictoryValue )
{
	tClientHex clientHex( hex, maxEconomyValue, maxVictoryValue );

	if( m_20 )
	{
		int32 index = fn_0054c4e3( hex.fn_00539f9c().fn_0055f269(), hex.fn_00539f9c().fn_005e8f21() );
		m_10[ index ] = clientHex;
	}
	else
	{
		m_21 = hex.fn_00539f9c().fn_0055f269();
		m_22 = hex.fn_00539f9c().fn_005e8f21();
		m_10[ 0 ] = clientHex;
	}
}

}
