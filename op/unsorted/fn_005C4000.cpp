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

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the float
// value read under the lock at offset 0x2a8.
class fn_005c42f0_Holder
{
public:
	float FUN_005c42f0();

	int		m00;
	tOptex	m04;
	char	pad[0x2a8 - 4 - sizeof(tOptex)];
	float	m2a8;
};

// FUNCTION: STARFLEETOP 0x005c42f0
float fn_005c42f0_Holder::FUN_005c42f0()
{
	m04.Enter();
	float value = m2a8;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the float
// value read under the lock at offset 0x2ac.
class fn_005c4319_Holder
{
public:
	float FUN_005c4319();

	int		m00;
	tOptex	m04;
	char	pad[0x2ac - 4 - sizeof(tOptex)];
	float	m2ac;
};

// FUNCTION: STARFLEETOP 0x005c4319
float fn_005c4319_Holder::FUN_005c4319()
{
	m04.Enter();
	float value = m2ac;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the int
// value read under the lock at offset 0x30c.
class fn_005c4342_Holder
{
public:
	int FUN_005c4342();

	int		m00;
	tOptex	m04;
	char	pad[0x30c - 4 - sizeof(tOptex)];
	int		m30c;
};

// FUNCTION: STARFLEETOP 0x005c4342
int fn_005c4342_Holder::FUN_005c4342()
{
	m04.Enter();
	int value = m30c;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the int
// value read under the lock at offset 0x310.
class fn_005c4362_Holder
{
public:
	int FUN_005c4362();

	int		m00;
	tOptex	m04;
	char	pad[0x310 - 4 - sizeof(tOptex)];
	int		m310;
};

// FUNCTION: STARFLEETOP 0x005c4362
int fn_005c4362_Holder::FUN_005c4362()
{
	m04.Enter();
	int value = m310;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the int
// value read under the lock at offset 0x314.
class fn_005c4382_Holder
{
public:
	int FUN_005c4382();

	int		m00;
	tOptex	m04;
	char	pad[0x314 - 4 - sizeof(tOptex)];
	int		m314;
};

// FUNCTION: STARFLEETOP 0x005c4382
int fn_005c4382_Holder::FUN_005c4382()
{
	m04.Enter();
	int value = m314;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the int
// value read under the lock at offset 0x318.
class fn_005c43a2_Holder
{
public:
	int FUN_005c43a2();

	int		m00;
	tOptex	m04;
	char	pad[0x318 - 4 - sizeof(tOptex)];
	int		m318;
};

// FUNCTION: STARFLEETOP 0x005c43a2
int fn_005c43a2_Holder::FUN_005c43a2()
{
	m04.Enter();
	int value = m318;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the int
// value read under the lock at offset 0x31c.
class fn_005c43c2_Holder
{
public:
	int FUN_005c43c2();

	int		m00;
	tOptex	m04;
	char	pad[0x31c - 4 - sizeof(tOptex)];
	int		m31c;
};

// FUNCTION: STARFLEETOP 0x005c43c2
int fn_005c43c2_Holder::FUN_005c43c2()
{
	m04.Enter();
	int value = m31c;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the int
// value read under the lock at offset 0x320.
class fn_005c43e2_Holder
{
public:
	int FUN_005c43e2();

	int		m00;
	tOptex	m04;
	char	pad[0x320 - 4 - sizeof(tOptex)];
	int		m320;
	int		m324;
	int		m328;
	int FUN_005c4402();
	int		m32c;
	int FUN_005c4422();
	int FUN_005c4442();
};

// FUNCTION: STARFLEETOP 0x005c43e2
int fn_005c43e2_Holder::FUN_005c43e2()
{
	m04.Enter();
	int value = m320;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4402
int fn_005c43e2_Holder::FUN_005c4402()
{
	m04.Enter();
	int value = m324;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4422
int fn_005c43e2_Holder::FUN_005c4422()
{
	m04.Enter();
	int value = m328;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4442
int fn_005c43e2_Holder::FUN_005c4442()
{
	m04.Enter();
	int value = m32c;
	m04.Leave();
	return value;
}
