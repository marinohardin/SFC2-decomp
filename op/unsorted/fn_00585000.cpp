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

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0x28; its name is not recovered.
class fn_0058581d_owner {
public:
	int fn_0058581d();
	tOptex fAccess;
	char fn_0058581d_pad0[0x28 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x0058581d
int fn_0058581d_owner::fn_0058581d()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0x2c; its name is not recovered.
class fn_00585835_owner {
public:
	int fn_00585835();
	tOptex fAccess;
	char fn_00585835_pad0[0x2c - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585835
int fn_00585835_owner::fn_00585835()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex guarding an int at +0xa4; its name and the optex offset are not recovered.
class fn_0058584d_owner {
public:
	int fn_0058584d();
	tOptex fAccess;
	char fn_0058584d_pad0[0xa4 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x0058584d
int fn_0058584d_owner::fn_0058584d()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xa8; its name is not recovered.
class fn_00585868_owner {
public:
	int fn_00585868();
	tOptex fAccess;
	char fn_00585868_pad0[0xa8 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585868
int fn_00585868_owner::fn_00585868()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xac; its name is not known.
class fn_00585883_owner {
public:
	int fn_00585883();
	tOptex fAccess;
	char fn_00585883_pad0[0xac - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585883
int fn_00585883_owner::fn_00585883()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xb0; its name is not known.
class fn_0058589e_owner {
public:
	int fn_0058589e();
	tOptex fAccess;
	char fn_0058589e_pad0[0xb0 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x0058589e
int fn_0058589e_owner::fn_0058589e()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xb4; its name is not known.
class fn_005858b9_owner {
public:
	int fn_005858b9();
	tOptex fAccess;
	char fn_005858b9_pad0[0xb4 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005858b9
int fn_005858b9_owner::fn_005858b9()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xb8; its name is not known.
class fn_005858d4_owner {
public:
	int fn_005858d4();
	tOptex fAccess;
	char fn_005858d4_pad0[0xb8 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005858d4
int fn_005858d4_owner::fn_005858d4()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xbc; its name is not known.
class fn_005858ef_owner {
public:
	int fn_005858ef();
	tOptex fAccess;
	char fn_005858ef_pad0[0xbc - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005858ef
int fn_005858ef_owner::fn_005858ef()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xc0; its name is not known.
class fn_0058590a_owner {
public:
	int fn_0058590a();
	tOptex fAccess;
	char fn_0058590a_pad0[0xc0 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x0058590a
int fn_0058590a_owner::fn_0058590a()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xc4; its name is not known.
class fn_00585925_owner {
public:
	int fn_00585925();
	tOptex fAccess;
	char fn_00585925_pad0[0xc4 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585925
int fn_00585925_owner::fn_00585925()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xc8; its name is not known.
class fn_00585940_owner {
public:
	int fn_00585940();
	tOptex fAccess;
	char fn_00585940_pad0[0xc8 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585940
int fn_00585940_owner::fn_00585940()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xcc; its name is not known.
class fn_0058595b_owner {
public:
	int fn_0058595b();
	tOptex fAccess;
	char fn_0058595b_pad0[0xcc - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x0058595b
int fn_0058595b_owner::fn_0058595b()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xd0; its name is not known.
class fn_00585976_owner {
public:
	int fn_00585976();
	tOptex fAccess;
	char fn_00585976_pad0[0xd0 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585976
int fn_00585976_owner::fn_00585976()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xd4; its name is not known.
class fn_00585991_owner {
public:
	int fn_00585991();
	tOptex fAccess;
	char fn_00585991_pad0[0xd4 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585991
int fn_00585991_owner::fn_00585991()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xd8; its name is not known.
class fn_005859ac_owner {
public:
	int fn_005859ac();
	tOptex fAccess;
	char fn_005859ac_pad0[0xd8 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005859ac
int fn_005859ac_owner::fn_005859ac()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xdc; its name is not known.
class fn_005859c7_owner {
public:
	int fn_005859c7();
	tOptex fAccess;
	char fn_005859c7_pad0[0xdc - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005859c7
int fn_005859c7_owner::fn_005859c7()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xe0; its name is not known.
class fn_005859e2_owner {
public:
	int fn_005859e2();
	tOptex fAccess;
	char fn_005859e2_pad0[0xe0 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005859e2
int fn_005859e2_owner::fn_005859e2()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xe4; its name is not known.
class fn_005859fd_owner {
public:
	int fn_005859fd();
	tOptex fAccess;
	char fn_005859fd_pad0[0xe4 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x005859fd
int fn_005859fd_owner::fn_005859fd()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xe8; its name is not known.
class fn_00585a18_owner {
public:
	int fn_00585a18();
	tOptex fAccess;
	char fn_00585a18_pad0[0xe8 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585a18
int fn_00585a18_owner::fn_00585a18()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xec; its name is not known.
class fn_00585a33_owner {
public:
	int fn_00585a33();
	tOptex fAccess;
	char fn_00585a33_pad0[0xec - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585a33
int fn_00585a33_owner::fn_00585a33()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xf0; its name is not known.
class fn_00585a4e_owner {
public:
	int fn_00585a4e();
	tOptex fAccess;
	char fn_00585a4e_pad0[0xf0 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585a4e
int fn_00585a4e_owner::fn_00585a4e()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xf4; its name is not known.
class fn_00585a69_owner {
public:
	int fn_00585a69();
	tOptex fAccess;
	char fn_00585a69_pad0[0xf4 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585a69
int fn_00585a69_owner::fn_00585a69()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xf8; its name is not known.
class fn_00585a84_owner {
public:
	int fn_00585a84();
	tOptex fAccess;
	char fn_00585a84_pad0[0xf8 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585a84
int fn_00585a84_owner::fn_00585a84()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0xfc; its name is not known.
class fn_00585a9f_owner {
public:
	int fn_00585a9f();
	tOptex fAccess;
	char fn_00585a9f_pad0[0xfc - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585a9f
int fn_00585a9f_owner::fn_00585a9f()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x104; its name is not known.
class fn_00585aba_owner {
public:
	float fn_00585aba();
	tOptex fAccess;
	char fn_00585aba_pad0[0x104 - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585aba
float fn_00585aba_owner::fn_00585aba()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x108; its name is not known.
class fn_00585adc_owner {
public:
	float fn_00585adc();
	tOptex fAccess;
	char fn_00585adc_pad0[0x108 - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585adc
float fn_00585adc_owner::fn_00585adc()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x10c; its name is not known.
class fn_00585afe_owner {
public:
	float fn_00585afe();
	tOptex fAccess;
	char fn_00585afe_pad0[0x10c - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585afe
float fn_00585afe_owner::fn_00585afe()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x110; its name is not known.
class fn_00585b20_owner {
public:
	float fn_00585b20();
	tOptex fAccess;
	char fn_00585b20_pad0[0x110 - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585b20
float fn_00585b20_owner::fn_00585b20()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0x168; its name is not known.
class fn_00585b42_owner {
public:
	int fn_00585b42();
	tOptex fAccess;
	char fn_00585b42_pad0[0x168 - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585b42
int fn_00585b42_owner::fn_00585b42()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding an int at +0x16c; its name is not known.
class fn_00585b5d_owner {
public:
	int fn_00585b5d();
	tOptex fAccess;
	char fn_00585b5d_pad0[0x16c - sizeof(tOptex)];
	int fValue;
};

// FUNCTION: STARFLEETOP 0x00585b5d
int fn_00585b5d_owner::fn_00585b5d()
{
	fAccess.Enter();
	int value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x158; its name is not known.
class fn_00585b78_owner {
public:
	float fn_00585b78();
	tOptex fAccess;
	char fn_00585b78_pad0[0x158 - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585b78
float fn_00585b78_owner::fn_00585b78()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}

// HYPOTHESIS: a class with a tOptex at +0x0 guarding a float at +0x15c; its name is not known.
class fn_00585b9a_owner {
public:
	float fn_00585b9a();
	tOptex fAccess;
	char fn_00585b9a_pad0[0x15c - sizeof(tOptex)];
	float fValue;
};

// FUNCTION: STARFLEETOP 0x00585b9a
float fn_00585b9a_owner::fn_00585b9a()
{
	fAccess.Enter();
	float value = fValue;
	fAccess.Leave();
	return value;
}
