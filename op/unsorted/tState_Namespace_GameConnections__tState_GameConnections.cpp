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
void fn_004d3331(const char *name);
// HYPOTHESIS: returns a handle for the control with the given id.
int fn_004d3446(int id);
// HYPOTHESIS: pointers to the names "OtherConnectionsTCPIP" and "ipEntryField".
extern const char *g_00844ea4;
extern const char *g_00844ea8;
bool fn_0046a9e2();
// HYPOTHESIS: enables (1) or disables (0) the control with the given id.
void fn_004d34c5(int id, bool enable);
// HYPOTHESIS: pointers to the names "OtherConnectionsModem", "modemEntry" and "cCancelGame".
extern const char *g_00844eac;
extern const char *g_00844eb0;
extern const char *g_00844ec0;
void fn_004d3340(const char *name);
// HYPOTHESIS: the second argument is the group's name, as here; cdecl.
void fn_004d346f(const char *name, const char *group);
// HYPOTHESIS: three global strings, 0x28 bytes apart, handed to tNetControl::fn_004327a7.
extern tString g_0090da38;
extern tString g_0090da10;
extern tString g_0090d9e8;

// HYPOTHESIS: a pointer to the name "OtherConnectionsSerialPort".
extern const char *g_00844eb4;
void fn_004d3564(const char *psz1, const char *psz2, int n);
// HYPOTHESIS: an entry of the port list fn_004329af returns; 0x0060f6d0 returns its name.
// tNetControl.cpp declares a class with the same getter only inside that unit.
class c_004329af_entry
{
public:
	const char *fn_0060f6d0();
	char m00[0xc];
};

// HYPOTHESIS: a separator character read by fn_00468677.
extern char g_00844ec4;

// HYPOTHESIS: an object whose slot 68 (+0x110) returns its text; the class is not known.
class c_00468677_source
{
public:
	virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2(); virtual void vfunction3();
	virtual void vfunction4(); virtual void vfunction5(); virtual void vfunction6(); virtual void vfunction7();
	virtual void vfunction8(); virtual void vfunction9(); virtual void vfunction10(); virtual void vfunction11();
	virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14(); virtual void vfunction15();
	virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19();
	virtual void vfunction20(); virtual void vfunction21(); virtual void vfunction22(); virtual void vfunction23();
	virtual void vfunction24(); virtual void vfunction25(); virtual void vfunction26(); virtual void vfunction27();
	virtual void vfunction28(); virtual void vfunction29(); virtual void vfunction30(); virtual void vfunction31();
	virtual void vfunction32(); virtual void vfunction33(); virtual void vfunction34(); virtual void vfunction35();
	virtual void vfunction36(); virtual void vfunction37(); virtual void vfunction38(); virtual void vfunction39();
	virtual void vfunction40(); virtual void vfunction41(); virtual void vfunction42(); virtual void vfunction43();
	virtual void vfunction44(); virtual void vfunction45(); virtual void vfunction46(); virtual void vfunction47();
	virtual void vfunction48(); virtual void vfunction49(); virtual void vfunction50(); virtual void vfunction51();
	virtual void vfunction52(); virtual void vfunction53(); virtual void vfunction54(); virtual void vfunction55();
	virtual void vfunction56(); virtual void vfunction57(); virtual void vfunction58(); virtual void vfunction59();
	virtual void vfunction60(); virtual void vfunction61(); virtual void vfunction62(); virtual void vfunction63();
	virtual void vfunction64(); virtual void vfunction65(); virtual void vfunction66(); virtual void vfunction67();
	virtual tString *vfunction68();
};

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
	bool fn_0046858a();
	void fn_0046860d();
	// HYPOTHESIS: copies the text after the separator g_00844ec4, or all of it; types are guesses.
	void fn_00468677(c_00468677_source *source, tString &out);
	void fn_004686f6();
	bool fn_004688a9();
	// HYPOTHESIS: a member taking the address field and its name; the types are guesses.
	void fn_00468a45(int value, const char *name);
	// HYPOTHESIS: hands the address field a named value; the types are guesses.
	void fn_004688ee(int control, const char *name, const char *value);

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
	unsigned char m19;
	unsigned char m1a;
	unsigned char m1b;
	tValueI< tString > m1c;
	tValueI< tString > m5c;
	// HYPOTHESIS: four more value objects, as the strings at +0x134 and +0x174 suggest.
	tValueI< tString > m9c;
	tValueI< tString > mdc;
	tValueI< tString > m11c;
	tValueI< tString > m15c;
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

