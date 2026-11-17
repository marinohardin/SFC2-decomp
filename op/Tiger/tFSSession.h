//	tFSSession.h: the file system session and the objects its cookies resolve to: the iterator
//	parents, the file system part, and the platform volume, directory and path. tgfsys.cpp defines
//	them and unsorted/fn_00436000.cpp needs them complete for its cookie maps.
//
//	HYPOTHESIS: the original's headers for these classes are not recovered; this one holds the
//	single declaration both units need.

#ifndef tFSSession_h
#define tFSSession_h

#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"

// HYPOTHESIS: a date value returned by value, one signed 64-bit word, as twDir::mGetDate
// (STARFLEETOP 0x00631860) sign-extends both halves of the file time into it.
class tDate
{
public:
	__int64	f_00;
};

// tFileSession and eSpecialDir are declared in their own header, as tFile.cpp uses them too.
#include "Tiger/tFileSession.h"

// HYPOTHESIS: named by the exe through twVolume::mGetVolType's symbol; the enumerators are not
// recovered. mGetVolType maps removable to 0, fixed and RAM disks to 1, CD-ROM to 2, remote to 3.
enum eVolType
{
};


#include "unsorted/tQueue.h"

// HYPOTHESIS: the objects the iterator cookies resolve to; their layouts are not recovered.
// HYPOTHESIS: 0x28 bytes, as tVolIterator::mValid reads the map pair's count at +0x3c.
class tVolIterParent
{
public:
	int32u	mAdd( tVolume volume );
	// HYPOTHESIS: out of line, as tVolIterator::mNext (STARFLEETOP 0x0061f450) calls it with the
	// volume passed by value and returns its result.
	tVolume	mNext( tVolume volume );

	// HYPOTHESIS: the volumes iterated, a queue at +0x00, and the node last returned at +0x0c, as
	// tVolIterator::mFirst (STARFLEETOP 0x0061f2f0) sets it to the queue's head. The member names
	// and the remaining words are not recovered.
	tQueue<tVolume>					fVolumes;	// +0x00
	tQueue<tVolume>::fn_QueueNode	*fCurrent;	// +0x0c
	int32u							f_10[6];
};

// HYPOTHESIS: 0x28 bytes, as tDirIterator::mValid reads the map pair's count at +0x3c.
class tDirIterParent
{
public:
	// Out of line in the exe (STARFLEETOP 0x0061ba80, 0x00634f20).
	tDirIterParent();
	~tDirIterParent();

	int32u	mAdd( tDir dir );
	// HYPOTHESIS: out of line, as tDirIterator::mNext (STARFLEETOP 0x0061f7f0) calls it with the
	// directory passed by value and returns its result, like tVolIterParent::mNext.
	tDir	mNext( tDir dir );

	// HYPOTHESIS: the directories iterated, a queue at +0x00, and the node last returned at +0x0c,
	// as tDirIterator::mFirst (STARFLEETOP 0x0061f690) sets it to the queue's head, like
	// tVolIterParent. The member names and the remaining words are not recovered.
	tQueue<tDir>				fDirs;		// +0x00
	tQueue<tDir>::fn_QueueNode	*fCurrent;	// +0x0c
	int32u						f_10[6];
};

// HYPOTHESIS: 0x28 bytes, as tPathIterator::mValid reads the map pair's count at +0x3c.
class tPathIterParent
{
public:
	// Out of line in the exe (STARFLEETOP 0x0061bab0, 0x00634f50).
	tPathIterParent();
	~tPathIterParent();

	int32u	mAdd( tPath path );
	// HYPOTHESIS: out of line, as tPathIterator::mFirst (STARFLEETOP 0x0061fa60) calls it on the
	// resolved parent and returns its result, where the volume and directory iterators have it
	// expanded inline.
	tPath	mFirst( void );
	// HYPOTHESIS: out of line, as tPathIterator::mNext (STARFLEETOP 0x0061fb60) calls it with the
	// path passed by value and returns its result, like tVolIterParent::mNext.
	tPath	mNext( tPath path );

	int32u	f_00[10];
};

// HYPOTHESIS: the slots before mGetSpecialDir (vtable +0x24) are not recovered; these three are
// file system part methods named by the exe, placed to fill slots 6 to 8.
class tgFileSystemPart: public tPart
{
public:
	GenAbstractBinding( tgFileSystemPart );

