//	tState_CampaignSelector.h: tState_Namespace_CampaignSelector::tState_CampaignSelector.
// Evidence:  class (RTTI; original file unknown)

#ifndef tState_CampaignSelector_h
#define tState_CampaignSelector_h 1

#include "unsorted/tGameState.h"

namespace tState_Namespace_CampaignSelector
{

// HYPOTHESIS: derived from tGameState like the other states; the constructor is at 0x0045e40a
// and the destructor at 0x0045e681. The members are not placed yet.
class tState_CampaignSelector : public tGameState
{
public:
	tState_CampaignSelector();
	virtual ~tState_CampaignSelector();
	// The vtable at 0x0079e8cc overrides slots 1, 2, 3, 12 and 14 of tGameState's (named as the
	// vtable export names them: slot N is vfunctionN+1); the rest are inherited.
	virtual bool vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	virtual char *vfunction13();
	virtual void vfunction15(c_0046aa10_message *param_1);
};

}

#endif	// tState_CampaignSelector_h
