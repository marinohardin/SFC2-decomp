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
