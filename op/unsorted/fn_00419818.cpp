#include "unsorted/tEntityShip.h"

// HYPOTHESIS: a tEntityShip member; it writes the three points at 0x8e8, 0x8f4 and 0x900 of the
// ship's layout, clearing the first and setting the other two from the point given.
// FUNCTION: STARFLEETOP 0x00419818
void tEntityShip::fn_00419818(const tPoint3 &aPoint)
{
	field_8e8.mSet(0.0f, 0.0f, 0.0f);
	field_8f4 = aPoint;
	field_900.x = aPoint.x * 100.0;
	field_900.y = aPoint.y * 100.0;
	field_900.z = aPoint.z * 100.0;
}
