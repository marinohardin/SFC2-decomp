// Functions of tState_Namespace_GameConnections::tState_GameConnections (RTTI); the
// original file is not known.

#include "Tiger/tstring.h"
#include "Tiger/tstringtag.h"
#include "unsorted/tNetControl.h"
#include "unsorted/tApplication.h"
#include <string.h>
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tValueI.h"
#include "unsorted/c_0046aa10_msg.h"
#include <vector>
#include "unsorted/tObject_0092deb4.h"

// The state's name: a pointer to the string "GameConnections" (exe 0x00844e78).
extern char *g_00844e78;

// HYPOTHESIS: tables of names the state hands to fn_004d3179; their contents are not known.
extern const char *g_00844e34[];
extern const char *g_00844e4c[];
extern const char *g_00844e64[];
extern const char *g_00844e70;
extern int g_00844ec8;

// HYPOTHESIS: a global whose address is passed to fn_004d31ed; its type is not known.
extern char g_00845040[];

// HYPOTHESIS: cdecl functions; their names are not known.
void fn_004be81f(int arg);
void fn_004d31ed(const char *name, int arg, void *p, int width, int height);
void fn_004d3179(const char *name, const tString &value);
void fn_004d3774(int id);
void fn_004d32c4(const char *name);
bool fn_0046a9e2();
// HYPOTHESIS: enables (1) or disables (0) the control with the given id.
void fn_004d34c5(int id, int enable);

namespace tState_Namespace_GameConnections
{

// HYPOTHESIS: only the slots rebuilt so far; members placed by their offsets.
class tState_GameConnections
{
public:
	char *vfunction13();
	bool vfunction2();
	bool vfunction4();
	void vfunction15(c_0046aa10_message *msg);

	void fn_004682c7();
	void fn_00468527();
	void fn_00469271();
	void fn_00469534();
	void fn_00468297();
	bool fn_004683ff();
	bool fn_00468481();
	// HYPOTHESIS: a member taking the address field and its name; the types are guesses.
	void fn_00468a45(int value, const char *name);

