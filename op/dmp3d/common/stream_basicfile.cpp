#include "stream_basicfile.h"

// FUNCTION: STARFLEETOP 0x006a2dc0
// HYPOTHESIS: the original passes __FILE__ and __LINE__ (41) through an error macro; the
// literals stand in for them so the build path and line do not change the string.
unsigned char tStream_BasicFile::mGet( void *buffer, unsigned long size )
{
	if ( mFile->mRead( buffer, size ) != size )
	{
		if ( field_08 )
		{
			throw tErrInfo( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\stream_basicfile.cpp", 41,
				"tStream_BasicFile::mGet(): Read error\n" );
		}
		return false;
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x006a2e40
// HYPOTHESIS: as in mGet, the literals stand in for __FILE__ and __LINE__ (62).
unsigned char tStream_BasicFile::mPut( const void *buffer, unsigned long size )
{
	if ( mFile->mWrite( buffer, size ) != size )
	{
		if ( field_08 )
		{
			throw tErrInfo( "D:\\work\\QSI\\Projects\\CORE\\dmp3d\\common\\stream_basicfile.cpp", 62,
				"tStream_BasicFile::mPut(): Write error\n" );
		}
		return false;
	}
	return true;
}
