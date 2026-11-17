//	tObject_0096ed60.h: the global object at 0x0096ed60.
// Evidence:  calls (original name and file unknown)

#ifndef tObject_0096ed60_h
#define tObject_0096ed60_h 1

#include "Common/BaseTypes.h"

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
	C_0052ccf9 *fn_0052ccf9();
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
