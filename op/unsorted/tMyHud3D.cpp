// Unit:      unsorted/tMyHud3D.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <math.h>

#include "Source/entity/entity_physical.h"

// HYPOTHESIS: the entity tMyHud3D draws around; a class derived from tPhysicalEntity, since
// 0x004b0a39 calls 0x00413b34 on it and slots past tPhysicalEntity's last (vfunction64).
// vfunctionN is slot N-1 of its vftable, as for tPhysicalEntity.
class tObject_004b0a39_entity: public tPhysicalEntity
{
public:
	virtual void vfunction65();
	virtual void vfunction66();
	virtual void vfunction67();
	// HYPOTHESIS: returns a truth value; with the global at 0x00918888 in state 5 it selects
	// the position from 0x0041613e.
	virtual bool vfunction68();
	// HYPOTHESIS: returns an int; 0x004b1ba9 compares it with 0x00525081 of the entity table.
	virtual int vfunction69();
	// HYPOTHESIS: returns an int; 0x004b0a39 draws only while it is below 13.
	virtual int vfunction70();
	// HYPOTHESIS: returns a one-byte value; 0x004b1ba9 compares it with 2.
	virtual char vfunction71();
	// HYPOTHESIS: returns a truth value; 0x004b1ba9 picks the second texture row when it is true.
	virtual bool vfunction72();

	// HYPOTHESIS: returns a point by value that 0x004b0a39 adds to fCurrPos.
	tPoint3 fn_004130d0();
	// HYPOTHESIS: takes an index from 0 to 5 and returns a level from which 0x004b0a39 picks
	// an entry of the table at 0x00852448 (0 to 9) and a kind (1 to 3).
	int fn_00415672(int aIndex);
	// HYPOTHESIS: returns a 2-byte value that 0x004b14db compares with field_0x90.
	short fn_0041556f();
	// HYPOTHESIS: takes a point by value; tMyHud3D::vfunction8 calls it.
	void fn_004b26ca(tPoint3 aPoint);
};

// HYPOTHESIS: the object at 0x50 of the one the entity's vfunction8 returns; only 0x00423402
// is called on it, returning a 4-byte value.
class tObject_004b14db_slot8_50
{
public:
	int fn_00423402();
};









// HYPOTHESIS: the object the entity's vfunction8 returns; 0x004b14db passes the result of
// 0x00423025 (given the result of its member's 0x00423402) on to 0x004b1692.
class tObject_004b14db_slot8
{
public:
	int fn_00423025(int aValue);

	char field_0x0[0x50];
	tObject_004b14db_slot8_50 field_0x50;
};

// HYPOTHESIS: a global matrix at 0x009199f0 that 0x004b14db hands to the view before drawing.
extern tMatrix gObject_009199f0;

// HYPOTHESIS: the object at 0x00918888; only slot 8 (offset 0x20) is called, returning an int.
class tObject_00918888
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual int vfunction8();
};

extern tObject_00918888 *gObject_00918888;

// HYPOTHESIS: an 8-byte entry of the table of ten at 0x00852448; it is copied as two words.
// 0x004b0e1b uses fField0 as a row (0 to 3) of its texture table and fField4 as a level,
// drawing nothing when it is 0.
struct tObject_00852448
{
	int fField0;
	float fField4;
};

// HYPOTHESIS: returns the unit direction (sine, cosine) of one of six items (index 0 to 5),
// a sixth of a turn apart.
tPoint2 fn_004b14aa(int aIndex);

// HYPOTHESIS: takes a texel position (0 to 256) and returns a texture coordinate.
float fn_005050e2(float aValue);

// HYPOTHESIS: a 4-byte global that 0x004b0e1b passes to the view's vfunction77.
extern int gObject_0092f600;

extern tObject_00852448 gObject_00852448[10];

// HYPOTHESIS: the type of the local static at 0x009199b8: a point, passed where a tPoint3 is
// expected, whose destructor does nothing but is registered with atexit (0x004b14da).
struct tObject_009199b8: public tPoint3
{
	inline tObject_009199b8(const float aX, const float aY, const float aZ)
	{
		x = aX; y = aY; z = aZ;
	}
	inline ~tObject_009199b8() {}
};

// HYPOTHESIS: returns by value a point derived from the view and the point given.
tPoint3 fn_0041613e(tObject_00413ac3_arg *aView, const tPoint3 &aPoint);

// HYPOTHESIS: draws one of six items through the view; (view, index, table entry, kind,
// scale, flag). The original passes the flag as one 4-byte argument.
void fn_004b0e1b(tObject_00413ac3_arg *aView, int aIndex, tObject_00852448 *aEntry, int aKind,
	float aScale, bool aFlag);

