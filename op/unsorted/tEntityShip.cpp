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
#include "unsorted/tAICaptain.h"
#include "unsorted/fn_0090bed4_Object.h"
#include "unsorted/tSensorGroupSystem.h"

extern float g_008e0cd4[];
extern tEntityMgr g_008e1060;
extern tEntity *g_008e1090[];

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
extern float g_00796ebc;

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

// HYPOTHESIS: the message is read as one dword at +4
// FUNCTION: STARFLEETOP 0x00414cf5
void tEntityShip::fn_00414cf5(tNetMsg *aMsg)
{
	gNetControl.mQueueMsg(aMsg, 0x1a, fID, 0.0);
	field_a10 = 0x1a;
	*((int *)field_a20 + 1) = *((int *)aMsg + 1);
}

// HYPOTHESIS: the message is read as one dword at +4
// FUNCTION: STARFLEETOP 0x00414d2f
void tEntityShip::fn_00414d2f(tNetMsg *aMsg)
{
	gNetControl.mQueueMsg(aMsg, 0x16, fID, 0.0);
	field_a10 = 0x16;
	*((int *)field_a24 + 1) = *((int *)aMsg + 1);
}

// FUNCTION: STARFLEETOP 0x00414d69
int tEntityShip::fn_00414d69()
{
	return field_a94;
}

// FUNCTION: STARFLEETOP 0x00414d70
void tEntityShip::vfunction17()
{
	if (fField5C == 1)
		field_ad4->vfunction113(1.0f);
	tEntity::vfunction17();
}

// HYPOTHESIS: returns the entity's position, or the AI captain's helm target while the flag at
// 0x5c is set.
// FUNCTION: STARFLEETOP 0x00415479
tPoint3 tEntityShip::fn_00415479()
{
	if (fField5C)
	{
		tAICaptain *captain = (tAICaptain *)g_p0090bed4->fn_005235a7(fID);
		// HYPOTHESIS: the original's 0x0051e3a1 returns a pointer to the point; the header declares void.
		tPoint3 *(tAICaptain::*getPoint)() = (tPoint3 *(tAICaptain::*)())&tAICaptain::fn_0051e3a1;
		tPoint3 *point = (captain->*getPoint)();
		if (point)
			return *point;
		return captain->fn_0051e37d();
	}
	else
	{
		tEntity *target = (fTargetID < 0 || fTargetID >= 0x4000) ? 0 : g_008e1090[fTargetID];
		if (target)
			return target->fCurrPos;
		tPoint3 other;
		char flag;
		fn_0041553a(&other, &flag);
		if (flag)
			return other;
		return fCurrPos;
	}
}

// HYPOTHESIS: the object at +0x14 has no declared class with this slot; slot 18 takes the point by value.
// MATCH-ONLY: the call goes through the vtable by hand until the class of fField14 is declared.
// FUNCTION: STARFLEETOP 0x004155c1
void tEntityShip::fn_004155c1(tPoint3 *aPoint)
{
	if (fField14)
	{
		typedef void (tEntityShip::*tSlot18)(tPoint3);
		(((tEntityShip *)fField14)->*(*(tSlot18 *)(*(void ***)fField14 + 0x48 / 4)))(*aPoint);
	}
}

// HYPOTHESIS: slot 22 of the object at +0x14 returns a 16-bit angle; the result relative to
// the angle at 0x90 is folded into -0x1000..0x1000.
// MATCH-ONLY: the call goes through the vtable by hand until the class of fField14 is declared.
// FUNCTION: STARFLEETOP 0x004155ed
short tEntityShip::vfunction66(char param_1)
{
	typedef short (tEntityShip::*tSlot22)();
	if (fField14)
	{
		if (param_1)
		{
			short angle = ((((tEntityShip *)fField14)->*(*(tSlot22 *)(*(void ***)fField14 + 0x58 / 4)))() - field_0x90) & 0x1fff;
			if (angle > 0x1000)
				angle -= 0x2000;
			return angle;
		}
		return (((tEntityShip *)fField14)->*(*(tSlot22 *)(*(void ***)fField14 + 0x58 / 4)))();
	}
	return 0;
}

// HYPOTHESIS: forwards two arguments to slot 17 of the object at +0x14.
// FUNCTION: STARFLEETOP 0x00415628
void tEntityShip::fn_00415628(int param_1, int param_2)
{
	if (fField14)
	{
		typedef void (tEntityShip::*tSlot17)(int, int);
		(((tEntityShip *)fField14)->*(*(tSlot17 *)(*(void ***)fField14 + 0x44 / 4)))(param_1, param_2);
	}
}

// HYPOTHESIS: forwards one argument to slot 21 of the object at +0x14.
// FUNCTION: STARFLEETOP 0x0041563f
void tEntityShip::fn_0041563f(int param_1)
{
	if (fField14)
	{
		typedef void (tEntityShip::*tSlot21)(int);
		(((tEntityShip *)fField14)->*(*(tSlot21 *)(*(void ***)fField14 + 0x54 / 4)))(param_1);
	}
}

// HYPOTHESIS: forwards one argument to slot 20 of the object at +0x14, then calls 0x00419e4a with 0x1f.
// FUNCTION: STARFLEETOP 0x00415652
void tEntityShip::fn_00415652(int param_1)
{
	if (fField14)
	{
		typedef void (tEntityShip::*tSlot20)(int);
		(((tEntityShip *)fField14)->*(*(tSlot20 *)(*(void ***)fField14 + 0x50 / 4)))(param_1);
		fn_00419e4a(0x1f);
	}
}

