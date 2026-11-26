// Unit:      unsorted/tShipUI.cpp
// Functions: 0x004cd034 to 0x004cd115, 3 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/tShipUI.h"
#include "unsorted/fn_004D3143.h"
#include "unsorted/tValueBase.h"

// HYPOTHESIS: the value objects the constructor binds to names; their type is not known.
extern tValueBase g_009257e8;
extern tValueBase g_009257a8;
extern tValueBase g_00925768;
extern tValueBase g_00925728;
extern tValueBase g_00925708;
extern tValueBase g_009256e8;
extern tValueBase g_009256c8;
extern char g_00925548[];
extern const char* g_00854fd4[];

// FUNCTION: STARFLEETOP 0x004cd034
tShipUI::tShipUI()
{
	m_100c = 0;
	m_1010 = 0;
	m_1018 = 0;
	m_101c = 0;
	m_1020 = 0;
	m_1100 = 0;
	fn_004d35ac(&g_009257e8, "ShipIndex");
	fn_004d35ac(&g_009257a8, "VariantText");
	fn_004d35ac(&g_00925768, "VesselStats");
	fn_004d35ac(&g_00925728, "VesselName");
	fn_004d35ac(&g_00925708, "cVesselProfile");
	fn_004d35ac(&g_009256e8, "cTurnClass");
	fn_004d35ac(&g_009256c8, "cMoveCost");
	char* v = g_00925548;
	const char** n = g_00854fd4;
	do
	{
		fn_004d35ac((tValueBase*)v, *n);
		n++;
		v += 0x40;
	}
	while ((int)n < 0x00854fec);	// HYPOTHESIS: the end is the address after the table, as a constant
}
