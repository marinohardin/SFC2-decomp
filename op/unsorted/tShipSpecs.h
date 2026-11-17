#ifndef TSHIPSPECS_H
#define TSHIPSPECS_H

// HYPOTHESIS: only the size (0x6ac), the constructor/destructor and the int at +0x5dc are known.
class tShipSpecs
{
	public:
		tShipSpecs();
		~tShipSpecs();
		char m_00[0x5dc];
		int m_5dc;
		char m_5e0[0x6ac - 0x5e0];
};

#endif