// HYPOTHESIS: slot 93 of the object at 0xad4 returns an object whose slot 97 takes one argument
// and returns a count; the result is graded by the state at 0x878.
// MATCH-ONLY: the second call goes through the vtable by hand until that object's class is declared.
// FUNCTION: STARFLEETOP 0x00415672
int tEntityShip::fn_00415672(int param_1)
{
	if (field_ad4)
	{
		void *obj = field_ad4->vfunction93();
		if (obj)
		{
			typedef int (tEntityShip::*tSlot97)(int);
			int count = (((tEntityShip *)obj)->*(*(tSlot97 *)(*(void ***)obj + 0x184 / 4)))(param_1);
			if (field_878 <= 11)
			{
				if (field_878 <= 4)
					return count;
				if (count > 0)
					return 4;
			}
		}
	}
	return 0;
}

// HYPOTHESIS: same chain as fn_00415672, returning slot 97's result ungraded.
// MATCH-ONLY: the second call goes through the vtable by hand until that object's class is declared.
// FUNCTION: STARFLEETOP 0x004156b8
int tEntityShip::fn_004156b8(int param_1)
{
	if (field_ad4)
	{
		void *obj = field_ad4->vfunction93();
		if (obj)
		{
			typedef int (tEntityShip::*tSlot97)(int);
			return (((tEntityShip *)obj)->*(*(tSlot97 *)(*(void ***)obj + 0x184 / 4)))(param_1);
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004156e3
fn_004288d1_Object *tEntityShip::fn_004156e3()
{
	field_a44->fn_004288d1();
	return field_a44;
}

// HYPOTHESIS: asks every system of type 0x15 among indices 0x31 to 0x49 (slot 12 gives the type)
// through its slot 156; stops with 0 at the first that answers 0.
// MATCH-ONLY: the system's calls go through the vtable by hand until its class is declared.
// FUNCTION: STARFLEETOP 0x004157ee
int tEntityShip::fn_004157ee(int param_1, int param_2, int param_3)
{
	for (int i = 0; i < 25; i++)
	{
		void *sys = field_ad4->vfunction147(i + 0x31);
		if (sys)
		{
			typedef int (tEntityShip::*tSlot12)();
			typedef int (tEntityShip::*tSlot156)(int, int, int);
			if ((((tEntityShip *)sys)->*(*(tSlot12 *)(*(void ***)sys + 0x30 / 4)))() == 0x15)
			{
				param_1 = (((tEntityShip *)sys)->*(*(tSlot156 *)(*(void ***)sys + 0x270 / 4)))(param_1, param_2, param_3);
				if (param_1 == 0)
					return 0;
			}
		}
	}
	return param_1;
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

// FUNCTION: STARFLEETOP 0x00415aed
bool tEntityShip::fn_00415aed(tEntityShip *aOther)
{
	float theirs = aOther->vfunction14() / (aOther->vfunction63() + 1e-5);
	float mine = vfunction14() / (vfunction63() + 1e-5);
	if (aOther && theirs >= mine)
	{
		if (aOther->vfunction14() != fRadius || aOther->fID >= fID)
			return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00415b6b
float tEntityShip::vfunction63()
{
	if (field_a40 == 7 || field_a40 == 8)
		return 0.0f;
	return field_8a4 / vfunction55();
}

// HYPOTHESIS: a grade for the entity given, 0xd meaning none; while the ship's ID is switched to
// the entity's, the sensor group's answer for 0 is added, capped at 0xc.
// FUNCTION: STARFLEETOP 0x00415ff4
int tEntityShip::vfunction71(tEntity *aEntity)
{
	tID id = vfunction12();
	int result = 0;
	tEntity *entity = g_008e1060.fn_004249c0(aEntity);
	if (entity)
	{
		result = ((tEntityShip *)entity)->vfunction70();
		if (result != 0xd)
		{
			vfunction13(aEntity->fID);
			tSensorGroupSystem *sensors = field_ad4->vfunction84();
			if (sensors)
			{
				result += sensors->fn_0043eca6(0);
				if (result > 0xc)
					result = 0xc;
			}
			vfunction13(id);
		}
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x00416067
void tEntityShip::vfunction13(tID aID)
{
	fTargetID = aID;
	if (fn_00416085())
		fn_004160c2();
}

// FUNCTION: STARFLEETOP 0x00416085
bool tEntityShip::fn_00416085()
{
	bool found = false;
	fn_00416085_Object *ids = field_ad4->vfunction88();
	if (ids)
	{
		for (int i = 0; i < 4; i++)
		{
			if (ids->vfunction116(i) == fTargetID)
				found = true;
		}
	}
	return found;
}

// FUNCTION: STARFLEETOP 0x004160c2
void tEntityShip::fn_004160c2()
{
	fn_00416085_Object *ids = field_ad4->vfunction88();
	if (ids)
	{
		// HYPOTHESIS: the same slot 116 as in 0x00416085, its result unused here
		for (int i = 0; i < 4; i++)
			ids->vfunction116(i);
	}
}

// FUNCTION: STARFLEETOP 0x004160ee
bool tEntityShip::vfunction68()
{
	tObject_00413a8d_arg *&object = field_0x88;
	if (object->vfunction25() - object->vfunction26() > g_00796ebc && field_870 != 4 && field_870 != 5)
		return false;
	return true;
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
