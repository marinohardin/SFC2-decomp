// tsound_fileio.cpp

#include <windows.h>

#include "Tiger/tcglue.h"
#include "Tiger/tgSound.h"

#include "Tiger/tatomic.h"

#include "Tiger/tsound_fileio.h"

class	tSoundFileIO : public tSoundFileIOBase
{
	public:
						tSoundFileIO	( void )	{ field_04 = INVALID_HANDLE_VALUE; field_08 = 0; field_09 = 0; }
		virtual			~tSoundFileIO	( void );
		virtual	iTruth	mOpenForRead	( const char *name );
		virtual	iTruth	mOpenForWrite	( const char *name );
		virtual	iTruth	mClose		( void );
		virtual	iTruth	mEOF		( void );
		virtual	int32u	mRead		( void *buffer, int32u size );
		virtual	int32u	mWrite		( const void *buffer, int32u size );
		virtual	DWORD	mFilePos	( void );
		virtual	DWORD	mFileSize	( void );
		virtual	iTruth	mSeekSet	( LONG position );
		virtual	iTruth	mSeekCur	( LONG offset );
		virtual	iTruth	mSeekEnd	( LONG offset );
		virtual	iTruth	mReadAsync	( void *buffer, int32u size,
									  void (*callback)( void *, int32u ), void *data );
		virtual	int32u	mRead		( void *buffer, int32u size, int32u position );
		virtual	iTruth	mReadAsync	( void *buffer, int32u size, int32u position,
									  void (*callback)( void *, int32u ), void *data );

	// HYPOTHESIS: layout from the destructor's stores; names not yet known.
		HANDLE			field_04;		// the open file, INVALID_HANDLE_VALUE when closed
		int8u			field_08;
		int8u			field_09;

	private:
		// The lock around every file read (its name and access are in the exe's symbols).
		static	tAtomicLock	fLock;
		// Set once the reader thread is running (its name and access are in the exe's symbols).
		static	int8u		fInitialized;

		friend	int8u	_InitSoundFileIO	( void );
		friend	void	_DoneSoundFileIO	( void );
		friend	class tSoundFileIOBase	*_NewSoundFileIO	( void );
};

// tgSoundPart, tSay and the sound error reporting are declared in Tiger/tgSound.h.

// HYPOTHESIS: one queued asynchronous read request, 0x1c bytes; its inline constructor
// sets the byte at +0x19 (seen inlined in 0x0066ebd0). Field meanings follow the arguments
// of tSoundFileIO::mReadAsync and how 0x0066ec50 uses them; names are not yet known.
struct	tAsynchSoundRequest
{
						tAsynchSoundRequest	( void )	{ field_19 = 1; }

		tSoundFileIO	*field_00;							// file to read from
		void			*field_04;							// buffer
		int32u			field_08;							// position
		int32u			field_0c;							// size, then bytes read
		void			(*field_10)( void *, int32u );		// completion callback
		void			*field_14;							// callback data
		int8u			field_18;							// read done
		int8u			field_19;							// slot free
		int8u			field_1a[ 2 ];
};

class	tAsynchSoundQueue
{
	public:
						tAsynchSoundQueue	( void );
						~tAsynchSoundQueue	( void );

				iTruth	mReadNext			( void );
				void	mReadNextInterrupt	( void );
		// HYPOTHESIS: an inline that queues one read, seen inlined in mReadAsync
		// (0x0066fc80); it has no address and its name is not known.
		inline	iTruth	fn_QueueRead		( tSoundFileIO *file, void *buffer, int32u position,
											  int32u size, void (*callback)( void *, int32u ),
											  void *data );

		// HYPOTHESIS: an inline that reports whether reads are still queued, seen inlined
		// in _DoneSoundFileIO (0x0066fee0); its name is not known.
		inline	iTruth	fn_Busy				( void )	{ return field_e04 != field_e00; }

