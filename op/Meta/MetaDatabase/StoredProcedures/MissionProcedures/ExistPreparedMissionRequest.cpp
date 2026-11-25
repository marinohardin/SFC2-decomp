#define DATASTORE_OUT_OF_LINE_BUFFER_GET	// MATCH-ONLY: tBuffer::Get is the out-of-line copy in fn_0042A000.cpp (0x0042a31c)
#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"

#include "unsorted/nSwitch__tRequestWithResponse.h"

// HYPOTHESIS: the class layout below is reconstructed from the function's accesses only.
class tExistPreparedMissionRequest : public nSwitch::tRequestWithResponse
{
public:
	class tPreparedMissionKey : public nDataStore::tStreamable
	{
	public:
		virtual void StreamOut( nDataStore::tOutput& output ) const;
		virtual void StreamIn( nDataStore::tInput& input );
		char pad[4];
	};

	virtual void vfunction3( nDataStore::tInput& input );
	tPreparedMissionKey fKey;
	bool fFlag;
};

// FUNCTION: STARFLEETOP 0x0055095b
void tExistPreparedMissionRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fObjectVersion;
	fObjectVersion = 0;
	if( c != 0 )
	{
#line 111
		validate( 0 );
	}
	else
	{
		fFlag = ( input >> fKey ).Get() != 0;
	}
}
