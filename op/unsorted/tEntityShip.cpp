// Placeholder written by tools/units.py. Delete this block when the unit's first
// function lands; the generator never touches a file with content.
// Unit:      unsorted/tEntityShip.cpp
// Functions: 0x00414b8f to 0x00416e4f, 62 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)
struct tNetMsg;
#include <string.h>
#include "unsorted/tEntityShip.h"
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tObject_00918888.h"
#include "Tiger/tvectormath.h"
#include "unsorted/tEntityMgr.h"
#include "unsorted/tCloakSystem.h"
#include "unsorted/tObject_0090be60.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tObject_0096ed60.h"
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tNetControl.h"

extern float g_008e0cd4[];
extern tEntityMgr g_008e1060;

// HYPOTHESIS: slot 73 of the object at 0x0092deb4 takes three arguments here; tObject_0092deb4.h
// declares it with two (HEADER REQUEST).
typedef void (tObject_0092deb4::*tSlot73)(int a, int b, int c);

bool __stdcall fn_004274b8(tID aID, int aValue);

// HYPOTHESIS: the view has a slot 58 (offset 0xe8) taking a size and returning a buffer; it is
// untyped in entity_physical.h, so the call goes through this member pointer (HEADER REQUEST).
typedef void *(tObject_00413ac3_arg::*tSlot58)(int aSize);

// HYPOTHESIS: tests whether an angle lies within a span around a centre, in 0x2000-step angle units.
// FUNCTION: STARFLEETOP 0x00414bab
bool __stdcall fn_00414bab(int aCenter, short aAngle, int aSpan)
{
	short lo = (aCenter - aSpan) & 0x1fff;
	short hi = (aCenter + aSpan) & 0x1fff;
	if (lo < hi)
	{
		if (lo <= aAngle && aAngle <= hi)
			return true;
	}
	else if ((lo <= aAngle && aAngle <= 0x2000) || (aAngle >= 0 && aAngle <= hi))
		return true;
	return false;
}

extern double g_00c7c148;
extern double g_00797040;

// HYPOTHESIS: the message's angle (short at +4) and flag (byte at +6) are read through a cast; the
// view of the message at 0x00414bfa is not declared yet. The result of slot 33's class is read
// through the entity itself.
// FUNCTION: STARFLEETOP 0x00414bfa
void tEntityShip::fn_00414bfa(tNetMsg *aMsg)
{
	// HYPOTHESIS: the original passes the centre as a short
	bool (__stdcall *angleTest)(short, short, int) = (bool (__stdcall *)(short, short, int))fn_00414bab;
	double now = g_00c7c148;
	double last = field_ac0;
	if (now <= last + g_00797040 || angleTest(*(short *)((char *)field_a18 + 4), *(short *)((char *)aMsg + 4), 0x16))
	{
		if (((vfunction33_result *)this)->fn_0041021f())
			return;
		if (angleTest(*(short *)((char *)field_a18 + 4), *(short *)((char *)aMsg + 4), 0x72))
			return;
	}
	field_ac0 = now;
	gNetControl.mQueueMsg(aMsg, 0x18, fID, 0.0);
	field_a10 = 0x18;
	char *cur = (char *)field_a18;
	*(short *)(cur + 4) = *(short *)((char *)aMsg + 4);
	cur[6] = ((char *)aMsg)[6];
}

// HYPOTHESIS: the message is read as three dwords at +4, +8 and +0xc
// FUNCTION: STARFLEETOP 0x00414cad
void tEntityShip::fn_00414cad(tNetMsg *aMsg)
{
	gNetControl.mQueueMsg(aMsg, 0x19, fID, 0.0);
	field_a10 = 0x19;
	int *cur = (int *)field_a1c + 1;
	int *msg = (int *)aMsg + 1;
	cur[0] = msg[0];
	cur[1] = msg[1];
	cur[2] = msg[2];
}

// FUNCTION: STARFLEETOP 0x00414b8f
// FUNCTION: STARFLEETOP 0x004158c1
tEntityShip::~tEntityShip()
{
	// HYPOTHESIS: only the releases at the tail of the original are written; its opening part
	// (a loop of 0x19 steps over locals, calls through an object at -0x40) is not recovered yet.
	if (field_1bc)
	{
		delete field_1bc;
		field_1bc = 0;
	}
	if (field_a44)
	{
		delete field_a44;
		field_a44 = 0;
	}
	for (int i = 0; i < 10; i++)
	{
		if (field_a54[i])
			delete field_a54[i];
	}
	if (field_a18)
		delete field_a18;
	if (field_a1c)
		delete field_a1c;
	if (field_a20)
		delete field_a20;
	if (field_a24)
		delete field_a24;
}