	// HYPOTHESIS: layout from the constructor's stores; names not yet known.
		tAsynchSoundRequest	field_000[ 128 ];
		int32				field_e00;
		int32				field_e04;
		tAtomicLock			field_e08;
		tAtomicLock			field_e2c;
		// HYPOTHESIS: the reader thread's handle and id, as _InitSoundFileIO passes them to
		// CreateThread; not touched by the constructor.
		HANDLE				field_e50;
		DWORD				field_e54;
		int8u				field_e58;
};

DWORD	AsynchSoundIOMain	( void *data );

tAsynchSoundQueue	gAsynchSoundQueue;
tAsynchSoundQueue	*theAsynchSoundQueue;

// FUNCTION: STARFLEETOP 0x0066ebd0
tAsynchSoundQueue::tAsynchSoundQueue( void )
{
	field_e58 = 1;
	field_e00 = 0;
	field_e04 = 0;
	theAsynchSoundQueue = this;
}

// HYPOTHESIS: the original reports through one error macro that prints the message and
// then " -- %s %d\n" with __FILE__ and __LINE__; the file name is the build machine's path,
// so it is written out here, as are the line numbers.
#define kThisFile "D:\\work\\QSI\\Projects\\QSIInc\\Tiger\\tsound_fileio.cpp"

// HYPOTHESIS: two inline lock waits, defined at lines 552 and 572 of the original file
// (their reports carry those lines); they have no address and their names are not known.
// The first gives up when the sound part is at interrupt.
// HYPOTHESIS: the first reports whether it took the lock (mReadAsync gives up when not).
inline iTruth	fn_WaitForLock552( tAtomicLock &lock )
{
	int32	tries = 0;

	while ( !lock.mAcquire() )
	{
		if ( tries >= 1000 )
			break;
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			return 0;
		tries++;
	}
	if ( tries == 1000 )
	{
		AddSoundErrorDuringInterrupt( "Warning -- Lock up -- going on!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 552 );
	}
	return 1;
}

inline void	fn_WaitForLock572( tAtomicLock &lock )
{
	int32	tries = 0;

	while ( !lock.mAcquire() )
	{
		if ( tries >= 1000 )
			break;
		tries++;
	}
	if ( tries == 1000 )
	{
		AddSoundErrorDuringInterrupt( "Warning -- Lock up -- going on!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 572 );
	}
}

inline iTruth	tAsynchSoundQueue::fn_QueueRead( tSoundFileIO *file, void *buffer, int32u position,
											 int32u size, void (*callback)( void *, int32u ),
											 void *data )
{
	tAsynchSoundRequest	*request;

	if ( !fn_WaitForLock552( field_e08 ) )
		return 0;

	request = &field_000[ field_e04 ];
	if ( request->field_19 && file != NULL && buffer != NULL )
	{
		request->field_00 = file;
		request->field_04 = buffer;
		request->field_08 = position;
		request->field_0c = size;
		// HYPOTHESIS: the order of the remaining stores follows the original's instructions.
		request->field_19 = 0;
		request->field_14 = data;
		request->field_18 = 0;
		request->field_10 = callback;
		if ( ++field_e04 == 128 )
			field_e04 = 0;
		field_e08.mRelease();
		return 1;
	}
	field_e08.mRelease();
	return 0;
}

// FUNCTION: STARFLEETOP 0x0066ec50
iTruth	tAsynchSoundQueue::mReadNext( void )
{
	tAsynchSoundRequest	*request;

	if ( field_e00 == field_e04 )
		return 0;

	fn_WaitForLock552( field_e08 );
	fn_WaitForLock572( field_e2c );

	request = &field_000[ field_e00 ];
	if ( !request->field_19 && !request->field_18 )
	{
		if ( request->field_0c > 200000 )
		{
			AddSoundErrorDuringInterrupt( "Reading a whole bunch at interrupt %ld", request->field_0c );
			AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 368 );
		}
		request->field_0c = request->field_00->mRead( request->field_04, request->field_0c, request->field_08 );
		request->field_18 = 1;
	}
	field_e08.mRelease();

	request = &field_000[ field_e00 ];
	if ( !request->field_19 )
	{
		if ( !request->field_18 && request->field_10 )
			request->field_10( request->field_14, 0 );
		else if ( request->field_10 )
			request->field_10( request->field_14, request->field_0c );
		request->field_19 = 1;
	}
	field_e2c.mRelease();

	fn_WaitForLock552( field_e08 );
	if ( ++field_e00 == 128 )
		field_e00 = 0;
	field_e08.mRelease();

	if ( field_e00 == field_e04 )
		return 0;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0066ee80
void	tAsynchSoundQueue::mReadNextInterrupt( void )
{
	tAsynchSoundRequest	*request;
	int32				index;

	if ( field_e00 == field_e04 )
		return;

	if ( field_e04 - field_e00 > 2 )
	{
		AddSoundErrorDuringInterrupt( "%ld waiting to read!", field_e04 - field_e00 );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 622 );
	}

	if ( field_e08.mAcquire() == 1 )
	{
		while ( field_e2c.mAcquire() == 1 )
		{
			request = &field_000[ field_e00 ];
			if ( !request->field_19 && !request->field_18 )
			{
				if ( request->field_0c > 200000 )
				{
					AddSoundErrorDuringInterrupt( "Reading a whole bunch at interrupt %ld", request->field_0c );
					AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 368 );
				}
				request->field_0c = request->field_00->mRead( request->field_04, request->field_0c, request->field_08 );
				request->field_18 = 1;
			}

			index = field_e00;
			if ( ++field_e00 == 128 )
				field_e00 = 0;
			field_e08.mRelease();

			request = &field_000[ index ];
			if ( !request->field_19 )
			{
				if ( !request->field_18 && request->field_10 )
					request->field_10( request->field_14, 0 );
				else if ( request->field_10 )
					request->field_10( request->field_14, request->field_0c );
				request->field_19 = 1;
			}
			field_e2c.mRelease();

			if ( atgSoundPart->fSomeoneIsWaitingForControl == 1 )
			{
				AddSoundErrorDuringInterrupt( "Someone is waiting for control!" );
				AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 643 );
				return;
			}
			if ( field_e00 == field_e04 )
				return;
			if ( !field_e08.mAcquire() )
				return;
		}
		field_e08.mRelease();
	}
}

