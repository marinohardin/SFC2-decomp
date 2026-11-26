//	tGameState.h: tGameState, the base of the game's states (tCampaignState, tPanelMgr).
// Evidence:  class (RTTI; original file unknown)

#ifndef tGameState_h
#define tGameState_h 1

// The constructor (0x00469cd7) links the state into a list of every state and numbers it; the
// destructor (0x00469d19) unlinks it. The vtable at 0x0079ec4c has 19 slots.
// NAMING: the virtual functions are named as the vtable export names them: slot N is
// vfunctionN+1 (slot 0 is the deleting destructor). The slots are declared with the original's
// own arities (the dispatchers 0x0046a46a to 0x0046a57c and the shared bodies give them): 0x005d0311
// (slots 4 to 6, 8 to 10, 14 and 16) is "ret 4", 0x004bc9a7 (slots 1 and 3) returns true,
// 0x005d80e0 (slot 2) is "ret", 0x005e8efe (slot 11) returns the word at +4, 0x00737f86 (slot 12)
// is the C runtime's pure-virtual handler and 0x00402fa5 (slot 18) returns zero (tState_Tactical's
// override, 0x004af807, returns an address). The argument types of the stubs are the types the
// dispatchers and the overriding states use. Slot 14 takes the queue's message (c_0046aa10_msg.h).
class tView3D;
struct c_0046aa10_message;

class tGameState
{
public:
	tGameState();
	virtual ~tGameState();
	// 0x00469e0c is in no vtable (slot 17 is 0x0046ad4d); the states call it directly with a
	// state name, and it returns false unless this is the current state (0x0046a59a).
	bool fn_00469e0c(const char *name);
	// 0x00469e2a is in no vtable either; it calls 0x0046995a on the global at 0x0090db88.
	void fn_00469e2a();
	virtual bool vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	virtual void vfunction5(tView3D *param_1);
	virtual void vfunction6(int param_1);
	virtual void vfunction7(int param_1);
	virtual void vfunction8(int *param_1);
	virtual void vfunction9(int *param_1);
	virtual void vfunction10(int param_1);
	virtual void vfunction11(int param_1);
	virtual tGameState *vfunction12();
	virtual char *vfunction13() = 0;
	virtual bool vfunction14(int param_1);
	virtual void vfunction15(c_0046aa10_message *param_1);
	virtual void vfunction16(int *param_1);
	virtual void vfunction17(int *param_1);
	virtual void vfunction18(int *param_1);
	virtual void *vfunction19();

	// HYPOTHESIS: the next state in the list (its head at 0x0090db24) and this state's number
	// (from the count at 0x0090db20).
	tGameState *m_04;
	int m_08;
};

#endif	// tGameState_h
