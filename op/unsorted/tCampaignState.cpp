// tCampaignState: functions of this class (original file unknown).

#include "unsorted/c_0092deb4.h"
#include "unsorted/tApplication.h"

// HYPOTHESIS: a cdecl function of one pointer argument at 0x004d32cf (the call's
// displacement lands there; the analysis export places it inside FUN_004d32c4).
void fn_004d32cf(void *p);

// HYPOTHESIS: a 0x20-byte entry of the list at tCampaignState+0x18.
struct c_tState_Campaign_entry
{
	char m_pad[0x20];
};

// HYPOTHESIS: the object at tCampaignState+0xc. The list at tCampaignState+0x18, its
// index at +0x1018 and the id at +0x1110 are taken to be its members: the original computes
// this object's address before it reads the id, as for a member read through it.
class c_004cd1db
{
public:
	// HYPOTHESIS: three arguments; the split of the pushes between this call and the two
	// virtual calls inside its argument list is read from the register use at the call
	// sites, not from the callee. Types unknown.
	// HYPOTHESIS: the destructor at 0x004cd115; tCampaignState's destructor calls it.
	~c_004cd1db();
	void fn_004cd1db(int a, int b, int id);
	void fn_004cd2c9();
	void fn_004cd2f6();
	void fn_004cd739();

	char m_pad_00[0xc];
	c_tState_Campaign_entry m_0c[0x80];
	int m_100c;
	char m_pad_1010[0xf4];
	int m_1104;
};

// HYPOTHESIS: the object at tCampaignState+0x1114; only its size (0x1c, up to the flag at
// +0x1130) and one thiscall method of one pointer argument are known.
class c_0045dece
{
public:
	// HYPOTHESIS: the destructor at 0x005d97a8; tCampaignState's destructor calls it.
	~c_0045dece();
	void fn_0045dece(const char **name);
	void fn_0045df4b();

	char m_pad_00[0xc];
	// HYPOTHESIS: the number of names held; an iterator starts from it.
	int m_0c;
	char m_pad_10[0x8];
	// HYPOTHESIS: the name compared against the top of the stack (the bottom entry).
	const char *m_18;
};

// The base of tState_Campaign: tState_Campaign's constructor (0x0045dde5) calls this class's
// constructor (0x0045de5a) before it stores its own vtable.
// HYPOTHESIS: the members live here, not in tState_Campaign: vfunction4 reads +0x1110 and
// tState_Campaign's constructor constructs nothing of its own.
// HYPOTHESIS: tCampaignState has a base class at offset 0: its destructor (0x0045de01) calls
// a destructor at 0x00469d19 on this after its members. Only that destructor is known.
class c_00469d19
{
public:
	virtual ~c_00469d19();
};

class tCampaignState : public c_00469d19 {
public:
	tCampaignState();
	// HYPOTHESIS: the original declares no destructor. The compiler-generated one (code at
	// 0x0045ddf7, which the analysis export splits at 0x0045de01) stores no vtable; an
	// implicit destructor builds byte for byte, but the harness cannot claim a function with
	// no source line, so this declared one (which stores the vtable) stands in for it.
	virtual ~tCampaignState();
	tCampaignState *scalar_deleting_destructor(unsigned char flags);
	virtual int vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	// HYPOTHESIS: slots 4 to 11 are placeholders that keep the offset of vfunction13 (0x30).
	virtual void vfunction5();
	// HYPOTHESIS: one argument (the original returns with ret 4).
	virtual void vfunction6(int arg);
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	// HYPOTHESIS: returns the state's name, as tState_NetSetup's slot 0x30 does.
	virtual const char *vfunction13();
	// HYPOTHESIS: slot 14 is a placeholder that keeps the offset of vfunction15.
	virtual void vfunction14();
	virtual void vfunction15(int *arg);

	char m_pad_04[0x8];
	c_004cd1db m_0c;
	c_0045dece m_1114;
	// HYPOTHESIS: set once the player's name has been sent.
	bool m_1130;
};

// FUNCTION: STARFLEETOP 0x0045da31
bool tCampaignState::vfunction4()
{
	// HYPOTHESIS: the value at +0x1110, a pointer here.
	fn_004d32cf((void *)m_0c.m_1104);
	return true;
}