// 0x0066f0d4 is AsynchSoundIOMain's own `return 0` after its ExitThread( 0 ), which the compiler
// does not know never returns; it is part of AsynchSoundIOMain (0x0066f040), not a function.







// FUNCTION: STARFLEETOP 0x0066f110
tAsynchSoundQueue::~tAsynchSoundQueue( void )
{
}

// FUNCTION: STARFLEETOP 0x0066f170
// FUNCTION: STARFLEETOP 0x0066f190
tSoundFileIO::~tSoundFileIO( void )
{
	tAsynchSoundQueue	*queue;
	int32				index;

	if ( field_04 != INVALID_HANDLE_VALUE )
	{
		CloseHandle( field_04 );
		field_08 = 0;
		field_09 = 0;
		field_04 = INVALID_HANDLE_VALUE;

		// Free every queued request still waiting to read from this file.
		queue = theAsynchSoundQueue;
		if ( queue->field_e00 != queue->field_e04 )
		{
			fn_WaitForLock572( queue->field_e2c );
			for ( index = queue->field_e00; index != queue->field_e04; )
			{
				if ( !queue->field_000[ index ].field_19 && this == queue->field_000[ index ].field_00 )
					queue->field_000[ index ].field_19 = 1;
				if ( ++index == 128 )
					index = 0;
			}
			queue->field_e2c.mRelease();
		}
	}
}

