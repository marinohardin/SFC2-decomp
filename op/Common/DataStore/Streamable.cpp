#include "Common/DataStore/Streamable.h"
#include "Common/Crypto/CRC32.h"

#include <typeinfo.h>

namespace nDataStore
{

// FUNCTION: STARFLEETOP 0x006f8cd0
// FUNCTION: STARFLEETOP 0x006f8cb0
tStreamable::tCRCFailed::~tCRCFailed()
{
}

// FUNCTION: STARFLEETOP 0x006f8d50
void tStreamable::tCRCFailed::ReportException( const char* caughtDescription )
{
	tBaseException::ReportException( caughtDescription );
}

// FUNCTION: STARFLEETOP 0x006f8e90
tStreamable::tObjectIDInfo::tObjectIDInfo()
: fCRC( 0xffffffff )
{
}

// FUNCTION: STARFLEETOP 0x006f8eb0
tStreamable::tStreamable( iObjectVersion versionNumber )
: fObjectID( 0xffffffff )
, fObjectVersion( versionNumber )
{
}

// FUNCTION: STARFLEETOP 0x006f8ed0
tStreamable::~tStreamable()
{
}

// The inlined basic_string::_Freeze and _Tidy lack the toolchain's _FROZEN test (the library
// difference noted at 0x00708820); the rest of 0x006f8ee0 lines up.
// FUNCTION: STARFLEETOP 0x006f8ee0
tStreamable::iObjectID tStreamable::GetObjectID() const
{
	if( fObjectID == tCRC32::kDefaultSeed )
	{
		const type_info& objectType = typeid( *this );

		tObjectIDInfo* idInfo = &fObjectIDInfoLookup[ &objectType ];

		if( idInfo->fCRC == tCRC32::kDefaultSeed )
		{
			tCRC32 crc;
			std::string name = objectType.raw_name();
			idInfo->fObjectName = objectType.name();
			idInfo->fCRC = crc.UpdateCRC( tCRC32::kDefaultSeed, name.begin(), name.size() );
		}

		fObjectID = idInfo->fCRC;
	}

	return fObjectID;
}

// FUNCTION: STARFLEETOP 0x006f8d90
// FUNCTION: STARFLEETOP 0x006f8d70
tStreamable::iObjectIDInfoLookup tStreamable::fObjectIDInfoLookup;

};
