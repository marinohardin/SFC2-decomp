#include "Common/Exceptions/Validate.h"
#include <map>
#include <string>

// HYPOTHESIS: the owning class and its ship name map are reconstructed from this one
// function's accesses only (the map's head node sits at +0x3c of the object). The key is taken
// by reference and the name sits at +0x1c of the tree node, which fits a std::string key. The class name is a placeholder; only fShipNames comes from the assertion.
class tShipNames
{
public:
	std::string fn_0055b087( const std::string& shipClass );

	char fPad[0x38];
	std::map< std::string, std::string > fShipNames;
};

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

// HYPOTHESIS: a rate table whose find helper at 0x0055973a takes the key by value; its end node
// sits at +0x64 and the function returns the address of the rate inside the found node.
struct fStoresRateNode
{
	char pad[0x20];
	double fRate;
};

struct fStoresRateTree
{
	void find( fStoresRateNode** pResult, int key );
	fStoresRateNode* end() { return fpEnd; }

	char pad[4];
	fStoresRateNode* fpEnd;
};

class tStoresItemRates
{
public:
	double* fGetRateAddress( fStoresRateNode* itRate );

	char fPad[0x60];
	fStoresRateTree fItemRates;
};

// FUNCTION: STARFLEETOP 0x0055b12f
double* tStoresItemRates::fGetRateAddress( fStoresRateNode* itRate )
{
	fItemRates.find( &itRate, (int)itRate );
#line 221
	validate( itRate != fItemRates.end() );
	return &itRate->fRate;
}
