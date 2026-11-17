// Unit:      unsorted/tGameState.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
#include "unsorted/tGameState.h"
#include "unsorted/tApplication.h"

#include <string>

// HYPOTHESIS: names; the count of states made so far and the head of the list of all states.
int g_0090db20;
tGameState *g_0090db24;

// FUNCTION: STARFLEETOP 0x00469cd7
tGameState::tGameState()
{
	m_08 = g_0090db20++;
	m_04 = g_0090db24;
	g_0090db24 = this;
}

// FUNCTION: STARFLEETOP 0x00469cfd
// FUNCTION: STARFLEETOP 0x00469d19
// HYPOTHESIS: the body unlinks this state from the list headed at g_0090db24.
tGameState::~tGameState()
{
	tGameState *prev = 0;
	for (tGameState *cur = g_0090db24; cur; cur = cur->m_04)
	{
		if (cur == this)
		{
			if (prev)
				prev->m_04 = m_04;
			else
				g_0090db24 = m_04;
			return;
		}
		prev = cur;
	}
}

// HYPOTHESIS: a string held at 0x0090db10; this function returns a copy of it by value.
// FUNCTION: STARFLEETOP 0x00469db4
// FUNCTION: STARFLEETOP 0x00469d9c
std::string g_0090db10;

// FUNCTION: STARFLEETOP 0x00469d46
std::string fn_00469d46()
{
	return g_0090db10;
}

// FUNCTION: STARFLEETOP 0x00469d7b
void fn_00469d7b(const std::string& s)
{
	g_0090db10.assign(s, 0, std::string::npos);
}

// HYPOTHESIS: param_1 is an object of an unidentified class; only the two virtual slots called
// (0x68 and 0x74) are known, and the argument types are guesses.
class fn_00469ddd_target
{
public:
	virtual void v00();
	virtual void v01();
	virtual void v02();
	virtual void v03();
	virtual void v04();
	virtual void v05();
	virtual void v06();
	virtual void v07();
	virtual void v08();
	virtual void v09();
	virtual void v0a();
	virtual void v0b();
	virtual void v0c();
	virtual void v0d();
	virtual void v0e();
	virtual void v0f();
	virtual void v10();
	virtual void v11();
	virtual void v12();
	virtual void v13();
	virtual void v14();
	virtual void v15();
	virtual void v16();
	virtual void v17();
	virtual void v18();
	virtual void v19();
	virtual void vfunction26(int a, int b, int c);
	virtual void v1b();
	virtual void v1c();
	virtual void vfunction29(int a, int b);
};

// FUNCTION: STARFLEETOP 0x00469ddd
void tGameState::vfunction8(int *param_1)
{
	fn_00469ddd_target *target = (fn_00469ddd_target *)param_1;
	target->vfunction26(0, 0, 0);
	target->vfunction29(1, 1);
}

// FUNCTION: STARFLEETOP 0x00469dfd
bool tGameState::vfunction14(int param_1)
{
	return 1;
}

// FUNCTION: STARFLEETOP 0x00469e02
void tGameState::vfunction16(int *param_1)
{
	*param_1 = 0;
}

tGameState *fn_0046a59a();
bool fn_0046a62d(int param_1);
bool fn_0046a62d(const char *param_1);

// FUNCTION: STARFLEETOP 0x00469e0c
bool tGameState::vfunction17(int param_1)
{
	if (fn_0046a59a() != this)
		return 0;
	return fn_0046a62d(param_1);
}

// HYPOTHESIS: fn_0046995a is a method of an unidentified class; this passes a global object of it as this.
class fn_0046995a_owner
{
public:
	void fn_0046995a();
};

extern fn_0046995a_owner g_fn_0046995a_owner;

// FUNCTION: STARFLEETOP 0x00469e2a
void tGameState::vfunction18()
{
	g_fn_0046995a_owner.fn_0046995a();
}

// HYPOTHESIS: fn_0046995e is the next method of the same unidentified class.
class fn_0046995e_owner
{
public:
	void fn_0046995e();
};

extern fn_0046995e_owner g_fn_0046995e_owner;

// FUNCTION: STARFLEETOP 0x00469e34
void fn_00469e34()
{
	g_fn_0046995e_owner.fn_0046995e();
}

// HYPOTHESIS: fn_00469979 is a method of another unidentified class.
class fn_00469979_owner
{
public:
	void fn_00469979(int param_1);
};

extern fn_00469979_owner g_fn_00469979_owner;

// FUNCTION: STARFLEETOP 0x00469e3e
void fn_00469e3e(int param_1)
{
	g_fn_00469979_owner.fn_00469979(param_1);
}

