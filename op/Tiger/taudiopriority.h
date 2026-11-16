// taudiopriority.h: Tiger's audio priority manager part and its per-class tracking records
// (class names from the exe's RTTI and the SDK library symbols).

#ifndef TAUDIOPRIORITY_H
#define TAUDIOPRIORITY_H

#include "Tiger/tpart.h"
#include "unsorted/tSafeArray.h"

// HYPOTHESIS: the enum's name is from the SDK library symbols; its values are not recovered.
enum eAudioPreemptType
{
};

// HYPOTHESIS: a 0x10-byte list, from where the inlined tAudioFileClass constructor in
// mNewAudioFileClass (0x00661810) places it; its virtuals and member names are not recovered.
class tHandleList
{
	public:
		tHandleList( long );
		virtual		~tHandleList( void );
		// HYPOTHESIS: the slot after the destructor; the exe's vtable is not checked yet.
		virtual		void	mInit	( long );
		// HYPOTHESIS: slot order after mInit is not checked against the exe's vtable.
		virtual		unsigned long	mGetFirstHandle	( void );

		long	f04;
		long	f08;
		// HYPOTHESIS: a heap block, from the operator delete call on it in the inlined
		// destructor (0x00661f30); its element type is not known.
		long	*f0c;
};

// HYPOTHESIS: inline, from its body inlined into ~tAudioFileClass (0x00661f30).
inline tHandleList::~tHandleList( void )
{
	if ( f0c )
	{
		delete [] f0c;
		f0c = NULL;
	}
	f08 = 0;
	f04 = 0;
}

class tClassActiveChannelList : public tHandleList
{
	public:
		inline tClassActiveChannelList( void ) : tHandleList( 0 ) {}
		virtual		~tClassActiveChannelList( void );
};

// HYPOTHESIS: inline and empty, from ~tAudioFileClass (0x00661f30) storing only the base's vtable.
inline tClassActiveChannelList::~tClassActiveChannelList( void )
{
}

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
		// HYPOTHESIS: no destructor is declared; the implicit one (0x00661f30) stores no vtable
		// pointer of its own, which a written destructor does.

		virtual		void	vfunction0	( void );
		virtual		long	vfunction1	( void );
		virtual		void	vfunction2	( long );
		virtual		void	vfunction3	( void );
		// HYPOTHESIS: mSet (signature from the SDK library symbols) in slot 4, from
		// mSetAudioFileClass (0x00661f60) calling through +0x10.
		virtual		void	mSet	( long, long, eAudioPreemptType, double );
		virtual		void	vfunction5	( void );
		virtual		void	vfunction6	( void );
		virtual		void	vfunction7	( void );
		virtual		void	vfunction8	( void );
		// Slots 9 and 11 from the exe's vtable at 0x007b759c and the SDK library symbols.
		virtual		double	mGetAtmostOneCanPlayPerThisNumberOfMicroseconds	( void );
		virtual		void	vfunction10	( void );
		virtual		void	mSetEarliestNextStartTime	( double );
		virtual		void	vfunction12	( void );
		virtual		void	vfunction13	( void );
		virtual		void	mRemoveHandle	( unsigned long );
		virtual		void	mAddHandle	( unsigned long );

					unsigned char	mDoWeMeetSpaceConstraints	( long, long, double & );

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

// The slot order follows the exe's vtable at 0x0079923c (signatures from the SDK library
// symbols); slots 2 and 4 are not recovered. The layout after the vtable follows the stores in
// mSet at 0x00661300; member names are not known.
class tClassTrackingInfo
{
	public:
		virtual		void	mSet	( unsigned long, unsigned long, long, long, long * );
		virtual		long	mGetPriority	( void );
		virtual		void	vfunction2	( void );
		virtual		void	mSetAvailable	( void );
		// HYPOTHESIS: a truth value, from mAreWeAllowedToPlay (0x00661fa0) comparing it with 1.
		virtual		unsigned char	vfunction4	( void );
		virtual		long	mFadeToStop	( void );
		virtual		unsigned char	mAreWeValid	( void );

		// HYPOTHESIS: inline, from mClose (0x00662540) storing the vtable before operator delete.
		~tClassTrackingInfo( void ) {}

		unsigned long	f04;
		unsigned long	f08;
		long			f0c;
		long			f10[4];
		long			f20;
};

// The slot order follows the exe's vtable at 0x007991e0 (signatures from the SDK library
// symbols).
class tAudioPriorityManagerPart : public tPart
{
	public:
		virtual		iTruth	mIsReady	( void );
		virtual		void	mActivate	( void );
		virtual		void	mDeactivate	( void );
		virtual		unsigned char	mAreWeAllowedToPlay	( unsigned long, long, long, long, long *, unsigned long &, double & );
		virtual		void	mWeHaveStoppedPlaying	( unsigned long );
		// HYPOTHESIS: VC6 lists overloaded virtuals in reverse order of declaration; slot 8 holds
		// the overload with arguments, slot 9 the one without.
		virtual		long	mNewAudioFileClass	( void );
		virtual		long	mNewAudioFileClass	( long, long, eAudioPreemptType, double );
		virtual		void	mRemoveClass	( long );
		virtual		tAudioFileClass *	mGetAudioFileClassPtrFromHandle	( long );
		virtual		long	mGetAudioFileClassHandleFromReference	( long );
		virtual		unsigned char	mReleaseAudioFileClassPtr	( long );
		virtual		void	mSetAudioFileClass	( long, long, long, eAudioPreemptType, double );
		// HYPOTHESIS: slots 15 and 16 (0x004359a7, 0x004359bf) have no name or signature yet.
		virtual		void	vfunction15	( void );
		// HYPOTHESIS: slot 16 maps a handle to its tracking record, from
		// mIsFirstHigherPriorityThanSecond (0x00662770) calling mGetPriority on its result.
		virtual		tClassTrackingInfo *	vfunction16	( unsigned long );
		virtual		unsigned char	mIsFirstHigherPriorityThanSecond	( unsigned long, unsigned long );
		virtual		unsigned char	mIsHigherPriority	( long, long, eAudioPreemptType );

		GenAbstractBinding( tAudioPriorityManagerPart )

		// HYPOTHESIS: a table of file classes indexed by handle and its length, from the reads in
		// mGetAudioFileClassPtrFromHandle at 0x006617a0; names and the field at +0x10 are not known.
		tAudioFileClass	**f0c;
		long			f10;
		long			f14;
		// HYPOTHESIS: the tracking records in use, by handle, and the records themselves, from
		// mAreWeAllowedToPlay (0x00661fa0); the field at +0x34 is not known. The count
		// of 256 records and the cursor after them come from mGetNextAvailableClassTrackingInfo
		// (0x00662400).
		tSafeArray< tClassTrackingInfo >	f18;
		long			f34;
		tClassTrackingInfo	f38[ 256 ];
		long			f2438;

	protected:
		virtual		void	mClose	( void );

		long	mGetNextAvailableClassTrackingInfo	( void );
};

#endif // TAUDIOPRIORITY_H
