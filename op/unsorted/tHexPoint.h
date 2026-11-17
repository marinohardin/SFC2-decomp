//	tHexPoint.h: tHexPoint, a 0x14-byte map position.
// Evidence:  class name from the analysis of its destructor at 0x005b6a36 (original file unknown)

#ifndef tHexPoint_h
#define tHexPoint_h 1

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36) is 0x14 bytes; its members are not recovered.
// unsorted/tClientCharacter.cpp still carries its own copy of this declaration;
// it should include this header instead.
class tHexPoint {
public:
	tHexPoint();
	~tHexPoint();
	tHexPoint &operator=(const tHexPoint &other);
	int m_00;
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
};

#endif
