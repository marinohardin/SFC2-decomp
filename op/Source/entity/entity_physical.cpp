// Unit:      Source/entity/entity_physical.cpp
// Evidence:  assert-path (the original's assertion strings name this file)
// Profile:   game-release (harness/units.json)

#include "Source/entity/entity_physical.h"
#include "Tiger/tpart.h"
#include "unsorted/tNetControl.h"
#include <string.h>

#include "unsorted/tObject_0040ce21.h"








// HYPOTHESIS: the memory part, bound through the SDK's GenAbstractBinding (its
// Search_tMemoryPart is the copy at 0x004133d3); tPart supplies slots 0 to 5, slot 6 is a
// placeholder and slot 7 (offset 0x1c) allocates (size, line, file).
class tMemoryPart: public tPart
{
public:
	// FUNCTION: STARFLEETOP 0x004133d3
	GenAbstractBinding( tMemoryPart );
	virtual void vfunction6();
	virtual void *mAllocate(int aSize, int aLine, const char *aFile);
	// HYPOTHESIS: slot 8 (offset 0x20) resizes a block from mAllocate to the new size and
	// returns it; 0x00413c9a grows its ID array through it.
	virtual void *vfunction8(void *aBlock, int aSize);
	virtual void vfunction9();
	// HYPOTHESIS: slot 10 (offset 0x28) frees a block from mAllocate; the destructor
	// 0x00413de0 passes it each ID array before clearing the pointer.
	virtual void vfunction10(void *aBlock);
};

// HYPOTHESIS: the stream archive helpers of the SDK's sfcstreams (ArchiveInt32TM and its
// siblings); which sibling each address is has not been confirmed, so they keep their
// addresses as names. The tag-less ones are called as (stream, mode).
void fn_0040e689(tSFCStream &s);
void fn_0040e691(tSFCStream &s);
void fn_0040ed22(tSFCStream &s, unsigned mode);
void fn_0040ecfd(const char *aTitle, tSFCStream &s, unsigned mode);
void fn_0040ec85(int aSize, void *aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040e9df(int &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eafc(bool &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eb35(float &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040ecc4(char *aValue, tSFCStream &s, unsigned mode, const char *aText);

// The 16 objects held at +0x10030 of the object at 0x008e1060; only slot 66 is called here.
// HYPOTHESIS: the class is inferred from 0x00413448 only.
class tObject_008e1060_slot
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14();
	virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17();
	virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20();
	virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26();
	virtual void vfunction27(); virtual void vfunction28(); virtual void vfunction29();
	virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
	virtual void vfunction33(); virtual void vfunction34(); virtual void vfunction35();
	virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38();
	virtual void vfunction39(); virtual void vfunction40(); virtual void vfunction41();
	virtual void vfunction42(); virtual void vfunction43(); virtual void vfunction44();
	virtual void vfunction45(); virtual void vfunction46(); virtual void vfunction47();
	virtual void vfunction48(); virtual void vfunction49(); virtual void vfunction50();
	virtual void vfunction51(); virtual void vfunction52(); virtual void vfunction53();
	virtual void vfunction54(); virtual void vfunction55(); virtual void vfunction56();
	virtual void vfunction57(); virtual void vfunction58(); virtual void vfunction59();
	virtual void vfunction60(); virtual void vfunction61(); virtual void vfunction62();
	virtual void vfunction63(); virtual void vfunction64(); virtual void vfunction65();
	virtual void vfunction66(tPhysicalEntity *aEntity);
};

// HYPOTHESIS: the object that 0x00413a37 reaches through slot 67 (offset 0x10c) of the
// object at 0xad4; only slot 80 (offset 0x140) is called, without arguments.
class tObject_00410fd4_slot
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14();
	virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17();
	virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20();
	virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26();
	virtual void vfunction27(); virtual void vfunction28(); virtual void vfunction29();
	virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
	virtual void vfunction33(); virtual void vfunction34(); virtual void vfunction35();
	virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38();
	virtual void vfunction39(); virtual void vfunction40(); virtual void vfunction41();
	virtual void vfunction42(); virtual void vfunction43(); virtual void vfunction44();
	virtual void vfunction45(); virtual void vfunction46(); virtual void vfunction47();
	virtual void vfunction48(); virtual void vfunction49(); virtual void vfunction50();
	virtual void vfunction51(); virtual void vfunction52(); virtual void vfunction53();
	virtual void vfunction54(); virtual void vfunction55(); virtual void vfunction56();
	virtual void vfunction57(); virtual void vfunction58(); virtual void vfunction59();
	virtual void vfunction60(); virtual void vfunction61(); virtual void vfunction62();
	virtual void vfunction63(); virtual void vfunction64(); virtual void vfunction65();
	virtual void vfunction66(); virtual void vfunction67(); virtual void vfunction68();
	virtual void vfunction69(); virtual void vfunction70(); virtual void vfunction71();
	virtual void vfunction72(); virtual void vfunction73(); virtual void vfunction74();
	virtual void vfunction75(); virtual void vfunction76(); virtual void vfunction77();
	virtual void vfunction78(); virtual void vfunction79(); virtual void vfunction80();
};

// HYPOTHESIS: the object held at 0xad4 of the result of 0x00410fd4; only slot 67 (offset
// 0x10c) is called, without arguments, returning a pointer.
class tObject_00410fd4_member
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14();
	virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17();
	virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20();
	virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26();
	virtual void vfunction27(); virtual void vfunction28(); virtual void vfunction29();
	virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
	virtual void vfunction33(); virtual void vfunction34(); virtual void vfunction35();
	virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38();
	virtual void vfunction39(); virtual void vfunction40(); virtual void vfunction41();
	virtual void vfunction42(); virtual void vfunction43(); virtual void vfunction44();
	virtual void vfunction45(); virtual void vfunction46(); virtual void vfunction47();
	virtual void vfunction48(); virtual void vfunction49(); virtual void vfunction50();
	virtual void vfunction51(); virtual void vfunction52(); virtual void vfunction53();
	virtual void vfunction54(); virtual void vfunction55(); virtual void vfunction56();
	virtual void vfunction57(); virtual void vfunction58(); virtual void vfunction59();
	virtual void vfunction60(); virtual void vfunction61(); virtual void vfunction62();
	virtual void vfunction63(); virtual void vfunction64(); virtual void vfunction65();
	virtual void vfunction66();
	virtual tObject_00410fd4_slot *vfunction67();
};

// HYPOTHESIS: what 0x00410fd4 returns for an entity; only the pointer at 0xad4 is known.
class tObject_00410fd4
{
public:
	char field_0x0[0xad4];
	tObject_00410fd4_member *field_0xad4;
};

#include "Source/network/NetMsgClass.h"

























// HYPOTHESIS: the fields, from what 0x00413541 stores into them.
struct tNetMsg_kDamage: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	short fDirection;
	short fAmount;
	int fSeed;
	char fFlags;
	int fID;
};

