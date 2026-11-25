// tSeqScene.cpp: the class declaration only; the functions are written one at a time.

#include "unsorted/tSeqScene.h"
#include "unsorted/tScrollBar.h"

// FUNCTION: STARFLEETOP 0x0067dbc0
void tSeqScene::fn_0067dbc0()
{
	field_E8 = 0;
	field_EC &= ~1;
	field_F0 = 0;
	field_F4 = -1;
	field_F8 = -1;
	field_FC = 10;
	field_100 = 0;
	field_104 = "";
	field_130 = "";
}

// FUNCTION: STARFLEETOP 0x0067dcb0
void tSeqScene::vfunction43()
{
	field_F0 = 0;
	field_EC &= ~1;
	field_D8 = field_D4;
	if (field_D4 && field_E0)
		vfunction41(field_D4);
}

// FUNCTION: STARFLEETOP 0x0067dd40
void tSeqScene::vfunction40()
{
	if (field_D4)
		vfunction43();
	tVisGroup::vfunction40();
	if (field_D4)
	{
		field_EC |= 1;
		field_E8 = fn_00672870();
	}
}

// FUNCTION: STARFLEETOP 0x0067dd90
void tSeqScene::fn_0067dd90()
{
	field_EC &= ~1;
}

// FUNCTION: STARFLEETOP 0x0067dda0
void tSeqScene::fn_0067dda0()
{
	field_EC |= 1;
	field_E8 = fn_00672870() - (1000 / (unsigned long)field_FC) * field_F0;
}

// FUNCTION: STARFLEETOP 0x0067dde0
void tSeqScene::fn_0067dde0()
{
	field_EC |= 1;
	field_F0 = field_E0;
}

// HYPOTHESIS: the object at 0x00c690d0 that holds the groups; 0x00674250 takes the object at +0x50.
class fn_00674250_Owner
{
public:
	void fn_00674250(int param_1);
};

extern fn_00674250_Owner *g_00c690d0;

// FUNCTION: STARFLEETOP 0x0067e1a0
void tSeqScene::vfunction44()
{
	field_68.fn_0067f090(this, 1, 0);
	int object = field_12C;
	if (object && g_00c690d0)
		g_00c690d0->fn_00674250(object);
}

// HYPOTHESIS: a 0x20-byte event record (the caller's frame); code 10, the object's id at +0xc,
// the sending object at +0x10.
struct fn_006932f0_Event
{
	int code;
	int field_4;
	int field_8;
	int id;
	void *sender;
	int field_14;
	int field_18;
	int field_1C;
};

// HYPOTHESIS: 0x006932f0 lies inside tEvtMgr's functions (0x006930d0 to 0x006933d0) and is
// called on the global at 0x00c69828; it posts the event. tEvtMgr has no header yet.
class tEvtMgr
{
public:
	void fn_006932f0(fn_006932f0_Event *event);
};

extern tEvtMgr g_00c69828;

// FUNCTION: STARFLEETOP 0x0067e1d0
bool tSeqScene::fn_0067e1d0()
{
	if ((unsigned long)field_F0 < (unsigned long)field_E0)
	{
		field_D8 = vfunction41(field_D8);
		field_F0++;
	}
	else if (field_EC & 1)
	{
		fn_006932f0_Event event;
		event.id = field_48;
		event.code = 10;
		event.sender = this;
		g_00c69828.fn_006932f0(&event);
		vfunction44();
		if (field_EC & 1)
		{
			if (field_F8 != -1 && field_F4 != -1 && (unsigned long)field_E0 > 1)
			{
				field_D8 = field_F4 + field_D4;
				field_F0 = field_F8;
				vfunction41(field_D8);
				unsigned long now = fn_00672870();
				field_E8 = now - (1000 / (unsigned long)field_FC) * field_F0;
			}
			else
				field_EC &= ~1;
		}
		return false;
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x0067e2d0
void tSeqScene::vfunction30()
{
	// HYPOTHESIS: +0x20 is a tObjBase field not declared yet.
	if (*(int *)((char *)this + 0x20))
		tVisGroup::vfunction30();
	if (field_EC & 1)
	{
		unsigned long elapsed = fn_00672870() - field_E8;
		while (elapsed >= (1000 / (unsigned long)field_FC) * field_F0 && fn_0067e1d0())
			;
	}
}
