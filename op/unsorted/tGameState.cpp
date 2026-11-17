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