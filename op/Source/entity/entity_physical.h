//	entity_physical.h: tPhysicalEntity, the physical entity, and the objects its members point to.
// Evidence:  the original header of tPhysicalEntity is not known; it sits beside the unit that
//            defines most of its functions.

#ifndef entity_physical_h
#define entity_physical_h 1

#include "unsorted/tEntity.h"

// HYPOTHESIS: the object 0x00413ac3 is given; only slot 57 (offset 0xe4) is called, with
// the result of 0x00413b34.
class tObject_00413ac3_arg
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8();
	virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14();
	virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17();
	virtual int vfunction18(int aValue);
	virtual void vfunction19();
	virtual int vfunction20(int aValue);
	virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26();
	virtual void vfunction27(); virtual void vfunction28(); virtual void vfunction29();
	virtual void vfunction30(); virtual void vfunction31(); virtual void vfunction32();
	virtual void vfunction33(); virtual void vfunction34(); virtual void vfunction35();
	virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38();
	virtual void vfunction39(); virtual void vfunction40(); virtual void vfunction41();
	virtual void vfunction42(); virtual void vfunction43(); virtual void vfunction44();
	virtual void vfunction45();
	// HYPOTHESIS: sets a one-byte state and returns the previous one; tMyHud3D::vfunction6
	// (0x004b2725) passes 0 and restores the result afterwards.
	virtual bool vfunction46(bool aValue);
	virtual void vfunction47();
	virtual void vfunction48(); virtual void vfunction49(); virtual void vfunction50();
	virtual void vfunction51(); virtual void vfunction52(); virtual void vfunction53();
	// HYPOTHESIS: sets a matrix; tMyHud3D::vfunction4 (0x004b14db) passes back a copy of
	// the one vfunction55 returned.
	virtual void vfunction54(tMatrix *aMatrix);
	// HYPOTHESIS: returns a matrix; tMyHud3D::vfunction2 (0x004b0a39) transforms a point by it.
	virtual tMatrix *vfunction55();
	virtual void vfunction56();
	virtual void vfunction57(void *aValue);
	// HYPOTHESIS: slots 58 to 77 as tAICaptain's 0x005200c8 calls them to draw lines: 18 and 20
	// set a state and return the old one, 67 begins a set of lines and says whether to draw
	// them, 68 ends it, 69 adds a vertex, 76 sets a colour and 77 takes a value.
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
	virtual void vfunction64();
	virtual void vfunction65();
	virtual void vfunction66();
	// HYPOTHESIS: takes an int and returns a truth value; 0x004b0a39 draws only when
	// vfunction67(7) is true, and closes with vfunction68.
	virtual bool vfunction67(int aValue);
	virtual void vfunction68();
	// HYPOTHESIS: takes a vertex position (x, y, z); 0x004b0e1b calls it after vfunction73.
	virtual void vfunction69(float aX, float aY, float aZ);
	// HYPOTHESIS: takes a vertex position as a point; tMyHud3D::fn_004b1692 (0x004b1692)
	// passes one after each vfunction76 colour.
	virtual void vfunction70(tPoint3 *aPoint);
	virtual void vfunction71();
	virtual void vfunction72();
	// HYPOTHESIS: takes a texture coordinate (u, v) for the next vertex.
	virtual void vfunction73(float aU, float aV);
	virtual void vfunction74();
	virtual void vfunction75();
	// HYPOTHESIS: takes a colour (three floats); 0x004b0e1b passes one level three times.
	virtual void vfunction76(float aR, float aG, float aB);
	// HYPOTHESIS: takes one 4-byte argument; 0x004b0a39 passes 0.
	virtual void vfunction77(int aValue);
};

// HYPOTHESIS: the object that 0x00413a8d stores at 0x88 of the entity; slot 9 (offset 0x24)
// returns a float, and 0x00413ac3 calls slot 6 (offset 0x18).
class tObject_00413a8d_arg
{
public:
	virtual void vfunction0();
	// HYPOTHESIS: slot 1 (offset 4) disposes of the object and returns the pointer to store
	// in its place; the destructor 0x00413de0 stores the result back at 0x88.
	virtual tObject_00413a8d_arg *vfunction1();
	virtual void vfunction2();
	virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5();
	// HYPOTHESIS: (object, float, flag); 0x00413ac3 passes the object it was given, 0.0 and 0.
	virtual void vfunction6(tObject_00413ac3_arg *aObject, float aValue, int aFlag);
	virtual void vfunction7(); virtual void vfunction8();
	virtual float vfunction9();
};

