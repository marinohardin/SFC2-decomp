// tgfsys.cpp: the Tiger file system session.

// The original includes the file system part headers here. The unit map has no place for them
// yet, so the declarations this unit needs from them are kept here.
#include <windows.h>
#include "Tiger/tsay.h"
#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"

// HYPOTHESIS: a date value returned by value, one signed 64-bit word, as twDir::mGetDate
// (STARFLEETOP 0x00631860) sign-extends both halves of the file time into it.
class tDate
{
public:
	__int64	f_00;
};

// HYPOTHESIS: the enumerators are not recovered.
enum eSpecialDir
{
};

// HYPOTHESIS: named by the exe through twVolume::mGetVolType's symbol; the enumerators are not
// recovered. mGetVolType maps removable to 0, fixed and RAM disks to 1, CD-ROM to 2, remote to 3.
enum eVolType
{
};


// HYPOTHESIS: a queue of 12 bytes, a vtable pointer and two words cleared by the constructor, as
// tFSSession::tFSSession stores them; the class is named by the exe, its members are not.
// HYPOTHESIS: the queue links nodes of 12 bytes, a vtable pointer, the queued item's address and
// the next node, as tQueue::pop (STARFLEETOP 0x006202c0) and tFSSession::~tFSSession read them;
// the node class and its members are not named by the exe.
template <class tType>
class tQueue
{
public:
	class fn_QueueNode
	{
	public:
		virtual ~fn_QueueNode();

		tType			*fItem;		// +0x04
		fn_QueueNode	*fNext;		// +0x08
	};

	tQueue() { f_04 = 0; f_08 = 0; }
	virtual ~tQueue();

	// HYPOTHESIS: out of line in the exe, as tSession::mAddCookie calls it.
	void push( tType &item );

	void pop()
	{
		fn_QueueNode	*node = f_04;

		f_04 = node->fNext;
		if ( f_04 == 0 )
			f_08 = 0;
		delete node;
	}

	fn_QueueNode	*f_04;	// head
	fn_QueueNode	*f_08;	// tail
};

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

namespace std
{
	template <class _Ty> struct less;
}

// HYPOTHESIS: a hash of 256 singly linked buckets keyed by the cookie's low byte; the class,
// tMapPair, iterator, operator* and operator++(int) are named by the exe. Member names, the
// bucket layout and the inline lookup and removal bodies are not recovered.
template <class tKeyType, class tValueType, class tCompare>
class tCountMap
{
public:
	class tMapPair
	{
	public:
		tMapPair( tKeyType &key, tValueType &value );
		virtual ~tMapPair();

		tKeyType	fKey;		// +0x04
		tValueType	fValue;		// +0x0c
		int32u		fCount;		// +0x68
		tMapPair	*fNext;		// +0x6c
	};

	class iterator
	{
	public:
		iterator( tMapPair *node ) { fNode = node; }

		tMapPair	&operator*();
		iterator	operator++( int );

		tMapPair	*fNode;
	};

	iterator mFind( const tKeyType &key )
	{
		iterator	it( fHead );

		for ( ; it.fNode != 0; it++ )
			if ( (*it).fKey.fCookie == key.fCookie )
				break;
		return it;
	}

	// Out of line in the exe (STARFLEETOP 0x00621b60).
	iterator	find( tKeyType &key );
	// Out of line in the exe (STARFLEETOP 0x006220e0).
	iterator	end( void );

	tValueType	&operator[]( tKeyType &key );

	// MATCH-ONLY: operator[] as tVolume::mGetSortName (STARFLEETOP 0x0061d8b0) has it expanded
	// inline, where tVolume::mGetName calls it out of line. A found pair moves to the front of
	// its bucket; a missing key gets a new pair with a default value at the front.
	tValueType &fn_0061d8b0_index( tKeyType &key )
	{
		tMapPair	*prev = 0;
		tMapPair	*pair;

		for ( pair = fHead; pair != 0; prev = pair, pair = pair->fNext )
			if ( pair->fKey == key )
				break;

		if ( pair == 0 )
		{
			tValueType	value;

			pair = new tMapPair( key, value );
			if ( fHead != 0 )
				pair->fNext = fHead;
			else
				fTail = pair;
			fHead = pair;
		}
		else if ( prev != 0 )
		{
			prev->fNext = pair->fNext;
			pair->fNext = fHead;
			fHead = pair;
			if ( fTail == pair )
				fTail = prev;
		}
		return pair->fValue;
	}

