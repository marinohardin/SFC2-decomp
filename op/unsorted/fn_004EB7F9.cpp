#include "unsorted/tBaseWeapon.h"
#include "unsorted/tNetMsg_kCreateDirect.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/fn_0090be9c_type.h"

extern tEntityMgr g_008e1060;
extern double g_00799f68;

// HYPOTHESIS: tTRBeam (RTTI) derives from tBaseWeapon; this unit's function sits among its
// vtable entries and calls its 0x004eb5a7, as tPhaserA's twin at 0x004e492c does. It has no
// shared header yet, so it is declared here.
class tTRBeam : public tBaseWeapon
{
public:
	int fn_004eb5a7();
	void fn_004eb7f9(float param_1);
};

// HYPOTHESIS: sends the create message for the beam's shot to its target, delayed by the
// frame time plus three times the argument.
// FUNCTION: STARFLEETOP 0x004eb7f9
void tTRBeam::fn_004eb7f9(float param_1)
{
	int value = fn_004eb5a7();
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

// HYPOTHESIS: tTRBeamLight_Creator (RTTI) registers itself like tPhaserA_Creator; its
// constructor is 0x004eb9fa in its own unit. It has no shared header yet, so it is declared here.
class tTRBeamLight_Creator
{
public:
	tTRBeamLight_Creator();
	virtual ~tTRBeamLight_Creator();
};

// FUNCTION: STARFLEETOP 0x004eb9e4
// FUNCTION: STARFLEETOP 0x004eb9da
tTRBeamLight_Creator g_0092f220;
