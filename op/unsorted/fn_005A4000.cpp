#include "Common/Thread/Optex.h"

// HYPOTHESIS: class and member names are not recovered; the object is a tOptex
// with a member at +0x238 read under the lock.
class fn_005A4173_Owner : public tOptex
{
public:
	int		fn_005A4173();
	int		fn_005A418e();

	int		fn_005A41a9();

	char		pad[0x208 - sizeof(tOptex)];
	int		fValue208;
	int		fValue20c;
	float		fValue210;
	float		fValue214;
	char		pad2[0x238 - 0x218];
	int		fValue;

	float		fn_005A41c4();
	float		fn_005A41e6();
};

// FUNCTION: STARFLEETOP 0x005a4173
int fn_005A4173_Owner::fn_005A4173()
{
	Enter();
	int value = fValue;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a418e
int fn_005A4173_Owner::fn_005A418e()
{
	Enter();
	int value = fValue20c;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a41a9
int fn_005A4173_Owner::fn_005A41a9()
{
	Enter();
	int value = fValue208;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a41c4
float fn_005A4173_Owner::fn_005A41c4()
{
	Enter();
	float value = fValue210;
	Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005a41e6
float fn_005A4173_Owner::fn_005A41e6()
{
	Enter();
	float value = fValue214;
	Leave();
	return value;
}
