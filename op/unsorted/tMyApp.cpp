// Unit:      unsorted/tMyApp.cpp
// Functions: 0x0040c5e6 to 0x0040c724, 10 in game/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Tiger/tpart.h"
#include "unsorted/tApplication.h"

#include <string.h>

#include "Tiger/tbasicfile.h"

// HYPOTHESIS: the object tMyApp holds at +0x4; vfunction3 calls its slot +0x2c first.
class fn_0040c5e6_Owned {
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();    // +0x2c
	virtual bool vfunction13();    // +0x30, called by tMyApp::vfunction24
	virtual void vfunction14(int);    // +0x34, called by tMyApp::vfunction21
	virtual void vfunction15(int);    // +0x38, called by tMyApp::vfunction15
	virtual void vfunction16(int);    // +0x3c, called by tMyApp::vfunction16
	virtual void vfunction17(int, int, int, int);    // +0x40, called by tMyApp::vfunction18
	virtual void vfunction18();    // +0x44, called by tMyApp::vfunction22
	virtual void vfunction19();    // +0x48, called by tMyApp::vfunction23
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22(char *);    // +0x54, called by tMyApp::vfunction19
};

// HYPOTHESIS: two one-byte flags; 0x00898c35 guards the part deactivation so it runs once,
// 0x00898c34 is set when vfunction3 has run.
extern bool g_00898c34;
extern bool g_00898c35;

class tMyApp {
public:
	bool vfunction3();
	bool vfunction24();
	bool vfunction25();
	bool vfunction15(int param_1);
	bool vfunction16(int param_1);
	bool vfunction18(int param_1, int param_2, int param_3, int param_4);
	bool vfunction19(char *param_1);
	void vfunction21(int param_1);
	void vfunction22();
	void vfunction23();
	// HYPOTHESIS: a member taking the flag at +0x8; vfunction15 calls it on this after toggling.
	void fn_0069aa30(bool);

	// HYPOTHESIS: +0x0 is the vtable pointer (vfunction3 is a virtual); kept as a field until the
	// class's virtuals are laid out.
	void *m_0;
	fn_0040c5e6_Owned *m_4;
	// HYPOTHESIS: a one-byte flag that vfunction15 toggles when it sees message 0x115.
	bool m_8;
};

// HYPOTHESIS: a free (or static) function returning a flag; vfunction15 calls it without a this.
bool fn_0069ac10();

// FUNCTION: STARFLEETOP 0x0040c5e6
bool tMyApp::vfunction3()
{
	m_4->vfunction12();
	tBasicFile::mDone();
	if (!g_00898c35) {
		g_00898c35 = true;
		tPart::mDeactivateParts();
	}
	g_00898c34 = true;
	return true;
}

// FUNCTION: STARFLEETOP 0x0040c612
bool tMyApp::vfunction25()
{
	g_p008cc580->vfunction2("SFC.INI");
	return !g_p008cc580->vfunction5("Debug", "CatchAll", 1);
}

// FUNCTION: STARFLEETOP 0x0040c63f
bool tMyApp::vfunction24()
{
	if (g_00898c34) {
		return false;
	}
	return m_4->vfunction13() ? true : false;
}

// FUNCTION: STARFLEETOP 0x0040c659
bool tMyApp::vfunction15(int param_1)
{
	if (!g_00898c34) {
		m_4->vfunction15(param_1);
		if (param_1 == 0x115 && fn_0069ac10()) {
			m_8 ^= 1;
			fn_0069aa30(m_8);
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0040c699
bool tMyApp::vfunction16(int param_1)
{
	if (!g_00898c34) {
		m_4->vfunction16(param_1);
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0040c6b3
bool tMyApp::vfunction18(int param_1, int param_2, int param_3, int param_4)
{
	if (g_00898c34) {
		return false;
	}
	m_4->vfunction17(param_1, param_2, param_3, param_4);
	return true;
}

// FUNCTION: STARFLEETOP 0x0040c6dd
bool tMyApp::vfunction19(char *param_1)
{
	if (g_00898c34) {
		return false;
	}
	// HYPOTHESIS: the cdecl call to 0x00729723 is the CRT strlen (one argument, unsigned test).
	if (strlen(param_1) > 0) {
		m_4->vfunction22(param_1);
	}
	return true;
}

// FUNCTION: STARFLEETOP 0x0040c70d
void tMyApp::vfunction21(int param_1)
{
	m_4->vfunction14(param_1);
}

// FUNCTION: STARFLEETOP 0x0040c71c
void tMyApp::vfunction22()
{
	m_4->vfunction18();
}

// FUNCTION: STARFLEETOP 0x0040c724
void tMyApp::vfunction23()
{
	m_4->vfunction19();
}
