#include "Common/Thread/Optex.h"

// HYPOTHESIS: class and member names are not recovered; the object is a tOptex
// with a member at +0x238 read under the lock.
class fn_005A4173_Owner : public tOptex
{
public:
	int		fn_005A4173();
	int		fn_005A418e();

	char		pad[0x20c - sizeof(tOptex)];
	int		fValue20c;
	char		pad2[0x238 - 0x210];
	int		fValue;
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
