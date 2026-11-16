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
		virtual	void	vfunction32	( void );
		virtual	void	vfunction33	( void );
		virtual	void	vfunction34	( void );
		virtual	void	vfunction35	( void );
		virtual	void	vfunction36	( void );
		virtual	void	vfunction37	( void );
		virtual	void	vfunction38	( void );
		virtual	void	vfunction39	( void );
		virtual	void	vfunction40	( void );
		virtual	void	vfunction41	( void );
		virtual	void	vfunction42	( void );
		virtual	void	vfunction43	( void );
		virtual	void	vfunction44	( void );
		virtual	void	vfunction45	( void );
		virtual	void	vfunction46	( void );
		virtual	void	vfunction47	( void );
		virtual	void	vfunction48	( void );
		virtual	void	vfunction49	( void );
		virtual	void	vfunction50	( void );
		virtual	void	vfunction51	( void );
		virtual	void	vfunction52	( void );
		virtual	void	vfunction53	( void );
		virtual	void	vfunction54	( void );
		virtual	void	vfunction55	( void );
		virtual	void	vfunction56	( void );
		virtual	void	vfunction57	( void );
		virtual	void	vfunction58	( void );
		virtual	void	vfunction59	( void );
		virtual	void	vfunction60	( void );
		virtual	void	vfunction61	( void );
		virtual	void	vfunction62	( void );
		virtual	void	vfunction63	( void );
		virtual	void	vfunction64	( void );
		virtual	void	vfunction65	( void );
		virtual	void	vfunction66	( void );
		virtual	void	vfunction67	( void );
		virtual	void	vfunction68	( void );
		virtual	void	vfunction69	( void );
		virtual	void	vfunction70	( void );
		virtual	void	vfunction71	( void );
		virtual	void	vfunction72	( void );
		virtual	void	vfunction73	( void );
		// HYPOTHESIS: slot 74 (+0x128, called by tClassTrackingInfo::mFadeToStop with a channel,
		// 500.0 and -1) is one of the two fades with this signature in the SDK library symbols;
		// which one is not checked against the vtable.
		virtual	unsigned char	mFadeToStopAndResetNow	( unsigned long, double, long );

		eSoundChannelStatus	mGetStatus	( unsigned long );

		GenAbstractBinding( tBasicSoundFileManager )
};

#endif // TSNDMGR_H
