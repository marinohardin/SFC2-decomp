// taudiopriority.cpp: Tiger's audio priority manager (file name from the original's
// assertion strings).

#include "Tiger/taudiopriority.h"

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
