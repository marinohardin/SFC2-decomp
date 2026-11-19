//	MetaMapDataRequest.cpp
//
//	The map data request: the hexes of the meta map as the client receives them.
//
#include "Meta/MetaDatabase/StoredProcedures/MapProcedures/MetaMapDataRequest.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/tMetaMapHex.h"

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

}
