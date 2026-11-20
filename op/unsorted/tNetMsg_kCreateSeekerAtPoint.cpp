#include "Source/network/NetMsgClass.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/tIndividualShipSystem.h"

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

// HYPOTHESIS: the seeker entity; 0x0041eeaf sits among tEntity_Seeker's functions. Only the
// members 0x0041eeaf uses are listed; the class is not declared elsewhere yet.
class tEntity_Seeker
{
public:
	void fn_0041ee42(unsigned char *a, char *b, char *c);
	void fn_0041eeaf();
	void fn_0041ef62();

	char fField00[0x1c];
	int fField1c;			// +0x1c
	char fField20[0x8];
	int fField28;			// +0x28
	char fField2c[0x8];
	tPoint3 fPosition;		// +0x34
	char fField40[0x184];
	tID fField1c4;			// +0x1c4
};

// HYPOTHESIS: tells every machine to create this seeker at its point, sent six times
// 150 apart.
// FUNCTION: STARFLEETOP 0x0041eeaf
void tEntity_Seeker::fn_0041eeaf()
{
	if (g_008e1060.fn_0042496f(fField1c4)) {
		tNetMsg_kCreateSeekerAtPoint msg;
		msg.fField04 = fField28;
		msg.fField0d = 0;
		msg.fField0c = '%';
		fn_0041ee42(&msg.fField0c, &msg.fField0d, &msg.fField14);
		msg.fField0d = 0;
		msg.fField10 = 0xc;
		msg.fPoint = fPosition;
		if (msg.fField0c == '/')
			msg.fField0c = '%';
		for (int i = 0; i < 900; i += 150) {
			msg.fField08 = g_008e1060.mGetNextPlayerID();
			gNetControl.mQueueMsg(&msg, 0xc, fField1c4, i);
		}
	}
}

// HYPOTHESIS: reports event 8 for this seeker, then sends the create message six times
// 150 apart, with a sixth of one of two globals in +0x10 chosen by the seeker's kind.
// FUNCTION: STARFLEETOP 0x0041ef62
void tEntity_Seeker::fn_0041ef62()
{
	tNetMsg_kCreateSeekerAtPoint msg;
	g_0084b47c->vfunction7(8, fField1c);
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
	msg.fField04 = fField28;
	msg.fPoint = fPosition;
	for (int i = 0; i < 900; i += 150) {
		msg.fField08 = g_008e1060.mGetNextPlayerID();
		gNetControl.mQueueMsg(&msg, 0xc, fField1c4, i);
	}
}
