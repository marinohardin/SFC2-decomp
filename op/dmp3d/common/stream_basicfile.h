#ifndef STREAM_BASICFILE_H
#define STREAM_BASICFILE_H

#include "Tiger/tbasicfile.h"

// The exception class thrown by the 3D library (name from the exe's throw information,
// __TI1?AVtErrInfo@@, and the SDK's constructor symbol ??0tErrInfo@@QAA@PBDH0ZZ).
// HYPOTHESIS: a "file(line)" location in the first 0x200 bytes and the formatted message in the
// next 0x400 (from its sprintf and vsprintf targets).
class tErrInfo
{
public:
	tErrInfo( const char *file, int line, const char *format, ... );

	char	location[0x200];
	char	message[0x400];
};

// The stream interface (name from the SDK's symbol ?mByteInput@tStream_Base@@QAEEPAXK@Z).
// HYPOTHESIS: the four slots of tStream_BasicFile's vtable (0x007959e0) are declared here,
// pure, in its order; mByteInput is the non-virtual reader the model loader calls.
class tStream_Base
{
public:
	virtual unsigned long	mGetPos( void ) = 0;
	virtual unsigned char	mSetPos( unsigned long position ) = 0;
	virtual unsigned char	mGet( void *buffer, unsigned long size ) = 0;
	virtual unsigned char	mPut( const void *buffer, unsigned long size ) = 0;

	unsigned char	mByteInput( void *buffer, unsigned long size );
};

// HYPOTHESIS: layout and members inferred from the error strings only.
class tStream_BasicFile : public tStream_Base
{
public:
	// HYPOTHESIS: an inline constructor; 0x0042fef9 stores the file, the vtable and the flag.
	tStream_BasicFile( tBasicFile *file, bool flag ) : mFile( file ) { field_08 = flag; }

	virtual unsigned long	mGetPos( void );
	virtual unsigned char	mSetPos( unsigned long position );
	virtual unsigned char	mGet( void *buffer, unsigned long size );
	virtual unsigned char	mPut( const void *buffer, unsigned long size );

	tBasicFile	*mGetFile( void ) { return mFile; }

	tBasicFile	*mFile;
	bool		field_08;	// HYPOTHESIS: throw on read error when set
};

#endif
