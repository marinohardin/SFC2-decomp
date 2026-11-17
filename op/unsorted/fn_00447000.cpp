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
	virtual void vfunction100();
	virtual int vfunction101();
	virtual void vfunction102();
	virtual int vfunction103();
	virtual int vfunction104();
	virtual void vfunction105();
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

// HYPOTHESIS: the class owning 0x0044715d is not known; only its table pointer at 0xec is read.
class c_0044715d
{
public:
	void fn_0044715d(tObject_0042496f *ship);

	char field_0x0[0xec];
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
