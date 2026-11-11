//	tAIOfficer.h: tAIOfficer, the base of a ship captain's AI officers.
// Evidence:  class (RTTI; original file unknown)

#ifndef tAIOfficer_h
#define tAIOfficer_h 1

// HYPOTHESIS: the layout is inferred from tAIWeaponOfficer's constructor only: a virtual
// table, then the ID its derived constructors look the ship up by at +0x4, 0x14 bytes in all.
// The constructor at 0x0051a065 takes one argument.
// HYPOTHESIS: the object at +0x10, called by 0x00514141; only two of its methods are known,
// both taking the ship ID first.
class tObject_00525270
{
public:
	int fn_00525270(int aShipID);
	bool fn_005259b2(int aShipID, int param_2, int param_3, int *param_4, int param_5);
};

class tObject_00525081;

class tAIOfficer
{
public:
	tAIOfficer(int aParam);
	virtual ~tAIOfficer();
	// HYPOTHESIS: slot 2 (offset 0x4) takes a selector and the target's ID; 0x0051e613 passes
	// the captain's chosen selector and its target to each officer.
	virtual void vfunction2(int *param_1, int param_2);
	// HYPOTHESIS: slot 3 (offset 0x8) says whether the ship can use the officer; tAICaptain's
	// constructor deletes an officer for which it is false.
	virtual bool vfunction3();
	// HYPOTHESIS: slot 4 (offset 0xc) takes one argument; tAICaptain's 0x0051f153 forwards its second argument.
	// HYPOTHESIS: slot 4 (offset 0xc) takes one argument; tAICaptain's 0x0051f153 forwards its second argument.
	virtual void vfunction4(int param_1);
	// HYPOTHESIS: slot 5 (offset 0x10) takes the formation; tAICaptain's 0x0051f184 hands its
	// new formation to every officer.
	virtual void vfunction5(tObject_00525081 *param_1);
	// HYPOTHESIS: slot 6 (offset 0x14) takes no arguments and returns nothing: tAICaptain's
	// 0x0051f9e9 calls it on the tractor officer that way.
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	// HYPOTHESIS: slots 11 and 12 (offsets 0x2c and 0x30) take the selector and the integer
	// 0x005142b3 was given; nothing returned is used.
	virtual void vfunction12(int *param_1, int param_2);
	virtual void vfunction13(int *param_1, int param_2);
	// HYPOTHESIS: slot 14 (offset 0x34) returns an integer; 0x00514141 passes it on.
	virtual int vfunction14();

	// HYPOTHESIS: takes a float; tAICaptain's constructor passes its own float at 0x3a30 to
	// each officer it keeps. The function lies far from the officers, so it may belong to a
	// base not yet recovered.
	void fn_005c7fc6(float aValue);

	int fShipID;                    // +0x04
	char fField08[0x8];             // +0x08
	tObject_00525270 *fField10;     // +0x10
};

#endif	// tAIOfficer_h
