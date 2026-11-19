#include "Common/DataStore/DataStoreInput.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Exceptions/Validate.h"

#include "unsorted/tRequestWithResponse.h"

#include <vector>

// HYPOTHESIS: the class layout below is reconstructed from the function's accesses only.
class tChooseMissionRequest : public tRequestWithResponse
{
public:
	// HYPOTHESIS: the elements stream through the vector operator>> emitted at 0x00550150.
	class tMissionSelection : public nDataStore::tStreamable
	{
	public:
		virtual void StreamOut( nDataStore::tOutput& output ) const;
		virtual void StreamIn( nDataStore::tInput& input );
	};

	virtual void vfunction3( nDataStore::tInput& input );
	std::vector< tMissionSelection > fSelections;
	int32u fValue;
	bool fFlag;
};

// FUNCTION: STARFLEETOP 0x0054ffe5
void tChooseMissionRequest::vfunction3( nDataStore::tInput& input )
{
	tRequestWithResponse::vfunction3( input );
	int8u c = fObjectVersion;
	fObjectVersion = 2;
	switch( c )
	{
	case 0:
		input >> fSelections;
		fValue = 45000;
		break;
	case 1:
		input >> fSelections >> fValue;
		break;
	case 2:
		fFlag = ( input >> fSelections >> fValue ).Get() != 0;
		break;
	default:
#line 195
		validate( 0 );
	}
}
