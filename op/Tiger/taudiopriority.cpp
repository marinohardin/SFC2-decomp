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

// FUNCTION: STARFLEETOP 0x00661fa0
unsigned char tAudioPriorityManagerPart::mAreWeAllowedToPlay( unsigned long classRef, long arg2, long arg3, long numHandles, long *handles, unsigned long &handle, double &spaceTime )
{
	unsigned long index = f18.mFirstItemIndex();
	tClassTrackingInfo *info = f18.mGetItemAtThisIndex( index );
	while ( info )
	{
		if ( !info->mAreWeValid() )
		{
			info->mSetAvailable();
			f18.mRemoveItemAtThisIndex( index );
		}
		index = f18.mGetNextItemIndex( index );
		info = f18.mGetItemAtThisIndex( index );
	}

	spaceTime = 0;
	double space = 0;
	long i;
	for ( i = 0; i < numHandles; i++ )
	{
		tAudioFileClass *fileClass = BindPart( tAudioPriorityManagerPart )->mGetAudioFileClassPtrFromHandle( handles[i] );
		if ( fileClass )
		{
			space = 0;
			if ( atgSoundPart->vfunction125() < fileClass->f38 )
				return false;
			if ( !fileClass->mDoWeMeetSpaceConstraints( arg3, arg2, space ) )
				return false;
			if ( space > spaceTime )
				spaceTime = space;
		}
	}

	long available = mGetNextAvailableClassTrackingInfo();
	if ( available == -1 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("Could not get an available class tracking info for priority"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("Could not get an available class tracking info for priority"); }
#line 379
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 277
		return false;
	}

	index = f18.mAddItem( &f38[available] );
	info = &f38[available];
	for ( i = 0; i < numHandles; i++ )
	{
		tAudioFileClass *fileClass = BindPart( tAudioPriorityManagerPart )->mGetAudioFileClassPtrFromHandle( handles[i] );
		if ( fileClass )
		{
			double nextStart = atgSoundPart->vfunction125() + space;
			nextStart += fileClass->mGetAtmostOneCanPlayPerThisNumberOfMicroseconds();
			fileClass->mSetEarliestNextStartTime( nextStart );
		}
	}
	info->mSet( index, classRef, arg2, numHandles, handles );
	if ( info->vfunction4() == kTrue )
	{
		f18.mDropItem( index );
		return false;
	}
	handle = index;
	return true;
}

// FUNCTION: STARFLEETOP 0x006624b0
void tAudioPriorityManagerPart::mWeHaveStoppedPlaying( unsigned long handle )
{
	tClassTrackingInfo *info = f18.mPeekItem( handle );
	if ( info )
	{
		info->mSetAvailable();
		f18.mClearItem( handle );
	}
}

