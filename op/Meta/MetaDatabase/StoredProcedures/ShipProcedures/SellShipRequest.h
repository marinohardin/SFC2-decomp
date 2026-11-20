#if !defined( SellShipRequest_h )
#define SellShipRequest_h

#include "Common/Database/DatabaseID.h"
#include "unsorted/tRequestWithResponse.h"
#include <map>
#include <string>

namespace nStoredProcedureArguments
{

// HYPOTHESIS: RTTI names the class and its base. nAsyncDatabaseProcedure::tTradeInShip holds
// one at +0x4c and it ends where that class's character begins (+0xc4), so it is 0x78 bytes;
// the rate tables below sit inside it (+0x3c, +0x50, +0x64).
class tSellShipRequest : public tRequestWithResponse
{
public:
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	// HYPOTHESIS: returns the ID of the ship to trade in; 0x0056964b compares it with the IDs
	// of the character's ships.
	const nDatabase::tID& fn_0055e613() const;

	char m_18[0x78 - 0x18];
};

};

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

// HYPOTHESIS: a third rate table whose end node sits at +0x68; the function returns the address
// of the rate inside the found node (node + 0x20).
class tShipItemRates
{
public:
	double* fGetRateAddress( fShipRepairRateNode* rate );

	char fPad[0x64];
	fShipRepairRateTree fItemRates;
};

#endif
