#include "Tiger/tgfsys.h"
#include "Tiger/tstring.h"
#include <stdlib.h>

// HYPOTHESIS: tParsePath is declared here only for mGetPath; the class is not recovered.
class tParsePath
{
public:
	tPath mGetPath( tString path, tDir dir );
protected:
	// HYPOTHESIS: the string at +0x00 is compared with each path part (the parent directory
	// name); the one at +0x28 is the path separator. Their original names are not recovered.
	tString f00;
	tString f28;
	void mGetPathOrDir( tPath* path, tDir* dir, tString name, tDir base );
};

// HYPOTHESIS: the shared parser object at 0x00c80868.
extern tParsePath g_00c80868;

#include "Tiger/tFile.h"

// FUNCTION: STARFLEETOP 0x00757250
tPath tFile::gGetPath( tString path, tDir dir )
{
	return g_00c80868.mGetPath( path, dir );
}

// tFile::mInit (STARFLEETOP 0x00757310) comes next in the original. It is written at the end of
// this file, after the tFileSession header it needs, so that the functions below keep
// their line numbers. It builds the application, Assets, Sounds, Art, Movies and Q3
// directories from tFileSession::mGetAppPath, keeps the ones that exist in the search vector
// and sets fInitialized.
//
//

// The SDK library names the function at this address ??0tFile@@QAE@XZ.
// FUNCTION: STARFLEETOP 0x00757630
tFile::tFile()
{
}

// FUNCTION: STARFLEETOP 0x00757650
// FUNCTION: STARFLEETOP 0x00757670
inline tFile::~tFile()
{
	tPath* path = &fPath;
	if ( path->mValid() )
	{
		path->mFree();
	}
}

// FUNCTION: STARFLEETOP 0x007576d0
iTruth tFile::mIsOpen( void )
{
	tPath* path = &fPath;
	return path->mValid() && path->mExist();
}

// FUNCTION: STARFLEETOP 0x00757700
unsigned char tFile::mOpenForRead( const char* name, unsigned char flag )
{
	return mOpenForRead( tString( name ), flag );
}

unsigned char tFile::fInitialized;

