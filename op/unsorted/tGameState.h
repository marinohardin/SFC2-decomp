//	tGameState.h: tGameState, the base of the game's states (tCampaignState, tPanelMgr).
// Evidence:  class (RTTI; original file unknown)

#ifndef tGameState_h
#define tGameState_h 1

// The constructor (0x00469cd7) links the state into a list of every state and numbers it; the
// destructor (0x00469d19) unlinks it. Only the slots the derived classes need are declared: the
// vtable at 0x0079ec4c has 19.
class tGameState
{
public:
	tGameState();
	virtual ~tGameState();

	// HYPOTHESIS: the next state in the list (its head at 0x0090db24) and this state's number
	// (from the count at 0x0090db20).
	tGameState *m_04;
	int m_08;
};

#endif	// tGameState_h
