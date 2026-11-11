//	tObject_0096eca8.h: the global object at 0x0096eca8, used by the AI officers.

#ifndef tObject_0096eca8_h
#define tObject_0096eca8_h

#include "tEntity.h"

class tObject_00528a11;

// HYPOTHESIS: what 0x00527afe returns; only the angle at +0x18 is known, from 0x005169e0.
class tObject_00527afe
{
public:
	char field_0x0[0x18];
	short field_0x18;
};

// HYPOTHESIS: what 0x00528ddb returns for an entity ID; tAIHelmOfficer's 0x00515ea9 passes
// its target ID with 0, 4 and 0, and reads only a point from the result.
class tObject_00528ddb
{
public:
	// HYPOTHESIS: returns a point by value; 0x00515ea9 copies it to 0x80 of the officer.
	tPoint3 fn_00527c51();
	// HYPOTHESIS: returns a point by value for a fraction; 0x00515f27 passes 0.2 or 0.99.
	tPoint3 fn_00527bfd(float param_1);
	// HYPOTHESIS: a signed value; 0x005169e0 keeps only its sign as a turn direction.
	int fn_0055e617();
	// HYPOTHESIS: returns an object whose short at +0x18 0x005169e0 reads as an angle.
	tObject_00527afe *fn_00527afe();
	// HYPOTHESIS: an angle 0x005169e0 adds to the one above.
	short fn_00527b02();
};

// HYPOTHESIS: the global at 0x0096eca8; 0x005143d2 takes a float from it.
class tObject_0096eca8
{
public:
	float fn_00528a88();
	// HYPOTHESIS: 0x005146f0 passes 0 and its selector; the float returned is not used there.
	float fn_0052944d(float param_1, int *param_2);
	// HYPOTHESIS: the argument types; 0x00515ea9 passes an entity ID, then 0, 4 and 0.
	tObject_00528ddb *fn_00528ddb(int aID, int param_2, int param_3, int param_4);
	// HYPOTHESIS: takes an entity ID; 0x0051692e passes the helm officer's ship ID.
	void fn_00528dad(int aID);
	// HYPOTHESIS: a distance between two entities; 0x0052008f passes the captain's ship ID
	// and the ID its current order names.
	float fn_00528fbc(int aID, int aOtherID);
	// HYPOTHESIS: takes a float by reference and a selector; tAICaptain's 0x005200c8 passes 0.025
	// and then reads a point at that float. The float returned is not used there.
	float fn_00529485(float &aValue, const tObject_00528a11 &aSelector);
};

extern tObject_0096eca8 gObject_0096eca8;

#endif	// tObject_0096eca8_h
