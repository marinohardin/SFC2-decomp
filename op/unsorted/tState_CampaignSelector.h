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
};

}

#endif	// tState_CampaignSelector_h
