#ifndef STREAM_BASICFILE_H
#define STREAM_BASICFILE_H

#include "Tiger/tbasicfile.h"

// HYPOTHESIS: the exception object built by 0x006a2ec0: a "file(line)" location in the
// first 0x200 bytes and the formatted message in the next 0x400 (from its sprintf and
// vsprintf targets). The user-declared default constructor is also a hypothesis: with it,
// mGet keeps the return slot apart from the thrown copy, as the exe does.
struct fn_006a2ec0_Error
{
	fn_006a2ec0_Error() {}

	char	location[0x200];
	char	message[0x400];
};

fn_006a2ec0_Error fn_006a2ec0( const char *file, int line, const char *format, ... );

// HYPOTHESIS: layout and members inferred from the error strings only.
class tStream_BasicFile
{
public:
	virtual bool	mGet( void *buffer, unsigned long size );
	virtual bool	mPut( const void *buffer, unsigned long size );

	tBasicFile	*mGetFile( void ) { return mFile; }

	tBasicFile	*mFile;
	bool		field_08;	// HYPOTHESIS: throw on read error when set
};

#endif
