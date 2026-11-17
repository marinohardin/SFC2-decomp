#include "Common/Thread/Optex.h"

// HYPOTHESIS: the owning class is not recovered; the function enters and leaves the tOptex
// held at offset 4 of its object.
class fn_005c427c_Owner
{
public:
	void FUN_005c427c();

	int		m00;
	tOptex	m04;
};

// FUNCTION: STARFLEETOP 0x005c427c
void fn_005c427c_Owner::FUN_005c427c()
{
	m04.Enter();
	m04.Leave();
}
