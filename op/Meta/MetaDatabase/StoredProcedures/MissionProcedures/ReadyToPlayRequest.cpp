#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"

#include "unsorted/nSwitch__tRequestWithResponse.h"
#include "unsorted/tMissionProfile.h"
#include <vector>
// HYPOTHESIS: the class layout below is reconstructed from the function's accesses only; the
// second read calls the vector operator>> emitted at 0x0053d754 among tMissionProfile's code.
class tReadyToPlayRequest : public nSwitch::tRequestWithResponse
{
public:
	virtual void vfunction3( nDataStore::tInput& input );
	int32u fA;
	std::vector< tMissionProfile::tMissionInfo > fB;
};

// FUNCTION: STARFLEETOP 0x005517de
void tReadyToPlayRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fObjectVersion;
	fObjectVersion = 1;
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
