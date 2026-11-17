// Functions with no unit or class evidence on page 0x0044c000; original file unknown.

#include <stdlib.h>
#include <string.h>

// HYPOTHESIS: the callees below have no recovered class; declared by address.
class fn_0044c185_trait;
class fn_0044c185_object;

struct fn_0044c185_vec3
{
	float x;
	float y;
	float z;

	fn_0044c185_vec3();
	fn_0044c185_vec3(float ax, float ay, float az);
	fn_0044c185_vec3 & operator=(const fn_0044c185_vec3 & other);
	void FUN_0069b1d0(const fn_0044c185_vec3 * other);
	void FUN_0069b3d0();
	fn_0044c185_vec3 FUN_0069b270(float k) const;
};

// HYPOTHESIS: inline constructors and assignment; the original copies the
// vector field by field, not as a block.
inline fn_0044c185_vec3::fn_0044c185_vec3()
{
}

inline fn_0044c185_vec3::fn_0044c185_vec3(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
}

inline fn_0044c185_vec3 & fn_0044c185_vec3::operator=(const fn_0044c185_vec3 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

// HYPOTHESIS: a point and a direction, filled together by FUN_0044c276.
struct fn_0044c185_frame
{
	fn_0044c185_vec3 pos;
	fn_0044c185_vec3 dir;
};

extern void * FUN_0040fe6c(const char * name);
extern float FUN_0069b0e0();

extern float g_00797280;
extern float g_0079de70;

class fn_0044c185_trait
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4(int mode, fn_0044c185_frame * frame, float * scale);
};

// HYPOTHESIS: a named point set; slot 10 looks a point up by label.
class fn_0044c276_points
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
	virtual char vfunction10(fn_0044c185_vec3 * point, const char * label);
};

class fn_0044c185_object
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
	virtual void vfunction12();
	virtual float vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18(fn_0044c185_trait * trait);

	fn_0044c276_points * FUN_004b94dc();
};

char FUN_0044c276(fn_0044c185_vec3 * point, fn_0044c185_object * object, const char * name);

// FUNCTION: STARFLEETOP 0x0044c185
void FUN_0044c185(fn_0044c185_object * object)
{
	fn_0044c185_trait * trait = (fn_0044c185_trait *)FUN_0040fe6c("tETrait_PlasmaVent");
	if (trait != 0)
	{
		float scale = object->vfunction13() * g_00797280;
		fn_0044c185_frame frame;
		if (FUN_0044c276(&frame.pos, object, "HardPoint"))
		{
			frame.dir.FUN_0069b1d0(&frame.pos);
			frame.dir.FUN_0069b3d0();
			frame.dir = frame.dir.FUN_0069b270(g_0079de70);
		}
		else
		{
			frame.pos = fn_0044c185_vec3(FUN_0069b0e0() * scale, FUN_0069b0e0() * scale, FUN_0069b0e0() * scale);
			frame.dir = frame.pos;
			frame.dir.FUN_0069b3d0();
			frame.dir = frame.dir.FUN_0069b270(g_0079de70);
		}
		trait->vfunction4(1, &frame, &scale);
		object->vfunction18(trait);
	}
}

// FUNCTION: STARFLEETOP 0x0044c276
char FUN_0044c276(fn_0044c185_vec3 * point, fn_0044c185_object * object, const char * name)
{
	fn_0044c276_points * points = object->FUN_004b94dc();
	if (points == 0)
	{
		return -1;
	}
	char label[80];
	strcpy(label, name);
	char * digits = label + strlen(label);
	int start = rand() % 20;
	fn_0044c185_vec3 * next = point;
	int found = 0;
	int index = start;
	do
	{
		_itoa(index, digits, 10);
		if (points->vfunction10(next, label))
		{
			found++;
			next++;
			if (found >= 2)
			{
				return 1;
			}
		}
		index++;
		if (index >= 20)
		{
			index = 0;
		}
	}
	while (index != start);
	return 0;
}

// FUNCTION: STARFLEETOP 0x0044c308
void FUN_0044c308(fn_0044c185_object * object)
{
	fn_0044c185_frame frame;
	if (FUN_0044c276(&frame.pos, object, "HardPoint"))
	{
		fn_0044c185_trait * trait = (fn_0044c185_trait *)FUN_0040fe6c("tETrait_ElectricSpark");
		if (trait != 0)
		{
			float scale = 1.0f;
			trait->vfunction4(1, &frame, &scale);
			object->vfunction18(trait);
		}
	}
}

