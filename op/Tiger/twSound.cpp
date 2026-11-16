#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include "Tiger/tsay.h"

// HYPOTHESIS: only the three slots this unit calls are known (offsets 0x0c,
// 0x30 and 0x34: the window, the bits per sample and the sample rate of the
// primary buffer); the other slots and their names are not.
class tInitSoundPartInfo
{
public:
	virtual	void	vfunction1	( void );
	virtual	void	vfunction2	( void );
	virtual	void	vfunction3	( void );
	virtual	HWND	vfunction4	( void );
	virtual	void	vfunction5	( void );
	virtual	long	vfunction6	( void );
	virtual	long	vfunction7	( void );
	virtual	void	vfunction8	( void );
	virtual	void	vfunction9	( void );
	virtual	void	vfunction10	( void );
	virtual	void	vfunction11	( void );
	virtual	void	vfunction12	( void );
	virtual	WORD	vfunction13	( void );
	virtual	DWORD	vfunction14	( void );
};

#include "Tiger/tgSound.h"
// The sound part's classes are declared in tgSound.h; these lines are kept so line numbers stay.
#include "Tiger/tvectormath.h"



















































































































// HYPOTHESIS: the sound part's own say macro, after the release build's
// TGSAY2: at interrupt time the message is buffered, otherwise the buffered
// messages are flushed and the message printed.
#define SOUNDSAY(a)												\
	if( atgSoundPart->mAreWeInInterruptMode() == kTrue )					\
		AddSoundErrorDuringInterrupt a;							\
	else														\
	{															\
		PrintOutBufferedErrors();								\
		gSoundSay.mJustSayIt a;									\
	}
#define SOUNDSAY2(a)	{ SOUNDSAY(a); SOUNDSAY((" -- %s %d\n", __FILE__, __LINE__)); }
long	gNumBuffersCreated;
long	gNumBuffersReleased;

class twSoundPart : public tgSoundPart
{
public:
					twSoundPart					( void );
	virtual	unsigned char	mDetermineHardwareCaps	( void );
	virtual	iTruth	mInitDirectSound			( tInitSoundPartInfo* initInfo );
	virtual	void	mDone						( void );
	virtual	long	mDuplicateSoundChannel		( long channel );
	virtual	long	mAddActiveSoundChannel		( tSoundChannelFormat* format, tSoundChannel::tFillSelectionFunction& fill, tSoundChannel::tSelectionFunction& next, unsigned long userData );
	virtual	long	mAddSoundChannel			( tSoundChannelFormat* format );
	virtual	unsigned char	mSet3DListener3DPosition	( tPoint3* position );
	virtual	unsigned char	mSet3DListenerOrientation	( tPoint3* forward, tPoint3* up );
	virtual	unsigned char	mSet3DListenerVelocity	( tPoint3* velocity );
	virtual	unsigned char	mSet3DListenerUnitScaleInMeters	( double scale );
	virtual	long	mGetHardwareInformation		( long info );
	virtual	iTruth	mInit						( tInitSoundPartInfo* initInfo );
	// twSoundPart's own virtuals (the SDK library's symbols), vtable order (0x00798a3c, slots 141-160).
	virtual	unsigned char	mCreatePrimaryBuffer	( void );
	virtual	long	mGetMinFrequency			( void );
	virtual	long	mGetMaxFrequency			( void );
	virtual	long	mGetMaxHwBuffers			( void );
	virtual	long	mGetMaxHwStaticBuffers		( void );
	virtual	long	mGetMaxHwStreamingBuffers	( void );
	virtual	long	mGetMaxHw3DBuffers			( void );
	virtual	long	mGetFreeHwBuffers			( void );
	virtual	long	mGetFreeHwStaticBuffers		( void );
	virtual	long	mGetFreeHwStreamingBuffers	( void );
	virtual	long	mGetFreeHw3DBuffers			( void );
	virtual	long	mGetTotalHwMemBytes			( void );
	virtual	long	mGetFreeHwMemBytes			( void );
	virtual	long	mGetMaxContigFreeHwMemBytes	( void );
	virtual	long	mGetPlayCpuOverheadSwBuffers	( void );
	virtual	long	mGetTransferToHardwareRateInKBytesPerSec	( void );
	virtual	unsigned char	mCanWeHandle16BitOutput	( void );
	virtual	unsigned char	mCanWeHandleStereoOutput	( void );
	virtual	unsigned char	mCanWeHandleMono8BitOutput	( void );
	virtual	LPDIRECTSOUND*	mDirectSoundObject	( void );
	// HYPOTHESIS: two DirectSound interfaces mDone releases before the DirectSound object;
	// the primary buffer and the 3D listener by their order, names and types are not known.
	LPDIRECTSOUNDBUFFER		fUnknown_0130;
	LPDIRECTSOUND3DLISTENER	fUnknown_0134;
	LPDIRECTSOUND	fDirectSound;
	// The DirectSound capabilities: mDetermineHardwareCaps (0x00645bb0) clears these 0x60
	// bytes, sets dwSize to 0x60 and passes them to GetCaps. The name is ours.
	DSCAPS			field_0x13c;
};
extern twSoundPart*	atwSoundPart;
// HYPOTHESIS: the original was built against the DirectX 7 headers, whose
// DSBUFFERDESC is 36 bytes, with guid3DAlgorithm after lpwfxFormat; the
// compiler's own dsound.h is the DirectX 6 one, 20 bytes. Same fields, in the
// same order, under a name of our own.
struct DSBUFFERDESC7
{
	DWORD			dwSize;
	DWORD			dwFlags;
	DWORD			dwBufferBytes;
	DWORD			dwReserved;
	LPWAVEFORMATEX	lpwfxFormat;
	GUID			guid3DAlgorithm;
};
// MATCH-ONLY: the original's line numbers, which its " -- %s %d" messages
// print; this file's own lines differ. The original's __FILE__ was its full
// build path, D:\work\QSI\Projects\QSIInc\Tiger\twSound.cpp; naming it here
// breaks the compiler's source listing, so only the line is set.
#line 31
// FUNCTION: STARFLEETOP 0x00641320
iTruth twSoundPart::mInitDirectSound( tInitSoundPartInfo* initInfo )
{
	if( initInfo == NULL )
	{
		SOUNDSAY2(( "Null initInfo" ));
		return kFalse;
	}
	HWND hwnd = initInfo->vfunction4();

	fDirectSound = NULL;
	if( DirectSoundCreate( NULL, &fDirectSound, NULL ) != DS_OK )
	{
		fDirectSound = NULL;
		return kFalse;
	}

	if( fDirectSound == NULL )
	{
		SOUNDSAY2(( "Null directSound" ));
		return kFalse;
	}

	if( fDirectSound->SetCooperativeLevel( hwnd, DSSCL_EXCLUSIVE ) != DS_OK )
	{
		if( fDirectSound )
		{
			fDirectSound->Release();
			fDirectSound = NULL;
		}
		return kFalse;
	}

	mDetermineHardwareCaps();

	DSBUFFERDESC7		desc;
	WAVEFORMATEX		format;
	LPDIRECTSOUNDBUFFER	primary;
	memset( &desc, 0, sizeof( desc ) );
	memset( &format, 0, sizeof( format ) );
	primary = NULL;
	desc.dwSize				= sizeof( desc );
	format.wFormatTag		= WAVE_FORMAT_PCM;
	format.nChannels		= 2;
	format.nSamplesPerSec	= initInfo->vfunction14();
	format.wBitsPerSample	= initInfo->vfunction13();
	format.nBlockAlign		= (BYTE)( (WORD)( format.wBitsPerSample * format.nChannels ) / 8 );
	format.nAvgBytesPerSec	= format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize			= 0;
	desc.dwFlags			= DSBCAPS_PRIMARYBUFFER;
	desc.dwBufferBytes		= 0;
	desc.dwReserved			= 0;
	desc.lpwfxFormat		= NULL;

	if( fDirectSound->CreateSoundBuffer( (LPDSBUFFERDESC)&desc, &primary, NULL ) != DS_OK )
		return kFalse;
	if( primary == NULL )
	{
		SOUNDSAY2(( "Couldn't CreateSoundBuffer!" ));
		return kTrue;
	}

	gNumBuffersCreated++;
	primary->SetFormat( &format );
	primary->Release();
	gNumBuffersReleased++;
	return kTrue;
}
// The channel's base classes are declared in tgSound.h; these lines are kept so line numbers stay.




































class twStandardSoundChannel : public tSoundChannel
{
public:
	// HYPOTHESIS: inline; twSoundPart::mDuplicateSoundChannel inlines it.
					twStandardSoundChannel	( void )	{ mInit(); }
	// The SDK library names the destructor (0x00646bd0, among the out-of-line inline copies).
	virtual			~twStandardSoundChannel	( void );
	virtual	void	mClear	( void );
	virtual	void	mReset	( iTruth );
	virtual	iTruth	mPauseChannel	( void );
	virtual	unsigned char	mPlayChannel	( void );
	virtual	unsigned char	mSetFrequency	( long );
	virtual	unsigned char	mSetPan	( long );
	virtual	unsigned char	mSetVolume	( long );
	virtual	unsigned char	mStopAndClearChannel	( void );
	virtual	unsigned char	mStopAndResetChannel	( void );
	virtual	unsigned char	mSet3DSoundPosition	( tPoint3* );
	virtual	unsigned char	mSet3DSoundOrientation	( tPoint3* );
	virtual	unsigned char	mSet3DSoundVelocity	( tPoint3* );
	virtual	unsigned char	mSet3DSoundCones	( float, long );
	virtual	unsigned char	mSet3DSoundReferenceDistance	( double );
	virtual	unsigned char	mSet3DSoundProfile	( t3DSoundProfile* );
	virtual	void	mDone	( void );
	virtual	void	mInit	( void );

