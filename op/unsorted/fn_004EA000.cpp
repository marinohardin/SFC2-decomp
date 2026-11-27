#include "unsorted/tPlasmaLauncher.h"
#include "unsorted/tNetMsg_kCreateSeeker.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tEntityMgr.h"

extern tEntityMgr g_008e1060;

// HYPOTHESIS: tPlasmaDRackTube (RTTI) is the plasma-D rack, a plasma launcher; this unit's
// function sits among its vtable entries and reads the launcher's members at +0x478 and +0x488.
class tPlasmaDRackTube : public tPlasmaLauncher
{
public:
	void fn_004ea379(int param_1, int param_2);
};

// HYPOTHESIS: sends the create message for a seeker the rack launches when it is armed and
// its float at +0x488 is not positive; the second argument is not read.
// FUNCTION: STARFLEETOP 0x004ea379
void tPlasmaDRackTube::fn_004ea379(int param_1, int param_2)
{
	// MATCH-ONLY: the dword at +0x478 lies in tPlasmaLauncher's unnamed padding.
	if (*(int *)&pad14c[0x478 - 0x14c] != 0 && field_488 <= 0.0f)
	{
		tNetMsg_kCreateSeeker msg;
		msg.field_04 = param_1;
		msg.field_08 = g_008e1060.mGetNextPlayerID();
		// MATCH-ONLY: the byte at +0xc4 lies in tBaseWeapon's unnamed padding.
		msg.field_0d = pad_6c[0xc4 - 0x6c];
		msg.field_0e = field_fc;
		msg.field_0c = 5;
		msg.field_10 = 10;
		msg.field_12 = vfunction140();
		gNetControl.mQueueMsg(&msg, 0xb, vfunction37(), 0.0);
	}
}
