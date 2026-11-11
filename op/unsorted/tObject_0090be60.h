//	tObject_0090be60.h: the object that the pointer at 0x0090be60 holds.
// Evidence:  calls from 0x00413541 and 0x004e2b02 (original name and file unknown)

#ifndef tObject_0090be60_h
#define tObject_0090be60_h 1

// HYPOTHESIS: the class is inferred from its callers only; fn_00427581 returns 5 for the
// relation that sends kAIChangeTeamRelationshipToWar, and fn_0042753a returns a positive id
// for the ship id it is given, or zero.
class tObject_0090be60
{
public:
	int fn_0042748f(int aValue);
	int fn_0042753a(int aValue);
	int fn_00427581(int aValue1, int aValue2);
};

extern tObject_0090be60 *gObject_0090be60;

#endif // tObject_0090be60_h
