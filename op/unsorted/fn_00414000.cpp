#include "Source/entity/entity_physical.h"
#include "Tiger/tstring.h"

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
void fn_0041455b(int a, int b)
{
	new tEntity_Placeholder(a, b);
}

// HYPOTHESIS: the global's name is unknown; the path string is from the exe.
// FUNCTION: STARFLEETOP 0x004145e7
// FUNCTION: STARFLEETOP 0x004145d7
tString g_008e0d48 = "assets\\WeaponGroups\\";

// HYPOTHESIS: the globals at 0x008e0cd0 are a table of vectors; names unknown.
extern float g_008e0cd0[36];

// FUNCTION: STARFLEETOP 0x00414602
void fn_00414602()
{
	g_008e0cd0[0] = 0.0f;
	g_008e0cd0[1] = 0.0f;
	g_008e0cd0[6] = 2.0f;
	g_008e0cd0[2] = 0.0f;
	g_008e0cd0[10] = -3.0f;
	g_008e0cd0[15] = 2.0f;
	g_008e0cd0[3] = -2.0f;
	g_008e0cd0[4] = -2.0f;
	g_008e0cd0[5] = 0.0f;
	g_008e0cd0[7] = -2.0f;
	g_008e0cd0[8] = 0.0f;
	g_008e0cd0[9] = 0.0f;
	g_008e0cd0[11] = 0.0f;
	g_008e0cd0[12] = -2.0f;
	g_008e0cd0[13] = -4.0f;
	g_008e0cd0[14] = 0.0f;
	g_008e0cd0[16] = -4.0f;
	g_008e0cd0[17] = 0.0f;
	for (int i = 18; i < 30; i++)
		g_008e0cd0[i] = 0.0f;
}