	// HYPOTHESIS: the description and format the channel's buffer is created from, filled by
	// mDone; tSoundChannel ends at 0x180, so they are this class's first fields.
	DSBUFFERDESC7		fUnknown_0180;
	WAVEFORMATEX		fUnknown_01a4;
	LPDIRECTSOUNDBUFFER	fSoundBuffer;
	// The buffer's 3D interface: mCreate3DBuffer queries it, mDone releases it
	// before the buffer itself. The name is not known.
	LPDIRECTSOUND3DBUFFER	fUnknown_01bc;
	// HYPOTHESIS: the two pointers and two sizes of the current buffer lock;
	// fCurrentBufSize1 is named by the original's "Big Trouble" message.
	LPVOID				fUnknown_01c0;
	LPVOID				fUnknown_01c4;
	DWORD				fCurrentBufSize1;
	DWORD				fUnknown_01cc;
	// HYPOTHESIS: the same four values as returned by the last Lock.
	LPVOID				fUnknown_01d0;
	LPVOID				fUnknown_01d4;
	DWORD				fUnknown_01d8;
	DWORD				fUnknown_01dc;
	// HYPOTHESIS: the buffer position mLockHardwareForSilence locks from.
	long				fUnknown_01e0;
	char				fUnknown_01e4[0x4];
	DWORD				fUnknown_01e8;
	DWORD				fUnknown_01ec;
protected:
	iTruth	mUpdateAmountReadableAndAmountPlayed	( double );
	unsigned char	mLockHardware	( void );
	unsigned char	mLockHardwareForSilence	( void );
	unsigned char	mUnlockHardware	( void );
	unsigned char	mWriteData	( unsigned char*, long );
	void	mWriteSilence	( void );
	unsigned char	mDoActiveUpdate	( double );
	virtual unsigned char	mUpdateBuffersAndSelections	( double );
	virtual unsigned char	mUpdateAmountPlayed	( void );
	// HYPOTHESIS: own virtuals follow tSoundChannel's 122 slots; the getter at 125 and mCreate3DBuffer at 128 are called through the vtable.
	virtual void vfunction122(); virtual void vfunction123(); virtual void vfunction124();
	virtual LPDIRECTSOUNDBUFFER mGetfDSBuffer();
	// Slot 126: twSoundPart::mAddActiveSoundChannel calls it through the vtable.
public:
	virtual unsigned char	mAddActiveSoundChannel	( tSoundChannelFormat*, long, tFillSelectionFunction&, tSelectionFunction&, unsigned long );
protected:
	virtual unsigned char	mRestoreBuffers	( void );
	virtual unsigned char	mCreate3DBuffer	( void );
public:
	virtual unsigned char	mDuplicateSoundChannel	( long, long );
	virtual unsigned char	mAddSoundChannel	( tSoundChannelFormat*, long );
	virtual unsigned char	mDoFillSelection	( unsigned char );
	// HYPOTHESIS: slot not recovered; nothing in this unit calls it through the vtable.
	virtual void	mDoAllImmediateNonInterruptCommands	( void );
	virtual	void	mDump	( void );
};
// MATCH-ONLY: the original's line numbers, as for mInitDirectSound above.
#line 109
// FUNCTION: STARFLEETOP 0x00641640
void twStandardSoundChannel::mClear( void )
{
	tSoundChannel::mClear();

	if( fSoundBuffer )
	{
		HRESULT err = fSoundBuffer->SetCurrentPosition( field_0x15c );
		if( err != DS_OK )
		{
			SOUNDSAY2(( "DS error calling SetCurrentPosition: %ld", err ));
		}
	}

	fUnknown_01c0 = 0;
	fUnknown_01c4 = 0;
	fCurrentBufSize1 = 0;
	fUnknown_01cc = 0;
}

// MATCH-ONLY: the original's line numbers, as for mInitDirectSound above.
#line 133
// FUNCTION: STARFLEETOP 0x00641710
void twStandardSoundChannel::mReset( iTruth flag )
{
	tSoundChannel::mReset( flag );

	if( fSoundBuffer )
	{
		HRESULT err = fSoundBuffer->SetCurrentPosition( field_0x15c );
		if( err != DS_OK )
		{
			SOUNDSAY2(( "DS error calling SetCurrentPosition: %ld", err ));
		}
	}

	fUnknown_01c0 = 0;
	fUnknown_01c4 = 0;
	fCurrentBufSize1 = 0;
	fUnknown_01cc = 0;
}

// MATCH-ONLY: the #line directives inside set the original's line numbers for
// its three " -- %s %d" messages (183, 194, 236), as for mInitDirectSound above;
// the original had more lines between them than this source does.
// FUNCTION: STARFLEETOP 0x006417f0
iTruth twStandardSoundChannel::mPauseChannel( void )
{
	DWORD playCursor;
	DWORD status;
	if( fSoundBuffer && fSoundBuffer->GetStatus( &status ) == DS_OK && status )
	{
		DWORD position = field_0x164;
		if( field_0x28 == 0x20 && fSoundBuffer )
		{
			fUnknown_01e8 = 0;
			fUnknown_01ec = 0;
			if( atgSoundPart == NULL )
			{
#line 183
				SOUNDSAY2(( "atgSoundPart was NULL" ));
				return kFalse;
			}
			mUpdateAmountReadableAndAmountPlayed( atgSoundPart->vfunction125() );

			if( mAreWeStatic() == kTrue )
			{
				if( fSoundBuffer == NULL )
				{
#line 194
					SOUNDSAY2(( "fDSBuffer was NULL" ));
					return kFalse;
				}
				if( fSoundBuffer->GetCurrentPosition( &playCursor, NULL ) != DS_OK )
				{
					field_0x168 = 0;
					return kFalse;
				}
				position = playCursor - ( playCursor & 3 );
				if( BindPart( tSoundPart )->vfunction13() == 1 )
					field_0x164 = position;
			}

			if( fSoundBuffer == NULL )
			{
#line 236
				SOUNDSAY2(( "fDSBuffer was NULL" ));
				return kFalse;
			}
			if( fSoundBuffer->Stop() != DS_OK )
				return kFalse;
			field_0x15c = position;
			if( fSoundBuffer && fSoundBuffer->SetCurrentPosition( position ) != DS_OK )
				return kFalse;
			field_0x28 = 8;
			return kTrue;
		}
	}
	return kFalse;
}

// The original's line for mDone is not known; this file's own numbering resumes.
#line 511
// FUNCTION: STARFLEETOP 0x00641af0
void twStandardSoundChannel::mDone( void )
{
	tSoundChannel::mDone();

	if( fUnknown_01bc )
	{
		fUnknown_01bc->Release();
		fUnknown_01bc = NULL;
	}
	if( fSoundBuffer )
	{
		fSoundBuffer->Release();
		fSoundBuffer = NULL;
		gNumBuffersReleased++;
	}

	fUnknown_01a4.wFormatTag		= WAVE_FORMAT_PCM;
	fUnknown_01a4.nChannels			= field_0x10.vfunction5();
	fUnknown_01a4.nSamplesPerSec	= field_0x10.vfunction7();
	fUnknown_01a4.wBitsPerSample	= field_0x10.vfunction9();
	fUnknown_01a4.nBlockAlign		= (BYTE)( (WORD)( fUnknown_01a4.nChannels * fUnknown_01a4.wBitsPerSample ) / 8 );
	fUnknown_01a4.nAvgBytesPerSec	= fUnknown_01a4.nSamplesPerSec * fUnknown_01a4.nBlockAlign;
	fUnknown_01a4.cbSize			= 0;
	fUnknown_0180.dwSize			= sizeof( fUnknown_0180 );
	fUnknown_0180.dwFlags			= DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	fUnknown_0180.dwBufferBytes		= field_0x10.vfunction3();
	fUnknown_0180.dwReserved		= 0;
	fUnknown_0180.lpwfxFormat		= &fUnknown_01a4;

	fUnknown_01c0 = 0;
	fUnknown_01c4 = 0;
	fCurrentBufSize1 = 0;
	fUnknown_01cc = 0;
}

// FUNCTION: STARFLEETOP 0x00641bf0
void twStandardSoundChannel::mInit( void )
{
	tSoundChannel::mInit();

	fSoundBuffer	= NULL;
	fUnknown_01bc	= NULL;
	memset( &fUnknown_0180, 0, sizeof( fUnknown_0180 ) );
	fUnknown_0180.dwSize			= sizeof( fUnknown_0180 );
	memset( &fUnknown_01a4, 0, sizeof( fUnknown_01a4 ) );
	fUnknown_01a4.wFormatTag		= WAVE_FORMAT_PCM;
	fUnknown_01a4.nChannels			= field_0x10.vfunction5();
	fUnknown_01a4.nSamplesPerSec	= field_0x10.vfunction7();
	fUnknown_01a4.wBitsPerSample	= field_0x10.vfunction9();
	fUnknown_01a4.nBlockAlign		= (BYTE)( (WORD)( fUnknown_01a4.nChannels * fUnknown_01a4.wBitsPerSample ) / 8 );
	fUnknown_01a4.nAvgBytesPerSec	= fUnknown_01a4.nSamplesPerSec * fUnknown_01a4.nBlockAlign;
	fUnknown_01a4.cbSize			= 0;
	fUnknown_0180.dwFlags			= DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
	// MATCH-ONLY: the original stores fUnknown_01e8 between the call and the
	// store of its result; the source form that produced that is not known.
	DWORD bufferBytes = field_0x10.vfunction3();
	fUnknown_01e8 = 0;
	fUnknown_0180.dwBufferBytes		= bufferBytes;
	fUnknown_0180.dwReserved		= 0;
	fUnknown_0180.lpwfxFormat		= &fUnknown_01a4;

	fUnknown_01c0 = 0;
	fUnknown_01c4 = 0;
	fCurrentBufSize1 = 0;
	fUnknown_01cc = 0;
	fUnknown_01ec = 0;
}

// MATCH-ONLY: the #line directive inside sets the original's line number for
// its " -- %s %d" message (389), as for mPauseChannel above. This annotation
// stays on line 586 of the file, where the harness gives it this function and
// not mWriteSilence, whose message line is 585.
// FUNCTION: STARFLEETOP 0x00641cf0
unsigned char twStandardSoundChannel::mLockHardware( void )
{
	if( fSoundBuffer == NULL )
		return kFalse;

	LPVOID	ptr1;
	LPVOID	ptr2;
	DWORD	bytes1;
	DWORD	bytes2;
	if( fSoundBuffer->Lock( field_0x160, field_0x168, &ptr1, &bytes1, &ptr2, &bytes2, 0 ) != DS_OK )
		return kFalse;
	if( ptr1 == NULL )
		return kFalse;

	fUnknown_01d0 = ptr1;
	fUnknown_01d4 = ptr2;
	fUnknown_01d8 = bytes1;
	fUnknown_01dc = bytes2;
	fUnknown_01c0 = ptr1;
	fUnknown_01c4 = ptr2;
	fCurrentBufSize1 = bytes1;
	if( fCurrentBufSize1 > 10000000 )
	{
#line 389
		SOUNDSAY2(( "Big Trouble,fCurrentBufSize1 = %ld", fCurrentBufSize1 ));
	}
	fUnknown_01cc = fUnknown_01dc;
	return kTrue;
}

