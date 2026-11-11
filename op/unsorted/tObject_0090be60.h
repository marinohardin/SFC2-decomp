//	tObject_0090be60.h: the object that the pointer at 0x0090be60 holds.
// Evidence:  calls from 0x00413541 and 0x004e2b02 (original name and file unknown)

#ifndef tObject_0090be60_h
#define tObject_0090be60_h 1

#include <vector>

class tObject_004274f4;

// HYPOTHESIS: the class is inferred from its callers only; fn_00427581 returns 5 for the
// relation that sends kAIChangeTeamRelationshipToWar, and fn_0042753a returns a positive id
// for the ship id it is given, or zero. fn_00426f1a returns a list whose size 0x0048c418
// compares with the fleet panel's count; fn_004274f4 returns the object the fleet entry's
// buttons are forwarded to for a fleet ID.
class tObject_0090be60
{
public:
	int fn_0042748f(int aValue);
	// HYPOTHESIS: returns a value for an ID; 0x0051e3ac keeps it in a new entry.
	int fn_0042750a(int aValue);
	int fn_0042753a(int aValue);
	int fn_00427581(int aValue1, int aValue2);
	// HYPOTHESIS: sets the relation of two teams; 0x0051e1f7 passes 6 when it was 5.
	void fn_004275fe(int aValue1, int aValue2, int aRelation);
	std::vector<int> *fn_00426f1a(int aValue);
	tObject_004274f4 *fn_004274f4(int aValue);
};

extern tObject_0090be60 *gObject_0090be60;

#endif // tObject_0090be60_h
