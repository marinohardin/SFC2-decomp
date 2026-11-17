#ifndef TSHIP_H
#define TSHIP_H

#include "Common/Database/DatabaseObject.h"

// HYPOTHESIS: a database object like its neighbours (tMetaMapHex, tBattles); its allocator
// news 0x434 bytes, constructs it with 0x00545737 and sets the allocator ID on it.
// Members past the base are not known yet.
class tShip : public nDatabase::tDatabaseObject
{
public:
	tShip();

	char fPad_48[0x3ec];
};

#endif
