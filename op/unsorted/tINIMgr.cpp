// HYPOTHESIS: tINIMgr has a vftable (0x795c0c) and one pointer-sized member at +4;
// 0x0040ceae is the compiler-generated static initializer of a global tINIMgr
// (the constructor inlined), not a callable constructor.
// HYPOTHESIS: the destructor was defined inline: the original inlines its body (vftable
// store, then a call to 0x0040cf2e) into both the scalar deleting destructor at 0x0040cec0
// and the atexit destructor of the global at 0x0040ceee. 0x0040cf2e (unnamed, so
// fn_0040cf2e) releases the object at +4 through two virtual calls and clears the member.
class tBasicFile;

// HYPOTHESIS: the object tINIFile's slot +0xc returns (0x0040cf58 calls its first
// virtual with two arguments); its class is not known.
class fn_0040cf58_entry {
public:
	virtual int vfunction0(int param_1, int param_2);
	// HYPOTHESIS: slot +0x4 returns a bool (0x0040cf8b).
	virtual bool vfunction1(int param_1, int param_2);
	virtual int vfunction2(int param_1, int param_2);
	// HYPOTHESIS: slot +0xc returns a bool (0x0040cff2).
	virtual bool vfunction3(int param_1, int param_2);
	// HYPOTHESIS: slot +0x10 takes four arguments (0x0040d025); its return type is not known.
	virtual int vfunction4(int param_1, int param_2, int param_3, int param_4);
	// HYPOTHESIS: slot +0x14 returns a bool (0x0040d05e).
	virtual bool vfunction5(int param_1, int param_2);
};

// Declared from the exe's mangled name ?mNewINIFile@tINIFile@@SAPAV1@PBDP6APAVtBasicFile@@XZ@Z
// (0x0062c670); the class is defined in its own unit.
class tINIFile {
public:
	static tINIFile* mNewINIFile(const char* name, tBasicFile* (*factory)());
	// HYPOTHESIS: tINIFile is polymorphic; 0x0040cf2e calls the virtuals at vftable
	// offsets +0x10 and then +0x8 without arguments before dropping the pointer. Their
	// names are not known; the slots are named by index.
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	// HYPOTHESIS: slot +0xc looks up an entry by its first argument (0x0040cf58).
	virtual fn_0040cf58_entry* vfunction3(int param_1, int param_2);
	virtual void vfunction4();
};

class tINIMgr {
public:
	tINIMgr() : m_field4(0) {}
	virtual ~tINIMgr() { fn_0040cf2e(); }
	virtual void fn_0040cf12(const char* param_1);
	// HYPOTHESIS: 0x0040cf2e is the virtual at vftable slot +8; 0x0040cf12 calls it
	// through the vftable before replacing the member at +4.
	virtual void fn_0040cf2e();
	virtual void fn_0040cf4b();
	virtual int fn_0040cf58(int param_1, int param_2, int param_3);
	virtual bool fn_0040cf8b(int param_1, int param_2, int param_3);
	virtual int fn_0040cfbe(int param_1, int param_2, int param_3);
	virtual bool fn_0040cff2(int param_1, int param_2, int param_3);
	virtual int fn_0040d025(int param_1, int param_2, int param_3, int param_4, int param_5);
	virtual bool fn_0040d05e(int param_1, int param_2, int param_3);
	tINIMgr* fn_0040cec0(unsigned int flags);
private:
	tINIFile* m_field4;
};

// The global manager. The compiler generates three functions from this definition, with no
// source line of their own: the dynamic initializer (_$E1, the inline constructor expanded),
// the atexit registration (_$E3) and the atexit destructor (_$E2).
// FUNCTION: STARFLEETOP 0x0040ceee
// FUNCTION: STARFLEETOP 0x0040cee2
// FUNCTION: STARFLEETOP 0x0040ceae
tINIMgr g_tINIMgr;

// MATCH-ONLY: stands in for the compiler-generated scalar deleting destructor
// (??_GtINIMgr@@UAEPAXI@Z), which has no source line the harness could claim; the
// original source had only the inline destructor above, and the compiler wrote this body.
// FUNCTION: STARFLEETOP 0x0040cec0
tINIMgr* tINIMgr::fn_0040cec0(unsigned int flags)
{
	this->tINIMgr::~tINIMgr();
	if (flags & 1)
		operator delete(this);
	return this;
}

