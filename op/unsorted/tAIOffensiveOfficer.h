//	tAIOffensiveOfficer.h: tAIOffensiveOfficer, the AI officer that rates headings for attack.
// Evidence:  class (RTTI; original file unknown)

#ifndef tAIOffensiveOfficer_h
#define tAIOffensiveOfficer_h 1

#include "tAIOfficer.h"

class tObject_005237dc;

// HYPOTHESIS: declared only as far as tAICaptain uses it; tAICaptain's constructor passes its
// ship ID and its power object at 0xd4.
class tAIOffensiveOfficer : public tAIOfficer
{
public:
	tAIOffensiveOfficer(int aShipID, tObject_005237dc *aPower);
	virtual ~tAIOffensiveOfficer();
	// HYPOTHESIS: as tAIDefensiveOfficer's vfunction15: rates a selector against the target's ID.
	virtual float vfunction15(int *param_1, int param_2);
	void fn_0051e275_inline(const int &param_1, const int &param_2);
	// HYPOTHESIS: 0x0051e613 passes the captain's target ID; the address lies far from the
	// officer's other methods, so it may belong to a base.
	void fn_00420f2f(int aID);
	void fn_00518f89();
	// HYPOTHESIS: takes a value; tAICaptain's 0x0051f0e1 passes 0 before ordering a disengage.
	void fn_00519130(int param_1);
	// HYPOTHESIS: these return the counts 0x0051e613 turns into ratios; those taking a float
	// get 0.3.
	short fn_00519a88();
	short fn_00519ac3(float param_1);
	short fn_00519b08();
	short fn_00519b43(float param_1);
	short fn_00519b88();
	short fn_00519bc3();
	// HYPOTHESIS: draws what the officer knows through the object given; tAICaptain's 0x005200c8
	// ends with it.
	void fn_00519c02(tObject_00413ac3_arg *aObject);

	char field_0x14[0x8c];
	int field_0xa0;                         // +0xa0
	int field_0xa4;                         // +0xa4
	char field_0xa8[0xd0];
	// HYPOTHESIS: 0x0051e454 sets it to 3 for a ship standing still outside a larger formation.
	short field_0x178;                      // +0x178
	char field_0x17a[0x22];
	// HYPOTHESIS: a flag 0x0051e613 copies to the captain's 0x3a44 once.
	char field_0x19c;                       // +0x19c
	char field_0x19d[0x3];
};

#endif	// tAIOffensiveOfficer_h