// HYPOTHESIS: the second object tMyHud3D::vfunction5 (0x004b1ed5) is given; vfunctionN is slot
// N of its vftable. Only slots 12, 63, 105 and 154 are called; the others are placeholders that
// keep the vftable order.
class tObject_004b1ed5_arg
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	// HYPOTHESIS: returns a kind; 0x004b1ed5 draws only when it is 0x15.
	virtual int vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	// HYPOTHESIS: returns a truth value (a byte tested for zero).
	virtual bool vfunction63();
	virtual void vfunction64();
	virtual void vfunction65();
	virtual void vfunction66();
	virtual void vfunction67();
	virtual void vfunction68();
	virtual void vfunction69();
	virtual void vfunction70();
	virtual void vfunction71();
	virtual void vfunction72();
	virtual void vfunction73();
	virtual void vfunction74();
	virtual void vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	virtual void vfunction80();
	virtual void vfunction81();
	virtual void vfunction82();
	virtual void vfunction83();
	virtual void vfunction84();
	virtual void vfunction85();
	virtual void vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	virtual void vfunction89();
	virtual void vfunction90();
	virtual void vfunction91();
	virtual void vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual void vfunction95();
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual void vfunction99();
	virtual void vfunction100();
	virtual void vfunction101();
	virtual void vfunction102();
	virtual void vfunction103();
	virtual void vfunction104();
	// HYPOTHESIS: returns a signed one-byte value; 0x004b1ed5 requires it to be above 0.
	virtual char vfunction105();
	virtual void vfunction106();
	virtual void vfunction107();
	virtual void vfunction108();
	virtual void vfunction109();
	virtual void vfunction110();
	virtual void vfunction111();
	virtual void vfunction112();
	virtual void vfunction113();
	virtual void vfunction114();
	virtual void vfunction115();
	virtual void vfunction116();
	virtual void vfunction117();
	virtual void vfunction118();
	virtual void vfunction119();
	virtual void vfunction120();
	virtual void vfunction121();
	virtual void vfunction122();
	virtual void vfunction123();
	virtual void vfunction124();
	virtual void vfunction125();
	virtual void vfunction126();
	virtual void vfunction127();
	virtual void vfunction128();
	virtual void vfunction129();
	virtual void vfunction130();
	virtual void vfunction131();
	virtual void vfunction132();
	virtual void vfunction133();
	virtual void vfunction134();
	virtual void vfunction135();
	virtual void vfunction136();
	virtual void vfunction137();
	virtual void vfunction138();
	virtual void vfunction139();
	virtual void vfunction140();
	virtual void vfunction141();
	virtual void vfunction142();
	virtual void vfunction143();
	virtual void vfunction144();
	virtual void vfunction145();
	virtual void vfunction146();
	virtual void vfunction147();
	virtual void vfunction148();
	virtual void vfunction149();
	virtual void vfunction150();
	virtual void vfunction151();
	virtual void vfunction152();
	virtual void vfunction153();
	// HYPOTHESIS: returns a float; 0x004b1ed5 truncates it to the radius of the rings it draws.
	virtual float vfunction154();
};

// tHud3D: an 11-slot vftable (0x007a15c0), 4 bytes. HYPOTHESIS: the slots are placeholders.
class tHud3D
{
public:
	virtual ~tHud3D();
};

// tMyHud3D: vftable 0x007a1590, 28 bytes.
class tMyHud3D: public tHud3D
{
public:
	virtual ~tMyHud3D();
	virtual void vfunction2(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView);
	virtual void vfunction3(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView);
	virtual void vfunction4(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView);
	virtual void vfunction5(tObject_004b0a39_entity *aEntity, tObject_004b1ed5_arg *aObject,
		tObject_00413ac3_arg *aView);
	virtual void vfunction6(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView);
	// HYPOTHESIS: (flag, entity); it sets the entity's byte at 0x79 when given one.
	virtual void vfunction7(char aFlag, tObject_004b0a39_entity *aEntity);
	// HYPOTHESIS: (point, entity); it passes the point by value to 0x004b26ca on the entity.
	virtual void vfunction8(const tPoint3 &aPoint, tObject_004b0a39_entity *aEntity);
	// HYPOTHESIS: (ID, entity); it passes the point of the object with that ID to 0x004b26ca
	// on the entity.
	virtual void vfunction9(int aID, tObject_004b0a39_entity *aEntity);
	// HYPOTHESIS: sets field_0x4 (the mode vfunction2 checks) when given 0, 1 or 3.
	virtual void vfunction10(int aValue);
	// HYPOTHESIS: (two values, flag, value); it passes the three values on to an object chosen
	// by the flag and by the kind gObject_0092deb4 reports.
	virtual void vfunction11(int aA, int aB, char aFlag, int aD);

