// taudiopriority.cpp: Tiger's audio priority manager (file name from the original's
// assertion strings).

#include "Tiger/taudiopriority.h"
#include "Tiger/tsndmgr.h"

// FUNCTION: STARFLEETOP 0x00661300
void tClassTrackingInfo::mSet( unsigned long classRef, unsigned long arg2, long arg3, long numHandles, long *handles )
{
	f04 = classRef;
	f08 = arg2;
	f0c = arg3;
	f20 = 0;
	for ( long i = 0; i < numHandles; i++ )
	{
		tAudioFileClass *fileClass = BindPart( tAudioPriorityManagerPart )->mGetAudioFileClassPtrFromHandle( handles[i] );
		if ( fileClass )
		{
			f10[f20++] = handles[i];
			fileClass->mAddHandle( f04 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x00661400
void tClassTrackingInfo::mSetAvailable( void )
{
	for ( long i = 0; i < f20; i++ )
	{
		tAudioFileClass *fileClass = BindPart( tAudioPriorityManagerPart )->mGetAudioFileClassPtrFromHandle( f10[i] );
		if ( fileClass )
			fileClass->mRemoveHandle( f04 );
	}
	f20 = 0;
}

// FUNCTION: STARFLEETOP 0x006614e0
long tClassTrackingInfo::mGetPriority( void )
{
	return BindPart( tBasicSoundFileManager )->mGetChannelPriority( f08, -1 );
}

// FUNCTION: STARFLEETOP 0x00661570
unsigned char tClassTrackingInfo::mAreWeValid( void )
{
	// HYPOTHESIS: the meaning of status 0x10 and the names of slots 8 and 19 are not recovered.
	if ( BindPart( tBasicSoundFileManager )->mGetStatus( f08 ) == 0x10 )
	{
		BindPart( tBasicSoundFileManager )->vfunction19( f08 );
		return false;
	}
	return BindPart( tBasicSoundFileManager )->vfunction8( f08 );
}

// FUNCTION: STARFLEETOP 0x00661700
long tClassTrackingInfo::mFadeToStop( void )
{
	return BindPart( tBasicSoundFileManager )->mFadeToStopAndResetNow( f08, 500.0, -1 );
}

// FUNCTION: STARFLEETOP 0x006617a0
tAudioFileClass *tAudioPriorityManagerPart::mGetAudioFileClassPtrFromHandle( long handle )
{
	if ( handle < 0 || handle >= f14 )
		return NULL;
	return f0c[handle];
}
