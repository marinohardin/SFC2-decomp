//	taudiopriority.h: the audio priority manager part.

#ifndef	taudiopriority_h
#define	taudiopriority_h

#include "Tiger/tpart.h"

// HYPOTHESIS: declared only as far as the matched code needs it. The names come from the SDK
// library's symbols; the order of slots 6 and 7 is not recovered from the exe's vtable.
class	tAudioPriorityManagerPart : public tPart
{
	public:
		GenAbstractBinding( tAudioPriorityManagerPart );

		virtual		iTruth	mIsReady	( void );

		virtual		unsigned char	mAreWeAllowedToPlay	( unsigned long, long, long, long, long *, unsigned long &, double & );
		virtual		void	mWeHaveStoppedPlaying	( unsigned long );
};

#endif	// end taudiopriority_h block
