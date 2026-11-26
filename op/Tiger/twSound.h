//	twSound.h: the Windows sound part's standard sound channel, shared by twSound.cpp and
//	the channel's own out-of-line members.

#ifndef twSound_h
#define twSound_h

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include "Tiger/tgSound.h"
#include "Tiger/tvectormath.h"

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

// The allocation in twSoundPart::mDuplicateSoundChannel (0x006452c0) pushes 0x1f0.
typedef char twStandardSoundChannel_size_is_0x1f0[sizeof(twStandardSoundChannel) == 0x1f0 ? 1 : -1];

#endif
