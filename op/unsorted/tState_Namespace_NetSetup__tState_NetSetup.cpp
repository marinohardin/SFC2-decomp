//	tState_NetSetup: a game state of the network setup screens.
// Evidence:  class (RTTI; original file unknown)

#include "Tiger/tstring.h"
#include "Tiger/tstringtag.h"
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tValueI.h"
#include "unsorted/tGameState.h"
#include "unsorted/c_0046aa10_msg.h"
#include <vector>
#include "unsorted/tNetControl.h"
#include "unsorted/tApplication.h"
#include <string.h>
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/tEntity.h"

// HYPOTHESIS: cdecl functions shared with tState_GameConnections; their names are not known.
void fn_004be81f(int arg);
void fn_004d31ed(const char *name, int arg, void *p, int width, int height);
void fn_004d3774(int id);
void fn_004d32c4(const char *name);
bool fn_0046a9e2();
// HYPOTHESIS: enables (1) or disables (0) the control with the given id.
void fn_004d34c5(int id, bool enable);

// HYPOTHESIS: the name of the screen the state shows, a global handed with it to fn_004d31ed,
// and a value the state resets to 10 (as tState_GameConnections resets its own).
extern char *g_00849398;
extern char g_008494b8[];
extern int g_008493d4;
// HYPOTHESIS: pointers to the names "IPSetup" and "ipEntryField".
extern const char *g_008493b0;
extern const char *g_008493b4;
// HYPOTHESIS: pointers to the names "ModemSetup", "modemEntry" and "cCancelGame".
extern const char *g_008493b8;
extern const char *g_008493bc;
extern const char *g_008493cc;
// HYPOTHESIS: a separator character read by fn_004768a5.
extern char g_008493d0;
void fn_004d346f(const char *name, const char *group);
void fn_004d3331(const char *name);
void fn_004d3340(const char *name);
// HYPOTHESIS: returns the control with the given id.
#include "unsorted/c_00468677_source.h"
c_00468677_source *fn_004d3446(int id);
// HYPOTHESIS: three global strings, 0x28 bytes apart, handed to tNetControl::fn_004327a7.
extern tString g_00910ac8;
extern tString g_00910aa0;
extern tString g_00910a78;

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

	void fn_004764bd();
	void fn_004764ed();
	bool fn_00476678();
	bool fn_004766f1();
	bool fn_00476754();
	void fn_004767f1();
	void fn_004768a5(c_00468677_source *source, tString &out);
	// HYPOTHESIS: puts the value into the control under the given key, as
	// tState_GameConnections::fn_004688ee does.
	void fn_00476997(int control, const char *name, const char *value);
	// HYPOTHESIS: stores an address setting under the given key; callee pops its arguments.
	void fn_00476aee(int value, const char *key);
	// HYPOTHESIS: fills the control kept at the first pointer from the second; callee pops.
	void fn_00476d52(int *control, void *source, int id);
	void fn_00477440();
	void fn_00477689();

	unsigned char m0c;
	unsigned char m0d;
	unsigned char m0e;
	unsigned char m0f;
	unsigned char m10;
	unsigned char m11;
	char m12[2];
	int m14;
	unsigned char m18;
	char m19[3];
	tValueI< tString > m1c;
	tValueI< tString > m5c;
	tValueI< tString > m9c;
	tValueI< tString > mdc;
	tValueI< tString > m11c;
	tValueI< tString > m15c;
	tValueI< tString > m19c;
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

// FUNCTION: STARFLEETOP 0x004764bd
void tState_NetSetup::fn_004764bd()
{
	switch (m1f0)
	{
	case 0x10004:
		fn_00476aee(m1dc, "IPAddress");
		break;
	case 0x10008:
		fn_00476aee(m1e4, "ModemAddress");
		break;
	}
}

