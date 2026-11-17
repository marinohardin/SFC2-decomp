//	tCurrentTime.h
//
//	A streamable object whose name comes from the exe's RTTI (vtable 0x00798204).
//
#if !defined( tCurrentTime_h )
#define tCurrentTime_h

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: layout read from the copy constructor at 0x0042739e: five scalars
// copied one by one, then a 16-byte aggregate copied as a block (perhaps a SYSTEMTIME).
// The virtuals past tStreamable's are not recovered.
class tCurrentTime : public nDataStore::tStreamable
{
public:
	tCurrentTime( const tCurrentTime& source );

	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );

	int32u	field_0xc;
	int32u	field_0x10;
	int32u	field_0x14;
	int32u	field_0x18;
	int32u	field_0x1c;
	struct
	{
		int32u	fWord[4];
	}		field_0x20;
};

#endif
