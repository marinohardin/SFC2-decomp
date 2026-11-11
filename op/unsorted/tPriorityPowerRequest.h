//	tPriorityPowerRequest.h: tPriorityPowerRequest, a request for power with a priority.
// Evidence:  class (RTTI; original file unknown)

#ifndef tPriorityPowerRequest_h
#define tPriorityPowerRequest_h 1

// HYPOTHESIS: derives from tEnergyRequest (RTTI), which is not declared yet. The size, 0x10,
// is inferred from tAIWeaponOfficer, whose own fields start 0x10 bytes after this base. The
// constructor at 0x00524112 takes one argument.
// HYPOTHESIS: the object at +0x8; only its method at 0x005238d7 is known, called by
// 0x005147ab with a mode, an amount, 1, the integer at +0xc and a selector.
class tObject_005238d7
{
public:
	void fn_005238d7(int param_1, float param_2, int param_3, int param_4, int *param_5);
};

class tPriorityPowerRequest
{
public:
	tPriorityPowerRequest(int aParam);
	virtual void vfunction1();
	virtual void vfunction2();
	// HYPOTHESIS: slot 3 (offset 0x8) takes the selector and the integer 0x005146f0 was given
	// and the value of 0x00524109; the float it returns is not used there.
	// The name says which class's slot it is, as tAIOfficer has a slot 3 of its own.
	virtual float vfunction3_for_tPriorityPowerRequest(int *param_1, int param_2,
		float param_3);

	// HYPOTHESIS: returns a float; called by 0x005146f0 only so far.
	float fn_00524109();

	char fField04[0x4];       // +0x04
	// HYPOTHESIS: from 0x005147ab only: an object whose method 0x005238d7 it calls with the
	// integer at +0xc.
	tObject_005238d7 *field_0x8;  // +0x08
	int field_0xc;                // +0x0c
};

#endif	// tPriorityPowerRequest_h