	// HYPOTHESIS: (view, two 2-byte values, size, value); 0x004b14db calls it directly.
	void fn_004b1692(tObject_00413ac3_arg *aView, short aA, short aB, float aSize, int aValue);
	// HYPOTHESIS: returns 1 or -1, the direction of the shorter turn between two angles.
	int fn_004b1654(tAngle aFrom, tAngle aTo);

	// HYPOTHESIS: an int; vfunction2 draws only while it is at least 1.
	int field_0x4;
	char field_0x8[0x8];
	// HYPOTHESIS: the point of the last entity vfunction6 drew a marker for (its fPoint7A).
	tPoint3 field_0x10;
};

// FUNCTION: STARFLEETOP 0x004b0a39
void tMyHud3D::vfunction2(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView)
{
	if (field_0x4 < 1)
		return;
	if (aEntity->vfunction70() >= 13)
		return;

	tMatrix *matrix = aEntity->fn_00413b34(false);
	tPoint3 origin;
	matrix->mTransform(origin, tPoint3(0.0f, 0.0f, 0.0f));
	tPoint3 forward;
	matrix->mTransform(forward, tPoint3(0.0f, 0.0f, 1.0f));
	forward -= origin;

	static tObject_009199b8 up(0.0f, 0.0f, 1.0f);
	tPoint3 side = forward % up;
	side.mNormalize();
	forward = up % side;

	tMatrix m;
	tMtx4x4 &mtx = m.mGetMtx4x4();
	mtx[0][0] = side.x;
	mtx[1][0] = side.y;
	mtx[2][0] = side.z;
	mtx[0][1] = forward.x;
	mtx[1][1] = forward.y;
	mtx[2][1] = forward.z;
	mtx[0][2] = up.x;
	mtx[1][2] = up.y;
	mtx[2][2] = up.z;

	tPoint3 position = aEntity->fCurrPos + aEntity->fn_004130d0();
	if (gObject_00918888->vfunction8() == 5 && aEntity->vfunction68())
		position = fn_0041613e(aView, position);

	mtx[0][3] = position.x;
	mtx[1][3] = position.y;
	mtx[2][3] = position.z;

	tPoint3 view(position);
	aView->vfunction55()->mTransform3x3(view);
	float distance = view * view;

	bool flag = true;
	float scale = aEntity->vfunction14() * 0.5f > 1.0f ? aEntity->vfunction14() * 0.5f : 1.0f;
	if (distance > scale * 18250.0)
		flag = false;

	// HYPOTHESIS: half of 60 degrees plus a quarter turn (120 degrees), built up in separate
	// statements; written as one expression the compiler folds it into a single constant.
	double angle = kPi / 3.0;
	angle *= 0.5;
	angle += kPi * 0.5;
	m.mRotZRadians(angle);
	aView->vfunction57(&m);

	tObject_00852448 entries[6];
	int kinds[6];
	for (int i = 0; i < 6; i++)
	{
		int level = aEntity->fn_00415672(i);
		if (level < 0)
			entries[i] = gObject_00852448[0];
		else if (level >= 10)
			entries[i] = gObject_00852448[9];
		else
			entries[i] = gObject_00852448[level];

		if (level > 6)
			kinds[i] = 3;
		else
			kinds[i] = (level > 3) + 1;
	}

	aView->vfunction77(0);
	float size = aEntity->vfunction14();
	if (aView->vfunction67(7))
	{
		fn_004b0e1b(aView, 0, &entries[0], kinds[0], size, flag);
		fn_004b0e1b(aView, 1, &entries[1], kinds[1], size, flag);
		fn_004b0e1b(aView, 2, &entries[2], kinds[2], size, flag);
		fn_004b0e1b(aView, 3, &entries[3], kinds[3], size, flag);
		fn_004b0e1b(aView, 4, &entries[4], kinds[4], size, flag);
		fn_004b0e1b(aView, 5, &entries[5], kinds[5], size, flag);
		aView->vfunction68();
	}
}

