// Unit:      unsorted/tGameState.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
#include "unsorted/tGameState.h"

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
