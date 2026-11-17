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
	char	pad2[0x33c - 0x330];
	int		m33c;
	int FUN_005c4482();
	int FUN_005c44a2();
	int FUN_005c44c2();
	int FUN_005c44e2(int param_1);
	int		m340;
	int		m344;
	float	m348;
	int		m34c;
	float FUN_005c450c();
	int FUN_005c4462();
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

// FUNCTION: STARFLEETOP 0x005c4462
int fn_005c43e2_Holder::FUN_005c4462()
{
	m04.Enter();
	int value = m34c;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4482
int fn_005c43e2_Holder::FUN_005c4482()
{
	m04.Enter();
	int value = m33c;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c44a2
int fn_005c43e2_Holder::FUN_005c44a2()
{
	m04.Enter();
	int value = m340;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c44c2
int fn_005c43e2_Holder::FUN_005c44c2()
{
	m04.Enter();
	int value = m344;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c44e2
int fn_005c43e2_Holder::FUN_005c44e2(int param_1)
{
	switch (param_1)
	{
	case 0:
		return FUN_005c4482();
	case 1:
		return FUN_005c44a2();
	case 2:
		return FUN_005c44c2();
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x005c450c
float fn_005c43e2_Holder::FUN_005c450c()
{
	m04.Enter();
	float value = m348;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the optex sits at offset 4 and the float
// value read under the lock at offset 0x484.
class fn_005c4535_Holder
{
public:
	float FUN_005c4535();

	int		m00;
	tOptex	m04;
	char	pad[0x484 - 4 - sizeof(tOptex)];
	float	m484;
};

// FUNCTION: STARFLEETOP 0x005c4535
float fn_005c4535_Holder::FUN_005c4535()
{
	m04.Enter();
	float value = m484;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float value read under the lock sits at
// offset 0x488.
class fn_005c455e_Holder
{
public:
	float FUN_005c455e();

	int		m00;
	tOptex	m04;
	char	pad[0x488 - 4 - sizeof(tOptex)];
	float	m488;
};

// FUNCTION: STARFLEETOP 0x005c455e
float fn_005c455e_Holder::FUN_005c455e()
{
	m04.Enter();
	float value = m488;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2e0.
class fn_005c4587_Holder
{
public:
	int FUN_005c4587();

	int		m00;
	tOptex	m04;
	char	pad[0x2e0 - 4 - sizeof(tOptex)];
	int		m2e0;
};

// FUNCTION: STARFLEETOP 0x005c4587
int fn_005c4587_Holder::FUN_005c4587()
{
	m04.Enter();
	int value = m2e0;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2c8.
class fn_005c45a7_Holder
{
public:
	int FUN_005c45a7();

	int		m00;
	tOptex	m04;
	char	pad[0x2c8 - 4 - sizeof(tOptex)];
	int		m2c8;
};

// FUNCTION: STARFLEETOP 0x005c45a7
int fn_005c45a7_Holder::FUN_005c45a7()
{
	m04.Enter();
	int value = m2c8;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2cc.
class fn_005c45c7_Holder
{
public:
	int FUN_005c45c7();

	int		m00;
	tOptex	m04;
	char	pad[0x2cc - 4 - sizeof(tOptex)];
	int		m2cc;
};

// FUNCTION: STARFLEETOP 0x005c45c7
int fn_005c45c7_Holder::FUN_005c45c7()
{
	m04.Enter();
	int value = m2cc;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2d0.
class fn_005c45e7_Holder
{
public:
	int FUN_005c45e7();

	int		m00;
	tOptex	m04;
	char	pad[0x2d0 - 4 - sizeof(tOptex)];
	int		m2d0;
};

// FUNCTION: STARFLEETOP 0x005c45e7
int fn_005c45e7_Holder::FUN_005c45e7()
{
	m04.Enter();
	int value = m2d0;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2d4.
class fn_005c4607_Holder
{
public:
	int FUN_005c4607();

	int		m00;
	tOptex	m04;
	char	pad[0x2d4 - 4 - sizeof(tOptex)];
	int		m2d4;
};

// FUNCTION: STARFLEETOP 0x005c4607
int fn_005c4607_Holder::FUN_005c4607()
{
	m04.Enter();
	int value = m2d4;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x350.
class fn_005c4627_Holder
{
public:
	float FUN_005c4627();

	int		m00;
	tOptex	m04;
	char	pad[0x350 - 4 - sizeof(tOptex)];
	float	m350;
};

// FUNCTION: STARFLEETOP 0x005c4627
float fn_005c4627_Holder::FUN_005c4627()
{
	m04.Enter();
	float value = m350;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x354.
class fn_005c4650_Holder
{
public:
	float FUN_005c4650();

	int		m00;
	tOptex	m04;
	char	pad[0x354 - 4 - sizeof(tOptex)];
	float	m354;
};

// FUNCTION: STARFLEETOP 0x005c4650
float fn_005c4650_Holder::FUN_005c4650()
{
	m04.Enter();
	float value = m354;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x358.
class fn_005c4679_Holder
{
public:
	float FUN_005c4679();

	int		m00;
	tOptex	m04;
	char	pad[0x358 - 4 - sizeof(tOptex)];
	float	m358;
};

// FUNCTION: STARFLEETOP 0x005c4679
float fn_005c4679_Holder::FUN_005c4679()
{
	m04.Enter();
	float value = m358;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x35c.
class fn_005c46a2_Holder
{
public:
	float FUN_005c46a2();

	int		m00;
	tOptex	m04;
	char	pad[0x35c - 4 - sizeof(tOptex)];
	float	m35c;
};

// FUNCTION: STARFLEETOP 0x005c46a2
float fn_005c46a2_Holder::FUN_005c46a2()
{
	m04.Enter();
	float value = m35c;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x360.
class fn_005c46cb_Holder
{
public:
	float FUN_005c46cb();

	int		m00;
	tOptex	m04;
	char	pad[0x360 - 4 - sizeof(tOptex)];
	float	m360;
};

// FUNCTION: STARFLEETOP 0x005c46cb
float fn_005c46cb_Holder::FUN_005c46cb()
{
	m04.Enter();
	float value = m360;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x338.
class fn_005c46f4_Holder
{
public:
	float FUN_005c46f4();

	int		m00;
	tOptex	m04;
	char	pad[0x338 - 4 - sizeof(tOptex)];
	float	m338;
};

// FUNCTION: STARFLEETOP 0x005c46f4
float fn_005c46f4_Holder::FUN_005c46f4()
{
	m04.Enter();
	float value = m338;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x330.
class fn_005c471d_Holder
{
public:
	float FUN_005c471d();

	int		m00;
	tOptex	m04;
	char	pad[0x330 - 4 - sizeof(tOptex)];
	float	m330;
};

// FUNCTION: STARFLEETOP 0x005c471d
float fn_005c471d_Holder::FUN_005c471d()
{
	m04.Enter();
	float value = m330;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the float read under the lock sits at
// offset 0x334.
class fn_005c4746_Holder
{
public:
	float FUN_005c4746();

	int		m00;
	tOptex	m04;
	char	pad[0x334 - 4 - sizeof(tOptex)];
	float	m334;
};

// FUNCTION: STARFLEETOP 0x005c4746
float fn_005c4746_Holder::FUN_005c4746()
{
	m04.Enter();
	float value = m334;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2f4.
class fn_005c476f_Holder
{
public:
	int FUN_005c476f();

	int		m00;
	tOptex	m04;
	char	pad[0x2f4 - 4 - sizeof(tOptex)];
	int		m2f4;
};

// FUNCTION: STARFLEETOP 0x005c476f
int fn_005c476f_Holder::FUN_005c476f()
{
	m04.Enter();
	int value = m2f4;
	m04.Leave();
	return value;
}

// HYPOTHESIS: the owning class is not recovered; the value read under the lock sits at
// offset 0x2f8.
class fn_005c478f_Holder
{
public:
	int FUN_005c478f();

	int		m00;
	tOptex	m04;
	char	pad[0x2f8 - 4 - sizeof(tOptex)];
	int		m2f8;
	int		m2fc;
	int		m300;
	int		m304;
	int FUN_005c47af();
	float	m308;
	int FUN_005c47cf();
	float FUN_005c47ef();
	int* FUN_005c4818();
	int* FUN_005c4835();
	fn_005c478f_Holder* FUN_005c4852();
	fn_005c478f_Holder* FUN_005c486f();
	fn_005c478f_Holder* FUN_005c488c();
	fn_005c478f_Holder* FUN_005c48a9();
	fn_005c478f_Holder* FUN_005c48c6();
	fn_005c478f_Holder* FUN_005c48e6();
	fn_005c478f_Holder* FUN_005c4906();
	fn_005c478f_Holder* FUN_005c4926();
	fn_005c478f_Holder* FUN_005c4946();
	fn_005c478f_Holder* FUN_005c4966();
};

// FUNCTION: STARFLEETOP 0x005c478f
int fn_005c478f_Holder::FUN_005c478f()
{
	m04.Enter();
	int value = m2f8;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c47af
int fn_005c478f_Holder::FUN_005c47af()
{
	m04.Enter();
	int value = m300;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c47cf
int fn_005c478f_Holder::FUN_005c47cf()
{
	m04.Enter();
	int value = m304;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c47ef
float fn_005c478f_Holder::FUN_005c47ef()
{
	m04.Enter();
	float value = m308;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4818
int* fn_005c478f_Holder::FUN_005c4818()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x20, taken under the lock.
	int* value = (int*)this + 8;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4835
int* fn_005c478f_Holder::FUN_005c4835()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x30, taken under the lock.
	int* value = (int*)this;
	value += 12;
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4852
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c4852()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x40, taken under the lock.
	fn_005c478f_Holder* value = this;
	value = (fn_005c478f_Holder*)((char*)value + 0x40);
	m04.Leave();
	return value;
}

// FUNCTION: STARFLEETOP 0x005c486f
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c486f()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x50, taken under the lock.
	int value = (int)this;
	value += 0x50;
	m04.Leave();
	return (fn_005c478f_Holder*)value;
}

// FUNCTION: STARFLEETOP 0x005c488c
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c488c()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x60, taken under the lock.
	int value = (int)this;
	value += 0x60;
	m04.Leave();
	return (fn_005c478f_Holder*)value;
}

// FUNCTION: STARFLEETOP 0x005c48a9
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c48a9()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x70, taken under the lock.
	int value = (int)this;
	value += 0x70;
	m04.Leave();
	return (fn_005c478f_Holder*)value;
}

// FUNCTION: STARFLEETOP 0x005c48c6
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c48c6()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x80, taken under the lock.
	int* p = (int*)this;
	p += 0x20;
	m04.Leave();
	return (fn_005c478f_Holder*)p;
}

// FUNCTION: STARFLEETOP 0x005c48e6
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c48e6()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0x90, taken under the lock.
	fn_005c478f_Holder* value = this;
	value = (fn_005c478f_Holder*)((int)value + 0x90);
	m04.Leave();
	// MATCH-ONLY: two returns keep the sum in esi before Leave (add esi, 0x90); one return
	// rematerialises it as lea after the call. The original's real shape is not recovered.
	if (value)
		return value;
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4906
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c4906()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0xa0, taken under the lock.
	fn_005c478f_Holder* value = this;
	value = (fn_005c478f_Holder*)((int)value + 0xa0);
	m04.Leave();
	// MATCH-ONLY: same two-return shape as FUN_005c48e6 to keep the sum in esi across Leave.
	if (value)
		return value;
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4926
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c4926()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0xb0, taken under the lock.
	fn_005c478f_Holder* value = this;
	value = (fn_005c478f_Holder*)((int)value + 0xb0);
	m04.Leave();
	// MATCH-ONLY: same two-return shape as FUN_005c4906 to keep the sum in esi across Leave.
	if (value)
		return value;
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4946
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c4946()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0xc0, taken under the lock.
	fn_005c478f_Holder* value = this;
	value = (fn_005c478f_Holder*)((int)value + 0xc0);
	m04.Leave();
	// MATCH-ONLY: same two-return shape as FUN_005c4926 to keep the sum in esi across Leave.
	if (value)
		return value;
	return value;
}

// FUNCTION: STARFLEETOP 0x005c4966
fn_005c478f_Holder* fn_005c478f_Holder::FUN_005c4966()
{
	m04.Enter();
	// HYPOTHESIS: returns the address of the member at offset 0xd0, taken under the lock.
	fn_005c478f_Holder* value = this;
	value = (fn_005c478f_Holder*)((int)value + 0xd0);
	m04.Leave();
	// MATCH-ONLY: same two-return shape as FUN_005c4946 to keep the sum in esi across Leave.
	if (value)
		return value;
	return value;
}
