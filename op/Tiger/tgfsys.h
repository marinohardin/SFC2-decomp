//	tgfsys.h: the Tiger file system's cookies: the session, the volume, directory and path
//	cookies, and their iterators, each a cookie value with the session it belongs to.
//
//	Declarations follow the layouts the exe's functions pin: tSession is a vtable pointer and
//	the cookie word, passed by value as eight bytes; tCookie is a cookie word with its session.

#ifndef tgfsys_h
#define tgfsys_h

#pragma once

#include "Tiger/tbasic.h"

class tString;
class tDate;
class tVolume;
class tDir;
class tPath;
class tVolIterator;
class tDirIterator;
class tPathIterator;
class tPathCallback;

// HYPOTHESIS: the vtable pointer and the cookie word that tSession and tCookie share, as a base
// of both; the exe has no vtable or name for it. The SDK library names mValid for tSession and
// for tCookie apart, so the original may have had no such base; it stays because the calls the
// original makes to the out-of-line cookie assignment (STARFLEETOP 0x00636080) come out only
// with the assignment declared a level below tCookie.
class fn_CookieValue
{
public:
	enum { kInvalidCookie = 0xFFFFFFFF };

	fn_CookieValue() {}
	fn_CookieValue( int32u cookie ) { fCookie = cookie; }

	fn_CookieValue &operator=( const fn_CookieValue &other ) { fCookie = other.fCookie; return *this; }

	virtual iTruth	mValid( void )		{ return fCookie != kInvalidCookie; }
	virtual void	mInvalidate( void )	{ fCookie = kInvalidCookie; }
	// HYPOTHESIS: the third virtual (vtable +0x08), as tVolume::mGetSortName (STARFLEETOP
	// 0x0061d8b0) calls it on a map pair's key with the cookie searched for, and takes a true
	// result as a match.
	virtual iTruth	operator==( const fn_CookieValue &other ) const;
	// HYPOTHESIS: an inline comparison, as tVolume::mGetSortName's search takes the cookies'
	// difference as a truth value before testing it.
	iTruth	operator!=( const fn_CookieValue &other ) const	{ return fCookie != other.fCookie; }

	// HYPOTHESIS: the cookie value; its original name is not recovered.
	int32u	fCookie;
};

class tSession: public fn_CookieValue
{
public:
	// HYPOTHESIS: the cookie is set in these bodies, after the vtable pointer, as the original
	// stores show.
	tSession() { mInvalidate(); }
	tSession( const tSession &other ) { *this = other; }
	// HYPOTHESIS: declared in the class: with the compiler's implicit assignment,
	// tFSSession::mDuplicatePath (STARFLEETOP 0x0061ca60) calls it out of line where the original
	// copies the session cookie inline.
	tSession &operator=( const tSession &other ) { fCookie = other.fCookie; return *this; }

	void	mAddCookie( tVolume &volume );
	void	mAddCookie( tDir &dir );
	void	mAddCookie( tPath &path );
	void	mAddCookie( tVolIterator &volIterator );
	void	mAddCookie( tDirIterator &dirIterator );
	void	mAddCookie( tPathIterator &pathIterator );
};

// A cookie paired with the session it belongs to, 16 bytes: the original's out-of-line tCookie
// copy (STARFLEETOP 0x00407c5b) copies both, and tFile's path accessor (STARFLEETOP 0x00757ce0)
// stores tCookie's vtable pointer after the session is built and before the cookie is set, as
// this class's own constructor does.
class tCookie: public fn_CookieValue
{
public:
	tCookie() { mInvalidate(); }
	// HYPOTHESIS: the copy takes the cookie and the session, and the derived copies assign the
	// session again: the copy tVolIterator::mFirst (STARFLEETOP 0x0061f2f0) returns stores the
	// session cookie both before and after the tVolume vtable pointer. Argument copies elsewhere
	// show the second store only.
	tCookie( const tCookie &other ) { fCookie = other.fCookie; fSession = other.fSession; }

	tSession	fSession;	// +0x08
};

// HYPOTHESIS: a directory cookie with its session, 16 bytes as tFileSession::mFileOpenDialog's
// default argument shows: a cookie and a session, both invalid when default constructed.
class tDir: public tCookie
{
public:
	tDir() {}
	// HYPOTHESIS: a constructor taking the session; tDirIterator::mFirst (STARFLEETOP 0x0061f690)
	// returns an empty tDir that stores the invalid session after the vtable pointer, while the
	// default arguments of tFSSession::mFileOpenDialog and mFileSaveDialog do not.
	tDir( int ) { fSession = tSession(); }
	// HYPOTHESIS: the copy assigns the session in its body, as the argument copy in
	// tFSSession::mFileOpenDialog stores the session cookie after the tDir vtable pointer.
	tDir( const tDir &other );

