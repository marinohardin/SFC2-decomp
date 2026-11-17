// tMetaMapHex: declarations of this class (original file unknown).
// Only what the rebuilt functions need is declared yet.
#pragma once

#if !defined( tMetaMapHex_h )
#define tMetaMapHex_h

#include "port/types.h"
#include "Common/Database/DatabaseObject.h"
#include "unsorted/tHexPoint.h"
#include <map>
#include <vector>

// HYPOTHESIS: a database object; tBattles::StreamOut passes it to the tStreamable output
// operator with the base at offset 8, as for tDatabaseObject.
class tMetaMapHex : public nDatabase::tDatabaseObject
{
public:
	// HYPOTHESIS: constructed with one argument (tBattles passes 0); its type is not known.
	tMetaMapHex( int32u param_1 );
	~tMetaMapHex();
	// HYPOTHESIS: an out-of-line assignment; tBattles::operator= calls it for both copies.
	tMetaMapHex& operator=( const tMetaMapHex& other );

	// HYPOTHESIS: a hex point of the map, returned by value; 0x0042ac56 adds it to the points
	// 0x00539ff1 returns.
	tHexPoint fn_00539f9c();
	// HYPOTHESIS: hex points returned by value; 0x0042ac56 passes -1 for both arguments, whose
	// meaning is not known.
	std::vector< tHexPoint > fn_00539ff1( int32 param_1, int32 param_2 );
	// HYPOTHESIS: returns a map that tBattles::SQLFillInsertStream looks up with the keys 0
	// and 1; the key and value types are guesses from how the values are streamed.
	std::map< int32, float >& fn_0053a2fc();
	// HYPOTHESIS: a number per index; tBattles::SQLFillInsertStream writes it for 0 and 1.
	int32 fn_0053a414( int32 param_1 );
	// HYPOTHESIS: places the map at a hex point; 0x0042ac56 passes the one 0x0042abe4 returns.
	void fn_0053a49c( const tHexPoint& point );

	// HYPOTHESIS: 0xd0 bytes in all, the distance between the two copies tBattles holds.
	char fPad_48[0x88];
};

#endif
