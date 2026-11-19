#include "unsorted/tMetaClientNewsCtlrC.h"
#include "unsorted/fn_00571a58_owner.h"
#include "Common/Exceptions/Validate.h"

// HYPOTHESIS: returns the thread state whose default use the optex takes; the name is not known.
tThreadState *fn_005d4c97();

// FUNCTION: STARFLEETOP 0x005dd252
tMetaClientNewsCtlrC::tMetaClientNewsCtlrC( void* relay ) :
	fRelay( relay ),
	fOptex( fn_005d4c97()->DefaultThreadObjectUse() )
{
#line 53
	validate( relay );

	// MATCH-ONLY: push_back is reached through the folded copy claimed at 0x00571a69.
	{
		const tMenuItem::eType& type = (tMenuItem::eType)0;
		((fn_00571a58_owner*)&fMenuTypes)->fn_00571a69( (int)&type );
	}
	{
		const tMenuItem::eType& type = (tMenuItem::eType)1;
		((fn_00571a58_owner*)&fMenuTypes)->fn_00571a69( (int)&type );
	}
}
