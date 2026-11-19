#include "Common/Exceptions/BaseException.h"

// HYPOTHESIS: the owning class and the lookup table are not recovered; a tree
// header at +0x38/+0x3c with a find helper that takes the key by value.
struct fRepairRateNode
{
	char pad[0x20];
	double fRate;
};

struct fRepairRateIter
{
	fRepairRateIter( fRepairRateNode* p ) : fpNode( p ) {}
	operator int() const { return (int)fpNode; }
	fRepairRateNode* fpNode;
};

struct fRepairRateTree
{
	void find( fRepairRateIter* pResult, int key );
	char pad[4];
	fRepairRateNode* fpEnd;
};

class tRepairRates
{
public:
	double GetRepairRate( fRepairRateIter shipID );
protected:
	char pad[0x38];
	fRepairRateTree fRepairRates;
};

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