// HYPOTHESIS: the globals at 0x0090db88..0x0090db9c are four values and a flag; types guessed.
extern int g_0090db88;
extern int g_0090db8c;
extern int g_0090db90;
extern int g_0090db94;
extern char g_0090db9c;

void fn_0069ab80(int *a, int *b, int c, int d);

// FUNCTION: STARFLEETOP 0x00469e4d
void fn_00469e4d()
{
	if (g_0090db9c == 0)
	{
		fn_0069ab80(&g_0090db88, &g_0090db8c, g_0090db90, g_0090db94);
	}
}

// HYPOTHESIS: the object returned by fn_0040c28c and the global at 0x0090db70 are of unidentified classes.
class fn_0040c28c_object
{
public:
	int fn_006fe8a0(int);
	int fn_0069aa00(int);
};

class fn_0090db70_owner
{
public:
	char fn_00672b60(fn_0040c28c_object *, int);
	void fn_006734e0(int, int, int, int);
};

fn_0040c28c_object *fn_0040c28c();
extern fn_0090db70_owner g_0090db70;
void fn_00464be8();
void fn_00469ab8(int *a);
extern int g_0090db48;

// FUNCTION: STARFLEETOP 0x00469e75
char fn_00469e75(int param_1, int param_2)
{
	fn_0040c28c_object *obj = fn_0040c28c();
	int a = obj->fn_006fe8a0(1);
	int b = obj->fn_0069aa00(a);
	if (g_0090db70.fn_00672b60(obj, b) == 0)
		return 0;
	g_0090db9c = 0;
	g_0090db90 = param_1;
	g_0090db94 = param_2;
	fn_00464be8();
	fn_00469ab8(&g_0090db48);
	fn_0046a62d("startup");
	return 1;
}

// HYPOTHESIS: g_0090db44 holds a pointer to an object released through fn_0046a688.
extern int g_0090db44;
void fn_0046a688(int param_1);

// HYPOTHESIS: fn_00672f40 is a method of an unidentified class, called on a global object (this = its address).
class fn_00672f40_owner
{
public:
	void fn_00672f40();
};

extern fn_00672f40_owner g_fn_00672f40_owner;

// FUNCTION: STARFLEETOP 0x00469ed8
void fn_00469ed8()
{
	if (g_0090db44 != 0)
	{
		fn_0046a688(g_0090db44);
		g_0090db44 = 0;
	}
	g_fn_00672f40_owner.fn_00672f40();
}

// HYPOTHESIS: methods of the unidentified classes of the globals at 0x0090db70 and 0x0090dbb0,
// the object held in g_0090db44 (only its slot at 0x38 is known), and the flag at 0x0090e004.
class fn_006735e0_owner
{
public:
	void fn_006735e0(unsigned int param_1);
};

class fn_00469c0a_owner
{
public:
	void fn_00469c0a(int *ev);
};

class fn_0090db44_object
{
public:
	virtual void v00();
	virtual void v01();
	virtual void v02();
	virtual void v03();
	virtual void v04();
	virtual void v05();
	virtual void v06();
	virtual void v07();
	virtual void v08();
	virtual void v09();
	virtual void v0a();
	virtual void v0b();
	virtual void v0c();
	virtual void v0d();
	virtual void vfunction15(int *ev);
	virtual void vfunction16(int *ev);
	virtual void vfunction17(int *ev);
};

extern fn_006735e0_owner g_fn_006735e0_owner;
extern fn_00469c0a_owner g_fn_00469c0a_owner;
extern char g_0090e004;
bool fn_0069ac10();
bool fn_0069ac30();

