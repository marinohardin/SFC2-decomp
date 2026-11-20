#define DATASTORE_INLINE_NUMBER_READ	// MATCH-ONLY: StreamIn inlines some of the int32u reads
#include "unsorted/tPlayingTeamInfo.h"
#include "Common/DataStore/DataStoreInput.h"
#include "Common/Exceptions/Validate.h"

// FUNCTION: STARFLEETOP 0x0053e70e
void tPlayingTeamInfo::StreamIn( nDataStore::tInput& input )
{
	iObjectVersion version = fObjectVersion;
	fObjectVersion = 2;

	switch( version )
	{
		case 0:
			input >> field_0xc >> field_0x1c >> field_0x20 >> field_0x24 >> field_0x28 >> field_0x2c >> field_0x4c >> field_0x3c >> field_0x54;
			field_0x50 = 0;
			break;

		case 1:
		{
			// HYPOTHESIS: version 1 streamed a flag that version 2 replaced by the value at 0x50.
			bool flag;
			input >> field_0xc >> field_0x1c >> field_0x20 >> field_0x24 >> field_0x28 >> field_0x2c >> field_0x4c >> field_0x3c >> field_0x54 >> flag;
			field_0x50 = flag ? 2 : 0;
			break;
		}

		case 2:
			input >> field_0xc >> field_0x1c >> field_0x20 >> field_0x24 >> field_0x28 >> field_0x2c >> field_0x4c >> field_0x3c >> field_0x50 >> field_0x54;
			break;

		default:
#line 100
			validate( 0 );
			break;
	}
}