// FUNCTION: STARFLEETOP 0x004764ed
void tState_NetSetup::fn_004764ed()
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
	fn_004d34c5(0x10008, 1);
	fn_004d34c5(0x10005, 1);
	fn_004d34c5(0x10004, 1);
	fn_004d34c5(0x10007, 1);

	for (c_0043257a_node *node = gNetControl.fn_0043257a(); node; node = node->next)
	{
		char *name = node->name;
		if (!strcmp(name, "IPX"))
		{
			fn_004d34c5(0x10006, 0);
			m0f = 1;
		}
		else if (!strcmp(name, "Modem"))
		{
			fn_004d34c5(0x10008, 0);
			m0d = 1;
		}
		else if (!strcmp(name, "Serial"))
		{
			fn_004d34c5(0x10005, 0);
			m0e = 1;
		}
		else if (!strcmp(name, "IP"))
		{
			fn_004d34c5(0x10004, 0);
			fn_004d34c5(0x10007, 0);
			m10 = 1;
			m11 = 1;
		}
		else if ((!strcmp(name, "MPlayer") && !m14) || (!strcmp(name, "DPSim") && m14))
		{
			fn_004d34c5(0x10007, 0);
			m11 = 1;
		}
	}
}

// FUNCTION: STARFLEETOP 0x00476678
bool tState_NetSetup::fn_00476678()
{
	tString value = m1c;
	gObject_0092deb4->vfunction4(value);
	bool result = gNetControl.fn_004325a9(1);
	gNetControl.fn_0043289d();
	return result;
}

// FUNCTION: STARFLEETOP 0x004766f1
bool tState_NetSetup::fn_004766f1()
{
	fn_004d3331(g_008493b0);
	fn_004d3340(g_008493b4);
	if (m1dc == 0)
		m1dc = (int)fn_004d3446(0x10012);
	fn_00476997(m1dc, "IPAddress", m208.mCharStr());
	return gNetControl.fn_004325a9(2);
}

// FUNCTION: STARFLEETOP 0x00476754
bool tState_NetSetup::fn_00476754()
{
	fn_004d3331(g_008493b8);
	fn_004d3340(g_008493bc);
	fn_004d346f(g_008493cc, g_008493b8);
	fn_004d34c5(0x10018, 0);
	bool result = gNetControl.fn_004325a9(3);
	fn_00476d52(&m1e0, &m11c, 0x10016);
	if (m1e4 == 0)
		m1e4 = (int)fn_004d3446(0x10017);
	fn_00476997(m1e4, "ModemAddress", 0);
	m19[0] = 0;
	m12[0] = 1;
	return result;
}

// FUNCTION: STARFLEETOP 0x004767f1
void tState_NetSetup::fn_004767f1()
{
	switch (m1f0)
	{
	case 0x10005:
		gNetControl.fn_004327a7(m15c.m18.mCharStr(), m19c.m18, g_00910ac8, g_00910aa0, g_00910a78);
		break;
	case 0x10008:
		fn_00476aee(m1e4, "ModemAddress");
		gNetControl.fn_004326aa(m11c.m18, 0);
		break;
	}
}

}	// namespace tState_Namespace_NetSetup

// HYPOTHESIS: a free function; it does not use this. tState_GameConnections calls it.
// The byte at +0x14 of the entity table is cleared; its member is not declared yet.
// FUNCTION: STARFLEETOP 0x0047685b
bool fn_0047685b()
{
	int result = gNetControl.fn_00431c7f();
	gObject_008e1060.mSetPlayerNumber(gNetControl.fn_00431c79());
	gObject_008e1060.fField00[0x14] = 0;
	int id = gObject_008e1060.mGetNextPlayerID();
	gObject_0092deb4->vfunction10(id, 0, 0);
	return result == 0 ? true : false;
}

// FUNCTION: STARFLEETOP 0x004768a5
void tState_Namespace_NetSetup::tState_NetSetup::fn_004768a5(c_00468677_source *source, tString &out)
{
	tString *text = source->vfunction68();
	if (text)
	{
		int len = text->mStrLen();
		int pos = text->find(g_008493d0);
		if (pos >= 0 && pos < len)
			out = text->substr(0, pos);
		else
			out = *text;
	}
}
