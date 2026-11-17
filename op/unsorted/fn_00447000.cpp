#include "Tiger/tcglue.h"
#include "Tiger/tstring.h"
#include "unsorted/tEntity.h"
#include <string.h>
#include <string>

// HYPOTHESIS: a ship system as 0x0044715d reads it: slot 105 (offset 0x1a4) returns its name,
// slots 100, 102 and 103 (offsets 0x190, 0x198 and 0x19c) three integers. Its class is not known.
class tObject_0044715d_system
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
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual void vfunction99();
	virtual void vfunction100(int);
	virtual int vfunction101();
	virtual void vfunction102(int);
	virtual int vfunction103();
	virtual int vfunction104();
	virtual void vfunction105(const tString &);
	virtual tString *vfunction106();
};

// HYPOTHESIS: one row of the table 0x0044715d fills, one per ship system.
struct tObject_0044715d_row
{
	char field_0x0[0x8];
	std::string field_0x8;
	int field_0x18;
	int field_0x1c;
	int field_0x20;
	char field_0x24[0x4];
};

// HYPOTHESIS: the table at 0xec of the object 0x0044715d belongs to; seven rows from +0x10.
struct tObject_0044715d_table
{
	char field_0x0[0x10];
	tObject_0044715d_row field_0x10[7];
};

// HYPOTHESIS: three counters, each with a maximum, a second value and a current value.
struct tObject_00447415_limits
{
	char field_0x0[0x1a];
	char field_0x1a;
	char field_0x1b;
	short field_0x1c;
	char field_0x1e[0x2];
	short field_0x20;
	short field_0x22;
	short field_0x24;
	short field_0x26[25];
	short field_0x58[25];
	short field_0x8a[25];
	short field_0xbc[25];
	char field_0xee[0x2];
	char field_0xf0;
	char field_0xf1;
	char field_0xf2;
	char field_0xf3;
	char field_0xf4;
	char field_0xf5;
	char field_0xf6;
	char field_0xf7;
	char field_0xf8;
	char field_0xf9;
	char field_0xfa;
	char field_0xfb;
	char field_0xfc[0x10];
	char field_0x10c[3];
	char field_0x10f[3];
	char field_0x112[3];
	char field_0x115[0x65];
	// HYPOTHESIS: four byte arrays indexed alike; their lengths are not known.
	char field_0x17a[4];
	char field_0x17e[4];
	char field_0x182[4];
	char field_0x186[3];
	char field_0x189[0x3];
	// HYPOTHESIS: strings indexed alike; the array length is not known.
	std::string field_0x18c[4];
};

// HYPOTHESIS: the class owning 0x0044715d is not known; only its table pointer at 0xec is read.
class c_0044715d
{
public:
	void fn_0044715d(tObject_0042496f *ship);
	void fn_004472bc(tObject_0042496f *ship);
	int fn_00447415(int side, int kind);
	void fn_00447531(int side, int value);
	void fn_004475bc(int kind, int value);
	int fn_004476cf(int level);
	int fn_00447740(int level);
	int fn_004477b1(int index);
	void fn_004477c6(int index, char value, char current);
	int fn_004477f8(int index);
	int fn_0044780d(int index);
	std::string fn_00447822(int index);
	void fn_00447867(int index, std::string value);
	int fn_004478b5(int level);
	void fn_00447926(int kind, int value);
	int fn_004479b4(int kind);
	int fn_00447a54(int kind);
	void fn_00447aa7(tObject_0042496f *ship);
	// HYPOTHESIS: 0x00445a8c and 0x004466ba take the same this and ship as 0x0044715d.
	void fn_00445a8c(tObject_0042496f *ship);
	void fn_004466ba(tObject_0042496f *ship);

	char field_0x0[0x4];
	char field_0x4;
	char field_0x5[0x83];
	int field_0x88;
	char field_0x8c[0x5c];
	tObject_00447415_limits *field_0xe8;
	tObject_0044715d_table *field_0xec;
};

