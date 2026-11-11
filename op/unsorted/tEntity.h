//	tEntity.h: tEntity, the base entity, and the entity table at 0x008e1060.
// Evidence:  class (original file unknown)

#ifndef tEntity_h
#define tEntity_h 1

#include "Tiger/tbasic.h"

// The original includes tvectormath.h here, which has no unit either; the declarations the
// entity units need from it are kept here under its guard.
#ifndef	tvectormath_h
#define	tvectormath_h	1

// The SDK's tvectormath.h declares its point types inside pack(1), so a tPoint3 member has
// byte alignment; tEntity's point at +0x7a depends on it.
#pragma pack(push)
#pragma pack(1)

typedef short	tAngle;

struct tPoint3
{
public:
	float	x, y, z;

public:
	inline tPoint3(const float aX, const float aY, const float aZ)
	{
		x = aX; y = aY; z = aZ;
	}
	inline tPoint3(const tPoint3 &p)
	{
		x = p.x; y = p.y; z = p.z;
	}
	inline tPoint3()
	{
		// do nothing, leave uninitialized
	}

	inline float &operator[](const int i)              { return (&x)[i];	}
	inline void mSet(const float aX, const float aY, const float aZ)
	{
		x = aX; y = aY; z = aZ;
	}

	inline tPoint3 &operator=(const tPoint3 &a)
	{
		x = a.x; y = a.y; z = a.z;
		return(*this);
	}

	// HYPOTHESIS: the out-of-line operators at 0x0069b120, 0x0069b190 and 0x0069b270, and
	// the methods at 0x0069b390 and 0x0069b3d0, in the order the SDK declares them.
	tPoint3 operator+(const tPoint3 &a) const;
	// HYPOTHESIS: 0x0069b160 (+=) and 0x0069b250 (dot product), by the SDK's order and use.
	tPoint3 &operator+=(const tPoint3 &a);
	tPoint3 operator-(const tPoint3 &a) const;
	float operator*(const tPoint3 &a) const;
	tPoint3 operator*(const float d) const;
	float mLength(void) const;
	iTruth mNormalize(void);
};

// As the SDK declares it.
const double kPi = 3.14159265358979323846;

#pragma pack(pop)

// Only the members the entity units call are declared.
class tMatrix
{
protected:
	float fM[4][4];
	unsigned char fIdentity;
	unsigned char f3x4;

public:
	void mIdentity(void);
	inline float mGetItem(const unsigned kRow, const unsigned kCol) const	{	return(fM[kRow][kCol]);	}
	void mRotX(const tAngle angle);
	void mRotY(const tAngle angle);
	void mRotZ(const tAngle angle);
	void mForceTranslate(const float x, const float y, const float z);
	void mTransform(tPoint3 &p) const;
	void mTransform3x3(tPoint3 &p) const;
};

#endif	// end tvectormath_h block

// As the SDK declares it.
typedef int32 tID;

class tSFCStream;
class tPhysicalEntity;
class tObject_0041018e_arg;
class tObject_00413ac3_arg;
class tObject_008e1060_slot;
class fn_004e2b02_ship_type;
class tObject_00410fd4;

// vfunctionN is slot N-1 of the vftable (0x00795cb4, 22 slots); slot 0 is the scalar deleting
// destructor.
// HYPOTHESIS: the slots not yet recovered are placeholders that keep the vftable order.
class tEntity
{
public:
	tEntity();
	virtual ~tEntity();
	virtual int vfunction2();
	virtual void vfunction3();
	// HYPOTHESIS: draws the entity through the object passed in, as tPhysicalEntity's
	// override (0x00413ac3) does; the argument's type is not known.
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	virtual void vfunction8();
	virtual iTruth vfunction9();
	virtual void vfunction10(iTruth aValue);
	virtual void vfunction11(tSFCStream &s, int32u mode);
	// HYPOTHESIS: returns an entity ID; 0x00413ea8 looks it up through 0x004249d3.
	virtual int vfunction12();
	virtual void vfunction13(tID aID);
	virtual float vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();

	// Names of the archived fields come from the labels the archive function (0x0041004d)
	// writes. HYPOTHESIS: the other field types and meanings are read from the constructor's
	// stores only.
	tEntity *fNext;              // +0x04
	// HYPOTHESIS: tPhysicalEntity's vfunction50 to vfunction52 walk a list of entities
	// through this pointer, so it may be the next entity rather than the previous one.
	tEntity *fPrev;              // +0x08
	void *fField0C;              // +0x0c
	void *fField10;              // +0x10
	void *fField14;              // +0x14
	// HYPOTHESIS: flag bits as well; 0x00413ea8 tests bits 1, 2 and 14.
	int32 fClassification;       // +0x18
	tID fID;                     // +0x1c
	// HYPOTHESIS: flag bits; 0x00413ac3 draws only while bit 0 is set.
	int32u fProperties;          // +0x20
	// HYPOTHESIS: tPhysicalEntity's vfunction5 sends kKillEntity once while it is set.
	iTruth fIsAlive;             // +0x24
	tID fTargetID;               // +0x28
	// HYPOTHESIS: 0x00413a8d derives it from the object it stores at 0x88.
	float fRadius;               // +0x2c
	int32 fSelectStatus;         // +0x30
	// HYPOTHESIS: 0x00413b34 builds the entity's matrix at 0x98 from it.
	tPoint3 fCurrPos;            // +0x34
	tPoint3 fLastPos;            // +0x40
	tPoint3 fLastMapPos;         // +0x4c
	float fMapRadius;            // +0x58, not set by the constructor
	iTruth fField5C;             // +0x5c
	iTruth fBelongsToHumanFleet; // +0x5d
	tID fTeam;                   // +0x60
	int fField64;                // +0x64
	unsigned int fField68;       // +0x68
	int32 fScriptNotify;         // +0x6c
	int fField70;                // +0x70, not set by the constructor
	int fField74;                // +0x74, not set by the constructor
	iTruth fField78;             // +0x78
	iTruth fField79;             // +0x79
	tPoint3 fPoint7A;            // +0x7a
	iTruth fField86;             // +0x86

