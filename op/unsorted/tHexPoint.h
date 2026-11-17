#ifndef UNSORTED_THEXPOINT_H
#define UNSORTED_THEXPOINT_H

// HYPOTHESIS: tHexPoint (dtor 0x005b6a36, assignment 0x005b69b0) is 0x14 bytes; its members are
// not recovered.
class tHexPoint {
public:
	tHexPoint();
	// HYPOTHESIS: 0x005b6973, called with the source pointer by 0x0042abe4.
	tHexPoint(const tHexPoint &other);
	~tHexPoint();
	tHexPoint &operator=(const tHexPoint &other);
	// HYPOTHESIS: two numbers that key a hex point in 0x0042ac56's map (first, second); their
	// meaning is not known.
	int fn_0055f269();
	int fn_005e8f21();
	int m_00;
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
};

#endif
