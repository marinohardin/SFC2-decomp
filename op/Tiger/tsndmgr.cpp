//	tsndmgr.cpp: the sound file manager and its play information.

#include "Tiger/tbasic.h"
#include "Tiger/tgSound.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HYPOTHESIS: fields as mInit stores them; field names and the virtuals' slot order are not recovered.
class tAFPlayInfo
{
public:
	virtual	long	mGetRandomFrequency	( void );
	virtual	long	mGetRandomVolume	( void );
	virtual	long	mGetAcualIndexOffsetIfRandom	( void );
	virtual	void	mSetNextInSequence	( long next );
	virtual	iTruth	mAddClass	( long theClass );
	virtual	void	mInit	( long, char *, char *, char *, long, long, long, long, double, double, unsigned long, unsigned long, unsigned char, unsigned char, long );
	virtual	unsigned char	mIsThisInThisThreshold	( long value );
	virtual	unsigned char	mIsThisInThisGroup	( long value );
	// HYPOTHESIS: slots 8 to 14 by mUpdateTest's call at +0x38; the SDK library names them, the
	// order is not recovered.
	virtual	double	mGetMinimumDelay	( void );
	virtual	void	mSetMinimumDelay	( double delay );
	virtual	double	mGetMaximumDelay	( void );
	virtual	void	mSetMaximumDelay	( double delay );
	virtual	unsigned char	mAreWeDolby	( void );
	virtual	unsigned char	mAreWe3D	( void );
	virtual	unsigned long	mGetActualChannelGroupIndex	( void );

	tAFPlayInfo &	operator=	( tAFPlayInfo &other );

