// tgfsys.cpp: the Tiger file system session.

// The original includes the file system part headers here. The unit map has no place for them
// yet, so the declarations this unit needs from them are kept in Tiger headers of their own.
#include <windows.h>
#include "Tiger/tsay.h"
#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"

#include "Tiger/tFSSession.h"

// tDate, eVolType, the iterator parents, tgFileSystemPart, tFSSession, twVolume, twDir, tgPath
// and twPath are declared in Tiger/tFSSession.h, as unsorted/fn_00436000.cpp needs them too;
// these lines keep the line numbers.
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

#include "Tiger/tCountMap.h"

// tCountMap, tValueCountMap and tCookieManager are declared in Tiger/tCountMap.h, as
// unsorted/fn_00436000.cpp uses them too; these lines keep the line numbers.
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


template <class tVolumeType, class tDirType, class tPathType>
class tFileSystemCookieManager
{
public:
	tFSSession	*mResolve( tSession &session );
	// Out of line in the exe (STARFLEETOP 0x006357d0), as twDir::mGetPathIterator calls it.
	tPath	mMake( tPathType &path );

	// HYPOTHESIS: each removal lowers the count tFileSession::tFileSession raises, then removes the
	// cookie from its manager, as tFSSession::~tFSSession has them; these overloads are not named
	// by the exe.
	void mRemove( tVolume &cookie )			{ f_04--; fVolumeCookies.mRemove( cookie ); }	void mRemove( tDir &cookie )			{ f_04--; fDirCookies.mRemove( cookie ); }
	void mRemove( tPath &cookie )			{ f_04--; fPathCookies.mRemove( cookie ); }
	void mRemove( tVolIterator &cookie )	{ f_04--; fVolIterCookies.mRemove( cookie ); }
	void mRemove( tDirIterator &cookie )	{ f_04--; fDirIterCookies.mRemove( cookie ); }
	void mRemove( tSession &cookie )		{ f_04--; fSessionCookies.mRemove( cookie ); }
	void mRemove( tPathIterator &cookie )	{ f_04--; fPathIterCookies.mRemove( cookie ); }

	// HYPOTHESIS: f_04 is the count tFileSession::tFileSession raises; f_00 is not recovered. The
	// managers follow the offsets tFSSession::~tFSSession removes through; the member names are
	// not recovered.
	int32u										f_00;
	int32u										f_04;
	tCookieManager<tVolume,tVolumeType>			fVolumeCookies;		// +0x08
	tCookieManager<tDir,tDirType>				fDirCookies;		// +0xc10
	tCookieManager<tPath,tPathType>				fPathCookies;		// +0x1818
	tCookieManager<tSession,tFSSession>			fSessionCookies;	// +0x2420
	tCookieManager<tVolIterator,tVolIterParent>	fVolIterCookies;	// +0x3028
	tCookieManager<tDirIterator,tDirIterParent>	fDirIterCookies;	// +0x3c30
	tCookieManager<tPathIterator,tPathIterParent>	fPathIterCookies;	// +0x4838
};

class tFSCookiePart: public tPart
{
public:
	GenAbstractBinding( tFSCookiePart );

	tFileSystemCookieManager<twVolume,twDir,twPath>	fFSCookies;		// +0x0c
};

// tFileSession is declared in Tiger/tFileSession.h; these lines keep the line numbers.
//
//
//
//
//
//
//

// FUNCTION: STARFLEETOP 0x0061bae0
tFileSession::tFileSession()
{
	tFSSession	*fsSession = new tFSSession;

	tFSCookiePart	*part = BindPart( tFSCookiePart );
	part->fFSCookies.f_04++;
	tSession	session = part->fFSCookies.fSessionCookies.mMake( *fsSession );
	fSession = session;

	BindPart( tFSCookiePart )->fFSCookies.mResolve( fSession )->mSession( fSession );

	delete fsSession;
}

// FUNCTION: STARFLEETOP 0x0061bc40
// FUNCTION: STARFLEETOP 0x0061bdd0
inline tFileSession::~tFileSession()
{
	BindPart( tFSCookiePart )->fFSCookies.mRemove( fSession );


}

// FUNCTION: STARFLEETOP 0x0061bf50
tVolume tFileSession::mGetNamedVolume( tString &name )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.mResolve( fSession );

	return fsSession.mGetNamedVolume( name );
}

// HYPOTHESIS: this lookup calls the out-of-line tCountMap::find, where mGetNamedVolume's has the
// search expanded inline; the resolve is written out here to keep both as the exe has them.
// FUNCTION: STARFLEETOP 0x0061c070
tPath tFileSession::mGetAppPath( void )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( fSession );

	return fsSession.mGetAppPath();
}

// HYPOTHESIS: like mGetAppPath, this lookup calls the out-of-line tCountMap::find.
// FUNCTION: STARFLEETOP 0x0061c170
tDir tFileSession::mGetSpecialDir( eSpecialDir dir )
{
	return BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( fSession ).mGetSpecialDir( dir );
}

// FUNCTION: STARFLEETOP 0x0061c290
tPath tFileSession::mDuplicatePath( tPath path )
{
	return BindPart( tFSCookiePart )->fFSCookies.mResolve( fSession )->mDuplicatePath( path );
}

// FUNCTION: STARFLEETOP 0x0061c320
tPath tFileSession::mFileOpenDialog( tString &prompt, tString &filter )
{
	return BindPart( tFSCookiePart )->fFSCookies.mResolve( fSession )->mFileOpenDialog( prompt, filter, tDir() );
}

// FUNCTION: STARFLEETOP 0x0061c380
tPath tFileSession::mFileSaveDialog( tString &prompt, tString &filter )
{
	return BindPart( tFSCookiePart )->fFSCookies.mResolve( fSession )->mFileSaveDialog( prompt, filter );
}

// FUNCTION: STARFLEETOP 0x0061c3e0
tFSSession::tFSSession()
{
}

// FUNCTION: STARFLEETOP 0x0061c470
tFSSession::tFSSession( const tFSSession &other )
{
	*this = other;
}

