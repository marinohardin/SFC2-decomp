//	tHexPoint.h: tHexPoint, a 0x14-byte map position.
// Evidence:  class name from the exe's RTTI (.?AVtHexPoint@@) and its assert text "location != tHexPoint( -1, -1 )"; original file unknown

#ifndef tHexPoint_h
#define tHexPoint_h 1

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36) is 0x14 bytes; its members are not recovered. The
// constructor at 0x005b6a12 builds a tStreamable base and stores its two arguments at +0xc and
// +0x10.
class tHexPoint {
public:
	tHexPoint();
	// HYPOTHESIS: the two coordinates; their types are not known.
	tHexPoint(int x, int y);
	// HYPOTHESIS: 0x005b6973 is the copy constructor (0x005bc50f passes the source).
	tHexPoint(const tHexPoint &other);
	// The exe's vtable 0x007b4230 holds the scalar deleting destructor in slot 0, and the vector
	// of these destroys each element through it (0x0053a83f), so the destructor is virtual and
	// +0x00 is the vtable pointer.
	virtual ~tHexPoint();
	tHexPoint &operator=(const tHexPoint &other);
	// HYPOTHESIS: 0x005b69d7, called by 0x00543a20 before it stores a new point.
	bool operator!=(const tHexPoint &other) const;
	// HYPOTHESIS: sets the two coordinates (tMetaMapHex's StreamIn, old version).
	void fn_005b6b62(int x, int y);
	// HYPOTHESIS: a member taking two arguments, both passed as 0 by 0x00543445,
	// which returns its result as tServCharacter's bool.
	bool fn_005b6a5c(int a, int b);
	// HYPOTHESIS: two numbers that tMetaMapHex's and tBattles's SQLFillInsertStream write and
	// tAsyncDatabaseSingle::vfunction13 matches against a hex; the stream takes them as long.
	long fn_0055f269() const;
	long fn_005e8f21() const;
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
};

#endif