// HYPOTHESIS: the object at 0x8c of the entity; the destructor 0x00413de0 disposes of it
// through slot 1 (offset 4) and stores the result back, as it does for the one at 0x88.
class tObject_00413de0_8c
{
public:
	virtual void vfunction0();
	virtual tObject_00413de0_8c *vfunction1();
};

// The class of the member at 0x1a4, from its vftable (0x007958b0) and the slot names the
// analysis gives it; only the slot order is known, not the parameter types.
class tDynamicMemoryFreeList
{
public:
	// HYPOTHESIS: an inline destructor that empties the list; the destructor 0x00413de0
	// stores this vftable and calls mClearList (slot 18) directly for the member at 0x1a4.
	virtual ~tDynamicMemoryFreeList() { mClearList(); }
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void mGetPreviousListMember();
	virtual void mGetNextListMember();
	virtual void mGetPreviousCircularListMember();
	virtual void mGetNextCircularListMember();
	virtual void mGetListMemberAtThisIndex();
	virtual void mAddToFrontOfList();
	virtual void mAddToBackOfList();
	virtual void mInsertAfterListMember();
	virtual void mInsertBeforeListMember();
	virtual void mRemoveFromList();
	virtual void mIsListMember();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void mGetZeroIndexedListMemberPosition();
	virtual void mClearList();
	virtual void mDeleteListMembers();
	virtual void vfunction21();
	virtual void mDumpList();

	// HYPOTHESIS: the 12 bytes after the vftable pointer; their layout is not recovered.
	char field_0x4[0xc];
};

class tPhysicalEntity: public tEntity
{
public:
public:
	// HYPOTHESIS: the slots not yet recovered are placeholders that keep the vftable order;
	// vfunctionN is slot N-1 of the vftable; slot 0 is the scalar deleting destructor. Slots 1
	// to 21 are tEntity's; the ones this class overrides are declared here.
	virtual ~tPhysicalEntity();
	virtual void vfunction3();
	// HYPOTHESIS: draws the model through the object passed in; its type is not known.
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	virtual void vfunction5();
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	virtual void vfunction11(tSFCStream &s, int32u mode);
	// HYPOTHESIS: returns an integer; 0x0041351b subtracts from its result in eax.
	virtual int vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	// HYPOTHESIS: five 4-byte arguments.
	virtual void vfunction26(int a1, int a2, int a3, int a4, int a5);
	// HYPOTHESIS: (source ID, direction, amount, seed, flags), from the tNetMsg_kDamage it
	// fills; 0x00413448 passes (-1, 0, 10001, 0, 12).
	virtual void vfunction27(int aID, short aDirection, unsigned aAmount, int aSeed, char aFlags);
	// HYPOTHESIS: the flag 0x00413b34 takes, passed on unchanged, with its matrix returned;
	// vfunction29 transforms a point by the result.
	virtual tMatrix *vfunction28(bool aValue);
	// HYPOTHESIS: returns the point transformed by the matrix from vfunction28(aValue).
	virtual tPoint3 vfunction29(const tPoint3 &aPoint, bool aValue);
	// HYPOTHESIS: returns the point rotated by the matrix 0x00413b34 builds.
	virtual tPoint3 vfunction30(const tPoint3 &aPoint);
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	// HYPOTHESIS: returns the member at 0x10c, archived as "fNumberWeAreTractoring".
	virtual int vfunction34();
	// HYPOTHESIS: whether the ID is among the field_0x10c IDs at field_0x100 (the IDs this
	// entity tractors, since 0x10c is archived as "fNumberWeAreTractoring").
	virtual bool vfunction35(int aID);
	virtual void vfunction36(int aID);
	virtual void vfunction37(int aID);
	virtual void vfunction38(int aID, bool aValue);
	virtual void vfunction39(int aID);
	// HYPOTHESIS: returns a pointer to the member at 0x11c; its type is not known.
	virtual int *vfunction40();
	virtual void vfunction41();
	// HYPOTHESIS: returns the name buffer at 0x13c that vfunction43 sets.
	virtual char *vfunction42();
	virtual void vfunction43(char *aName);
	// HYPOTHESIS: returns the name buffer at 0x15c that vfunction45 sets.
	virtual char *vfunction44();
	virtual void vfunction45(char *aName);
	// HYPOTHESIS: takes an entity and its ID; vfunction50 calls it for each entity in a list.
	virtual void vfunction46(tPhysicalEntity *aEntity, int aID);
	// HYPOTHESIS: takes an entity ID; vfunction52 calls it on each entity in a list.
	virtual void vfunction47(int aID);
	virtual void vfunction48();
	// HYPOTHESIS: the argument is another entity; 0x00413ea8 reads its flags at 0x18, its
	// team at 0x60 and calls its slots 22 and 11.
	virtual bool vfunction49(tPhysicalEntity *aEntity);
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	// HYPOTHESIS: stores its argument in the float member at 0x17c that vfunction55 returns.
	virtual void vfunction54(float aValue);
	// The original loads the member at 0x17c with fld, so the member is a float.
	virtual float vfunction55();
	// HYPOTHESIS: sets the tPoint3 member at 0x198 from three coordinates; the original
	// takes the member's address first, as an inlined tPoint3::mSet does.
	virtual void vfunction56(float aX, float aY, float aZ);
	// HYPOTHESIS: returns by value the tPoint3 member at 0x198 that vfunction56 sets; the
	// original copies it through the hidden return pointer and returns that pointer.
	virtual tPoint3 vfunction57();
	virtual void vfunction58();
	// HYPOTHESIS: returns the 4-byte member at 0x18c, loaded with mov eax, so an integer or
	// pointer; its meaning is not known.
	virtual int vfunction59();
	// HYPOTHESIS: sets the ID at 0x18c that vfunction59 returns.
	virtual void vfunction60(int aID);
	// HYPOTHESIS: sets the one-byte flag at 0x190; its meaning is not known.
	virtual void vfunction61();
	// HYPOTHESIS: clears the one-byte flag at 0x190 that vfunction61 sets.
	virtual void vfunction62();
	virtual void vfunction63();
	// HYPOTHESIS: forwards to vfunction63 (pure in this class) through the vftable.
	virtual void vfunction64();

