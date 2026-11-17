//	tsndmgr.cpp: the sound file manager and its play information.

#include "Tiger/tbasic.h"
#include "Tiger/tgSound.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// HYPOTHESIS: fields as mInit stores them, names not recovered; virtuals in the order of the exe's vtable at 0x007b77c8, named from the SDK library.
class tAFPlayInfo
{
public:
	tAFPlayInfo( void );
	virtual	double	mGetMinimumDelay	( void );
	virtual	void	mSetMinimumDelay	( double delay );
	virtual	double	mGetMaximumDelay	( void );
	virtual	void	mSetMaximumDelay	( double delay );
	virtual	unsigned char	mIsThisInThisThreshold	( long value );
	virtual	unsigned char	mIsThisInThisGroup	( long value );
	virtual	unsigned char	mAreWeDolby	( void );
	virtual	unsigned char	mAreWe3D	( void );
	virtual	void	mIncreaseRandomCount	( void );
	virtual	void	mDecreaseRandomCount	( void );
	virtual	long	mGetRandomFrequency	( void );
	virtual	long	mGetRandomVolume	( void );
	virtual	long	mGetAcualIndexOffsetIfRandom	( void );
	virtual	unsigned long	mGetActualAudioFileIndex	( void );
	virtual	unsigned long	mGetActualChannelGroupIndex	( void );
	virtual	void	mSetActualAudioFileIndex	( unsigned long index );
	virtual	void	mSetActualChannelGroupIndex	( unsigned long index );
	virtual	void	mInit	( long, char *, char *, char *, long, long, long, long, double, double, unsigned long, unsigned long, unsigned char, unsigned char, long );
	virtual	iTruth	mAddClass	( long theClass );
	virtual	long	mGetNumberOfClasses	( void );
	virtual	long *	mGetClasses	( void );
	virtual	long	mGetNextInSequence	( void );
	virtual	void	mSetNextInSequence	( long next );
	tAFPlayInfo &	operator=	( tAFPlayInfo &other );
	long			field_0x8;	// HYPOTHESIS: 0x4 is the alignment gap VC6 leaves after the vtable pointer for the doubles.
	char			field_0xc[0x20];
	char			field_0x2c[0x20];
	char			field_0x4c[0x40];
	long			field_0x8c;
	long			field_0x90;
	long			field_0x94;
	long			field_0x98;
	char			fUnknown_009c[0x4];
	double			field_0xa0;
	double			field_0xa8;
	long			field_0xb0;
	unsigned long	field_0xb4;
	unsigned long	field_0xb8;
	long			field_0xbc;
	unsigned char	field_0xc0;
	unsigned char	field_0xc1;
	long			field_0xc4[4];	// HYPOTHESIS: a list of up to four classes at 0xc4 and its count at 0xd4, from mAddClass.
	long			field_0xd4;
};
// FUNCTION: STARFLEETOP 0x00663720
unsigned char	mystrequivalent( char *s1, char *s2 )
{
	int		len = strlen( s1 );
	if ( len != (int)strlen( s2 ) )
		return 0;
	for ( int i = 0; i < len; i++ )
	{
		char	c1 = s1[i];
		char	c2 = s2[i];
		if ( c1 >= 'a' )
			c1 -= 0x20;
		if ( c2 >= 'a' )
			c2 -= 0x20;
		if ( c1 != c2 )
			return 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x00663780
unsigned char	mystrequivalentN( char *s1, char *s2, long n )
{
	int		len1 = strlen( s1 );
	int		len2 = strlen( s2 );
	if ( len1 > n )
		len1 = n;
	if ( len2 > n )
		len2 = n;
	if ( len1 != len2 )
		return 0;
	for ( int i = 0; i < len1; i++ )
	{
		char	c1 = s1[i];
		char	c2 = s2[i];
		if ( c1 >= 'a' )
			c1 -= 0x20;
		if ( c2 >= 'a' )
			c2 -= 0x20;
		if ( c1 != c2 )
			return 0;
	}
	return 1;
}
// FUNCTION: STARFLEETOP 0x006637f0
long	tAFPlayInfo::mGetRandomFrequency( void )
{
	if ( field_0x8c == field_0x90 )
		return -1;
	return field_0x8c - (long)( (double)rand() * ( field_0x90 - field_0x8c ) * ( -1.0 / RAND_MAX ) );
}

// FUNCTION: STARFLEETOP 0x00663850
long	tAFPlayInfo::mGetRandomVolume( void )
{
	if ( field_0x94 == field_0x98 )
		return field_0x94;
	return field_0x94 - (long)( (double)rand() * ( field_0x98 - field_0x94 ) * ( -1.0 / RAND_MAX ) );
}

// FUNCTION: STARFLEETOP 0x006638a0
long	tAFPlayInfo::mGetAcualIndexOffsetIfRandom( void )
{
	if ( field_0xb0 < 2 )
		return 0;
	return ( rand() * field_0xb0 ) / RAND_MAX;
}

// FUNCTION: STARFLEETOP 0x006638e0
iTruth	tAFPlayInfo::mAddClass( long theClass )
{
	for ( long i = 0; i < field_0xd4; i++ )
	{
		if ( field_0xc4[i] == theClass )
			return kTrue;
	}
	if ( field_0xd4 == 4 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("We've hit the class limit!"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("We've hit the class limit!"); }
#line 254
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 137
		return kFalse;
	}
	field_0xc4[field_0xd4] = theClass;
	field_0xd4++;
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x006639c0
void	tAFPlayInfo::mSetNextInSequence( long next )
{
	field_0xbc = next;
}

// FUNCTION: STARFLEETOP 0x006639d0
void	tAFPlayInfo::mInit( long a1, char *s1, char *s2, char *s3, long a5, long a6, long a7, long a8, double d1, double d2, unsigned long u1, unsigned long u2, unsigned char b1, unsigned char b2, long next )
{
	field_0x8 = a1;
	strcpy( field_0xc, s1 );
	strcpy( field_0x2c, s2 );
	strcpy( field_0x4c, s3 );
	field_0x8c = a5;
	field_0x90 = a6;
	field_0xbc = next;
	field_0x94 = a7;
	field_0x98 = a8;
	field_0xc0 = b1;
	field_0xc1 = b2;
	field_0xd4 = 0;
	field_0xb0 = 0;
	field_0xa0 = d1;
	field_0xa8 = d2;
	field_0xb4 = u1;
	field_0xb8 = u2;
}

// FUNCTION: STARFLEETOP 0x00663ae0
tAFPlayInfo &	tAFPlayInfo::operator=( tAFPlayInfo &other )
{
	field_0x8 = other.field_0x8;
	strcpy( field_0xc, other.field_0xc );
	strcpy( field_0x2c, other.field_0x2c );
	strcpy( field_0x4c, other.field_0x4c );
	field_0x8c = other.field_0x8c;
	field_0x90 = other.field_0x90;
	field_0x94 = other.field_0x94;
	field_0x98 = other.field_0x98;
	field_0xc0 = other.field_0xc0;
	field_0xc1 = other.field_0xc1;
	field_0xd4 = other.field_0xd4;
	for ( long i = 0; i < field_0xd4; i++ )
		field_0xc4[i] = other.field_0xc4[i];
	field_0xa0 = other.field_0xa0;
	field_0xa8 = other.field_0xa8;
	field_0xb0 = other.field_0xb0;
	field_0xb4 = other.field_0xb4;
	field_0xb8 = other.field_0xb8;
	field_0xbc = other.field_0xbc;
	return *this;
}

// FUNCTION: STARFLEETOP 0x00663c40
unsigned char	tAFPlayInfo::mIsThisInThisThreshold( long value )
{
	if ( field_0x2c[0] == 0 || value == -1 )
		return 1;
	long	wanted = value;
	long	pos = 0;
	long	found;
	if ( sscanf( field_0x2c, "%ld", &found ) <= 0 || wanted == found || found == -1 )
		return 1;
	while ( field_0x2c[pos] != 0 )
	{
		while ( field_0x2c[pos] != ',' && field_0x2c[pos] != ';' && field_0x2c[pos] != 0 )
			pos++;
		if ( field_0x2c[pos] != 0 )
			pos++;
		// HYPOTHESIS: the original scans from the start of the string each time, not from pos.
		if ( sscanf( field_0x2c, "%ld", &found ) <= 0 )
			break;
		if ( wanted == found || found == -1 )
			return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00663ce0
unsigned char	tAFPlayInfo::mIsThisInThisGroup( long value )
{
	if ( field_0xc[0] == 0 )
		return 1;
	long	wanted = value;
	long	pos = 0;
	long	found;
	if ( wanted == 0 || sscanf( field_0xc, "%ld", &found ) <= 0 || wanted == found || found == 0 )
		return 1;
	do
	{
		char	c = field_0xc[pos];
		if ( c == 0 )
			return 0;
		while ( c != ',' && c != ';' && c != 0 )
			c = field_0xc[++pos];
		if ( field_0xc[pos] != 0 )
			pos++;
		if ( sscanf( &field_0xc[pos], "%ld", &found ) <= 0 )
			return 0;
	} while ( wanted != found && found != 0 );
	return 1;
}

#include "Tiger/tpart.h"
#include "Tiger/tclock.h"
#include "unsorted/tRegisteredAudioType.h"
#include "Tiger/taudiopriority.h"
#include "Tiger/tAudioFile.h"
#include "Tiger/tsndmgr.h"
// MATCH-ONLY: tBasicSoundFileManager is declared in Tiger/tsndmgr.h; the blank lines below hold
// the place of its former declaration here, so that every function after it keeps its line.
// HYPOTHESIS: tChannelGroup is declared in Tiger/taudiochannel.cpp and tChannelGroupManagerPart in
// Tiger/tgSound.cpp, neither in a header yet; both are restated here only as far as the functions
// below need them. tChannelGroup's slots follow the exe's vtable at 0x007b7454, with the SDK
// library's symbols for the entries; the declaration in Tiger/taudiochannel.cpp has another order.
// Slot 7 of the manager part takes a group index and returns the group, as the SDK library's
// mGetChannelGroup does.
class tChannelGroup;
class tChannelGroupManagerPart : public tPart { public: GenAbstractBinding(tChannelGroupManagerPart); virtual unsigned long vfunction6(); virtual tChannelGroup * mGetChannelGroup(unsigned long index); /* HYPOTHESIS: slot 8 takes the group index, by mCloseFile's call once a group has no owners */ virtual void vfunction8(unsigned long index); virtual void vfunction9(); virtual void vfunction10(); /* HYPOTHESIS: slot 11 by mDoDefaultNextAttachment's call (group, handle, file), the SDK library's mAttachFileToChannel signature */ virtual unsigned char mAttachFileToChannel(unsigned long group, unsigned long handle, long file); };
class tChannelGroup { public: long mGetAvailableChannel(unsigned long param_1); virtual void mAddOwner(); virtual void mRemoveOwner(); virtual unsigned char mAttachSoundToChannel(unsigned long param_1, tSoundSelection *param_2, long param_3); virtual unsigned char mAttachFileToChannel(unsigned long param_1, long param_2); virtual unsigned char mAttachFileToBaseGroup(long param_1); virtual unsigned char mKillRetainedMode(); virtual unsigned char mClearAttachments(); virtual unsigned char mPlayChannel(unsigned long param_1, long param_2); virtual unsigned char mPauseChannel(unsigned long param_1, long param_2); virtual unsigned char mStopAndResetChannel(unsigned long param_1, long param_2); virtual unsigned char mStopAndClearChannel(unsigned long param_1, long param_2); virtual unsigned char mSetFrequency(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetPan(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetVolume(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetPriority(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetFinishFunction(unsigned long param_1, void (* const param_2)(unsigned long, unsigned char), long param_3, long param_4, unsigned char param_5); virtual unsigned char mSetChannelAttributes(unsigned long param_1, tChannelAttributes *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundProfile(unsigned long param_1, t3DSoundProfile *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundPosition(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundOrientation(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundVelocity(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundCones(unsigned long param_1, float param_2, long param_3, long param_4, unsigned char param_5); virtual unsigned char mSet3DSoundReferenceDistance(unsigned long param_1, double param_2, long param_3, unsigned char param_4); virtual long mGetFrequency(unsigned long param_1, long param_2); virtual long mGetPan(unsigned long param_1, long param_2); virtual long mGetVolume(unsigned long param_1, long param_2); virtual tChannelAttributes * mGetChannelAttributes(unsigned long param_1, long param_2); virtual t3DSoundProfile * mGet3DSoundProfile(unsigned long param_1, long param_2); virtual tPoint3 * mGet3DSoundPosition(unsigned long param_1, long param_2); virtual tPoint3 * mGet3DSoundOrientation(unsigned long param_1, long param_2); virtual tPoint3 * mGet3DSoundVelocity(unsigned long param_1, long param_2); virtual double mGet3DSoundReferenceDistance(unsigned long param_1, long param_2); virtual float mGet3DSoundConeAngleForNoAttenuation(unsigned long param_1, long param_2); virtual long mGet3DSoundConeAttenuationLevel(unsigned long param_1, long param_2); virtual long mGetChannelPriority(unsigned long param_1, long param_2); /* HYPOTHESIS: returns eSoundChannelStatus, not declared yet */ virtual long mGetChannelStatus(unsigned long param_1, long param_2); virtual long mTotalBytesLeftToPlay(unsigned long param_1, long param_2); virtual double mTotalNumberOfSecondsLeftToPlay(unsigned long param_1, long param_2); virtual long mNumberOfAttachmentsQueuedUp(unsigned long param_1, long param_2); virtual tSoundChannelFormat * mGetSoundChannelFormat(unsigned long param_1, long param_2); virtual unsigned char mAreWeLooping(unsigned long param_1, long param_2); virtual unsigned char mAreWeStreaming(unsigned long param_1, long param_2); virtual unsigned char mAreWe3D(unsigned long param_1, long param_2); virtual unsigned char mAreWeStatic(unsigned long param_1, long param_2); virtual unsigned char mAreWeFixedFrequency(unsigned long param_1, long param_2); virtual unsigned char mAreWeARecordingChannel(unsigned long param_1, long param_2); virtual unsigned char mTryToLocateInHardware(unsigned long param_1, long param_2); virtual unsigned char mTryToLocateInSoftware(unsigned long param_1, long param_2); virtual unsigned char mAreWeFixedPan(unsigned long param_1, long param_2); virtual unsigned char mAreWeFixedVolume(unsigned long param_1, long param_2); virtual unsigned char mAreWeAGlobalSound(unsigned long param_1, long param_2); virtual unsigned char mDoWeRetainSelections(unsigned long param_1, long param_2); /* The two mDump overloads in this order, as mDump's call through slot 53 shows. */ virtual void mDump(unsigned long param_1, long param_2); virtual void mDump(); virtual unsigned char mAddUpdateSoundPositionFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5, unsigned char param_6); virtual unsigned char mAddDelayCommand(unsigned long param_1, double param_2, long param_3, unsigned char param_4); virtual unsigned char mFadeToVolume(unsigned long param_1, long param_2, double param_3, long param_4, unsigned char param_5); virtual unsigned char mFadeToStopAndResetNow(unsigned long param_1, double param_2, long param_3); virtual unsigned char mFadeToStopAndClearNow(unsigned long param_1, double param_2, long param_3); virtual unsigned char mPlaySpecialLoop(unsigned long param_1, long param_2, long param_3, long param_4, long param_5, long param_6); virtual unsigned char mEndSpecialLoop(unsigned long param_1, long param_2); virtual unsigned char mAddGetNextAttachmentFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5); char fUnknown_0004[0x10]; long field_14; char fUnknown_0018[0xc]; /* fNumberOfOwners, per the dump text in Tiger/taudiochannel.cpp */ long field_24; };
























































































































// FUNCTION: STARFLEETOP 0x00663d80
tBasicSoundFileManager::tBasicSoundFileManager( void )
{
	field_0x71c = 0;
	field_0x720 = 0;
	field_0x724 = 0;
	field_0xc = 0;
	field_0x10 = 0;
	field_0x14 = 0;
	field_0x718 = 0;
	field_0x748 = 0;
	field_0x730 = 1.0;
	field_0x2578 = 0;
	for ( int i = 0; i < 0x100; i++ )
	{
		field_0xb4c[i] = -1;
		field_0x2350[i] = 1;
	}
	field_0x234c = 0;
	field_0x740 = 2000.0;
}
// HYPOTHESIS: empty body; the unwind frame fits destroying field_0x18 and the tPart base alone.
// FUNCTION: STARFLEETOP 0x00663e60
// FUNCTION: STARFLEETOP 0x00663e80
tBasicSoundFileManager::~tBasicSoundFileManager( void )
{
}
// The SDK library's symbol for the table at 0x00882160 gives the name; contents not written.
extern	int	gVolumeModifierTable[];
// FUNCTION: STARFLEETOP 0x00663ee0
void	tBasicSoundFileManager::mAdjustGlobalVolume( void )
{
	double	volume = field_0x730;
	long	lastCount = field_0x748;
	field_0x748 = BindPart( tSoundPart )->mGetNumberCurrentlyPlaying();
	if ( field_0x748 != lastCount )
	{
		if ( field_0x748 >= 51 )
			field_0x748 = 50;
		volume = gVolumeModifierTable[field_0x748] * volume;
		BindPart( tSoundPart )->mSetGlobalVolume( (long)volume );
	}
}

// FUNCTION: STARFLEETOP 0x00664050
void	tBasicSoundFileManager::mUpdateAllSounds( void )
{
	if ( atgSoundPart->mAreWeInInterruptMode() == kFalse && field_0x2578 )
		mUpdateTest();
	if ( atgSoundPart->mAreWeInInterruptMode() == kFalse )
		mAdjustGlobalVolume();
}

// FUNCTION: STARFLEETOP 0x006640a0
void	tBasicSoundFileManager::mUpdateTest( void )
{
	if ( field_0x2578 )
	{
		if ( field_0x2550 < field_0x2560 )
		{
			if ( BindPart( tBasicSoundFileManager )->mIsSoundPlaying( field_0x2554 ) == 0 && aClockPart->mGetMicroseconds() > field_0x2570 )
			{
				// HYPOTHESIS: the original said BindPart( tBasicSoundFileManager )->mGetStatus( field_0x2554 ):
				// the exe expands mGetStatus here and still emits it at 0x00668230, as automatic inlining
				// does. Marked inline, this compiles exact but mGetStatus is no longer emitted, so the
				// expansion is written out by hand until the unit's inlining is settled.
				tBasicSoundFileManager *	manager = BindPart( tBasicSoundFileManager );
				unsigned long	handle = field_0x2554;
				eSoundChannelStatus	status = (eSoundChannelStatus)-1;
				if ( manager->mValidTrackingNumber( handle ) )
				{
					unsigned long	group = manager->field_0x10[manager->field_0x74c[manager->mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
					tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
					if ( channelGroup )
						status = (eSoundChannelStatus)channelGroup->mGetChannelStatus( handle, -1 );
				}
				if ( status == 0x10 )
					BindPart( tBasicSoundFileManager )->mStop( field_0x2554 );
				field_0x2550++;
				if ( field_0x2550 >= field_0x2560 )
				{
					field_0x2578 = 0;
					return;
				}
				field_0x2554 = BindPart( tBasicSoundFileManager )->mPlay( field_0x2550, -1, 0.0, -99999999, -1, -1, -1, NULL, NULL );
				if ( field_0x2554 == 0 )
				{
					if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Could not play sound #%ld", field_0x2550); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Could not play sound #%ld", field_0x2550); }
#line 805
					if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 475
					field_0x2554 = BindPart( tBasicSoundFileManager )->mPlay( field_0x2550, -1, 0.0, -99999999, -1, -1, -1, NULL, NULL );
					return;
				}
				field_0x2570 = aClockPart->mGetMicroseconds() + 100000.0;
				field_0x2568 = aClockPart->mGetMicroseconds() + field_0x2558 * 1000.0;
				if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Playing sound #%ld", field_0x2550); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Playing sound #%ld", field_0x2550); }
#line 812
				if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 484
				return;
			}
			if ( aClockPart->mGetMicroseconds() > field_0x2568 )
			{
				BindPart( tBasicSoundFileManager )->mFadeToStopAndResetNow( field_0x2554, 0.0, -1 );
				if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Stopping Sound #%ld", field_0x2550); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Stopping Sound #%ld", field_0x2550); }
#line 818
				if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 493
			}
		}
		else
			field_0x2578 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00664870
unsigned long	tBasicSoundFileManager::mGetTrackingIndex( unsigned long handle )
{
	return handle & 0xffff;
}

// FUNCTION: STARFLEETOP 0x00664880
unsigned short	tBasicSoundFileManager::mGetTrackingOffset( unsigned long handle )
{
	return (unsigned short)( handle >> 16 );
}

// FUNCTION: STARFLEETOP 0x00664890
unsigned char	tBasicSoundFileManager::mValidTrackingNumber( unsigned long handle )
{
	unsigned long	index = mGetTrackingIndex( handle );
	if ( (short)mGetTrackingOffset( handle ) == field_0x2350[index] )
		return 1;
	return 0;
}

// FUNCTION: STARFLEETOP 0x006648c0
void	tBasicSoundFileManager::mClearTrackingNumber( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) == 1 )
	{
		unsigned long	index = mGetTrackingIndex( handle );
		field_0xb4c[index] = -1;
		field_0x2350[index]++;
		if ( field_0x2350[index] == 0 )
			field_0x2350[index] = 1;
	}
}

// FUNCTION: STARFLEETOP 0x00664910
unsigned long	tBasicSoundFileManager::mNewTrackingNumber( long info )
{
	if ( field_0x234c >= 0x100 )
		field_0x234c = 0;
	unsigned long	start = field_0x234c;
	while ( 1 )
	{
		if ( field_0xb4c[field_0x234c] == -1 )
		{
			field_0xb4c[field_0x234c] = info;
			field_0x2350[field_0x234c]++;
			if ( field_0x2350[field_0x234c] == 0 )
				field_0x2350[field_0x234c] = 1;
			return ( (unsigned short)field_0x2350[field_0x234c] << 16 ) + field_0x234c++;
		}
		field_0x234c++;
		if ( field_0x234c >= 0x100 )
			field_0x234c = 0;
		if ( field_0x234c == start )
			break;
	}
	if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("we've run out of tracking numbers!"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("we've run out of tracking numbers!"); }




























































































































































































































































































































































#line 906
	if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
	return 0;
}









// FUNCTION: STARFLEETOP 0x00664a50
unsigned char	tBasicSoundFileManager::mAddRegisteredAudioType( char * name, unsigned long (*func1)( char *, long, long, long, tSoundChannelFormat *, long, void * ), unsigned long (*func2)( unsigned long, long ), long unused, long value, void * data, long extra )
{
	if ( field_0x718 < 0x40 )
	{
		tRegisteredAudioType *	type = &field_0x18[field_0x718];
		type->field_0x0 = new char[strlen( name ) + 1];
		strcpy( type->field_0x0, name );
		type->field_0x4 = func1;
		type->field_0xc = value;
		type->field_0x10 = data;
		type->field_0x14 = func2;
		type->field_0x8 = 1;
		type->field_0x18 = extra;
		field_0x718++;
		return 1;
	}
	return 0;
}


// FUNCTION: STARFLEETOP 0x00664b00
void	tBasicSoundFileManager::mStop( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			channelGroup->mStopAndResetChannel( handle, -1 );
	}
}

// FUNCTION: STARFLEETOP 0x00664bd0
unsigned char	tBasicSoundFileManager::mAttachSoundToChannel( unsigned long handle, tSoundSelection * selection )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAttachSoundToChannel( handle, selection, -1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00664cb0
unsigned char	tBasicSoundFileManager::mAttachFileToChannel( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAttachFileToChannel( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00664d90
unsigned char	tBasicSoundFileManager::mResumeChannel( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mPlayChannel( handle, -1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00664e70
unsigned char	tBasicSoundFileManager::mPauseChannel( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mPauseChannel( handle, -1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00664f50
unsigned char	tBasicSoundFileManager::mStopAndResetChannel( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mStopAndResetChannel( handle, -1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665030
unsigned char	tBasicSoundFileManager::mStopAndClearChannel( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mStopAndClearChannel( handle, -1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665110
unsigned char	tBasicSoundFileManager::mSetFrequency( unsigned long handle, long frequency )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSetFrequency( handle, frequency, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006651f0
unsigned char	tBasicSoundFileManager::mSetPan( unsigned long handle, long pan )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSetPan( handle, pan, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006652d0
unsigned char	tBasicSoundFileManager::mSetVolume( unsigned long handle, long volume )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSetVolume( handle, volume, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006653b0
unsigned char	tBasicSoundFileManager::mSetFinishFunction( unsigned long handle, void (* const function)( unsigned long, unsigned char ), long data )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSetFinishFunction( handle, function, data, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006654a0
unsigned char	tBasicSoundFileManager::mAddUpdateSoundPositionFunction( unsigned long handle, void (* function)( unsigned long, long ), long data, double interval, long channel, unsigned char immediate )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAddUpdateSoundPositionFunction( handle, function, data, interval, channel, immediate );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006655a0
unsigned char	tBasicSoundFileManager::mAddDelayCommand( unsigned long handle, double delay, long channel, unsigned char immediate )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAddDelayCommand( handle, delay, channel, immediate );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665690
unsigned char	tBasicSoundFileManager::mPlaySpecialLoop( unsigned long handle, long param_2, long param_3, long param_4, long param_5, long param_6 )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mPlaySpecialLoop( handle, param_2, param_3, param_4, param_5, param_6 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665780
unsigned char	tBasicSoundFileManager::mEndSpecialLoop( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mEndSpecialLoop( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665860
unsigned char	tBasicSoundFileManager::mAddGetNextAttachmentFunction( unsigned long handle, void (* function)( unsigned long, long ), long data, double interval, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAddGetNextAttachmentFunction( handle, function, data, interval, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665950
unsigned char	tBasicSoundFileManager::mSetChannelAttributes( unsigned long handle, tChannelAttributes * attributes )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSetChannelAttributes( handle, attributes, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665a30
unsigned char	tBasicSoundFileManager::mSet3DSoundProfile( unsigned long handle, t3DSoundProfile * profile )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSet3DSoundProfile( handle, profile, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665b10
unsigned char	tBasicSoundFileManager::mSet3DSoundPosition( unsigned long handle, tPoint3 * position )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSet3DSoundPosition( handle, position, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665bf0
unsigned char	tBasicSoundFileManager::mSet3DSoundOrientation( unsigned long handle, tPoint3 * orientation, long param_3, unsigned char param_4 )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSet3DSoundOrientation( handle, orientation, param_3, param_4 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665ce0
unsigned char	tBasicSoundFileManager::mSet3DSoundVelocity( unsigned long handle, tPoint3 * velocity, long param_3, unsigned char param_4 )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSet3DSoundVelocity( handle, velocity, param_3, param_4 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665dd0
unsigned char	tBasicSoundFileManager::mSet3DSoundCones( unsigned long handle, float angle, long level, long param_4, unsigned char param_5 )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSet3DSoundCones( handle, angle, level, param_4, param_5 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665ec0
unsigned char	tBasicSoundFileManager::mSet3DSoundReferenceDistance( unsigned long handle, double distance, long param_3, unsigned char param_4 )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSet3DSoundReferenceDistance( handle, distance, param_3, param_4 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00665fb0
tChannelAttributes *	tBasicSoundFileManager::mGetChannelAttributes( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGetChannelAttributes( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666090
t3DSoundProfile *	tBasicSoundFileManager::mGet3DSoundProfile( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundProfile( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666170
tPoint3 *	tBasicSoundFileManager::mGet3DSoundPosition( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundPosition( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666250
tPoint3 *	tBasicSoundFileManager::mGet3DSoundOrientation( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundOrientation( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666330
tPoint3 *	tBasicSoundFileManager::mGet3DSoundVelocity( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundVelocity( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666410
double	tBasicSoundFileManager::mGet3DSoundReferenceDistance( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundReferenceDistance( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006664f0
float	tBasicSoundFileManager::mGet3DSoundConeAngleForNoAttenuation( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundConeAngleForNoAttenuation( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006665d0
long	tBasicSoundFileManager::mGet3DSoundConeAttenuationLevel( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGet3DSoundConeAttenuationLevel( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006666b0
long	tBasicSoundFileManager::mTotalBytesLeftToPlay( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mTotalBytesLeftToPlay( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666790
double	tBasicSoundFileManager::mTotalNumberOfSecondsLeftToPlay( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mTotalNumberOfSecondsLeftToPlay( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666870
long	tBasicSoundFileManager::mNumberOfAttachmentsQueuedUp( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mNumberOfAttachmentsQueuedUp( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666950
tSoundChannelFormat *	tBasicSoundFileManager::mGetSoundChannelFormat( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGetSoundChannelFormat( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666a30
unsigned char	tBasicSoundFileManager::mAreWeLooping( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeLooping( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666b10
unsigned char	tBasicSoundFileManager::mAreWeStreaming( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeStreaming( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666bf0
unsigned char	tBasicSoundFileManager::mAreWe3D( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWe3D( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666cd0
unsigned char	tBasicSoundFileManager::mAreWeStatic( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeStatic( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666db0
unsigned char	tBasicSoundFileManager::mAreWeFixedFrequency( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeFixedFrequency( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666e90
unsigned char	tBasicSoundFileManager::mAreWeARecordingChannel( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeARecordingChannel( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00666f70
unsigned char	tBasicSoundFileManager::mTryToLocateInHardware( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mTryToLocateInHardware( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00667050
unsigned char	tBasicSoundFileManager::mTryToLocateInSoftware( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mTryToLocateInSoftware( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00667130
unsigned char	tBasicSoundFileManager::mAreWeFixedPan( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeFixedPan( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00667210
unsigned char	tBasicSoundFileManager::mAreWeFixedVolume( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeFixedVolume( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006672f0
unsigned char	tBasicSoundFileManager::mAreWeAGlobalSound( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mAreWeAGlobalSound( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006673d0
unsigned char	tBasicSoundFileManager::mDoWeRetainSelections( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mDoWeRetainSelections( handle, channel );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006674b0
void	tBasicSoundFileManager::mDump( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			channelGroup->mDump( handle, channel );
	}
}

// FUNCTION: STARFLEETOP 0x00667590
unsigned char	tBasicSoundFileManager::mSetPriority( unsigned long handle, long priority )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mSetPriority( handle, priority, -1, 1 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00667670
long	tBasicSoundFileManager::mGetChannelPriority( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGetChannelPriority( handle, channel );
	}
	return -99999999;
}

// FUNCTION: STARFLEETOP 0x00667750
long	tBasicSoundFileManager::mGetFrequency( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGetFrequency( handle, channel );
	}
	return -99999999;
}

// FUNCTION: STARFLEETOP 0x00667830
long	tBasicSoundFileManager::mGetPan( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGetPan( handle, channel );
	}
	return -99999999;
}

// FUNCTION: STARFLEETOP 0x00667910
long	tBasicSoundFileManager::mGetVolume( unsigned long handle, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mGetVolume( handle, channel );
	}
	return -99999999;
}

// FUNCTION: STARFLEETOP 0x006679f0
unsigned char	tBasicSoundFileManager::mFadeToVolume( unsigned long handle, long volume, double timeToTake, long channel, unsigned char immediate )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mFadeToVolume( handle, volume, timeToTake, channel, immediate );
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00667ae0
unsigned char	tBasicSoundFileManager::mFadeToStopAndResetNow( unsigned long handle, double timeToTake, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mFadeToStopAndResetNow( handle, timeToTake, channel );
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00667bd0
unsigned char	tBasicSoundFileManager::mFadeToStopAndClearNow( unsigned long handle, double timeToTake, long channel )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return channelGroup->mFadeToStopAndClearNow( handle, timeToTake, channel );
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00667cc0
void	tBasicSoundFileManager::mUpdatePosition( unsigned long handle )
{
	long	index = mGetTrackingIndex( handle );
	if ( index >= 0 && field_0x174c[index] )
		field_0x174c[index]( field_0xf4c[index], handle );
}

// FUNCTION: STARFLEETOP 0x00667cf0
void	tBasicSoundFileManager::mFinishSound( unsigned long handle, unsigned char how )
{
	long	index = mGetTrackingIndex( handle );
	if ( index >= 0 )
	{
		BindPart( tAudioPriorityManagerPart )->mWeHaveStoppedPlaying( field_0x134c[index] );
		if ( field_0x1f4c[index] )
			field_0x1f4c[index]( field_0xf4c[index], handle, how );
		mClearTrackingNumber( handle );
	}
}

// FUNCTION: STARFLEETOP 0x00667dd0
void	tBasicSoundFileManager::mGetNextAttachment( unsigned long handle )
{
	long	index = mGetTrackingIndex( handle );
	if ( index >= 0 && field_0x1b4c[index] )
		field_0x1b4c[index]( handle, field_0xf4c[index] );
}

// FUNCTION: STARFLEETOP 0x00667e00
void	tBasicSoundFileManager::mSetGetNextAttachmentFunction( unsigned long handle, void (* function)( unsigned long, long ) )
{
	field_0x1b4c[mGetTrackingIndex( handle )] = function;
}

// FUNCTION: STARFLEETOP 0x00667e20
void	tBasicSoundFileManager::mDoDefaultNextAttachment( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		long	index = mGetTrackingIndex( handle );
		if ( index >= 0 )
		{
			long	info = field_0x74c[index];
			long	next = field_0x10[info].mGetNextInSequence();
			unsigned long	group = field_0x10[info].mGetActualChannelGroupIndex();
			if ( next == 0 )
			{
				tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
				channelGroup->mAddDelayCommand( handle, atgSoundPart->vfunction125() + 2000000.0, -1, 1 );
				channelGroup->mStopAndResetChannel( handle, -1 );
				return;
			}
			info += field_0x10[info].mGetNextInSequence();
			long	file = field_0x10[info].mGetActualAudioFileIndex();
			if ( file == -1 )
			{
				tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
				channelGroup->mAddDelayCommand( handle, atgSoundPart->vfunction125() + 2000000.0, -1, 1 );
				channelGroup->mStopAndResetChannel( handle, -1 );
				return;
			}
			if ( group == -1 )
			{
				tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
				channelGroup->mAddDelayCommand( handle, atgSoundPart->vfunction125() + 2000000.0, -1, 1 );
				channelGroup->mStopAndResetChannel( handle, -1 );
				return;
			}
			field_0x74c[index] = info;
			BindPart( tChannelGroupManagerPart )->mAttachFileToChannel( group, handle, file );
		}
	}
}

// FUNCTION: STARFLEETOP 0x00668150
unsigned char	tBasicSoundFileManager::mIsSoundPlaying( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			if ( channelGroup->mGetChannelStatus( handle, -1 ) == 0x20 )
				return 1;
			else
				return 0;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00668230
eSoundChannelStatus	tBasicSoundFileManager::mGetStatus( unsigned long handle )
{
	if ( mValidTrackingNumber( handle ) )
	{
		unsigned long	group = field_0x10[field_0x74c[mGetTrackingIndex( handle )]].mGetActualChannelGroupIndex();
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
		if ( channelGroup )
			return (eSoundChannelStatus)channelGroup->mGetChannelStatus( handle, -1 );
	}
	return (eSoundChannelStatus)-1;
}

// FUNCTION: STARFLEETOP 0x00668310
long	tBasicSoundFileManager::mGetRegisteredNumber( char *name )
{
	if ( name )
	{
		long	start = 0;
		while ( name[start] == ' ' || name[start] == '"' )
			start++;
		long	end = strlen( name ) - 1;
		while ( name[end] == ' ' || name[end] == '"' )
		{
			if ( end < 0 )
				return -1;
			end--;
		}
		if ( end >= 0 )
		{
			name[end + 1] = 0;
			for ( long i = 0; i < field_0x718; i++ )
				if ( mystrequivalent( &name[start], field_0x18[i].field_0x0 ) == 1 )
					return i;
		}
	}
	return -1;
}

// HYPOTHESIS: the constructor the original inlined into the array new below; its defaults from the stores.
inline	tAFPlayInfo::tAFPlayInfo( void )
{
	mInit( -1, "", "", "", 22050, 22050, 100, 100, 0.0, 0.0, -1, -1, 0, 0, 0 );
}

// FUNCTION: STARFLEETOP 0x006683a0
unsigned char	tBasicSoundFileManager::mSetAFPlayInfoAtThisPosition( long position, long u1, long u2, long a1, char *s1, char *s2, char *s3, long a5, long a6, long a7, long a8, double d1, double d2, unsigned char b1, unsigned char b2, long next )
{
	if ( position >= field_0x14 )
	{
		if ( field_0x14 == 0 )
		{
			long	size = 0x800;
			if ( position > 0x800 )
				size = ( ( (unsigned long)( position - 1 ) / 0x800 ) + 1 ) * 0x800;
			field_0x10 = new tAFPlayInfo[size];
			if ( !field_0x10 )
				return 0;
			field_0x14 = size;
		}
		else
		{
			long	size = field_0x14 + 0x800;
			if ( size < position )
				size += (unsigned long)( position - size + 0x7ff ) / 0x800 * 0x800;
			tAFPlayInfo *	list = new tAFPlayInfo[size];
			if ( !list )
				return 0;
			for ( long i = 0; i < field_0x14; i++ )
				list[i] = field_0x10[i];
			field_0x14 = size;
			delete [] field_0x10;
			field_0x10 = list;
		}
	}
	field_0x10[position].mInit( a1, s1, s2, s3, a5, a6, a7, a8, d1, d2, u1, u2, b1, b2, next );
	return 1;
}

// FUNCTION: STARFLEETOP 0x006685b0
void	tBasicSoundFileManager::mCloseAllSoundsInGroup( long group )
{
	for ( long i = 0; i < field_0x14; i++ )
	{
		if ( field_0x10[i].mIsThisInThisGroup( group ) == 1 )
		{
			mCloseFile( i );
			field_0x10[i].mInit( -1, "", "", "", 22050, 22050, 100, 100, 0.0, 0.0, -1, -1, 0, 0, 0 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x00668640
void	tBasicSoundFileManager::mCloseAllSoundsNotInThreshold( long threshold )
{
	for ( long i = 0; i < field_0x14; i++ )
	{
		if ( field_0x10[i].mIsThisInThisThreshold( threshold ) == 0 )
		{
			mCloseFile( i );
			field_0x10[i].mInit( -1, "", "", "", 22050, 22050, 100, 100, 0.0, 0.0, -1, -1, 0, 0, 0 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x006686d0
void	tBasicSoundFileManager::mCloseAllSounds( void )
{
	for ( long i = 0; i < field_0x14; i++ )
	{
		mCloseFile( i );
		field_0x10[i].mInit( -1, "", "", "", 22050, 22050, 100, 100, 0.0, 0.0, -1, -1, 0, 0, 0 );
	}
}

// FUNCTION: STARFLEETOP 0x00668740
void	tBasicSoundFileManager::mCloseFile( long index )
{
	if ( field_0x10[index].mGetActualChannelGroupIndex() != -1 )
	{
		tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( field_0x10[index].mGetActualChannelGroupIndex() );
		if ( channelGroup )
		{
			channelGroup->mRemoveOwner();
			if ( channelGroup->field_24 == 0 )
				BindPart( tChannelGroupManagerPart )->vfunction8( field_0x10[index].mGetActualChannelGroupIndex() );
		}
		BindPart( tAudioFileInfoPart )->mDeleteSoundFile( field_0x10[index].mGetActualAudioFileIndex() );
		field_0x10[index].mInit( -1, "", "", "", 22050, 22050, 100, 100, 0.0, 0.0, -1, -1, 0, 0, 0 );
	}
}

// FUNCTION: STARFLEETOP 0x00668970
void	tBasicSoundFileManager::mParseAllSoundsFile( char *fileName, long a2, long a3 )
{
	// HYPOTHESIS: only the opening is recovered: remember the file name, read the file, then stop before the token loop.
	long	size;
	if ( fileName == 0 )
	{
		if ( field_0xc == 0 )
			return;
	}
	else
	{
		if ( field_0xc != 0 )
		{
			free( (void *)field_0xc );
			field_0xc = 0;
		}
		char *	copy = (char *)malloc( strlen( fileName ) + 1 );
		field_0xc = (long)copy;
		if ( copy == 0 )
			return;
		strcpy( copy, fileName );
	}
	signed char *	text = mReadInFileAsString( (char *)field_0xc, 1, &size );
	if ( text == 0 || size == 0 )
		return;
}

// FUNCTION: STARFLEETOP 0x0066ac50
signed char *	tBasicSoundFileManager::mGetNextToken( char *token, const char *text, long tokenSize, long *remaining, char delimiter )
{
	const char *	end = text;
	if ( delimiter == '\t' )
	{
		while ( *end != '\0' && *end != '\t' && *end != '\n' && *end != '\r' )
			end++;
	}
	else
	{
		while ( *end != '\0' && *end != '\n' && *end != '\r' )
			end++;
	}
	long	length;
	if ( *end == '\0' )
	{
		length = *remaining;
		*remaining = 0;
	}
	else
	{
		length = end - text;
		*remaining -= length + 1;
		if ( length == 0 && ( *end == '\n' || *end == '\r' ) )
			length = 1;
	}
	if ( length > tokenSize - 1 )
		length = tokenSize - 1;
	memcpy( token, text, length );
	token[length] = '\0';
	if ( *remaining == 0 )
		return 0;
	end++;
	if ( token[0] == '\n' || token[0] == '\r' )
	{
		while ( *end != '\0' && ( *end == '\n' || *end == '\r' ) )
		{
			( *remaining )--;
			end++;
		}
	}
	return (signed char *)end;
}

#include "Tiger/tsound_fileio.h"
// The memory part, declared in full only in Source/entity/entity_physical.cpp, is restated
// for the Tiger units in unsorted/tMemoryPart.h. Slot 7 (offset 0x1c) allocates (size, line,
// file).
#include "unsorted/tMemoryPart.h"

// FUNCTION: STARFLEETOP 0x0066ad40
signed char *	tBasicSoundFileManager::mReadInFileAsString( char *fileName, long a2, long *size )
{
	if ( fileName == 0 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("trouble opening file"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("trouble opening file"); }
#line 3383
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
		*size = 0;
		return 0;
	}
	tBasicFile *	file = _NewSoundFileIO();
	if ( file->mOpenForRead( fileName ) == kFalse )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("trouble opening file"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("trouble opening file"); }
#line 3393
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
		delete file;
		*size = 0;
		return 0;
	}
	long	fileSize = file->mFileSize();
	if ( fileSize > 0 )
	{
#line 3408
		signed char *	buffer = (signed char *)BindPart( tMemoryPart )->mAllocate( fileSize + 1, __LINE__, __FILE__ );
		if ( buffer == 0 )
		{
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("we could not get any memory"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("we could not get any memory"); }
#line 3411
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
			file->mClose();
			delete file;
			*size = 0;
		}
		else
		{
			*size = file->mRead( buffer, fileSize );
			buffer[*size] = '\0';
			( *size )++;
			file->mClose();
			delete file;
		}
		return buffer;
	}
	if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("the file had no size?"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("the file had no size?"); }
#line 3422
	if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
	file->mClose();
	delete file;
	*size = 0;
	return 0;
}
// MATCH-ONLY: the original line number is not known; this returns to the file's own numbering
#line 2024
// The SDK library's symbol names this tBasicSoundFileManager::mIsReady; the type descriptors its
// three Test_ copies cast to (0x0083f420, 0x00881fa8, 0x00881d38) give the parts it waits for.
// FUNCTION: STARFLEETOP 0x0066b0f0
iTruth	tBasicSoundFileManager::mIsReady( void )
{
	if ( TestPart( tAudioPriorityManagerPart ) == kTrue && TestPart( tChannelGroupManagerPart ) == kTrue && TestPart( tAudioFileInfoPart ) == kTrue )
		return kTrue;
	return kFalse;
}
// FUNCTION: STARFLEETOP 0x0066b110
void	tBasicSoundFileManager::mDeactivate( void )
{
	mCloseAllSounds();
	if ( field_0xc != 0 )
	{
		free( (void *)field_0xc );
		field_0xc = 0;
	}
	if ( field_0x10 != 0 )
	{
		free( field_0x10 );
		field_0x10 = 0;
	}
}
// FUNCTION: STARFLEETOP 0x0066b150
void	DefaultSndMgrUpdatePositionFunction( unsigned long handle, long data )
{
	BindPart( tBasicSoundFileManager )->mUpdatePosition( handle );
}
// FUNCTION: STARFLEETOP 0x0066b1e0
void	DefaultSndMgrFinishFunction( unsigned long handle, unsigned char how )
{
	BindPart( tBasicSoundFileManager )->mFinishSound( handle, how );
}
// FUNCTION: STARFLEETOP 0x0066b270
void	DefaultNextAttachmentFunction( unsigned long handle, long data )
{
	BindPart( tBasicSoundFileManager )->mDoDefaultNextAttachment( handle );
}
// FUNCTION: STARFLEETOP 0x0066b300
void	tBasicSoundFileManager::mSetNextInSequence( long index, long next )
{
	field_0x10[index].mSetNextInSequence( next - index );
}
// FUNCTION: STARFLEETOP 0x0066b330
unsigned long	tBasicSoundFileManager::mPlay( long index, long data, double delay, long priority, long volume, long frequency, long pan, void (*finish)( long, unsigned long, unsigned char ), void (*nextAttachment)( unsigned long, long ) )
{
	if ( index == -1 )
		return 0;
	if ( index < 0 || index >= field_0x14 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("What'cha trying to play???"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("What'cha trying to play???"); }
#line 3561
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
		return 0;
	}
	if ( priority == -99999999 )
		priority = 100;
	long	trueNum = index + field_0x10[index].mGetAcualIndexOffsetIfRandom();
	unsigned long	file = field_0x10[trueNum].mGetActualAudioFileIndex();
	if ( file == -1 )
		return 0;
	unsigned long	group = field_0x10[trueNum].mGetActualChannelGroupIndex();
	if ( group == -1 )
		return 0;
	unsigned long	handle = mNewTrackingNumber( group );
	if ( handle == 0 )
		return 0;
	unsigned long	slot = mGetTrackingIndex( handle );
	tChannelGroup *	channelGroup = BindPart( tChannelGroupManagerPart )->mGetChannelGroup( group );
	field_0xf4c[slot] = data;
	field_0x74c[slot] = trueNum;
	double	wait = 0.0;
	if ( field_0x10[trueNum].mGetNumberOfClasses() == 0 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Playing a sound with no classes -- afHandle=%ld,trueNum=%ld!", trueNum, slot); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Playing a sound with no classes -- afHandle=%ld,trueNum=%ld!", trueNum, slot); }
#line 3603
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
	}
	else
	{
		long	channels = channelGroup->field_14;
		if ( !BindPart( tAudioPriorityManagerPart )->mAreWeAllowedToPlay( handle, channels, priority, field_0x10[trueNum].mGetNumberOfClasses(), field_0x10[trueNum].mGetClasses(), field_0x134c[slot], wait ) )
		{
			mClearTrackingNumber( handle );
			return 0;
		}
	}
	if ( wait < delay )
		wait = delay;
	if ( channelGroup->mGetAvailableChannel( handle ) == -1 )
	{
		channelGroup->mGetAvailableChannel( handle );
		BindPart( tAudioPriorityManagerPart )->mWeHaveStoppedPlaying( field_0x134c[slot] );
		mClearTrackingNumber( handle );
		return 0;
	}
	if ( channelGroup->mDoWeRetainSelections( handle, -1 ) == 1 )
	{
		if ( channelGroup->mNumberOfAttachmentsQueuedUp( handle, -1 ) == 0 )
			channelGroup->mAttachFileToBaseGroup( file );
	}
	else
		BindPart( tChannelGroupManagerPart )->mAttachFileToChannel( group, handle, file );
	if ( !channelGroup->mAreWeFixedFrequency( handle, -1 ) )
	{
		if ( frequency == -1 )
			frequency = field_0x10[trueNum].mGetRandomFrequency();
		channelGroup->mSetFrequency( handle, frequency, -1, 1 );
	}
	if ( volume == -1 )
		volume = field_0x10[trueNum].mGetRandomVolume();
	channelGroup->mSetVolume( handle, volume, -1, 1 );
	if ( !channelGroup->mAreWeFixedPan( handle, -1 ) )
	{
		if ( pan == -1 )
			pan = 0;
		channelGroup->mSetPan( handle, pan, -1, 1 );
	}
	channelGroup->mSetPriority( handle, priority, -1, 1 );
	if ( field_0x10[trueNum].mAreWe3D() == 1 && field_0x720 )
	{
		field_0x174c[slot] = field_0x720;
		channelGroup->mAddUpdateSoundPositionFunction( handle, DefaultSndMgrUpdatePositionFunction, handle, field_0x738, 0, 1 );
	}
	else if ( field_0x10[trueNum].mAreWeDolby() == 1 && field_0x71c )
	{
		field_0x174c[slot] = field_0x71c;
		channelGroup->mAddUpdateSoundPositionFunction( handle, DefaultSndMgrUpdatePositionFunction, handle, field_0x738, 0, 1 );
	}
	else
	{
		field_0x174c[slot] = field_0x724;
		if ( field_0x724 )
			channelGroup->mAddUpdateSoundPositionFunction( handle, DefaultSndMgrUpdatePositionFunction, handle, field_0x738, 0, 1 );
	}
	field_0x1f4c[slot] = finish;
	channelGroup->mSetFinishFunction( handle, DefaultSndMgrFinishFunction, handle, 0, 1 );
	if ( field_0x10[trueNum].mGetNextInSequence() )
		field_0x1b4c[slot] = DefaultNextAttachmentFunction;
	else
		field_0x1b4c[slot] = nextAttachment;
	if ( field_0x1b4c[slot] )
		channelGroup->mAddGetNextAttachmentFunction( handle, field_0x1b4c[slot], handle, field_0x740, 0 );
	if ( wait > 0.0 )
		channelGroup->mAddDelayCommand( handle, atgSoundPart->vfunction125() + wait * 1000.0, -1, 1 );
	channelGroup->mPlayChannel( handle, -1 );
	return handle;
}