// FUNCTION: STARFLEETOP 0x00469ef9
void fn_00469ef9(unsigned int param_1)
{
	if (g_0090db9c != 0)
		return;

	unsigned int key = param_1;
	bool special = false;
	switch (key)
	{
	case 8:
		key = 0x100;
		break;
	case 9:
		key = 0x101;
		break;
	case 0xd:
		key = 0x102;
		break;
	case 0x1b:
		key = 0x103;
		break;
	case 0x20:
		key = 0x104;
		break;
	case 0x100:
		key = 0x105;
		special = true;
		break;
	case 0x101:
		key = 0x106;
		special = true;
		break;
	case 0x102:
		key = 0x107;
		special = true;
		break;
	case 0x103:
		key = 0x108;
		special = true;
		break;
	case 0x104:
		key = 0x109;
		special = true;
		break;
	case 0x105:
		key = 0x10a;
		special = true;
		break;
	case 0x106:
		key = 0x10b;
		special = true;
		break;
	case 0x107:
		key = 0x10c;
		special = true;
		break;
	case 0x108:
		key = 0x10d;
		special = true;
		break;
	case 0x109:
		key = 0x10e;
		special = true;
		break;
	case 0x10a:
		key = 0x10f;
		special = true;
		break;
	case 0x10b:
		key = 0x110;
		special = true;
		break;
	case 0x10c:
		key = 0x111;
		special = true;
		break;
	case 0x10d:
		key = 0x112;
		special = true;
		break;
	case 0x10e:
		key = 0x113;
		special = true;
		break;
	case 0x10f:
		key = 0x114;
		special = true;
		break;
	case 0x110:
		key = 0x115;
		special = true;
		break;
	case 0x111:
		key = 0x116;
		special = true;
		break;
	case 0x112:
		key = 0x117;
		special = true;
		break;
	case 0x113:
		key = 0x118;
		special = true;
		break;
	case 0x114:
		key = 0x119;
		special = true;
		break;
	case 0x115:
		key = 0x11a;
		special = true;
		break;
	case 0x116:
		key = 0x11b;
		break;
	case 0x117:
		key = 0x11c;
		break;
	case 0x118:
		key = 0x11d;
		break;
	case 0x119:
		key = 0x11e;
		break;
	case 0x11a:
		key = 0x11f;
		break;
	case 0x11b:
		key = 0x120;
		break;
	case 0x11c:
		key = 0x121;
		break;
	case 0x11d:
		key = 0x122;
		break;
	case 0x11e:
		key = 0x123;
		break;
	case 0x11f:
		key = 0x124;
		break;
	case 0x120:
		key = 0x125;
		break;
	case 0x121:
		key = 0x126;
		break;
	case 0x122:
		key = 0x127;
		break;
	case 0x123:
		key = 0x128;
		break;
	case 0x124:
		key = 0x129;
		break;
	case 0x125:
		key = 0x12a;
		break;
	case 0x126:
		key = 0x12b;
		break;
	case 0x127:
		key = 0x12c;
		break;
	case 0x128:
		key = 0x12d;
		break;
	case 0x129:
		key = 0x12e;
		break;
	case 0x12a:
		key = 0x12f;
		break;
	case 0x12b:
		key = 0x130;
		break;
	}
	if (special)
		g_fn_006735e0_owner.fn_006735e0(param_1);

	int flags = 0;
	if (fn_0069ac10())
		flags = 1;
	if (fn_0069ac30())
		flags |= 2;
	if (param_1 == 8 && fn_0069ac10() && fn_0069ac30())
		g_0090e004 = g_0090e004 == 0;

	// HYPOTHESIS: an eight-int event record; slot 0 the type, 1 the key, 5 the modifier flags,
	// 6 and 7 the values fn_0069ab80 fills.
	int ev[8];
	ev[0] = 0x100;
	ev[1] = key;
	ev[5] = flags;
	ev[6] = 0;
	ev[7] = 0;
	fn_0069ab80(&ev[6], &ev[7], g_0090db90, g_0090db94);
	if (g_0090db44 != 0)
	{
		((fn_0090db44_object *)g_0090db44)->vfunction15(ev);
		if (ev[0] == 0)
			return;
	}
	g_fn_00469c0a_owner.fn_00469c0a(ev);
}

// FUNCTION: STARFLEETOP 0x0046a36d
void fn_0046a36d(int param_1, int param_2, int param_3, int param_4)
{
	if (g_0090db9c != 0)
		return;

	g_0090db70.fn_006734e0(param_1, param_2, param_3, param_4);

	int type;
	switch (param_4)
	{
	case 1:
		type = 1;
		break;
	case 2:
		type = 2;
		break;
	case 3:
		type = 3;
		break;
	case 5:
		type = 4;
		break;
	case 6:
		type = 5;
		break;
	case 8:
		type = 6;
		break;
	case 9:
		type = 7;
		break;
	default:
		return;
	}

	// HYPOTHESIS: same eight-int event record as fn_00469ef9.
	int ev[8];
	unsigned int flags = 0;
	if (fn_0069ac10())
		flags = 1;
	if (fn_0069ac30())
		flags |= 2;
	ev[1] = -1;
	ev[5] = flags;
	ev[0] = type;
	ev[6] = param_1;
	ev[7] = param_2;
	if (g_0090db44 != 0)
	{
		((fn_0090db44_object *)g_0090db44)->vfunction17(ev);
		if (ev[0] == 0)
			return;
	}
	g_fn_00469c0a_owner.fn_00469c0a(ev);
}

