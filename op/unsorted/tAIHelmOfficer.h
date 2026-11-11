//	tAIHelmOfficer.h: tAIHelmOfficer, the AI officer that steers a ship.
// Evidence:  class (RTTI; original file unknown)

#ifndef tAIHelmOfficer_h
#define tAIHelmOfficer_h 1

#include "tPriorityPowerRequest.h"
#include "tEntity.h"

class tObject_005237dc;

// HYPOTHESIS: the object at 0x140 of tAIHelmOfficer; 0x00515f27 takes an entity ID from it
// and passes it the ship's ID with a point.
class tObject_00525081
{
public:
	int fn_00525081();
	// HYPOTHESIS: whether the ship with this ID is in the formation; 0x00516879 asks it.
	bool fn_00525228(int aID);
	// HYPOTHESIS: a count for a ship in the formation; 0x0051e454 compares it with 1.
	int fn_005251e5(int aID);
	// HYPOTHESIS: a role of the ship with this ID in the formation; tAICaptain's 0x0051f602 lets
	// the formation choose the target for 1 and 3.
	int fn_0052518f(int aID);
	// HYPOTHESIS: the formation's target ID; tAICaptain's 0x0051f602 takes it as its own.
	int fn_00525338();
	void fn_005252b0(int aID, tPoint3 *aPoint);
	// HYPOTHESIS: 0x0051e613 passes its ship's ID and 1 at the end of its update.
	void fn_005253e8(int aID, int param_2);
};

// HYPOTHESIS: derives from tPriorityPowerRequest alone (RTTI), unlike the other AI officers;
// its own fields are known only from 0x00515db1. The base's destructor is not declared, so
// this one only restores the vtable.
class tAIHelmOfficer : public tPriorityPowerRequest
{
public:
	// HYPOTHESIS: the constructor at 0x00515c3d; tAICaptain's constructor passes its ship ID
	// and its power object at 0xd4, and the officer fills the 0x15c bytes up to its officers.
	tAIHelmOfficer(int aShipID, tObject_005237dc *aPower);
	virtual ~tAIHelmOfficer();

	void fn_00515db1();
	void fn_00515e79();
	void fn_00515ea9();
	void fn_00515f27();
	void fn_00516879();
	void fn_0051692e();
	void fn_0051696c(float aMaxSpeed);
	// HYPOTHESIS: takes a float; tAICaptain's 0x0051fce4 pushes one.
	void fn_005169d6(float param_1);
	void fn_005169e0(int param_1, int param_2);
	// HYPOTHESIS: takes no arguments and returns nothing; 0x0051e3a1 only jumps to it.
	void fn_00516dc1();
	// HYPOTHESIS: rates heading param_2 for the captain's 0x0051e613 and returns a kind in
	// param_3 (1 and 2 are treated apart).
	float fn_00516dc6(int param_1, int param_2, int *param_3);
	void fn_00516e2a();
	// HYPOTHESIS: takes a point by value; tAICaptain's 0x0051f1ed passes the point to leave for
	// once its ship has docked.
	void fn_00517e1a(tPoint3 aPoint);
	// HYPOTHESIS: takes the captain's target ID; tAICaptain's 0x0051ff8b passes it on.
	void fn_00517db0(int aTargetID);
	// HYPOTHESIS: an inline setter seen only in 0x0051e613: a mode at 0x5c and the fields
	// after it reset.
	void fn_0051e613_inline(int aMode);
	// HYPOTHESIS: tAICaptain's constructor calls it when the ship's state at 0xa40 is 7.
	void fn_00517e85();
	// HYPOTHESIS: takes the formation; tAICaptain's 0x0051f184 passes its new one.
	void fn_00517edc(tObject_00525081 *aFormation);
	// HYPOTHESIS: returns a point by reference; tAICaptain's 0x0051e37d copies it out. Its
	// address lies far from the officer's other methods, so it may belong to a base.
	const tPoint3 &fn_005462db();
	virtual float vfunction3_for_tPriorityPowerRequest(int *param_1, int param_2, float param_3);


