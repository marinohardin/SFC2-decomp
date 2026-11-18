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
#include <string>
#include "unsorted/tObject_0092deb4.h"
#include "Tiger/tFormatStr.h"
#include "unsorted/tGameState.h"

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
#include "unsorted/fn_004D3143.h"







// HYPOTHESIS: pointers to the names "OtherConnectionsTCPIP" and "ipEntryField".
extern const char *g_00844ea4;
extern const char *g_00844ea8;
bool fn_0046a9e2();


// HYPOTHESIS: pointers to the names "OtherConnectionsModem", "modemEntry" and "cCancelGame".
extern const char *g_00844eac;
extern const char *g_00844eb0;
extern const char *g_00844ec0;



// HYPOTHESIS: three global strings, 0x28 bytes apart, handed to tNetControl::fn_004327a7.
extern tString g_0090da38;
extern tString g_0090da10;
extern tString g_0090d9e8;

// HYPOTHESIS: a pointer to the name "OtherConnectionsSerialPort".
extern const char *g_00844eb4;
// HYPOTHESIS: a pointer to the name "GameConnectionsSelect".
extern const char *g_00844ea0;
void fn_004d3564(const char *psz1, const char *psz2, int n);
// c_004329af_entry is declared in unsorted/tNetControl.h.








// HYPOTHESIS: pointers to the names "cCreateGame", "cJoinGame" and "multiplayer".
extern const char *g_00844eb8;
extern const char *g_00844ebc;
extern const char *g_00844e90;
// HYPOTHESIS: cdecl; the meaning of the arguments is not known.
void fn_004d34e8(const char *name, const char *group, int n);
// HYPOTHESIS: the current time, and the modem state's flag and time of its last progress dot.
extern double g_00c7c148;
extern char g_0090daa8;
extern double g_0090dab0;

// HYPOTHESIS: pointers to state names handed to fn_00469e0c; cdecl helpers.
extern const char *g_00844e88;
extern const char *g_00844e9c;
void fn_0040fa40(const std::string &name, int n);
void fn_0046aa58();

// HYPOTHESIS: tables of control names fn_00469271 registers with fn_004d35e3; cdecl helpers
// whose meaning is not known. fn_004d35ac binds a value object to a control name.
#include "unsorted/s_00844ed0_entry.h"










extern s_00844ed0_entry g_00844ed0[];
extern s_00844fd0_entry g_00844fd0[];
extern const char *g_00845018[];
extern const char *g_0084501c;
extern const char *g_00845020;
extern const char *g_00845024;
extern const char *g_00845028;






// HYPOTHESIS: a separator character read by fn_00468677.
extern char g_00844ec4;

#include "unsorted/c_00468677_source.h"

namespace tState_Namespace_GameConnections
{

// HYPOTHESIS: only the slots rebuilt so far; members placed by their offsets.
class tState_GameConnections : public tGameState
{
public:
	tState_GameConnections();
	// MATCH-ONLY: the exe's destructor (0x004696e0) destroys the members and calls ~tGameState
	// without storing the vtable first, as only VC6's implicit destructor does; the original
	// declared none. It is written out so the address can be claimed; it adds that store.
	// Slot 0 holds the scalar deleting destructor (0x00469900).
	virtual ~tState_GameConnections();
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
	virtual char *vfunction13();
	virtual void vfunction14();
	virtual void vfunction15(c_0046aa10_message *msg);

	void fn_004682c7();
	bool fn_00468527();
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
	void fn_00468a45(c_00468677_source *control, const char *name);
	// HYPOTHESIS: fills the address field from the settings section name, or with value when
	// the section holds no entries.
	void fn_004688ee(c_00468677_source *control, const char *name, const char *value);
	// HYPOTHESIS: a member; it hides the four connection panels.
	void fn_00468bbb();
	// HYPOTHESIS: a member; it disables three controls.
	void fn_00468beb();
	// HYPOTHESIS: a member; it enables the controls fn_00468beb disables, the third only for
	// the control id 0x1001c.
	void fn_00468c13(int id);
	// HYPOTHESIS: a member; it polls the modem connection and shows its progress.
	void fn_00468c49();
	// HYPOTHESIS: a member; it stores the address field, then runs gNetControl.fn_00431821.
	void fn_00469096();
	bool fn_004690a5(int id);
	bool fn_0047685b();
	void fn_004695c4(const char *name, int n);

