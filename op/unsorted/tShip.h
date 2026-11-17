//	tShip.h: tShip, a ship as the server's database holds it.

#if !defined( tShip_h )
#define tShip_h

#pragma once

#include "Common/Database/DatabaseObject.h"

// The class and its base are the exe's RTTI (nDatabase::tQuery<tShip>,
// nDatabase::tMemberEval<tShip,nDatabase::tID const&,...>). Its own members are not known yet.
class tShip : public nDatabase::tDatabaseObject
{
};

#endif
