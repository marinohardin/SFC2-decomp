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

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the value
// read under the lock at offset 0x2e4.
class fn_005c4290_Holder
{
public:
	int FUN_005c4290();

	int		m00;
	tOptex	m04;
	char	pad[0x2e4 - 4 - sizeof(tOptex)];
	int		m2e4;
};

// FUNCTION: STARFLEETOP 0x005c4290
int fn_005c4290_Holder::FUN_005c4290()
{
	m04.Enter();
	int value = m2e4;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the value
// read under the lock at offset 0x2d8.
class fn_005c42b0_Holder
{
public:
	int FUN_005c42b0();

	int		m00;
	tOptex	m04;
	char	pad[0x2d8 - 4 - sizeof(tOptex)];
	int		m2d8;
};

// FUNCTION: STARFLEETOP 0x005c42b0
int fn_005c42b0_Holder::FUN_005c42b0()
{
	m04.Enter();
	int value = m2d8;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the value
// read under the lock at offset 0x2dc.
class fn_005c42d0_Holder
{
public:
	int FUN_005c42d0();

	int		m00;
	tOptex	m04;
	char	pad[0x2dc - 4 - sizeof(tOptex)];
	int		m2dc;
};

// FUNCTION: STARFLEETOP 0x005c42d0
int fn_005c42d0_Holder::FUN_005c42d0()
{
	m04.Enter();
	int value = m2dc;
	m04.Leave();
	return value;
}
