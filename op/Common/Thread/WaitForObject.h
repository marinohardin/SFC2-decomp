// Common/Thread/WaitForObject.h

#if !defined( WaitForObject_h )
#define WaitForObject_h

#include "Common/BaseTypes.h"

// HYPOTHESIS: the original header is not known. The members are the ones the symbols name
// (0x0070e250-0x0070e2c0); the fields are read from the bytes of tResult's constructor.
class tWaitForObject
{
public:
	// The decoded result of WaitForSingleObject.
	class tResult
	{
	public:
		tResult( int32u waitResult );

		bool TimedOut() const;
		bool Ready() const;

	protected:
		// HYPOTHESIS: names; the constructor stores 3 for WAIT_TIMEOUT (0x102), 2 and
		// (result - 0x80) for WAIT_ABANDONED_0 and up, 1 and the result for WAIT_OBJECT_0
		// and up, 0 and 0 otherwise.
		int32u fKind;
		int32u fIndex;
	};

	static tResult Wait( void* handle, int32u timeout );
};

#endif