// FUNCTION: STARFLEETOP 0x0061c540
tFSSession::~tFSSession()
{
	while ( f_04.f_04 != 0 )
	{
		tVolume	&volume = *f_04.f_04->fItem;

		BindPart( tFSCookiePart )->fFSCookies.mRemove( volume );
		f_04.pop();
	}
	while ( f_10.f_04 != 0 )
	{
		tDir	&dir = *f_10.f_04->fItem;

		BindPart( tFSCookiePart )->fFSCookies.mRemove( dir );
		f_10.pop();
	}
	while ( f_1c.f_04 != 0 )
	{
		tPath	&path = *f_1c.f_04->fItem;

		BindPart( tFSCookiePart )->fFSCookies.mRemove( path );
		f_1c.pop();
	}
	while ( f_28.f_04 != 0 )
	{
		tVolIterator	&volIterator = *f_28.f_04->fItem;

		BindPart( tFSCookiePart )->fFSCookies.mRemove( volIterator );
		f_28.pop();
	}
	while ( f_34.f_04 != 0 )
	{
		tDirIterator	&dirIterator = *f_34.f_04->fItem;

		BindPart( tFSCookiePart )->fFSCookies.mRemove( dirIterator );
		f_34.pop();
	}
	while ( f_40.f_04 != 0 )
	{
		tPathIterator	&pathIterator = *f_40.f_04->fItem;

		BindPart( tFSCookiePart )->fFSCookies.mRemove( pathIterator );
		f_40.pop();
	}
}

// FUNCTION: STARFLEETOP 0x0061c9a0
tVolume tFSSession::mGetNamedVolume( tString &name )
{
	return BindPart( tgFileSystemPart )->mGetNamedVolume( fSession, name );
}

// FUNCTION: STARFLEETOP 0x0061ca20
tPath tFSSession::mGetAppPath( void )
{
	return BindPart( tgFileSystemPart )->mGetAppPath( fSession );
}

// The original keeps the two argument copies' unwind pointers in separate stack slots, a frame
// 4 bytes larger; this build shares one. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061ca60
tPath tFSSession::mDuplicatePath( tPath path )
{
	return BindPart( tgFileSystemPart )->mDuplicatePath( fSession, path );
}

// FUNCTION: STARFLEETOP 0x0061cb00
tPath tFSSession::mFileOpenDialog( tString &prompt, tString &filter, tDir dir )
{
	return BindPart( tgFileSystemPart )->mFileOpenDialog( fSession, prompt, filter, dir );
}

// FUNCTION: STARFLEETOP 0x0061cb80
tPath tFSSession::mFileSaveDialog( tString &prompt, tString &filter, tDir dir )
{
	return BindPart( tgFileSystemPart )->mFileSaveDialog( fSession, prompt, filter, dir );
}

// FUNCTION: STARFLEETOP 0x0061cc00
void tSession::mAddCookie( tVolume &volume )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( *this );

	volume.fSession = fsSession.fSession;
	fsSession.f_04.push( volume );
}

// FUNCTION: STARFLEETOP 0x0061cd00
void tSession::mAddCookie( tDir &dir )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( *this );

	dir.fSession = fsSession.fSession;
	fsSession.f_10.push( dir );
}

// FUNCTION: STARFLEETOP 0x0061ce00
void tSession::mAddCookie( tPath &path )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( *this );

	path.fSession = fsSession.fSession;
	fsSession.f_1c.push( path );
}

// FUNCTION: STARFLEETOP 0x0061cf00
void tSession::mAddCookie( tVolIterator &volIterator )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( *this );

	volIterator.fSession = fsSession.fSession;
	fsSession.f_28.push( volIterator );
}

// FUNCTION: STARFLEETOP 0x0061d000
void tSession::mAddCookie( tDirIterator &dirIterator )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( *this );

	dirIterator.fSession = fsSession.fSession;
	fsSession.f_34.push( dirIterator );
}

// FUNCTION: STARFLEETOP 0x0061d100
void tSession::mAddCookie( tPathIterator &pathIterator )
{
	tFSSession	&fsSession = BindPart( tFSCookiePart )->fFSCookies.fSessionCookies.fn_0061c070_mResolve( *this );

	pathIterator.fSession = fsSession.fSession;
	fsSession.f_40.push( pathIterator );
}

// FUNCTION: STARFLEETOP 0x0061d200
iTruth tVolume::mValid( void )
{
	return tCookie::mValid() && BindPart( tFSCookiePart )->fFSCookies.fVolumeCookies.fn_0061d200_mValid( *this );
}

// FUNCTION: STARFLEETOP 0x0061d2f0
iTruth tDir::mValid( void )
{
	return tCookie::mValid() && BindPart( tFSCookiePart )->fFSCookies.fDirCookies.fn_0061d200_mValid( *this );
}

// FUNCTION: STARFLEETOP 0x0061d3e0
iTruth tPath::mValid( void )
{
	return fCookie != tCookie::kInvalidCookie && BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_0061d200_mValid( *this );
}

// FUNCTION: STARFLEETOP 0x0061d4d0
iTruth tVolIterator::mValid( void )
{
	return tCookie::mValid() && BindPart( tFSCookiePart )->fFSCookies.fVolIterCookies.fn_0061d200_mValid( *this );
}

// FUNCTION: STARFLEETOP 0x0061d5c0
iTruth tDirIterator::mValid( void )
{
	return tCookie::mValid() && BindPart( tFSCookiePart )->fFSCookies.fDirIterCookies.fn_0061d200_mValid( *this );
}