// FUNCTION: STARFLEETOP 0x004b0e1b
void fn_004b0e1b(tObject_00413ac3_arg *aView, int aIndex, tObject_00852448 *aEntry, int aKind,
	float aScale, bool aFlag)
{
	if (aEntry->fField4 == 0.0f)
		return;

	int next = aIndex + 1;
	if (next > 5)
		next = 0;

	float radius[6];
	radius[0] = aScale * 1.22;
	radius[1] = aScale * 1.135;
	radius[2] = aScale * 1.115;
	radius[3] = aScale * 1.06;
	radius[4] = aScale;
	radius[5] = aScale * 0.9;

	tPoint2 p0 = fn_004b14aa(aIndex);
	tPoint2 p1 = fn_004b14aa(next);

	static float uv[4][4] = {
		{ fn_005050e2(0.0f), fn_005050e2(256.0f), fn_005050e2(63.0f), fn_005050e2(192.0f) },
		{ fn_005050e2(64.0f), fn_005050e2(256.0f), fn_005050e2(127.0f), fn_005050e2(192.0f) },
		{ fn_005050e2(128.0f), fn_005050e2(256.0f), fn_005050e2(191.0f), fn_005050e2(192.0f) },
		{ fn_005050e2(192.0f), fn_005050e2(256.0f), fn_005050e2(255.0f), fn_005050e2(192.0f) },
	};

	aView->vfunction77(gObject_0092f600);
	aView->vfunction76(aEntry->fField4, aEntry->fField4, aEntry->fField4);

	if (aFlag)
	{
		if (aKind > 0)
		{
			aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][1]);
			aView->vfunction69(p0.x * radius[4], p0.y * radius[4], 0.0f);
			aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][3]);
			aView->vfunction69(p1.x * radius[4], p1.y * radius[4], 0.0f);
			aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][3]);
			aView->vfunction69(p1.x * radius[5], p1.y * radius[5], 0.0f);
			aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][1]);
			aView->vfunction69(p0.x * radius[5], p0.y * radius[5], 0.0f);

			if (aKind > 1)
			{
				aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][1]);
				aView->vfunction69(p0.x * radius[2], p0.y * radius[2], 0.0f);
				aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][3]);
				aView->vfunction69(p1.x * radius[2], p1.y * radius[2], 0.0f);
				aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][3]);
				aView->vfunction69(p1.x * radius[3], p1.y * radius[3], 0.0f);
				aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][1]);
				aView->vfunction69(p0.x * radius[3], p0.y * radius[3], 0.0f);

				if (aKind > 2)
				{
					aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][1]);
					aView->vfunction69(p0.x * radius[0], p0.y * radius[0], 0.0f);
					aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][3]);
					aView->vfunction69(p1.x * radius[0], p1.y * radius[0], 0.0f);
					aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][3]);
					aView->vfunction69(p1.x * radius[1], p1.y * radius[1], 0.0f);
					aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][1]);
					aView->vfunction69(p0.x * radius[1], p0.y * radius[1], 0.0f);
				}
			}
		}
	}
	else
	{
		aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][1]);
		aView->vfunction69(p0.x * radius[0], p0.y * radius[0], 0.0f);
		aView->vfunction73(uv[aEntry->fField0][0], uv[aEntry->fField0][3]);
		aView->vfunction69(p1.x * radius[0], p1.y * radius[0], 0.0f);
		aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][3]);
		aView->vfunction69(p1.x * radius[5], p1.y * radius[5], 0.0f);
		aView->vfunction73(uv[aEntry->fField0][2], uv[aEntry->fField0][1]);
		aView->vfunction69(p0.x * radius[5], p0.y * radius[5], 0.0f);
	}
}

// FUNCTION: STARFLEETOP 0x004b14aa
tPoint2 fn_004b14aa(int aIndex)
{
	// HYPOTHESIS: the original multiplies by 0x555, a sixth of kAngle360.
	tAngle angle = aIndex * (kAngle360 / 6);
	tPoint2 result;
	result.x = Sine(angle);
	result.y = Cosine(angle);
	return result;
}

// FUNCTION: STARFLEETOP 0x004b14db
void tMyHud3D::vfunction4(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView)
{
	if (field_0x4 < 1)
		return;

	tMatrix saved = *aView->vfunction55();
	aView->vfunction57(&gObject_009199f0);

	tPoint3 position = aEntity->fCurrPos;
	tPoint3 offset = aEntity->fn_004130d0();
	position += offset;
	if (gObject_00918888->vfunction8() == 5 && aEntity->vfunction68())
		position = fn_0041613e(aView, position);

	tMatrix m;
	m.mIdentity();
	m.mTranslate(position.x, position.y, position.z);
	aView->vfunction57(&m);

	short a = aEntity->field_0x90;
	short b = aEntity->fn_0041556f();
	aView->vfunction54(&saved);

	if (a != b && aView->vfunction67(7))
	{
		aView->vfunction77(0);
		tObject_004b14db_slot8 *object = aEntity->vfunction8();
		fn_004b1692(aView, a, b, aEntity->vfunction14(),
			object->fn_00423025(object->field_0x50.fn_00423402()));
	}
	aView->vfunction68();
}

