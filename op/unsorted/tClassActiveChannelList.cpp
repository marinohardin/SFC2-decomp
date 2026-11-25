#include "Tiger/taudiopriority.h"

// MATCH-ONLY: not what the original said; the original's user of the class is not recovered.
// HYPOTHESIS: a use that makes the compiler emit the inline destructor's out-of-line copy
// (0x006636f0), which is claimed in Tiger/taudiopriority.h.
tClassActiveChannelList *fn_006636f0_make( void )
{
	return new tClassActiveChannelList;
}
