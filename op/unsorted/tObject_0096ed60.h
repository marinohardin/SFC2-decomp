//	tObject_0096ed60.h: the global object at 0x0096ed60.
// Evidence:  calls (original name and file unknown)

#ifndef tObject_0096ed60_h
#define tObject_0096ed60_h 1

#include "Common/BaseTypes.h"

class tString;

// HYPOTHESIS: the record 0x0052ccf9 returns; only the members 0x0060c1c7 reads are placed.
// m_1c is a kind (0, 1 and 2 are handled); m_3c0 is passed to slot 49 of the object at
// 0x0092deb4 as text.
class C_0052ccf9
{
public:
	char	m_00[0x1c];
	int		m_1c;
	char	m_20[0xfc - 0x20];
	int		m_fc;
	int		m_100;
	char	m_104[0x108 - 0x104];
	int		m_108;
	char	m_10c[0x3c0 - 0x10c];
	char	m_3c0[4];
};

// HYPOTHESIS: the object at 0x0096ed60; name unknown.
class tObject_0096ed60
{
public:
	// HYPOTHESIS: takes a text and a flag; tEntityMgr passes the value on to 0x0092deb4's slot 74.
	int32 fn_0052ca48(char *aText, int32 aFlag);
	// HYPOTHESIS: 0x0052ca48 returns with ret 4, so it takes the text alone, as 0x0060cb1b
	// calls it; the form above, used by tEntityMgr, is likely wrong (its 1 may belong to the
	// outer call) and waits for that unit.
	int32 fn_0052ca48(char *aText);
	// HYPOTHESIS: takes a team and its race (0x0060cb1b); the result is not used there.
	void fn_0052c64e(int32 aTeam, int32 aRace);
	C_0052ccf9 *fn_0052ccf9();
	// HYPOTHESIS: the mission title; 0x0060c51d copies it into a tString.
	tString &fn_0052cd2f();
	// HYPOTHESIS: a count and an entry by index; 0x0060c1c7 walks indices count-1 down to 0.
	int fn_0052ce1a();
	int fn_0052ce21(int index);
	int fn_0052ce32();
	int fn_0052ce39();
	int fn_0052ce40();
	int fn_0052ce67(int entry);
};

extern tObject_0096ed60 gObject_0096ed60;

#endif // tObject_0096ed60_h
