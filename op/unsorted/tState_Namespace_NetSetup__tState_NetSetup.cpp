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
#include "unsorted/tEntityMgr.h"

// HYPOTHESIS: cdecl functions shared with tState_GameConnections; their names are not known.
void fn_004be81f(int arg);
#include "unsorted/fn_004D3143.h"


bool fn_0046a9e2();



// HYPOTHESIS: the name of the screen the state shows, a global handed with it to fn_004d31ed,
// and a value the state resets to 10 (as tState_GameConnections resets its own).
extern char *g_00849398;
extern char g_008494b8[];
extern int g_008493d4;
// HYPOTHESIS: pointers to the names "IPSetup" and "ipEntryField".
extern const char *g_008493b0;
extern const char *g_008493b4;
// HYPOTHESIS: pointers to the names "ModemSetup", "modemEntry", "SerialSetup" and "cCancelGame".
extern const char *g_008493b8;
extern const char *g_008493bc;
extern const char *g_008493c0;
extern const char *g_008493cc;
// HYPOTHESIS: a separator character read by fn_004768a5.
extern char g_008493d0;




#include "unsorted/c_00468677_source.h"

// HYPOTHESIS: three global strings, 0x28 bytes apart, handed to tNetControl::fn_004327a7.
extern tString g_00910ac8;
extern tString g_00910aa0;
extern tString g_00910a78;
// HYPOTHESIS: six C strings fn_00476da0 lists in the serial rate control.
extern char *g_00849550[6];
// HYPOTHESIS: pointers to the names "cCreateGame", "cJoinGame" and the state fn_00476e72 goes
// to once connected, as in tState_GameConnections.
extern const char *g_008493c4;
extern const char *g_008493c8;
extern const char *g_008493a8;


// HYPOTHESIS: the current time, and the modem state's flag and time of its last progress dot.
extern double g_00c7c148;
extern char g_00910b38;
extern double g_00910b40;
// HYPOTHESIS: the state fn_004772ce goes to after starting the chosen protocol.
extern const char *g_008493a0;
// HYPOTHESIS: cdecl; opens the web page or launches the program file (its strings name both).
void fn_00434830(const char *url, const char *file);
void fn_0046aa58();
#include "Tiger/tFormatStr.h"
#include "unsorted/tState_NetSetup.h"
namespace tState_Namespace_NetSetup
{





















































































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
		m1dc = fn_004d3446(0x10012);
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
		m1e4 = fn_004d3446(0x10017);
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
using tState_Namespace_NetSetup::tState_NetSetup;	// the definitions below name the class unqualified
// HYPOTHESIS: a member that does not use this (fn_00476e72 passes this in ecx).
// tState_GameConnections calls it too. The object at 0x008e1060 is the tEntityMgr; the byte it
// clears at +0x14 is its protected fHost, so it is written through the table's view.
// FUNCTION: STARFLEETOP 0x0047685b
bool tState_NetSetup::fn_0047685b()
{
	int result = gNetControl.fn_00431c7f();
	((tEntityMgr &) gObject_008e1060).mSetPlayerNumber(gNetControl.fn_00431c79());
	gObject_008e1060.fField00[0x14] = 0;
	int id = ((tEntityMgr &) gObject_008e1060).mGetNextPlayerID();
	gObject_0092deb4->vfunction10(id, 0, 0);
	return result == 0 ? true : false;
}

// FUNCTION: STARFLEETOP 0x004768a5
void tState_NetSetup::fn_004768a5(c_00468677_source *source, tString &out)
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

// FUNCTION: STARFLEETOP 0x00476925
bool tState_NetSetup::fn_00476925()
{
	fn_004d3331(g_008493c0);
	fn_004d346f(g_008493cc, g_008493c0);
	bool result = gNetControl.fn_004325a9(4);
	fn_00476d52(&m1e8, &m15c, 0x1001c);
	fn_00476d52(&m1ec, &m19c, 0x1001d);
	m19[2] = 0;
	m19[1] = 1;
	return result;
}

// FUNCTION: STARFLEETOP 0x00476997
void tState_NetSetup::fn_00476997(c_00468677_source *control, const char *name, const char *value)
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

// FUNCTION: STARFLEETOP 0x00476aee
void tState_NetSetup::fn_00476aee(c_00468677_source *control, const char *name)
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
		tString *line = control->vfunction56(i);
		text = *line;
		if (line)
			g_p008cc580->vfunction10(name, key.mCharStr(), text.mCharStr());
		entry++;
	}

	key = prefix;
	key.mAppendInt32s(entry, 1);
	if (current)
		g_p008cc580->vfunction10(name, key.mCharStr(), current->mCharStr());
}

