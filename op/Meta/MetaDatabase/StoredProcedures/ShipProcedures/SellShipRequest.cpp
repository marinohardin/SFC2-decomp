#include "Common/Exceptions/Validate.h"
#include <map>
#include <string>

// HYPOTHESIS: the owning class and its trade-in rate map are reconstructed from this one
// function's accesses only (the map's head node sits at +0x40 of the object). The key is taken
// by reference and the rate sits at +0x20 of the tree node, which fits a 16-byte std::string key.
class tTradeInRates
{
public:
	double fGetRate( const std::string& shipClass );

	char fPad[0x3c];
	std::map< std::string, double > fTradeInRates;
};

// FUNCTION: STARFLEETOP 0x0055a13a
double tTradeInRates::fGetRate( const std::string& shipClass )
{
	std::map< std::string, double >::iterator rate = fTradeInRates.find( shipClass );
#line 159
	validate( rate != fTradeInRates.end() );
	return rate->second;
}
#line 26
// HYPOTHESIS: a second rate table (repair rates) with a find helper at 0x0055973a taking the key
// by value; its end node sits at +0x54 and the rate at +0x20 of a node.
struct fShipRepairRateNode
{
	char pad[0x20];
	double fRate;
};

struct fShipRepairRateTree
{
	void find( fShipRepairRateNode** pResult, int key );
	fShipRepairRateNode* end() { return fpEnd; }

	char pad[4];
	fShipRepairRateNode* fpEnd;
};

class tShipRepairRates
{
public:
	double fGetRate( fShipRepairRateNode* rate );

	char fPad[0x50];
	fShipRepairRateTree fRepairRates;
};

// FUNCTION: STARFLEETOP 0x0055a18e
double tShipRepairRates::fGetRate( fShipRepairRateNode* rate )
{
	fRepairRates.find( &rate, (int)rate );
#line 170
	validate( rate != fRepairRates.end() );
	return rate->fRate;
}
#line 61

// HYPOTHESIS: a third rate table whose end node sits at +0x68; the function returns the address
// of the rate inside the found node (node + 0x20).
class tShipItemRates
{
public:
	double* fGetRateAddress( fShipRepairRateNode* rate );

	char fPad[0x64];
	fShipRepairRateTree fItemRates;
};

// FUNCTION: STARFLEETOP 0x0055a1e2
double* tShipItemRates::fGetRateAddress( fShipRepairRateNode* rate )
{
	fItemRates.find( &rate, (int)rate );
#line 181
	validate( rate != fItemRates.end() );
	return &rate->fRate;
}
#line 82