// MATCH-ONLY: the #line directive inside sets the original's line number for
// its " -- %s %d" message (448), as for mLockHardware above. The one here and
// the one after the message only move this function's other lines past the
// earlier functions' in the compiler's listing, so the function is claimed by
// its own annotation; with other values the listing's first line for it falls
// inside mReset's range. They change no code.
#line 1000
// FUNCTION: STARFLEETOP 0x00641e50
unsigned char twStandardSoundChannel::mLockHardwareForSilence( void )
{
	if( fSoundBuffer == NULL )
		return kFalse;

	long start = fUnknown_01e0;
	if( start >= (long)field_0x10.vfunction3() )
		start -= field_0x10.vfunction3();

	long size = field_0x160 + field_0x168 - fUnknown_01e0;
	if( size <= 0 )
		return kFalse;

	LPVOID	ptr1;
	LPVOID	ptr2;
	DWORD	bytes1;
	DWORD	bytes2;
	if( fSoundBuffer->Lock( start, size, &ptr1, &bytes1, &ptr2, &bytes2, 0 ) != DS_OK )
		return kFalse;
	if( ptr1 == NULL )
		return kFalse;

	fUnknown_01d0 = ptr1;
	fUnknown_01d4 = ptr2;
	fUnknown_01d8 = bytes1;
	fUnknown_01dc = bytes2;
	fUnknown_01c0 = ptr1;
	fUnknown_01c4 = ptr2;
	fCurrentBufSize1 = bytes1;
	if( fCurrentBufSize1 > 10000000 )
	{
#line 448
		SOUNDSAY2(( "Big Trouble,fCurrentBufSize1 = %ld", fCurrentBufSize1 ));
#line 653
	}
	fUnknown_01cc = fUnknown_01dc;
	return kTrue;
}

// MATCH-ONLY: the #line directive inside sets the original's line number for
// its " -- %s %d" message (463), as for mLockHardware above; the one after it
// moves this function's other lines past the earlier functions' in the
// compiler's listing, as in mLockHardwareForSilence. They change no code.
// FUNCTION: STARFLEETOP 0x00641fe0
unsigned char twStandardSoundChannel::mUnlockHardware( void )
{
	if( fSoundBuffer == NULL )
	{
#line 463
		SOUNDSAY2(( "fDSBuffer was NULL" ));
#line 1100
		return kTrue;
	}

	fSoundBuffer->Unlock( fUnknown_01d0, fUnknown_01d8, fUnknown_01d4, fUnknown_01dc );
	return kTrue;
}

// MATCH-ONLY: the #line directives inside set the original's line numbers for
// its " -- %s %d" messages (509, 514, 526), as for mLockHardware above; the
// others move this function's other lines past the earlier functions' in the
// compiler's listing, as in mLockHardwareForSilence. They change no code.
#line 690
// FUNCTION: STARFLEETOP 0x006420a0
unsigned char twStandardSoundChannel::mWriteData( unsigned char* data, long size )
{
	if( data == NULL )
		return kFalse;
	// HYPOTHESIS: a separate read pointer; the original keeps one apart from
	// data. Where it is declared is not known: the original pushes edi only
	// after both checks and keeps it in size's slot, and neither placement
	// tried reproduces both.
	unsigned char* src = data;
	if( size <= 0 )
		return kFalse;
	field_0x160 += size;
	field_0x168 -= size;
	fUnknown_01e0 = field_0x160;
	if( (long)field_0x160 >= (long)field_0x10.vfunction3() )
		field_0x160 -= field_0x10.vfunction3();
	if( fCurrentBufSize1 > 10000000 )
	{
#line 509
		SOUNDSAY2(( "Big Trouble,fCurrentBufSize1 = %ld", fCurrentBufSize1 ));
#line 712
		fCurrentBufSize1 = 0;
	}
	if( size > 10000000 )
	{
#line 514
		SOUNDSAY2(( "Big Trouble,fCurrentBufSize1 = %ld, size is %ld", fCurrentBufSize1, size ));
#line 718
		// HYPOTHESIS: slot 9 of the channel; its name is not known.
		mDump();
		size = 0;
	}
	if( fUnknown_01c0 && fCurrentBufSize1 > 0 )
	{
		if( (long)fCurrentBufSize1 >= size )
		{
			memcpy( fUnknown_01c0, src, size );
			fCurrentBufSize1 -= size;
			if( fCurrentBufSize1 > 10000000 )
			{
#line 526
				SOUNDSAY2(( "Big Trouble,fCurrentBufSize1 = %ld, size is %ld", fCurrentBufSize1, size ));
#line 732
				fCurrentBufSize1 = 0;
			}
			fUnknown_01c0 = (unsigned char*)fUnknown_01c0 + size;
			return kTrue;
		}
		memcpy( fUnknown_01c0, src, fCurrentBufSize1 );
		src += fCurrentBufSize1;
		size -= fCurrentBufSize1;
		fUnknown_01c0 = 0;
		fCurrentBufSize1 = 0;
	}
	if( size > 0 && fUnknown_01c4 && fUnknown_01cc > 0 )
	{
		if( (long)fUnknown_01cc >= size )
		{
			memcpy( fUnknown_01c4, src, size );
			fUnknown_01cc -= size;
			fUnknown_01c4 = (unsigned char*)fUnknown_01c4 + size;
			return kTrue;
		}
		memcpy( fUnknown_01c4, src, fUnknown_01cc );
		fUnknown_01c4 = 0;
		fUnknown_01cc = 0;
	}
	return kTrue;
}

unsigned char GetSilenceData( long bitsPerSample );