// HYPOTHESIS: a 0x20-byte message the queue at 0x0046aa10 fills; only the id at +0x0 is
// read here, and the constructor sets the fields below.
struct c_0046aa10_msg
{
	c_0046aa10_msg();

	int id;
	int m_04;
	int m_08;
	int m_0C;
	int m_10;
	int m_14;
	int m_18;
	int m_1C;
};

inline c_0046aa10_msg::c_0046aa10_msg()
{
	id = 0;
	m_04 = -1;
	m_14 = 0;
	m_18 = -1;
	m_1C = -1;
}

// HYPOTHESIS: cdecl functions; their names are not known.
bool fn_0046aa10(c_0046aa10_msg *msg);
void fn_0046aa58();
void fn_0046a62d(const char *name);
void fn_0046a713();

// HYPOTHESIS: an 8-byte iterator over c_0045dece: a position and its owner. fn_005f7efa
// is taken to return the entry before the position (the top of the stack).
class c_0045dece_iterator
{
public:
	c_0045dece_iterator(c_0045dece *owner);
	const char **fn_005f7efa();

	int m_pos;
	c_0045dece *m_owner;
};

inline c_0045dece_iterator::c_0045dece_iterator(c_0045dece *owner)
{
	m_pos = owner->m_0c;
	m_owner = owner;
}

class tState_Campaign : public tCampaignState {
public:
	// The constructor (0x0045dde5) is not inline: the global's initializer calls it.
	tState_Campaign();
	virtual ~tState_Campaign();
	virtual void vfunction3();
	// HYPOTHESIS: the argument is not read here.
	virtual void vfunction6(int arg);
	// HYPOTHESIS: remaps a key event in place; arg[0] is the action written back, arg[1] the
	// key code read (the analysis export names this tCampaignState::vfunction15).
	virtual void vfunction15(int *arg);

	void fn_0045dc99(int id);
	bool fn_0045dd02();
};

// HYPOTHESIS: a cdecl function of two arguments at 0x0047559f (the analysis export places
// it inside FUN_004752b3).
void fn_0047559f(const char *name, const char *value);