// FUNCTION: STARFLEETOP 0x00662400
long tAudioPriorityManagerPart::mGetNextAvailableClassTrackingInfo( void )
{
	if ( f2438 >= 256 )
		f2438 = 0;
	long start = f2438;
	if ( f38[f2438].vfunction4() == kTrue )
		return f2438++;
	if ( ++f2438 >= 256 )
		f2438 = 0;
	while ( f2438 != start )
	{
		if ( f38[f2438].vfunction4() == kTrue )
			return f2438++;
		if ( ++f2438 >= 256 )
			f2438 = 0;
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00662510
iTruth tAudioPriorityManagerPart::mIsReady( void )
{
	if ( TestPart( tSoundPart ) == kTrue )
		return kTrue;
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00662520
void tAudioPriorityManagerPart::mActivate( void )
{
	f0c = NULL;
	f10 = 0;
	f14 = 0;
}

// FUNCTION: STARFLEETOP 0x00662530
void tAudioPriorityManagerPart::mDeactivate( void )
{
	mClose();
}

// FUNCTION: STARFLEETOP 0x00662540
void tAudioPriorityManagerPart::mClose( void )
{
	for ( long i = 0; i < f14; i++ )
	{
		if ( f0c[i] )
			mReleaseAudioFileClassPtr( i );
	}
	f18.mDeleteAll();
}

// FUNCTION: STARFLEETOP 0x006626c0
long tAudioPriorityManagerPart::mNewAudioFileClass( long reference, long arg2, eAudioPreemptType preempt, double arg4 )
{
	long handle = mGetAudioFileClassHandleFromReference( reference );
	if ( handle != -1 )
	{
		mSetAudioFileClass( handle, reference, arg2, preempt, arg4 );
		return handle;
	}
	handle = mNewAudioFileClass();
	if ( handle != -1 )
		mSetAudioFileClass( handle, reference, arg2, preempt, arg4 );
	return handle;
}

// FUNCTION: STARFLEETOP 0x00662730
unsigned char tAudioPriorityManagerPart::mIsHigherPriority( long first, long second, eAudioPreemptType preempt )
{
	// HYPOTHESIS: the enumerators for 4 and 2 are not recovered.
	if ( preempt == 4 )
		return false;
	if ( preempt == 2 )
	{
		if ( first >= second )
			return true;
		return false;
	}
	if ( first > second )
		return true;
	return false;
}

// FUNCTION: STARFLEETOP 0x00662770
unsigned char tAudioPriorityManagerPart::mIsFirstHigherPriorityThanSecond( unsigned long first, unsigned long second )
{
	tClassTrackingInfo *firstInfo = vfunction16( first );
	tClassTrackingInfo *secondInfo = vfunction16( second );
	if ( !firstInfo )
		return false;
	if ( !secondInfo )
		return false;
	return firstInfo->mGetPriority() >= secondInfo->mGetPriority();
}

// FUNCTION: STARFLEETOP 0x006627d0
void tHandleList::mInit( long size )
{
	if ( f0c )
	{
		delete [] f0c;
		f0c = NULL;
	}
	f08 = size;
	if ( size > 0 )
	{
		f0c = new long[ size ];
		if ( !f0c )
		{
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("trouble with memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("trouble with memory!\n"); }
#line 660
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 429
		}
	}
	f04 = 0;
}

// FUNCTION: STARFLEETOP 0x006628b0
tHandleList::tHandleList( long size )
{
	f0c = NULL;
	f08 = size;
	if ( size > 0 )
	{
		f0c = new long[ size ];
		if ( !f0c )
		{
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("trouble with memory!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("trouble with memory!\n"); }
#line 682
			if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 448
		}
	}
	f04 = 0;
}

// FUNCTION: STARFLEETOP 0x00662980
unsigned long tHandleList::mGetFirstHandle( void )
{
	if ( f04 == 0 )
		return 0;
	return f0c[ 0 ];
}

// FUNCTION: STARFLEETOP 0x00662990
unsigned long tHandleList::mGetThisHandle( long index )
{
	if ( index < 0 || index >= f04 )
		return 0;
	return f0c[ index ];
}

// FUNCTION: STARFLEETOP 0x006629b0
unsigned long tHandleList::mGetLastHandle( void )
{
	if ( f04 == 0 )
		return 0;
	return f0c[ f04 - 1 ];
}

// FUNCTION: STARFLEETOP 0x006629c0
unsigned char tHandleList::mAddToFrontOfList( unsigned long handle )
{
	if ( f04 == f08 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("We reached the maximum for a list!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("We reached the maximum for a list!\n"); }
#line 727
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 483
		return false;
	}
	if ( mIsHandle( handle ) == true )
		return false;
	for ( long i = f04 - 1; i >= 0; i-- )
		f0c[ i + 1 ] = f0c[ i ];
	f0c[ 0 ] = handle;
	f04++;
	return true;
}

// FUNCTION: STARFLEETOP 0x00662aa0
unsigned char tHandleList::mAddToBackOfList( unsigned long handle )
{
	if ( f04 == f08 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("We reached the maximum for a list!\n"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("We reached the maximum for a list!\n"); }
#line 760
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 506
		return false;
	}
	if ( mIsHandle( handle ) == true )
		return false;
	f0c[ f04 ] = handle;
	f04++;
	return true;
}

// FUNCTION: STARFLEETOP 0x00662b70
unsigned char tHandleList::mRemoveFromList( unsigned long handle )
{
	long position = mGetZeroIndexedHandlePosition( handle );
	if ( position == -1 )
		return false;
	for ( long i = position + 1; i < f04; i++ )
		f0c[ i - 1 ] = f0c[ i ];
	f04--;
	return true;
}

// FUNCTION: STARFLEETOP 0x00662bc0
unsigned char tHandleList::mIsHandle( unsigned long handle )
{
	for ( long i = 0; i < f04; i++ )
		if ( f0c[ i ] == handle )
			return true;
	return false;
}
