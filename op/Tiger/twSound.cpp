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
	virtual	void	vfunction6	( void );
	virtual	void	vfunction7	( void );
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
	virtual	void	mDetermineHardwareCaps		( void );
	virtual	iTruth	mInitDirectSound			( tInitSoundPartInfo* initInfo );

	// HYPOTHESIS: the fields between tgSoundPart's and the DirectSound object are not known.
	char			fUnknown_0130[0x8];
	LPDIRECTSOUND	fDirectSound;
};

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
	virtual	void	mClear	( void );
	virtual	void	mReset	( iTruth );
	virtual	iTruth	mPauseChannel	( void );
	virtual	void	mDone	( void );
	virtual	void	mInit	( void );

	// HYPOTHESIS: the description and format the channel's buffer is created from, filled by
	// mDone; tSoundChannel ends at 0x180, so they are this class's first fields.
	DSBUFFERDESC7		fUnknown_0180;
	WAVEFORMATEX		fUnknown_01a4;
	LPDIRECTSOUNDBUFFER	fSoundBuffer;
	// HYPOTHESIS: a second DirectSound interface on the buffer, released in
	// mDone before the buffer itself; the 3D buffer by its place.
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
				vfunction104();
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
		vfunction104();
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
			vfunction104();
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
				vfunction104();
			else
				vfunction103();
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
		vfunction104();
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
					vfunction99();
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
						command = field_0xec.vfunction14( command );
					}
					continue;
				}
			}
			else if( command->vfunction16() != 0x18 )
			{
#line 1295
				SOUNDSAY2(( "Big trouble -- command of this type %ld on attachment list", command->vfunction16() ));
#line 1390
				command = field_0xec.vfunction14( command );
				continue;
			}
			command = NULL;
		}
	}
	return kTrue;
}