// HYPOTHESIS: the direction (1 or -1) of the shorter turn from one angle to the other, a half
// turn being 0x1000. It ends with ret 8 and does not use ecx, so it is a member of tMyHud3D
// (fn_004b1692 being its likely caller) or a free __stdcall function.
// FUNCTION: STARFLEETOP 0x004b1654
int tMyHud3D::fn_004b1654(tAngle aFrom, tAngle aTo)
{
	if (aFrom < aTo)
		return aTo - aFrom < 0x1000 ? 1 : -1;
	return aFrom - aTo < 0x1000 ? -1 : 1;
}

// HYPOTHESIS: draws an arc from angle aA to aB, turning the shorter way, as pairs of vertices at
// radius aSize and twice aSize, one pair every 0x100 of angle; aValue (0 to 3) picks the colour.
// FUNCTION: STARFLEETOP 0x004b1692
void tMyHud3D::fn_004b1692(tObject_00413ac3_arg *aView, short aA, short aB, float aSize, int aValue)
{
	tPoint3 p;
	tPoint3 v;

	p.x = Cosine(aA);
	p.y = Sine(aA);
	p.z = 0.002f;

	int direction = fn_004b1654(aA, aB);

	switch (aValue)
	{
	case 0:
		aView->vfunction76(0.0f, 0.4f, 0.0f);
		break;
	case 1:
		aView->vfunction76(0.4f, 0.4f, 0.0f);
		break;
	case 2:
		aView->vfunction76(0.4f, 0.2f, 0.0f);
		break;
	case 3:
		aView->vfunction76(0.4f, 0.0f, 0.0f);
		break;
	// HYPOTHESIS: default repeats case 3 as its own body; folding them into one label
	// lays the switch out differently from the original.
	default:
		aView->vfunction76(0.4f, 0.0f, 0.0f);
		break;
	}

	v = p * aSize;
	aView->vfunction70(&v);
	float outer = aSize * 2.0f;
	v = p * outer;
	aView->vfunction70(&v);

	if (direction > 0)
	{
		if (aB < aA)
			aB += 0x2000;
		for (short a = (aA / 0x100 + 1) * 0x100; a < aB; a += 0x100)
		{
			p.x = Cosine(a);
			p.y = Sine(a);
			p.z = 0.002f;
			v = p * outer;
			aView->vfunction70(&v);
			v = p * aSize;
			aView->vfunction70(&v);
			v = p * aSize;
			aView->vfunction70(&v);
			v = p * outer;
			aView->vfunction70(&v);
		}
	}
	else
	{
		if (aB > aA)
			aB -= 0x2000;
		for (short a = (aA / 0x100) * 0x100; a > aB; a -= 0x100)
		{
			p.x = Cosine(a);
			p.y = Sine(a);
			p.z = 0.002f;
			v = p * outer;
			aView->vfunction70(&v);
			v = p * aSize;
			aView->vfunction70(&v);
			v = p * aSize;
			aView->vfunction70(&v);
			v = p * outer;
			aView->vfunction70(&v);
		}
	}

	p.x = Cosine(aB);
	p.y = Sine(aB);
	p.z = 0.001f;
	v = p * outer;
	aView->vfunction70(&v);
	v = p * aSize;
	aView->vfunction70(&v);
}

// HYPOTHESIS: the current time, a double (as in tCloakSystem and tNetControl).
extern double g_00c7c148;

// HYPOTHESIS: returns a float with no arguments (7 bytes: a load of a global and a return).
float fn_00412f21();

// HYPOTHESIS: three rows of texture coordinates (u0, v0, u1, v1), filled through fn_005050e2 by
// the initializer at 0x004b1ab1 (whose fragments are 0x004b1ac5 to 0x004b1b3d above).
extern float gObject_00919a78[3][4];

// HYPOTHESIS: draws three squares around the entity, shrinking with time, from the texture row
// vfunction72 and vfunction71 select.
// FUNCTION: STARFLEETOP 0x004b1ba9
void tMyHud3D::vfunction3(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView)
{
	if (field_0x4 < 1)
		return;

	tMatrix saved = *aView->vfunction55();
	aView->vfunction57(&gObject_009199f0);

	tPoint3 position = aEntity->fCurrPos;
	tMatrix m;
	m.mIdentity();
	m.mTranslate(position.x, position.y, position.z);
	aView->vfunction57(&m);

	float seconds = g_00c7c148 * 0.000001;
	float time = seconds * 0.4f;
	float size = fn_00412f21();

	aView->vfunction77(gObject_0092f600);
	aView->vfunction76(1.0f, 1.0f, 1.0f);
	aView->vfunction67(7);

	for (int i = 0; i < 3; i++)
	{
		float scale = (1.0f - i * (1.0f / 3.0f)) * (time - (int)time);
		float r = scale * size;

		int row = aEntity->vfunction72() != 0;
		if (aEntity->vfunction71() == 2 && row &&
			aEntity->vfunction69() == gObject_008e1060.fn_00525081())
			row++;

		aView->vfunction73(gObject_00919a78[row][0], gObject_00919a78[row][1]);
		float z = 1.0f - scale;
		aView->vfunction69(-r, -r, z);
		aView->vfunction73(gObject_00919a78[row][2], gObject_00919a78[row][1]);
		aView->vfunction69(r, -r, z);
		aView->vfunction73(gObject_00919a78[row][2], gObject_00919a78[row][3]);
		aView->vfunction69(r, r, z);
		aView->vfunction73(gObject_00919a78[row][0], gObject_00919a78[row][3]);
		aView->vfunction69(-r, r, z);
	}
	aView->vfunction68();
	aView->vfunction54(&saved);
}

