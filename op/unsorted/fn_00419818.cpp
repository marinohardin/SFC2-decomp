#include "unsorted/tEntityShip.h"
#include "unsorted/tObject_0040ce21.h"

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

// HYPOTHESIS: a tEntityShip member; it fills nine floats from 0x948 with zeros and the three
// values given.
// FUNCTION: STARFLEETOP 0x00419873
void tEntityShip::fn_00419873(short param_1, short param_2, short param_3)
{
	field_948[0] = 0.0f;
	field_948[1] = param_1;
	field_948[2] = 0.0f;
	field_948[3] = param_2;
	field_948[4] = 0.0f;
	field_948[5] = param_3;
	field_948[6] = param_1;
	field_948[7] = param_2;
	field_948[8] = param_3;
}

// HYPOTHESIS: a tEntityShip member; it adds 1.5 to the float at 0x18 of slot 27's record of
// the object at 0xad4, and hands it, cut to a whole number, to its slot 22.
// FUNCTION: STARFLEETOP 0x004198e0
void tEntityShip::fn_004198e0()
{
	static tObject_0040ce21 marker("More power to Ya!");

	tObject_00416d41_ad4 *&object = field_ad4;
	tObject_00416d41_bounds *record = object->vfunction27();
	object->vfunction22((int)(record->field_18 + 1.5f));
}

// HYPOTHESIS: a tEntityShip member; when the float at 0x18 of slot 27's record of the object at
// 0xad4 is above zero, it lowers it by 0.5 and hands it, cut to a whole number, to slot 22.
// FUNCTION: STARFLEETOP 0x0041994b
void tEntityShip::fn_0041994b()
{
	static tObject_0040ce21 marker("Whoa!");

	tObject_00416d41_ad4 *&object = field_ad4;
	tObject_00416d41_bounds *record = object->vfunction27();
	if (record->field_18 > 0.0f)
	{
		object->vfunction22((int)(record->field_18 - 0.5f));
	}
}
