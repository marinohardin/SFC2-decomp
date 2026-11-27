#include "unsorted/tBaseWeapon.h"
#include "unsorted/tNetMsg_kCreateDirect.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/fn_0090be9c_type.h"

extern tEntityMgr g_008e1060;
extern double g_00799f68;

// HYPOTHESIS: tPhaserA (RTTI) derives from tBaseWeapon; this unit's function sits among its
// vtable entries and calls its 0x004e475a, as tESGLance's twin at 0x004de035 does.
class tPhaserA : public tBaseWeapon
{
public:
	int fn_004e475a();
	void fn_004e492c(float param_1);
};

// HYPOTHESIS: sends the create message for the phaser's shot to its target, delayed by the
// frame time plus three times the argument.
// FUNCTION: STARFLEETOP 0x004e492c
void tPhaserA::fn_004e492c(float param_1)
{
	int value = fn_004e475a();
	tNetMsg_kCreateDirect msg;
	msg.field_0x4 = field_ec;
	msg.field_0x8 = g_008e1060.mGetNextPlayerID();
	// MATCH-ONLY: the byte at +0xc4 lies in tBaseWeapon's unnamed padding.
	msg.field_0xd = pad_6c[0xc4 - 0x6c];
	msg.field_0xc = (unsigned char)field_f8;
	msg.field_0xe = field_fc;
	msg.field_0x10 = value;
	msg.field_0x14 = (unsigned char)vfunction140();
	fn_0090be9c_type *source = g_0090be9c;
	unsigned int count = source->vfunction7();
	unsigned int frame = (int)(count * g_00799f68 / source->field_20);
	gNetControl.mQueueMsg(&msg, 6, vfunction37(), param_1 * 3.0f + (frame + 1) * 50);
}
