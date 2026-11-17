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