	// HYPOTHESIS: 0x4 is the alignment gap VC6 leaves after the vtable pointer for the doubles.
	long			field_0x8;
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
	char			fUnknown_00c2[0x2];
	// HYPOTHESIS: a list of up to four classes at 0xc4 and its count at 0xd4, from mAddClass.
	long			field_0xc4[4];
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

// HYPOTHESIS: declared only as far as the functions below need.
class tBasicSoundFileManager : public tPart
{
public:
	tBasicSoundFileManager( void );
	virtual	~tBasicSoundFileManager( void );
	GenAbstractBinding( tBasicSoundFileManager );
	// Slots 6 to 82 in the order of the exe's vtable at 0x007b7664; the types come from the
	// SDK library's symbols.
	virtual	unsigned long	mGetTrackingIndex	( unsigned long handle );
	virtual	unsigned short	mGetTrackingOffset	( unsigned long handle );
	virtual	unsigned char	mValidTrackingNumber	( unsigned long handle );
	virtual	void	mClearTrackingNumber	( unsigned long handle );
	virtual	unsigned long	mNewTrackingNumber	( long );
	virtual	long	mGetRegisteredNumber	( char * );
	// HYPOTHESIS: the sixth parameter reads as void * from the symbol's back reference.
	virtual	unsigned char	mAddRegisteredAudioType	( char *, unsigned long (*func1)( char *, long, long, long, tSoundChannelFormat *, long, void * ), unsigned long (*func2)( unsigned long, long ), long, long, void *, long );
	virtual	unsigned char	mSetAFPlayInfoAtThisPosition	( long, long, long, long, char *, char *, char *, long, long, long, long, double, double, unsigned char, unsigned char, long );
	virtual	void	mParseAllSoundsFile	( char *, long, long );
	virtual	signed char *	mGetNextToken	( char *, const char *, long, long *, char );
	virtual	signed char *	mReadInFileAsString	( char *, long, long * );
	virtual	unsigned long	mPlay	( long, long, double, long, long, long, long, void (*)( long, unsigned long, unsigned char ), void (*)( unsigned long, long ) );
	virtual	void	mSetNextInSequence	( long, long );
	virtual	void	mStop	( unsigned long );
	virtual	unsigned char	mAttachSoundToChannel	( unsigned long, tSoundSelection * );
	virtual	unsigned char	mAttachFileToChannel	( unsigned long, long );
	virtual	unsigned char	mResumeChannel	( unsigned long );
	virtual	unsigned char	mPauseChannel	( unsigned long );
	virtual	unsigned char	mStopAndResetChannel	( unsigned long );
	virtual	unsigned char	mStopAndClearChannel	( unsigned long );
	virtual	unsigned char	mSetFrequency	( unsigned long, long );
	virtual	unsigned char	mSetPan	( unsigned long, long );
	virtual	unsigned char	mSetVolume	( unsigned long, long );
	virtual	unsigned char	mSetPriority	( unsigned long, long );
	virtual	long	mGetChannelPriority	( unsigned long, long );
	virtual	long	mGetFrequency	( unsigned long, long );
	virtual	long	mGetPan	( unsigned long, long );
	virtual	long	mGetVolume	( unsigned long, long );
	virtual	unsigned char	mIsSoundPlaying	( unsigned long );
	virtual	unsigned char	mSetChannelAttributes	( unsigned long, tChannelAttributes * );
	virtual	unsigned char	mSet3DSoundProfile	( unsigned long, t3DSoundProfile * );
	virtual	unsigned char	mSet3DSoundPosition	( unsigned long, tPoint3 * );
	virtual	unsigned char	mSet3DSoundOrientation	( unsigned long, tPoint3 *, long, unsigned char );
	virtual	unsigned char	mSet3DSoundVelocity	( unsigned long, tPoint3 *, long, unsigned char );
	virtual	unsigned char	mSet3DSoundCones	( unsigned long, float, long, long, unsigned char );
	virtual	unsigned char	mSet3DSoundReferenceDistance	( unsigned long, double, long, unsigned char );
	virtual	tChannelAttributes *	mGetChannelAttributes	( unsigned long, long );
	virtual	t3DSoundProfile *	mGet3DSoundProfile	( unsigned long, long );
	virtual	tPoint3 *	mGet3DSoundPosition	( unsigned long, long );
	virtual	tPoint3 *	mGet3DSoundOrientation	( unsigned long, long );
	virtual	tPoint3 *	mGet3DSoundVelocity	( unsigned long, long );
	virtual	double	mGet3DSoundReferenceDistance	( unsigned long, long );
	virtual	float	mGet3DSoundConeAngleForNoAttenuation	( unsigned long, long );
	virtual	long	mGet3DSoundConeAttenuationLevel	( unsigned long, long );
	virtual	long	mTotalBytesLeftToPlay	( unsigned long, long );
	virtual	double	mTotalNumberOfSecondsLeftToPlay	( unsigned long, long );
	virtual	long	mNumberOfAttachmentsQueuedUp	( unsigned long, long );
	virtual	tSoundChannelFormat *	mGetSoundChannelFormat	( unsigned long, long );
	virtual	unsigned char	mAreWeLooping	( unsigned long, long );
	virtual	unsigned char	mAreWeStreaming	( unsigned long, long );
	virtual	unsigned char	mAreWe3D	( unsigned long, long );
	virtual	unsigned char	mAreWeStatic	( unsigned long, long );
	virtual	unsigned char	mAreWeFixedFrequency	( unsigned long, long );
	virtual	unsigned char	mAreWeARecordingChannel	( unsigned long, long );
	virtual	unsigned char	mTryToLocateInHardware	( unsigned long, long );
	virtual	unsigned char	mTryToLocateInSoftware	( unsigned long, long );
	virtual	unsigned char	mAreWeFixedPan	( unsigned long, long );
	virtual	unsigned char	mAreWeFixedVolume	( unsigned long, long );
	virtual	unsigned char	mAreWeAGlobalSound	( unsigned long, long );
	virtual	unsigned char	mDoWeRetainSelections	( unsigned long, long );
	virtual	void	mDump	( unsigned long, long );
	virtual	unsigned char	mSetFinishFunction	( unsigned long, void (* const function)( unsigned long, unsigned char ), long );
	virtual	unsigned char	mAddUpdateSoundPositionFunction	( unsigned long, void (* function)( unsigned long, long ), long, double, long, unsigned char );
	virtual	unsigned char	mAddDelayCommand	( unsigned long, double, long, unsigned char );
	virtual	unsigned char	mPlaySpecialLoop	( unsigned long, long, long, long, long, long );
	virtual	unsigned char	mEndSpecialLoop	( unsigned long, long );
	virtual	unsigned char	mAddGetNextAttachmentFunction	( unsigned long, void (* function)( unsigned long, long ), long, double, long );
	virtual	unsigned char	mFadeToVolume	( unsigned long, long, double, long, unsigned char );
	virtual	unsigned char	mFadeToStopAndResetNow	( unsigned long, double, long );
	virtual	unsigned char	mFadeToStopAndClearNow	( unsigned long, double, long );
	virtual	void	mCloseAllSoundsInGroup	( long );
	virtual	void	mCloseAllSoundsNotInThreshold	( long );
	virtual	void	mCloseAllSounds	( void );
	virtual	void	mCloseFile	( long );
	virtual	void	mAdjustGlobalVolume	( void );
	virtual	void	mSetGlobalVolume	( long );
	virtual	void	mUpdateAllSounds	( void );

