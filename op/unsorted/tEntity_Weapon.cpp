#include "unsorted/tEntity_Weapon.h"

// FUNCTION: STARFLEETOP 0x0041c1ef
tEntity_Weapon::tEntity_Weapon()
{
	fProperties |= 1;
	field_0x1c4 = -1;
	field_0x1e8 = 0.025f;
	fClassification |= 4;
	field_0x1d8 = 0;
	field_0x1f0 = 0;
	field_0x1f4 = 0;
	field_0x244 = 0;
	field_0x245 = 0;
}

// FUNCTION: STARFLEETOP 0x0041c23c
short tEntity_Weapon::vfunction66( char aValue )
{
	return 0;
}

// FUNCTION: STARFLEETOP 0x0041c242
tID tEntity_Weapon::vfunction68()
{
	return field_0x1c4;
}

// FUNCTION: STARFLEETOP 0x0041c249
int tEntity_Weapon::vfunction69()
{
	return field_0x1f0;
}

// HYPOTHESIS: a three-float setter; the class is unknown (likely tPoint3::mSet's out-of-line copy).
struct fn_0041c983_Triple
{
	float	a, b, c;
	fn_0041c983_Triple *fn_0041c983(float aA, float aB, float aC);
};

// FUNCTION: STARFLEETOP 0x0041c983
fn_0041c983_Triple *fn_0041c983_Triple::fn_0041c983(float aA, float aB, float aC)
{
	a = aA;
	b = aB;
	c = aC;
	return this;
}

// HYPOTHESIS: the stream archive helpers; see Source/entity/entity_physical.cpp.
void fn_0040ed22(tSFCStream &s, unsigned char mode);
void fn_0040ecfd(char *aTitle, tSFCStream &s, unsigned char mode);
void fn_0040e689(tSFCStream &s);
void fn_0040ec85(int aSize, void *aValue, tSFCStream &s, unsigned char mode, char *aText);
void fn_0040e9df(long *aValue, tSFCStream &s, unsigned char mode, char *aText);
void fn_0040eb35(float *aValue, tSFCStream &s, unsigned char mode, char *aText);
void fn_0040eb6e(tPoint3 *aValue, tSFCStream &s, unsigned char mode, char *aText);
void fn_0040ec4c(double *aValue, tSFCStream &s, unsigned char mode, char *aText);
void fn_0040e691(tSFCStream &s);

// FUNCTION: STARFLEETOP 0x0041c99c
void tEntity_Weapon::vfunction5()
{
	char* p = (char*)&fIsAlive;
	if (*p != 0)
	{
		*p = 0;
	}
}

// FUNCTION: STARFLEETOP 0x0041c9a7
void tEntity_Weapon::vfunction11(tSFCStream &s, int32u mode)
{
	tPhysicalEntity::vfunction11(s, mode);
	fn_0040ed22(s, mode);
	fn_0040ecfd("Entity weapon:", s, mode);
	fn_0040e689(s);
	char *base = (char *)this;
	fn_0040ec85(4, base + 0x1bc, s, mode, "fWeaponType");
	fn_0040ec85(1, base + 0x1c0, s, mode, "fMode");
	fn_0040e9df((long *)(base + 0x1c4), s, mode, "fFromID");
	fn_0040eb35((float *)(base + 0x1c8), s, mode, "fMaxDamage");
	fn_0040eb6e((tPoint3 *)(base + 0x1cc), s, mode, "fOffset");
	fn_0040e9df((long *)(base + 0x1d8), s, mode, "fSpecialDamageFlags");
	fn_0040ec4c((double *)(base + 0x1e0), s, mode, "fBirthTime");
	fn_0040ec4c((double *)(base + 0x1e8), s, mode, "fMaxAge");
	fn_0040e691(s);
}