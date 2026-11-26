// Unit:      unsorted/fn_00411790.cpp
// Functions: 0x00411790 to 0x004117a5, 2 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Source/entity/entity_physical.h"

void __cdecl fn_007364c0(char *aDest, const char *aSource);

// FUNCTION: STARFLEETOP 0x00411790
void tPhysicalEntity::vfunction41(int aValue)
{
	fn_007364c0((char *)&field_0x11c, (const char *)aValue);
}

int fn_004f3eda();

// HYPOTHESIS: no header declares the astral entities yet; the names are the exe's RTTI.
class tEntity_Astral : public tPhysicalEntity
{
public:
	// HYPOTHESIS: the original inlines this constructor into every derived one and it does
	// the work written in the Fissure constructor below; written there, VC6 keeps it inline.
	tEntity_Astral() {}
	virtual ~tEntity_Astral();
	int field_0x1bc;
};

class tEntity_Astral_Fissure : public tEntity_Astral
{
public:
	tEntity_Astral_Fissure();
	virtual ~tEntity_Astral_Fissure();
};

// FUNCTION: STARFLEETOP 0x004117a5
tEntity_Astral_Fissure::tEntity_Astral_Fissure()
{
	field_0x1bc = fn_004f3eda();
	fProperties |= 3;
}
