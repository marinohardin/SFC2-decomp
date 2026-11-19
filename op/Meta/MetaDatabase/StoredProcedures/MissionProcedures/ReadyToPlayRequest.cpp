#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"

#include "unsorted/tRequestWithResponse.h"

// HYPOTHESIS: the class layout below is reconstructed from the function's accesses only.
class tReadyToPlayRequest : public tRequestWithResponse
{
public:
	virtual void vfunction3( nDataStore::tInput& input );
	int32u fA;
	int32u fB;
};

// FUNCTION: STARFLEETOP 0x005517de
void tReadyToPlayRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fFlag8;
	fFlag8 = 1;
	switch( c )
	{
	case 0:
		input >> fA;
		break;
	case 1:
		input >> fA >> fB;
		break;
	default:
#line 145
		validate( 0 );
	}
}
