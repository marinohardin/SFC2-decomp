#ifndef tFile_h
#define tFile_h

#include "Tiger/tgfsys.h"
#include "Tiger/tstring.h"

// HYPOTHESIS: tFile is a class of static functions; only gGetPath is written here.
class tFile
{
public:
	tFile();
	virtual ~tFile();
	// The slot order below follows the vtable at 0x007bd984. VC6 groups overloads and lays
	// them out in reverse declaration order.
	virtual tPath	mPath( void );
	virtual unsigned char	mGetFile( unsigned char flag, const char* name1, const char* name2 );
	virtual unsigned char	mGetFile( unsigned char flag, tString& name1, tString& name2 );
	virtual tPath	mSearchOpen( tString& name, unsigned char flag );
	virtual tPath	mSearchOpen( tPath path, unsigned char flag );
	virtual iTruth	mIsOpen( void );
	virtual unsigned char	mOpenForRead( const char* name, unsigned char flag );
	virtual unsigned char	mOpenForRead( tString& name, unsigned char flag );
	virtual unsigned char	mOpenForRead( tPath path, unsigned char flag );
	virtual unsigned char	mClose( void );
	virtual long	mRead( void* buffer, long size );
	// The vtable at 0x007bd984 holds these twelve between mRead and mSeekCur.
	virtual unsigned char	mReadLong( long* value );
	virtual unsigned char	mReadUlong( unsigned long* value );
	virtual unsigned char	mReadShort( short* value );
	virtual unsigned char	mReadUshort( unsigned short* value );
	virtual unsigned char	mReadByte( unsigned char* value );
	virtual unsigned char	mReadChar( char* value );
	virtual unsigned char	mRead32s( long* value );
	virtual unsigned char	mRead32u( unsigned long* value );
	virtual unsigned char	mRead16s( short* value );
	virtual unsigned char	mRead16u( unsigned short* value );
	virtual unsigned char	mRead8s( signed char* value );
	virtual unsigned char	mRead8u( unsigned char* value );
	virtual unsigned char	mSeekCur( long offset );
	virtual unsigned char	mSeekSet( long offset );
	virtual unsigned char	mSeekEnd( long offset );
	// HYPOTHESIS: slot 27 returns a copy of the path held.
	virtual tPath	fn_slot28( void );
	virtual long	mFilePos( void );
	virtual long	mGetLength( void );
	virtual unsigned char	mEOF( void );
	virtual unsigned char	mOpenForWriteAppend( tString& name );
	virtual unsigned char	mOpenForWrite( const char* name, unsigned char flag );
	virtual unsigned char	mOpenForWrite( tString& name, unsigned char flag );
	virtual unsigned char	mOpenForWrite( tPath path, unsigned char flag );
	virtual long	mWrite( const void* buffer, long size );
	virtual unsigned char	mWriteLong( const long* value );
	virtual unsigned char	mWriteShort( const short* value );
	virtual unsigned char	mWriteByte( const unsigned char* value );
	// HYPOTHESIS: the path the file object holds, at +0x04 after the vtable pointer.
	tPath	fPath;
	// HYPOTHESIS: the original's offsets place the open flag at +0x1c; the 8 bytes before it are
	// not recovered.
	int32u	fUnknown[2];
	// HYPOTHESIS: set once a valid path is held.
	unsigned char	fOpened;
	static tPath gGetPath( tString path, tDir dir );
	static void mInit( void );
protected:
	static unsigned char fInitialized;
	// The directories mInit keeps, in the order it searches them.
	static std::vector<tDir> fSearchVector;
};

// HYPOTHESIS: the layout is read from tBufFile::tBufFile's stores: a tFile base at +0x00, a
// second tFile at +0x20 the buffer forwards to, then the buffer, its size and four zeroed words.
class tBufFile: public tFile
{
public:
	tBufFile( unsigned long size );
	virtual ~tBufFile();
	virtual unsigned char	mGetFile( unsigned char flag, const char* name1, const char* name2 );
	virtual unsigned char	mGetFile( unsigned char flag, tString& name1, tString& name2 );
	virtual tPath	mSearchOpen( tString& name, unsigned char flag );
	virtual tPath	mSearchOpen( tPath path, unsigned char flag );
	virtual unsigned char	mIsOpen( void );
	virtual unsigned char	mOpenForRead( const char* name );
	virtual unsigned char	mOpenForRead( tString& name );
	virtual unsigned char	mOpenForRead( tPath path );
	virtual unsigned char	mClose( void );
	virtual long	mRead( void* buffer, long size );
	virtual unsigned char	mSeekCur( long offset );
	virtual unsigned char	mSeekSet( long offset );
	virtual unsigned char	mSeekEnd( long offset );
	virtual tPath	mGetPath( void );
	virtual long	mFilePos( void );
	virtual long	mGetLength( void );
	virtual unsigned char	mEOF( void );
	virtual unsigned char	mOpenForWrite( const char* name );
	virtual unsigned char	mOpenForWrite( tString& name );
	virtual unsigned char	mOpenForWrite( tPath path );
	virtual long	mWrite( const void* buffer, long size );
	tFile	fFile;
	void*	fBuffer;
	unsigned long	fSize;
	unsigned long	f48;
	unsigned long	f4c;
	unsigned long	f50;
	unsigned long	f54;
};

#endif