// FUNCTION: STARFLEETOP 0x0066f2d0
iTruth	tSoundFileIO::mOpenForRead( const char *name )
{
	char	path[ MAX_PATH ];
	char	fileName[ MAX_PATH ];
	char	*slash;
	int32	length;
	int32	index;
	HANDLE	file;

	if ( name == NULL )
		return 0;

	if ( field_04 != INVALID_HANDLE_VALUE )
		mClose();

	// Use Windows path separators.
	length = strlen( name );
	for ( index = 0; index < length + 1; index++ )
	{
		if ( name[ index ] == '/' )
			fileName[ index ] = '\\';
		else
			fileName[ index ] = name[ index ];
	}

	file = CreateFileA( fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, NULL );
	if ( file == INVALID_HANDLE_VALUE )
	{
		// Not found as given: try it next to the executable.
		GetModuleFileNameA( GetModuleHandleA( NULL ), path, MAX_PATH );
		slash = strrchr( path, '\\' );
		if ( slash )
			*slash = 0;
		strcat( path, "\\" );
		strcat( path, fileName );
		file = CreateFileA( path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL, NULL );
		if ( file == INVALID_HANDLE_VALUE )
			return 0;
	}

	field_04 = file;
	field_08 = 1;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0066f440
iTruth	tSoundFileIO::mOpenForWrite( const char *name )
{
	char	fileName[ 256 ];
	int32	length;
	int32	index;
	HANDLE	file;

	if ( name == NULL )
		return 0;

	// Use Windows path separators.
	// HYPOTHESIS: the converted name goes unused and the file is created under the name as
	// given; the original's instructions do the same.
	length = strlen( name );
	for ( index = 0; index < length + 1; index++ )
	{
		if ( name[ index ] == '/' )
			fileName[ index ] = '\\';
		else
			fileName[ index ] = name[ index ];
	}

	field_04 = INVALID_HANDLE_VALUE;
	file = CreateFileA( name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( file == INVALID_HANDLE_VALUE )
		return 0;

	field_04 = file;
	return 1;
}

// FUNCTION: STARFLEETOP 0x0066f4e0
iTruth	tSoundFileIO::mClose( void )
{
	tAsynchSoundQueue	*queue;
	int32				index;

	// Closing a file that is not open succeeds.
	if ( field_04 == INVALID_HANDLE_VALUE )
		return 1;

	CloseHandle( field_04 );
	field_08 = 0;
	field_09 = 0;
	field_04 = INVALID_HANDLE_VALUE;

	// Free every queued request still waiting to read from this file.
	queue = theAsynchSoundQueue;
	if ( queue->field_e00 != queue->field_e04 )
	{
		fn_WaitForLock572( queue->field_e2c );
		for ( index = queue->field_e00; index != queue->field_e04; )
		{
			if ( !queue->field_000[ index ].field_19 && this == queue->field_000[ index ].field_00 )
				queue->field_000[ index ].field_19 = 1;
			if ( ++index == 128 )
				index = 0;
		}
		queue->field_e2c.mRelease();
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x0066f5d0
int32u	tSoundFileIO::mRead( void *buffer, int32u size )
{
	DWORD	bytesRead;

	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to read from a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 840 );
		return 0;
	}
	if ( !field_08 )
	{
		AddSoundErrorDuringInterrupt( "Trying to read from a file not opened for reading!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 846 );
		return 0;
	}
	if ( size == 0 )
		return 0;
	if ( buffer == NULL )
	{
		AddSoundErrorDuringInterrupt( "Trying to read to a NULL ptr!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 857 );
		return 0;
	}

	while ( !fLock.mAcquire() )
		;

	bytesRead = 0;
	if ( !ReadFile( field_04, buffer, size, &bytesRead, NULL ) )
	{
		AddSoundErrorDuringInterrupt( "Trouble reading a file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 868 );
	}

	// Fill whatever the file did not supply with silence.
	if ( bytesRead < size )
		memset( (int8u *)buffer + bytesRead, 0, size - bytesRead );

	fLock.mRelease();
	return bytesRead;
}

// FUNCTION: STARFLEETOP 0x0066f710
int32u	tSoundFileIO::mWrite( const void *buffer, int32u size )
{
	DWORD	bytesWritten;

	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to write to a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 890 );
		return 0;
	}
	if ( !field_09 )
	{
		AddSoundErrorDuringInterrupt( "Trying to write to a file not opened for writing!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 896 );
		return 0;
	}
	if ( size == 0 )
		return 0;
	if ( buffer == NULL )
	{
		AddSoundErrorDuringInterrupt( "Trying to write with a NULL ptr!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 907 );
		return 0;
	}

	while ( !fLock.mAcquire() )
		;

	if ( !WriteFile( field_04, buffer, size, &bytesWritten, NULL ) )
	{
		AddSoundErrorDuringInterrupt( "Trouble writing a file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 917 );
	}

	fLock.mRelease();
	return bytesWritten;
}

// FUNCTION: STARFLEETOP 0x0066f830
iTruth	tSoundFileIO::mEOF( void )
{
	return mFilePos() == mFileSize();
}

// FUNCTION: STARFLEETOP 0x0066f850
DWORD	tSoundFileIO::mFilePos( void )
{
	DWORD	position;

	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to get the position of a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 935 );
		return 0;
	}

	position = SetFilePointer( field_04, 0, NULL, FILE_CURRENT );
	if ( position == 0xFFFFFFFF )
	{
		AddSoundErrorDuringInterrupt( "Trying to get the position of a file failed!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 942 );
		return 0;
	}
	return position;
}

// FUNCTION: STARFLEETOP 0x0066f8b0
DWORD	tSoundFileIO::mFileSize( void )
{
	int32u	position;
	int32u	size;

	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to get the size of a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 954 );
		return 0;
	}

	// Seek to the end to read the size, then go back.
	position = mFilePos();
	mSeekEnd( 0 );
	size = mFilePos();
	mSeekSet( position );
	return size;
}

// FUNCTION: STARFLEETOP 0x0066f910
iTruth	tSoundFileIO::mSeekSet( LONG position )
{
	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to seek in a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 970 );
		return false;
	}

	return ( SetFilePointer( field_04, position, NULL, FILE_BEGIN ) == 0xFFFFFFFF ) ? false : true;
}

// FUNCTION: STARFLEETOP 0x0066f960
iTruth	tSoundFileIO::mSeekCur( LONG offset )
{
	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to seek in a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 987 );
		return false;
	}

	return ( SetFilePointer( field_04, offset, NULL, FILE_CURRENT ) == 0xFFFFFFFF ) ? false : true;
}

