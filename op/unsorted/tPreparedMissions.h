#ifndef TPREPAREDMISSIONS_H
#define TPREPAREDMISSIONS_H

#include "Common/Database/DatabaseObject.h"

// HYPOTHESIS: a database object like its neighbours (tMetaMapDesc, tBattles); its allocator
// news 0x98 bytes and sets the allocator ID on it. Members past the base are not known yet.
class tPreparedMissions : public nDatabase::tDatabaseObject
{
public:
	tPreparedMissions();

	char fPad_48[0x50];
};

#endif
