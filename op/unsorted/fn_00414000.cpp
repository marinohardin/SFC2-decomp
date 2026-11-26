#include "Source/entity/entity_physical.h"

// FUNCTION: STARFLEETOP 0x004140a9
// HYPOTHESIS: removes the entity ID from the threat list, shifting the later entries down.
void tPhysicalEntity::vfunction47(int aID)
{
	for (int i = field_0x184 - 1; i >= 0; i--)
	{
		if (field_0x180[i] == aID)
		{
			field_0x184 = field_0x184 - 1;
			for (int j = i; j < field_0x184; j++)
			{
				field_0x180[j] = field_0x180[j + 1];
			}
			return;
		}
	}
}

// FUNCTION: STARFLEETOP 0x004141fe
int tPhysicalEntity::vfunction23()
{
	return field_0x1b4;
}