	void mRemove( const tKeyType &key )
	{
		tMapPair	*prev = 0;
		tMapPair	*pair;

		for ( pair = fHead; pair != 0; prev = pair, pair = pair->fNext )
			if ( pair->fKey.fCookie == key.fCookie )
				break;
		if ( pair == 0 )
			throw "Couldn't find the object to delete";

		if ( --pair->fCount == 0 )
		{
			tMapPair	*head = fHead;

			if ( pair == head )
				fHead = head->fNext;
			else if ( pair == fTail )
			{
				fTail = prev;
				prev->fNext = 0;
			}
			else
				prev->fNext = pair->fNext;

			delete pair;

			if ( fHead == 0 )
				fTail = 0;
		}
	}

	int32u		f_00;
	tMapPair	*fHead;		// +0x04
	tMapPair	*fTail;		// +0x08
};

// HYPOTHESIS: the 256 count maps the cookie manager hashes its cookies over, by the cookie's low
// byte; the class and mResolve are named by the exe, which calls mResolve (STARFLEETOP
// 0x00637570) out of line to pick a cookie's map. The leading word is not recovered.
template <class tKeyType, class tValueType, class tCompare>
class tValueCountMap
{
public:
	typedef tCountMap<tKeyType,tValueType,tCompare>	tMap;

	tMap	&mResolve( tKeyType &key );

	int32u	f_00;
	tMap	fMaps[256];		// +0x04
};

// HYPOTHESIS: the cookie manager hashes its cookies over 256 count maps by the cookie's low byte;
// operator[] being called on one 12-byte map (STARFLEETOP 0x00621ba0) shows the maps are the
// buckets. The leading word is not recovered.
template <class tCookieType, class tObjectType>
class tCookieManager
{
public:
	typedef tCountMap<tCookieType,tObjectType,std::less<tCookieType> >	tMap;

	tCookieType	mMake( tObjectType &object );

	void mRemove( tCookieType &cookie )
	{
		tMap::iterator it = fValues.fMaps[cookie.fCookie & 0xff].mFind( cookie );

		if ( it.fNode == 0 || it.fNode->fCount == 0 )
			throw "Use of invalid cookie to remove an item";
		fValues.fMaps[cookie.fCookie & 0xff].mRemove( cookie );
	}

	// HYPOTHESIS: whether the cookie names a live item, as tVolume::mValid (STARFLEETOP
	// 0x0061d200) has it expanded inline; the search compares the node's key directly, where
	// mResolve's goes through the iterator. Its name is not recovered. The original frame is 4
	// bytes larger than this build's; the extra slot is not recovered.
	iTruth fn_0061d200_mValid( tCookieType &cookie )
	{
		tMap::iterator it( fValues.fMaps[cookie.fCookie & 0xff].fHead );

		for ( ; it.fNode != 0; it++ )
			if ( it.fNode->fKey.fCookie == cookie.fCookie )
				break;
		return it.fNode != 0 && it.fNode->fCount != 0;
	}

