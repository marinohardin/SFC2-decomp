// taudiopriority.cpp: Tiger's audio priority manager (file name from the original's
// assertion strings).
#include "Tiger/taudiopriority.h"
#include "Tiger/tsndmgr.h"
#include "Tiger/tgSound.h"

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

// HYPOTHESIS: the reference is what slot 1 of each file class returns.
// FUNCTION: STARFLEETOP 0x006617c0
long tAudioPriorityManagerPart::mGetAudioFileClassHandleFromReference( long reference )
{
	if ( f0c )
	{
		for ( long i = 0; i < f14; i++ )
		{
			if ( f0c[i] && f0c[i]->vfunction1() == reference )
				return i;
		}
	}
	return -1;
}

// MATCH-ONLY: each location note is one expansion of the unrecovered error macro, written on
// one line under the original's line number; the #line after it sets the numbering back to
// the file's own.
// FUNCTION: STARFLEETOP 0x00661810
long tAudioPriorityManagerPart::mNewAudioFileClass( void )
{
	long oldSize = f14;
	if ( oldSize == 0 )
	{
		f14 = 8;
		f0c = new tAudioFileClass *[f14];
		if ( f0c == NULL )
		{
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Having trouble creating memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Having trouble creating memory!\n"); }
#line 170
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 101
			f14 = 0;
			return -1;
		}
		for ( long i = 0; i < f14; i++ )
			f0c[i] = NULL;
		f0c[0] = new tAudioFileClass;
		if ( f0c[0] )
		{
			f0c[0]->vfunction2( 0 );
			f10 = 1;
			return 0;
		}
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Having trouble creating memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Having trouble creating memory!\n"); }
#line 192
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 117
		delete [] f0c;
		f0c = NULL;
		f14 = 0;
		return -1;
	}
	else if ( f10 == oldSize )
	{
		f14 = oldSize + 8;
		tAudioFileClass **newList = new tAudioFileClass *[f14];
		if ( newList == NULL )
		{
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Having trouble creating memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Having trouble creating memory!\n"); }
#line 211
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 132
			f14 -= 8;
			return -1;
		}
		long i;
		for ( i = 0; i < oldSize; i++ )
			newList[i] = f0c[i];
		for ( i = oldSize; i < f14; i++ )
			newList[i] = NULL;
		tAudioFileClass **oldList = f0c;
		f0c = newList;
		delete [] oldList;
		f0c[oldSize] = new tAudioFileClass;
		if ( f0c[oldSize] )
		{
			f0c[oldSize]->vfunction2( oldSize );
			f10++;
			return oldSize;
		}
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Having trouble creating memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Having trouble creating memory!\n"); }
#line 246
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 154
		return -1;
	}
	else if ( f0c[f10] == NULL )
	{
		f0c[f10] = new tAudioFileClass;
		if ( f0c[f10] )
		{
			f0c[f10]->vfunction2( f10 );
			f10++;
			return f10 - 1;
		}
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Having trouble creating memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Having trouble creating memory!\n"); }
#line 266
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 169
		return -1;
	}
	else
	{
		for ( long i = 0; i < f10; i++ )
		{
			if ( f0c[i] == NULL )
			{
				f0c[i] = new tAudioFileClass;
				if ( f0c[i] )
				{
					f0c[i]->vfunction2( i );
					f10++;
					return i;
				}
				if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Having trouble creating memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Having trouble creating memory!\n"); }
#line 286
				if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 188
				return -1;
			}
		}
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("How the heck did we get here!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("How the heck did we get here!\n"); }
#line 292
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 195
		return -1;
	}
}

// FUNCTION: STARFLEETOP 0x00661e90
void tAudioPriorityManagerPart::mRemoveClass( long reference )
{
	long handle = mGetAudioFileClassHandleFromReference( reference );
	mReleaseAudioFileClassPtr( handle );
}

// The implicit tAudioFileClass destructor is emitted out of line for the delete below.
// FUNCTION: STARFLEETOP 0x00661f30
// FUNCTION: STARFLEETOP 0x00661eb0
unsigned char tAudioPriorityManagerPart::mReleaseAudioFileClassPtr( long handle )
{
	if ( handle < 0 || handle >= f14 )
		return 0;
	if ( f0c[handle] )
	{
		f10--;
		delete f0c[handle];
		f0c[handle] = NULL;
		if ( f10 == 0 )
		{
			f14 = 0;
			delete [] f0c;
			f0c = NULL;
		}
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x00661f60
void tAudioPriorityManagerPart::mSetAudioFileClass( long reference, long arg2, long arg3, eAudioPreemptType preempt, double arg5 )
{
	tAudioFileClass *fileClass = mGetAudioFileClassPtrFromHandle( reference );
	if ( fileClass )
		fileClass->mSet( arg2, arg3, preempt, arg5 );
}