// FUNCTION: STARFLEETOP 0x0066f9b0
iTruth	tSoundFileIO::mSeekEnd( LONG offset )
{
	if ( field_04 == INVALID_HANDLE_VALUE )
	{
		AddSoundErrorDuringInterrupt( "Trying to seek in a closed file!" );
		AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 1005 );
		return false;
	}

	return ( SetFilePointer( field_04, offset, NULL, FILE_END ) == 0xFFFFFFFF ) ? false : true;
}

// FUNCTION: STARFLEETOP 0x0066fa40
int32u	tSoundFileIO::mRead( void *buffer, int32u size, int32u position )
{
	int32u	bytesRead;

	// HYPOTHESIS: each report is one expanded macro: buffer the message while the sound
	// part is at interrupt, otherwise flush the buffer and say it directly.
	if ( size == 0 )
	{
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			AddSoundErrorDuringInterrupt( "What the...!" );
		else
		{
			PrintOutBufferedErrors();
			gSoundSay.mJustSayIt( "What the...!" );
		}
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 1432 );
		else
		{
			PrintOutBufferedErrors();
			gSoundSay.mJustSayIt( " -- %s %d\n", kThisFile, 1432 );
		}
		return 0;
	}
	if ( vfunc_03c() )
	{
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			AddSoundErrorDuringInterrupt( "Trying to read while asynch read is in progress!" );
		else
		{
			PrintOutBufferedErrors();
			gSoundSay.mJustSayIt( "Trying to read while asynch read is in progress!" );
		}
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 1438 );
		else
		{
			PrintOutBufferedErrors();
			gSoundSay.mJustSayIt( " -- %s %d\n", kThisFile, 1438 );
		}
		return 0;
	}
	if ( !field_08 )
	{
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			AddSoundErrorDuringInterrupt( "Trying to read from a file that has not been opened!" );
		else
		{
			PrintOutBufferedErrors();
			gSoundSay.mJustSayIt( "Trying to read from a file that has not been opened!" );
		}
		if ( atgSoundPart->mAreWeInInterruptMode() == 1 )
			AddSoundErrorDuringInterrupt( " -- %s %d\n", kThisFile, 1444 );
		else
		{
			PrintOutBufferedErrors();
			gSoundSay.mJustSayIt( " -- %s %d\n", kThisFile, 1444 );
		}
		return 0;
	}

	if ( position != mFilePos() )
		mSeekSet( position );

	bytesRead = mRead( buffer, size );
	if ( bytesRead == 0xFFFFFFFF )
		return 0;
	return bytesRead;
}

