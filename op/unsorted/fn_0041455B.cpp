#include "unsorted/tEntityShip.h"

// FUNCTION: STARFLEETOP 0x00414b2a
int tEntityShip::vfunction70()
{
	return field_878;
}

// FUNCTION: STARFLEETOP 0x00414b31
float tEntityShip::vfunction64()
{
	return field_ac8;
}

// FUNCTION: STARFLEETOP 0x00414b38
float tEntityShip::vfunction65()
{
	return field_8a8;
}

// FUNCTION: STARFLEETOP 0x00414b3f
short tEntityShip::vfunction75()
{
	return field_92e;
}

// FUNCTION: STARFLEETOP 0x00414b47
void tEntityShip::fn_00414b47(int param_1)
{
	// HYPOTHESIS: a 100-byte record cleared on allocation; its class is not known.
	int *record = (int *)operator new(100);
	if (record != 0)
	{
		for (int i = 0; i < 25; i++)
			record[i] = 0;
	}
	else
		record = 0;
	field_a54[param_1] = record;
}