// FUNCTION: STARFLEETOP 0x0045da40
void tState_Campaign::vfunction3()
{
	c_0046aa10_msg msg;

	while (fn_0046aa10(&msg))
	{
		switch (msg.id)
		{
		case 0x102:
			fn_0046aa58();
			return;
		case 0x103:
			fn_0046a62d("SpaceStation");
			return;
		case 0x104:
			if (!fn_0045dd02())
			{
				fn_0046a713();
				return;
			}
			break;
		case 0x105:
			fn_0045dc99(0x105);
			g_0092deb4->fn_vtbl_08(&m_0c.m_0c[m_0c.m_100c], -1);
			break;
		case 0x10006:
			m_0c.fn_004cd2c9();
			m_0c.fn_004cd739();
			break;
		case 0x10007:
			m_0c.fn_004cd2f6();
			m_0c.fn_004cd739();
			break;
		case 0x10008:
		case 0x10009:
		case 0x1000a:
		case 0x1000b:
			fn_0045dc99(msg.id);
			switch (msg.id)
			{
			case 0x10008:
				g_0092deb4->fn_vtbl_140(4, 0);
				break;
			case 0x10009:
				g_0092deb4->fn_vtbl_140(3, 0);
				break;
			case 0x1000a:
				g_0092deb4->fn_vtbl_140(2, 0);
				break;
			case 0x1000b:
				g_0092deb4->fn_vtbl_140(0, 0);
				break;
			default:
				return;
			}
			{
				c_004cd1db *p = &m_0c;
				p->fn_004cd1db(g_0092deb4->fn_vtbl_9C(), g_0092deb4->fn_vtbl_144(0, "HULL_TEMPLATE"), p->m_1104);
				p->fn_004cd739();
			}
			break;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0045dbda
void tState_Campaign::vfunction6(int arg)
{
	// HYPOTHESIS: the value at +0x1110 holds a state name here; vfunction3 passes it as an id.
	const char *name = (const char *)m_0c.m_1104;

	if ((name == vfunction13() && !m_1130) || (name == "playerOptions" && !m_1130))
	{
		fn_0047559f("PlayerName1", g_0092deb4->fn_vtbl_14());
		m_1130 = true;
	}
}

// FUNCTION: STARFLEETOP 0x0045dc2b
void tState_Campaign::vfunction15(int *arg)
{
	switch (arg[1])
	{
	case 0x102:
		arg[0] = 0x103;
		break;
	case 0x103:
		arg[0] = 0x102;
		break;
	case 0x115:
		if (g_p008cc580->vfunction5("Debug", "DebugKeys", 0))
		{
			if (arg[1] == 0x115)
				arg[0] = 0x206;
			if (arg[1] == 0x116)
				arg[0] = 0x205;
			if (arg[1] == 0x117)
				arg[0] = 0x204;
		}
		break;
	}
}

// HYPOTHESIS: cdecl functions; their names are not known.
void fn_004d32c4(const char *name);
void fn_004d31ed(const char *name, int arg, void *p, int width, int height);

// HYPOTHESIS: a global whose address is passed to fn_004d31ed; its type is not known.
extern char g_00843ed0[];

// FUNCTION: STARFLEETOP 0x0045dc99
void tState_Campaign::fn_0045dc99(int id)
{
	// HYPOTHESIS: the value at +0x1110 holds a state name, as in vfunction6.
	const char **name = (const char **)&m_0c.m_1104;

	fn_004d32c4(*name);
	m_1114.fn_0045dece(name);
	switch (id)
	{
	case 0x105:
		*name = "playerOptions";
		break;
	case 0x10008:
	case 0x10009:
	case 0x1000a:
	case 0x1000b:
		*name = "VesselLibrary";
		break;
	}
	fn_004d31ed(*name, 1, g_00843ed0, 640, 480);
}

// FUNCTION: STARFLEETOP 0x0045dd02
bool tState_Campaign::fn_0045dd02()
{
	// HYPOTHESIS: the value at +0x1110 holds a state name, as in fn_0045dc99; the name
	// pushed there is popped here.
	const char **name = (const char **)&m_0c.m_1104;

	fn_004d32c4(*name);
	*name = *c_0045dece_iterator(&m_1114).fn_005f7efa();
	m_1114.fn_0045df4b();
	if (*name == m_1114.m_18)
		return false;

	fn_004d31ed(*name, 1, g_00843ed0, 640, 480);
	const char *current = *name;
	if (current == "VesselLibrary")
	{
		c_004cd1db *p = &m_0c;
		p->fn_004cd1db(g_0092deb4->fn_vtbl_9C(), g_0092deb4->fn_vtbl_144(0, "HULL_TEMPLATE"), (int)current);
		p->fn_004cd739();
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x0045dde5
tState_Campaign::tState_Campaign()
{
}

// HYPOTHESIS: the original declares no destructor here either. The atexit stub reaches
// 0x0045ddf7 through 0x0045dde0, a lone jmp: that jmp is tState_Campaign's generated
// destructor, and 0x0045ddf7 (mov eax, data; call __EH_prolog) is the first 10 bytes of
// tCampaignState's, whose rest the analysis export calls FUN_0045de01.
// This definition only gives the declared virtual destructor a body and does not match.
// FUNCTION: STARFLEETOP 0x0045ddf7
tState_Campaign::~tState_Campaign()
{
}

// HYPOTHESIS: the rest of tCampaignState's destructor (see the declaration); it starts at
// 0x0045ddf7, so this build carries 10 bytes and a vtable store more than the original.
// FUNCTION: STARFLEETOP 0x0045de01
tCampaignState::~tCampaignState()
{
}

// FUNCTION: STARFLEETOP 0x0045deac
// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line,
// so it is written out as a member: destroy, then delete when bit 0 of the flag is set.
tCampaignState *tCampaignState::scalar_deleting_destructor(unsigned char flags)
{
	tCampaignState::~tCampaignState();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: tCampaignState's constructor, which tState_Campaign's constructor calls.
// FUNCTION: STARFLEETOP 0x0045de5a
tCampaignState::tCampaignState()
{
}

// HYPOTHESIS: a global tState_Campaign at 0x0090bfd8; its name is not known. The compiler
// generates its dynamic initializer (0x0045ddc0, a tail call to the constructor) from this
// definition, and its atexit registration of the destructor (0x0045ddca) too.
// FUNCTION: STARFLEETOP 0x0045ddca
// FUNCTION: STARFLEETOP 0x0045ddc0
tState_Campaign g_0090bfd8;
