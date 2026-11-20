#include "Meta/MetaDatabase/StoredProcedures/ShipProcedures/SellShipRequest.h"
#include "Common/Exceptions/Validate.h"

// FUNCTION: STARFLEETOP 0x0055a13a
double tTradeInRates::fGetRate( const std::string& shipClass )
{
	std::map< std::string, double >::iterator rate = fTradeInRates.find( shipClass );
#line 159
	validate( rate != fTradeInRates.end() );
	return rate->second;
}
#line 13

// FUNCTION: STARFLEETOP 0x0055a18e
double tShipRepairRates::fGetRate( fShipRepairRateNode* rate )
{
	fRepairRates.find( &rate, (int)rate );
#line 170
	validate( rate != fRepairRates.end() );
	return rate->fRate;
}
#line 23

// FUNCTION: STARFLEETOP 0x0055a1e2
double* tShipItemRates::fGetRateAddress( fShipRepairRateNode* rate )
{
	fItemRates.find( &rate, (int)rate );
#line 181
	validate( rate != fItemRates.end() );
	return &rate->fRate;
}