	void	mUpdateTest	( void );

	// HYPOTHESIS: fields placed by the offsets the constructor stores; names not recovered.
	long					field_0xc;
	tAFPlayInfo *			field_0x10;
	long					field_0x14;
	tRegisteredAudioType	field_0x18[0x40];
	long					field_0x718;
	long					field_0x71c;
	long					field_0x720;
	long					field_0x724;
	char					fUnknown_0728[0x8];
	double					field_0x730;
	char					fUnknown_0738[0x8];
	double					field_0x740;
	long					field_0x748;
	// HYPOTHESIS: the play information index of each tracking slot, by mUpdateTest.
	long					field_0x74c[0x100];
	long					field_0xb4c[0x100];
	char					fUnknown_0f4c[0x1400];
	// HYPOTHESIS: unsigned, by the unsigned bound check in mNewTrackingNumber; the next slot to try.
	unsigned long			field_0x234c;
	short					field_0x2350[0x100];
	// HYPOTHESIS: the sound test's state, by mUpdateTest: the sound number, its handle, its play
	// time in milliseconds, the last number, and the times to stop it and to play the next.
	long					field_0x2550;
	unsigned long			field_0x2554;
	long					field_0x2558;
	char					fUnknown_255c[0x4];
	long					field_0x2560;
	char					fUnknown_2564[0x4];
	double					field_0x2568;
	double					field_0x2570;
	unsigned char			field_0x2578;
};

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

// HYPOTHESIS: empty body; the destructor's unwind frame fits the compiler's destruction of
// field_0x18 and the tPart base alone.
// FUNCTION: STARFLEETOP 0x00663e60
// FUNCTION: STARFLEETOP 0x00663e80
tBasicSoundFileManager::~tBasicSoundFileManager( void )
{
}

// The SDK library's symbol for the table at 0x00882160 gives the name; its contents are not
// written here.
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

// HYPOTHESIS: tChannelGroup is declared in Tiger/taudiochannel.cpp and tChannelGroupManagerPart in
// Tiger/tgSound.cpp, neither in a header yet; both are restated here only as far as the functions
// below need them. tChannelGroup's slots follow the exe's vtable at 0x007b7454, with the SDK
// library's symbols for the entries; the declaration in Tiger/taudiochannel.cpp has another order.
// Slot 7 of the manager part
// takes a group index and returns the group, as the SDK library's mGetChannelGroup does.
class tChannelGroup { public: virtual void mAddOwner(); virtual void mRemoveOwner(); virtual unsigned char mAttachSoundToChannel(unsigned long param_1, tSoundSelection *param_2, long param_3); virtual unsigned char mAttachFileToChannel(unsigned long param_1, long param_2); virtual unsigned char mAttachFileToBaseGroup(long param_1); virtual unsigned char mKillRetainedMode(); virtual unsigned char mClearAttachments(); virtual unsigned char mPlayChannel(unsigned long param_1, long param_2); virtual unsigned char mPauseChannel(unsigned long param_1, long param_2); virtual unsigned char mStopAndResetChannel(unsigned long param_1, long param_2); virtual unsigned char mStopAndClearChannel(unsigned long param_1, long param_2); virtual unsigned char mSetFrequency(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetPan(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetVolume(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetPriority(unsigned long param_1, long param_2, long param_3, unsigned char param_4); virtual unsigned char mSetFinishFunction(unsigned long param_1, void (* const param_2)(unsigned long, unsigned char), long param_3, long param_4, unsigned char param_5); virtual unsigned char mSetChannelAttributes(unsigned long param_1, tChannelAttributes *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundProfile(unsigned long param_1, t3DSoundProfile *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundPosition(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundOrientation(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundVelocity(unsigned long param_1, tPoint3 *param_2, long param_3, unsigned char param_4); virtual unsigned char mSet3DSoundCones(unsigned long param_1, float param_2, long param_3, long param_4, unsigned char param_5); virtual unsigned char mSet3DSoundReferenceDistance(unsigned long param_1, double param_2, long param_3, unsigned char param_4); virtual long mGetFrequency(unsigned long param_1, long param_2); virtual long mGetPan(unsigned long param_1, long param_2); virtual long mGetVolume(unsigned long param_1, long param_2); virtual tChannelAttributes * mGetChannelAttributes(unsigned long param_1, long param_2); virtual t3DSoundProfile * mGet3DSoundProfile(unsigned long param_1, long param_2); virtual tPoint3 * mGet3DSoundPosition(unsigned long param_1, long param_2); virtual tPoint3 * mGet3DSoundOrientation(unsigned long param_1, long param_2); virtual tPoint3 * mGet3DSoundVelocity(unsigned long param_1, long param_2); virtual double mGet3DSoundReferenceDistance(unsigned long param_1, long param_2); virtual float mGet3DSoundConeAngleForNoAttenuation(unsigned long param_1, long param_2); virtual long mGet3DSoundConeAttenuationLevel(unsigned long param_1, long param_2); virtual long mGetChannelPriority(unsigned long param_1, long param_2); /* HYPOTHESIS: returns eSoundChannelStatus, not declared yet */ virtual long mGetChannelStatus(unsigned long param_1, long param_2); virtual long mTotalBytesLeftToPlay(unsigned long param_1, long param_2); virtual double mTotalNumberOfSecondsLeftToPlay(unsigned long param_1, long param_2); virtual long mNumberOfAttachmentsQueuedUp(unsigned long param_1, long param_2); virtual tSoundChannelFormat * mGetSoundChannelFormat(unsigned long param_1, long param_2); virtual unsigned char mAreWeLooping(unsigned long param_1, long param_2); virtual unsigned char mAreWeStreaming(unsigned long param_1, long param_2); virtual unsigned char mAreWe3D(unsigned long param_1, long param_2); virtual unsigned char mAreWeStatic(unsigned long param_1, long param_2); virtual unsigned char mAreWeFixedFrequency(unsigned long param_1, long param_2); virtual unsigned char mAreWeARecordingChannel(unsigned long param_1, long param_2); virtual unsigned char mTryToLocateInHardware(unsigned long param_1, long param_2); virtual unsigned char mTryToLocateInSoftware(unsigned long param_1, long param_2); virtual unsigned char mAreWeFixedPan(unsigned long param_1, long param_2); virtual unsigned char mAreWeFixedVolume(unsigned long param_1, long param_2); virtual unsigned char mAreWeAGlobalSound(unsigned long param_1, long param_2); virtual unsigned char mDoWeRetainSelections(unsigned long param_1, long param_2); /* HYPOTHESIS: the two mDump overloads' slot order */ virtual void mDump(); virtual void mDump(unsigned long param_1, long param_2); virtual unsigned char mAddUpdateSoundPositionFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5, unsigned char param_6); virtual unsigned char mAddDelayCommand(unsigned long param_1, double param_2, long param_3, unsigned char param_4); virtual unsigned char mFadeToVolume(unsigned long param_1, long param_2, double param_3, long param_4, unsigned char param_5); virtual unsigned char mFadeToStopAndResetNow(unsigned long param_1, double param_2, long param_3); virtual unsigned char mFadeToStopAndClearNow(unsigned long param_1, double param_2, long param_3); virtual unsigned char mPlaySpecialLoop(unsigned long param_1, long param_2, long param_3, long param_4, long param_5, long param_6); virtual unsigned char mEndSpecialLoop(unsigned long param_1, long param_2); virtual unsigned char mAddGetNextAttachmentFunction(unsigned long param_1, void (*param_2)(unsigned long, long), long param_3, double param_4, long param_5); };
class tChannelGroupManagerPart : public tPart { public: GenAbstractBinding(tChannelGroupManagerPart); virtual unsigned long vfunction6(); virtual tChannelGroup * mGetChannelGroup(unsigned long index); };

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