// FUNCTION: STARFLEETOP 0x0046a2d4
void fn_0046a2d4(unsigned int param_1)
{
	if (g_0090db9c != 0)
		return;

	g_fn_006735e0_owner.fn_006735e0(param_1);

	int flags = 0;
	if (fn_0069ac10())
		flags = 1;
	if (fn_0069ac30())
		flags |= 2;

	// HYPOTHESIS: same eight-int event record as fn_00469ef9, type 0x101.
	int ev[8];
	ev[0] = 0x101;
	ev[1] = param_1;
	ev[5] = flags;
	ev[6] = 0;
	ev[7] = 0;
	fn_0069ab80(&ev[6], &ev[7], g_0090db90, g_0090db94);
	if (g_0090db44 != 0)
	{
		((fn_0090db44_object *)g_0090db44)->vfunction16(ev);
		if (ev[0] == 0)
			return;
	}
	g_fn_00469c0a_owner.fn_00469c0a(ev);
}

// HYPOTHESIS: the globals at 0x0090dba4 and 0x0090dba8 are a pending flag and a mode value; types guessed.
extern bool g_0090dba4;
extern int g_0090dba8;

void fn_0046a46a(tGameState *param_1);

// FUNCTION: STARFLEETOP 0x0046a428
void fn_0046a428()
{
	if (g_0090db9c != 0)
		return;

	if (g_0090dba4)
	{
		g_0090dba4 = false;
		if (g_0090dba8 == 0)
			g_fn_00469979_owner.fn_00469979(1);
		else
			g_fn_00469979_owner.fn_00469979(0);
	}
	tGameState *state = fn_0046a59a();
	if (state != 0)
		fn_0046a46a(state);
}

// HYPOTHESIS: the object returned by 0x004bcfd3 is unnamed; 0x004bd088 is a method on it.
class tUnnamed_004bcfd3
{
public:
	void fn_004bd088();
};

tUnnamed_004bcfd3 *fn_004bcfd3();

// FUNCTION: STARFLEETOP 0x0046a46a
void fn_0046a46a(tGameState *param_1)
{
	try
	{
		fn_004bcfd3()->fn_004bd088();
		param_1->vfunction2();
	}
	catch (...)
	{
	}
}

// HYPOTHESIS: the argument is a tView3D (the caller passes one); slot 4 of the state is unnamed.
// FUNCTION: STARFLEETOP 0x0046a4a8
void fn_0046a4a8(tView3D *param_1)
{
	tGameState *state = fn_0046a59a();
	if (state != 0)
		state->vfunction4(param_1);
}

// HYPOTHESIS: slot 5 of the state takes one argument here; the header declares it without one.
class fn_0046a4bd_state
{
public:
	virtual void v00();
	virtual void v01();
	virtual void v02();
	virtual void v03();
	virtual void v04();
	virtual void vfunction5(int param_1);
};

// HYPOTHESIS: 0x00672f80 is a method of the class of the global at 0x0090db70.
class fn_0090db70_owner2
{
public:
	void fn_00672f80(int param_1);
};

extern fn_0090db70_owner2 g_0090db70_2;
extern unsigned int g_0090dba0;
// HYPOTHESIS: the key string at 0x008458f4 (its text is not recovered; it is shared with other uses).
extern char g_sz008458f4[];
void fn_0046aa9a(int param_1);

// FUNCTION: STARFLEETOP 0x0046a4bd
void fn_0046a4bd(int param_1)
{
	tGameState *state = fn_0046a59a();
	if (state != 0)
		((fn_0046a4bd_state *)state)->vfunction5(param_1);
	if (g_0090e004 == 0)
		g_0090db70_2.fn_00672f80(param_1);
	if ((int)g_0090dba0 < 0)
		g_0090dba0 = g_p008cc580->vfunction5("Debug", g_sz008458f4, 0) != 0;
	if (g_0090dba0 != 0)
		fn_0046aa9a(param_1);
}

// HYPOTHESIS: slot 6 of the state takes one argument here; the header declares it without one.
class fn_0046a51f_state
{
public:
	virtual void v00();
	virtual void v01();
	virtual void v02();
	virtual void v03();
	virtual void v04();
	virtual void v05();
	virtual void vfunction6(int param_1);
};

// FUNCTION: STARFLEETOP 0x0046a51f
void fn_0046a51f(int param_1)
{
	tGameState *state = fn_0046a59a();
	if (state != 0)
		((fn_0046a51f_state *)state)->vfunction6(param_1);
}