// FUNCTION: STARFLEETOP 0x0061d6b0
iTruth tPathIterator::mValid( void )
{
	return tCookie::mValid() && BindPart( tFSCookiePart )->fFSCookies.fPathIterCookies.fn_0061d200_mValid( *this );
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mValid: an unused slot
// sits between the thrown string and operator++'s result. The slot is not recovered; the
// instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061d7a0
tString &tVolume::mGetName( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fVolumeCookies.mResolve( *this ).mGetName();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName: the thrown
// string and operator++'s result have separate slots there. The slot is not recovered; the
// instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061d8b0
tString tVolume::mGetSortName( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fVolumeCookies.fn_0061d8b0_mResolve( *this ).mGetSortName();
}

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two; the source form that gives the original's choice is not recovered. The instructions
// otherwise agree.
// FUNCTION: STARFLEETOP 0x0061dad0
tDir tVolume::mGetRoot( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fVolumeCookies.mResolve( *this ).mGetRoot();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061dbf0
tString &tDir::mGetName( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mGetName();
}

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two, as in tVolume::mGetRoot; the instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061dd00
tVolume tDir::mGetParentVol( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mGetParentVol();
}

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two, as in tVolume::mGetRoot; the instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061de20
tDir tDir::mGetParentDir( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mGetParentDir();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061df40
int32u tDir::mGetSize( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mGetSize();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e050
iTruth tDir::mExist( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mExist();
}

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two with the plain mResolve; holding the cookie's map in a local for operator[] gives its order.
// FUNCTION: STARFLEETOP 0x0061e160
tPathIterator tDir::mGetPathIterator( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.fn_0061e160_mResolve( *this ).mGetPathIterator();
}

// The resolve holds the cookie's map in a local for operator[], as tDir::mGetPathIterator
// (STARFLEETOP 0x0061e160) has it; that gives the original's registers.
// FUNCTION: STARFLEETOP 0x0061e280
tDir tDir::mGetNamedDir( tString &name )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.fn_0061e160_mResolve( *this ).mGetNamedDir( name );
}

// The resolve has operator[] expanded inline, as tVolume::mGetSortName (STARFLEETOP 0x0061d8b0)
// has it. As there, the original frame is 4 bytes larger; the extra slot is not recovered. The
// instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e3a0
tPath tDir::mGetNamedFile( tString &name )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.fn_0061d8b0_mResolve( *this ).mGetNamedFile( name );
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e5c0
iTruth tDir::mDelete( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mDelete();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e6d0
tString &tPath::mGetName( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mGetName();
}

// The resolve holds the cookie's map in a local for operator[], as tDir::mGetPathIterator
// (STARFLEETOP 0x0061e160) has it; that gives the original's registers.
// FUNCTION: STARFLEETOP 0x0061e7e0
tDate tPath::mGetDate( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_0061e160_mResolve( *this ).mGetDate();
}

// The resolve holds the cookie's map in a local for operator[], as tDir::mGetPathIterator
// (STARFLEETOP 0x0061e160) has it; that gives the original's registers.
// FUNCTION: STARFLEETOP 0x0061e900
tDir tPath::mGetParentDir( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_0061e160_mResolve( *this ).mGetParentDir();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061ea20
iTruth tPath::mExist( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mExist();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061eb30
int32u tPath::mGetSize( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mGetSize();
}

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061ec40
iTruth tPath::mCreate( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mCreate();
}

// The resolve tests the found iterator in a helper, which gives the original's four stack slots.
// FUNCTION: STARFLEETOP 0x0061ed50
iTruth tPath::mDelete( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_0061ed50_mResolve( *this ).mDelete();
}

// FUNCTION: STARFLEETOP 0x0061ee60
int32u tPath::mRead( void *buffer, int32u offset, int32u size, tPathCallback *callback )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mRead( buffer, offset, size, callback );
}

// FUNCTION: STARFLEETOP 0x0061ef90
int32u tPath::mWrite( void *buffer, int32u offset, int32u size, tPathCallback *callback )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mWrite( buffer, offset, size, callback );
}

// The resolve tests the found iterator in a helper, as in tPath::mDelete, which gives the
// original's four stack slots.
// FUNCTION: STARFLEETOP 0x0061f0c0
int32u tPath::mFree( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_0061ed50_mResolve( *this ).mFree();
}

// FUNCTION: STARFLEETOP 0x0061f1d0
int32u tVolIterator::mAdd( tVolume volume )
{
	tVolIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fVolIterCookies.fn_0061c070_mResolve( *this );

	return parent.mAdd( volume );
}

// The original stores tCookie's vtable where this build stores fn_SessionCookie's: the
// intermediate class overrides nothing, so its vtable is taken to be folded into tCookie's at
// link time. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061f2f0
tVolume tVolIterator::mFirst( void )
{
	tVolIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fVolIterCookies.fn_0061c070_mResolve( *this );

	if ( parent.fVolumes.f_04 != 0 )
	{
		parent.fCurrent = parent.fVolumes.f_04;
		return *parent.fCurrent->fItem;
	}
	return tVolume();
}

// FUNCTION: STARFLEETOP 0x0061f450
tVolume tVolIterator::mNext( tVolume volume )
{
	tVolIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fVolIterCookies.fn_0061c070_mResolve( *this );

	return parent.mNext( volume );
}

// FUNCTION: STARFLEETOP 0x0061f570
int32u tDirIterator::mAdd( tDir dir )
{
	tDirIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fDirIterCookies.fn_0061c070_mResolve( *this );

	return parent.mAdd( dir );
}

// As in tVolIterator::mFirst, the original stores tCookie's vtable where this build stores
// fn_SessionCookie's. The original's empty return also stores the invalid session cookie once more
// after the tDir vtable pointer, as tVolume's default constructor does; giving tDir that
// constructor breaks the default tDir arguments of tFileSession::mFileOpenDialog and
// mFileSaveDialog, so how this one is constructed is not recovered.
// FUNCTION: STARFLEETOP 0x0061f690
tDir tDirIterator::mFirst( void )
{
	tDirIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fDirIterCookies.fn_0061c070_mResolve( *this );

	if ( parent.fDirs.f_04 != 0 )
	{
		parent.fCurrent = parent.fDirs.f_04;
		return *parent.fCurrent->fItem;
	}
	return tDir( 0 );
}

// FUNCTION: STARFLEETOP 0x0061f7f0
tDir tDirIterator::mNext( tDir dir )
{
	tDirIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fDirIterCookies.fn_0061c070_mResolve( *this );

	return parent.mNext( dir );
}

// The original's resolve tests find's result against the end of the map tValueCountMap::mResolve
// picks, and calls operator[] on that map, as fn_0061f910_mResolve writes it; building with that
// form makes the comparison fail, so this one uses the resolve of tDirIterator::mAdd for now.
// FUNCTION: STARFLEETOP 0x0061f910
int32u tPathIterator::mAdd( tPath path )
{
	tPathIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fPathIterCookies.fn_0061c070_mResolve( *this );

	return parent.mAdd( path );
}

// FUNCTION: STARFLEETOP 0x0061fa60
tPath tPathIterator::mFirst( void )
{
	tPathIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fPathIterCookies.fn_0061c070_mResolve( *this );

	return parent.mFirst();
}

// The original resolves with fn_0061f910_mResolve's form, as tPathIterator::mAdd does; the map
// reference local there orders the resolve before operator[]'s argument push.
// FUNCTION: STARFLEETOP 0x0061fb60
tPath tPathIterator::mNext( tPath path )
{
	tPathIterParent	&parent = BindPart( tFSCookiePart )->fFSCookies.fPathIterCookies.fn_0061f910_mResolve( *this );

	return parent.mNext( path );
}

// MATCH-ONLY: the original has ~tFileSession both inlined into the scalar deleting destructor and
// emitted out of line at 0x0061bdd0. An inline destructor gets an out-of-line copy when an
// exception unwind path calls it, as it does for this local; the original caller is not recovered.
// HYPOTHESIS: some function of this unit keeps a tFileSession on the stack across a call.
void fn_use_file_session( void )
{
	tFileSession	session;

	session.mValid();
}

// FUNCTION: STARFLEETOP 0x006310c0
tString twVolume::mGetSortName( void )
{
	return f_44;
}

// HYPOTHESIS: the buffers the root is copied into for the system calls, as in twfsys.cpp; their
// names are not recovered.
extern char g_00c3caec[MAX_PATH];
extern iWideChar g_00c3cbf0[MAX_PATH];

// FUNCTION: STARFLEETOP 0x006310e0
int32u twVolume::mGetClusterSize( void )
{
	DWORD	sectorsPerCluster;
	DWORD	bytesPerSector;
	DWORD	freeClusters;
	DWORD	totalClusters;

	f_6c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	if ( !GetDiskFreeSpaceW( g_00c3cbf0, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters ) )
	{
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			f_6c.mCopyCharStr( g_00c3caec, MAX_PATH );
			if ( !GetDiskFreeSpaceA( g_00c3caec, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters ) )
				throw "Error in system getting cluster size";
		}
		else
			throw "Error in system getting cluster size";
	}
	return sectorsPerCluster * bytesPerSector;
}

// FUNCTION: STARFLEETOP 0x006311a0
int32u twVolume::mGetTotalClusters( void )
{
	DWORD	sectorsPerCluster;
	DWORD	bytesPerSector;
	DWORD	freeClusters;
	DWORD	totalClusters;

	f_6c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	if ( !GetDiskFreeSpaceW( g_00c3cbf0, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters ) )
	{
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			f_6c.mCopyCharStr( g_00c3caec, MAX_PATH );
			if ( !GetDiskFreeSpaceA( g_00c3caec, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters ) )
				throw "Error in system getting cluster size";
		}
		else
			throw "Error in system getting cluster size";
	}
	return totalClusters;
}

// FUNCTION: STARFLEETOP 0x00631260
int32u twVolume::mGetFreeClusters( void )
{
	DWORD	sectorsPerCluster;
	DWORD	bytesPerSector;
	DWORD	freeClusters;
	DWORD	totalClusters;

	f_6c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	if ( !GetDiskFreeSpaceW( g_00c3cbf0, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters ) )
	{
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			f_6c.mCopyCharStr( g_00c3caec, MAX_PATH );
			if ( !GetDiskFreeSpaceA( g_00c3caec, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters ) )
				throw "Error in system getting cluster size";
		}
		else
			throw "Error in system getting cluster size";
	}
	return freeClusters;
}

// FUNCTION: STARFLEETOP 0x00631320
eVolType twVolume::mGetVolType( void )
{
	UINT	driveType;

	f_6c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	driveType = GetDriveTypeW( g_00c3cbf0 );
	if ( ( driveType == DRIVE_UNKNOWN || driveType == DRIVE_NO_ROOT_DIR ) && GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
	{
		f_6c.mCopyCharStr( g_00c3caec, MAX_PATH );
		driveType = GetDriveTypeA( g_00c3caec );
	}
	switch ( driveType )
	{
		case DRIVE_REMOVABLE:
			return (eVolType)0;
		case DRIVE_FIXED:
		case DRIVE_RAMDISK:
			return (eVolType)1;
		case DRIVE_REMOTE:
			return (eVolType)3;
		case DRIVE_CDROM:
			return (eVolType)2;
		default:
			throw "Error in system getting vol type";
	}
}

// FUNCTION: STARFLEETOP 0x006313d0
iTruth twVolume::mIsEjectable( void )
{
	return 0;
}

// HYPOTHESIS: the original expands ~twDir inline here (three tString destructors, no vtable
// store) and copies the made cookie into the directory at +0x34 and once more through the
// out-of-line tCookie copy before returning it; that tail is not recovered. ~twDir stays out of
// line because tDir::mGetNamedFile (STARFLEETOP 0x0061e3a0) calls it so.
// FUNCTION: STARFLEETOP 0x006313e0
tDir twVolume::mGetRoot( void )
{
	tString	name( "" );
	tDir	parent;
	twDir	dir( name, f_34, parent );

	dir.f_50 = f_30;
	tFSCookiePart	*part = BindPart( tFSCookiePart );
	part->fFSCookies.f_04++;
	tDir	result = part->fFSCookies.fDirCookies.mMake( dir );
	return result;
	return part->fFSCookies.fDirCookies.mMake( dir );
}

// FUNCTION: STARFLEETOP 0x006315d0
void twDir::mMakeFullPath( void )
{
	f_8c = f_24;
	tDir	dir( f_14 );

	while ( dir.mValid() )
	{
		tString	name( dir.mGetName() );

		if ( name.mStrLen() )
		{
			name += "\\";
			name += f_8c;
			f_8c = name;
		}
		dir = dir.mGetParentDir();
	}

	tString	root( BindPart( tFSCookiePart )->fFSCookies.fVolumeCookies.fn_006315d0_mResolve( f_04 ).f_6c );

	root += f_8c;
	f_8c = root;
}

// FUNCTION: STARFLEETOP 0x00631840
tString twDir::mGetSortName( void )
{
	return f_64;
}

// HYPOTHESIS: out of line in twfsys.cpp (STARFLEETOP 0x0062ff80) and later in this unit
// (STARFLEETOP 0x00631930); a null buffer selects the shared system buffer.
iWideChar *gCopyWideStr(tString &str, iWideChar *buffer);
char *gCopyCharStr(tString &str, char *buffer);

// FUNCTION: STARFLEETOP 0x00631860
tDate twDir::mGetDate( void )
{
	tDate	date;

	date.f_00 = 0;
	if ( mExist() )
	{
		tString		&path = f_8c;
		HANDLE		file;
		FILETIME	writeTime;

		SetLastError( 0 );
		file = CreateFileW( gCopyWideStr( path, 0 ), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			if ( file )
				CloseHandle( file );
			file = CreateFileA( gCopyCharStr( path, 0 ), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
		}
		if ( GetFileTime( file, 0, 0, &writeTime ) )
		{
			date.f_00 = ( (__int64)(long)writeTime.dwHighDateTime << 32 ) | (long)writeTime.dwLowDateTime;
			CloseHandle( file );
		}
		else
			throw "Failed to get file time";
	}
	return date;
}

// FUNCTION: STARFLEETOP 0x00631930
char *gCopyCharStr(tString &str, char *buffer)
{
	if ( buffer )
	{
		str.mCopyCharStr( buffer, MAX_PATH );
		return buffer;
	}
	str.mCopyCharStr( g_00c3caec, MAX_PATH );
	return g_00c3caec;
}

// HYPOTHESIS: as in twVolume::mGetRoot, the original expands ~twDir inline and, after storing the
// made cookie at +0x54, copies it once more through the out-of-line tCookie copy into a 16-byte
// temporary; that tail is not recovered.
// FUNCTION: STARFLEETOP 0x00631b30
tDir twDir::mGetNamedDir( tString &name )
{
	if ( name == "" )
		throw "You cannot get a named object without giving a valid name!";
	twDir	dir( name, f_04, f_54 );

	dir.f_50 = f_50;
	tFSCookiePart	*part = BindPart( tFSCookiePart );
	part->fFSCookies.f_04++;
	tDir	result = part->fFSCookies.fDirCookies.mMake( dir );
	dir.f_54 = result;
	return result;
}

// HYPOTHESIS: the original builds a path for the name in this directory, makes its cookie and
// returns the cookie handed back through tPath's own cookie lookup; ~twPath is expanded inline.
// FUNCTION: STARFLEETOP 0x00631d30
tPath twDir::mGetNamedFile( tString &name )
{
	if ( name == "" )
		throw "You cannot get a named object without giving a valid name!";
	twPath	path( name, f_54 );

	path.f_40 = f_50;
	tFSCookiePart	*part = BindPart( tFSCookiePart );
	tPath	made = part->fFSCookies.fPathCookies.mMake( path );
	tPath	result = made;
	return result;
}

// FUNCTION: STARFLEETOP 0x00631f60
iTruth twDir::mExist( void )
{
	iTruth	exists = false;

	if ( f_24 == "" && !(&f_14)->mValid() && (&f_04)->mValid() )
		return true;

	struct { WIN32_FIND_DATAW w; char pad[0x1b0]; }	wide;	// MATCH-ONLY: 0x1b0 unused bytes the original's frame holds
	WIN32_FIND_DATAA	data;
	HANDLE	find;

	f_8c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	find = FindFirstFileW( g_00c3cbf0, &wide.w );
	if ( find != INVALID_HANDLE_VALUE )
	{
		if ( wide.w.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			exists = true;
	}
	else if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
	{
		f_8c.mCopyCharStr( g_00c3caec, MAX_PATH );
		find = FindFirstFileA( g_00c3caec, &data );
		if ( find != INVALID_HANDLE_VALUE )
		{
			if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				exists = true;
		}
	}
	FindClose( find );
	return exists;
}

// HYPOTHESIS: the original expands ~twDir inline in both loops, assigns the made cookie through
// the out-of-line tDir::operator= (STARFLEETOP 0x00636080), copies +0x54 once more through the
// out-of-line tDir copy (STARFLEETOP 0x00633e80) and copies cookies through the out-of-line
// tCookie copy (STARFLEETOP 0x00407c5b); the header has those inline, so they are not recovered.
// The word the directory's +0x50 is copied into is the iterator parent's +0x14.
// FUNCTION: STARFLEETOP 0x00632040
tDirIterator twDir::mGetDirIterator( void )
{
	tDirIterParent	parent;

	tFSCookiePart	*part = BindPart( tFSCookiePart );
	parent.f_10[1] = f_50;
	part->fFSCookies.f_04++;
	tDirIterator	iter = part->fFSCookies.fDirIterCookies.mMake( parent );

	tString	path( f_8c );

	// HYPOTHESIS: the wide copy whose buffer is tested before the separator check is gCopyWideStr
	// with no buffer, expanded inline.
	path.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	iWideChar	*wideName = g_00c3cbf0;
	if ( wideName && path[path.mStrLen() - 1] != '\\' )
		path += "\\";
	path += "*.*";

	WIN32_FIND_DATAA	data;
	WIN32_FIND_DATAW	wide;
	HANDLE	find;

	path.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	find = FindFirstFileW( g_00c3cbf0, &wide );
	if ( find == INVALID_HANDLE_VALUE )
	{
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path.mCopyCharStr( g_00c3caec, MAX_PATH );
			find = FindFirstFileA( g_00c3caec, &data );
			if ( find != INVALID_HANDLE_VALUE )
			{
				do
				{
					if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						tString	name( data.cFileName );

						if ( !( name == "." ) && !( name == ".." ) )
						{
							twDir	dir( name, f_04, f_54 );

							dir.f_50 = f_50;
							tFSCookiePart	*part = BindPart( tFSCookiePart );
							part->fFSCookies.f_04++;
							tDir	made = part->fFSCookies.fDirCookies.mMake( dir );
							dir.f_54 = made;
							tDir	self( dir.f_54 );
							iter.mAdd( made );
						}
					}
				} while ( FindNextFileA( find, &data ) );
			}
		}
	}
	else
	{
		do
		{
			if ( wide.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				tString	name( wide.cFileName );

				if ( !( name == "." ) && !( name == ".." ) )
				{
					twDir	dir( name, f_04, f_54 );

					dir.f_50 = f_50;
					tFSCookiePart	*part = BindPart( tFSCookiePart );
					part->fFSCookies.f_04++;
					tDir	made = part->fFSCookies.fDirCookies.mMake( dir );
					dir.f_54 = made;
					tDir	self( dir.f_54 );
					iter.mAdd( made );
				}
			}
		} while ( FindNextFileW( find, &wide ) );
	}
	FindClose( find );
	return iter;
}

// HYPOTHESIS: as in mGetDirIterator, the original copies the directory and path cookies through
// the out-of-line tCookie copy (STARFLEETOP 0x00407c5b) and keeps this in a stack slot; those
// are not recovered. The word the directory's +0x50 is copied into is the iterator parent's +0x14.
// FUNCTION: STARFLEETOP 0x00632700
tPathIterator twDir::mGetPathIterator( void )
{
	tPathIterParent	parent;

	parent.f_00[5] = f_50;
	tFSCookiePart	*part = BindPart( tFSCookiePart );
	part->fFSCookies.f_04++;
	tPathIterator	iter = part->fFSCookies.fPathIterCookies.mMake( parent );

	tString	path( f_8c );

	path.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	iWideChar	*wideName = g_00c3cbf0;
	if ( wideName && path[path.mStrLen() - 1] != '\\' )
		path += "\\";
	path += "*.*";

	WIN32_FIND_DATAA	data;
	WIN32_FIND_DATAW	wide;
	HANDLE	find;

	path.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	find = FindFirstFileW( g_00c3cbf0, &wide );
	if ( find == INVALID_HANDLE_VALUE )
	{
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path.mCopyCharStr( g_00c3caec, MAX_PATH );
			find = FindFirstFileA( g_00c3caec, &data );
			if ( find != INVALID_HANDLE_VALUE )
			{
				do
				{
					if ( !( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
					{
						tString	name( data.cFileName );
						twPath	file( name, f_54 );

						file.f_40 = f_50;
						tPath	made = BindPart( tFSCookiePart )->fFSCookies.mMake( file );
						tPath	cookie = file.mCookie( made );
						iter.mAdd( cookie );
					}
				} while ( FindNextFileA( find, &data ) );
			}
		}
	}
	else
	{
		do
		{
			if ( !( wide.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				tString	name( wide.cFileName );
				twPath	file( name, f_54 );

				file.f_40 = f_50;
				tPath	made = BindPart( tFSCookiePart )->fFSCookies.mMake( file );
				tPath	cookie = file.mCookie( made );
				iter.mAdd( cookie );
			}
		} while ( FindNextFileW( find, &wide ) );
	}
	FindClose( find );
	return iter;
}

// FUNCTION: STARFLEETOP 0x00632d30
iTruth twDir::mCreate( void )
{
	iTruth	created = false;

	if ( !mExist() )
	{
		f_8c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
		created = CreateDirectoryW( g_00c3cbf0, NULL );
		if ( !created && GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			f_8c.mCopyCharStr( g_00c3caec, MAX_PATH );
			created = CreateDirectoryA( g_00c3caec, NULL );
		}
	}
	return created;
}

// HYPOTHESIS: the original expands tSession's constructor inline in all four cookie
// constructions; this build calls it out of line in three of them while both loops are present.
// FUNCTION: STARFLEETOP 0x00632da0
iTruth twDir::mDelete( void )
{
	iTruth	deleted = false;

	if ( mExist() )
	{
		{
			tPathIterator	files = mGetPathIterator();
			tPath	file;
			for ( file = files.mFirst(); file.mValid(); file = files.mNext( file ) )
				file.mDelete();
		}
		{
			tDirIterator	dirs = mGetDirIterator();
			tDir	dir;
			for ( dir = dirs.mFirst(); dir.mValid(); dir = dirs.mNext( dir ) )
				dir.mDelete();
		}

		f_8c.mCopyWideStr( g_00c3cbf0, MAX_PATH );
		deleted = RemoveDirectoryW( g_00c3cbf0 );
		if ( !deleted && GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			f_8c.mCopyCharStr( g_00c3caec, MAX_PATH );
			deleted = RemoveDirectoryA( g_00c3caec );
		}
	}
	return deleted;
}

// FUNCTION: STARFLEETOP 0x00632fb0
void twPath::mEnsureFileClosed( void )
{
	if ( f_a8 > 0 || f_a4 != INVALID_HANDLE_VALUE )
	{
		if ( f_a4 != INVALID_HANDLE_VALUE )
			CloseHandle( f_a4 );
		f_a4 = INVALID_HANDLE_VALUE;
		f_a8 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00632ff0
void twPath::mMakeFullPath( void )
{
	f_7c = f_14;
	tDir	dir( f_04 );

	while ( dir.mValid() )
	{
		tString	name( dir.mGetName() );

		if ( name.mStrLen() )
		{
			name += "\\";
			name += f_7c;
			f_7c = name;
		}
		dir = dir.mGetParentDir();
	}

	tVolume	volume = f_04.mGetParentVol();
	tString	root( BindPart( tFSCookiePart )->fFSCookies.fVolumeCookies.fn_006315d0_mResolve( volume ).f_6c );

	root += f_7c;
	f_7c = root;
}

// FUNCTION: STARFLEETOP 0x00633260
tString twPath::mGetSortName( void )
{
	return f_54;
}

// FUNCTION: STARFLEETOP 0x00633280
iTruth twPath::mExist( void )
{
	if ( f_a8 < 1 )
	{
		HANDLE	file;
		tString	*path = &f_7c;
		SetLastError( 0 );
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		file =CreateFileW( g_00c3cbf0, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			if ( file )
				CloseHandle( file );
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			file = CreateFileA( g_00c3caec, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		}
		f_a4 = file;
		f_a8 = 1;
		if ( file == INVALID_HANDLE_VALUE )
			f_a8 = 0;
	}
	// MATCH-ONLY: the original returns setne al without clearing eax, as a bool result does.
	bool	exists = f_a4 != INVALID_HANDLE_VALUE;
	return exists;
}

// FUNCTION: STARFLEETOP 0x00633340
tDate twPath::mGetDate( void )
{
	tDate	date;

	date.f_00 = 0;
	if ( mExist() )
	{
		if ( f_a8 < 1 )
		{
			HANDLE	file;
			tString	*path = &f_7c;
			SetLastError( 0 );
			path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
			file = CreateFileW( g_00c3cbf0, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
			if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
			{
				if ( file )
					CloseHandle( file );
				file = CreateFileA( gCopyCharStr( *path, 0 ), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
			}
			f_a4 = file;
			f_a8 = 1;
			if ( file == INVALID_HANDLE_VALUE )
				f_a8 = 0;
		}
		FILETIME	writeTime;
		if ( GetFileTime( f_a4, 0, 0, &writeTime ) )
			date.f_00 = ( (__int64)(long)writeTime.dwHighDateTime << 32 ) | (long)writeTime.dwLowDateTime;
		else
			throw "Failed to get file time";
	}
	return date;
}

// FUNCTION: STARFLEETOP 0x00633440
int32u twPath::mGetSize( void )
{
	int32u	size = 0;

	if ( mExist() )
	{
		if ( f_a8 < 1 )
		{
			HANDLE	file;
			tString	*path = &f_7c;
			SetLastError( 0 );
			path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
			file = CreateFileW( g_00c3cbf0, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
			if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
			{
				if ( file )
					CloseHandle( file );
				path->mCopyCharStr( g_00c3caec, MAX_PATH );
				file = CreateFileA( g_00c3caec, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
			}
			f_a4 = file;
			f_a8 = 1;
			if ( file == INVALID_HANDLE_VALUE )
				f_a8 = 0;
		}
		DWORD	high = 0;
		DWORD	low = GetFileSize( f_a4, &high );
		if ( low == 0xffffffff && GetLastError() != NO_ERROR )
			throw "Failed to get file size";
		if ( high != 0 )
			low = 0xffffffff;
		size = low;
	}
	return size;
}

// FUNCTION: STARFLEETOP 0x00633550
iTruth twPath::mCreate( void )
{
	iTruth	created = false;

	if ( !mExist() )
	{
		if ( f_a8 < 3 )
		{
			if ( f_a8 > 0 || f_a4 != INVALID_HANDLE_VALUE )
			{
				if ( f_a4 != INVALID_HANDLE_VALUE )
					CloseHandle( f_a4 );
				f_a4 = INVALID_HANDLE_VALUE;
				f_a8 = 0;
			}
			HANDLE	file;
			tString	*path = &f_7c;
			SetLastError( 0 );
			path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
			file = CreateFileW( g_00c3cbf0, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
			if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
			{
				CloseHandle( file );
				path->mCopyCharStr( g_00c3caec, MAX_PATH );
				file = CreateFileA( g_00c3caec, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
			}
			f_a4 = file;
			f_a8 = 3;
			if ( file == INVALID_HANDLE_VALUE )
				f_a8 = 0;
		}
		created = f_a4 != INVALID_HANDLE_VALUE;
	}
	return created;
}

// FUNCTION: STARFLEETOP 0x00633660
iTruth twPath::mDelete( void )
{
	iTruth	deleted = false;

	if ( mExist() )
	{
		if ( f_a8 > 0 || f_a4 != INVALID_HANDLE_VALUE )
		{
			if ( f_a4 != INVALID_HANDLE_VALUE )
				CloseHandle( f_a4 );
			f_a4 = INVALID_HANDLE_VALUE;
			f_a8 = 0;
		}
		tString	*path = &f_7c;
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		deleted = DeleteFileW( g_00c3cbf0 );
		if ( !deleted && GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			deleted = DeleteFileA( g_00c3caec );
		}
	}
	return deleted;
}

// FUNCTION: STARFLEETOP 0x00633700
iTruth twPath::mLock( void )
{
	iTruth	locked = false;

	if ( mExist() )
	{
		SetLastError( 0 );
		tString	*path = &f_7c;
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		locked = GetFileAttributesW( g_00c3cbf0 ) & FILE_ATTRIBUTE_READONLY;
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			locked = GetFileAttributesA( g_00c3caec ) & FILE_ATTRIBUTE_READONLY;
		}
	}
	return locked;
}

// FUNCTION: STARFLEETOP 0x00633770
iTruth twPath::mLock( iTruth lock )
{
	iTruth	done = false;

	if ( mExist() )
	{
		SetLastError( 0 );
		tString	*path = &f_7c;
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		DWORD	attributes = GetFileAttributesW( g_00c3cbf0 );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			attributes = GetFileAttributesA( g_00c3caec );
		}
		if ( lock )
			attributes |= FILE_ATTRIBUTE_READONLY;
		else
			attributes &= ~FILE_ATTRIBUTE_READONLY;
		SetLastError( 0 );
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		done = SetFileAttributesW( g_00c3cbf0, attributes );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			done = SetFileAttributesA( g_00c3caec, attributes );
		}
	}
	return done;
}

// FUNCTION: STARFLEETOP 0x00633850
iTruth twPath::mHide( void )
{
	iTruth	hidden = false;

	if ( mExist() )
	{
		SetLastError( 0 );
		tString	*path = &f_7c;
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		hidden = ( GetFileAttributesW( g_00c3cbf0 ) & FILE_ATTRIBUTE_HIDDEN ) != 0;
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			hidden = ( GetFileAttributesA( g_00c3caec ) & FILE_ATTRIBUTE_HIDDEN ) != 0;
		}
	}
	return hidden;
}

// FUNCTION: STARFLEETOP 0x006338c0
iTruth twPath::mHide( iTruth hide )
{
	iTruth	done = false;

	if ( mExist() )
	{
		SetLastError( 0 );
		tString	*path = &f_7c;
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		DWORD	attributes = GetFileAttributesW( g_00c3cbf0 );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			attributes = GetFileAttributesA( g_00c3caec );
		}
		if ( hide )
			attributes |= FILE_ATTRIBUTE_HIDDEN;
		else
			attributes &= ~FILE_ATTRIBUTE_HIDDEN;
		SetLastError( 0 );
		path->mCopyWideStr( g_00c3cbf0, MAX_PATH );
		done = SetFileAttributesW( g_00c3cbf0, attributes );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			path->mCopyCharStr( g_00c3caec, MAX_PATH );
			done = SetFileAttributesA( g_00c3caec, attributes );
		}
	}
	return done;
}

#include "unsorted/twIORequest.h"

// Named by the SDK library's symbol; defined below.
void *OpenForReadChar( tString &path );

// Named by the exe's symbol.
extern twIORequestQueueManager	*gAsynchManager;

// FUNCTION: STARFLEETOP 0x006339a0
int32u twPath::mRead( void *buffer, int32u offset, int32u size, tPathCallback *callback )
{
	if ( f_a8 < 2 )
	{
		if ( f_a8 > 0 || f_a4 != INVALID_HANDLE_VALUE )
		{
			if ( f_a4 != INVALID_HANDLE_VALUE )
				CloseHandle( f_a4 );
			f_a4 = INVALID_HANDLE_VALUE;
			f_a8 = 0;
		}
		HANDLE	file;
		tString	*path = &f_7c;
		SetLastError( 0 );
		file = CreateFileW( gCopyWideStr( *path, 0 ), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			if ( file )
				CloseHandle( file );
			file = OpenForReadChar( *path );
		}
		f_a4 = file;
		f_a8 = 2;
		if ( file == INVALID_HANDLE_VALUE )
			f_a8 = 0;
	}
	if ( callback )
	{
		// HYPOTHESIS: 0 is the read request.
		gAsynchManager->mRequest( twIORequest( fn_twIORequestType_0, f_44, buffer, offset, size, callback ) );
	}
	else
	{
		if ( SetFilePointer( f_a4, offset, NULL, FILE_BEGIN ) == 0xffffffff )
			throw "Bad seek.";
		DWORD	read;
		if ( !ReadFile( f_a4, buffer, size, &read, NULL ) )
		{
			if ( GetLastError() != ERROR_HANDLE_EOF )
				throw "Error reading from file.";
		}
		else if ( read != size )
			throw "Incorrect number of bytes read from file.";
	}
	return size;
}

// FUNCTION: STARFLEETOP 0x00633bb0
void *OpenForReadChar( tString &path )
{
	path.mCopyCharStr( g_00c3caec, MAX_PATH );
	return CreateFileA( g_00c3caec, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
}

// HYPOTHESIS: inline, as twPath::mRead leaves no call to it; the copy here only sets the tPath
// member's vtable pointer.
// FUNCTION: STARFLEETOP 0x00633be0
inline twIORequest::~twIORequest()
{
}

// Named by the SDK library's symbol; defined below.
void *OpenForWriteChar( tString &path );

// FUNCTION: STARFLEETOP 0x00633bf0
int32u twPath::mWrite( void *buffer, int32u offset, int32u size, tPathCallback *callback )
{
	if ( f_a8 < 3 )
	{
		if ( f_a8 > 0 || f_a4 != INVALID_HANDLE_VALUE )
		{
			if ( f_a4 != INVALID_HANDLE_VALUE )
				CloseHandle( f_a4 );
			f_a4 = INVALID_HANDLE_VALUE;
			f_a8 = 0;
		}
		HANDLE	file;
		tString	*path = &f_7c;
		SetLastError( 0 );
		file = CreateFileW( gCopyWideStr( *path, 0 ), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
		if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
		{
			CloseHandle( file );
			file = OpenForWriteChar( *path );
		}
		f_a4 = file;
		f_a8 = 3;
		if ( file == INVALID_HANDLE_VALUE )
			f_a8 = 0;
	}
	if ( f_a4 == INVALID_HANDLE_VALUE )
	{
		if ( mLock() )
			throw "File is locked and cannot be opened for writing";
		throw "Unable to open file for write.";
	}
	if ( callback )
	{
		// HYPOTHESIS: 1 is the write request.
		gAsynchManager->mRequest( twIORequest( fn_twIORequestType_1, f_44, buffer, offset, size, callback ) );
	}
	else
	{
		SetFilePointer( f_a4, offset, NULL, FILE_BEGIN );
		DWORD	written;
		if ( !WriteFile( f_a4, buffer, size, &written, NULL ) )
			throw "Error writing to file.";
		if ( written != size )
			throw "Incorrect number of bytes written to file.";
	}
	return size;
}

// FUNCTION: STARFLEETOP 0x00633e10
void *OpenForWriteChar( tString &path )
{
	path.mCopyCharStr( g_00c3caec, MAX_PATH );
	return CreateFileA( g_00c3caec, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
}

// FUNCTION: STARFLEETOP 0x00633ec0
int32u twPath::mFree( void )
{
	if ( f_a4 != INVALID_HANDLE_VALUE )
	{
		FlushFileBuffers( f_a4 );
		if ( f_a8 > 0 || f_a4 != INVALID_HANDLE_VALUE )
		{
			if ( f_a4 != INVALID_HANDLE_VALUE )
				CloseHandle( f_a4 );
			f_a4 = INVALID_HANDLE_VALUE;
			f_a8 = 0;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00633f20
iTruth twPath::mExec( tString &args )
{
	tString	command( '"' );
	command += f_7c;
	command += "\" ";
	command += args;
	command.mCopyCharStr( g_00c3caec, MAX_PATH );
	if ( (int32s)WinExec( g_00c3caec, SW_SHOW ) < 32 )
		return false;
	return true;
}

// FUNCTION: STARFLEETOP 0x00633fe0
unsigned long AsynchIOMain( void *data )
{
	twIORequestQueue	*queue = (twIORequestQueue *)data;
	for ( ;; )
	{
		if ( queue->fn_00633fe0_IsEmpty() )
		{
			WaitForSingleObject( queue->f_18, INFINITE );
			queue->f_14 = 0;
			ReleaseMutex( queue->f_18 );
			WaitForSingleObject( queue->f_04, INFINITE );
		}
		WaitForSingleObject( queue->f_18, INFINITE );
		if ( queue->f_14 == -1 )
			break;
		ReleaseMutex( queue->f_18 );
		while ( !queue->fn_00633fe0_IsEmpty() )
		{
			queue->f_08.f_04->fItem->mProcessRequest();
			WaitForSingleObject( queue->f_18, INFINITE );
			queue->f_08.pop();
			ReleaseMutex( queue->f_18 );
		}
	}
	delete queue;
	return 0;
}

// Named by the exe's symbols; defined after mProcessRequest.
void *OpenForWrite( tString &path );
void *OpenForRead( tString &path );

template <>
twPath &tCookieManager<tPath,twPath>::fn_00636ad0_mResolve( tPath &cookie );

// FUNCTION: STARFLEETOP 0x006340b0
void twIORequest::mProcessRequest( void )
{
	BOOL	done;
	DWORD	transferred;
	try
	{
		switch ( f_00 )
		{
			case fn_twIORequestType_0:
			{
				twPath	&path = BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_006340b0_mResolve( f_04 );
				if ( path.f_a8 < 2 )
				{
					path.mEnsureFileClosed();
					path.f_a4 = OpenForRead( path.f_7c );
					path.f_a8 = 2;
					if ( path.f_a4 == INVALID_HANDLE_VALUE )
						path.f_a8 = 0;
				}
				HANDLE	file = BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_006340b0_mResolve( f_04 ).f_a4;
				SetFilePointer( file, f_18, NULL, FILE_BEGIN );
				done = ReadFile( file, f_14, f_1c, &transferred, NULL );
				break;
			}
			case fn_twIORequestType_1:
			{
				// MATCH-ONLY: the original calls mResolve out of line here (STARFLEETOP 0x00636ad0).
				twPath	&path = BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_00636ad0_mResolve( f_04 );

				if ( path.f_a8 < 3 )
				{
					path.mEnsureFileClosed();
					path.f_a4 = OpenForWrite( path.f_7c );
					path.f_a8 = 3;
					if ( path.f_a4 == INVALID_HANDLE_VALUE )
						path.f_a8 = 0;
				}
				HANDLE	file = BindPart( tFSCookiePart )->fFSCookies.fPathCookies.fn_00636ad0_mResolve( f_04 ).f_a4;
				SetFilePointer( file, f_18, NULL, FILE_BEGIN );
				done = WriteFile( file, f_14, f_1c, &transferred, NULL );
				break;
			}
		}
		if ( !done )
			throw "Error in asynchronous I/O.";
		if ( transferred != f_1c )
			throw "Incorrect number of bytes transferred.";
		if ( f_20 )
			f_20->fn_006340b0_vfunction0( f_04, 0, transferred );
	}
	catch ( ... )
	{
		if ( f_20 )
			f_20->fn_006340b0_vfunction0( f_04, 1, 0 );
	}
}

// FUNCTION: STARFLEETOP 0x006344a0
void *OpenForWrite( tString &path )
{
	HANDLE	file;
	SetLastError( 0 );
	path.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	file = CreateFileW( g_00c3cbf0, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
	if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
	{
		CloseHandle( file );
		path.mCopyCharStr( g_00c3caec, MAX_PATH );
		file = CreateFileA( g_00c3caec, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
	}
	return file;
}

// FUNCTION: STARFLEETOP 0x00634520
void *OpenForRead( tString &path )
{
	HANDLE	file;
	SetLastError( 0 );
	path.mCopyWideStr( g_00c3cbf0, MAX_PATH );
	file = CreateFileW( g_00c3cbf0, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED )
	{
		if ( file )
			CloseHandle( file );
		path.mCopyCharStr( g_00c3caec, MAX_PATH );
		file = CreateFileA( g_00c3caec, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	}
	return file;
}

template <>
twPath &tCookieManager<tPath,twPath>::fn_00636ad0_mResolve( tPath &cookie )
{
	if ( fValues.count( cookie ) == 0 )
		throw "Use of invalid cookie to resolve an item";
	return fValues[cookie];
}
