// taudiopriority.h: Tiger's audio priority manager part and its per-class tracking records
// (class names from the exe's RTTI and the SDK library symbols).

#ifndef TAUDIOPRIORITY_H
#define TAUDIOPRIORITY_H

#include "Tiger/tpart.h"

// HYPOTHESIS: a 0x10-byte list, from where the inlined tAudioFileClass constructor in
// mNewAudioFileClass (0x00661810) places it; its virtuals and member names are not recovered.
class tHandleList
{
	public:
		tHandleList( long );
		virtual		~tHandleList( void );

		long	f04;
		long	f08;
		long	f0c;
};

class tClassActiveChannelList : public tHandleList
{
	public:
		inline tClassActiveChannelList( void ) : tHandleList( 0 ) {}
		virtual		~tClassActiveChannelList( void );
};

// HYPOTHESIS: mAddHandle (signature from the SDK library symbols) sits in vtable slot 15 (the
// call at 0x00661300 goes through +0x3c); the slots before it are not recovered yet. Slot 2
// takes the handle mNewAudioFileClass gives the new class (0x00661810). The members follow
// the stores of its inlined constructor there (0x40 bytes, the list at +8 behind the vtable
// pointer padded for the double at +0x38); their names are not known.
class tAudioFileClass
{
	public:
		inline tAudioFileClass( void ) : f30( 0 )
		{
			f18 = 0;
			f24 = -1;
			f1c = 0;
			f34 = 0;
		}
		// HYPOTHESIS: not virtual; mReleaseAudioFileClassPtr calls it directly before freeing.
		~tAudioFileClass( void );

		virtual		void	vfunction0	( void );
		virtual		long	vfunction1	( void );
		virtual		void	vfunction2	( long );
		virtual		void	vfunction3	( void );
		virtual		void	vfunction4	( void );
		virtual		void	vfunction5	( void );
		virtual		void	vfunction6	( void );
		virtual		void	vfunction7	( void );
		virtual		void	vfunction8	( void );
		virtual		void	vfunction9	( void );
		virtual		void	vfunction10	( void );
		virtual		void	vfunction11	( void );
		virtual		void	vfunction12	( void );
		virtual		void	vfunction13	( void );
		virtual		void	mRemoveHandle	( unsigned long );
		virtual		void	mAddHandle	( unsigned long );

		tClassActiveChannelList	f08;
		long	f18;
		long	f1c;
		long	f20;
		long	f24;
		long	f28;
		long	f2c;
		long	f30;
		long	f34;
		double	f38;
};

// HYPOTHESIS: mGetAudioFileClassPtrFromHandle (signature from the SDK library symbols) sits in
// vtable slot 11 (the call at 0x00661300 goes through +0x2c); slots 6 to 10 are not recovered
// yet.
class tAudioPriorityManagerPart : public tPart
{
	public:
		virtual		void	vfunction6	( void );
		virtual		void	vfunction7	( void );
		virtual		void	vfunction8	( void );
		virtual		void	vfunction9	( void );
		virtual		void	vfunction10	( void );
		virtual		tAudioFileClass *	mGetAudioFileClassPtrFromHandle	( long );
		virtual		long	mGetAudioFileClassHandleFromReference	( long );
		// HYPOTHESIS: mReleaseAudioFileClassPtr in slot 13, from mRemoveClass (0x00661e90) passing
		// the handle through +0x34; the slots of mNewAudioFileClass and mRemoveClass are not known.
		virtual		unsigned char	mReleaseAudioFileClassPtr	( long );
		virtual		long	mNewAudioFileClass	( void );
		virtual		void	mRemoveClass	( long );

		GenAbstractBinding( tAudioPriorityManagerPart )

		// HYPOTHESIS: a table of file classes indexed by handle and its length, from the reads in
		// mGetAudioFileClassPtrFromHandle at 0x006617a0; names and the field at +0x10 are not known.
		tAudioFileClass	**f0c;
		long			f10;
		long			f14;
};

// HYPOTHESIS: mSet is the first virtual (signature from the SDK library symbols); the layout
// after the vtable follows the stores in mSet at 0x00661300; member names are not known.
class tClassTrackingInfo
{
	public:
		virtual		void	mSet	( unsigned long, unsigned long, long, long, long * );
		virtual		void	mSetAvailable	( void );
		virtual		long	mGetPriority	( void );
		virtual		unsigned char	mAreWeValid	( void );
		virtual		long	mFadeToStop	( void );

		unsigned long	f04;
		unsigned long	f08;
		long			f0c;
		long			f10[4];
		long			f20;
};

#endif // TAUDIOPRIORITY_H
