//	tHexPoint.h: tHexPoint, a 0x14-byte map position.
// Evidence:  class name from the exe's RTTI (.?AVtHexPoint@@) and its assert text "location != tHexPoint( -1, -1 )"; original file unknown

#ifndef tHexPoint_h
#define tHexPoint_h 1

#include "port/types.h"

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36) is 0x14 bytes; its members are not recovered.
class tHexPoint {
public:
	tHexPoint();
	// HYPOTHESIS: 0x005b6973 is the copy constructor (0x005bc50f passes the source).
	tHexPoint(const tHexPoint &other);
	~tHexPoint();
	// HYPOTHESIS: the out-of-line assignment at 0x005b69d0.
	tHexPoint &operator=(const tHexPoint &other);
	// HYPOTHESIS: two numbers that tBattles::SQLFillInsertStream writes and
	// tAsyncDatabaseSingle::vfunction13 matches against a hex; their meaning is not known.
	int32 fn_0055f269();
	int32 fn_005e8f21();
	int m_00;
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
};

#endif
