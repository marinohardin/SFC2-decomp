//	tGameState.h: tGameState, the base of the game's states (tCampaignState, tPanelMgr).
// Evidence:  class (RTTI; original file unknown)

#ifndef tGameState_h
#define tGameState_h 1

// The constructor (0x00469cd7) links the state into a list of every state and numbers it; the
// destructor (0x00469d19) unlinks it. Only the slots the derived classes need are declared: the
// vtable at 0x0079ec4c has 19.
class tView3D;

class tGameState
{
public:
	tGameState();
	virtual ~tGameState();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4(tView3D *param_1);
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8(int *param_1);
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual char *vfunction12();
	virtual void vfunction13();
	virtual bool vfunction14(int param_1);
	virtual void vfunction15();
	virtual void vfunction16(int *param_1);
	virtual bool vfunction17(int param_1);
	virtual void vfunction18();

	// HYPOTHESIS: the next state in the list (its head at 0x0090db24) and this state's number
	// (from the count at 0x0090db20).
	tGameState *m_04;
	int m_08;
};

#endif	// tGameState_h
