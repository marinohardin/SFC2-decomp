//	tTeamSlot.h: tTeamSlot, one character's place on a mission team (RTTI .?AVtTeamSlot@@).

#if !defined( tTeamSlot_h )
#define tTeamSlot_h

#pragma once

#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/fn_0042b0fd_owner.h"

class tServCharacter;

// HYPOTHESIS: a streamable (its first fields sit at +0xc, after tStreamable's); the members are
// placed by the offsets 0x00541499 writes, and their names are not known.
class tTeamSlot : public nDataStore::tStreamable
{
public:
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	void fn_00541499( tServCharacter* character );

	char field_0xc[8];
	int field_0x14;
	bool field_0x18;
	nDatabase::tID field_0x1c;
	fn_0042b0fd_owner field_0x2c;
};

#endif