// HYPOTHESIS: a time in seconds, a float.
extern float gObject_0092f61c;
// HYPOTHESIS: an angle step as a fraction of a turn, applied per segment.
extern float gObject_009199b0;
// HYPOTHESIS: two texture radii, filled by the initializers ending at 0x004b1eb1 and 0x004b1ecd.
extern float gObject_009199ac;
extern float gObject_009199a8;
// HYPOTHESIS: a row of texture coordinates (u0, v0, u1, v1) filled by the initializer whose
// fragments are 0x004b1e4d to 0x004b1e85; elements 2 and 3 are the texture centre.
extern float gObject_00919aa8[4];

// HYPOTHESIS: two constants of this unit, loaded from memory rather than folded, so declared as
// const variables: pi written to 11 decimals (0x007a1580) and a ring width (0x007a1588).
const double gObject_007a1580 = 3.14159265359;
const float gObject_007a1588 = 2.5f;

// HYPOTHESIS: draws a turning ring of eight segments, mirrored into four quadrants, around the
// first entity, with a radius from the object's slot 154.
// FUNCTION: STARFLEETOP 0x004b1ed5
void tMyHud3D::vfunction5(tObject_004b0a39_entity *aEntity, tObject_004b1ed5_arg *aObject,
	tObject_00413ac3_arg *aView)
{
	if (field_0x4 < 1)
		return;
	if (!aView || !aEntity || !aObject)
		return;
	if (aObject->vfunction12() != 0x15)
		return;

	int count;
	if (aObject->vfunction105() > 0 && aObject->vfunction63())
		count = (int)aObject->vfunction154();
	else
		count = 0;
	if (count <= 0)
		return;

	tMatrix saved = *aView->vfunction55();
	aView->vfunction57(&gObject_009199f0);

	float time = gObject_0092f61c * (1.0f / 48.0f);
	float angle = -((time - (int)time) * gObject_007a1580);
	tPoint3 position = aEntity->fCurrPos;
	tMatrix m;
	m.mIdentity();
	m.mTranslate(position.x, position.y, position.z);
	m.mRotZRadians(angle);
	aView->vfunction57(&m);

	aView->vfunction77(gObject_0092f600);
	aView->vfunction76(1.0f, 1.0f, 1.0f);
	aView->vfunction67(7);

	for (int i = 0; i < 8; i++)
	{
		float s0 = sin(i * gObject_009199b0 * gObject_007a1580 * 2.0);
		float c0 = cos(i * gObject_009199b0 * gObject_007a1580 * 2.0);
		float s1 = sin((i + 1) * gObject_009199b0 * gObject_007a1580 * 2.0);
		float c1 = cos((i + 1) * gObject_009199b0 * gObject_007a1580 * 2.0);

		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s0, gObject_009199ac * c0 + gObject_00919aa8[3]);
		aView->vfunction69(s0 * count, c0 * count, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s1, gObject_009199ac * c1 + gObject_00919aa8[3]);
		aView->vfunction69(s1 * count, c1 * count, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s1, gObject_009199a8 * c1 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * s1, (count - gObject_007a1588) * c1, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s0, gObject_009199a8 * c0 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * s0, (count - gObject_007a1588) * c0, 0.0f);

		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s0, gObject_009199ac * c0 + gObject_00919aa8[3]);
		aView->vfunction69(-(s0 * count), c0 * count, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s1, gObject_009199ac * c1 + gObject_00919aa8[3]);
		aView->vfunction69(-(s1 * count), c1 * count, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s1, gObject_009199a8 * c1 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * -s1, (count - gObject_007a1588) * c1, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s0, gObject_009199a8 * c0 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * -s0, (count - gObject_007a1588) * c0, 0.0f);

		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s0, gObject_009199ac * c0 + gObject_00919aa8[3]);
		aView->vfunction69(-(s0 * count), -(c0 * count), 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s1, gObject_009199ac * c1 + gObject_00919aa8[3]);
		aView->vfunction69(-(s1 * count), -(c1 * count), 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s1, gObject_009199a8 * c1 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * -s1, (count - gObject_007a1588) * -c1, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s0, gObject_009199a8 * c0 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * -s0, (count - gObject_007a1588) * -c0, 0.0f);

		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s0, gObject_009199ac * c0 + gObject_00919aa8[3]);
		aView->vfunction69(s0 * count, -(c0 * count), 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199ac * s1, gObject_009199ac * c1 + gObject_00919aa8[3]);
		aView->vfunction69(s1 * count, -(c1 * count), 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s1, gObject_009199a8 * c1 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * s1, (count - gObject_007a1588) * -c1, 0.0f);
		aView->vfunction73(gObject_00919aa8[2] - gObject_009199a8 * s0, gObject_009199a8 * c0 + gObject_00919aa8[3]);
		aView->vfunction69((count - gObject_007a1588) * s0, (count - gObject_007a1588) * -c0, 0.0f);
	}
	aView->vfunction68();
	aView->vfunction54(&saved);
}