	virtual iTruth	mValid( void );

	tString	&mGetName( void );
	tVolume	mGetParentVol( void );
	tDir	mGetParentDir( void );
	int32u	mGetSize( void );
	iTruth	mExist( void );
	tPathIterator	mGetPathIterator( void );
	tDir	mGetNamedDir( tString &name );
	tPath	mGetNamedFile( tString &name );
	iTruth	mDelete( void );
};

// HYPOTHESIS: inline, as most callers expand it; the exe's out-of-line copy comes from a caller
// that does not (twDir::mGetDirIterator, STARFLEETOP 0x00632040), not yet recovered.
// FUNCTION: STARFLEETOP 0x00633e80
inline tDir::tDir( const tDir &other ): tCookie( other ) { fSession = other.fSession; }

// HYPOTHESIS: a path cookie with its session like tDir, 16 bytes as tFileSession::mDuplicatePath's
// argument copy shows; twFileSystemPart::mGetAppPath (STARFLEETOP 0x0062e9f0) stores the invalid
// cookies of its local before the tPath vtable pointer, as a base constructor does.
class tPath: public tCookie
{
public:
	tPath() {}
	// HYPOTHESIS: the copy assigns the session in its body, like tDir's, as the original stores the
	// copied session cookie both before and after the tPath vtable pointer.
	tPath( const tPath &other ): tCookie( other ) { fSession = other.fSession; }
	virtual ~tPath() {}

	virtual iTruth	mValid( void );

	tString	&mGetName( void );
	tDate	mGetDate( void );
	tDir	mGetParentDir( void );
	iTruth	mExist( void );
	int32u	mGetSize( void );
	iTruth	mCreate( void );
	iTruth	mDelete( void );
	int32u	mRead( void *buffer, int32u offset, int32u size, tPathCallback *callback );
	int32u	mWrite( void *buffer, int32u offset, int32u size, tPathCallback *callback );
	int32u	mFree( void );
};

// HYPOTHESIS: a volume cookie with its session, like tDir: tSession::mAddCookie sets the session
// cookie at +0x0c.
class tVolume: public tCookie
{
public:
	// HYPOTHESIS: the body invalidates the session again, as tVolIterator::mFirst (STARFLEETOP
	// 0x0061f2f0) stores the invalid session cookie once more after the tVolume vtable pointer.
	tVolume() { fSession = tSession(); }
	// HYPOTHESIS: the copy assigns the session in its body, like tDir's, as the argument copy in
	// tVolIterator::mAdd (STARFLEETOP 0x0061f1d0) stores the session cookie after the tVolume
	// vtable pointer.
	tVolume( const tVolume &other ): tCookie( other ) { fSession = other.fSession; }

	virtual iTruth	mValid( void );

	tString	&mGetName( void );
	tString	mGetSortName( void );
	tDir	mGetRoot( void );
};

// HYPOTHESIS: a volume iterator cookie with its session, like tVolume: tSession::mAddCookie sets
// the session cookie at +0x0c.
class tVolIterator: public tCookie
{
public:
	tVolIterator() {}
	// HYPOTHESIS: the copy assigns the session in its body, like tDir's, as the original stores the
	// copied session cookie both before and after the tVolIterator vtable pointer.
	tVolIterator( const tVolIterator &other );

	virtual iTruth	mValid( void );

	int32u	mAdd( tVolume volume );
	tVolume	mFirst( void );
	tVolume	mNext( tVolume volume );
};

// HYPOTHESIS: iterator cookies, like tSession; the cookie managers key on them as on the others.
// Their bodies are not recovered.

// HYPOTHESIS: a directory iterator cookie with its session, like tVolIterator:
// tSession::mAddCookie sets the session cookie at +0x0c.
class tDirIterator: public tCookie
{
public:
	virtual iTruth	mValid( void );

	int32u	mAdd( tDir dir );
	tDir	mFirst( void );
	tDir	mNext( tDir dir );
};

// HYPOTHESIS: a path iterator cookie with its session, like tDirIterator.
class tPathIterator: public tCookie
{
public:
	virtual iTruth	mValid( void );

	int32u	mAdd( tPath path );
	tPath	mFirst( void );
	tPath	mNext( tPath path );
};

// HYPOTHESIS: twIORequest::mProcessRequest (STARFLEETOP 0x006340b0) calls the first virtual
// with the path by value, a zero and the bytes transferred; the names are not recovered.
class tPathCallback
{
public:
	virtual void	fn_006340b0_vfunction0( tPath path, int32u arg, int32u size );
};

#endif