	// The search is written out here rather than through mFind: tVolume::mGetName (STARFLEETOP
	// 0x0061d7a0) keeps the iterator in this function's scope, where an inlined mFind's local
	// would share its slot with the thrown string.
	tObjectType &mResolve( tCookieType &cookie )
	{
		tMap::iterator it( fValues.fMaps[cookie.fCookie & 0xff].fHead );

		for ( ; it.fNode != 0; it++ )
			if ( (*it).fKey.fCookie == cookie.fCookie )
				break;
		if ( it.fNode == 0 || it.fNode->fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues.fMaps[cookie.fCookie & 0xff][cookie];
	}

	// MATCH-ONLY: mResolve as tVolume::mGetSortName (STARFLEETOP 0x0061d8b0) has it, with the
	// search's comparison taken as a truth value and operator[] expanded inline.
	tObjectType &fn_0061d8b0_mResolve( tCookieType &cookie )
	{
		tMap::iterator it( fValues.fMaps[cookie.fCookie & 0xff].fHead );

		for ( ; it.fNode != 0; it++ )
			if ( !( (*it).fKey != cookie ) )
				break;
		if ( it.fNode == 0 || it.fNode->fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues.fMaps[cookie.fCookie & 0xff].fn_0061d8b0_index( cookie );
	}

	// MATCH-ONLY: mResolve as tFileSession::mGetAppPath (STARFLEETOP 0x0061c070) has it, calling
	// the out-of-line tCountMap::find where tFileSession::mGetNamedVolume has the search expanded
	// inline. The original has one resolve; why the two expand differently is not recovered.
	tObjectType &fn_0061c070_mResolve( tCookieType &cookie )
	{
		tMap::iterator it = fValues.fMaps[cookie.fCookie & 0xff].find( cookie );

		if ( it.fNode == 0 || (*it).fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues.fMaps[cookie.fCookie & 0xff][cookie];
	}

	// MATCH-ONLY: mResolve as tPathIterator::mAdd (STARFLEETOP 0x0061f910) has it, searching with
	// the out-of-line tCountMap::find and testing the result against the end of the map
	// tValueCountMap::mResolve picks, which also gives operator[] its map. Why it expands
	// differently from fn_0061c070_mResolve is not recovered.
	tObjectType &fn_0061f910_mResolve( tCookieType &cookie )
	{
		tValueCountMap<tCookieType,tObjectType,std::less<tCookieType> >	&values = fValues;	tMap::iterator it = values.fMaps[cookie.fCookie & 0xff].find( cookie );
		if ( it.fNode == values.mResolve( cookie ).end().fNode || (*it).fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		tMap	&map = values.mResolve( cookie );	return map[cookie];
	}

	// MATCH-ONLY: mResolve as twDir::mMakeFullPath (STARFLEETOP 0x006315d0) has it, with the
	// bucket index held in a local for the search, the comparison taken as a truth value, the
	// found iterator copied before it is tested and operator[] out of line.
	tObjectType &fn_006315d0_mResolve( tCookieType &cookie )
	{
		int32u	index = cookie.fCookie & 0xff;
		tMap::iterator it( fValues.fMaps[index].fHead );
		for ( ; it.fNode != 0; it++ )
			if ( !( (*it).fKey != cookie ) )
				break;
		tMap::iterator found = it;
		if ( found.fNode == 0 || (*found).fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		tMap	&map = fValues.fMaps[cookie.fCookie & 0xff];
		return map[cookie];
	}

	int32u											f_00;
	tValueCountMap<tCookieType,tObjectType,std::less<tCookieType> >	fValues;	// +0x04
};

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
	tFSCookiePart	*part = BindPart( tFSCookiePart );
	part->fFSCookies.f_04--;
	part->fFSCookies.fSessionCookies.mRemove( fSession );
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
// string and operator++'s result have separate slots there. The original computes the first
// bucket's address with the index's increment folded into the lea; neither is recovered.
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
// two, as in tVolume::mGetRoot; the instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e160
tPathIterator tDir::mGetPathIterator( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mGetPathIterator();
}

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two, as in tVolume::mGetRoot; the instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e280
tDir tDir::mGetNamedDir( tString &name )
{
	return BindPart( tFSCookiePart )->fFSCookies.fDirCookies.mResolve( *this ).mGetNamedDir( name );
}

// The resolve has operator[] expanded inline, as tVolume::mGetSortName (STARFLEETOP 0x0061d8b0)
// has it. As there, the original frame is 4 bytes larger and the first bucket's address has the
// index's increment folded into the lea; neither is recovered.
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

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two, as in tVolume::mGetRoot; the instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e7e0
tDate tPath::mGetDate( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mGetDate();
}

// The original holds this in ebx and the part search's index in ebp, where this build swaps the
// two, as in tVolume::mGetRoot; the instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061e900
tDir tPath::mGetParentDir( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mGetParentDir();
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

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061ed50
iTruth tPath::mDelete( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mDelete();
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

// The original frame is 4 bytes larger than this build's, as in tVolume::mGetName; the extra
// slot is not recovered. The instructions otherwise agree.
// FUNCTION: STARFLEETOP 0x0061f0c0
int32u tPath::mFree( void )
{
	return BindPart( tFSCookiePart )->fFSCookies.fPathCookies.mResolve( *this ).mFree();
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
			queue->f_0c->f_04->mProcessRequest();
			WaitForSingleObject( queue->f_18, INFINITE );
			fn_00633fe0_Node	*node = queue->f_0c;
			queue->f_0c = node->f_08;
			if ( !queue->f_0c )
				queue->f_10 = 0;
			delete node;
			ReleaseMutex( queue->f_18 );
		}
	}
	delete queue;
	return 0;
}
