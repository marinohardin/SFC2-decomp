//	tState_NetSetup: a game state of the network setup screens.
// Evidence:  class (RTTI; original file unknown)

#include "unsorted/tGameState.h"

namespace tState_Namespace_NetSetup
{

// HYPOTHESIS: derives from tGameState like tState_GameConnections; only the slots rebuilt so
// far are declared.
class tState_NetSetup : public tGameState
{
public:
	virtual void vfunction1();
	virtual bool vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	// HYPOTHESIS: returns the state's name.
	virtual char *vfunction13();
};

// FUNCTION: STARFLEETOP 0x00475095
char *tState_NetSetup::vfunction13()
{
	return "netsetup";
}

}	// namespace tState_Namespace_NetSetup
