#include "Source/network/NetMsgClass.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tEntity_Seeker.h"

extern tEntityMgr g_008e1060;
// HYPOTHESIS: two int globals the seeker picks between; read only here so far.
extern int g_0085600c;
extern int g_00856010;

// HYPOTHESIS: the fields, from what 0x0041eeaf stores into them; the point at +0x15 is
// written unaligned, so the message is packed.
#pragma pack(push, 1)
struct tNetMsg_kCreateSeekerAtPoint: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
	int fField04;
	int fField08;
	unsigned char fField0c;
	char fField0d;
	char fPad0e[2];
	int fField10;
	char fField14;
	tPoint3 fPoint;
};
#pragma pack(pop)

// HYPOTHESIS: tells every machine to create this seeker at its point, sent six times
// 150 apart.
// FUNCTION: STARFLEETOP 0x0041eeaf
void tEntity_Seeker::fn_0041eeaf()
{
	if (g_008e1060.fn_0042496f(field_0x1c4)) {
		tNetMsg_kCreateSeekerAtPoint msg;
		msg.fField04 = fTargetID;
		msg.fField0d = 0;
		msg.fField0c = '%';
		fn_0041ee42(&msg.fField0c, &msg.fField0d, &msg.fField14);
		msg.fField0d = 0;
		msg.fField10 = 0xc;
		msg.fPoint = fCurrPos;
		if (msg.fField0c == '/')
			msg.fField0c = '%';
		for (int i = 0; i < 900; i += 150) {
			msg.fField08 = g_008e1060.mGetNextPlayerID();
			gNetControl.mQueueMsg(&msg, 0xc, field_0x1c4, i);
		}
	}
}

// HYPOTHESIS: reports event 8 for this seeker, then sends the create message six times
// 150 apart, with a sixth of one of two globals in +0x10 chosen by the seeker's kind.
// FUNCTION: STARFLEETOP 0x0041ef62
void tEntity_Seeker::fn_0041ef62()
{
	tNetMsg_kCreateSeekerAtPoint msg;
	g_0084b47c->vfunction7(8, fID);
	fn_0041ee42(&msg.fField0c, &msg.fField0d, &msg.fField14);
	int value;
	switch (msg.fField0c) {
	case 1:
		value = g_00856010;
		break;
	case 0:
		value = g_0085600c;
		break;
	default:
		value = g_0085600c;
		break;
	}
	msg.fField0c = '0';
	msg.fField0d = 2;
	msg.fField10 = value / 6;
	msg.fField04 = fTargetID;
	msg.fPoint = fCurrPos;
	for (int i = 0; i < 900; i += 150) {
		msg.fField08 = g_008e1060.mGetNextPlayerID();
		gNetControl.mQueueMsg(&msg, 0xc, field_0x1c4, i);
	}
}