// FUNCTION: STARFLEETOP 0x00476c64
void tState_NetSetup::fn_00476c64()
{
	fn_004d3340(g_008493b0);
	fn_004d3340(g_008493b8);
	fn_004d3340(g_008493c0);
}

// FUNCTION: STARFLEETOP 0x00476c89
void tState_NetSetup::fn_00476c89()
{
	m1e0->vfunction65(0);
	fn_004d34c5(0x1000e, 0);
	m1e8->vfunction65(0);
	m1ec->vfunction65(0);
	fn_004d34c5(0x1001e, 0);
	fn_004d34c5(0x1001f, 0);
}

// FUNCTION: STARFLEETOP 0x00476ce5
void tState_NetSetup::fn_00476ce5(int id)
{
	fn_004d34c5(0x1001e, 1);
	fn_004d34c5(0x1001f, 1);
	switch (id)
	{
	case 0x10018:
		m1e0->vfunction65(1);
		fn_004d34c5(0x1000e, 1);
		break;
	case 0x1001e:
		m1e8->vfunction65(1);
		break;
	case 0x1001f:
		m1ec->vfunction65(1);
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00476d52
void tState_NetSetup::fn_00476d52(c_00468677_source **control, void *source, int id)
{
	c_00468677_source *found = fn_004d3446(id);
	if (found)
	{
		found->vfunction65(0);
		fn_00476da0(&found, id);
		found->vfunction67(1, source);
		*control = found;
	}
}

// FUNCTION: STARFLEETOP 0x00476da0
void tState_NetSetup::fn_00476da0(c_00468677_source **control, int id)
{
	(*control)->vfunction50();
	switch (id)
	{
	case 0x10016:
	case 0x1001c:
		{
			std::vector< c_004329af_entry > *ports = gNetControl.fn_004329af();
			if (ports)
			{
				for (c_004329af_entry *entry = ports->begin(); entry != ports->end(); entry++)
				{
					tString name(entry->fn_0060f6d0());
					(*control)->vfunction54(name, -1, 0, -1);
				}
			}
		}
		break;
	case 0x1001d:
		for (int i = 0; i < 6; i++)
		{
			tString rate(g_00849550[i]);
			(*control)->vfunction54(rate, -1, 0, -1);
		}
		break;
	}
}

// FUNCTION: STARFLEETOP 0x00476e72
void tState_NetSetup::fn_00476e72()
{
	if (g_00910b40 == 0.0)
		g_00910b40 = g_00c7c148;

	if (m1f0 == 0x10008 && !gNetControl.fn_004317c3())
	{
		if (!m12[0])
		{
			tString address;
			fn_004768a5(m1e4, address);
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
				m19[0] = 1;
				break;
			case 2:
				status += tag->mGetString(1);
				status += "\n";
				break;
			case 3:
			{
				tString address;
				fn_004768a5(m1e4, address);
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
				if (!g_00910b38)
				{
					status += tag->mGetString(2);
					g_00910b38 = 1;
				}
				else
				{
					double now = g_00c7c148;
					if (now > g_00910b40 + 1000000.0)
					{
						status += ".";
						g_00910b40 = g_00c7c148;
					}
				}
				break;
			case 5:
				status = tag->mGetString(4);
				m12[0] = 1;
				fn_004d34e8((long) g_008493c4, (long) g_008493b8, 0);
				fn_004d34e8((long) g_008493c8, (long) g_008493b8, 0);
				fn_004d346f(g_008493cc, g_008493b8);
				break;
			}
		}

		if (tag)
			delete tag;

		m5c = status;

		if (m19[0] && g_008493d4 >= 0)
		{
			g_00910b38 = 0;
			g_00910b40 = 0.0;
			if (!fn_0047685b())
			{
				if (g_008493d4-- > 0)
				{
					fn_004776d7("JoinStatus", 0);
					return;
				}
				else
				{
					fn_004776d7("JoinStatus", 3);
					fn_004d34e8((long) g_008493c4, (long) g_008493b8, 0);
					fn_004d34e8((long) g_008493c8, (long) g_008493b8, 0);
					fn_004d346f(g_008493cc, g_008493b8);
					gNetControl.fn_00433c46();
					g_008493d4 = 10;
				}
			}
			else
				fn_00469e0c(g_008493a8);
		}
	}
}

// FUNCTION: STARFLEETOP 0x004772bf
void tState_NetSetup::fn_004772bf()
{
	fn_004764bd();
	gNetControl.fn_00431821();
}

// FUNCTION: STARFLEETOP 0x004772ce
bool tState_NetSetup::fn_004772ce(int id)
{
	// The original leaves the result unset for an unknown id.
	bool result;

	gNetControl.fn_00433c46();
	switch (id)
	{
	case 0x10004:
		result = fn_004766f1();
		break;
	case 0x10005:
		result = fn_00476925();
		break;
	case 0x10006:
		result = fn_00476678();
		fn_00469e0c(g_008493a0);
		break;
	case 0x10007:
		if (m14 < 0)
			m14 = g_p008cc580->vfunction7("Network", "SimProtocol", 0);
		if (m14)
		{
			tString value = m1c;
			gObject_0092deb4->vfunction4(value);
			result = gNetControl.fn_004325a9(6);
			gNetControl.fn_0043290e(tString(""));
			fn_00469e0c(g_008493a0);
		}
		else
		{
			result = gNetControl.fn_004325a9(5);
			fn_00434830("http://www.mplayer.com/mic/sfc.html", "sfc.mpi");
			vfunction3();
			gNetControl.fn_00431821();
			fn_0046aa58();
		}
		break;
	case 0x10008:
		result = fn_00476754();
		break;
	}
	return result;
}

// HYPOTHESIS: the state's control name tables and the names its value objects bind to, as
// tState_GameConnections::fn_00469271 registers its own.
#include "unsorted/s_00844ed0_entry.h"
extern s_00844ed0_entry g_008493d8[];
extern s_00844fd0_entry g_00849450[];
extern const char *g_0084948c[];



// FUNCTION: STARFLEETOP 0x00477440
void tState_NetSetup::fn_00477440()
{
	int i;
	for (i = 0; i < 15; i++)
		fn_004d35e3(g_008493d8[i].name, g_008493d8[i].group, 0);
	for (i = 0; i < 5; i++)
		fn_004d35e3(g_00849450[i].name, g_00849450[i].group, g_00849450[i].n);

	m1c = tString("");
	m5c = tString("");
	m9c = tString("");
	mdc = tString("");
	m11c = tString("");
	m15c = tString("");
	m19c = tString("");

	fn_004d35ac(&m1c, g_0084948c[0]);
	fn_004d35ac(&m5c, g_0084948c[1]);
	fn_004d35ac(&m9c, g_0084948c[2]);
	fn_004d35ac(&mdc, g_0084948c[3]);
	fn_004d35ac(&m11c, g_0084948c[4]);
	fn_004d35ac(&m15c, g_0084948c[5]);
	fn_004d35ac(&m19c, g_0084948c[6]);
}




// FUNCTION: STARFLEETOP 0x00477689
void tState_NetSetup::fn_00477689()
{
	int i;
	for (i = 0; i < 15; i++)
		fn_004d35fa(g_008493d8[i].group);
	for (i = 0; i < 5; i++)
		fn_004d35fa(g_00849450[i].group);
	for (i = 0; i < 10; i++)
		fn_004d35bf(g_0084948c[i], 0);
}

// FUNCTION: STARFLEETOP 0x004776d7
void tState_NetSetup::fn_004776d7(const char *name, int n)
{
	tStringGroupTag *tag = new tStringGroupTag((char *)name);
	m5c = tag->mGetString(n);
	if (tag)
		delete tag;
}

// FUNCTION: STARFLEETOP 0x00477749
void tState_NetSetup::fn_00477749()
{
	if (m1f0 != 0x10005)
		return;
	if (gNetControl.fn_004317c3())
		return;

	if (!m19[1])
	{
		bool ready;
		int result = gNetControl.fn_00432cce(&ready, 0);
		if (!ready)
		{
			fn_004776d7("JoinStatus", 1);
			return;
		}
		if (!result)
			m19[1] = 0;
		else
		{
			m19[1] = 1;
			m19[2] = 1;
		}
	}

	if (m19[2])
	{
		if (fn_0047685b())
			fn_00469e0c(g_008493a8);
		else
			fn_004776d7("JoinStatus", 1);
	}
}
// FUNCTION: STARFLEETOP 0x004777dd
// FUNCTION: STARFLEETOP 0x004777d3
tState_Namespace_NetSetup::tState_NetSetup g_tState_NetSetup;

// Unclaimed: the implicit destructor (0x004777f3) and its scalar deleting destructor
// (0x00477a13). The original declared no destructor, and VC6 emits these with no source line to
// claim; writing one out would add a vtable store the original does not have.
// The empty comment lines here keep the unit's line numbers as they were when a destructor was
// written out (a claim is paired with the function that starts on the line below it).
//
//

// HYPOTHESIS: as in tState_GameConnections, the original calls the out-of-line tValueI<tString>
// constructor (0x0043d2ef) for m1c to m11c and expands it inline for m15c and m19c (EH states
// 1-5 then 5-8, m1f8 9, m208 10); this build expands all seven, so 0x0047792f is left
// unclaimed.
tState_NetSetup::tState_NetSetup()
{
}
