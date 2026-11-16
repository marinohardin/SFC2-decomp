// taudiopriority.h: Tiger's audio priority manager part and its per-class tracking records
// (class names from the exe's RTTI and the SDK library symbols).

#ifndef TAUDIOPRIORITY_H
#define TAUDIOPRIORITY_H

#include "Tiger/tpart.h"

// HYPOTHESIS: mAddHandle (signature from the SDK library symbols) sits in vtable slot 15 (the
// call at 0x00661300 goes through +0x3c); the slots before it are not recovered yet.
class tAudioFileClass
{
	public:
		virtual		void	vfunction0	( void );
		virtual		void	vfunction1	( void );
		virtual		void	vfunction2	( void );
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

		GenAbstractBinding( tAudioPriorityManagerPart )
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

		unsigned long	f04;
		unsigned long	f08;
		long			f0c;
		long			f10[4];
		long			f20;
};

#endif // TAUDIOPRIORITY_H