// FUNCTION: STARFLEETOP 0x004b2688
void tMyHud3D::vfunction7(char aFlag, tObject_004b0a39_entity *aEntity)
{
	if (aEntity)
		aEntity->fField79 = aFlag != 0;
}

// FUNCTION: STARFLEETOP 0x004b269e
void tMyHud3D::vfunction8(const tPoint3 &aPoint, tObject_004b0a39_entity *aEntity)
{
	if (aEntity)
		aEntity->fn_004b26ca(aPoint);
}

// FUNCTION: STARFLEETOP 0x004b26ca
void tObject_004b0a39_entity::fn_004b26ca(tPoint3 aPoint)
{
	fPoint7A = aPoint;
}

// FUNCTION: STARFLEETOP 0x004b26e4
void tMyHud3D::vfunction9(int aID, tObject_004b0a39_entity *aEntity)
{
	if (aEntity)
	{
		tObject_0042496f *object = (tObject_0042496f *)gObject_008e1060.fn_0042496f(aID);
		if (object)
			aEntity->fn_004b26ca(object->fn_00422cba());
	}
}

// HYPOTHESIS: for each live entity of the table's list whose flag at 0x79 is set, draws an arrow
// from the given entity towards that entity's point at 0x7a (the one vfunction8 and vfunction9
// set), as two triangles shaded from the tip, then outlines them.
// FUNCTION: STARFLEETOP 0x004b2725
void tMyHud3D::vfunction6(tObject_004b0a39_entity *aEntity, tObject_00413ac3_arg *aView)
{
	for (tEntity *e = gObject_008e1060.fn_0055f269(); e; e = e->fPrev)
	{
		if (!(e->fClassification & 2) || !e->fIsAlive || !e->fField79)
			continue;
		if (field_0x4 < 1)
			return;
		if (!aView)
			return;

		bool saved = aView->vfunction46(false);
		field_0x10 = tPoint3(e->fPoint7A);

		tPoint3 position = aEntity->fCurrPos;
		tPoint3 direction = field_0x10 - position;
		direction.mNormalize();

		tMatrix m(direction.x, -direction.y, 0.0f, 0.0f,
			direction.y, direction.x, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		m.mPostTranslate(position.x, position.y, position.z);
		aView->vfunction57(&m);

		float half = aEntity->vfunction14() * 0.5f;
		float base = aEntity->vfunction14() * 0.15f + aEntity->vfunction14();

		aView->vfunction77(0);
		aView->vfunction67(4);
		aView->vfunction76(1.0f, 1.0f, 1.0f);
		aView->vfunction69(base + half, 0.0f, 0.0f);
		aView->vfunction76(0.25f, 0.25f, 0.25f);
		aView->vfunction69(base, half * -0.25f, 0.0f);
		aView->vfunction76(0.65f, 0.65f, 0.65f);
		aView->vfunction69(base, 0.0f, 0.0f);
		aView->vfunction76(1.0f, 1.0f, 1.0f);
		aView->vfunction69(base + half, 0.0f, 0.0f);
		aView->vfunction76(0.25f, 0.25f, 0.25f);
		aView->vfunction69(base, half * 0.25f, 0.0f);
		aView->vfunction76(0.65f, 0.65f, 0.65f);
		aView->vfunction69(base, 0.0f, 0.0f);
		aView->vfunction68();

		aView->vfunction67(3);
		aView->vfunction76(0.5f, 0.5f, 0.5f);
		aView->vfunction69(base + half, 0.0f, 0.0f);
		aView->vfunction69(base, half * -0.25f, 0.0f);
		aView->vfunction69(base, half * 0.25f, 0.0f);
		aView->vfunction68();

		aView->vfunction46(saved);
	}
}

// MATCH-ONLY: an inline of tEntity.h's tMatrix, defined here so the harness can claim the copy
// this unit emits (the class body says why).
// FUNCTION: STARFLEETOP 0x004b2ae6
inline tMatrix::tMatrix(
	const float xx, const float xy, const float xz, const float xw,
	const float yx, const float yy, const float yz, const float yw,
	const float zx, const float zy, const float zz, const float zw,
	const float wx, const float wy, const float wz, const float ww)
{
	fIdentity = f3x4 = kFalse;
	fM[0][0] = xx;	fM[0][1] = xy;	fM[0][2] = xz;	fM[0][3] = xw;
	fM[1][0] = yx;	fM[1][1] = yy;	fM[1][2] = yz;	fM[1][3] = yw;
	fM[2][0] = zx;	fM[2][1] = zy;	fM[2][2] = zz;	fM[2][3] = zw;
	fM[3][0] = wx;	fM[3][1] = wy;	fM[3][2] = wz;	fM[3][3] = ww;
}

// The object pointed to by 0x0092deb4; only slot 39 (offset 0x9c) is called here.
#include "unsorted/tObject_0092deb4.h"

// HYPOTHESIS: the objects tMyHud3D::vfunction11 picks from; only slot 0 is called, with three
// 4-byte arguments.
class tObject_004b2b56_target
{
public:
	virtual void vfunction0(int aA, int aB, int aC);
};

// HYPOTHESIS: two objects per kind, the first used when the flag is set.
extern tObject_004b2b56_target *gObject_00917148;
extern tObject_004b2b56_target *gObject_00917168;
extern tObject_004b2b56_target *gObject_009172e0;
extern tObject_004b2b56_target *gObject_00917300;
extern tObject_004b2b56_target *gObject_009174e0;
extern tObject_004b2b56_target *gObject_00917500;
extern tObject_004b2b56_target *gObject_009176e8;
extern tObject_004b2b56_target *gObject_00917708;
extern tObject_004b2b56_target *gObject_009177c0;
extern tObject_004b2b56_target *gObject_009177e0;
extern tObject_004b2b56_target *gObject_00917900;
extern tObject_004b2b56_target *gObject_00917920;
extern tObject_004b2b56_target *gObject_00917aa8;
extern tObject_004b2b56_target *gObject_00917ac8;
extern tObject_004b2b56_target *gObject_00917bc0;
extern tObject_004b2b56_target *gObject_00917be0;
extern tObject_004b2b56_target *gObject_00917ce0;
extern tObject_004b2b56_target *gObject_00917d00;

// HYPOTHESIS: passes (aA, aB, aD) on to one of eighteen objects, chosen by the kind
// gObject_0092deb4 reports and by aFlag.
// FUNCTION: STARFLEETOP 0x004b2b56
void tMyHud3D::vfunction11(int aA, int aB, char aFlag, int aD)
{
	tObject_004b2b56_target *target;
	switch (gObject_0092deb4->vfunction39())
	{
	case 0:
		if (aFlag)
			target = gObject_00917148;
		else
			target = gObject_00917168;
		break;
	case 1:
		if (aFlag)
			target = gObject_009177c0;
		else
			target = gObject_009177e0;
		break;
	case 2:
		if (aFlag)
			target = gObject_00917ce0;
		else
			target = gObject_00917d00;
		break;
	case 3:
		if (aFlag)
			target = gObject_00917900;
		else
			target = gObject_00917920;
		break;
	case 4:
		if (aFlag)
			target = gObject_009174e0;
		else
			target = gObject_00917500;
		break;
	case 5:
		if (aFlag)
			target = gObject_009172e0;
		else
			target = gObject_00917300;
		break;
	case 6:
		if (aFlag)
			target = gObject_009176e8;
		else
			target = gObject_00917708;
		break;
	case 7:
		if (aFlag)
			target = gObject_00917aa8;
		else
			target = gObject_00917ac8;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		if (aFlag)
			target = gObject_00917bc0;
		else
			target = gObject_00917be0;
		break;
	default:
		if (aFlag)
			target = gObject_00917148;
		else
			target = gObject_00917168;
		break;
	}
	target->vfunction0(aA, aB, aD);
}

// FUNCTION: STARFLEETOP 0x004b2c83
void tMyHud3D::vfunction10(int aValue)
{
	switch (aValue)
	{
	case 0:
	case 1:
	case 3:
		field_0x4 = aValue;
		break;
	}
}
