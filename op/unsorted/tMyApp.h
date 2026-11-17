//	tMyApp.h: tMyApp, the application shell (its members sit in Source/app/shellApp.cpp and
//	unsorted/tMyApp.cpp), and the object it holds at +0x4.
// Evidence:  class (original file unknown)

#ifndef tMyApp_h
#define tMyApp_h 1

#include "unsorted/tApplication.h"

class tMyApp;

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
	// HYPOTHESIS: +0x50 takes the application and reports whether it started; tMyApp::vfunction2
	// throws "fTest3D.mInit() failed" when it returns false.
	virtual bool vfunction21(tMyApp *);
	virtual void vfunction22(char *);    // +0x54, called by tMyApp::vfunction19
};

// HYPOTHESIS: two one-byte flags; 0x00898c35 guards the part deactivation so it runs once,
// 0x00898c34 is set when vfunction3 has run.
extern bool g_00898c34;
extern bool g_00898c35;

// The destructor at 0x0040c37a stores tApplication's vtable last, so tMyApp derives from it.
class tMyApp : public tApplication {
public:
	bool vfunction2();
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

	fn_0040c5e6_Owned *m_4;
	// HYPOTHESIS: a one-byte flag that vfunction15 toggles when it sees message 0x115.
	bool m_8;
};

#endif	// tMyApp_h