// FUNCTION: STARFLEETOP 0x004162b9
void tEntityShip::vfunction4(tObject_00413ac3_arg *aObject)
{
	static tObject_0040ce21 marker("Look at the Pretty colors");

	if (field_0x88 && (fProperties & 1) && field_878 < 0xd)
	{
		tMatrix *matrix = vfunction28(false);
		aObject->vfunction57(matrix);
		if (gObject_00918888->vfunction8() == 5)
			vfunction67(aObject);

		float a = field_ad4->vfunction34()->field_18;
		float b = field_ad4->vfunction34()->field_20;
		float c = field_ad4->vfunction34()->field_1c;
		tObject_00416d41_bounds *bounds = field_ad4->vfunction34();
		float ratio = (a - b) / (c - bounds->field_20);

		if (vfunction32())
		{
			// MATCH-ONLY: the original calls slot 7 (offset 0x1c) of the object at 0x88 here; its
			// header gives slot 7 no arguments yet, so slot 6 stands in until it does (HEADER REQUEST).
			int state = field_870;
			if (state >= 0x18)
				field_0x88->vfunction6(aObject, vfunction74(0.0f), 0);
			else if (state > 0xd && state <= 0x12)
				field_0x88->vfunction6(aObject, vfunction74(1.0f - ratio), 2);
			else if (state == 6)
				field_0x88->vfunction6(aObject, vfunction74(1.0f - ratio), 1);
			else
				field_0x88->vfunction6(aObject, vfunction74(1.0f - ratio), 0);
		}
		else if (fn_00419c87())
		{
			tObject_00416d41_list *list = field_ad4->vfunction86();
			int count = 0;
			if (list != 0)
				count = list->vfunction9();

			int i = 0;
			float *p = g_008e0cd4;
			while (i < count || p == g_008e0cd4)
			{
				tMatrix m = *vfunction28(false);
				m.mPostTranslate(p[-1], p[0], p[1]);
				aObject->vfunction57(&m);
				field_0x88->vfunction6(aObject, 0.0f, 0);
				i++;
				p += 3;
			}
		}
		else
		{
			int state = field_870;
			if (state >= 0x18)
				field_0x88->vfunction6(aObject, 0.0f, 0);
			else if (state > 0xd && state <= 0x12)
				field_0x88->vfunction6(aObject, 1.0f - ratio, 2);
			else if (state == 6)
				field_0x88->vfunction6(aObject, 1.0f - ratio, 1);
			else
				field_0x88->vfunction6(aObject, 1.0f - ratio, 0);
		}
	}

	fn_004103e2(aObject);

	if (field_1bc)
	{
		tMatrix *matrix = vfunction28(false);
		tSlot58 slot58 = (tSlot58)&tObject_00413ac3_arg::vfunction58;
		void *saved = (aObject->*slot58)(0x44);
		if (memcmp(matrix, saved, 0x44) != 0)
			aObject->vfunction57(matrix);
		field_1bc->fn_00426563(aObject);
	}
}

// FUNCTION: STARFLEETOP 0x00416a0e
void tEntityShip::vfunction26(short a1, int a2, int a3, int a4, int a5)
{
	if (field_ad4 == 0)
		return;
	if (field_870 == 0xf)
		return;

	g_008e1060.fn_004235cd(a3, a5);

	int kind = a4;
	if ((kind == 8 || kind == 10 || kind == 0xb) && vfunction32())
	{
		tCloakSystem *cloak = field_ad4->vfunction95();
		if (cloak)
			cloak->fn_00438390();
	}

	int hit =field_ad4->vfunction144(a1, a2, a3, kind, a5);
	if (kind == 6 || kind == 7)
	{
		if (field_ad4->vfunction16())
			vfunction69(0);
		return;
	}

	gObject_0090be60->fn_004276bf(fTeam, field_ad4->vfunction154(), 2);
	gObject_0090be60->fn_004276bf(fTeam, field_ad4->vfunction155(), 1);
	if (hit && field_1bc)
		field_1bc->fn_00426886(a1);

	if (field_ad4->vfunction16())
	{
		vfunction69(0);
		if (gObject_0092deb4->vfunction7() != 0 && gObject_0092deb4->vfunction7() != 1)
			return;
		if (!fn_004274b8(a5, gObject_008e1060.fn_00525081()))
			return;
		tSlot73 slot73 = (tSlot73)&tObject_0092deb4::vfunction73;
		(gObject_0092deb4->*slot73)(0, gObject_0096ed60.fn_0052ca48((char *)&field_87d), 1);
		return;
	}

	int arg;
	if (kind == 9)
		arg = 1;
	else if (kind == 10)
		arg = 2;
	else
		arg = 0;
	fn_004102d9(0x80, a5, arg, 0, 0);
	if (!field_ad4->vfunction16())
		fn_00419e84(a5);
}