	// HYPOTHESIS: the parameter type and the return type; 0x004105dd pushes its one
	// argument unchanged, and 0x00413ac3 passes the result to its object's slot 57.
	tMatrix *fn_00413b34(bool aValue);

	// HYPOTHESIS: returns by value the first column of the matrix from fn_00413b34(true).
	tPoint3 fn_00413bc1();

	// HYPOTHESIS: a non-virtual member; its vftable slot, if it has one, is not recovered.
	void fn_00413a8d(tObject_00413a8d_arg *aObject);

	// HYPOTHESIS: the base entity's override of the same slot (tEntity::vfunction3), called
	// directly.
	void fn_004101d3();

	// HYPOTHESIS: the nested tractor item; only its virtual destructor is known so far.
	class tTractorItem
	{
	public:
		virtual ~tTractorItem();
	};

	// HYPOTHESIS: the object set by 0x00413a8d; its type is not known.
	tObject_00413a8d_arg *field_0x88;
	tObject_00413de0_8c *field_0x8c;
	// HYPOTHESIS: three 2-byte members, archived with a size of 2.
	short field_0x90;
	short field_0x92;
	short field_0x94;
	char field_0x96[0x2];
	// HYPOTHESIS: the entity's matrix, rebuilt by 0x00413b34 at most once per value of the
	// global at 0x00c7c140, which it stores at 0xdc.
	tMatrix field_0x98;
	int field_0xdc;
	char field_0xe0[0x20];
	// HYPOTHESIS: an array of field_0x10c ints, allocated when reading.
	int *field_0x100;
	int field_0x104;
	// HYPOTHESIS: an int set from vfunction38's flag argument, widened with movzx.
	int field_0x108;
	int field_0x10c;
	bool field_0x110;
	char field_0x111[0x3];
	int field_0x114;
	int field_0x118;
	int field_0x11c;
	char field_0x120[0x1c];
	// HYPOTHESIS: a 32-character name buffer; vfunction43 copies at most 31 characters into it.
	char field_0x13c[0x20];
	// HYPOTHESIS: a second 32-character name buffer; vfunction45 copies at most 31
	// characters into it.
	char field_0x15c[0x20];
	float field_0x17c;
	// HYPOTHESIS: an array of field_0x10c ints, allocated when reading.
	int *field_0x180;
	int field_0x184;
	int field_0x188;
	int field_0x18c;
	// HYPOTHESIS: a one-byte flag, since vfunction61 stores 1 into it with a byte move.
	bool field_0x190;
	char field_0x191[0x3];
	int field_0x194;
	tPoint3 field_0x198;
	tDynamicMemoryFreeList field_0x1a4;
	int field_0x1b4;
	int field_0x1b8;
};

#endif	// entity_physical_h
