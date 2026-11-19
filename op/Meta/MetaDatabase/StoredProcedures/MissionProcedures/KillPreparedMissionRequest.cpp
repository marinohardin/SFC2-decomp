#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"

#include "unsorted/tRequestWithResponse.h"

// HYPOTHESIS: the class layout below is reconstructed from the function's accesses only.
class tKillPreparedMissionRequest : public tRequestWithResponse
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
	bool fFlag;
	tPreparedMissionKey fKey;
};

// FUNCTION: STARFLEETOP 0x00550e96
void tKillPreparedMissionRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fFlag8;
	fFlag8 = 0;
	if( c != 0 )
	{
#line 104
		validate( 0 );
	}
	else
	{
		fFlag = input.Get() != 0;
		input >> fKey;
	}
}