// MATCH-ONLY: the #line directives inside set the original's line number for
// its " -- %s %d" message (585), as for mLockHardware above, and then restore
// this file's own numbering. They change no code.
// FUNCTION: STARFLEETOP 0x00642410
void twStandardSoundChannel::mWriteSilence( void )
{
	unsigned char silence = GetSilenceData( field_0x10.vfunction9() );
	fUnknown_01e0 = field_0x160 + field_0x168;
	if( fCurrentBufSize1 > 10000000 )
	{
		// The channel writes silence at interrupt time, so the message goes
		// straight to the interrupt-time buffer.
		AddSoundErrorDuringInterrupt( "Big Trouble,fCurrentBufSize1 = %ld", fCurrentBufSize1 );
#line 585
		AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
#line 779
		// HYPOTHESIS: slot 9 of the channel, as in mWriteData.
		mDump();
	}
	if( fUnknown_01c0 && fCurrentBufSize1 > 0 )
	{
		memset( fUnknown_01c0, silence, fCurrentBufSize1 );
		fUnknown_01c0 = 0;
		fCurrentBufSize1 = 0;
	}
	if( fUnknown_01c4 && fUnknown_01cc > 0 )
	{
		memset( fUnknown_01c4, silence, fUnknown_01cc );
		fUnknown_01c4 = 0;
		fUnknown_01cc = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00642510
iTruth twStandardSoundChannel::mUpdateAmountReadableAndAmountPlayed( double time )
{
	if( fSoundBuffer == NULL )
		return kFalse;

	// HYPOTHESIS: slot 72 of the channel; its name is not known.
	if( mAreWeStreaming() == kTrue )
	{
		// HYPOTHESIS: fUnknown_01e8 and fUnknown_01ec together hold the time of
		// the last update, as a double.
		if( time - *(double*)&fUnknown_01e8 < 250000.0 )
			return kTrue;
		*(double*)&fUnknown_01e8 = time;

		DWORD playCursor;
		if( fSoundBuffer->GetCurrentPosition( &playCursor, NULL ) != DS_OK )
		{
			field_0x168 = 0;
			return kFalse;
		}

		long oldPosition = field_0x164;
		long oldReadable = field_0x168;
		long newPosition = playCursor - ( playCursor & 3 );
		field_0x164 = newPosition;
		if( oldPosition == newPosition )
			return kTrue;
		if( oldPosition < newPosition )
			field_0x168 += newPosition - oldPosition;
		else
			field_0x168 += field_0x10.vfunction3() + ( field_0x164 - oldPosition );

		field_0x110 += field_0x168 - oldReadable;
		if( (long)field_0x168 > (long)field_0x10.vfunction3() )
			field_0x168 = field_0x10.vfunction3();
	}
	return kTrue;
}

// The sound selection class is declared in tgSound.h; these lines are kept so line numbers stay.





































unsigned char	gUpdateBuffers;

// MATCH-ONLY: the #line directives inside set the original's line numbers for
// its " -- %s %d" messages (697, 780, 789, 811), as for mLockHardware above;
// the others move this function's other lines past the earlier functions' in
// the compiler's listing, as in mLockHardwareForSilence. They change no code.
// FUNCTION: STARFLEETOP 0x00642630
unsigned char twStandardSoundChannel::mDoActiveUpdate( double time )
{
	if( fSoundBuffer == NULL )
	{
#line 697
		SOUNDSAY2(( "fDSBuffer was NULL" ));
#line 2000
		return kTrue;
	}

	gUpdateBuffers = kTrue;
	if( mAreWeStatic() == kTrue )
	{
		if( field_0x28 == 0x20 )
		{
			DWORD status;
			fSoundBuffer->GetStatus( &status );
			if( ( status & ( DSBSTATUS_PLAYING | DSBSTATUS_LOOPING ) ) == 0 )
			{
				// HYPOTHESIS: slot 104 of the channel; its name is not known.
				mStopAndResetChannel();
				gUpdateBuffers = kFalse;
				return kTrue;
			}
		}
		return kTrue;
	}

	DWORD playCursor;
	if( fSoundBuffer->GetCurrentPosition( &playCursor, NULL ) != DS_OK )
	{
		field_0x168 = 0;
		return kFalse;
	}

	// HYPOTHESIS: the buffer is played in two halves; each time the play
	// position crosses into the other half, the half just played is refilled.
	long oldPosition = field_0x164;
	long newPosition = playCursor - ( playCursor & 3 );
	DWORD halfSize = (long)field_0x10.vfunction3() >> 1;
	if( newPosition < halfSize && oldPosition < halfSize )
	{
		gUpdateBuffers = kFalse;
		return kTrue;
	}
	if( newPosition >= halfSize && oldPosition >= halfSize )
	{
		gUpdateBuffers = kFalse;
		return kTrue;
	}

	long played = newPosition - oldPosition;
	field_0x164 = newPosition;
	DWORD lockStart = 0;
	if( newPosition < halfSize && oldPosition >= halfSize )
	{
		played += field_0x10.vfunction3();
		lockStart = halfSize;
	}

	tSoundSelection* block = field_0x174( field_0x178, played );
	if( block == NULL )
	{
		mStopAndResetChannel();
		gUpdateBuffers = kFalse;
		return kTrue;
	}

	DWORD blockSize = block->vfunction9();
	if( blockSize != field_0x10.vfunction3() )
	{
		AddSoundErrorDuringInterrupt( "Major Trouble!" );
#line 780
		AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
#line 2100
		gUpdateBuffers = kFalse;
		return kTrue;
	}

	block->vfunction14( kFalse );
	void* data = block->vfunction22();
	if( data == NULL )
	{
		AddSoundErrorDuringInterrupt( "Major Trouble!" );
#line 789
		AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
#line 2200
		gUpdateBuffers = kFalse;
		block->vfunction23();
		block->vfunction14( kTrue );
		return kTrue;
	}

	LPVOID	ptr1;
	LPVOID	ptr2;
	DWORD	bytes1;
	DWORD	bytes2;
	if( fSoundBuffer->Lock( lockStart, halfSize, &ptr1, &bytes1, &ptr2, &bytes2, 0 ) == DS_OK && bytes1 == halfSize )
	{
		memcpy( ptr1, data, bytes1 );
		fSoundBuffer->Unlock( ptr1, bytes1, ptr2, bytes2 );
	}
	else
	{
		AddSoundErrorDuringInterrupt( "No Lock -- Major Trouble!" );
#line 811
		AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
#line 2300
	}
	gUpdateBuffers = kFalse;
	block->vfunction23();
	block->vfunction14( kTrue );
	return kTrue;
}
// MATCH-ONLY: the #line directives below and inside set the original's line numbers for its " -- %s %d" messages (850, 932, 960, 1183, 1188), as for mLockHardware above; the others put this file's own numbering back. They change no code.
#line 995
// FUNCTION: STARFLEETOP 0x00642980
unsigned char twStandardSoundChannel::mUpdateBuffersAndSelections( double time )
{
	if( BindPart( tSoundPart )->vfunction13() == 1 )
		return mDoActiveUpdate( time );

	if( fSoundBuffer == NULL )
	{
#line 850
		SOUNDSAY2(( "fDSBuffer was NULL" ));
#line 1006
		return kTrue;
	}

	gUpdateBuffers = kTrue;
	mUpdateAmountReadableAndAmountPlayed( time );
	if( mAreWeStatic() == kTrue && field_0x28 == 0x20 )
	{
		DWORD status;
		fSoundBuffer->GetStatus( &status );
		if( ( status & ( DSBSTATUS_PLAYING | DSBSTATUS_LOOPING ) ) == 0 )
		{
			mStopAndResetChannel();
			gUpdateBuffers = kFalse;
			return kTrue;
		}
	}

	tSoundSelection* selection = mGetSelectionToAttach();
	if( selection == NULL && field_0xec.vfunction38() == kFalse )
		selection = mGetSelectionToAttach();

	long amountLeftToRead = 0;
	if( selection )
	{
		amountLeftToRead = selection->vfunction26();
		if( amountLeftToRead < 0 )
		{
			SOUNDSAY(( "Sound Trouble amountLeftToRead = %ld\n", amountLeftToRead ));
		}
		while( selection )
		{
			if( amountLeftToRead != 0 )
				break;
			if( field_0x148 == kTrue )
			{
				if( field_0x10c == field_0x154 )
				{
					selection->vfunction20();
					while( field_0x10c > field_0x150 )
					{
						field_0x10c--;
						field_0x108 = field_0xec.vfunction7( field_0x108 );
						selection = mGetSelectionToAttach();
						if( selection )
							selection->vfunction20();
					}
				}
				else
				{
					field_0x10c++;
					field_0x108 = field_0xec.vfunction9( field_0x108 );
					selection = mGetSelectionToAttach();
					if( selection == NULL )
						break;
					amountLeftToRead = selection->vfunction26();
					if( amountLeftToRead < 0 )
					{
#line 932
						SOUNDSAY2(( "Sound Trouble2 amountLeftToRead = %ld\n", amountLeftToRead ));
#line 1064
					}
				}
			}
			else if( field_0x10c != field_0x158 && mAreWeStatic() != kTrue )
			{
				field_0x10c++;
				field_0x108 = field_0xec.vfunction9( field_0x108 );
				selection = mGetSelectionToAttach();
				if( selection == NULL )
					break;
				amountLeftToRead = selection->vfunction26();
				if( amountLeftToRead < 0 )
				{
					AddSoundErrorDuringInterrupt( "Sound Trouble amountLeftToRead = %ld\n", amountLeftToRead );
#line 960
					AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
#line 1087
				}
			}
			else
			{
				field_0x108 = -1;
				selection = NULL;
			}
		}
	}

	if( field_0xec.vfunction38() == kTrue )
	{
		if( field_0x28 == 0x20 && field_0x168 == field_0x10.vfunction3() )
		{
			if( mDoWeRetainSelections() == kTrue )
				mStopAndResetChannel();
			else
				mStopAndClearChannel();
			if( mLockHardwareForSilence() == kTrue )
			{
				mWriteSilence();
				mUnlockHardware();
			}
		}
		gUpdateBuffers = kFalse;
		return kTrue;
	}

	if( mDoWeRetainSelections() == kTrue && mAreWeStreaming() == kTrue && amountLeftToRead == 0
		&& field_0x168 == mGetBufferLength() && ( field_0x28 == 0x20 || field_0x28 == 0x10 ) )
	{
		mStopAndResetChannel();
		gUpdateBuffers = kFalse;
		return kTrue;
	}

	if( field_0x168 == 0 )
	{
		gUpdateBuffers = kFalse;
		return kTrue;
	}
	if( (long)field_0x168 < ( (long)field_0x10.vfunction3() >> 2 )
		&& amountLeftToRead >= (long)field_0x168 && mAreWeStreaming() == kTrue )
	{
		gUpdateBuffers = kFalse;
		return kTrue;
	}

	if( selection )
	{
		if( mLockHardware() != kTrue )
		{
			gUpdateBuffers = kFalse;
			return kTrue;
		}
		while( (long)field_0x168 > 0 && selection )
		{
			unsigned char* data = (unsigned char*)selection->vfunction22();
			if( data != NULL )
			{
				field_0xb8 = 0;
				if( amountLeftToRead >= (long)field_0x168 )
				{
					selection->vfunction24( field_0x168 );
					if( mWriteData( data, field_0x168 ) == kTrue )
					{
						selection->vfunction23();
					}
					else
					{
						selection->vfunction23();
						mUnlockHardware();
						gUpdateBuffers = kFalse;
						return kFalse;
					}
				}
				else
				{
					selection->vfunction24( amountLeftToRead );
					if( mWriteData( data, amountLeftToRead ) == kTrue )
					{
						selection->vfunction23();
						amountLeftToRead = 0;
						while( selection )
						{
							if( amountLeftToRead != 0 )
								break;
							if( field_0x148 == kTrue )
							{
								if( field_0x10c == field_0x154 )
								{
									selection->vfunction20();
									while( field_0x10c > field_0x150 )
									{
										field_0x10c--;
										field_0x108 = field_0xec.vfunction7( field_0x108 );
										tSoundSelection* other = mGetSelectionToAttach();
										if( other )
											other->vfunction20();
									}
									selection = mGetSelectionToAttach();
									if( selection == NULL )
										break;
									amountLeftToRead = selection->vfunction26();
								}
								else
								{
									field_0x10c++;
									field_0x108 = field_0xec.vfunction9( field_0x108 );
									selection = mGetSelectionToAttach();
									if( selection == NULL )
										break;
									amountLeftToRead = selection->vfunction26();
								}
							}
							else if( field_0x10c != field_0x158 && mAreWeStatic() != kTrue )
							{
								field_0x10c++;
								field_0x108 = field_0xec.vfunction9( field_0x108 );
								selection = mGetSelectionToAttach();
								if( selection == NULL )
									break;
								amountLeftToRead = selection->vfunction26();
							}
							else
							{
								field_0x108 = -1;
								selection = NULL;
							}
						}
					}
					else
					{
						selection->vfunction23();
						mUnlockHardware();
						gUpdateBuffers = kFalse;
						return kFalse;
					}
				}
				if( field_0x28 == 0x10 )
					mPlayChannel();
			}
			else
			{
#line 1183
				SOUNDSAY2(( "Null ptr selection =%ld", selection ));
#line 1147
				mWriteSilence();
				if( ++field_0xb8 == 50 )
				{
#line 1188
					SOUNDSAY2(( "Null ptr Problem!" ));
#line 1153
					selection->vfunction24( amountLeftToRead );
				}
				mUnlockHardware();
				gUpdateBuffers = kFalse;
				return kTrue;
			}
		}
		if( (long)field_0x168 > 0 )
		{
			mWriteSilence();
			if( mAreWeStatic() == kTrue )
				field_0x168 = 0;
		}
		mUnlockHardware();
	}
	else
	{
		long amountOfSilence = field_0x160 - fUnknown_01e0 + field_0x168;
		if( amountOfSilence > (long)field_0x10.vfunction3() >> 2
			|| ( amountOfSilence == 0 && field_0x168 == mGetBufferLength() ) )
		{
			if( mLockHardwareForSilence() == kTrue )
			{
				mWriteSilence();
				if( mAreWeStatic() == kTrue )
					field_0x168 = 0;
				mUnlockHardware();
			}
		}
	}
	gUpdateBuffers = kFalse;
	return kTrue;
}

// tSoundChannelCommand and its selection are declared in tgSound.h; these lines are kept so line numbers stay.


































// MATCH-ONLY: the #line directives inside set the original's line numbers for
// its " -- %s %d" messages (1270, 1295), as for mLockHardware above; the others
// put this file's own numbering back. They change no code.
#line 1309
// FUNCTION: STARFLEETOP 0x006432b0
unsigned char twStandardSoundChannel::mUpdateAmountPlayed( void )
{
	if( mAreWeStatic() == kTrue )
		return kTrue;

	tSoundChannelCommand* command = mGetFirstPlayingCommand();
	if( (long)field_0x110 > 0 )
	{
		if( command == NULL )
			field_0x110 = 0;
		while( (long)field_0x110 > 0 && command )
		{
			if( command->vfunction16() == 0 )
			{
				tSoundSelection* selection = command->vfunction32();
				if( selection == NULL )
				{
#line 1270
					SOUNDSAY2(( "A selection was goofy!" ));
#line 1350
				}
				else
				{
					long amountLeft = selection->vfunction27();
					if( amountLeft >= (long)field_0x110 )
					{
						selection->vfunction25( field_0x110 );
						field_0x110 = 0;
						command = NULL;
					}
					else
					{
						selection->vfunction25( amountLeft );
						field_0x110 -= amountLeft;
						command = (tSoundChannelCommand*)field_0xec.vfunction14( command );
					}
					continue;
				}
			}
			else if( command->vfunction16() != 0x18 )
			{
#line 1295
				SOUNDSAY2(( "Big trouble -- command of this type %ld on attachment list", command->vfunction16() ));
#line 1390
				command = (tSoundChannelCommand*)field_0xec.vfunction14( command );
				continue;
			}
			command = NULL;
		}
	}
	return kTrue;
}

// MATCH-ONLY: the #line directives put this file's own numbering back, and inside set the
// original's line number for the " -- %s %d" message (1326). They change no code.
#line 1366
// FUNCTION: STARFLEETOP 0x006434e0
unsigned char twStandardSoundChannel::mCreate3DBuffer( void )
{
	if( atwSoundPart->mCreatePrimaryBuffer() != kTrue )
		return kFalse;

	if( fSoundBuffer == NULL )
	{
#line 1326
		SOUNDSAY2(( "fDSBuffer was NULL" ));
#line 1377
		return kFalse;
	}
	if( fSoundBuffer->QueryInterface( IID_IDirectSound3DBuffer, (void**)&fUnknown_01bc ) == DS_OK )
		return kTrue;
	return kFalse;
}

// MATCH-ONLY: the #line directives inside set the original's line numbers for its three
// " -- %s %d" messages (1425, 1430, 1536); the last one puts this file's own numbering back.
// FUNCTION: STARFLEETOP 0x006435b0
unsigned char twStandardSoundChannel::mDuplicateSoundChannel( long param_1, long param_2 )
{
	twStandardSoundChannel* source = (twStandardSoundChannel*)atgSoundPart->mGetSoundChannelPtrFromHandle( param_1 );
	if( source == NULL || fSoundBuffer != NULL )
		return kFalse;

	field_0x10 = *source->mGetSoundChannelFormat();
	vfunc_20( param_2 );
	fUnknown_0180.dwFlags			= source->fUnknown_0180.dwFlags;
	fUnknown_0180.dwBufferBytes		= source->fUnknown_0180.dwBufferBytes;
	fUnknown_01a4.nChannels			= source->fUnknown_01a4.nChannels;
	fUnknown_01a4.nSamplesPerSec	= source->fUnknown_01a4.nSamplesPerSec;
	fUnknown_01a4.wBitsPerSample	= source->fUnknown_01a4.wBitsPerSample;
	fUnknown_01a4.nBlockAlign		= source->fUnknown_01a4.nBlockAlign;
	fUnknown_01a4.nAvgBytesPerSec	= source->fUnknown_01a4.nAvgBytesPerSec;
	LPDIRECTSOUNDBUFFER* buffer = &fSoundBuffer;	// MATCH-ONLY: a store through a pointer keeps the next vtable load after it
	*buffer = NULL;
	if( mAreWeStreaming() == kTrue && mAreWe3D() == kTrue )
	{
#line 1425
		SOUNDSAY2(( "3D sounds can not be streaming!!!" ));
		return kFalse;
	}
	if( mAreWe3D() == kTrue && (&field_0x10)->vfunction5() > 1 )
	{
#line 1430
		SOUNDSAY2(( "3D channels must be mono!" ));
		return kFalse;
	}

	if( mAreWeStatic() )
	{
		atwSoundPart->fDirectSound->DuplicateSoundBuffer( source->mGetfDSBuffer(), &fSoundBuffer );
		field_0x168 = source->mGetAmountThatCanBeAdded();
	}

	// HYPOTHESIS: set once a streaming buffer was created, the way the selections are
	// copied below.
	unsigned char created = kFalse;
	if( mAreWeStreaming() )
	{
		HRESULT err = atwSoundPart->fDirectSound->CreateSoundBuffer( (LPDSBUFFERDESC)&fUnknown_0180, &fSoundBuffer, NULL );
		field_0x168 = (&field_0x10)->vfunction3();
		created = kTrue;
		if( err != DS_OK )
			return kFalse;
	}

	gNumBuffersCreated++;
	if( mAreWe3D() == kTrue )
	{
		if( mCreate3DBuffer() != kTrue )
		{
			mRemoveSoundChannel();
			return kFalse;
		}
	}
	else
		fUnknown_01bc = NULL;

	field_0x28 = 4;
	atgSoundPart->mAddChannelAtHandle( this, param_2 );
	tChannelAttributes* attributes = &field_0x30;
	attributes->mInit();
	attributes->mSetFrequency( (&field_0x10)->vfunction7() );
	field_0x94 = (&field_0x10)->vfunction12();
	field_0x98 = (&field_0x10)->vfunction13();

	field_0x110 = 0;
	field_0x164 = 0;
	field_0x15c = 0;
	field_0x160 = 0;
	if( fSoundBuffer && fSoundBuffer->SetCurrentPosition( 0 ) != DS_OK )
	{
		mRemoveSoundChannel();
		return kFalse;
	}

	if( BindPart( tSoundPart )->vfunction13() == 1 )
	{
		if( mDoFillSelection( 0 ) != kTrue )
		{
			mRemoveSoundChannel();
			return kFalse;
		}
		return kTrue;
	}

	if( mDoWeRetainSelections() == kTrue )
	{
		tSoundCommandList* list = source->mGetAttachmentsList();
		tSoundChannelCommand* command = (tSoundChannelCommand*)list->vfunction5();
		tSoundChannelCommand* first = command;
		while( command )
		{
			tSoundSelection* selection = command->vfunction32();
			command = (tSoundChannelCommand*)list->vfunction14( command );
			if( selection == NULL )
				first = command;
		}
		while( first )
		{
			tSoundSelection* copy = first->vfunction32()->vfunction42();
			if( created )
				copy->vfunction20();
			long handle = atgSoundPart->mCreateCommand( 0, (long)copy, 0 );
			if( handle == -1 )
			{
#line 1536
				SOUNDSAY2(( "Command troubles!" ));
			}
			else
				mAddAttachment( handle );
			first = (tSoundChannelCommand*)list->vfunction14( first );
		}
		mAutoRemoveSelections();
	}
	mAutoRemoveChannel();
	return kTrue;
}

// MATCH-ONLY: the #line directives inside set the original's line numbers for its five
// " -- %s %d" messages (1578, 1584, 1590, 1596, 1652); the last one puts this file's own numbering back.
// FUNCTION: STARFLEETOP 0x00643b30
unsigned char twStandardSoundChannel::mAddSoundChannel( tSoundChannelFormat* format, long handle )
{
	if( format == NULL )
		return kFalse;
	if( fSoundBuffer != NULL )
		return kFalse;
	LPDIRECTSOUNDBUFFER* buffer = &fSoundBuffer;

	long blockSize = format->vfunction12();
	long length = format->vfunction3();
	if( length % blockSize > 0 )
		format->vfunction2( format->vfunction3() - length % blockSize + format->vfunction12() );
	field_0x10 = *format;
	vfunc_20( handle );

	DWORD flags = 0;
	if( mAreWe3D() == kTrue && (&field_0x10)->vfunction5() > 1 )
	{
#line 1578
		SOUNDSAY2(( "3D channels must be mono!\n" ));
		return kFalse;
	}
	if( !mDoWeRetainSelections() && mAreWeLooping() == kTrue )
	{
#line 1584
		SOUNDSAY2(( "Only retained sounds can loop! -- turning off looping" ));
		(&field_0x10)->vfunction10( (&field_0x10)->vfunction11() - 4 );
	}
	if( mAreWeStreaming() == kTrue && mAreWe3D() == kTrue )
	{
#line 1590
		SOUNDSAY2(( "3D sounds can not be streaming!!!" ));
		return kFalse;
	}
	if( !mAreWeStreaming() && !mAreWeStatic() )
	{
#line 1596
		SOUNDSAY2(( "A sound must be static or streaming -- turning static on" ));
		(&field_0x10)->vfunction10( (&field_0x10)->vfunction11() + 1 );
	}

	if( !mAreWeFixedFrequency() )
		flags = DSBCAPS_CTRLFREQUENCY;
	if( mTryToLocateInHardware() == kTrue )
		flags |= DSBCAPS_LOCHARDWARE;
	else if( mTryToLocateInSoftware() == kTrue )
		flags |= DSBCAPS_LOCSOFTWARE;
	else if( mAreWeStatic() == kTrue )
		flags |= DSBCAPS_STATIC;
	if( mAreWe3D() == kTrue )
		flags |= DSBCAPS_CTRL3D;
	if( !mAreWeFixedPan() )
		flags |= DSBCAPS_CTRLPAN;
	if( !mAreWeFixedVolume() )
		flags |= DSBCAPS_CTRLVOLUME;
	if( mAreWeAGlobalSound() == kTrue )
		flags |= DSBCAPS_GLOBALFOCUS;
	fUnknown_0180.dwFlags			= flags;
	fUnknown_0180.dwBufferBytes		= (&field_0x10)->vfunction3();
	fUnknown_01a4.nChannels			= (&field_0x10)->vfunction5();
	fUnknown_01a4.nSamplesPerSec	= (&field_0x10)->vfunction7();
	fUnknown_01a4.wBitsPerSample	= (&field_0x10)->vfunction9();
	fUnknown_01a4.nBlockAlign		= (BYTE)(&field_0x10)->vfunction12();
	fUnknown_01a4.nAvgBytesPerSec	= fUnknown_01a4.nSamplesPerSec * fUnknown_01a4.nBlockAlign;

	if( mAreWe3D() == kTrue && (&field_0x10)->vfunction5() > 1 )
	{
#line 1652
		SOUNDSAY2(( "3D channels must be mono!" ));
		return kFalse;
	}

	*buffer = NULL;
	if( atwSoundPart->fDirectSound->CreateSoundBuffer( (LPDSBUFFERDESC)&fUnknown_0180, buffer, NULL ) != DS_OK )
		return kFalse;

	gNumBuffersCreated++;
	if( mAreWe3D() == kTrue )
	{
		if( mCreate3DBuffer() != kTrue )
		{
			mRemoveSoundChannel();
			return kFalse;
		}
	}
	else
		fUnknown_01bc = NULL;

	field_0x28 = 4;
	tChannelAttributes* attributes = &field_0x30;
	attributes->mInit();
	attributes->mSetFrequency( (&field_0x10)->vfunction7() );
	field_0x168 = (&field_0x10)->vfunction3();
	field_0x94 = (&field_0x10)->vfunction12();
	field_0x98 = (&field_0x10)->vfunction13();

	field_0x110 = 0;
	field_0x164 = 0;
	field_0x15c = 0;
	field_0x160 = 0;
	if( *buffer )
		(*buffer)->SetCurrentPosition( 0 );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644110
unsigned char twStandardSoundChannel::mDoFillSelection( unsigned char param_1 )
{
	field_0x15c = 0;
	if( fSoundBuffer->SetCurrentPosition( 0 ) != DS_OK )
		return kFalse;
	field_0x164 = 0;

	tSoundSelection* second = NULL;
	tSoundSelection* first = field_0x170( field_0x178, param_1, second );
	if( first == NULL )
		return kFalse;

	first->vfunction14( kFalse );
	long total = first->vfunction9();
	long firstSize = total;
	long secondSize = 0;
	void* secondData = NULL;
	if( second )
	{
		second->vfunction14( kFalse );
		secondSize = second->vfunction9();
		total += secondSize;
	}

	if( total != (&field_0x10)->vfunction3() )
	{
		AddSoundErrorDuringInterrupt( "Major Trouble!" );
#line 1735
		AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
		return kFalse;
	}

	void* data = first->vfunction22();
	if( data == NULL )
	{
		AddSoundErrorDuringInterrupt( "Major Trouble!" );
#line 1742
		AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
		return kFalse;
	}
	if( second )
	{
		secondData = second->vfunction22();
		if( secondData == NULL )
		{
			AddSoundErrorDuringInterrupt( "Major Trouble!" );
#line 1751
			AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
			first->vfunction23();
			return kFalse;
		}
	}

	LPVOID	ptr1;
	LPVOID	ptr2;
	DWORD	bytes1;
	DWORD	bytes2;
	LPDIRECTSOUNDBUFFER buffer = fSoundBuffer;
	if( buffer->Lock( 0, (&field_0x10)->vfunction3(), &ptr1, &bytes1, &ptr2, &bytes2, 0 ) == DS_OK && bytes1 == (&field_0x10)->vfunction3() )
	{
		memcpy( ptr1, data, firstSize );
		// The second block is copied at the offset of its own size, as the original does.
		if( secondData )
			memcpy( (char*)ptr1 + secondSize, secondData, secondSize );
		fSoundBuffer->Unlock( ptr1, bytes1, ptr2, bytes2 );
		first->vfunction23();
		first->vfunction14( kTrue );
		if( second )
		{
			first->vfunction14( kTrue );	// the original's: first, not second
			second->vfunction23();
		}
		return kTrue;
	}

	AddSoundErrorDuringInterrupt( "No Lock -- Major Trouble!" );
#line 1772
	AddSoundErrorDuringInterrupt( " -- %s %d\n", __FILE__, __LINE__ );
	first->vfunction23();
	first->vfunction14( kTrue );
	if( second )
	{
		second->vfunction14( kTrue );
		second->vfunction23();
	}
	return kFalse;
}
#line 1705

// FUNCTION: STARFLEETOP 0x00644380
unsigned char twStandardSoundChannel::mAddActiveSoundChannel( tSoundChannelFormat* format, long handle, tFillSelectionFunction& fill, tSelectionFunction& next, unsigned long userData )
{
	field_0x170 = fill;
	field_0x174 = next;
	field_0x178 = userData;
	if( !mAddSoundChannel( format, handle ) )
		return kFalse;
	return mDoFillSelection( kFalse );
}

// FUNCTION: STARFLEETOP 0x006443e0
unsigned char twStandardSoundChannel::mPlayChannel( void )
{
	DWORD status;
	if( fSoundBuffer == NULL )
		return kFalse;
	if( fSoundBuffer->GetStatus( &status ) != DS_OK )
	{
		if( mAutoRemoveChannel() == kTrue )
			mDone();
		return kFalse;
	}
	fUnknown_01e8 = 0;
	fUnknown_01ec = 0;
	if( status & DSBSTATUS_BUFFERLOST )
		mRestoreBuffers();
	else if( status )
		return kTrue;

	unsigned char regular = BindPart( tSoundPart )->vfunction13() == 0;
	if( !gUpdateBuffers && regular )
	{
		mLockInData();
		mUpdateBuffersAndSelections( atgSoundPart->vfunction125() );
		if( field_0x168 > ( (&field_0x10)->vfunction3() >> 2 ) )
			return kFalse;
	}
	if( fSoundBuffer->SetCurrentPosition( field_0x15c ) != DS_OK )
	{
		if( mAutoRemoveChannel() == kTrue )
			mDone();
		return kFalse;
	}
	if( regular && field_0x168 == (&field_0x10)->vfunction3() )
	{
		field_0x28 = 0x10;
		atgSoundPart->vfunction123( field_0c );
		return kTrue;
	}

	mSetPan( mGetPan() );
	mSetFrequency( mGetFrequency() );
	HRESULT err;
	if( mAreWeStreaming() == kTrue || mAreWeLooping() == kTrue )
		err = fSoundBuffer->Play( 0, 0, DSBPLAY_LOOPING );
	else
		err = fSoundBuffer->Play( 0, 0, 0 );
	if( err != DS_OK )
	{
		if( mAutoRemoveChannel() == kTrue )
			mDone();
		return kFalse;
	}
	field_0x28 = 0x20;
	atgSoundPart->vfunction123( field_0c );
	field_0x16c = 0;
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644650
unsigned char twStandardSoundChannel::mSetFrequency( long frequency )
{
	if( mAreWeFixedFrequency() == kTrue )
		return kFalse;
	if( fSoundBuffer == NULL )
		return kFalse;
	HRESULT err = fSoundBuffer->SetFrequency( (long)( atgSoundPart->vfunction75() * frequency ) );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->mSetFrequency( frequency );
	return kTrue;
}

// The SDK library names the table: 101 attenuations in hundredths of a decibel, indexed
// from -100 (silent) to 0 (full level).
extern long LinearVolumes[];

// FUNCTION: STARFLEETOP 0x006446c0
unsigned char twStandardSoundChannel::mSetPan( long pan )
{
	if( fSoundBuffer == NULL )
		return kFalse;
	long total = atgSoundPart->vfunction76() + pan;
	if( total < -100 )
		total = -100;
	else if( total > 100 )
		total = 100;
	long dsPan;
	if( total <= 0 )
		dsPan = LinearVolumes[100 + total];
	else
		dsPan = -LinearVolumes[100 - total];
	HRESULT err = fSoundBuffer->SetPan( dsPan );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction4( pan );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644750
unsigned char twStandardSoundChannel::mSetVolume( long volume )
{
	if( fSoundBuffer == NULL )
		return kFalse;
	long total = (long)( atgSoundPart->vfunction124() * volume );
	if( total < 0 )
		total = 0;
	else if( total > 100 )
		total = 100;
	HRESULT err = fSoundBuffer->SetVolume( LinearVolumes[total] );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction6( volume );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x006447c0
unsigned char twStandardSoundChannel::mStopAndClearChannel( void )
{
	if( field_0x28 == 0x20 )
	{
		if( fSoundBuffer == NULL )
			return kFalse;
		DWORD status = 0;
		if( fSoundBuffer->GetStatus( &status ) != DS_OK )
			return kFalse;
		if( status == 0 )
		{
			if( fSoundBuffer->Stop() != DS_OK )
				return kFalse;
			mAddFinishCommand( kFalse );
		}
		else
		{
			if( fSoundBuffer->Stop() != DS_OK )
				return kFalse;
			mAddFinishCommand( kTrue );
		}
		field_0x28 = 4;
	}
	else if( field_0x28 == 8 )
	{
		mAddFinishCommand( kTrue );
		field_0x28 = 4;
	}
	else if( field_0x28 == 0x10 )
	{
		mAddFinishCommand( kFalse );
		field_0x28 = 4;
	}
	mSetUpdateMode( (eSoundChannelUpdateMode)3 );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644860
unsigned char twStandardSoundChannel::mStopAndResetChannel( void )
{
	if( field_0x28 == 0x20 )
	{
		if( fSoundBuffer == NULL )
			return kFalse;
		DWORD status = 0;
		if( fSoundBuffer->GetStatus( &status ) != DS_OK )
			return kFalse;
		if( status == 0 )
		{
			if( fSoundBuffer->Stop() != DS_OK )
				return kFalse;
			mAddFinishCommand( kFalse );
		}
		else
		{
			if( fSoundBuffer->Stop() != DS_OK )
				return kFalse;
			mAddFinishCommand( kTrue );
		}
	}
	else if( field_0x28 == 8 )
	{
		mAddFinishCommand( kTrue );
	}
	else if( field_0x28 == 0x10 )
	{
		mAddFinishCommand( kFalse );
	}
	else
		return kTrue;
	field_0x28 = 4;
	mSetUpdateMode( (eSoundChannelUpdateMode)2 );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644900
unsigned char twStandardSoundChannel::mSet3DSoundPosition( tPoint3* position )
{
	if( fUnknown_01bc == NULL )
		return kFalse;
	HRESULT err = fUnknown_01bc->SetPosition( position->x, position->y, position->z, DS3D_IMMEDIATE );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction12( position );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644970
unsigned char twStandardSoundChannel::mSet3DSoundOrientation( tPoint3* orientation )
{
	if( fUnknown_01bc == NULL )
		return kFalse;
	HRESULT err = fUnknown_01bc->SetConeOrientation( orientation->x, orientation->y, orientation->z, DS3D_IMMEDIATE );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction16( orientation );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x006449c0
unsigned char twStandardSoundChannel::mSet3DSoundVelocity( tPoint3* velocity )
{
	if( fUnknown_01bc == NULL )
		return kFalse;
	HRESULT err = fUnknown_01bc->SetVelocity( velocity->x, velocity->y, velocity->z, DS3D_IMMEDIATE );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction20( velocity );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644a10
unsigned char twStandardSoundChannel::mSet3DSoundCones( float angle, long level )
{
	if( fUnknown_01bc == NULL )
		return kFalse;
	HRESULT err = fUnknown_01bc->SetConeAngles( 0, (DWORD)( angle * 2 ), DS3D_DEFERRED );
	if( err != DS_OK )
		return kFalse;
	err = fUnknown_01bc->SetConeOutsideVolume( level * -100, DS3D_IMMEDIATE );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction24( angle );
	attributes->vfunction26( level );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644aa0
unsigned char twStandardSoundChannel::mSet3DSoundReferenceDistance( double distance )
{
	if( fUnknown_01bc == NULL )
		return kFalse;
	HRESULT err = fUnknown_01bc->SetMinDistance( (float)distance, DS3D_IMMEDIATE );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction22( distance );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644af0
unsigned char twStandardSoundChannel::mSet3DSoundProfile( t3DSoundProfile* profile )
{
	if( fUnknown_01bc == NULL )
		return kFalse;
	tPoint3* position = profile->vfunction5();
	HRESULT err = fUnknown_01bc->SetPosition( position->x, position->y, position->z, DS3D_DEFERRED );
	if( err != DS_OK )
		return kFalse;
	tPoint3* orientation = profile->vfunction9();
	err = fUnknown_01bc->SetConeOrientation( orientation->x, orientation->y, orientation->z, DS3D_DEFERRED );
	if( err != DS_OK )
		return kFalse;
	tPoint3* velocity = profile->vfunction13();
	err = fUnknown_01bc->SetVelocity( velocity->x, velocity->y, velocity->z, DS3D_DEFERRED );
	if( err != DS_OK )
		return kFalse;
	err = fUnknown_01bc->SetConeAngles( (DWORD)( profile->vfunction17() * 2 ), 360, DS3D_DEFERRED );
	if( err != DS_OK )
		return kFalse;
	err = fUnknown_01bc->SetMinDistance( (float)profile->vfunction15(), DS3D_DEFERRED );
	if( err != DS_OK )
		return kFalse;
	err = fUnknown_01bc->SetConeOutsideVolume( profile->vfunction19() * -100, DS3D_IMMEDIATE );
	if( err != DS_OK )
		return kFalse;
	tChannelAttributes* attributes = &field_0x30;
	attributes->vfunction8( profile );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644c30
unsigned char twStandardSoundChannel::mRestoreBuffers( void )
{
	fSoundBuffer->Restore();
	mReset( kTrue );
	if( BindPart( tSoundPart )->vfunction13() == 1 )
		mDoFillSelection( kTrue );
	else
		mUpdateBuffersAndSelections( atgSoundPart->vfunction125() );
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00644d10
void twStandardSoundChannel::mDoAllImmediateNonInterruptCommands( void )
{
	tSoundChannelCommand* command = mGetFirstImmediateNonInterruptCommand();
	while( command && mDoCommand( command ) == kTrue )
	{
		tSoundCommandList* list = &field_0xbc;
		list->mRemoveFromList( command );
		command->mMakeAvailable();
		command = mGetFirstImmediateNonInterruptCommand();
	}
}

// MATCH-ONLY: the #line directives set the original's line numbers for its " -- %s %d" messages (2361-2369) and then put this file's own numbering back. The field names are the format strings'.
// FUNCTION: STARFLEETOP 0x00644d60
void twStandardSoundChannel::mDump( void )
{
#line 2361
	SOUNDSAY2(( "twStandardSoundChannel Dump: " ));

	SOUNDSAY2(( " fBufferDesc: dwSize=%ld,dwFlags=%ld,dwBufferBytes=%ld,dwReserved=%ld,lpwfxFormat=%ld", fUnknown_0180.dwSize, fUnknown_0180.dwFlags, fUnknown_0180.dwBufferBytes, fUnknown_0180.dwReserved, fUnknown_0180.lpwfxFormat ));
	SOUNDSAY2(( " fWaveFormat: wFormatTag=%ld; nChannels=%ld; nSamplesPerSec=%ld; nAvgBytesPerSec=%ld; nBlockAlign=%ld; wBitsPerSample=%ld; cbSize=%ld", fUnknown_01a4.wFormatTag, fUnknown_01a4.nChannels, fUnknown_01a4.nSamplesPerSec, fUnknown_01a4.nAvgBytesPerSec, fUnknown_01a4.nBlockAlign, fUnknown_01a4.wBitsPerSample, fUnknown_01a4.cbSize ));
	SOUNDSAY2(( " fDSBuffer = %ld,fDS3DBuffer=%ld", fSoundBuffer, fUnknown_01bc ));
	SOUNDSAY2(( " fStartPlayPosition = %ld,fStartPosition=%ld,fLastPlayPosition=%ld,fAmountThatCanBeAdded=%ld", field_0x15c, field_0x160, field_0x164, field_0x168 ));

	SOUNDSAY2(( " fCurrentBufPosition1 = %ld,fCurrentBufPosition2=%ld,fCurrentBufSize1=%ld,fCurrentBufSize2=%ld", fUnknown_01c0, fUnknown_01c4, fCurrentBufSize1, fUnknown_01cc ));
	SOUNDSAY2(( " fStartBufPosition1 = %ld,fStartBufPosition2=%ld,fStartBufSize1=%ld,fStartBufSize2=%ld", fUnknown_01d0, fUnknown_01d4, fUnknown_01d8, fUnknown_01dc ));
	tSoundChannel::mDump();
}
#line 2055

// FUNCTION: STARFLEETOP 0x006452a0
twSoundPart::twSoundPart( void )
{
	atwSoundPart = this;
}

// MATCH-ONLY: the #line directives inside set the original's line number for its __LINE__
// (2387) and then put this file's own numbering back. They change no code.
// FUNCTION: STARFLEETOP 0x006452c0
long twSoundPart::mDuplicateSoundChannel( long channel )
{
	if( field_0x114.field_0x4[2] != 1 )
		return -1;

#line 2387
	if( xAttemptToGainControl( __FILE__, __LINE__, 0, 0 ) == kFalse )
		return 0;

	long handle = mFindAvailableSoundChannelHandle();
	if( handle < 0 )
	{
		mReleaseControl();
		return -1;
	}

	twStandardSoundChannel* soundChannel = new twStandardSoundChannel;
	if( soundChannel == NULL )
	{
		mReleaseControl();
		return -1;
	}

	if( soundChannel->mDuplicateSoundChannel( channel, handle ) != kTrue )
	{
		mReleaseControl();
		delete soundChannel;
		return -1;
	}
	mReleaseControl();
	return handle;
}

// MATCH-ONLY: the #line directives set the original's line numbers (2419, 2436), as for mDuplicateSoundChannel above.
// FUNCTION: STARFLEETOP 0x00645510
long twSoundPart::mAddActiveSoundChannel( tSoundChannelFormat* format, tSoundChannel::tFillSelectionFunction& fill, tSoundChannel::tSelectionFunction& next, unsigned long userData )
{
	if( field_0x114.field_0x4[2] != 1 )
		return -1;

#line 2419
	if( xAttemptToGainControl( __FILE__, __LINE__, 0, 0 ) == kFalse )
		return 0;

	long handle = mFindAvailableSoundChannelHandle();
	if( handle < 0 )
	{
		mReleaseControl();
		return -1;
	}

	twStandardSoundChannel* soundChannel = new twStandardSoundChannel;
	if( soundChannel == NULL )
	{
		mReleaseControl();
		return -1;
	}

	if( soundChannel->mAddActiveSoundChannel( format, handle, fill, next, userData ) != kTrue )
	{
		mReleaseControl();
		delete soundChannel;
		return -1;
	}
	mAddChannelAtHandle( soundChannel, handle );
#line 2436
	SOUNDSAY2(( "adding active chanel at %ld", handle ));
	mReleaseControl();
	return handle;
}
long	gNumDuplicatesCreated;	// HYPOTHESIS: the SDK library's name for 0x00c64790 (one vote, from this function)
#line 2136
// FUNCTION: STARFLEETOP 0x00645770
long twSoundPart::mAddSoundChannel( tSoundChannelFormat* format )
{
	if( field_0x114.field_0x4[2] != 1 )
		return -1;

#line 2454
	if( xAttemptToGainControl( __FILE__, __LINE__, 0, 0 ) == kFalse )
		return 0;

	long handle = mFindAvailableSoundChannelHandle();
	if( handle < 0 )
	{
		mReleaseControl();
		return -1;
	}

	twStandardSoundChannel* soundChannel = new twStandardSoundChannel;
	gNumDuplicatesCreated++;
	if( soundChannel == NULL )
	{
		mReleaseControl();
		return -1;
	}

	if( soundChannel->mAddSoundChannel( format, handle ) != kTrue )
	{
		mReleaseControl();
		delete soundChannel;
		return -1;
	}
	mAddChannelAtHandle( soundChannel, handle );
	mReleaseControl();
	return handle;
}
#line 2172

// The getters below each return one DSCAPS field. Six of their addresses carry other classes'
// names in the analysis (identical bodies); the names here are the SDK library's twSoundPart
// getters, matched to the DSCAPS field each reads.
// FUNCTION: STARFLEETOP 0x00645940
LPDIRECTSOUND*	twSoundPart::mDirectSoundObject( void )
{
	return &fDirectSound;
}

// MATCH-ONLY: the #line directives set the original's line numbers (2498, 2517) and then put this file's own numbering back.
// FUNCTION: STARFLEETOP 0x00645950
void twSoundPart::mDone( void )
{
	tgSoundPart::mDone();
#line 2498
	xAttemptToGainControl( __FILE__, __LINE__, 0, 1 );
	if( fUnknown_0134 )
	{
		fUnknown_0134->Release();
		fUnknown_0134 = NULL;
	}
	if( fUnknown_0130 )
	{
		fUnknown_0130->Release();
		fUnknown_0130 = NULL;
	}
	if( fDirectSound )
	{
		fDirectSound->Release();
		fDirectSound = NULL;
	}
	SOUNDSAY(( "Finishing up: Number created=%ld,numberReleased=%ld", gNumBuffersCreated, gNumBuffersReleased ));
#line 2517
	SOUNDSAY(( " -- %s %d\n", __FILE__, __LINE__ ));
	mReleaseControl();
}
#line 2210

// FUNCTION: STARFLEETOP 0x00645a80
unsigned char	twSoundPart::mCanWeHandle16BitOutput( void )
{
	if( ( field_0x13c.dwFlags & DSCAPS_PRIMARY16BIT ) > 0 )
		return kTrue;
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00645a90
unsigned char	twSoundPart::mCanWeHandleStereoOutput( void )
{
	if( ( field_0x13c.dwFlags & DSCAPS_PRIMARYSTEREO ) > 0 )
		return kTrue;
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00645aa0
unsigned char	twSoundPart::mCanWeHandleMono8BitOutput( void )
{
	if( ( field_0x13c.dwFlags & DSCAPS_PRIMARY8BIT ) > 0 && ( field_0x13c.dwFlags & DSCAPS_PRIMARYMONO ) > 0 )
		return kTrue;
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00645ac0
long	twSoundPart::mGetMinFrequency( void )
{
	return field_0x13c.dwMinSecondarySampleRate;
}

// FUNCTION: STARFLEETOP 0x00645ad0
long	twSoundPart::mGetMaxFrequency( void )
{
	return field_0x13c.dwMaxSecondarySampleRate;
}

// FUNCTION: STARFLEETOP 0x00645ae0
long	twSoundPart::mGetMaxHwBuffers( void )
{
	return field_0x13c.dwMaxHwMixingAllBuffers;
}

// FUNCTION: STARFLEETOP 0x00645af0
long	twSoundPart::mGetMaxHwStaticBuffers( void )
{
	return field_0x13c.dwMaxHwMixingStaticBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b00
long	twSoundPart::mGetMaxHwStreamingBuffers( void )
{
	return field_0x13c.dwMaxHwMixingStreamingBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b10
long	twSoundPart::mGetMaxHw3DBuffers( void )
{
	return field_0x13c.dwMaxHw3DStaticBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b20
long	twSoundPart::mGetFreeHwBuffers( void )
{
	return field_0x13c.dwFreeHwMixingAllBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b30
long	twSoundPart::mGetFreeHwStaticBuffers( void )
{
	return field_0x13c.dwFreeHwMixingStaticBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b40
long	twSoundPart::mGetFreeHwStreamingBuffers( void )
{
	return field_0x13c.dwFreeHwMixingStreamingBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b50
long	twSoundPart::mGetFreeHw3DBuffers( void )
{
	return field_0x13c.dwFreeHw3DStaticBuffers;
}

// FUNCTION: STARFLEETOP 0x00645b60
long	twSoundPart::mGetTotalHwMemBytes( void )
{
	return field_0x13c.dwTotalHwMemBytes;
}

// FUNCTION: STARFLEETOP 0x00645b70
long	twSoundPart::mGetFreeHwMemBytes( void )
{
	return field_0x13c.dwFreeHwMemBytes;
}

// FUNCTION: STARFLEETOP 0x00645b80
long	twSoundPart::mGetMaxContigFreeHwMemBytes( void )
{
	return field_0x13c.dwMaxContigFreeHwMemBytes;
}

// FUNCTION: STARFLEETOP 0x00645b90
long	twSoundPart::mGetPlayCpuOverheadSwBuffers( void )
{
	return field_0x13c.dwPlayCpuOverheadSwBuffers;
}

// FUNCTION: STARFLEETOP 0x00645ba0
long	twSoundPart::mGetTransferToHardwareRateInKBytesPerSec( void )
{
	return field_0x13c.dwUnlockTransferRateHwBuffers;
}

// FUNCTION: STARFLEETOP 0x00645bb0
unsigned char	twSoundPart::mDetermineHardwareCaps( void )
{
	memset( &field_0x13c, 0, sizeof( field_0x13c ) );
	field_0x13c.dwSize = sizeof( field_0x13c );
	if( fDirectSound->GetCaps( &field_0x13c ) == DS_OK )
		return kTrue;
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00645be0
unsigned char	twSoundPart::mCreatePrimaryBuffer( void )
{
	if( fUnknown_0130 )
		return kTrue;
	DSBUFFERDESC7	desc = { 0 };
	memset( &desc, 0, sizeof( desc ) );
	desc.dwSize			= sizeof( desc );
	desc.dwFlags		= DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	desc.dwBufferBytes	= 0;
	if( fDirectSound->CreateSoundBuffer( (LPDSBUFFERDESC)&desc, &fUnknown_0130, NULL ) != DS_OK )
		return kFalse;
	gNumBuffersCreated++;
	if( fUnknown_0130->QueryInterface( IID_IDirectSound3DListener, (void**)&fUnknown_0134 ) != DS_OK )
		return kFalse;
	return kTrue;
}

// MATCH-ONLY: the #line directive sets the original's line number for its __LINE__ (2824), as for mDuplicateSoundChannel above.
// FUNCTION: STARFLEETOP 0x00645d90
unsigned char	twSoundPart::mSet3DListener3DPosition( tPoint3* position )
{
	if( field_0x114.field_0x4[2] != 1 )
		return kFalse;
#line 2824
	xAttemptToGainControl( __FILE__, __LINE__, 0, 1 );
	if( fUnknown_0130 == NULL )
	{
		if( mCreatePrimaryBuffer() != kTrue )
		{
			mReleaseControl();
			return kFalse;
		}
	}
	fUnknown_0134->SetPosition( position->x, position->y, position->z, DS3D_IMMEDIATE );
	t3DListenerProfile* profile = &field_0x68;
	profile->mSet3DPosition( position );
	mReleaseControl();
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00645e20
unsigned char	twSoundPart::mSet3DListenerOrientation( tPoint3* forward, tPoint3* up )
{
	if( field_0x114.field_0x4[2] != 1 )
		return kFalse;
#line 2843
	xAttemptToGainControl( __FILE__, __LINE__, 0, 1 );
	if( fUnknown_0130 == NULL )
	{
		if( mCreatePrimaryBuffer() != kTrue )
		{
			mReleaseControl();
			return kFalse;
		}
	}
	fUnknown_0134->SetOrientation( forward->x, forward->y, forward->z, up->x, up->y, up->z, DS3D_IMMEDIATE );
	t3DListenerProfile* profile = &field_0x68;
	profile->mSetForwardDirection( forward );
	profile->mSetUpDirection( up );
	mReleaseControl();
	return kTrue;
}
// MATCH-ONLY: the #line directives put this file's own numbering back and set the original's line number for its __LINE__ (2869), as for mDuplicateSoundChannel above.
#line 2401
// FUNCTION: STARFLEETOP 0x00645ed0
unsigned char	twSoundPart::mSet3DListenerVelocity( tPoint3* velocity )
{
	if( field_0x114.field_0x4[2] != 1 )
		return kFalse;
#line 2869
	xAttemptToGainControl( __FILE__, __LINE__, 0, 1 );
	if( fUnknown_0130 == NULL )
	{
		if( mCreatePrimaryBuffer() != kTrue )
		{
			mReleaseControl();
			return kFalse;
		}
	}
	fUnknown_0134->SetVelocity( velocity->x, velocity->y, velocity->z, DS3D_IMMEDIATE );
	t3DListenerProfile* profile = &field_0x68;
	profile->mSetVelocity( velocity );
	mReleaseControl();
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00645f60
unsigned char	twSoundPart::mSet3DListenerUnitScaleInMeters( double scale )
{
	if( field_0x114.field_0x4[2] != 1 )
		return kFalse;
#line 2891
	xAttemptToGainControl( __FILE__, __LINE__, 0, 1 );
	if( fUnknown_0130 == NULL )
	{
		if( mCreatePrimaryBuffer() != kTrue )
		{
			mReleaseControl();
			return kFalse;
		}
	}
	fUnknown_0134->SetDistanceFactor( (float)scale, DS3D_IMMEDIATE );
	t3DListenerProfile* profile = &field_0x68;
	profile->mSetUnitScaleInMeters( scale );
	mReleaseControl();
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x00645ff0
long	twSoundPart::mGetHardwareInformation( long info )
{
	if( field_0x114.field_0x4[2] != 0 )
		return 1;
	return -0x40;
}

// FUNCTION: STARFLEETOP 0x00646010
iTruth	twSoundPart::mInit( tInitSoundPartInfo* initInfo )
{
	if( tgSoundPart::mInit( initInfo ) != kTrue )
		return kFalse;
#line 3032
	xAttemptToGainControl( __FILE__, __LINE__, 0, 1 );
	fUnknown_0130 = NULL;
	fUnknown_0134 = NULL;
	fDirectSound = NULL;
	if( initInfo->vfunction4() == NULL || mInitDirectSound( initInfo ) == kFalse )
	{
		field_0x114.field_0x4[2] = 4;
		mReleaseControl();
		return kFalse;
	}
	switch( initInfo->vfunction6() )
	{
		case 1:
			fDirectSound->SetSpeakerConfig( DSSPEAKER_MONO );
			break;
		case 4:
			fDirectSound->SetSpeakerConfig( DSSPEAKER_QUAD );
			break;
		case 8:
			fDirectSound->SetSpeakerConfig( DSSPEAKER_SURROUND );
			break;
		case 16:
			fDirectSound->SetSpeakerConfig( DSSPEAKER_HEADPHONE );
			break;
		case 2:
			if( initInfo->vfunction7() < 7 )
				fDirectSound->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_MIN ) );
			else if( initInfo->vfunction7() < 15 )
				fDirectSound->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_NARROW ) );
			else if( initInfo->vfunction7() < 60 )
				fDirectSound->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE ) );
			else
				fDirectSound->SetSpeakerConfig( DSSPEAKER_COMBINED( DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_MAX ) );
			break;
	}
	mReleaseControl();
	return kTrue;
}
