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

}