// FUNCTION: STARFLEETOP 0x00468527
void tState_GameConnections::fn_00468527()
{
	fn_004d3331(g_00844ea4);
	fn_004d32c4(g_00844ea8);
	if (m1dc == 0)
		m1dc = fn_004d3446(0x10015);
	fn_004688ee(m1dc, "IPAddress", m1fc.mCharStr());
	gNetControl.fn_004325a9(2);
}

// FUNCTION: STARFLEETOP 0x0046858a
bool tState_GameConnections::fn_0046858a()
{
	fn_004d3331(g_00844eac);
	fn_004d3340(g_00844eb0);
	fn_004d346f(g_00844ec0, g_00844eac);
	fn_004d34c5(0x1001c, 0);
	bool result = gNetControl.fn_004325a9(3);
	if (m1e0 == 0)
		m1e0 = fn_004d3446(0x1001b);
	fn_004688ee(m1e0, "ModemAddress", 0);
	m19 = 0;
	m12[0] = 1;
	return result;
}

// FUNCTION: STARFLEETOP 0x0046860d
void tState_GameConnections::fn_0046860d()
{
	switch (m1e4)
	{
	case 0x10005:
		gNetControl.fn_004327a7(m15c.m18.mCharStr(), m19c.m18, g_0090da38, g_0090da10, g_0090d9e8);
		break;
	case 0x10009:
		fn_00468a45(m1e0, "ModemAddress");
		gNetControl.fn_004326aa(m11c.m18, 0);
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00468677
void tState_GameConnections::fn_00468677(c_00468677_source *source, tString &out)
{
	tString *text = source->vfunction68();
	if (text)
	{
		int len = text->mStrLen();
		int pos = text->find(g_00844ec4);
		if (pos >= 0 && pos < len)
			out = text->substr(pos + 1);
		else
			out = *text;
	}
}

// FUNCTION: STARFLEETOP 0x004686f6
void tState_GameConnections::fn_004686f6()
{
	bool free[6];
	free[0] = true;
	free[1] = true;
	free[2] = true;
	free[3] = true;
	free[4] = true;
	free[5] = true;
	char port[10] = "";

	std::vector< c_004329af_entry > *ports = gNetControl.fn_004329af();
	if (ports)
	{
		for (c_004329af_entry *entry = ports->begin(); entry != ports->end(); entry++)
		{
			for (int i = 0; i < 6; i++)
			{
				sprintf(port, "COM%d", i + 1);
				if (!strcmp(entry->fn_0060f6d0(), port))
				{
					free[i] = false;
					break;
				}
			}
		}

		int first = -1;
		for (int i = 0; i < 6; i++)
		{
			if (first < 0 && !free[i])
				first = i + 1;
			fn_004d34c5(0x10036 + i, free[i]);
		}

		if (first > -1)
		{
			fn_004d3564(g_00844eb4, "OtherConnectionsSerialRadio", first);
			sprintf(port, "COM%d", first);
			{
				tString text(port);
				m15c = text;
			}
			{
				tString text(g_00844e34[m224[0]]);
				m19c = text;
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x004688a9
bool tState_GameConnections::fn_004688a9()
{
	fn_004d3331(g_00844eb4);
	fn_004d346f(g_00844ec0, g_00844eb4);
	bool result = gNetControl.fn_004325a9(4);
	fn_004686f6();
	m1b = 0;
	m1a = 1;
	return result;
}

}
