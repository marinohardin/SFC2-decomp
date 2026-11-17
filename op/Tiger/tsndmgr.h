// tsndmgr.h: the sound file manager part.

#ifndef TSNDMGR_H
#define TSNDMGR_H

#include "Tiger/tpart.h"
#include "Tiger/tgSound.h"	// eSoundChannelStatus and the channel types
#include "unsorted/tRegisteredAudioType.h"

class tAFPlayInfo;

// HYPOTHESIS: declared as far as the sound units need it. The virtuals follow the exe's vtable;
// the callers in Tiger/taudiopriority.cpp agree: tClassTrackingInfo::mGetPriority calls
// mGetChannelPriority through +0x78 (slot 30), mFadeToStop calls mFadeToStopAndResetNow through
// +0x128 (slot 74), and mAreWeValid calls mValidTrackingNumber (slot 8) and mStop (slot 19).
class tBasicSoundFileManager : public tPart
{
	public:
		tBasicSoundFileManager( void );
		virtual	~tBasicSoundFileManager( void );
		GenAbstractBinding( tBasicSoundFileManager );
		virtual	iTruth	mIsReady	( void );
		virtual	void	mDeactivate	( void );
		// Slots 6 to 83 in the order of the exe's vtable at 0x007b7664; the types from the SDK library's symbols.
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
		virtual	unsigned long	mPlay	( long, long, double, long, long, long, long, void (*finish)( long, unsigned long, unsigned char ), void (*nextAttachment)( unsigned long, long ) );
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
		virtual	void	mUpdatePosition	( unsigned long );
		virtual	void	mFinishSound	( unsigned long, unsigned char );
		virtual	void	mGetNextAttachment	( unsigned long );
		virtual	void	mSetGetNextAttachmentFunction	( unsigned long, void (* function)( unsigned long, long ) );	// HYPOTHESIS: vtable slot after mGetNextAttachment.
		virtual	void	mDoDefaultNextAttachment	( unsigned long );
		void	mUpdateTest	( void );
		eSoundChannelStatus	mGetStatus	( unsigned long handle );
		// HYPOTHESIS: fields placed by the offsets the constructor stores; names not recovered.
		long					field_0xc;
		tAFPlayInfo *			field_0x10;
		long					field_0x14;
		tRegisteredAudioType	field_0x18[0x40];
		long					field_0x718;
		void					(* field_0x71c)( unsigned long, long );	// HYPOTHESIS: 0x71c to 0x724 are the position functions mPlay gives a Dolby, a 3D and any other sound.
		void					(* field_0x720)( unsigned long, long );
		void					(* field_0x724)( unsigned long, long );
		char					fUnknown_0728[0x8];
		double					field_0x730;
		double					field_0x738;	// HYPOTHESIS: the position update interval mPlay passes.
		double					field_0x740;
		long					field_0x748;
		// HYPOTHESIS: the play information index of each tracking slot, by mUpdateTest.
		long					field_0x74c[0x100];
		long					field_0xb4c[0x100];
		unsigned long			field_0xf4c[0x100];	// HYPOTHESIS: the argument of each slot's position function, by mUpdatePosition.
		unsigned long			field_0x134c[0x100];	// HYPOTHESIS: each slot's audio class handle, by mFinishSound.
		void					(* field_0x174c[0x100])( unsigned long, long );	// HYPOTHESIS: each slot's position function; type from mPlay's symbol.
		void					(* field_0x1b4c[0x100])( unsigned long, long );	// HYPOTHESIS: each slot's next attachment function, by mGetNextAttachment; type from mSetGetNextAttachmentFunction's symbol.
		void					(* field_0x1f4c[0x100])( long, unsigned long, unsigned char );	// HYPOTHESIS: each slot's finish function; type from mPlay's symbol.
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

#endif // TSNDMGR_H