#include "unsorted/fn_0040ce21_type.h"
#include "unsorted/fn_0090be9c_type.h"

extern double g_0079de48;
extern float g_0079de40;
extern double g_00799f68;
extern double g_0079a148;

// HYPOTHESIS: one ship system as seen by the key handler; vfunction9 is a level,
// vfunction52 takes the key's argument, vfunction55 is a count, vfunction96 a flag.
class fn_0044c35d_part
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
	virtual int vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
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
	virtual void vfunction52(int level);
	virtual void vfunction53();
	virtual void vfunction54();
	virtual int vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
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
	virtual char vfunction96();
};

// HYPOTHESIS: the record behind the slot at +0x5cc (and behind vfunction84).
class fn_0044c35d_group
{
public:
	char pad00[0x58];
	fn_0044c35d_part * field_58;
	fn_0044c35d_part * field_5c;
	fn_0044c35d_part * field_60;
	fn_0044c35d_part * field_64;
	fn_0044c35d_part * field_68;
	char pad6c[0x80 - 0x6c];
	fn_0044c35d_part * field_80;
	fn_0044c35d_part * field_84;
};

// HYPOTHESIS: the ship object at +0x6ac.
class fn_0044c35d_ship
{
public:
	char pad00[0x870];
	int field_870;
	int FUN_00414f41();
};

// HYPOTHESIS: the system set that owns the key handler; slots are named by offset / 4.
class fn_0044c35d_systems
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
	virtual void vfunction12();
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
	virtual fn_0044c35d_part * vfunction58();
	virtual fn_0044c35d_part * vfunction59();
	virtual fn_0044c35d_part * vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual fn_0044c35d_part * vfunction63();
	virtual void vfunction64();
	virtual void vfunction65();
	virtual void vfunction66();
	virtual void vfunction67();
	virtual void vfunction68();
	virtual fn_0044c35d_part * vfunction69();
	virtual fn_0044c35d_part * vfunction70();
	virtual fn_0044c35d_part * vfunction71();
	virtual fn_0044c35d_part * vfunction72();
	virtual fn_0044c35d_part * vfunction73();
	virtual fn_0044c35d_part * vfunction74();
	virtual fn_0044c35d_part * vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	virtual void vfunction80();
	virtual void vfunction81();
	virtual void vfunction82();
	virtual void vfunction83();
	virtual fn_0044c35d_group * vfunction84();
	virtual void vfunction85();
	virtual fn_0044c35d_part * vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	virtual void vfunction89();
	virtual void vfunction90();
	virtual void vfunction91();
	virtual fn_0044c35d_part * vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual void vfunction95();
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual fn_0044c35d_part * vfunction99();

	char pad04[0x584 - 0x04];
	fn_0044c35d_part * field_584[0x4a];
	fn_0044c35d_ship * field_6ac;

	char FUN_0044c35d(int key, short param_2, char param_3);
	char FUN_0044d50b(short param_2, char param_3);
	char FUN_0044d8fe(char param_3);
	char FUN_0044dd86(char param_3);
};