// FUNCTION: STARFLEETOP 0x00757770
unsigned char tFile::mOpenForRead( tString& name, unsigned char flag )
{
	if ( !fInitialized )
	{
		mInit();
	}
	fPath = mSearchOpen( name, 0 );
	if ( fPath.mValid() )
	{
		fOpened = 1;
		return fPath.mExist();
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x007577d0
unsigned char tFile::mOpenForRead( tPath path, unsigned char flag )
{
	if ( !fInitialized )
	{
		mInit();
	}
	if ( path.mValid() )
	{
		fPath = mSearchOpen( path, 0 );
		if ( fPath.mValid() )
		{
			fOpened = 1;
			return fPath.mExist();
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00757bb0
unsigned char tFile::mClose( void )
{
	if ( fPath.mValid() )
	{
		fPath.mFree();
		fUnknown[1] = 0;
		fUnknown[0] = 0;
		return 1;
	}
	return 0;
}

// HYPOTHESIS: fUnknown[0] is the read position and fUnknown[1] the size, as mClose clears both.
// FUNCTION: STARFLEETOP 0x00757be0
long tFile::mRead( void* buffer, long size )
{
	if ( !fPath.mValid() || size < 0 )
	{
		return -1;
	}
	if ( (int32u)size > fUnknown[1] - fUnknown[0] )
	{
		size = fUnknown[1] - fUnknown[0];
	}
	long got = fPath.mRead( buffer, fUnknown[0], size, 0 );
	fUnknown[0] += got;
	return got;
}

// FUNCTION: STARFLEETOP 0x00757c30
unsigned char tFile::mSeekCur( long offset )
{
	if ( fPath.mValid() )
	{
		if ( (long)( fUnknown[1] - fUnknown[0] ) >= offset && (long)fUnknown[0] >= -offset )
		{
			fUnknown[0] += offset;
			return 1;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00757c70
unsigned char tFile::mSeekSet( long offset )
{
	if ( !fPath.mValid() || offset < 0 || (int32u)offset > fUnknown[1] )
	{
		return 0;
	}
	fUnknown[0] = offset;
	return 1;
}

// FUNCTION: STARFLEETOP 0x00757ca0
unsigned char tFile::mSeekEnd( long offset )
{
	if ( !fPath.mValid() || offset > 0 || (int32u)-offset > fUnknown[1] )
	{
		return 0;
	}
	fUnknown[0] = fUnknown[1] + offset;
	return 1;
}

// FUNCTION: STARFLEETOP 0x00757ce0
tPath tFile::fn_slot28( void )
{
	return fPath;
}

// FUNCTION: STARFLEETOP 0x00757d20
long tFile::mFilePos( void )
{
	if ( fPath.mValid() )
	{
		return fUnknown[0];
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00757d40
long tFile::mGetLength( void )
{
	if ( fPath.mValid() )
	{
		return fUnknown[1];
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00757d60
unsigned char tFile::mEOF( void )
{
	return fUnknown[0] == (int32u)mGetLength();
}

// HYPOTHESIS: the search directories, a vector of tDir at 0x00c808bc (begin) and 0x00c808c0 (end).
extern tDir* g_00c808bc;
extern tDir* g_00c808c0;

// HYPOTHESIS: an object at 0x00c80924 whose first virtual accepts a name.
class tObject_00c80924
{
public:
	virtual unsigned char	fn_slot0( tString& name );
	// HYPOTHESIS: the second virtual takes a path by value, as mSearchOpen( tPath ) calls it.
	virtual unsigned char	fn_slot1( tPath path );
};
extern tObject_00c80924* g_00c80924;

// FUNCTION: STARFLEETOP 0x00757880
tPath tFile::mSearchOpen( tString& name, unsigned char flag )
{
	if ( !fInitialized )
	{
		mInit();
	}
	tDir dir;
	for ( tDir* it = g_00c808bc; it != g_00c808c0; it++ )
	{
		dir.fCookie = it->fCookie;
		dir.fSession.fCookie = it->fSession.fCookie;
		tPath found = dir.mGetNamedFile( name );
		if ( found.mValid() && found.mExist() )
		{
			if ( flag == 0 )
			{
				// MATCH-ONLY: the field copies through a pointer stand for an inline path assignment;
				// fPath = found makes the build call tSession::operator= where the original does not.
				tPath* path = &fPath;
				path->fCookie = found.fCookie;
				path->fSession.fCookie = found.fSession.fCookie;
				fUnknown[1] = fPath.mGetSize();
			}
			fUnknown[0] = 0;
			return found;
		}
	}
	if ( g_00c80924 == 0 || flag != 0 || !g_00c80924->fn_slot0( name ) )
	{
		fUnknown[0] = 0;
		fUnknown[1] = 0;
		return tPath();
	}
	return mSearchOpen( name, 0 );
}

#include "Tiger/tFileSession.h"

// tFileSession and eSpecialDir are declared in that header, shared with tgfsys.cpp; the
// next lines keep this file's line numbers.
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
extern tFileSession* g_00c808c8;	// HYPOTHESIS: the file session at 0x00c808c8.

// HYPOTHESIS: the special directory value 6 is not named by the exe.
// FUNCTION: STARFLEETOP 0x00757d80
unsigned char tFile::mOpenForWriteAppend( tString& name )
{
	if ( !fInitialized )
	{
		mInit();
	}
	tDir dir = g_00c808c8->mGetSpecialDir( (eSpecialDir)6 );
	fPath = dir.mGetNamedFile( name );
	if ( !fPath.mValid() )
	{
		if ( g_00c80924 != 0 && g_00c80924->fn_slot1( fPath ) )
		{
			return mOpenForWriteAppend( name );
		}
	}
	else
	{
		fOpened = 0;
		fUnknown[0] = 0;
		if ( fPath.mExist() )
		{
			fUnknown[1] = fPath.mGetSize();
			return 1;
		}
		fUnknown[1] = 0;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00757e80
unsigned char tFile::mOpenForWrite( const char* name, unsigned char flag )
{
	return mOpenForWrite( tString( name ), flag );
}

// FUNCTION: STARFLEETOP 0x00757ef0
unsigned char tFile::mOpenForWrite( tString& name, unsigned char flag )
{
	if ( !fInitialized )
	{
		mInit();
	}
	tDir dir = g_00c808c8->mGetSpecialDir( (eSpecialDir)6 );
	fPath = dir.mGetNamedFile( name );
	if ( !fPath.mValid() )
	{
		if ( g_00c80924 != 0 && g_00c80924->fn_slot1( fPath ) )
		{
			return mOpenForWrite( name, flag );
		}
		return 0;
	}
	if ( fPath.mExist() )
	{
		mClose();
		fPath.mDelete();
	}
	fOpened = 0;
	fUnknown[0] = 0;
	return fPath.mCreate();
}

// FUNCTION: STARFLEETOP 0x007579f0
tPath tFile::mSearchOpen( tPath path, unsigned char flag )
{
	if ( !fInitialized )
	{
		mInit();
	}
	tPath result;
	if ( path.mValid() )
	{
		if ( path.mExist() )
		{
			if ( flag == 0 )
			{
				path.mFree();
				// MATCH-ONLY: field copies stand for the inline path assignment, as in the
				// tString overload.
				tPath dup = g_00c808c8->mDuplicatePath( path );
				tPath* p = &fPath;
				p->fCookie = dup.fCookie;
				p->fSession.fCookie = dup.fSession.fCookie;
				fUnknown[1] = fPath.mGetSize();
			}
			fUnknown[0] = 0;
			return fPath;
		}
		if ( flag == 0 )
		{
			return mSearchOpen( path.mGetName(), 0 );
		}
	}
	if ( g_00c80924 != 0 && flag == 0 && g_00c80924->fn_slot1( path ) )
	{
		return mSearchOpen( path, 0 );
	}
	fUnknown[0] = 0;
	fUnknown[1] = 0;
	return result;
}

// FUNCTION: STARFLEETOP 0x00757ff0
unsigned char tFile::mOpenForWrite( tPath path, unsigned char flag )
{
	if ( !fInitialized )
	{
		mInit();
	}
	if ( !path.mValid() )
	{
		return 0;
	}
	path.mFree();
	fPath = g_00c808c8->mDuplicatePath( path );
	if ( !fPath.mValid() )
	{
		tDir dir = g_00c808c8->mGetSpecialDir( (eSpecialDir)6 );
		fPath = dir.mGetNamedFile( path.mGetName() );
		if ( !fPath.mValid() )
		{
			if ( g_00c80924 && g_00c80924->fn_slot1( fPath ) )
			{
				return mOpenForWrite( path, flag );
			}
			return 0;
		}
	}
	if ( fPath.mExist() )
	{
		mClose();
		fPath.mDelete();
	}
	fOpened = 0;
	fUnknown[0] = 0;
	return fPath.mCreate();
}

// FUNCTION: STARFLEETOP 0x007581d0
long tFile::mWrite( const void* buffer, long size )
{
	if ( !fPath.mValid() || fOpened )
	{
		return -1;
	}
	long got = fPath.mWrite( (void*)buffer, fUnknown[0], size, 0 );
	long remaining = fUnknown[1] - fUnknown[0];
	long extra = got <= remaining ? 0 : got - remaining;
	fUnknown[1] += extra;
	fUnknown[0] += got;
	return got;
}

// FUNCTION: STARFLEETOP 0x00758240
unsigned char tFile::mGetFile( unsigned char flag, const char* name1, const char* name2 )
{
	return mGetFile( flag, tString( name1 ), tString( name2 ) );
}

// FUNCTION: STARFLEETOP 0x007582d0
unsigned char tFile::mGetFile( unsigned char flag, tString& name1, tString& name2 )
{
	if ( flag )
	{
		fPath = g_00c808c8->mFileSaveDialog( name2, name1 );
		if ( fPath.mValid() )
		{
			return mOpenForWrite( fPath, 0 );
		}
	}
	else
	{
		fPath = g_00c808c8->mFileOpenDialog( name2, name1 );
		if ( fPath.mValid() )
		{
			return mOpenForRead( fPath, 0 );
		}
	}
	return 0;
}

// The callee 0x0072eed0 is the C library's malloc.
// FUNCTION: STARFLEETOP 0x00758390
tBufFile::tBufFile( unsigned long size )
{
	fBuffer = 0;
	f48 = 0;
	f4c = 0;
	f50 = 0;
	f54 = 0;
	fBuffer = malloc( size );
	fSize = size;
}

// FUNCTION: STARFLEETOP 0x00758400
// FUNCTION: STARFLEETOP 0x00758420
tBufFile::~tBufFile()
{
	if ( fBuffer )
	{
		free( fBuffer );
	}
}

// FUNCTION: STARFLEETOP 0x007584d0
unsigned char tBufFile::mGetFile( unsigned char flag, const char* name1, const char* name2 )
{
	tFile* file = &fFile;
	if ( file->mGetFile( flag, name1, name2 ) )
	{
		f4c = f48;
		f50 = 0;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758510
tPath tBufFile::mSearchOpen( tString& name, unsigned char flag )
{
	tFile* file = &fFile;
	tPath path = file->mSearchOpen( name, flag );
	if ( path.mValid() )
	{
		if ( path.mExist() && flag == 0 )
		{
			f50 = 0;
			f4c = f48;
		}
	}
	return path;
}

// FUNCTION: STARFLEETOP 0x00758c90
unsigned char tBufFile::mGetFile( unsigned char flag, tString& name1, tString& name2 )
{
	tFile* file = &fFile;
	if ( file->mGetFile( flag, name1, name2 ) )
	{
		f50 = 0;
		f4c = f48;
		return 1;
	}
	return 0;
}

// HYPOTHESIS: tStream is an abstract byte sink; slot 3 (+0x0c) writes a buffer, the byte at +4
// asks for the length to be written byte-swapped.
class tStream
{
public:
	virtual void	fn_slot0( void );
	virtual void	fn_slot1( void );
	virtual void	fn_slot2( void* buffer, long size );
	virtual void	fn_slot3( void* buffer, long size );
	unsigned char	fSwap;
	tStream&	operator<<( unsigned long v );
	tStream&	operator<<( char c );
	tStream&	operator<<( const tString& s );
	tStream&	operator>>( const tString& s );
};

// HYPOTHESIS: the long and char writers were inline overloads; the exe has no copy of either.
inline tStream& tStream::operator<<( unsigned long v )
{
	unsigned long out = v;
	if ( fSwap )
	{
		unsigned short hi = ( (unsigned short*)&v )[1];
		unsigned short lo = ( (unsigned short*)&v )[0];
		( (unsigned short*)&out )[0] = (unsigned short)( ( hi >> 8 ) | ( hi << 8 ) );
		( (unsigned short*)&out )[1] = (unsigned short)( ( lo >> 8 ) | ( lo << 8 ) );
	}
	fn_slot3( &out, 4 );
	return *this;
}

inline tStream& tStream::operator<<( char c )
{
	fn_slot3( &c, 1 );
	return *this;
}

// FUNCTION: STARFLEETOP 0x00758cd0
tStream& tStream::operator<<( const tString& s )
{
	tString& str = (tString&)s;
	long i = 0;
	unsigned long length = str.mStrLen();
	*this << length;
	while ( length-- )
		*this << (char)str[i++];
	return *this;
}

// FUNCTION: STARFLEETOP 0x00758d60
tStream& tStream::operator>>( const tString& s )
{
	tString& str = (tString&)s;
	unsigned long length;
	char c;
	fn_slot2( &length, 4 );
	if ( fSwap )
	{
		unsigned short hi = ( (unsigned short*)&length )[1];
		unsigned short lo = ( (unsigned short*)&length )[0];
		( (unsigned short*)&length )[0] = (unsigned short)( ( hi >> 8 ) | ( hi << 8 ) );
		( (unsigned short*)&length )[1] = (unsigned short)( ( lo >> 8 ) | ( lo << 8 ) );
	}
	str.mEndStrAt( 0 );
	str.mReserve( length );
	while ( length-- )
	{
		fn_slot2( &c, 1 );
		str += (iBasicStringElement)c;
	}
	return *this;
}

// HYPOTHESIS: tFileStream is a tStream over a tFile held at +0x08; only mSetPos is recovered, so
// the slots between tStream's and mSetPos are not declared.
class tFileStream: public tStream
{
public:
	virtual void	fn_slot4( void );
	virtual void	fn_slot5( void );
	virtual void	fn_slot6( void );
	virtual void	fn_slot7( void );
	virtual void	mSetPos( unsigned long pos );
	virtual tStream&	mGet( void* buffer, unsigned long size );
	virtual tStream&	mPut( void* buffer, unsigned long size );
	tFile*	fFile;
};

// FUNCTION: STARFLEETOP 0x00758e00
void tFileStream::mSetPos( unsigned long pos )
{
	if ( !fFile->mSeekSet( pos ) )
	{

		throw "file seek error";

	}
}

// FUNCTION: STARFLEETOP 0x00758e30
tStream& tFileStream::mGet( void* buffer, unsigned long size )
{
	if ( size )
	{
		if ( !fFile->mRead( buffer, size ) )
		{
			throw "file read error";
		}
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x00758e70
tStream& tFileStream::mPut( void* buffer, unsigned long size )
{
	if ( size )
	{
		if ( !fFile->mWrite( buffer, size ) )
		{
			throw "file write error";
		}
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x00758eb0
void tParsePath::mGetPathOrDir( tPath* path, tDir* dir, tString name, tDir base )
{
	if ( path == 0 && dir == 0 )
	{
		return;
	}
	tFileSession* session = g_00c808c8;
	tPath none = tPath();
	f00.mStrLen();
	int sepLen = f28.mStrLen();
	int nameLen = name.mStrLen();
	int pos = 0;
	if ( name.mStrLen() > 2&& name.substr( 0, 2 ) == "//" )
	{
		int end = name.find( f28, 2 );
		if ( end < 0 || end >= nameLen )
		{
			end = nameLen;
		}
		tString server = name.substr( 2, end - 2 );
		if ( server.mStrLen() == 2 && ( server[1] == ':' || server[1] == '|' ) )
		{
			server = server + "\\";
		}
		tVolume vol = session->mGetNamedVolume( server );
		if ( !vol.mValid() )
		{
			if ( path )
			{
				*path = none;
			}
			if ( dir )
			{
				dir->fCookie = tCookie::kInvalidCookie;
				dir->fSession.fCookie = tCookie::kInvalidCookie;
			}
			return;
		}
		pos = end + sepLen;
		base = vol.mGetRoot();
	}
	else if ( name.find( f28, 0 ) == 0 )
	{
		pos = sepLen;
		int end = name.find( f28, pos );
		if ( end < 0 || end >= nameLen )
		{
			end = nameLen;
		}
		tString part = name.substr( pos, end - pos );
		tVolume vol = session->mGetNamedVolume( part );
		if ( vol.mValid() )
		{
			pos = end + sepLen;
		}
		else
		{
			vol = base.mGetParentVol();
		}
		base = vol.mGetRoot();
	}
	else if ( name.find( ":", 0 ) == 1 && name.find( f28, 0 ) == 2 )
	{
		tString drive( name.substr( 0, 2 ) + "\\" );
		tVolume vol = session->mGetNamedVolume( drive );
		if ( !vol.mValid() )
		{
			if ( path )
			{
				*path = none;
			}
			if ( dir )
			{
				dir->fCookie = tCookie::kInvalidCookie;
				dir->fSession.fCookie = tCookie::kInvalidCookie;
			}
			return;
		}
		pos = sepLen + 2;
		base = vol.mGetRoot();
	}
	if ( !base.mValid() )
	{
		if ( path )
		{
			*path = none;
		}
		if ( dir )
		{
			dir->fCookie = tCookie::kInvalidCookie;
			dir->fSession.fCookie = tCookie::kInvalidCookie;
		}
		return;
	}
	int next;
	while ( ( next = name.find( f28, pos ) ) >= 0 && next < nameLen )
	{
		if ( next == 0 )
		{
			return;
		}
		tString part = name.substr( pos, next - pos );
		pos = next + sepLen;
		if ( part == f00 )
		{
			base = base.mGetParentDir();
		}
		else
		{
			base = base.mGetNamedDir( part );
		}
		if ( !base.mValid() || !base.mExist() )
		{
			return;
		}
	}
	if ( path )
	{
		tString rest = name.substr( pos, nameLen - pos );
		*path = base.mGetNamedFile( rest );
	}
	else
	{
		*dir = base;
	}
}

// FUNCTION: STARFLEETOP 0x00758bd0
tPath tBufFile::mSearchOpen( tPath path, unsigned char flag )
{
	tPath result = fFile.mSearchOpen( path, flag );
	if ( result.mValid() )
	{
		if ( result.mExist() && flag == 0 )
		{
			f50 = 0;
			f4c = f48;
		}
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x007585c0
unsigned char tBufFile::mIsOpen( void )
{
	tFile* file = &fFile;
	return file->mIsOpen();
}

// FUNCTION: STARFLEETOP 0x007585d0
unsigned char tBufFile::mOpenForRead( const char* name )
{
	tFile* file = &fFile;
	if ( file->mOpenForRead( name, 0 ) )
	{
		f50 = 0;
		f4c = f48;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758600
unsigned char tBufFile::mOpenForRead( tString& name )
{
	tFile* file = &fFile;
	if ( file->mOpenForRead( name, 0 ) )
	{
		f50 = 0;
		f4c = f48;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758630
unsigned char tBufFile::mOpenForRead( tPath path )
{
	if ( ( &fFile )->mOpenForRead( path, 0 ) )
	{
		f50 = 0;
		f4c = f48;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x007586b0
unsigned char tBufFile::mClose( void )
{
	tFile* file = &fFile;
	return file->mClose();
}

// FUNCTION: STARFLEETOP 0x007586c0
long tBufFile::mRead( void* buffer, long size )
{
	long done = 0;
	if ( 0 < size )
	do
	{
		if ( f4c == f48 && fBuffer )
		{
			tFile* file = &fFile;
			if ( file->mEOF() )
			{
				return done;
			}
			if ( file->mFilePos() != f50 )
			{
				if ( !file->mSeekSet( f50 ) )
				{
					return done;
				}
			}
			f54 = file->mFilePos();
			f48 = file->mRead( fBuffer, fSize );
			if ( f48 == 0xffffffff )
			{
				f48 = 0;
				return done;
			}
			f4c = 0;
		}
		unsigned long n = f48 - f4c;
		if ( (unsigned long)( size - done ) < n )
		{
			n = size - done;
		}
		memcpy( (char*)buffer + done, (char*)fBuffer + f4c, n );
		f4c += n;
		done += n;
		f50 += n;
	} while ( done < size );
	return done;
}

// FUNCTION: STARFLEETOP 0x007587a0
unsigned char tFile::mReadLong( long* value )
{
	return mRead( value, 4 ) == 4;
}

// FUNCTION: STARFLEETOP 0x007587c0
unsigned char tFile::mReadShort( short* value )
{
	return mRead( value, 2 ) == 2;
}

// FUNCTION: STARFLEETOP 0x007587e0
unsigned char tFile::mReadByte( unsigned char* value )
{
	return mRead( value, 1 ) == 1;
}

// FUNCTION: STARFLEETOP 0x00758800
unsigned char tBufFile::mSeekCur( long offset )
{
	unsigned long pos = f50;
	tFile* file = &fFile;
	if ( (unsigned long)file->mFilePos() == pos || file->mSeekSet( pos ) )
	{
		long before = file->mFilePos();
		if ( file->mSeekCur( offset ) )
		{
			long after = file->mFilePos();
			long cur = f4c;
			long end = f48;
			f50 = after;
			long delta = after - before;
			if ( cur != end )
			{
				if ( delta < 0 )
				{
					if ( delta < -cur )
					{
						delta = end - cur;
					}
				}
				else if ( delta >= end - cur )
				{
					delta = end - cur;
				}
				f4c += delta;
			}
			return 1;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758890
unsigned char tBufFile::mSeekSet( long offset )
{
	unsigned long pos = f50;
	tFile* file = &fFile;
	if ( (unsigned long)file->mFilePos() == pos || file->mSeekSet( pos ) )
	{
		long before = file->mFilePos();
		if ( file->mSeekSet( offset ) )
		{
			long after = file->mFilePos();
			long cur = f4c;
			long end = f48;
			f50 = after;
			long delta = after - before;
			if ( cur != end )
			{
				if ( delta < 0 )
				{
					if ( delta < -cur )
					{
						delta = end - cur;
					}
				}
				else if ( delta >= end - cur )
				{
					delta = end - cur;
				}
				f4c += delta;
			}
			return 1;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758920
unsigned char tBufFile::mSeekEnd( long offset )
{
	unsigned long pos = f50;
	tFile* file = &fFile;
	if ( (unsigned long)file->mFilePos() == pos || file->mSeekSet( pos ) )
	{
		long before = file->mFilePos();
		if ( file->mSeekEnd( offset ) )
		{
			long after = file->mFilePos();
			long cur = f4c;
			long end = f48;
			f50 = after;
			long delta = after - before;
			if ( cur != end )
			{
				if ( delta < 0 )
				{
					if ( delta < -cur )
					{
						delta = end - cur;
					}
				}
				else if ( delta >= end - cur )
				{
					delta = end - cur;
				}
				f4c += delta;
			}
			return 1;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x007589b0
long tBufFile::mFilePos( void )
{
	return f50;
}

// FUNCTION: STARFLEETOP 0x007589c0
long tBufFile::mGetLength( void )
{
	return fFile.mGetLength();
}

// FUNCTION: STARFLEETOP 0x007589d0
unsigned char tBufFile::mEOF( void )
{
	fFile.mSeekSet( f50 );
	return fFile.mEOF();
}

// FUNCTION: STARFLEETOP 0x007589f0
unsigned char tBufFile::mOpenForWrite( const char* name )
{
	if ( fFile.mOpenForWrite( name, 0 ) )
	{
		f4c = f48;
		f50 = 0;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758a30
unsigned char tBufFile::mOpenForWrite( tString& name )
{
	if ( fFile.mOpenForWrite( name, 0 ) )
	{
		f4c = f48;
		f50 = 0;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758a70
unsigned char tBufFile::mOpenForWrite( tPath path )
{
	if ( ( &fFile )->mOpenForWrite( path, 0 ) )
	{
		f4c = f48;
		f50 = 0;
		return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00758af0
long tBufFile::mWrite( const void* buffer, long size )
{
	unsigned long pos = f50;
	if ( fFile.mFilePos() != pos )
	{
		if ( !fFile.mSeekSet( pos ) )
		{
			return 0;
		}
	}
	long written = fFile.mWrite( buffer, size );
	if ( written != -1 )
	{
		f4c = f48;
	}
	f50 += written;
	return written;
}

// FUNCTION: STARFLEETOP 0x00758b50
unsigned char tFile::mWriteLong( const long* value )
{
	return mWrite( value, 4 ) == 4;
}

// FUNCTION: STARFLEETOP 0x00758b70
unsigned char tFile::mWriteShort( const short* value )
{
	return mWrite( value, 2 ) == 2;
}

// FUNCTION: STARFLEETOP 0x00758b90
unsigned char tFile::mWriteByte( const unsigned char* value )
{
	return mWrite( value, 1 ) == 1;
}

// FUNCTION: STARFLEETOP 0x00758bb0
tPath tBufFile::mGetPath( void )
{
	return fFile.fn_slot28();
}

// FUNCTION: STARFLEETOP 0x00757610
void gFileDeInit( void )
{
	if ( g_00c808c8 != 0 )
	{
		delete g_00c808c8;
		g_00c808c8 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x007594c0
tPath tParsePath::mGetPath( tString path, tDir dir )
{
	tPath result = tPath();
	mGetPathOrDir( &result, 0, path, dir );
	return result;
}

// The directories tFile::mInit looks up; the SDK's library names them as statics of the
// original's tFile unit.
static tDir gSoundsDir;
static tDir gArtDir;
static tDir gAppDir;
static tDir gQ3Dir;
static tDir gAssetsDir;
static tDir gMoviesDir;

std::vector<tDir> tFile::fSearchVector;

// Written here, out of the original's order, after the tFileSession header it needs; see
// the note after tFile::gGetPath.
// FUNCTION: STARFLEETOP 0x00757310
void tFile::mInit( void )
{
	if ( fInitialized )
	{
		return;
	}
	g_00c808c8 = new tFileSession;
	gAppDir = g_00c808c8->mGetAppPath().mGetParentDir();
	if ( gAppDir.mExist() )
	{
		gAssetsDir = gAppDir.mGetNamedDir( tString( "Assets" ) );
		if ( gAssetsDir.mExist() )
		{
			gSoundsDir = gAssetsDir.mGetNamedDir( tString( "Sounds" ) );
			gArtDir = gAssetsDir.mGetNamedDir( tString( "Art" ) );
			gMoviesDir = gAssetsDir.mGetNamedDir( tString( "Movies" ) );
		}
		gQ3Dir = gAppDir.mGetNamedDir( tString( "Q3" ) );
	}
	std::vector<tDir> dirs;
	dirs.push_back( gAppDir );
	dirs.push_back( gQ3Dir );
	dirs.push_back( gAssetsDir );
	dirs.push_back( gSoundsDir );
	dirs.push_back( gArtDir );
	dirs.push_back( gMoviesDir );
	try
	{
		std::vector<tDir>::iterator it = dirs.begin();
		while ( it != dirs.end() )
		{
			if ( it->mValid() && it->mExist() )
			{
				tString name( it->mGetName() );
				it++;
			}
			else
			{
				dirs.erase( it );
				it = dirs.begin();
			}
		}
		fSearchVector = dirs;
	}
	catch ( ... )
	{
		throw;
	}
	fInitialized = 1;
}
