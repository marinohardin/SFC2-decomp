#include "Tiger/taudiopriority.h"

// MATCH-ONLY: not what the original said; the original's user of the class is not recovered.
// HYPOTHESIS: a use that makes the compiler emit the inline destructor's out-of-line copy
// and its deleting destructor (0x006636d0, 0x006636f0).
tClassActiveChannelList *fn_006636d0_make( void )
{
	return new tClassActiveChannelList;
}
