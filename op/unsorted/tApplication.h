//	tApplication.h: tApplication, and the settings object at 0x008cc580.
// Evidence:  class (original file unknown)

#ifndef tApplication_h
#define tApplication_h 1

// HYPOTHESIS: the application object; tApp3dBase::vfunction11 and tSfcApp3d::vfunction21 take
// it as their argument. fn_0069a880 sets the display size (width, height).
class tApplication {
public:
	virtual ~tApplication();
	// HYPOTHESIS: takes a one-byte out flag that it clears; the type is a guess from the store width.
	void vfunction5(char *param_1);
	int vfunction7();
	int vfunction8();
	// HYPOTHESIS: one unused four-byte argument (ret 4); its type is unknown.
	bool vfunction19(int);

	void fn_0069a880(int, int);
	// HYPOTHESIS: sets the window title; tMyApp::vfunction2 passes "Starfleet Command".
	void fn_0069a9a0(const char *);
	int fn_0069a940();
	int fn_0069a970();
	int fn_0069aa00();
	int fn_006fe8a0();
	void fn_0069a9ad(void *);
};

// HYPOTHESIS: the settings object at 0x008cc580. +0x04 opens the named settings file, +0x08
// shuts the object down, +0x10 reads a named integer setting (section, key, default), +0x14
// stores one (section, key, value) and +0x20 reads a named string setting (section, key,
// default, buffer, size). It is kept here, beside the application that reads it, until it has a
// header of its own.
class fn_008cc580_Settings {
public:
	virtual void vfunction1();
	virtual void vfunction2(const char *);    // +0x04
	virtual void vfunction3();    // +0x08
	virtual void vfunction4();
	virtual int vfunction5(const char *, const char *, int);    // +0x10
	virtual void vfunction6(const char *, const char *, int);    // +0x14
	// HYPOTHESIS: reads a named setting (section, key, default) like +0x10.
	virtual int vfunction7(const char *, const char *, int);    // +0x18
	virtual void vfunction8();
	// HYPOTHESIS: returns the setting's length; tSFCPlayer and 0x00523198 size a buffer by it.
	virtual int vfunction9(const char *, const char *, const char *, char *, int);    // +0x20
	// HYPOTHESIS: stores a named string setting (section, key, value) like +0x14.
	virtual void vfunction10(const char *, const char *, const char *);    // +0x24
};

extern fn_008cc580_Settings *g_p008cc580;

#endif	// tApplication_h
