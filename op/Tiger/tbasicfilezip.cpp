//	tbasicfilezip.cpp: tBasicFileZip, the file interface over a file stored in the data zip.

#include "Tiger/tbasicfilezip.h"
#include "Tiger/tZipInfoMgr.h"

namespace tBasicFileZipNamespace
{

// FUNCTION: STARFLEETOP 0x00628610
unsigned char tBasicFileZip::mOpenForRead( const char *name )
{
	mClose();
	if ( field_4 )
	{
		if ( field_4->mOpenForRead( name ) )
		{
			field_14 = 0;
			return 1;
		}
		if ( field_18 )
		{
			const tZipInfoMgr::tZipEntry *pEntry = field_18->mFindEntry( name );
			if ( pEntry && pEntry->field_10 == 0 && pEntry->field_8 == pEntry->field_c &&
				 field_4->mOpenForRead( field_18->field_8 ) )
			{
				field_8 = pEntry->field_c;
				field_c = 0;
				field_10 = pEntry->field_4;
				field_14 = 1;
				if ( !mSeekSet( 0 ) )
				{
					mClose();
					return 0;
				}
				return 1;
			}
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x006286b0
unsigned char tBasicFileZip::mOpenForWrite( const char *name )
{
	mClose();
	if ( field_4 )
	{
		if ( field_4->mOpenForWrite( name ) )
		{
			field_14 = 0;
			return 1;
		}
	}
	return 0;
}

}
