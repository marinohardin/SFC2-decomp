//	tCurrentTime.h: tCurrentTime, the meta campaign's clock position.

#if !defined( tCurrentTime_h )
#define tCurrentTime_h

#pragma once

#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: the eight constructor arguments are the turn, the turns per year, the
// milliseconds per turn, the base year and four starting dates read from the settings
// "Time/Clock/..." (0x0056c414); the argument names are not known.
class tCurrentTime : public nDataStore::tStreamable
{
public:
	tCurrentTime( long turn, long turnsPerYear, long milliSecondsPerTurn, long baseYear, long startingDate0, long startingDate1, long startingDate2, long startingDate3 );
	// HYPOTHESIS: 0x0042739e is the compiler's memberwise copy, out of line: five scalars
	// copied one by one, then the 16 bytes at +0x20 copied as a block.
	tCurrentTime( const tCurrentTime& source );

	// HYPOTHESIS: 0x0056c414 calls the assignment out of line (0x0045be62).
	tCurrentTime& operator=( const tCurrentTime& other );

	void StreamOut( nDataStore::tOutput& output ) const;
	void StreamIn( nDataStore::tInput& input );

	// HYPOTHESIS: tested by 0x0056c414 before the clock is reset; name not known.
	bool fn_005c7eac();

	// HYPOTHESIS: an accessor 0x0056d2cd passes to tAuctionItem::fn_00545560; name not known.
	long fn_0055f269();

	// HYPOTHESIS: the assignment 0x0045be62 copies 0x30 bytes: five longs from +0xc and four
	// more from +0x20 in a loop, likely the four starting dates. The members' names are not known.
	long fField0c;
	long fField10;
	long fField14;
	long fField18;
	long fField1c;
	long fField20[4];
};

#endif
