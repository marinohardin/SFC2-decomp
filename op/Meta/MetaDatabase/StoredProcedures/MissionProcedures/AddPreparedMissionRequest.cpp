#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/tRequestWithResponse.h"

// HYPOTHESIS: class layouts below are reconstructed from the function's accesses only.
class tMissionBase
{
public:
	virtual void m0();
	char pad4[4];
};

class tPrepared : public tMissionBase, public nDataStore::tStreamable
{
public:
	virtual void StreamOut( nDataStore::tOutput& output ) const;
	virtual void StreamIn( nDataStore::tInput& input );
	char pad[0x80];
};

class tAddPreparedMissionRequest : public tRequestWithResponse
{
public:
	virtual void vfunction3( nDataStore::tInput& input );
	tPrepared fMission;
	char pad2[4];
	bool fFlag;
};

// FUNCTION: STARFLEETOP 0x0054fa6d
void tAddPreparedMissionRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	char c = fObjectVersion;
	fObjectVersion = 0;
	if( c != 0 )
	{
#line 128
		validate( 0 );
	}
	else
	{
		fFlag = ( input >> fMission ).Get() != 0;
	}
}