// FUNCTION: STARFLEETOP 0x0066fc50
iTruth	tSoundFileIO::mReadAsync( void *buffer, int32u size,
								  void (*callback)( void *, int32u ), void *data )
{
	return mReadAsync( buffer, size, mFilePos(), callback, data );
}

// FUNCTION: STARFLEETOP 0x0066fc80
iTruth	tSoundFileIO::mReadAsync( void *buffer, int32u size, int32u position,
								  void (*callback)( void *, int32u ), void *data )
{
	if ( theAsynchSoundQueue == NULL )
		return 0;
	return theAsynchSoundQueue->fn_QueueRead( this, buffer, position, size, callback, data );
}

// FUNCTION: STARFLEETOP 0x0066fda0
int8u	_InitSoundFileIO( void )
{
	if ( !tSoundFileIO::fInitialized )
	{
		fn_WaitForLock552( gAsynchSoundQueue.field_e08 );
		fn_WaitForLock572( gAsynchSoundQueue.field_e2c );

		gAsynchSoundQueue.field_e58 = 0;
		gAsynchSoundQueue.field_e50 = CreateThread( NULL, 0,
													(LPTHREAD_START_ROUTINE)AsynchSoundIOMain,
													&gAsynchSoundQueue, 0,
													&gAsynchSoundQueue.field_e54 );
		if ( gAsynchSoundQueue.field_e50 == NULL )
		{
			gAsynchSoundQueue.field_e2c.mRelease();
			gAsynchSoundQueue.field_e08.mRelease();
			throw "Error creating thread";
		}
		gAsynchSoundQueue.field_e2c.mRelease();
		gAsynchSoundQueue.field_e08.mRelease();
		tSoundFileIO::fInitialized = 1;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x0066fee0
void	_DoneSoundFileIO( void )
{
	if ( tSoundFileIO::fInitialized )
	{
		tSoundFileIO::fInitialized = 0;
		if ( gAsynchSoundQueue.field_e58 != 1 )
		{
			fn_WaitForLock552( gAsynchSoundQueue.field_e08 );
			fn_WaitForLock572( gAsynchSoundQueue.field_e2c );

			// Tell the reader thread to stop, then wait for it to drain the queue.
			gAsynchSoundQueue.field_e58 = 1;
			gAsynchSoundQueue.field_e2c.mRelease();
			gAsynchSoundQueue.field_e08.mRelease();
			while ( gAsynchSoundQueue.fn_Busy() )
				;
			CloseHandle( gAsynchSoundQueue.field_e50 );
		}
	}
}

// FUNCTION: STARFLEETOP 0x00670000
tSoundFileIOBase *	_NewSoundFileIO( void )
{
	// HYPOTHESIS: the original source repeated the body of _InitSoundFileIO here
	// (the exe has it expanded in place and not as a call).
	if ( !tSoundFileIO::fInitialized )
	{
		fn_WaitForLock552( gAsynchSoundQueue.field_e08 );
		fn_WaitForLock572( gAsynchSoundQueue.field_e2c );

		gAsynchSoundQueue.field_e58 = 0;
		gAsynchSoundQueue.field_e50 = CreateThread( NULL, 0,
													(LPTHREAD_START_ROUTINE)AsynchSoundIOMain,
													&gAsynchSoundQueue, 0,
													&gAsynchSoundQueue.field_e54 );
		if ( gAsynchSoundQueue.field_e50 == NULL )
		{
			gAsynchSoundQueue.field_e2c.mRelease();
			gAsynchSoundQueue.field_e08.mRelease();
			throw "Error creating thread";
		}
		gAsynchSoundQueue.field_e2c.mRelease();
		gAsynchSoundQueue.field_e08.mRelease();
		tSoundFileIO::fInitialized = 1;
	}
	return new tSoundFileIO;
}
