#ifndef TSHIPSPECS_H
#define TSHIPSPECS_H

// HYPOTHESIS: only the size (0x6ac), the constructor/destructor and the int at +0x5dc are known.
class tShipSpecs
{
	public:
		tShipSpecs();
		~tShipSpecs();
		char m_00[0xc];
		int m_0c;
		int m_10;
		char m_14[0x5dc - 0x14];
		int m_5dc;
		char m_5e0[0x6ac - 0x5e0];
};

#endif
