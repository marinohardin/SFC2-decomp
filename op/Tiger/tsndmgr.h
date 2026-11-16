// tsndmgr.h: the sound file manager part.

#ifndef TSNDMGR_H
#define TSNDMGR_H

#include "Tiger/tpart.h"

enum eSoundChannelStatus {};

// HYPOTHESIS: declared only as far as the functions in the sound units need; mGetChannelPriority
// is the call through +0x78 (slot 30) in tClassTrackingInfo::mGetPriority, slots 6 to 29 are not
// recovered, and the slot of mGetTrackingIndex is unknown.
class tBasicSoundFileManager : public tPart
{
	public:
		virtual	void	vfunction6	( void );
		virtual	void	vfunction7	( void );
		virtual	unsigned char	vfunction8	( unsigned long );
		virtual	void	vfunction9	( void );
		virtual	void	vfunction10	( void );
		virtual	void	vfunction11	( void );
		virtual	void	vfunction12	( void );
		virtual	void	vfunction13	( void );
		virtual	void	vfunction14	( void );
		virtual	void	vfunction15	( void );
		virtual	void	vfunction16	( void );
		virtual	void	vfunction17	( void );
		virtual	void	vfunction18	( void );
		virtual	void	vfunction19	( unsigned long );
		virtual	void	vfunction20	( void );
		virtual	void	vfunction21	( void );
		virtual	void	vfunction22	( void );
		virtual	void	vfunction23	( void );
		virtual	void	vfunction24	( void );
		virtual	void	vfunction25	( void );
		virtual	void	vfunction26	( void );
		virtual	void	vfunction27	( void );
		virtual	void	vfunction28	( void );
		virtual	void	vfunction29	( void );
		virtual	long	mGetChannelPriority	( unsigned long, long );
		virtual	unsigned long	mGetTrackingIndex	( unsigned long handle );

		eSoundChannelStatus	mGetStatus	( unsigned long );

		GenAbstractBinding( tBasicSoundFileManager )
};

#endif // TSNDMGR_H