// FUNCTION: STARFLEETOP 0x0040cf12
// HYPOTHESIS: releases the current INI file, then opens param_1 with the default
// (null) tBasicFile factory.
void tINIMgr::fn_0040cf12(const char* param_1)
{
	fn_0040cf2e();
	m_field4 = tINIFile::mNewINIFile(param_1, 0);
}

// FUNCTION: STARFLEETOP 0x0040cf2e
// HYPOTHESIS: closes the current INI file (slot +0x10), releases it (slot +0x8),
// and clears the member.
void tINIMgr::fn_0040cf2e()
{
	if (m_field4) {
		m_field4->vfunction4();
		m_field4->vfunction2();
		m_field4 = 0;
	}
}

// FUNCTION: STARFLEETOP 0x0040cf4b
// HYPOTHESIS: forwards to the current INI file's slot +0x10 (the call 0x0040cf2e makes
// before releasing it) without releasing the file.
void tINIMgr::fn_0040cf4b()
{
	if (m_field4)
		m_field4->vfunction4();
}

// FUNCTION: STARFLEETOP 0x0040cf58
// HYPOTHESIS: a lookup with a default: asks the current INI file (slot +0xc) for the
// object named by param_1, and when one is found asks it (slot +0x0) for param_2,
// passing param_3 as the default; otherwise returns param_3.
int tINIMgr::fn_0040cf58(int param_1, int param_2, int param_3)
{
	if (m_field4) {
		fn_0040cf58_entry* entry = m_field4->vfunction3(param_1, 0);
		if (param_1 && entry)
			return entry->vfunction0(param_2, param_3);
	}
	return param_3;
}

// FUNCTION: STARFLEETOP 0x0040cf8b
// HYPOTHESIS: the bool counterpart of 0x0040cf58: looks up the object named by param_1
// (slot +0xc) and passes param_2 and param_3 to its slot +0x4; false when there is none.
bool tINIMgr::fn_0040cf8b(int param_1, int param_2, int param_3)
{
	if (m_field4) {
		fn_0040cf58_entry* entry = m_field4->vfunction3(param_1, 0);
		if (param_1 && entry)
			return entry->vfunction1(param_2, param_3);
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0040cfbe
// HYPOTHESIS: the third typed counterpart of 0x0040cf58: looks up the object named by
// param_1 (slot +0xc) and asks its slot +0x8 for param_2 with param_3 as the default.
// The return type is not known; int is a guess.
int tINIMgr::fn_0040cfbe(int param_1, int param_2, int param_3)
{
	if (m_field4) {
		fn_0040cf58_entry* entry = m_field4->vfunction3(param_1, 0);
		if (param_1 && entry)
			return entry->vfunction2(param_2, param_3);
	}
	return param_3;
}

// FUNCTION: STARFLEETOP 0x0040cff2
// HYPOTHESIS: the second bool counterpart of 0x0040cf58: looks up the object named by
// param_1 (slot +0xc) and passes param_2 and param_3 to its slot +0xc; false when there
// is none.
bool tINIMgr::fn_0040cff2(int param_1, int param_2, int param_3)
{
	if (m_field4) {
		fn_0040cf58_entry* entry = m_field4->vfunction3(param_1, 0);
		if (param_1 && entry)
			return entry->vfunction3(param_2, param_3);
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0040d025
// HYPOTHESIS: the four-argument counterpart of 0x0040cf58: looks up the object named by
// param_1 (slot +0xc) and passes param_2 to param_5 to its slot +0x10; 0 when there is
// none. The return type is not known; int is a guess.
int tINIMgr::fn_0040d025(int param_1, int param_2, int param_3, int param_4, int param_5)
{
	if (m_field4) {
		fn_0040cf58_entry* entry = m_field4->vfunction3(param_1, 0);
		if (param_1 && entry)
			return entry->vfunction4(param_2, param_3, param_4, param_5);
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0040d05e
// HYPOTHESIS: the third bool counterpart of 0x0040cf58: looks up the object named by
// param_1 (slot +0xc) and passes param_2 and param_3 to its slot +0x14; false when there
// is none.
bool tINIMgr::fn_0040d05e(int param_1, int param_2, int param_3)
{
	if (m_field4) {
		fn_0040cf58_entry* entry = m_field4->vfunction3(param_1, 0);
		if (param_1 && entry)
			return entry->vfunction5(param_2, param_3);
	}
	return false;
}
