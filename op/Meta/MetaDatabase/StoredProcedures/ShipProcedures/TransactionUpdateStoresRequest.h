//	TransactionUpdateStoresRequest.h
//
//	The request that buys stores for a character's ships, and its response. Only what the
//	rebuilt functions need is declared yet.
//
#if !defined( TransactionUpdateStoresRequest_h )
#define TransactionUpdateStoresRequest_h

#pragma once

#include <map>
#include <string>
#include <vector>

#include "Common/Database/DatabaseID.h"
#include "unsorted/tRequestWithResponse.h"
#include "unsorted/nSwitch__tResponse.h"

class tShip;

// HYPOTHESIS: the owning class and its ship name map are reconstructed from this one
// function's accesses only (the map's head node sits at +0x3c of the object). The key is taken
// by reference and the name sits at +0x1c of the tree node; 0x00569c83 passes a ship's ID as the
// key, so the key is probably a tID (0x10 bytes) rather than a std::string. The class name is a
// placeholder; only fShipNames comes from the assertion.
class tShipNames
{
public:
	std::string fn_0055b087( const std::string& shipClass );

	char fPad[0x38];
	std::map< std::string, std::string > fShipNames;
};

// HYPOTHESIS: a rate table whose find helper at 0x0055973a takes the key by value; its end node
// sits at +0x64 and the function returns the address of the rate inside the found node.
// 0x00569c83 uses the address as the ship's item rates object.
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

// HYPOTHESIS: a second table of the same shape; its last node sits at +0x50 and the function
// returns the address of the value inside the found node (+0x1c). 0x00569c83 copies a stores
// state from that address.
struct fStoresStateNode
{
	char pad[0x1c];
	int fValue;
};

struct fStoresStateTree
{
	void find( fStoresStateNode** pResult, int key );
	fStoresStateNode* end() { return fpEnd; }

	char pad[4];
	fStoresStateNode* fpEnd;
};

class tStoresStates
{
public:
	int* fn_0055b183( fStoresStateNode* itStores );

	char fPad[0x4c];
	fStoresStateTree fStoresStates;
};

// HYPOTHESIS: the object 0x0058b243 returns from the request; 0x005c7f7c gives the most
// missiles a ship may carry.
class c_005c7f7c
{
public:
	int fn_005c7f7c();
};

namespace nStoredProcedureArguments
{

	// RTTI names both classes. 0x00569c83 holds the request in 0xa4 bytes (built by 0x0055ac6a,
	// destroyed by 0x0055af53) and the response in 0x20 bytes, a ship vector at +0x10.
	class tTransactionUpdateStoresRequest : public tRequestWithResponse
	{
	public:
		class tTransactionUpdateStoresResponse : public nSwitch::tResponse
		{
		public:
			// HYPOTHESIS: the constructor (0x0055ac2b) takes the request and the updated ships.
			tTransactionUpdateStoresResponse( const tTransactionUpdateStoresRequest& request, const std::vector<tShip>& ships );

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			std::vector<tShip> m_10;
		};

		tTransactionUpdateStoresRequest();
		virtual ~tTransactionUpdateStoresRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// The assertion text of 0x00569c83 names the first getter. HYPOTHESIS: the exe shares the
		// code of both getters with tRepairShipsRequest's.
		const nDatabase::tID& GetCharacterID() const;
		const std::vector<nDatabase::tID>& fn_005596e9() const;
		c_005c7f7c* fn_0058b243();

		std::vector<nDatabase::tID> m_18;
		nDatabase::tID m_28;
		// HYPOTHESIS: the name, stores and rate tables (tShipNames, tStoresStates,
		// tStoresItemRates view this object) fill the rest.
		char m_38[0xa4 - 0x38];
	};

};

#endif
