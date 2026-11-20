#include "Common/Exceptions/Validate.h"
#include <map>
#include <string>
#include "Meta/MetaDatabase/StoredProcedures/ShipProcedures/TransactionUpdateStoresRequest.h"












// FUNCTION: STARFLEETOP 0x0055b087
std::string tShipNames::fn_0055b087( const std::string& shipClass )
{
	std::map< std::string, std::string >::iterator itName = fShipNames.find( shipClass );
#line 203
	validate( itName != fShipNames.end() );
	if( itName != fShipNames.end() )
	{
		return itName->second;
	}
	return "";
}
#line 29



























// FUNCTION: STARFLEETOP 0x0055b12f
double* tStoresItemRates::fGetRateAddress( fStoresRateNode* itRate )
{
	fItemRates.find( &itRate, (int)itRate );
#line 221
	validate( itRate != fItemRates.end() );
	return &itRate->fRate;
}
#line 66



























// FUNCTION: STARFLEETOP 0x0055b183
int* tStoresStates::fn_0055b183( fStoresStateNode* itStores )
{
	fStoresStates.find( &itStores, (int)itStores );
#line 234
	validate( itStores != fStoresStates.end() );
	return &itStores->fValue;
}