	char m00[0xc];
	unsigned char m0c;
	unsigned char m0d;
	unsigned char m0e;
	unsigned char m0f;
	unsigned char m10;
	unsigned char m11;
	char m12[2];
	int m14;
	unsigned char m18;
	char m19[0x1c - 0x19];
	tValueI< tString > m1c;
	tValueI< tString > m5c;
	char m9c[0x19c - 0x9c];
	tValueI< tString > m19c;
	int m1dc;
	int m1e0;
	int m1e4;
	char *m1e8;
	// HYPOTHESIS: names the state hands to fn_004d32c4 on exit; the element type is a guess.
	std::vector< char * > m1ec;
	tString m1fc;
	int m224[6];
	int m23c;
};

// FUNCTION: STARFLEETOP 0x00466674
char *tState_GameConnections::vfunction13()
{
	return g_00844e78;
}

// FUNCTION: STARFLEETOP 0x0046667a
bool tState_GameConnections::vfunction2()
{
	{
		c_0046aa10_message msg;
		while (fn_0046aa10(&msg))
			;
	}

	static tObject_0040ce21 marker("who wants play");

	fn_004be81f(0);
	m18 = 0;
	fn_00469271();
	m1e8 = g_00844e78;
	fn_004d31ed(g_00844e78, 1, g_00845040, 640, 480);

	tStringGroupTag *tag = new tStringGroupTag("trnasEntry");
	m5c = tag->mGetString(0);
	// HYPOTHESIS: the original tests the pointer before both deletes; an explicit test, as a
	// destructor on tStringGroupTag would change tIntTag::mGetString's frame.
	if (tag)
		delete tag;

	tStringGroupTag *tag2 = new tStringGroupTag("LAN");
	m1fc = tag2->mGetString(0);
	if (tag2)
		delete tag2;

	m14 = -1;
	fn_004682c7();
	g_00844ec8 = 10;
	m1e4 = -1;
	m0c = 0;
	m1dc = 0;
	m1e0 = 0;

	for (unsigned int i = 0; i < 6; i++)
	{
		tString text(g_00844e34[m224[i]]);
		fn_004d3179(g_00844e4c[i], text);
	}

	{
		tString text(g_00844e34[0]);
		m19c = text;
	}
	{
		tString text(g_00844e64[m23c]);
		fn_004d3179(g_00844e70, text);
	}

	fn_004d3774(0x10032);

	if (gNetControl.fn_0043456f() || gNetControl.fn_0043457d() != "")
	{
		c_0046aa10_message send;
		send.id = 0x10004;
		fn_0046aa2b(&send);
	}

	fn_00468527();
	m1e4 = 0x10004;
	return true;
}

// FUNCTION: STARFLEETOP 0x0046691b
bool tState_GameConnections::vfunction4()
{
	static tObject_0040ce21 marker("bye now");

	fn_00469534();
	fn_004d32c4(m1e8);
	while (!m1ec.empty())
	{
		fn_004d32c4(*(m1ec.end() - 1));
		m1ec.pop_back();
	}
	m1ec.erase(m1ec.begin(), m1ec.end());
	m1dc = 0;
	m1e0 = 0;
	return true;
}

// FUNCTION: STARFLEETOP 0x0046822d
void tState_GameConnections::vfunction15(c_0046aa10_message *msg)
{
	switch (msg->m_04)
	{
	case 'C':
	case 'c':
		if (fn_0046a9e2())
			msg->id = 0x10003;
		break;
	case 'V':
	case 'v':
		if (fn_0046a9e2())
			msg->id = 0x10002;
		break;
	case 0x101:
		msg->id = 0x10035;
		break;
	case 0x102:
		fn_004d3774(0x10032);
		break;
	case 0x103:
		msg->id = 0x102;
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00468297
void tState_GameConnections::fn_00468297()
{
	switch (m1e4)
	{
	case 0x10004:
		fn_00468a45(m1dc, "IPAddress");
		break;
	case 0x10009:
		fn_00468a45(m1e0, "ModemAddress");
		break;
	}
}

// FUNCTION: STARFLEETOP 0x004682c7
void tState_GameConnections::fn_004682c7()
{
	if (m14 < 0)
		m14 = g_p008cc580->vfunction7("Network", "SimProtocol", 0);

	gNetControl.fn_00431917();
	m0d = 0;
	m0e = 0;
	m0f = 0;
	m10 = 0;
	m11 = 0;
	fn_004d34c5(0x10006, 1);
	fn_004d34c5(0x10007, 1);
	fn_004d34c5(0x10004, 1);
	fn_004d34c5(0x10008, 1);

	for (c_0043257a_node *node = gNetControl.fn_0043257a(); node; node = node->next)
	{
		char *name = node->name;
		if (!strcmp(name, "IPX"))
		{
			fn_004d34c5(0x10006, 0);
			m0f = 1;
		}
		else if (!strcmp(name, "IP"))
		{
			fn_004d34c5(0x10007, 0);
			fn_004d34c5(0x10004, 0);
			fn_004d34c5(0x10008, 0);
			m10 = 1;
			m11 = 1;
		}
		else if ((!strcmp(name, "MPlayer") && !m14) || (!strcmp(name, "DPSim") && m14))
		{
			fn_004d34c5(0x10008, 0);
			m11 = 1;
		}
	}
}

// FUNCTION: STARFLEETOP 0x004683ff
bool tState_GameConnections::fn_004683ff()
{
	tString value = m1c;
	gObject_0092deb4->vfunction4(value);
	bool result = gNetControl.fn_004325a9(1);
	gNetControl.fn_0043258a(1);
	gNetControl.fn_0043289d();
	return result;
}

// FUNCTION: STARFLEETOP 0x00468481
bool tState_GameConnections::fn_00468481()
{
	tString value = m1c;
	gObject_0092deb4->vfunction4(value);
	bool result = gNetControl.fn_004325a9(2);
	gNetControl.fn_0043258a(1);
	gNetControl.fn_0043290e(tString(""));
	return result;
}

}
