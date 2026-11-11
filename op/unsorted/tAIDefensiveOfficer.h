//	tAIDefensiveOfficer.h: tAIDefensiveOfficer, the AI officer that rates headings for defence.
// Evidence:  class (RTTI; original file unknown)

#ifndef tAIDefensiveOfficer_h
#define tAIDefensiveOfficer_h 1

#include "tAIOfficer.h"

// HYPOTHESIS: declared only as far as tAICaptain uses it; its constructor takes the ship ID.
class tAIDefensiveOfficer : public tAIOfficer
{
public:
	tAIDefensiveOfficer(int aShipID);
	virtual ~tAIDefensiveOfficer();
	// HYPOTHESIS: rates a selector against the target's ID; 0x0051e613 weighs the offensive and
	// defensive officers' answers. Not slot 6 of tAIOfficer, which returns nothing
	// (0x0051f9e9), so taken as the first slot after tAIOfficer's.
	virtual float vfunction15(int *param_1, int param_2);
	// HYPOTHESIS: a mode 0x0051e613 tests for 0 to 5 and hands to the helm officer.
	int fn_00515288();
	// HYPOTHESIS: a value 0x0051e613 hands to the helm officer with the mode.
	int fn_005152ae();

	char field_0x14[0x14];
	// HYPOTHESIS: the target ID; 0x0051e613 sets it to the captain's.
	int field_0x28;                         // +0x28
	char field_0x2c[0x3c];
};

#endif	// tAIDefensiveOfficer_h
