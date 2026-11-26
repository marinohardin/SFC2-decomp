// Placeholder written by tools/units.py. Delete this block when the unit's first
// function lands; the generator never touches a file with content.
// Unit:      unsorted/tEntityShip.cpp
// Functions: 0x00414b8f to 0x00416e4f, 62 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <string.h>
#include "unsorted/tEntityShip.h"
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tObject_00918888.h"
#include "Tiger/tvectormath.h"

extern float g_008e0cd4[];

// HYPOTHESIS: the view has a slot 58 (offset 0xe8) taking a size and returning a buffer; it is
// untyped in entity_physical.h, so the call goes through this member pointer (HEADER REQUEST).
typedef void *(tObject_00413ac3_arg::*tSlot58)(int aSize);

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
