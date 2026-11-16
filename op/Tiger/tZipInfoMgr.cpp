//	tZipInfoMgr.cpp: tZipInfoMgr, the zip directory of the data files.

#include "Tiger/tZipInfoMgr.h"


// FUNCTION: STARFLEETOP 0x00627f50
tZipInfoMgr::tZipInfoMgr()
{
	field_0 = 0;
	field_4 = 0;
	field_10c = 0;
}

// FUNCTION: STARFLEETOP 0x00627f60
void tZipInfoMgr::mDone()
{
	if ( field_0 )
	{
		delete[] field_0;
		field_0 = 0;
	}
	field_4 = 0;
	field_10c = 0;
}
