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

// FUNCTION: STARFLEETOP 0x00414205
float tPhysicalEntity::vfunction24()
{
	return (float)field_0x1b4 / field_0x1b8;
}

// HYPOTHESIS: tEntity_Placeholder is the RTTI name; no header declares it yet. Size 0x1c8 from
// the operator new argument, constructor (0x00414278) arguments unknown.
class tEntity_Placeholder
{
public:
	tEntity_Placeholder(int a, int b);

	char	pad00[0x1c8];
};

// FUNCTION: STARFLEETOP 0x0041455b
tEntity_Placeholder* fn_0041455b(int a, int b)
{
	tEntity_Placeholder* p = new tEntity_Placeholder(a, b);
	return p;
}
