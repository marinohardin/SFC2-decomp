// tIdiom.cpp

#include "unsorted/tIdiom.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/DataStoreOutput.h"

// FUNCTION: STARFLEETOP 0x005d37d2
tIdiom::tIdiom()
	: nDataStore::tStreamable( 0 )
{
}

// FUNCTION: STARFLEETOP 0x005d37fb
// FUNCTION: STARFLEETOP 0x005d3817
tIdiom::~tIdiom()
{
	return;
}

// FUNCTION: STARFLEETOP 0x005d3852
tIdiom::tIdiom( const std::string& text )
{
	fn_005d3a6a( text );
}

// FUNCTION: STARFLEETOP 0x005d38a4
tIdiom::tIdiom( const tIdiom& other )
{
	fObjectID = other.fObjectID;
	fObjectVersion = other.fObjectVersion;
	field_0c = other.field_0c;
}

// FUNCTION: STARFLEETOP 0x005d3906
void tIdiom::StreamOut( nDataStore::tOutput& output ) const
{
	output << field_0c;
}

// FUNCTION: STARFLEETOP 0x005d3918
void tIdiom::StreamIn( nDataStore::tInput& input )
{
	input >> field_0c;
}
