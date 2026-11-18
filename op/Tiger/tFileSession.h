//	tFileSession.h: tFileSession, the application's file session, and the special directories it
//	looks up. tgfsys.cpp defines the class and tFile.cpp uses it.
//
//	HYPOTHESIS: the original's header for the class is not recovered; this one holds the single
//	declaration both units need.

#ifndef tFileSession_h
#define tFileSession_h

#include "Tiger/tgfsys.h"
#include "Tiger/tstring.h"

// HYPOTHESIS: the enumerators are not recovered.
enum eSpecialDir
{
};

class tFileSession
{
public:
	tFileSession();
	virtual ~tFileSession();

	virtual iTruth	mValid( void );
	virtual void	mInvalidate( void );

	tVolume		mGetNamedVolume( tString &name );
	tPath		mGetAppPath( void );
	tDir		mGetSpecialDir( eSpecialDir dir );
	tPath		mDuplicatePath( tPath path );
	// HYPOTHESIS: the argument names are not recovered.
	tPath		mFileOpenDialog( tString &prompt, tString &filter );
	// HYPOTHESIS: the argument names are not recovered.
	tPath		mFileSaveDialog( tString &prompt, tString &filter );

	// HYPOTHESIS: +0x04 is not written by the constructor; its meaning is not recovered.
	int32u		f_04;
	tSession	fSession;
};

#endif