// FUNCTION: STARFLEETOP 0x0044715d
void c_0044715d::fn_0044715d(tObject_0042496f *ship)
{
	if (ship && ship->field_0xad4)
	{
		int i = 6;
		do
		{
			tObject_0044715d_system *system = NULL;
			// HYPOTHESIS: slots 77 and 80 return ship systems too; their classes are declared apart.
			if (i == 1)
				system = (tObject_0044715d_system *)ship->field_0xad4->vfunction77();
			if (i == 0)
				system = ship->field_0xad4->vfunction79();
			if (i == 5)
				system = ship->field_0xad4->vfunction78();
			if (i == 4)
				system = (tObject_0044715d_system *)ship->field_0xad4->vfunction80();
			if (i == 3)
				system = ship->field_0xad4->vfunction81();
			if (i == 2)
				system = ship->field_0xad4->vfunction83();
			if (i == 6)
				system = ship->field_0xad4->vfunction82();
			if (system)
			{
				field_0xec->field_0x10[i].field_0x8 = system->vfunction106()->mCharStr();
				field_0xec->field_0x10[i].field_0x18 = system->vfunction101();
				field_0xec->field_0x10[i].field_0x1c = system->vfunction103();
				field_0xec->field_0x10[i].field_0x20 = system->vfunction104();
			}
		}
		while (i--);
	}
}

// FUNCTION: STARFLEETOP 0x004472bc
void c_0044715d::fn_004472bc(tObject_0042496f *ship)
{
	if (ship && ship->field_0xad4)
	{
		int i = 6;
		do
		{
			tObject_0044715d_system *system = NULL;
			if (i == 1)
				system = (tObject_0044715d_system *)ship->field_0xad4->vfunction77();
			if (i == 0)
				system = ship->field_0xad4->vfunction79();
			if (i == 5)
				system = ship->field_0xad4->vfunction78();
			if (i == 4)
				system = (tObject_0044715d_system *)ship->field_0xad4->vfunction80();
			if (i == 3)
				system = ship->field_0xad4->vfunction81();
			if (i == 2)
				system = ship->field_0xad4->vfunction83();
			if (i == 6)
				system = ship->field_0xad4->vfunction82();
			if (system)
			{
				system->vfunction105(tString(field_0xec->field_0x10[i].field_0x8.c_str()));
				system->vfunction100(field_0xec->field_0x10[i].field_0x18);
				system->vfunction102(field_0xec->field_0x10[i].field_0x1c);
			}
		}
		while (i--);
	}
}

