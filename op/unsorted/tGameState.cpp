// Unit:      unsorted/tGameState.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
#include "unsorted/tGameState.h"

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