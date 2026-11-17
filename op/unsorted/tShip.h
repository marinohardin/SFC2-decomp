//	tShip.h: tShip, a ship as the server's database holds it.

#if !defined( tShip_h )
#define tShip_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The class and its base are the exe's RTTI (nDatabase::tQuery<tShip>,
// nDatabase::tMemberEval<tShip,nDatabase::tID const&,...>). Its own members are not known yet.
class tShip : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: the exe holds the destructor out of line (0x0042801a); 0x0056e095 calls it.
	virtual ~tShip();

	// HYPOTHESIS: the record is 0x434 bytes (0x0056e095 takes one by value and returns with ret 0x434).
	char fPad_48[0x434 - 0x48];
};

#endif
