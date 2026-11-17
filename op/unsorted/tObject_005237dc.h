#ifndef TOBJECT_005237DC_H
#define TOBJECT_005237DC_H

// HYPOTHESIS: a member object at +4 whose constructor is 0x00524090; its fields are unknown.
class tObject_00524090
{
public:
	tObject_00524090();

	char m_00[0x220];
};

// HYPOTHESIS: 0x005236d8 is a constructor: an index set to -1, then the member at +4 constructed.
class tObject_005236d8
{
public:
	tObject_005236d8();

	int m_00;
	tObject_00524090 m_04;
};

// HYPOTHESIS: 0x005237d1 is a constructor: a flag set, then a float cleared.
class tObject_005237d1
{
public:
	tObject_005237d1();

	bool m_00;
	float m_04;
};

// HYPOTHESIS: the 16-byte member at +0x34cc, laid out like a VC6 vector of 32-bit values (an
// allocator byte, then first, last and end-of-storage pointers); destroyed through 0x00571a7a,
// the destructor tPanelMgr's and tCampaignInfo's vectors use (each declared only in its own .cpp).
class tObject_005237dc_Vector
{
public:
	~tObject_005237dc_Vector();

	bool m_00;
	char m_01[3];
	int m_04;
	int m_08;
	int m_0c;
};

class tExtraEnergyRequest;
class tObject_00528a11;

// HYPOTHESIS: the power object at 0xd4 of tAICaptain, 0x34e0 bytes; its constructor (0x005237dc) takes the ship ID and 0x00523a86 adds an energy request to it.
class tObject_005237dc
{
public:
	tObject_005237dc(int aShipID);
	~tObject_005237dc();
	void fn_00523a86(tExtraEnergyRequest *aRequest);
	void fn_00523c41(int aMode); // HYPOTHESIS: takes a mode; 0x0051e454 passes 2 while the ship is in a formation, else 0.
	void fn_00523ad3(); // HYPOTHESIS: 0x0051e613 calls it before it chooses a selector; nothing returned is used.
	void fn_00523afd(tObject_00528a11 *aSelector); // HYPOTHESIS: takes the selector 0x0051e613 chose (the object at 0x35c8 of tAICaptain).

	float m_00[4];
	tObject_005237d1 m_10[25];
	bool m_d8;
	char m_d9[3];
	int m_dc[25];
	int m_140;
	int m_144;
	int m_148;
	int m_14c;
	tObject_005236d8 m_150[24];
	int m_34b0;
	char m_34b4[0x18];
	tObject_005237dc_Vector m_34cc;
	int m_34dc;
};

#endif
