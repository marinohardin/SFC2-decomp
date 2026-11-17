// Unit:      unsorted/fn_00585000.cpp
// Functions: 0x005856fd to 0x00585ffc, 79 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)
#include "Common/Thread/Optex.h"

// HYPOTHESIS: a class with a tOptex at +0x4 guarding a long at +0x1b0; its name is not recovered.
class fn_005856fd_owner {
public:
	long fn_005856fd();
	tOptex fAccess;
	char fn_005856fd_pad0[0x1b0 - sizeof(tOptex)];
	long fValue;
};

// FUNCTION: STARFLEETOP 0x005856fd
long fn_005856fd_owner::fn_005856fd()
{
	fAccess.Enter();
	long value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a long at +0x1a4; its name is not recovered.
class fn_00585718_owner {
public:
	long fn_00585718();
	tOptex fAccess;
	char fn_00585718_pad0[0x1a4 - sizeof(tOptex)];
	long fValue;
};

// FUNCTION: STARFLEETOP 0x00585718
long fn_00585718_owner::fn_00585718()
{
	fAccess.Enter();
	long value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a long at +0x1a8; its name is not recovered.
class fn_00585733_owner {
public:
	long fn_00585733();
	tOptex fAccess;
	char fn_00585733_pad0[0x1a8 - sizeof(tOptex)];
	long fValue;
};

// FUNCTION: STARFLEETOP 0x00585733
long fn_00585733_owner::fn_00585733()
{
	fAccess.Enter();
	long value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a long at +0x1ac; its name is not recovered.
class fn_0058574e_owner {
public:
	long fn_0058574e();
	tOptex fAccess;
	char fn_0058574e_pad0[0x1ac - sizeof(tOptex)];
	long fValue;
};

// FUNCTION: STARFLEETOP 0x0058574e
long fn_0058574e_owner::fn_0058574e()
{
	fAccess.Enter();
	long value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a long at +0x30; its name is not recovered.
class fn_00585769_owner {
public:
	long fn_00585769();
	tOptex fAccess;
	char fn_00585769_pad0[0x30 - sizeof(tOptex)];
	long fValue;
};

// FUNCTION: STARFLEETOP 0x00585769
long fn_00585769_owner::fn_00585769()
{
	fAccess.Enter();
	long value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x178; its name is not recovered.
class fn_00585781_owner {
public:
	float fn_00585781();
	tOptex fAccess;
	char fn_00585781_pad0[0x178 - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585781
float fn_00585781_owner::fn_00585781()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x17c; its name is not recovered.
class fn_005857a3_owner {
public:
	float fn_005857a3();
	tOptex fAccess;
	char fn_005857a3_pad0[0x17c - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x005857a3
float fn_005857a3_owner::fn_005857a3()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x180; its name is not recovered.
class fn_005857c5_owner {
public:
	float fn_005857c5();
	tOptex fAccess;
	char fn_005857c5_pad0[0x180 - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x005857c5
float fn_005857c5_owner::fn_005857c5()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0x100; its name is not recovered.
class fn_005857e7_owner {
public:
	int fn_005857e7();
	tOptex fAccess;
	char fn_005857e7_pad0[0x100 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005857e7
int fn_005857e7_owner::fn_005857e7()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0x164; its name is not recovered.
class fn_00585802_owner {
public:
	int fn_00585802();
	tOptex fAccess;
	char fn_00585802_pad0[0x164 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585802
int fn_00585802_owner::fn_00585802()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}
