// Unit:      unsorted/fn_0042A000.cpp
// Functions: 0x0042a309 to 0x0042aff3, 50 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)

#define DATASTORE_OUT_OF_LINE_GET
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
}