	void fn_00469636();

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
	c_00468677_source *m1dc;
	c_00468677_source *m1e0;
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
bool tState_GameConnections::fn_00468527()
{
	fn_004d3331(g_00844ea4);
	fn_004d32c4(g_00844ea8);
	if (m1dc == 0)
		m1dc = fn_004d3446(0x10015);
	fn_004688ee(m1dc, "IPAddress", m1fc.mCharStr());
	return gNetControl.fn_004325a9(2);
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

// FUNCTION: STARFLEETOP 0x004688ee
void tState_GameConnections::fn_004688ee(c_00468677_source *control, const char *name, const char *value)
{
	char buffer[49];
	tString prefix("Entry#");
	tString key;
	tString text;

	control->vfunction50();
	int count = g_p008cc580->vfunction5(name, "NumOfEntries", 0);
	if (count == 0 && value)
	{
		tString entry(value);
		control->vfunction54(entry, -1, 0, -1);
	}

	for (int i = 0; i < count; i++)
	{
		key = prefix;
		key.mAppendInt32s(i, 1);
		g_p008cc580->vfunction9(name, key.mCharStr(), "", buffer, sizeof(buffer));
		text = buffer;
		control->vfunction54(text, -1, 0, -1);
	}
}

// FUNCTION: STARFLEETOP 0x00468a45
void tState_GameConnections::fn_00468a45(c_00468677_source *control, const char *name)
{
	tString prefix("Entry#");
	tString key;
	tString text;

	int count = control->vfunction44();
	g_p008cc580->vfunction6(name, "NumOfEntries", count);
	int entry = 0;
	int selected = control->vfunction70();
	tString *current = control->vfunction68();

	for (int i = count - 1; i >= 0; i--)
	{
		if (i == selected)
			continue;
		key = prefix;
		key.mAppendInt32s(entry, 1);
		tString *item = control->vfunction56(i);
		text = *item;
		if (item)
			g_p008cc580->vfunction10(name, key.mCharStr(), text.mCharStr());
		entry++;
	}

	key = prefix;
	key.mAppendInt32s(entry, 1);
	if (current)
		g_p008cc580->vfunction10(name, key.mCharStr(), current->mCharStr());
}

// FUNCTION: STARFLEETOP 0x00468bbb
void tState_GameConnections::fn_00468bbb()
{
	fn_004d3340(g_00844ea0);
	fn_004d3340(g_00844ea4);
	fn_004d3340(g_00844eac);
	fn_004d3340(g_00844eb4);
}

// FUNCTION: STARFLEETOP 0x00468beb
void tState_GameConnections::fn_00468beb()
{
	fn_004d34c5(0x10011, 0);
	fn_004d34c5(0x10024, 0);
	fn_004d34c5(0x10025, 0);
}

// FUNCTION: STARFLEETOP 0x00468c13
void tState_GameConnections::fn_00468c13(int id)
{
	fn_004d34c5(0x10024, 1);
	fn_004d34c5(0x10025, 1);
	if (id == 0x1001c)
		fn_004d34c5(0x10011, 1);
}

// FUNCTION: STARFLEETOP 0x00468c49
void tState_GameConnections::fn_00468c49()
{
	if (g_0090dab0 == 0.0)
		g_0090dab0 = g_00c7c148;

	if (m1e4 == 0x10009 && !gNetControl.fn_004317c3())
	{
		if (!m12[0])
		{
			tString address;
			fn_00468677(m1e0, address);
			tString number = m11c;
			m12[0] = gNetControl.fn_004326aa(number, &address);
		}

		tString status = m5c;
		tStringGroupTag *tag = new tStringGroupTag("ModemStatus");

		int event;
		while ((event = gNetControl.fn_00431e6e()) != 0)
		{
			switch (event)
			{
			case 1:
				status = tag->mGetString(3);
				m19 = 1;
				break;
			case 2:
				status += tag->mGetString(1);
				status += "\n";
				break;
			case 3:
			{
				tString address;
				fn_00468677(m1e0, address);
				tFormatStr format;
				format.mTag("phoneNum", address.mCharStr());
				tString text = tag->mGetString(0);
				tString result;
				format.mGenFormattedStr(result, text);
				result += "\n";
				status = result;
				break;
			}
			case 4:
				if (!g_0090daa8)
				{
					status += tag->mGetString(2);
					g_0090daa8 = 1;
				}
				else
				{
					double now = g_00c7c148;
					if (now > g_0090dab0 + 1000000.0)
					{
						status += ".";
						g_0090dab0 = g_00c7c148;
					}
				}
				break;
			case 5:
				status = tag->mGetString(4);
				m12[0] = 1;
				fn_004d34e8(g_00844eb8, g_00844eac, 0);
				fn_004d34e8(g_00844ebc, g_00844eac, 0);
				fn_004d346f(g_00844ec0, g_00844eac);
				break;
			}
		}

		if (tag)
			delete tag;

		m5c = status;

		if (m19 && g_00844ec8 >= 0)
		{
			g_0090daa8 = 0;
			g_0090dab0 = 0.0;
			if (!fn_0047685b())
			{
				if (g_00844ec8-- > 0)
					fn_004695c4("JoinStatus", 0);
				else
				{
					fn_004695c4("JoinStatus", 3);
					fn_004d34e8(g_00844eb8, g_00844eac, 0);
					fn_004d34e8(g_00844ebc, g_00844eac, 0);
					fn_004d346f(g_00844ec0, g_00844eac);
					gNetControl.fn_00433c46();
					g_00844ec8 = 10;
				}
			}
			else
				fn_00469e0c(g_00844e90);
		}
	}
}

// FUNCTION: STARFLEETOP 0x00469096
void tState_GameConnections::fn_00469096()
{
	fn_00468297();
	gNetControl.fn_00431821();
}

// FUNCTION: STARFLEETOP 0x004690a5
bool tState_GameConnections::fn_004690a5(int id)
{
	// The original leaves the result unset for 0x1000a and for an unknown id.
	bool result;

	gNetControl.fn_00433c46();
	// HYPOTHESIS: the case order is the source order; it decides which case keeps the shared
	// fn_00469e0c call the compiler merges, and only this order reproduces the original's.
	switch (id)
	{
	case 0x10004:
		result = fn_00468527();
		break;
	case 0x10005:
		result = fn_004688a9();
		break;
	case 0x10006:
		result = fn_004683ff();
		fn_00469e0c(g_00844e88);
		break;
	case 0x1000a:
		fn_00469e0c(g_00844e9c);
		break;
	case 0x10007:
		result = fn_00468481();
		fn_00469e0c(g_00844e88);
		break;
	case 0x10008:
		if (m14 < 0)
			m14 = g_p008cc580->vfunction7("Network", "SimProtocol", 0);
		if (m14)
		{
			tString value = m1c;
			gObject_0092deb4->vfunction4(value);
			result = gNetControl.fn_004325a9(6);
			gNetControl.fn_0043290e(tString(""));
			fn_00469e0c(g_00844e88);
		}
		else
		{
			result = gNetControl.fn_004325a9(5);
			fn_0040fa40(std::string("http://www.gamespyarcade.com/launch/launch.asp?gamename=strfltcmd2"), 1);
			vfunction3();
			gNetControl.fn_00431821();
			fn_0046aa58();
		}
		break;
	case 0x10009:
		result = fn_0046858a();
		break;
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x00469271
void tState_GameConnections::fn_00469271()
{
	int i;
	for (i = 0; i < 32; i++)
		fn_004d35e3(g_00844ed0[i].name, g_00844ed0[i].group, 0);
	for (i = 0; i < 6; i++)
		fn_004d35e3(g_00844fd0[i].name, g_00844fd0[i].group, g_00844fd0[i].n);

	m1c = tString("");
	m5c = tString("");
	m9c = tString("");
	mdc = tString("");
	m11c = tString("");
	m15c = tString("");
	m19c = tString("");

	fn_004d35ac(&m1c, g_00845018[0]);
	fn_004d35ac(&m5c, g_0084501c);
	fn_004d35ac(&m9c, g_00845020);
	fn_004d35ac(&mdc, g_00845024);
	fn_004d35ac(&m11c, g_00845028);

	for (unsigned int j = 0; j < 6; j++)
		fn_004d35ac(new tValueI< tString >, g_00844e4c[j]);
	fn_004d35ac(new tValueI< tString >, g_00844e70);
}

// FUNCTION: STARFLEETOP 0x00469534
void tState_GameConnections::fn_00469534()
{
	int i;
	for (i = 0; i < 32; i++)
		fn_004d35fa(g_00844ed0[i].group);
	for (i = 0; i < 6; i++)
		fn_004d35fa(g_00844fd0[i].group);
	for (i = 0; i < 10; i++)
		fn_004d35bf(g_00845018[i], 0);

	for (unsigned int j = 0; j < 6; j++)
		delete fn_004d35bf(g_00844e4c[j], 0);
	delete fn_004d35bf(g_00844e70, 0);
}

// FUNCTION: STARFLEETOP 0x004695c4
void tState_GameConnections::fn_004695c4(const char *name, int n)
{
	tStringGroupTag *tag = new tStringGroupTag((char *)name);
	m5c = tag->mGetString(n);
	// HYPOTHESIS: the explicit test stands for a destructor on tStringGroupTag, as in vfunction2.
	if (tag)
		delete tag;
}

// FUNCTION: STARFLEETOP 0x00469636
void tState_GameConnections::fn_00469636()
{
	if (m1e4 != 0x10005)
		return;
	if (gNetControl.fn_004317c3())
		return;

	if (!m1a)
	{
		bool ready;
		int result = gNetControl.fn_00432cce(&ready, 0);
		if (!ready)
		{
			fn_004695c4("JoinStatus", 1);
			return;
		}
		if (!result)
			m1a = 0;
		else
		{
			m1a = 1;
			m1b = 1;
		}
	}

	if (m1b)
	{
		if (fn_0047685b())
			fn_00469e0c(g_00844e90);
		else
			fn_004695c4("JoinStatus", 1);
	}
}

// FUNCTION: STARFLEETOP 0x004696ca
// FUNCTION: STARFLEETOP 0x004696c0
tState_GameConnections g_0090d7a0;

// MATCH-ONLY: the class's implicit destructor (0x004696e0, unclaimed), written out for its deleting one.
// FUNCTION: STARFLEETOP 0x00469900
tState_GameConnections::~tState_GameConnections()
{
}

// HYPOTHESIS: the original calls the out-of-line tValueI<tString> constructor (0x0043d2ef) for
// the first five value members and expands it inline for m15c and m19c; this build expands all
// seven, likely an inlining budget of the full original unit, so 0x0046981c is left unclaimed.
tState_GameConnections::tState_GameConnections()
{
}

}
