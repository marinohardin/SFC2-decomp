//	tState_NetSetup: a game state of the network setup screens.
// Evidence:  class (RTTI; original file unknown)

#include "Tiger/tstring.h"
#include "Tiger/tstringtag.h"
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tValueI.h"
#include "unsorted/tGameState.h"
#include "unsorted/c_0046aa10_msg.h"
#include <vector>

// HYPOTHESIS: cdecl functions shared with tState_GameConnections; their names are not known.
void fn_004be81f(int arg);
void fn_004d31ed(const char *name, int arg, void *p, int width, int height);
void fn_004d3774(int id);
void fn_004d32c4(const char *name);
bool fn_0046a9e2();

// HYPOTHESIS: the name of the screen the state shows, a global handed with it to fn_004d31ed,
// and a value the state resets to 10 (as tState_GameConnections resets its own).
extern char *g_00849398;
extern char g_008494b8[];
extern int g_008493d4;

namespace tState_Namespace_NetSetup
{

// HYPOTHESIS: derives from tGameState like tState_GameConnections; only the slots rebuilt so
// far are declared, members placed by their offsets.
class tState_NetSetup : public tGameState
{
public:
	virtual void vfunction1();
	virtual bool vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	// HYPOTHESIS: returns the state's name.
	virtual char *vfunction13();
	virtual void vfunction14();
	virtual void vfunction15(c_0046aa10_message *msg);

	void fn_004764ed();
	void fn_00477440();
	void fn_00477689();

	unsigned char m0c;
	char m0d[7];
	int m14;
	unsigned char m18;
	char m19[0x43];
	tValueI< tString > m5c;
	char m9c[0x140];
	int m1dc;
	int m1e0;
	int m1e4;
	char m1e8[8];
	int m1f0;
	char *m1f4;
	// HYPOTHESIS: names the state hands to fn_004d32c4 on exit, as in tState_GameConnections.
	std::vector< char * > m1f8;
	tString m208;
};

// FUNCTION: STARFLEETOP 0x00475095
char *tState_NetSetup::vfunction13()
{
	return "netsetup";
}

// FUNCTION: STARFLEETOP 0x0047509b
bool tState_NetSetup::vfunction2()
{
	static tObject_0040ce21 marker("who wants play");

	fn_004be81f(0);
	m18 = 0;
	fn_00477440();
	m1f4 = g_00849398;
	fn_004d31ed(g_00849398, 1, g_008494b8, 640, 480);

	tStringGroupTag *tag = new tStringGroupTag("trnasEntry");
	m5c = tag->mGetString(0);
	if (tag)
		delete tag;

	tStringGroupTag *tag2 = new tStringGroupTag("LAN");
	m208 = tag2->mGetString(0);
	if (tag2)
		delete tag2;

	m14 = -1;
	fn_004764ed();
	g_008493d4 = 10;
	m1f0 = -1;
	m0c = 0;
	m1dc = 0;
	m1e0 = 0;
	m1e4 = 0;

	fn_004d3774(0x10020);
	return true;
}

// FUNCTION: STARFLEETOP 0x00475201
bool tState_NetSetup::vfunction4()
{
	static tObject_0040ce21 marker("bye now");

	fn_00477689();
	fn_004d32c4(m1f4);
	while (!m1f8.empty())
	{
		fn_004d32c4(*(m1f8.end() - 1));
		m1f8.pop_back();
	}
	m1f8.erase(m1f8.begin(), m1f8.end());
	m1dc = 0;
	m1e0 = 0;
	m1e4 = 0;
	return true;
}

// FUNCTION: STARFLEETOP 0x00476453
void tState_NetSetup::vfunction15(c_0046aa10_message *msg)
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
		msg->id = 0x10023;
		break;
	case 0x102:
		fn_004d3774(0x10020);
		break;
	case 0x103:
		msg->id = 0x102;
		break;
	}
}

}	// namespace tState_Namespace_NetSetup
