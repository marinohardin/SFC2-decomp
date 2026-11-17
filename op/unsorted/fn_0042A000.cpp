// Unit:      unsorted/fn_0042A000.cpp
// Functions: 0x0042a309 to 0x0042aff3, 50 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#define DATASTORE_OUT_OF_LINE_GET
#define DATASTORE_OUT_OF_LINE_BUFFER_GET
#define DATASTORE_OUT_OF_LINE_READ
#include "Common/DataStore/DataStoreInput.h"

namespace nDataStore {
// MATCH-ONLY: Get is declared inline in Common/DataStore, so the compiler emits its out-of-line
// copy (0x0042a309) only for a use it cannot inline; this pointer is that use.
byte (tInput::*gEmitInputGet)() = &tInput::Get;

// FUNCTION: STARFLEETOP 0x0042a309
byte tInput::Get()
{
	return fBuffer->Get( fGetPos++ );
}

// FUNCTION: STARFLEETOP 0x0042a31c
byte tBuffer::Get( iPos getPos ) const
{
	if( getPos >= Size() )
	{
// MATCH-ONLY: the original compiled this inline body from DataStoreBuffer.cpp, which its header
// includes; the exe's exception carries that file and line. The path names the existing unit
// so the compiler's source listing can open it.
#line 117 "..\\Common\\DataStore\\DataStoreBuffer.cpp"
		throw except::tOutOfBounds( "tBuffer", 0, Size(), getPos, __FILE__, __LINE__ );
#line 31 "fn_0042A000.cpp"
	}

	return fBuffer[getPos];
}

// MATCH-ONLY: as for Get above, this pointer is the use that makes the compiler emit Read's
// out-of-line copy (0x0042a377).
void (tInput::*gEmitInputRead)( byte*, int32 ) = &tInput::Read;

// FUNCTION: STARFLEETOP 0x0042a377
void tInput::Read( byte* data, int32 size )
{
	fBuffer->Read( data, size, fGetPos );

	fGetPos += size;
}
}
