#ifndef TMETACLIENTNEWSCTLRC_H
#define TMETACLIENTNEWSCTLRC_H

// tMetaClientNewsCtlrC (name from RTTI). Only what the matched code uses is declared.
// HYPOTHESIS: bases are left out until a function needs them; the class is single
// inheritance so that a pointer to its members is the 4 bytes the code passes.

#include <vector>
#include "Common/Thread/Optex.h"

class tNewsStory;

class tMetaClientNewsCtlrC
{
public:
	// HYPOTHESIS: tMenuItem and eType are named by the analysis of the callback instance at
	// 0x005de6a6; the enumerators are not known yet.
	class tMenuItem
	{
	public:
		enum eType
		{
		};
	};

	// The parameter name is from the constructor's validate( relay ); its type is not known.
	tMetaClientNewsCtlrC( void* relay );
	virtual ~tMetaClientNewsCtlrC();

protected:
	// HYPOTHESIS: member types follow the constructor (+0x04 checked non-null, +0x08 receives
	// two menu item types, +0x18 an optex); the element types of the later vectors are guesses.
	void*							fRelay;
	std::vector< tMenuItem::eType >	fMenuTypes;
	tOptex							fOptex;
	std::vector< tNewsStory* >		fStories;
	std::vector< tMenuItem* >		fMenuItems;
	std::vector< void* >			fSubscriptions;
};

#endif