// HYPOTHESIS: the field, from what 0x00413541 stores into it.
struct tNetMsg_kAIChangeTeamRelationshipToWar: public tNetMsg
{
	tNetMsg_kAIChangeTeamRelationshipToWar(int aID): fID(aID) {}
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	int fID;
};

// HYPOTHESIS: the fields are not yet known; the map at 0x004139c4 decides them.
struct tNetMsg_kTractorChangeMessage: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	int fID;
};

// HYPOTHESIS: the fields are not yet known; the map at 0x004139ca decides them.
struct tNetMsg_kDeepScan: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	int fID;
};

// HYPOTHESIS: one one-byte field, set to 1 by 0x004139ec; its meaning is not known.
struct tNetMsg_kKillEntity: public tNetMsg
{
	virtual const tNetMsg::tMapItem *mGetMap(void) const;

	bool field_0x4;
};

// The object that the pointer at 0x0090be60 holds.
// HYPOTHESIS: the class is inferred from 0x00413541 only; fn_00427581 returns 5 for the
// relation that sends kAIChangeTeamRelationshipToWar.
#include "unsorted/tObject_0090be60.h"






extern tObject_0090be60 *gObject_0090be60;

// FUNCTION: STARFLEETOP 0x00413448
void tPhysicalEntity::vfunction3()
{
	int i;
	tObject_008e1060_slot *slot;
	tPhysicalEntity *entity;

	fn_004101d3();
	for (i = 0; i < 16; i++)
	{
		slot = gObject_008e1060.fn_slot(i);
		if (slot == 0)
			break;
		slot->vfunction66(this);
	}
	if (field_0x18c > 0 && field_0x190)
	{
		entity = gObject_008e1060.fn_00424a2f(field_0x18c);
		if (entity)
			entity->vfunction27(-1, 0, 10001, 0, 12);
		field_0x18c = -1;
		field_0x190 = false;
	}
}

