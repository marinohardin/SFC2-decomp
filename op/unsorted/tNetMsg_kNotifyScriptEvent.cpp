#include "unsorted/tIndividualShipSystem.h"
#include "Source/network/NetMsgClass.h"
#include "unsorted/tNetControl.h"

#include "unsorted/fn_0090bed4_Object.h"
#include "unsorted/tObject_0090be60.h"

extern char g_0083bddc[];

// HYPOTHESIS: the fields, from what 0x004102a0 stores into them.
class tNetMsg_kNotifyScriptEvent: public tNetMsg
{
public:
	virtual const tNetMsg::tMapItem *mGetMap(void) const;
	char fFlag;
	int fValue;
};

// HYPOTHESIS: returns the address of a static descriptor of the message class
// FUNCTION: STARFLEETOP 0x004101c8
const tNetMsg::tMapItem *tNetMsg_kNotifyScriptEvent::mGetMap(void) const
{
	return (const tNetMsg::tMapItem *)g_0083bddc;
}

// FUNCTION: STARFLEETOP 0x0041021f
char vfunction33_result::fn_0041021f()
{
	return field_5c;
}

// Unclaimed (0x00410223): the original shares one four-byte getter between unrelated classes (tShip and
// c_00546027 call it too).
int vfunction33_result::fn_00410223()
{
	return field_64;
}

// FUNCTION: STARFLEETOP 0x00410227
void vfunction33_result::fn_00410227(int value)
{
	field_64 = value;
}

// HYPOTHESIS: returns field_64 and resets it to -1
// FUNCTION: STARFLEETOP 0x00410231
int vfunction33_result::fn_00410231()
{
	int old = field_64;
	field_64 = -1;
	return old;
}

// FUNCTION: STARFLEETOP 0x00410239
void vfunction33_result::fn_00410239(int value)
{
	field_68 = value;
}

// HYPOTHESIS: the team list 0x00426f1a returns; 0x00426b63 removes an entity ID from it and
// 0x00426aef adds one.
class tObject_00426b63_list
{
public:
	void fn_00426b63(int aID);
	void fn_00426aef(int aID, int aValue);
};

// HYPOTHESIS: the team of an entity changes; its +0x64 entry is forgotten first.
// FUNCTION: STARFLEETOP 0x00410243
void vfunction33_result::fn_00410243(int team, int value)
{
	if (field_64 != -1)
		g_p0090bed4->fn_00522b94(this, team);
	tObject_00426b63_list *list = (tObject_00426b63_list *)gObject_0090be60->fn_00426f1a(field_60);
	if (list)
		list->fn_00426b63(field_1c);
	list = (tObject_00426b63_list *)gObject_0090be60->fn_00426f1a(team);
	if (list) {
		field_60 = team;
		list->fn_00426aef(field_1c, value);
	}
}

// HYPOTHESIS: tells the other machines about a script event, by entity ID.
// FUNCTION: STARFLEETOP 0x004102a0
void vfunction33_result::fn_004102a0(int value)
{
	tNetMsg_kNotifyScriptEvent msg;
	msg.fFlag = 1;
	msg.fValue = value;
	gNetControl.mQueueMsg(&msg, 0x3a, field_1c, -1.0);
}