	virtual tVolIterator	mGetVolIterator( tSession session ) = 0;
	virtual tVolume			mGetNamedVolume( tSession session, tString name ) = 0;
	virtual tPath			mGetAppPath( tSession session ) = 0;
	virtual tDir			mGetSpecialDir( tSession session, eSpecialDir dir ) = 0;
	// HYPOTHESIS: slot 10 (vtable +0x28), as tFSSession::mDuplicatePath calls it.
	virtual tPath			mDuplicatePath( tSession session, tPath path ) = 0;
	// HYPOTHESIS: slot 11 (vtable +0x2c), as tFSSession::mFileOpenDialog calls it.
	virtual tPath			mFileOpenDialog( tSession session, tString &prompt, tString &filter,
								tDir dir ) = 0;
	// HYPOTHESIS: slot 12 (vtable +0x30), as tFSSession::mFileSaveDialog calls it.
	virtual tPath			mFileSaveDialog( tSession session, tString &prompt, tString &filter,
								tDir dir ) = 0;
};

class tFSSession
{
	friend class tFileSession;
	friend class tSession;

public:
	tFSSession();
	tFSSession( const tFSSession &other );
	virtual ~tFSSession();

	tVolume	mGetNamedVolume( tString &name );
	tPath	mGetAppPath( void );
	tPath	mDuplicatePath( tPath path );
	tPath	mFileOpenDialog( tString &prompt, tString &filter, tDir dir );
	tPath	mFileSaveDialog( tString &prompt, tString &filter, tDir dir = tDir() );
	tDir	mGetSpecialDir( eSpecialDir dir )
	{
		return BindPart( tgFileSystemPart )->mGetSpecialDir( fSession, dir );
	}

protected:
	void	mSession( tSession session );

	// HYPOTHESIS: size 0x5c from the allocation in tFileSession::tFileSession; the session at
	// +0x4c is the one tFileSession::mGetSpecialDir passes to the file system part. The queue
	// types follow the vtables tFSSession::tFSSession stores; the one at +0x10 is not named by the
	// exe and is taken to hold directories.
	tQueue<tVolume>			f_04;
	tQueue<tDir>			f_10;
	tQueue<tPath>			f_1c;
	tQueue<tVolIterator>	f_28;
	tQueue<tDirIterator>	f_34;
	tQueue<tPathIterator>	f_40;
	tSession	fSession;	// +0x4c
	tSession	f_54;
};

// HYPOTHESIS: the platform volume, directory and path objects; their layouts are not recovered.
// HYPOTHESIS: 0x94 bytes, as tVolume::mValid reads the volume map pair's count at +0xa8.
class twVolume
{
public:
	// HYPOTHESIS: the first virtual, as tVolume::mGetName (STARFLEETOP 0x0061d7a0) calls it
	// through the vtable's first slot on the resolved volume.
	virtual tString	&mGetName( void );
	// HYPOTHESIS: the second virtual, as tVolume::mGetSortName calls it.
	virtual tString	mGetSortName( void );
	// HYPOTHESIS: the volume queries the exe names for twVolume, in address order; the slot
	// after them is not recovered.
	virtual int32u	mGetClusterSize( void );
	virtual int32u	mGetTotalClusters( void );
	virtual int32u	mGetFreeClusters( void );
	virtual eVolType	mGetVolType( void );
	virtual iTruth	mIsEjectable( void );
	virtual void	fn_twVolume_slot7( void );
	// HYPOTHESIS: the ninth virtual (vtable +0x20), as tVolume::mGetRoot (STARFLEETOP 0x0061dad0)
	// calls it through that slot on the resolved volume.
	virtual tDir	mGetRoot( void );

	twVolume();
	~twVolume();

	// HYPOTHESIS: the volume's own cookie at +0x34, which mGetRoot passes to the root directory as
	// its parent volume; the members before it are not recovered.
	tString	f_04;
	int32u	f_2c;
	int32u	f_30;
	tVolume	f_34;
	// HYPOTHESIS: the sort name copied by mGetSortName from +0x44; later members not recovered.
	tString	f_44;
	// HYPOTHESIS: the volume's root path, which mGetClusterSize hands to the system.
	tString	f_6c;
};