// FUNCTION: STARFLEETOP 0x00413153
void tPhysicalEntity::vfunction11(tSFCStream &s, int32u mode)
{
	int i;

	tEntity::vfunction11(s, mode);
	fn_0040ed22(s, mode);
	fn_0040ecfd("Entity physical:", s, mode);
	fn_0040e689(s);
	fn_0040ec85(2, &field_0x92, s, mode, "fRotX");
	fn_0040ec85(2, &field_0x90, s, mode, "fRotY");
	fn_0040ec85(2, &field_0x94, s, mode, "fRotZ");
	fn_0040eafc(field_0x110, s, mode, "fWeAreBeingTractored");
	fn_0040e9df(field_0x118, s, mode, "fMaxNumberWeAreBeingTractoredBy");
	fn_0040e9df(field_0x104, s, mode, "fEntityWeAreBeingTractoredByID");
	fn_0040e9df(field_0x10c, s, mode, "fNumberWeAreTractoring");
	fn_0040e9df(field_0x114, s, mode, "fMaxNumberWeAreTractoring");
	if (field_0x10c > 0)
	{
		if (mode & 1)
		{
			field_0x100 = (int *)tMemoryPart::Search_tMemoryPart()->mAllocate(field_0x114 * 4, 131,
				"F:\\Projects\\Taldren\\Projects\\SFC2_5\\Source\\entity\\entity_physical.cpp");
		}
		for (i = 0; i < field_0x10c; i++)
			fn_0040e9df(field_0x100[i], s, mode, "fEntitiesWeAreTractoringIDs[i]");
	}
	else
	{
		field_0x100 = 0;
	}
	fn_0040e9df(field_0x184, s, mode, "fNumberOfThreats");
	fn_0040e9df(field_0x188, s, mode, "fMaxNumberOfThreats");
	if (mode & 1)
	{
		field_0x180 = (int *)tMemoryPart::Search_tMemoryPart()->mAllocate(field_0x188 * 4, 146,
			"F:\\Projects\\Taldren\\Projects\\SFC2_5\\Source\\entity\\entity_physical.cpp");
	}
	for (i = 0; i < field_0x10c; i++)
		fn_0040e9df(field_0x180[i], s, mode, "fThreatListIDs[i]");
	fn_0040e9df(field_0x194, s, mode, "fTractorClump");
	fn_0040ecc4((char *)&field_0x11c, s, mode, "fUIName");
	fn_0040ecc4((char *)&field_0x11c, s, mode, "fInstanceName");
	fn_0040eb35(field_0x17c, s, mode, "fMovementRaio");
	fn_0040eafc(field_0x190, s, mode, "fVoidedWildWeasels");
	fn_0040e9df(field_0x18c, s, mode, "fWildWeasel");
	fn_0040e9df(field_0x1b4, s, mode, "fHealth");
	fn_0040e9df(field_0x1b8, s, mode, "fMaxHealth");
	fn_0040e691(s);
}

// FUNCTION: STARFLEETOP 0x004134ce
// HYPOTHESIS: copies a name into the 32-byte member at 0x13c that vfunction42 returns,
// truncated and terminated at 31 characters.
void tPhysicalEntity::vfunction43(char *aName)
{
	strncpy(field_0x13c, aName, 31);
	field_0x13c[31] = 0;
}

// FUNCTION: STARFLEETOP 0x004134f1
// HYPOTHESIS: copies a name into the 32-byte member at 0x15c that vfunction44 returns,
// truncated and terminated at 31 characters; a null name leaves it unchanged.
void tPhysicalEntity::vfunction45(char *aName)
{
	if (aName)
	{
		strncpy(field_0x15c, aName, 31);
		field_0x15c[31] = 0;
	}
}

// FUNCTION: STARFLEETOP 0x0041351b
// HYPOTHESIS: applies damage; fHealth (0x1b4) becomes vfunction23's result less the second
// argument times 128, and vfunction5 runs once it is no longer positive.
void tPhysicalEntity::vfunction26(int a1, int a2, int a3, int a4, int a5)
{
	field_0x1b4 = vfunction23() - a2 * 128;
	if (field_0x1b4 <= 0)
		vfunction5();
}