	// HYPOTHESIS: returns float; the original leaves the callee's result in place.
	float fn_00410168();
	// HYPOTHESIS: 0x00423b3b calls these on an entity with a point: 0x00413112 on the result
	// of 0x004249c0, 0x00417d3f on the colliding entity with its swept position.
	void fn_00413112(const tPoint3 &aPoint);
	void fn_00417d3f(const tPoint3 &aPoint);
};

// The object at 0x008e1060: a table of 0x4000 entity pointers indexed by ID at +0x30, then 16
// more objects at +0x10030.
// HYPOTHESIS: the class and its lookups are inferred from 0x00410168 and tPhysicalEntity's
// functions only, the layout after the entity table from 0x00413448 only.
class tObject_008e1060
{
public:
	tEntity *fn_lookup(tID aID)
	{
		if (aID >= 0 && aID < 0x4000)
			return fEntities[aID];
		return 0;
	}
	float fn_004246e4(tEntity *aFrom, tEntity *aTo);
	// HYPOTHESIS: a float between a weapon's ship and its target, as fn_004246e4 between two
	// entities; tPhaserXBank's 0x004e55af stores it and compares it with a limit.
	float fn_00424705(fn_004e2b02_ship_type *aShip, tEntity *aTarget);
	// HYPOTHESIS: the same float between two entities given by ID; tPhaserXBank's 0x004e5958
	// passes its ship's ID and the target's.
	float fn_004246ab(int aFromID, int aToID);
	// HYPOTHESIS: looks a ship up by ID; tPhaserXBank's 0x004e5958 calls its slot at 0x7c.
	fn_004e2b02_ship_type *fn_0042496f(int aID);
	tObject_008e1060_slot *fn_slot(int aIndex)
	{
		if (aIndex >= 0 && aIndex < 16)
			return fSlots[aIndex];
		return 0;
	}
	// HYPOTHESIS: looks an entity up by ID, as the inlined table lookup does.
	tPhysicalEntity *fn_00424a2f(int aID);
	// HYPOTHESIS: the inlined table lookup, as 0x00413541 does it.
	tPhysicalEntity *fn_entity(int aID)
	{
		return (aID >= 0 && aID < 0x4000) ? (tPhysicalEntity *)fEntities[aID] : 0;
	}
	// HYPOTHESIS: the argument and result types; 0x00413541 passes an int and stores the
	// result as the damage message's seed.
	int fn_004235ac(int aValue);
	// HYPOTHESIS: takes an entity ID and returns a truth value; 0x004139ec sends kKillEntity
	// only when it is true.
	bool fn_0040fc04(int aID);
	// HYPOTHESIS: takes an entity and returns an object related to it; 0x00413a37 calls it
	// with this.
	tObject_00410fd4 *fn_00410fd4(tPhysicalEntity *aEntity);
	// HYPOTHESIS: takes an entity ID and returns a truth value; 0x00413ea8 goes on only when
	// it is true for this entity's ID.
	bool fn_004235f0(int aID);
	// HYPOTHESIS: looks an entity up by an ID that 0x00413ea8 gets from vfunction12.
	tPhysicalEntity *fn_004249d3(int aID);
	// HYPOTHESIS: returns the entity that owns the one given; 0x00413ea8 compares its team.
	tPhysicalEntity *fn_004249c0(tPhysicalEntity *aEntity);
	// HYPOTHESIS: returns the first entity of a list linked through 0x8; 0x00413f59 walks it.
	tPhysicalEntity *fn_0055f269();
	// HYPOTHESIS: returns an int; 0x00421bbe stores it in a new ship-control entry.
	int fn_004239d6();
	// fn_0042496f (declared above) is also the lookup 0x00421ea9 and 0x004220d5 make for a
	// ship-control entry's target; they read the result as a tEntity.

	// HYPOTHESIS: returns an entity ID; 0x00421ea9 compares it with its ship's fID.
	int fn_00525081();

	char fField00[0x30];                  // +0x00
	tEntity *fEntities[0x4000];           // +0x30
	tObject_008e1060_slot *fSlots[16];    // +0x10030
};

extern tObject_008e1060 gObject_008e1060;

#endif	// tEntity_h
