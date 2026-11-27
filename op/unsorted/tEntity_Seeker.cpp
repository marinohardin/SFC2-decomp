#include "Tiger/tvectormath.h"
#include "unsorted/sfcstreams.h"
#include "unsorted/tEntity_Seeker.h"

void fn_0040e689(tSFCStream &s);
void fn_0040e691(tSFCStream &s);
void fn_0040ed22(tSFCStream &s, unsigned char flags);
void fn_0040ecfd(char *text, tSFCStream &s, unsigned char flags);
void fn_0040e9df(long *value, tSFCStream &s, unsigned char flags, char *name);
void fn_0040eafc(unsigned char *value, tSFCStream &s, unsigned char flags, char *name);
void fn_0040eb35(float *value, tSFCStream &s, unsigned char flags, char *name);
void fn_0040eb6e(tPoint3 *value, tSFCStream &s, unsigned char flags, char *name);

#include <stdio.h>

void fn_004d88e9(const char *param_1);
void *FUN_0040fe6c(const char *name);
extern double g_0090bef0;
extern double g_00797a18;
extern float g_00797a58;

// HYPOTHESIS: the trait FUN_0040fe6c looks up by name; only slot 4 is known.
class tETrait_Explosion
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4(int mode, tPoint3 *point, float *scale);
};

class EntitySlot18View
{
public:
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
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19(tETrait_Explosion *trait);
};

// FUNCTION: STARFLEETOP 0x0041e042
tEntity_Seeker::tEntity_Seeker()
{
	fClassification |= 0x20;
	field_0x278 = -1;
	field_0x280 = 0.0;
	field_0x274 = 0;
	field_0x270 = 0x100;
	field_0x268 = 0x100;
	field_0x264 = 0.0f;
	field_0x275 = 1;
	field_0x27c = 0;
	field_0x26c = 0;
}

// FUNCTION: STARFLEETOP 0x0041e09b
float tEntity_Seeker::vfunction63()
{
	return field_0x260;
}

// FUNCTION: STARFLEETOP 0x0041e0a2
char tEntity_Seeker::vfunction70()
{
	return field_0x275;
}

// FUNCTION: STARFLEETOP 0x0041e0a9
int tEntity_Seeker::vfunction18()
{
	return 0xc;
}

// HYPOTHESIS: the member layout from +0x248 on, as the stream calls address it.
// FUNCTION: STARFLEETOP 0x0041e64f
void tEntity_Seeker::vfunction11(tSFCStream &s, int32u mode)
{
	tEntity_Weapon::vfunction11(s, mode);
	fn_0040ed22(s, mode);
	fn_0040ecfd("Entity seeker:", s, mode);
	fn_0040e689(s);
	fn_0040eb6e((tPoint3 *)&field_0x248[0], s, mode, "fDirection");
	fn_0040eb6e((tPoint3 *)&field_0x248[0xc], s, mode, "fVelocity");
	fn_0040eb35(&field_0x260, s, mode, "fSpeed");
	fn_0040eb35(&field_0x264, s, mode, "fScale");
	fn_0040e9df((long *)&field_0x268, s, mode, "fHealth");
	fn_0040e9df((long *)&field_0x270, s, mode, "fMaxHealth");
	fn_0040e9df((long *)&field_0x26c, s, mode, "fDamageAccrued");
	fn_0040eafc((unsigned char *)&field_0x274, s, mode, "fExplosionMode");
	fn_0040eafc((unsigned char *)&field_0x275, s, mode, "fActive");
	fn_0040e9df((long *)&field_0x278, s, mode, "fLastWeasel");
	fn_0040e691(s);
}

// FUNCTION: STARFLEETOP 0x0041e79a
void tEntity_Seeker::vfunction26(short a1, int a2, int a3, int a4, int a5)
{
	if (fIsAlive)
	{
		int health = vfunction23();
		field_0x268 = health;
		int kind;
		if (a4 != 8 ||
			((kind = *(int *)&field_0x1bc[0]) != 0 && kind != 1 && kind != 2 && kind != 3 && kind != 4 && kind != 5 &&
			kind != 0x28 && kind != 6 && kind != 7 && kind != 0x2f))
		{
			kind = *(int *)&field_0x1bc[0];
			if (kind < 0 || (kind > 7 && kind != 0x28))
				field_0x268 = health - (a2 << 6);
			else
				field_0x26c += a2;
		}
		if (kind == 0x15 || kind == 0x16)
		{
			unsigned int packed = a5;
			field_0x268 = 0;
			field_0x26c = packed >> 16;
			fTargetID = packed & 0xffff;
		}
		if (field_0x268 <= 0)
		{
			if ((unsigned int)a2 <= 10000)
			{
				if (kind == 0x33)
				{
					if (field_0x274 != 0)
						return;
					if (field_0x275 != 1)
						return;
					double now = g_0090bef0;
					fProperties &= ~2;
					*(double *)&field_0x1dc[4] = now;
					field_0x1e8 = g_00797a18;
					tETrait_Explosion *trait = (tETrait_Explosion *)FUN_0040fe6c("tETrait_Explosion");
					if (trait != 0)
					{
						*(float *)&a4 = vfunction14() * g_00797a58;
						tPoint3 point;
						point.x = 0.0f;
						point.y = 0.0f;
						point.z = 0.0f;
						trait->vfunction4(1, &point, (float *)&a4);
						// HYPOTHESIS: slot 18 takes the trait here; tEntity declares it without
						// an argument, so the call goes through a view of the vftable.
						((EntitySlot18View *)this)->vfunction19(trait);
					}
					field_0x274 = 1;
					field_0x260 = 0.0f;
					return;
				}
			}
			else if (kind == 0x33)
			{
				vfunction60(-1);
			}
			char buffer[128];
			sprintf(buffer, "Seeking weapon %d,Destroyed by Damage of some type\n", fID);
			fn_004d88e9(buffer);
			vfunction5();
		}
	}
}
