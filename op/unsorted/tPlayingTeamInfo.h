//	tPlayingTeamInfo.h
//
//	The streamed description of one team in a mission.
//
#pragma once

#if !defined( tPlayingTeamInfo_h )
#define tPlayingTeamInfo_h

#include <string>
#include <vector>

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"
#include "unsorted/tShip.h"

// HYPOTHESIS: the class named in RTTI (vtable 0x00798234); layout read from its copy
// constructor at 0x00427ab7: a string, four scalars, an ID, a vector copied out of line
// (0x00427bb2), two scalars and a byte.
class tPlayingTeamInfo : public nDataStore::tStreamable
{
public:
	tPlayingTeamInfo( const tPlayingTeamInfo& source );
	virtual	~tPlayingTeamInfo();

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	std::string			field_0xc;
	int32u				field_0x1c;
	int32u				field_0x20;
	int32u				field_0x24;
	int32u				field_0x28;
	nDatabase::tID		field_0x2c;
	std::vector< tShip >	field_0x3c;
	int32u				field_0x4c;
	int32u				field_0x50;
	bool				field_0x54;
};

#endif