	// HYPOTHESIS: a factor 0x00516879 sets to 20 or 8 by the ship's state, or 0 when following.
	float field_0x10;             // +0x10
	// HYPOTHESIS: the highest speed; 0x00515f27 clamps the speed it asks for to it.
	float field_0x14;             // +0x14
	// HYPOTHESIS: a state; 0x00515db1 clears it unless it is 3.
	int field_0x18;               // +0x18
	char field_0x1c[0x4];         // +0x1c
	// HYPOTHESIS: the ID the ship is looked up by.
	int field_0x20;               // +0x20
	char field_0x24[0x4];         // +0x24
	// HYPOTHESIS: the ship's position as 0x00515e79 last copied it.
	tPoint3 field_0x28;           // +0x28
	char field_0x34;              // +0x34
	// HYPOTHESIS: a flag that keeps 0x005169e0 from turning the ship to its heading.
	bool field_0x35;              // +0x35
	char field_0x36[0x2];         // +0x36
	// HYPOTHESIS: a float set by 0x005169d6.
	float field_0x38;             // +0x38
	char field_0x3c[0xc];         // +0x3c
	float field_0x48;             // +0x48
	// HYPOTHESIS: field_0x48 scaled by field_0x10, as 0x00516879 computes it.
	float field_0x4c;             // +0x4c
	// HYPOTHESIS: a flag that keeps 0x005169e0 from turning the ship to its heading.
	bool field_0x50;              // +0x50
	char field_0x51[0x3];         // +0x51
	// HYPOTHESIS: the ID of the entity the officer steers for; 0x00515db1 clears it to -1.
	int field_0x54;               // +0x54
	// HYPOTHESIS: slot 14 of that entity as 0x00515ea9 last read it.
	float field_0x58;             // +0x58
	// HYPOTHESIS: a mode 0x0051e613 sets to 0, 1 or 2 with the fields after it.
	int field_0x5c;               // +0x5c
	// HYPOTHESIS: a direction index; 0x005169e0 turns to the target when it is given it.
	int field_0x60;               // +0x60
	int field_0x64;               // +0x64
	char field_0x68;              // +0x68
	bool field_0x69;              // +0x69
	char field_0x6a[0x2];         // +0x6a
	// HYPOTHESIS: the defensive officer's mode and value, as 0x0051e613 hands them over.
	int field_0x6c;               // +0x6c
	int field_0x70;               // +0x70
	char field_0x74[0x8];         // +0x74
	int field_0x7c;               // +0x7c
	// HYPOTHESIS: a point 0x00515ea9 reads for that entity through the global at 0x0096eca8.
	tPoint3 field_0x80;           // +0x80
	// HYPOTHESIS: that entity's position as 0x00515ea9 last copied it.
	tPoint3 field_0x8c;           // +0x8c
	char field_0x98[0x90];        // +0x98
	// HYPOTHESIS: a flag 0x00516879 clears on each update.
	bool field_0x128;             // +0x128
	char field_0x129[0x3];        // +0x129
	// HYPOTHESIS: timers or weights 0x005169e0 compares with 0 before it turns the ship.
	float field_0x12c;            // +0x12c
	float field_0x130;            // +0x130
	float field_0x134;            // +0x134
	float field_0x138;            // +0x138
	int field_0x13c;              // +0x13c
	// HYPOTHESIS: an object 0x00515f27 asks for the ID of the entity to follow.
	tObject_00525081 *field_0x140; // +0x140
	// HYPOTHESIS: the speed 0x00515f27 asks for.
	float field_0x144;            // +0x144
	// HYPOTHESIS: the speed 0x00516c79 last sent.
	float field_0x148;            // +0x148
	// HYPOTHESIS: the heading 0x00515f27 asks for, or -1 for none.
	short field_0x14c;            // +0x14c
	// HYPOTHESIS: the followed entity's heading as 0x00515f27 last read it.
	short field_0x14e;            // +0x14e
	// HYPOTHESIS: the distance to the followed point as 0x00515f27 last measured it.
	float field_0x150;            // +0x150
	bool field_0x154;             // +0x154
	bool field_0x155;             // +0x155
	// HYPOTHESIS: set while the followed point is closer than 15.
	bool field_0x156;             // +0x156
	char field_0x157;             // +0x157
	// HYPOTHESIS: the time 0x00516c79 last sent a speed.
	float field_0x158;            // +0x158
};

#endif	// tAIHelmOfficer_h
