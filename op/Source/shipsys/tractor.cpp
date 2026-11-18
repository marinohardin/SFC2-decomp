// Unit:      Source/shipsys/tractor.cpp
// Evidence:  assert-path (the original's assertion strings name this file)
// Profile:   game-release (harness/units.json)

#include "Source/entity/entity_physical.h"
#include "unsorted/tMemoryPart.h"

// HYPOTHESIS: a tractor clump: a growable list of entity IDs (array, count, capacity) and the
// clump's own number, which each member entity keeps in its 0x194 ("fTractorClump").
class tObject_004557ee
{
public:
	void fn_004557ee(int aID);

	int *field_0x0;
	int field_0x4;
	int field_0x8;
	int field_0xc;
};

// FUNCTION: STARFLEETOP 0x004557ee
void tObject_004557ee::fn_004557ee(int aID)
{
	tPhysicalEntity *entity = gObject_008e1060.fn_004249d3(aID);
	if (entity && entity->field_0x194 != field_0xc)
	{
		for (int i = field_0x4 - 1; i >= 0; i--)
		{
			if (field_0x0[i] == aID)
				return;
		}
		if (field_0x8 == 0)
		{
			field_0x0 = (int *)tMemoryPart::Search_tMemoryPart()->mAllocate(16 * 4, 1172, __FILE__);
			field_0x8 = 16;
		}
		else if (field_0x4 == field_0x8)
		{
			field_0x8 += 16;
			field_0x0 = (int *)tMemoryPart::Search_tMemoryPart()->vfunction8(field_0x0, field_0x8 * 4);
		}
		field_0x0[field_0x4] = aID;
		entity->field_0x194 = field_0xc;
		field_0x4++;
	}
}

// HYPOTHESIS: the list's element class; its destructor is reached through the thunk at
// 0x004555f9.
class tObject_0045565f
{
public:
	// HYPOTHESIS: 0x004555d4 builds an item for its slot number.
	// MATCH-ONLY: throw(); the original's new-expression has no unwind frame.
	tObject_0045565f(int aIndex) throw();
	~tObject_0045565f();
	void fn_0045565f();

	// HYPOTHESIS: laid out like tObject_004557ee (array, count, ...); 0x1c bytes.
	int *field_0x0;
	int field_0x4;
	int field_0x8;
	int field_0xc;
	int field_0x10;
	int field_0x14;
	int field_0x18;
};

// HYPOTHESIS: an owner of a list of heap objects (array, count).
class tObject_00455897
{
public:
	void fn_00455897();
	// HYPOTHESIS: looks up the list's item for an ID (0 when absent).
	tObject_0045565f *fn_00455a40(int aID);
	void fn_004558e4(int aID);
	void fn_004558fb(int aID);
	void fn_00455942(tObject_0045565f *aItem, int aID);
	void fn_004559b7(int aID1, int aID2);
	// HYPOTHESIS: a member taking an entity ID (called with this and the first ID).
	void fn_00455624(int aID);
	void fn_00455a1a(int aID1, int aID2, int aUnused);
	// HYPOTHESIS: makes a new list item (an entity not yet in a clump gets one).
	tObject_0045565f *fn_00455a59();

	tObject_0045565f **field_0x0;
	int field_0x4;
};

// FUNCTION: STARFLEETOP 0x00455897
void tObject_00455897::fn_00455897()
{
	if (field_0x0)
	{
		for (int i = 0; i < field_0x4; i++)
		{
			if (field_0x0[i])
			{
				delete field_0x0[i];
				field_0x0[i] = 0;
			}
		}
		tMemoryPart::Search_tMemoryPart()->vfunction10(field_0x0);
		field_0x0 = 0;
		field_0x4 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x004558e4
void tObject_00455897::fn_004558e4(int aID)
{
	tObject_0045565f *item = fn_00455a40(aID);
	if (item)
		item->fn_0045565f();
}

// FUNCTION: STARFLEETOP 0x004558fb
void tObject_00455897::fn_004558fb(int aID)
{
	tPhysicalEntity *entity = gObject_008e1060.fn_004249d3(aID);
	if (entity && entity->field_0x194 == -1 && (entity->field_0x10c != 0 || entity->field_0x110))
	{
		tObject_0045565f *item = fn_00455a59();
		fn_00455942(item, aID);
	}
}

// FUNCTION: STARFLEETOP 0x00455942
void tObject_00455897::fn_00455942(tObject_0045565f *aItem, int aID)
{
	tPhysicalEntity *entity = gObject_008e1060.fn_004249d3(aID);
	if (entity && entity->field_0x194 == -1)
	{
		((tObject_004557ee *)aItem)->fn_004557ee(aID);
		int count = entity->field_0x10c;
		int *ids = entity->field_0x100;
		for (int i = 0; i < count; i++)
			fn_00455942(aItem, ids[i]);
		if (entity->field_0x110)
			fn_00455942(aItem, entity->field_0x104);
	}
}

// FUNCTION: STARFLEETOP 0x004559b7
void tObject_00455897::fn_004559b7(int aID1, int aID2)
{
	tPhysicalEntity *entity1 = gObject_008e1060.fn_004249d3(aID1);
	if (entity1)
	{
		tPhysicalEntity *entity2 = gObject_008e1060.fn_004249d3(aID2);
		if (entity2)
		{
			if (entity1->field_0x194 != -1)
				fn_004558e4(entity1->field_0x194);
			if (entity2->field_0x194 != -1)
				fn_004558e4(entity2->field_0x194);
			fn_004558fb(aID1);
		}
	}
}

// HYPOTHESIS: the third argument is never read (ret 0xc).
// FUNCTION: STARFLEETOP 0x00455a1a
void tObject_00455897::fn_00455a1a(int aID1, int aID2, int aUnused)
{
	fn_00455624(aID1);
	fn_004558fb(aID1);
	fn_004558fb(aID2);
}

// FUNCTION: STARFLEETOP 0x00455a40
tObject_0045565f *tObject_00455897::fn_00455a40(int aID)
{
	if (aID < field_0x4 && aID >= 0)
		return field_0x0[aID];
	return 0;
}

// FUNCTION: STARFLEETOP 0x00455a59
tObject_0045565f *tObject_00455897::fn_00455a59()
{
	for (int i = 0; i < field_0x4; i++)
	{
		if (field_0x0[i]->field_0x4 == 0)
			return field_0x0[i];
	}
	int count = 0;
	if (field_0x4 == 0)
	{
		field_0x0 = (tObject_0045565f **)tMemoryPart::Search_tMemoryPart()->mAllocate(16 * 4, 1388, __FILE__);
		field_0x4 = 16;
		for (int j = 0; j < field_0x4; j++)
			field_0x0[j] = new tObject_0045565f(j);
	}
	else
	{
		count = field_0x4;
		field_0x4 += 16;
		field_0x0 = (tObject_0045565f **)tMemoryPart::Search_tMemoryPart()->vfunction8(field_0x0, field_0x4 * 4);
		for (int j = count; j < field_0x4; j++)
			field_0x0[j] = new tObject_0045565f(j);
	}
	return field_0x0[count];
}
