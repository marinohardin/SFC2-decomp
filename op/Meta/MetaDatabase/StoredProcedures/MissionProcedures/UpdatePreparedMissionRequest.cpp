#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"

#include "unsorted/nSwitch__tRequestWithResponse.h"

// HYPOTHESIS: the class layout below is reconstructed from the function's accesses only.
class tUpdatePreparedMissionRequest : public nSwitch::tRequestWithResponse
{
public:
	class tDataBase
	{
	public:
		virtual void fn_base();
		int fBase;
	};

	// HYPOTHESIS: the streamed member has a leading 8-byte base before its tStreamable.
	class tData : public tDataBase, public nDataStore::tStreamable
	{
	public:
		virtual void StreamOut( nDataStore::tOutput& output ) const;
		virtual void StreamIn( nDataStore::tInput& input );
		char pad[4];
	};

	virtual void vfunction3( nDataStore::tInput& input );
	tData fData;
};

// FUNCTION: STARFLEETOP 0x00551a84
void tUpdatePreparedMissionRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fObjectVersion;
	fObjectVersion = 0;
	if( c != 0 )
	{
#line 102
		validate( 0 );
	}
	else
	{
		nDataStore::tStreamable* data = &fData;
		input >> *data;
	}
}
