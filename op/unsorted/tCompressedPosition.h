//	tCompressedPosition.h
//
//	Name from the exe's RTTI (.?AVtCompressedPosition@@). Constructor 0x00538ba5, destructor
//	0x00538bf6.
//
#if !defined( tCompressedPosition_h )
#define tCompressedPosition_h

#include "Common/BaseTypes.h"
#include "unsorted/tHexPoint.h"

// HYPOTHESIS: a 4-byte value without a destructor, built by 0x00538c23 from the object ID a
// notify event carries and passed by reference to tCompressedPosition's constructor.
class fn_00538c23_class
{
public:
	fn_00538c23_class( int32u value );

	int32u field_00;
};

// HYPOTHESIS: 0x14 bytes like tHexPoint, whose coordinate getters (0x0055f269, 0x005e8f21)
// the map change procedure calls on it, so it is taken to derive from tHexPoint.
class tCompressedPosition : public tHexPoint
{
public:
	tCompressedPosition( const fn_00538c23_class& compressed );
	virtual ~tCompressedPosition();
};

#endif
