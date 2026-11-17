#ifndef UNSORTED_THEXPOINT_H
#define UNSORTED_THEXPOINT_H

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36, assignment 0x005b69b0) is 0x14 bytes; its members are
// not recovered.
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
