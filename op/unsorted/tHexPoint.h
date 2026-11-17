// tHexPoint: declarations of this class (original file unknown).
// Only what the rebuilt functions need is declared yet.
#pragma once

#if !defined( tHexPoint_h )
#define tHexPoint_h

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36) is 0x14 bytes; its members are not recovered. The
// constructor at 0x005b6a12 builds a tStreamable base and stores its two arguments at +0xc and
// +0x10.
class tHexPoint {
public:
	tHexPoint();
	// HYPOTHESIS: the two coordinates; their types are not known.
	tHexPoint(int x, int y);
	~tHexPoint();
	tHexPoint &operator=(const tHexPoint &other);
	// HYPOTHESIS: sets the two coordinates (tMetaMapHex's StreamIn, old version).
	void fn_005b6b62(int x, int y);
	// HYPOTHESIS: the two coordinates in the order tMetaMapHex's SQLFillInsertStream writes
	// them; the stream takes them as long.
	long fn_0055f269();
	long fn_005e8f21();
	int m_00;
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
};

#endif