// HYPOTHESIS: 0xb4 bytes, as tDir::mValid reads the directory map pair's count at +0xc8.
class twDir
{
public:
	// HYPOTHESIS: the first virtual, as tDir::mGetName (STARFLEETOP 0x0061dbf0) calls it through
	// the vtable's first slot on the resolved directory.
	virtual tString	&mGetName( void );
	// HYPOTHESIS: the second virtual, as twVolume has its mGetSortName there.
	virtual tString	mGetSortName( void );
	// HYPOTHESIS: the third virtual (vtable +0x08), as tDir::mGetParentVol (STARFLEETOP
	// 0x0061dd00) calls it through that slot on the resolved directory.
	virtual tVolume	mGetParentVol( void );
	// HYPOTHESIS: the fourth virtual (vtable +0x0c), as tDir::mGetParentDir (STARFLEETOP
	// 0x0061de20) calls it through that slot on the resolved directory.
	virtual tDir	mGetParentDir( void );
	// HYPOTHESIS: the fifth slot, by the exe's symbol for twDir::mGetDate (STARFLEETOP 0x00631860).
	virtual tDate	mGetDate( void );
	// HYPOTHESIS: the sixth virtual (vtable +0x14), as tDir::mGetSize (STARFLEETOP 0x0061df40)
	// calls it through that slot on the resolved directory.
	virtual int32u	mGetSize( void );
	// HYPOTHESIS: the seventh virtual (vtable +0x18), as tDir::mExist (STARFLEETOP 0x0061e050)
	// calls it through that slot on the resolved directory.
	virtual iTruth	mExist( void );
	// The eighth slot, by the exe's symbol for twDir::mGetDirIterator (STARFLEETOP 0x00632040).
	virtual tDirIterator	mGetDirIterator( void );
	// HYPOTHESIS: the ninth virtual (vtable +0x20), as tDir::mGetPathIterator (STARFLEETOP
	// 0x0061e160) calls it through that slot on the resolved directory.
	virtual tPathIterator	mGetPathIterator( void );
	// HYPOTHESIS: the tenth virtual (vtable +0x24), as tDir::mGetNamedDir (STARFLEETOP
	// 0x0061e280) calls it through that slot on the resolved directory.
	virtual tDir	mGetNamedDir( tString &name );
	// HYPOTHESIS: the eleventh virtual (vtable +0x28), as tDir::mGetNamedFile (STARFLEETOP
	// 0x0061e3a0) calls it through that slot on the resolved directory.
	virtual tPath	mGetNamedFile( tString &name );
	// The twelfth slot, by the exe's symbol for twDir::mCreate (STARFLEETOP 0x00632d30).
	virtual iTruth	mCreate( void );
	// HYPOTHESIS: the thirteenth virtual (vtable +0x30), as tDir::mDelete (STARFLEETOP 0x0061e5c0)
	// calls it through that slot on the resolved directory.
	virtual iTruth	mDelete( void );

	// HYPOTHESIS: out of line, as tDir::mGetNamedFile calls both on the default value its inline
	// map lookup builds.
	twDir();
	// HYPOTHESIS: the name, the parent volume and the parent directory, as twVolume::mGetRoot
	// (STARFLEETOP 0x006313e0) passes them; the argument names are not recovered.
	twDir( const tString &name, tVolume volume, tDir parent );
	~twDir();


protected:
	void	mMakeFullPath( void );

public:
	// HYPOTHESIS: the parent volume at +0x04 and the parent directory at +0x14, as
	// twDir::mMakeFullPath (STARFLEETOP 0x006315d0) reads them; the name at +0x24 and the full
	// path built from it at +0x8c. The words at +0x4c and +0x50 and the strings at +0x64 are not
	// recovered.
	tVolume	f_04;
	tDir	f_14;
	tString	f_24;
	int32u	f_4c;
	int32u	f_50;
	tDir	f_54;
	tString	f_64;
	tString	f_8c;
};

// HYPOTHESIS: the platform-independent path twPath derives from, named by the exe's RTTI; its
// virtuals and members are not recovered, so twPath declares the slots itself. mCookie
// (STARFLEETOP 0x00634ea0) is named by the exe's symbol.
class tgPath
{
public:
	tPath	mCookie( tPath &cookie );
};

