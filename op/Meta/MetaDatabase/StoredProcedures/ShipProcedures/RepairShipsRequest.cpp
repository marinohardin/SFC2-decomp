#include "Common/Exceptions/BaseException.h"
#include "Meta/MetaDatabase/StoredProcedures/ShipProcedures/RepairShipsRequest.h"































// FUNCTION: STARFLEETOP 0x005596ed
double tRepairRates::GetRepairRate( fRepairRateIter shipID )
{
	fRepairRates.find( &shipID, shipID );
	if ( shipID.fpNode == fRepairRates.fpEnd )
	{
		throw nException::tBaseException( "shipID not found in fRepairRates", __FILE__, 137 );
	}
	return shipID.fpNode->fRate;
}