// FUNCTION: STARFLEETOP 0x00413541
// HYPOTHESIS: sends damage from entity aID to the other machines; when this entity has a
// team (0x64) and aID's team is related to it as 5, also tells the team aID is at war.
void tPhysicalEntity::vfunction27(int aID, short aDirection, unsigned aAmount, int aSeed, char aFlags)
{
	if (aAmount > 0)
	{
		tNetMsg_kDamage msg;
		msg.fDirection = aDirection;
		msg.fAmount = (short)aAmount;
		msg.fSeed = gObject_008e1060.fn_004235ac(aSeed);
		msg.fFlags = aFlags;
		msg.fID = aID;
		gNetControl.mQueueMsg(&msg, tNetMsg::kDamage, fID, 0.0);

		if (fField64 != -1)
		{
			tPhysicalEntity *entity = gObject_008e1060.fn_entity(aID);
			if (entity)
			{
				int relation = gObject_0090be60->fn_0042748f(fID);
				int team = entity->fTeam;
				if (gObject_0090be60->fn_00427581(relation, team) == 5)
				{
					tNetMsg_kAIChangeTeamRelationshipToWar war(aID);
					gNetControl.mQueueMsg(&war, tNetMsg::kAIChangeTeamRelationshipToWar,
						fField64, 0.0);
				}
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x0041360e
// HYPOTHESIS: the map lists fID only, as the SDK's messages build theirs (NetMsgClass.h).
const tNetMsg::tMapItem *tNetMsg_kAIChangeTeamRelationshipToWar::mGetMap(void) const
{
	static tNetMsg::tMapItem map[] =
	{
		MAKE_NMMI(tNetMsg_kAIChangeTeamRelationshipToWar, fID),

		{	0, 0	}	// Terminator
	};
	return(map);
}

// FUNCTION: STARFLEETOP 0x00413614
// HYPOTHESIS: not source code: the original is `mov eax, <funcinfo>; call <frame handler>`,
// the compiler's exception-handler thunk for the unwind data of the function before it.
// This stub only holds the address.
void tPhysicalEntity::vfunction7(tObject_0041018e_arg *param_1, int param_2)
{
}

// FUNCTION: STARFLEETOP 0x004139c4
const tNetMsg::tMapItem *tNetMsg_kTractorChangeMessage::mGetMap(void) const
{
	static tNetMsg::tMapItem map[] =
	{
		MAKE_NMMI(tNetMsg_kTractorChangeMessage, fID),

		{	0, 0	}	// Terminator
	};
	return(map);
}

// FUNCTION: STARFLEETOP 0x004139ca
const tNetMsg::tMapItem *tNetMsg_kDeepScan::mGetMap(void) const
{
	static tNetMsg::tMapItem map[] =
	{
		MAKE_NMMI(tNetMsg_kDeepScan, fID),

		{	0, 0	}	// Terminator
	};
	return(map);
}

// HYPOTHESIS: the compiler generates the scalar deleting destructor from this virtual
// destructor; the original's body (called from 0x004139d0) is not recovered yet, so this one
// is empty.
// FUNCTION: STARFLEETOP 0x004139d0
tPhysicalEntity::tTractorItem::~tTractorItem()
{
}

// FUNCTION: STARFLEETOP 0x004139ec
// HYPOTHESIS: sends kKillEntity for this entity once, the first time it runs with the flag
// at 0x24 set.
void tPhysicalEntity::vfunction5(void)
{
	if (fIsAlive)
	{
		fIsAlive = 0;
		if (gObject_008e1060.fn_0040fc04(fID))
		{
			tNetMsg_kKillEntity msg;
			msg.field_0x4 = true;
			gNetControl.mQueueMsg(&msg, tNetMsg::kKillEntity, fID, 0.0);
		}
	}
}

// FUNCTION: STARFLEETOP 0x00413a37
// HYPOTHESIS: stores the ID at 0x18c that vfunction59 returns, then, when this entity is
// still known by its ID, tells the object reached through the one at 0xad4 of the
// entity-manager result for this entity.
void tPhysicalEntity::vfunction60(int aID)
{
	field_0x18c = aID;
	tObject_00410fd4 *owner = gObject_008e1060.fn_00410fd4(this);
	if (gObject_008e1060.fn_0040fc04(fID) && owner && owner->field_0xad4)
	{
		tObject_00410fd4_slot *slot = owner->field_0xad4->vfunction67();
		if (slot)
			slot->vfunction80();
	}
}

// FUNCTION: STARFLEETOP 0x00413a8d
// HYPOTHESIS: stores the object at 0x88 and sets the float at 0x2c from its slot 9, raised
// to at least 3, or to 0 when there is no object.
void tPhysicalEntity::fn_00413a8d(tObject_00413a8d_arg *aObject)
{
	field_0x88 = aObject;
	if (aObject)
	{
		fRadius = aObject->vfunction9();
		if (fRadius < 3.0f)
			fRadius = 3.0f;
	}
	else
		fRadius = 0.0f;
}

// FUNCTION: STARFLEETOP 0x00413ac3
// HYPOTHESIS: hands the object the result of 0x00413b34, then has the model at 0x88 draw
// through it, while bit 0 of the flags at 0x20 is set.
void tPhysicalEntity::vfunction4(tObject_00413ac3_arg *aObject)
{
	static tObject_0040ce21 report("Humm is grey all we can do?");

	if (field_0x88 && (fProperties & 1))
	{
		aObject->vfunction57(fn_00413b34(0));
		field_0x88->vfunction6(aObject, 0.0f, 0);
	}
}

// HYPOTHESIS: a counter the matrix cache at 0xdc is checked against; its meaning is not known.
extern int gValue_00c7c140;

// FUNCTION: STARFLEETOP 0x00413b34
// HYPOTHESIS: rebuilds the matrix at 0x98 from the position at 0x34 and the three angles,
// unless it was already built for the current value of the global at 0x00c7c140 or the
// flag is set, and returns it.
tMatrix *tPhysicalEntity::fn_00413b34(bool aValue)
{
	if (gValue_00c7c140 != field_0xdc && !aValue)
	{
		field_0xdc = gValue_00c7c140;
		field_0x98.mIdentity();
		field_0x98.mForceTranslate(fCurrPos.x, fCurrPos.y, fCurrPos.z);
		field_0x98.mRotX(0x800);
		field_0x98.mRotY(field_0x90);
		field_0x98.mRotZ(field_0x94);
		field_0x98.mRotX(field_0x92);
	}
	return &field_0x98;
}

// FUNCTION: STARFLEETOP 0x00413bc1
// HYPOTHESIS: the first column of the cached matrix, without rebuilding it.
tPoint3 tPhysicalEntity::fn_00413bc1()
{
	tMatrix *matrix = fn_00413b34(true);
	return tPoint3(matrix->mGetItem(0, 0), matrix->mGetItem(1, 0), matrix->mGetItem(2, 0));
}

// FUNCTION: STARFLEETOP 0x00413bdf
tPoint3 tPhysicalEntity::vfunction29(const tPoint3 &aPoint, bool aValue)
{
	tPoint3 point = aPoint;
	vfunction28(aValue)->mTransform(point);
	return point;
}

// HYPOTHESIS: the callee (0x0069c97c) takes the point alone and sits just after mTransform
// (0x0069c970), so it is read as the rotation-only mTransform3x3; not yet confirmed.
// FUNCTION: STARFLEETOP 0x00413c24
tPoint3 tPhysicalEntity::vfunction30(const tPoint3 &aPoint)
{
	tPoint3 point = aPoint;
	fn_00413b34(false)->mTransform3x3(point);
	return point;
}

// FUNCTION: STARFLEETOP 0x00413c68
int tPhysicalEntity::vfunction34()
{
	return field_0x10c;
}

// FUNCTION: STARFLEETOP 0x00413c6f
bool tPhysicalEntity::vfunction35(int aID)
{
	for (int i = field_0x10c - 1; i >= 0; i--)
	{
		if (field_0x100[i] == aID)
		{
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00413c9a
// HYPOTHESIS: adds the ID to the field_0x10c IDs at field_0x100 unless it is already there,
// allocating room for 8 the first time and growing the room (field_0x114, archived as
// "fMaxNumberWeAreTractoring") by 8 whenever it is full.
void tPhysicalEntity::vfunction36(int aID)
{
	for (int i = field_0x10c - 1; i >= 0; i--)
	{
		if (field_0x100[i] == aID)
		{
			return;
		}
	}

	if (field_0x114 == 0)
	{
		field_0x100 = (int *)tMemoryPart::Search_tMemoryPart()->mAllocate(8 * 4, 710,
			"F:\\Projects\\Taldren\\Projects\\SFC2_5\\Source\\entity\\entity_physical.cpp");
		field_0x114 = 8;
	}
	else if (field_0x10c == field_0x114)
	{
		field_0x114 += 8;
		field_0x100 = (int *)tMemoryPart::Search_tMemoryPart()->vfunction8(field_0x100,
			field_0x114 * 4);
	}
	field_0x100[field_0x10c] = aID;
	field_0x10c++;
}

// FUNCTION: STARFLEETOP 0x00413d45
// HYPOTHESIS: removes the ID from the field_0x10c IDs at field_0x100, closing the gap; the
// counterpart of 0x00413c9a.
void tPhysicalEntity::vfunction37(int aID)
{
	for (int i = field_0x10c - 1; i >= 0; i--)
	{
		if (field_0x100[i] == aID)
		{
			field_0x10c--;
			for (int j = i; j < field_0x10c; j++)
			{
				field_0x100[j] = field_0x100[j + 1];
			}
			return;
		}
	}
}

// FUNCTION: STARFLEETOP 0x00413d98
// HYPOTHESIS: marks the entity as being tractored by aID (0x110 is archived as
// "fWeAreBeingTractored", 0x104 as "fEntityWeAreBeingTractoredByID"); the meaning of the
// flag stored at 0x108 is not known.
void tPhysicalEntity::vfunction38(int aID, bool aValue)
{
	field_0x110 = true;
	field_0x104 = aID;
	field_0x108 = aValue;
}

// FUNCTION: STARFLEETOP 0x00413db7
// HYPOTHESIS: clears the tractored-by state that vfunction38 sets, when aID is the entity
// tractoring this one.
void tPhysicalEntity::vfunction39(int aID)
{
	if (field_0x104 == aID)
	{
		field_0x104 = -1;
		field_0x110 = false;
	}
}

// HYPOTHESIS: only the name is known here; the class derives from tPhysicalEntity (its
// vftable at 0x00797430 shares tPhysicalEntity's slots), which is not declared as a base yet.
class tEntity_Weapon
{
public:
	~tEntity_Weapon();
};

// FUNCTION: STARFLEETOP 0x00413dd6
// HYPOTHESIS: not a function of its own: the original is `mov eax, <ehhandler>; call
// __EH_prolog`, the /O1 frame setup of the destructor whose body the analysis records
// separately at 0x00413de0 (tPhysicalEntity::~tPhysicalEntity); its unwind funclets
// 0x0075ba20 and 0x0075ba28 name this address as their parent. This stub only holds the
// address, as 0x00413614 does.
tEntity_Weapon::~tEntity_Weapon()
{
}

// HYPOTHESIS: the body after the frame setup at 0x00413dd6: disposes of the objects at 0x88
// and 0x8c, then frees the threat IDs (0x180, archived as "fNumberOfThreats" and
// "fMaxNumberOfThreats" at 0x184 and 0x188) and the IDs tractored (0x100, 0x10c, 0x114).
// The compiler emits the frame setup and this body as one function, so the rebuilt code
// is the original's 10 bytes at 0x00413dd6 followed by its 200 bytes here; scored against
// 0x00413de0 alone, everything after the frame setup is offset by those 10 bytes.
// FUNCTION: STARFLEETOP 0x00413de0
tPhysicalEntity::~tPhysicalEntity()
{
	if (field_0x88)
	{
		field_0x88 = field_0x88->vfunction1();
	}
	if (field_0x8c)
	{
		field_0x8c = field_0x8c->vfunction1();
	}
	if (field_0x180)
	{
		tMemoryPart::Search_tMemoryPart()->vfunction10(field_0x180);
		field_0x180 = 0;
		field_0x184 = 0;
		field_0x188 = 0;
	}
	if (field_0x100)
	{
		tMemoryPart::Search_tMemoryPart()->vfunction10(field_0x100);
		field_0x100 = 0;
		field_0x10c = 0;
		field_0x114 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x00413ea8
// HYPOTHESIS: whether aEntity is hostile to this one: this entity must have bit 1 of its
// flags at 0x18 set and still be known by its ID; an entity with bit 2 is judged by the team
// of the entity its vfunction12 names (relations 1 to 3), one with bit 14 always is, and one
// with bit 1 is when its owner's team differs and the teams' relation is 6.
bool tPhysicalEntity::vfunction49(tPhysicalEntity *aEntity)
{
	if (aEntity && (fClassification & 2) && gObject_008e1060.fn_004235f0(fID))
	{
		if (aEntity->fClassification & 4)
		{
			if (aEntity->vfunction23() <= 0)
				return false;
			tPhysicalEntity *entity = gObject_008e1060.fn_004249d3(aEntity->vfunction12());
			if (!entity)
				return false;
			int relation = gObject_0090be60->fn_00427581(fTeam, entity->fTeam);
			if (relation == 2 || relation == 1 || relation == 3)
				return true;
		}
		if (aEntity->fClassification & 0x4000)
			return true;
		if (aEntity->fClassification & 2)
		{
			tPhysicalEntity *owner = gObject_008e1060.fn_004249c0(aEntity);
			if (owner->fTeam != fTeam)
			{
				// MATCH-ONLY: the copy of the relations pointer puts its load ahead of the
				// arguments, as the original does; the register for this team still differs.
				tObject_0090be60 *relations = gObject_0090be60;
				bool war = relations->fn_00427581(fTeam, aEntity->fTeam) == 6;
				return war;
			}
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x00413f59
// HYPOTHESIS: rebuilds the threat list: empties it ("fNumberOfThreats" at 0x184), then passes
// every entity in the list the entity manager returns to vfunction46 with its ID.
void tPhysicalEntity::vfunction50()
{
	field_0x184 = 0;
	for (tPhysicalEntity *entity = gObject_008e1060.fn_0055f269(); entity;
		entity = (tPhysicalEntity *)entity->fPrev)
	{
		vfunction46(entity, entity->fID);
	}
}

// FUNCTION: STARFLEETOP 0x00413f8a
// HYPOTHESIS: announces this entity to every entity in the entity manager's list that has
// bit 0 of its flags at 0x18 set, through their vfunction46 with this entity's ID.
void tPhysicalEntity::vfunction51()
{
	for (tPhysicalEntity *entity = gObject_008e1060.fn_0055f269(); entity;
		entity = (tPhysicalEntity *)entity->fPrev)
	{
		if (entity->fClassification & 1)
		{
			entity->vfunction46(this, fID);
		}
	}
}

// FUNCTION: STARFLEETOP 0x00413fba
// HYPOTHESIS: withdraws this entity from every entity in the entity manager's list that has
// bit 0 of its flags at 0x18 set, through their vfunction47 with this entity's ID.
void tPhysicalEntity::vfunction52()
{
	for (tPhysicalEntity *entity = gObject_008e1060.fn_0055f269(); entity;
		entity = (tPhysicalEntity *)entity->fPrev)
	{
		if (entity->fClassification & 1)
		{
			entity->vfunction47(fID);
		}
	}
}

// FUNCTION: STARFLEETOP 0x00413fe9
// HYPOTHESIS: adds the entity's ID to the threat list ("fThreatListIDs" at 0x180, with
// "fNumberOfThreats" at 0x184) when it is not there yet and vfunction49 judges the entity
// hostile, allocating room for 32 the first time and growing the room ("fMaxNumberOfThreats"
// at 0x188) by 32 whenever it is full.
void tPhysicalEntity::vfunction46(tPhysicalEntity *aEntity, int aID)
{
	for (int i = field_0x184 - 1; i >= 0; i--)
	{
		if (field_0x180[i] == aID)
		{
			return;
		}
	}

	if (vfunction49(aEntity) == true)
	{
		if (field_0x188 == 0)
		{
			field_0x180 = (int *)tMemoryPart::Search_tMemoryPart()->mAllocate(32 * 4, 951,
				"F:\\Projects\\Taldren\\Projects\\SFC2_5\\Source\\entity\\entity_physical.cpp");
			field_0x188 = 32;
		}
		else if (field_0x184 == field_0x188)
		{
			field_0x188 += 32;
			field_0x180 = (int *)tMemoryPart::Search_tMemoryPart()->vfunction8(field_0x180,
				field_0x188 * 4);
		}
		field_0x180[field_0x184] = aID;
		field_0x184++;
	}
}