// HYPOTHESIS: 0xac bytes, as tPath::mValid reads the path map pair's count at +0xc0.
class twPath: public tgPath
{
public:
	// HYPOTHESIS: the first virtual, as tPath::mGetName (STARFLEETOP 0x0061e6d0) calls it through
	// the vtable's first slot on the resolved path.
	virtual tString	&mGetName( void );
	// The second slot, by the exe's symbol for twPath::mGetSortName (STARFLEETOP 0x00633260).
	virtual tString	mGetSortName( void );
	// HYPOTHESIS: the third virtual (vtable +0x08), as tPath::mGetParentDir (STARFLEETOP
	// 0x0061e900) calls it through that slot on the resolved path.
	virtual tDir	mGetParentDir( void );
	// HYPOTHESIS: the fourth virtual (vtable +0x0c), as tPath::mGetDate (STARFLEETOP 0x0061e7e0)
	// calls it through that slot on the resolved path.
	virtual tDate	mGetDate( void );
	// HYPOTHESIS: the fifth virtual (vtable +0x10), as tPath::mExist (STARFLEETOP 0x0061ea20)
	// calls it through that slot on the resolved path.
	virtual iTruth	mExist( void );
	// HYPOTHESIS: the sixth virtual (vtable +0x14), as tPath::mGetSize (STARFLEETOP 0x0061eb30)
	// calls it through that slot on the resolved path.
	virtual int32u	mGetSize( void );
	// HYPOTHESIS: the seventh virtual (vtable +0x18), as tPath::mCreate (STARFLEETOP 0x0061ec40)
	// calls it through that slot on the resolved path.
	virtual iTruth	mCreate( void );
	// HYPOTHESIS: the eighth virtual (vtable +0x1c), as tPath::mDelete (STARFLEETOP 0x0061ed50)
	// calls it through that slot on the resolved path.
	virtual iTruth	mDelete( void );
	// HYPOTHESIS: the ninth virtual (vtable +0x20), as its body follows mDelete's; the name is
	// the exe's symbol for STARFLEETOP 0x00633700, and the tenth (+0x24) is the exe's symbol for
	// STARFLEETOP 0x00633770, the eleventh (+0x28) for 0x00633850 and the twelfth (+0x2c) for 0x006338c0.
	virtual iTruth	mLock( void );
	virtual iTruth	mLock( iTruth lock );
	virtual iTruth	mHide( void );
	virtual iTruth	mHide( iTruth hide );
	// HYPOTHESIS: the thirteenth virtual (vtable +0x30), as tPath::mRead (STARFLEETOP 0x0061ee60)
	// calls it through that slot on the resolved path.
	virtual int32u	mRead( void *buffer, int32u offset, int32u size, tPathCallback *callback );
	// HYPOTHESIS: the fourteenth virtual (vtable +0x34), as tPath::mWrite (STARFLEETOP 0x0061ef90)
	// calls it through that slot on the resolved path.
	virtual int32u	mWrite( void *buffer, int32u offset, int32u size, tPathCallback *callback );
	// HYPOTHESIS: the fifteenth virtual (vtable +0x38), as tPath::mFree (STARFLEETOP 0x0061f0c0)
	// calls it through that slot on the resolved path.
	virtual int32u	mFree( void );
	// HYPOTHESIS: the sixteenth virtual, as its body follows mFree's; named by the exe's symbol.
	virtual iTruth	mExec( tString &args );

	// HYPOTHESIS: out of line, as twDir::mGetNamedFile (STARFLEETOP 0x00631d30) calls it with the
	// name and the directory the path lies in; ~twPath is expanded inline there.
	twPath( const tString &name, tDir dir );
	// HYPOTHESIS: inline, as twDir::mGetPathIterator (STARFLEETOP 0x00632700) has it expanded:
	// the twPath vtable store, the call to mFree and the members' destructors.
	~twPath() { mFree(); }

	// HYPOTHESIS: protected, by the exe's symbol, with twIORequest::mProcessRequest (STARFLEETOP
	// 0x006340b0) calling it as a friend.
	friend class twIORequest;
protected:
	void	mEnsureFileClosed( void );
	void	mMakeFullPath( void );

public:

	// HYPOTHESIS: the directory the path lies in at +0x04, the name at +0x14 and the directory's
	// word copied into +0x40; the cookie at +0x44 and the strings at +0x54 and +0x7c, the file
	// handle at +0xa4 and its count at +0xa8 are read by the inline destructor only.
	tDir	f_04;
	tString	f_14;
	int32u	f_3c;
	int32u	f_40;
	tPath	f_44;
	tString	f_54;
	tString	f_7c;
	void	*f_a4;
	int32	f_a8;
};

#endif
