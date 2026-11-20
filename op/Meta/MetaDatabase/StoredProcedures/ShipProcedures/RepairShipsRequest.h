//	RepairShipsRequest.h
//
//	The request that repairs ships. Only what the rebuilt functions need is declared yet.
//
#if !defined( RepairShipsRequest_h )
#define RepairShipsRequest_h

#pragma once

#include <vector>

#include "Common/Database/DatabaseID.h"
#include "unsorted/tRequestWithResponse.h"

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

// HYPOTHESIS: a view of nStoredProcedureArguments::tRepairShipsRequest; 0x00568b2e calls
// 0x005596ed on a request with no adjustment, so the table at +0x38 is the request's.
class tRepairRates
{
public:
	double GetRepairRate( fRepairRateIter shipID );
protected:
	char pad[0x38];
	fRepairRateTree fRepairRates;
};

namespace nStoredProcedureArguments
{

	// RTTI names the class. 0x00568b2e builds one with 0x00559449 and streams it in.
	class tRepairShipsRequest : public tRequestWithResponse
	{
	public:
		tRepairShipsRequest();
		virtual ~tRepairShipsRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: returns the ID of the character the ships belong to.
		const nDatabase::tID& fn_0055e877() const;
		// HYPOTHESIS: returns the IDs of the ships to repair; the exe shares the code with
		// tKillShipsRequest's getter.
		const std::vector<nDatabase::tID>& fn_005596e9() const;

		std::vector<nDatabase::tID> m_18;
		// HYPOTHESIS: the rest (the character ID and the repair rates at +0x38) is not known.
		char m_28[0x48 - 0x28];
	};

};

#endif