// FUNCTION: STARFLEETOP 0x00447415
int c_0044715d::fn_00447415(int side, int kind)
{
	switch (side)
	{
	case 0:
		switch (kind)
		{
		case 0:
			return field_0xe8->field_0x112[0] = (field_0xe8->field_0x112[0] < 0 || field_0xe8->field_0x112[0] > field_0xe8->field_0x10c[0]) ? 0 : field_0xe8->field_0x112[0];
		case 1:
			return field_0xe8->field_0x10f[0];
		case 2:
			return field_0xe8->field_0x10c[0];
		}
		break;
	case 1:
		switch (kind)
		{
		case 0:
			return field_0xe8->field_0x112[1] = (field_0xe8->field_0x112[1] < 0 || field_0xe8->field_0x112[1] > field_0xe8->field_0x10c[1]) ? 0 : field_0xe8->field_0x112[1];
		case 1:
			return field_0xe8->field_0x10f[1];
		case 2:
			return field_0xe8->field_0x10c[1];
		}
		break;
	case 2:
		switch (kind)
		{
		case 0:
			return field_0xe8->field_0x112[2] = (field_0xe8->field_0x112[2] < 0 || field_0xe8->field_0x112[2] > field_0xe8->field_0x10c[2]) ? 0 : field_0xe8->field_0x112[2];
		case 1:
			return field_0xe8->field_0x10f[2];
		case 2:
			return field_0xe8->field_0x10c[2];
		}
		break;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00447531
void c_0044715d::fn_00447531(int side, int value)
{
	switch (side)
	{
	case 0:
		if (value < 0 || value > field_0xe8->field_0x10c[0])
			value = 0;
		field_0xe8->field_0x112[0] = value;
		break;
	case 1:
		if (value < 0 || value > field_0xe8->field_0x10c[1])
			value = 0;
		field_0xe8->field_0x112[1] = value;
		break;
	case 2:
		if (value < 0 || value > field_0xe8->field_0x10c[2])
			value = 0;
		field_0xe8->field_0x112[2] = value;
		break;
	}
}

// FUNCTION: STARFLEETOP 0x004475bc
void c_0044715d::fn_004475bc(int kind, int value)
{
	int i;

	switch (kind)
	{
	case 0:
		field_0xe8->field_0x1a = value;
		break;
	case 1:
		field_0xe8->field_0x1b = value;
		break;
	case 2:
		field_0xe8->field_0x1c = value;
		break;
	case 3:
		field_0xe8->field_0x22 = value;
		for (i = 0; i < 25; i++)
		{
			if (field_0xe8->field_0xbc[i] != 0)
			{
				int n = value;
				if (value > field_0xe8->field_0x8a[i])
					n = field_0xe8->field_0x8a[i];
				field_0xe8->field_0x26[i] = n;
				value -= n;
			}
		}
		break;
	case 4:
		field_0xe8->field_0x24 = value;
		for (i = 0; i < 25; i++)
		{
			if (field_0xe8->field_0xbc[i] != 0)
			{
				int n = value;
				if (value > field_0xe8->field_0x8a[i] * (field_0xe8->field_0x1c + 1) * field_0xe8->field_0xbc[i] - field_0xe8->field_0x26[i])
					n = field_0xe8->field_0x8a[i] * (field_0xe8->field_0x1c + 1) * field_0xe8->field_0xbc[i] - field_0xe8->field_0x26[i];
				value -= n;
				field_0xe8->field_0x58[i] = n;
			}
		}
		break;
	case 5:
		field_0xe8->field_0x20 = value;
		break;
	}
}

// FUNCTION: STARFLEETOP 0x004476cf
int c_0044715d::fn_004476cf(int level)
{
	switch (level)
	{
	case 0:
		return field_0xe8->field_0x10f[0];
	case 1:
		return field_0xe8->field_0x10f[0] + (field_0xe8->field_0x10c[0] - field_0xe8->field_0x10f[0]) / 3;
	case 2:
		return field_0xe8->field_0x10f[0] + 2 * (field_0xe8->field_0x10c[0] - field_0xe8->field_0x10f[0]) / 3;
	case 3:
		return field_0xe8->field_0x10c[0];
	default:
		return 0;
	}
}

// FUNCTION: STARFLEETOP 0x00447740
int c_0044715d::fn_00447740(int level)
{
	switch (level)
	{
	case 0:
		return field_0xe8->field_0x10f[1];
	case 1:
		return field_0xe8->field_0x10f[1] + (field_0xe8->field_0x10c[1] - field_0xe8->field_0x10f[1]) / 3;
	case 2:
		return field_0xe8->field_0x10f[1] + 2 * (field_0xe8->field_0x10c[1] - field_0xe8->field_0x10f[1]) / 3;
	case 3:
		return field_0xe8->field_0x10c[1];
	default:
		return 0;
	}
}

// FUNCTION: STARFLEETOP 0x004477b1
int c_0044715d::fn_004477b1(int index)
{
	return field_0xe8->field_0x186[index];
}

// FUNCTION: STARFLEETOP 0x004477c6
void c_0044715d::fn_004477c6(int index, char value, char current)
{
	field_0xe8->field_0x17e[index] = value;
	field_0xe8->field_0x182[index] = value;
	field_0xe8->field_0x186[index] = current;
}

// FUNCTION: STARFLEETOP 0x004477f8
int c_0044715d::fn_004477f8(int index)
{
	return field_0xe8->field_0x182[index];
}

// FUNCTION: STARFLEETOP 0x0044780d
int c_0044715d::fn_0044780d(int index)
{
	return field_0xe8->field_0x17a[index];
}

// FUNCTION: STARFLEETOP 0x00447822
std::string c_0044715d::fn_00447822(int index)
{
	return field_0xe8->field_0x18c[index];
}

// FUNCTION: STARFLEETOP 0x00447867
void c_0044715d::fn_00447867(int index, std::string value)
{
	field_0xe8->field_0x18c[index] = value;
}

// FUNCTION: STARFLEETOP 0x004478b5
int c_0044715d::fn_004478b5(int level)
{
	switch (level)
	{
	case 0:
		return field_0xe8->field_0xf0;
	case 1:
		return field_0xe8->field_0xf0 + (field_0xe8->field_0xf1 - field_0xe8->field_0xf0) / 3;
	case 2:
		return field_0xe8->field_0xf0 + 2 * (field_0xe8->field_0xf1 - field_0xe8->field_0xf0) / 3;
	case 3:
		return field_0xe8->field_0xf1;
	default:
		return 0;
	}
}

// FUNCTION: STARFLEETOP 0x00447926
void c_0044715d::fn_00447926(int kind, int value)
{
	switch (kind)
	{
	case 0:
		if (value > field_0xe8->field_0xf1)
			value = field_0xe8->field_0xf1;
		field_0xe8->field_0xf2 = value;
		break;
	case 1:
		if (value > field_0xe8->field_0xfa)
			value = field_0xe8->field_0xfa;
		field_0xe8->field_0xfb = value;
		break;
	case 2:
		if (value > field_0xe8->field_0xf4)
			value = field_0xe8->field_0xf4;
		field_0xe8->field_0xf5 = value;
		break;
	case 3:
		if (value > field_0xe8->field_0xf7)
			value = field_0xe8->field_0xf7;
		field_0xe8->field_0xf8 = value;
		break;
	}
}

// FUNCTION: STARFLEETOP 0x004479b4
int c_0044715d::fn_004479b4(int kind)
{
	switch (kind)
	{
	case 0:
		return field_0xe8->field_0xf2 = (field_0xe8->field_0xf2 < 0 || field_0xe8->field_0xf2 > field_0xe8->field_0xf1) ? 0 : field_0xe8->field_0xf2;
	case 1:
		return field_0xe8->field_0xfb = (field_0xe8->field_0xfb < 0 || field_0xe8->field_0xfb > field_0xe8->field_0xfa) ? 0 : field_0xe8->field_0xfb;
	case 2:
		return field_0xe8->field_0xf5 = (field_0xe8->field_0xf5 < 0 || field_0xe8->field_0xf5 > field_0xe8->field_0xf4) ? 0 : field_0xe8->field_0xf5;
	case 3:
		return field_0xe8->field_0xf8 = (field_0xe8->field_0xf8 < 0 || field_0xe8->field_0xf8 > field_0xe8->field_0xf7) ? 0 : field_0xe8->field_0xf8;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00447a54
int c_0044715d::fn_00447a54(int kind)
{
	switch (kind)
	{
	case 0:
		return field_0xe8->field_0xf1;
	case 1:
		return field_0xe8->field_0xfa;
	case 2:
		return field_0xe8->field_0xf4;
	case 3:
		return field_0xe8->field_0xf7;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00447aa7
void c_0044715d::fn_00447aa7(tObject_0042496f *ship)
{
	if (ship != NULL)
	{
		field_0x4 = 1;
		clib::strcpy(field_0x5, ship->field_0x87d);
		field_0x88 = ship->field_0x86c;
		fn_00445a8c(ship);
		fn_004466ba(ship);
		fn_0044715d(ship);
	}
}

// HYPOTHESIS: the function really runs 0x00447af2-0x00447b79 (EH frame, a stack tShipSpecs); the recorded boundary stops after 31 bytes, so the body is left empty until it is corrected.
// FUNCTION: STARFLEETOP 0x00447af2
void fn_00447af2()
{
}