// FUNCTION: STARFLEETOP 0x0044c35d
char fn_0044c35d_systems::FUN_0044c35d(int key, short param_2, char param_3)
{
	char result = 0;
	if (key == 0x4d)
	{
		float scaled;
		if ((scaled = (float)((double)param_2 * g_0079de48)) < g_0079de40)
		{
			if (vfunction59() && vfunction59()->vfunction9() > 2)
			{
				vfunction59()->vfunction52(param_3);
				result = 1;
			}
			else if (vfunction60() && vfunction60()->vfunction9() > 2)
			{
				vfunction60()->vfunction52(param_3);
				result = 1;
			}
			else if (vfunction58() && vfunction58()->vfunction9() > 2)
			{
				vfunction58()->vfunction52(param_3);
				result = 1;
			}
		}
		else
		{
			if (vfunction58() && vfunction58()->vfunction9() > 2)
			{
				vfunction58()->vfunction52(param_3);
				result = 1;
				static fn_0040ce21_type boing("Boing!");
			}
			else if (vfunction60() && vfunction60()->vfunction9() > 2)
			{
				vfunction60()->vfunction52(param_3);
				result = 1;
			}
			else if (vfunction59() && vfunction59()->vfunction9() > 2)
			{
				vfunction59()->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x4a)
	{
		fn_0090be9c_type * first = g_0090be9c;
		int high = (int)((double)first->vfunction7() * g_00799f68 / (double)first->field_20);
		fn_0090be9c_type * second = g_0090be9c;
		int low = (int)((double)second->vfunction7() * g_0079a148 / (double)second->field_20);
		int step = high - low + 2;
		// MATCH-ONLY: the original enters the 0x4e, 0x4c and 0x4b bodies below directly from here.
		if (step >= 4 && step <= 6)
		{
			goto case_4c;
		}
		else if (step >= 8 && step <= 10)
		{
			goto case_4b;
		}
		else
		{
			goto case_4e;
		}
	}
	else if (key == 0x52)
	{
		fn_0044c35d_group * group = (fn_0044c35d_group *)field_584[18];
		if (group)
		{
			fn_0044c35d_part * part = group->field_58;
			if (part && part->vfunction9() > 2)
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x53)
	{
		fn_0044c35d_group * group = (fn_0044c35d_group *)field_584[18];
		if (group)
		{
			fn_0044c35d_part * part = group->field_5c;
			if (part && part->vfunction9() > 2)
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x54)
	{
		fn_0044c35d_group * group = (fn_0044c35d_group *)field_584[18];
		if (group)
		{
			fn_0044c35d_part * part = group->field_60;
			if (part && part->vfunction9() > 2)
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x56)
	{
		fn_0044c35d_group * group = (fn_0044c35d_group *)field_584[18];
		if (group)
		{
			fn_0044c35d_part * part = group->field_64;
			if (part && part->vfunction55() > 0)
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x55)
	{
		fn_0044c35d_group * group = (fn_0044c35d_group *)field_584[18];
		if (group)
		{
			fn_0044c35d_part * part = group->field_68;
			if (part && part->vfunction55() > 0)
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x57)
	{
		fn_0044c35d_part * part = vfunction63();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
	}
	else if (key == 0x58 || key == 0x59 || key == 0x5f)
	{
		fn_0044c35d_part * part = vfunction63();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
	}
	else if (key == 0x60)
	{
		fn_0044c35d_part * part = vfunction75();
		if (part && part->vfunction55() > 0 && !part->vfunction96())
		{
			part->vfunction52(param_3);
			result = 1;
		}
	}
	else if (key == 0x5a)
	{
		fn_0044c35d_group * group = vfunction84();
		if (group)
		{
			fn_0044c35d_part * part = group->field_80;
			if (part && part->vfunction55() > 0 && !part->vfunction96())
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x5b)
	{
		fn_0044c35d_group * group = vfunction84();
		if (group)
		{
			fn_0044c35d_part * part = group->field_84;
			if (part && part->vfunction55() > 0 && !part->vfunction96())
			{
				part->vfunction52(param_3);
				result = 1;
			}
		}
	}
	else if (key == 0x5c)
	{
		fn_0044c35d_part * part = vfunction69();
		if (part && part->vfunction55() > 0 && !part->vfunction96())
		{
			part->vfunction52(param_3);
			result = 1;
		}
	}
	else if (key == 0x4f)
	{
		fn_0044c35d_part * part = vfunction72();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
		else
		{
			part = vfunction74();
			if (part && part->vfunction55() > 0)
			{
				part->vfunction52(param_3);
				result = 1;
			}
			else
			{
				part = vfunction99();
				if (part && part->vfunction55() > 0)
				{
					part->vfunction52(param_3);
					result = 1;
				}
				else
				{
					part = vfunction71();
					if (part && part->vfunction55() > 0)
					{
						part->vfunction52(param_3);
						result = 1;
					}
				}
			}
		}
	}
	else if (key == 0x50)
	{
		fn_0044c35d_part * part = vfunction74();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
		else
		{
			part = vfunction99();
			if (part && part->vfunction55() > 0)
			{
				part->vfunction52(param_3);
				result = 1;
			}
			else
			{
				part = vfunction71();
				if (part && part->vfunction55() > 0)
				{
					part->vfunction52(param_3);
					result = 1;
				}
			}
		}
	}
	else if (key == 0x51)
	{
		fn_0044c35d_part * part = vfunction73();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
			static fn_0040ce21_type plink("Plink!");
		}
		else
		{
			part = vfunction74();
			if (part && part->vfunction55() > 0)
			{
				part->vfunction52(param_3);
				result = 1;
			}
			else
			{
				part = vfunction99();
				if (part && part->vfunction55() > 0)
				{
					part->vfunction52(param_3);
					result = 1;
				}
				else
				{
					part = vfunction71();
					if (part && part->vfunction55() > 0)
					{
						part->vfunction52(param_3);
						result = 1;
					}
				}
			}
		}
	}
	else if (key == 0x5d)
	{
		fn_0044c35d_part * part = vfunction70();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
	}
	else if (key == 0x5e)
	{
		fn_0044c35d_part * part = vfunction71();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
	}
	else if (key == 0xc)
	{
		fn_0044c35d_part * part = vfunction92();
		if (part && part->vfunction55() > 0)
		{
			part->vfunction52(param_3);
			result = 1;
		}
		else
		{
			part = vfunction86();
			if (part && part->vfunction55() > 0)
			{
				int type = field_6ac->field_870;
				if (type != 0x18 && type != 0x1a)
				{
					part->vfunction52(param_3);
					result = 1;
					if (field_6ac->FUN_00414f41() < 10)
					{
						part->vfunction52(param_3 * 4);
					}
				}
			}
		}
	}
	else
	{
		if (key >= 0 && key < 0x4a)
		{
			if (field_584[key] != 0 && field_584[key]->vfunction55() > 0)
			{
				field_584[key]->vfunction52(param_3);
				result = 1;
			}
		}
		if (key == 0x4e)
		{
case_4e:
			result = FUN_0044d50b(param_2, param_3);
		}
		else if (key == 0x4c)
		{
case_4c:
			result = FUN_0044d8fe(param_3);
		}
		else if (key == 0x4b)
		{
case_4b:
			result = FUN_0044dd86(param_3);
		}
	}
	return result;
}

#include "unsorted/tShipSystemsExt.h"
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tBaseWeapon.h"

// FUNCTION: STARFLEETOP 0x0044c951
tIndividualShipSystemList* tShipSystemsExt::vfunction171()
{
	static fn_0040ce21_type marker("Ask Beck!");
	return &m04;
}

// HYPOTHESIS: the block at m584 is a table of 0x4a dwords indexed by slot.
// FUNCTION: STARFLEETOP 0x0044c984
int tShipSystemsExt::vfunction149( int param_1 )
{
	if ( param_1 < 0 || param_1 >= 0x4a )
		return 0;
	return ( (int*)m584 )[param_1];
}

// HYPOTHESIS: the same table as vfunction149, read only for slots 0x31..0x48.
// FUNCTION: STARFLEETOP 0x0044c99f
int tShipSystemsExt::vfunction148( int param_1 )
{
	if ( param_1 < 0x31 || param_1 >= 0x49 )
		return 0;
	return ( (int*)m584 )[param_1];
}

// FUNCTION: STARFLEETOP 0x0044c9bb
float tShipSystemsExt::vfunction124()
{
	float total = 0.0f;
	for ( int i = 0x3b; i < 0x49; i++ )
	{
		tBaseWeapon* weapon = dynamic_cast<tBaseWeapon*>( ( (tIndividualShipSystem**)m584 )[i] );
		if ( weapon )
		{
			if ( weapon->vfunction64() == true )
			{
				if ( weapon->vfunction27() == true )
				{
					total += weapon->vfunction68() * weapon->vfunction106();
				}
			}
		}
	}
	return total;
}

// HYPOTHESIS: slots 0x31..0x48 of the table at m584 (vfunction148's range) hold weapons; the
// original reads them without a cast check.
// FUNCTION: STARFLEETOP 0x0044ca3f
float tShipSystemsExt::vfunction125()
{
	float total = 0.0f;
	for ( int i = 0x31; i < 0x49; i++ )
	{
		tBaseWeapon* weapon = ( (tBaseWeapon**)m584 )[i];
		if ( weapon )
		{
			if ( weapon->vfunction64() == true )
			{
				if ( weapon->vfunction27() == true )
				{
					if ( weapon->vfunction134() == false )
					{
						total += weapon->vfunction68() * weapon->vfunction106();
					}
				}
			}
		}
	}
	return total;
}

// FUNCTION: STARFLEETOP 0x0044cab9
float tShipSystemsExt::vfunction138()
{
	float total = 0.0f;
	for ( int i = 0x31; i < 0x49; i++ )
	{
		tBaseWeapon* weapon = ( (tBaseWeapon**)m584 )[i];
		if ( weapon )
		{
			if ( weapon->vfunction64() == true )
			{
				if ( weapon->vfunction28() == true )
				{
					total += weapon->vfunction68() * weapon->vfunction106();
				}
			}
		}
	}
	return total;
}

// FUNCTION: STARFLEETOP 0x0044cb25
int tShipSystemsExt::vfunction139()
{
	int count = 0;
	for ( int i = 0x31; i < 0x49; i++ )
	{
		tBaseWeapon* weapon = ( (tBaseWeapon**)m584 )[i];
		if ( weapon )
		{
			if ( weapon->vfunction64() == true )
			{
				if ( weapon->vfunction28() == true )
				{
					count += weapon->vfunction106();
				}
			}
		}
	}
	return count;
}

#include "unsorted/tESG2.h"

// FUNCTION: STARFLEETOP 0x0044cb6f
int tShipSystemsExt::vfunction140()
{
	int count = 0;
	for ( int i = 0x31; i < 0x49; i++ )
	{
		tBaseWeapon* weapon = ( (tBaseWeapon**)m584 )[i];
		if ( weapon )
		{
			if ( weapon->vfunction64() == true )
			{
				if ( weapon->vfunction28() == true )
				{
					weapon->vfunction106();
					tESG2* esg = dynamic_cast<tESG2*>( weapon );
					if ( esg )
					{
						count += esg->fn_004dd414();
					}
				}
			}
		}
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x0044cbd8
float tShipSystemsExt::vfunction113()
{
	return m6c8;
}

// FUNCTION: STARFLEETOP 0x0044cbdf
void tShipSystemsExt::vfunction114( float param_1 )
{
	tMinMaxFloat* value = &m6c8;
	value->vfunction3( param_1 );
}

// FUNCTION: STARFLEETOP 0x0044cbf9
tMinMaxFloat* tShipSystemsExt::vfunction126()
{
	return &m6f4;
}

// FUNCTION: STARFLEETOP 0x0044cc00
void tShipSystemsExt::vfunction127( float param_1 )
{
	tMinMaxFloat* value = &m6f4;
	value->vfunction3( param_1 );
}

// FUNCTION: STARFLEETOP 0x0044cc1a
void tShipSystemsExt::vfunction154( int param_1 )
{
	// HYPOTHESIS: the constructor clears +0x9a4..+0x9af as one block; this is its first int.
	m9a4[0] += param_1;
}

// FUNCTION: STARFLEETOP 0x0044cc27
void tShipSystemsExt::vfunction153()
{
	// HYPOTHESIS: m9a4 holds a three-entry history that shifts when m944 changes.
	if ( m948 != m944 )
	{
		m948 = m944;
		m9a4[2] = m9a4[1];
		m9a4[1] = m9a4[0];
		m9a4[0] = 0;
	}
}

// FUNCTION: STARFLEETOP 0x0044cc5d
void tShipSystemsExt::vfunction115( float param_1 )
{
	// HYPOTHESIS: m6f0 caps m6ec.
	m6ec += param_1;
	if ( m6ec > m6f0 )
	{
		m6ec = m6f0;
	}
}

// FUNCTION: STARFLEETOP 0x0044cc85
void tShipSystemsExt::vfunction116( float param_1 )
{
	m6ec -= param_1;
	if ( m6ec < 0.0f )
	{
		m6ec = 0.0f;
	}
}

// FUNCTION: STARFLEETOP 0x0044ccab
void tShipSystemsExt::vfunction117( float param_1 )
{
	m6ec = param_1;
	if ( param_1 > m6f0 )
	{
		m6ec = m6f0;
	}
	if ( m6ec < 0.0f )
	{
		m6ec = 0.0f;
	}
}

// FUNCTION: STARFLEETOP 0x0044cce6
float tShipSystemsExt::vfunction118()
{
	return m6ec;
}

// HYPOTHESIS: +0x648 holds 24 system pointers; m6f0 becomes the sum over the active ones of
// slot 10 times slot 67.
// FUNCTION: STARFLEETOP 0x0044cced
void tShipSystemsExt::vfunction119()
{
	m6f0 = 0.0f;
	for ( int i = 0; i < 24; i++ )
	{
		tIndividualShipSystem* system = ( (tIndividualShipSystem**)( m584 + 0xc4 ) )[i];
		if ( system != 0 )
		{
			if ( system->vfunction27() == true )
			{
				m6f0 += system->vfunction11() * system->vfunction68();
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x0044cd53
float tShipSystemsExt::vfunction120()
{
	return m6f0;
}

// FUNCTION: STARFLEETOP 0x0044cd5a
void tShipSystemsExt::vfunction128( float param_1 )
{
	tMinMaxFloat* value = &m718;
	value->vfunction3( param_1 + value->m18 );
}

// FUNCTION: STARFLEETOP 0x0044cd7a
void tShipSystemsExt::vfunction129( float param_1 )
{
	tMinMaxFloat* value = &m718;
	value->vfunction3( value->m18 - param_1 );
	if ( m718.m18 < 0.0f )
	{
		value->vfunction3( 0.0f );
	}
}
