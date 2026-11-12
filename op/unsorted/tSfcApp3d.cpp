// Unit:      unsorted/tSfcApp3d.cpp
// Functions: 0x0040aadc to 0x0040b366, 36 in game/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <stdlib.h>

// tsay.h carries the part binding macros (GenAbstractBinding, BindPart) until tpart.h has them.
#include "Tiger/tsay.h"

#include "unsorted/tApp3dBase.h"

// HYPOTHESIS: a member object at +0x24 whose constructor is 0x005298a6; its type is unknown.
class fn_005298a6_Member {
public:
	fn_005298a6_Member();
	// HYPOTHESIS: the member has a destructor; ~tSfcApp3d (0x0040ab43) calls it.
	~fn_005298a6_Member();
	// HYPOTHESIS: shuts the member down; the shutdown at 0x0040ab7e calls it on &m_field24.
	void fn_0040147b();
	// HYPOTHESIS: starts the member up; vfunction21 passes it the result of 0x006997b0.
	void fn_004013f0(int);

	// HYPOTHESIS: the member sits at +0x24, not +0x21, so it holds at least one
	// four-byte field; its contents are unknown.
	int m_field0;
};

// HYPOTHESIS: the sound system object at 0x0084b47c; the three getters return the volumes the
// shutdown stores under "Sound" (+0x44 GlobalVolume, +0x60 MusicVolume, +0x4c VoiceVolume),
// and +0x08 shuts it down. Only those slots are evidenced.
class fn_0084b47c_Sound {
public:
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();    // +0x08
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual int vfunction18();    // +0x44
	virtual void vfunction19();
	virtual int vfunction20();    // +0x4c
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual int vfunction25();    // +0x60
};

extern fn_0084b47c_Sound *g_p0084b47c;

// HYPOTHESIS: a global object at 0x008f5b98 with a shutdown method at 0x004316cf; its type is
// unknown.
class fn_008f5b98_Object {
public:
	void fn_004316cf();
};

extern fn_008f5b98_Object g_008f5b98;

#include "unsorted/fn_0096d5d8_Object.h"









// HYPOTHESIS: a global object at 0x0090bf18 with a shutdown method at 0x00458928.
class fn_0090bf18_Object {
public:
	bool fn_0045884c();
	void fn_00458928();
	void fn_0045a76a();
};

extern fn_0090bf18_Object g_0090bf18;

// The zip information manager (its SDK name, tZipInfoMgr::mDone); the instance at 0x008e0678.
class tZipInfoMgr {
public:
	void mDone();
};

extern tZipInfoMgr g_008e0678;

// HYPOTHESIS: a global object at 0x00894b28 with a shutdown method at 0x006a0770.
class fn_00894b28_Object {
public:
	void fn_006a0570(int);
	void fn_006a0770();
};

extern fn_00894b28_Object g_00894b28;

// HYPOTHESIS: the object the pointer at 0x00898c24 names (the mouse: vfunction21 starts it
// with the "Mouse" settings); +0x04 and +0x0c start it up, +0x08 shuts it down.
class fn_00898c24_Object {
public:
	virtual void vfunction1();
	virtual void vfunction2();    // +0x04
	virtual void vfunction3();    // +0x08
	virtual void vfunction4(int, int, int);    // +0x0c
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7(int);    // +0x18
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10(const char *);    // +0x24
};

extern fn_00898c24_Object *g_p00898c24;

// The thread pool (its SDK names, tThreadPool::Instance and tThreadPool::Shutdown).
class tThreadPool {
public:
	static tThreadPool *Instance();
	void SetMaxThreads(unsigned long);
	void Shutdown(unsigned long);
	void Heartbeat();
};

class tThreadState {
public:
	static void SetCurThreadState(tThreadState *);
};

class tOSVersion {
public:
	static tOSVersion *Instance();
	bool Init();
};

// HYPOTHESIS: the object 0x007134c0 returns; vfunction21 calls its +0x04 with 0.
class fn_007134c0_Object {
public:
	virtual void vfunction1();
	virtual void vfunction2(int);    // +0x04
};

// The string manager part (its type descriptor and its binding's cache pointer at 0x00898b80
// name it tStrManagerPart); vfunction21 binds it with the part binding macro.
// HYPOTHESIS: +0x20 loads a string file (name, flag); the slots before it after tPart's own
// are unknown.
class tStrManagerPart : public tPart {
public:
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9(const char *, int);    // +0x20

	// FUNCTION: STARFLEETOP 0x0040b0d5
	GenAbstractBinding( tStrManagerPart );
};

tThreadState *fn_005d4c97();
fn_007134c0_Object *fn_007134c0();
int fn_006997b0();
void fn_0040cdb8();
void fn_0040b14a();
bool fn_00469e75(int, int);
void fn_004ece00();

// HYPOTHESIS: settings globals vfunction21 reads from "3D" and "UI".
extern char g_b008584a2;    // "backdrop"
extern int g_008584d8;      // "spacedust"
extern char g_b0083c766;    // "shipstacking"
extern float g_f0083e7ac;   // "ambientlighting", clamped to 0..0.5
extern char g_b00c38458;    // "ASCIICharacterSet"

void fn_004ecee3();
void fn_00469ed8();
void fn_00504399();
void fn_0060c3fa();

// HYPOTHESIS: field names and types are unknown; offsets are from the constructor only.
class tSfcApp3d : public tApp3dBase {
public:
	tSfcApp3d();
	virtual ~tSfcApp3d();
	void vfunction12();
	void fn_0040ab7e();
	void fn_0040acb6();
	virtual bool vfunction21(tApplication *);
	// HYPOTHESIS: the per-frame update; the base slot it overrides is not established.
	bool vfunction13();
	void vfunction9(tView3D *);
	void vfunction8(tView3D *);
	void vfunction1(tView3D *);
	void vfunction3(tView3D *);
	void vfunction2(tView3D *);
	void vfunction4(tView3D *);

	char m_field18;    // +0x18
	char m_field19;    // +0x19
	char m_field1A;    // +0x1a
	int m_field1C;     // +0x1c
	char m_field20;    // +0x20
	fn_005298a6_Member m_field24;    // +0x24
};

// FUNCTION: STARFLEETOP 0x0040aadc
tSfcApp3d::tSfcApp3d()
{
	m_field19 = 0;
	m_field1A = 0;
	m_field1C = -1;
	m_field18 = 1;
	m_field20 = 1;
}

// The exception-handling prolog (mov eax, $L; call __EH_prolog) sits at 0x0040ab39, ten
// bytes before the recorded start; the 49 bytes from 0x0040ab43 match this body.
// FUNCTION: STARFLEETOP 0x0040ab43
tSfcApp3d::~tSfcApp3d()
{
}

// The recorded 10 bytes are only the exception-handling prolog (mov eax, $L; call
// __EH_prolog) of the shutdown routine; its body is recorded separately as 0x0040ab7e and is
// written in full as fn_0040ab7e below. The EH table of this prolog's handler (FuncInfo at
// 0x007e7dd0) covers 0x0040ab7e..0x0040ad35: thirteen try blocks, each with one catch(...),
// so the routine runs to 0x0040ad35 and the catch funclets and continuation pieces the analysis
// recorded as functions in between (0x0040ac0d..0x0040ad2f) are its own.
// HYPOTHESIS: when the analysis rows are merged, fn_0040ab7e becomes this function's body;
// this empty stub is a placeholder.
// STUB: STARFLEETOP 0x0040ab74
void tSfcApp3d::vfunction12()
{
}

// The body of tSfcApp3d::vfunction12 (the recorded row starts right after the
// exception-handling prolog at 0x0040ab74, so it cannot be claimed as that function here).
// It is written in full so the whole routine exists once in source: each try block of the
// original is a shutdown step that must not stop the ones after it. The recorded 143 bytes are
// the first two try blocks; the rest (0x0040ac19..0x0040ad35) follows them in the original.
// HYPOTHESIS: the order and grouping of the steps follow the original's state numbering.
// FUNCTION: STARFLEETOP 0x0040ab7e
void tSfcApp3d::fn_0040ab7e()
{
	try {
		g_008f5b98.fn_004316cf();
	} catch (...) {
	}
	try {
		int globalVolume = g_p0084b47c->vfunction18();
		int musicVolume = g_p0084b47c->vfunction25();
		int voiceVolume = g_p0084b47c->vfunction20();
		g_p008cc580->vfunction6("Sound", "GlobalVolume", globalVolume);
		g_p008cc580->vfunction6("Sound", "MusicVolume", musicVolume);
		g_p008cc580->vfunction6("Sound", "VoiceVolume", voiceVolume);
		g_p0084b47c->vfunction3();
	} catch (...) {
	}
	try {
		g_p0096d5d8->fn_00510b1b();
	} catch (...) {
	}
	try {
		g_0090bf18.fn_00458928();
	} catch (...) {
	}
	try {
		fn_004ecee3();
		g_008e0678.mDone();
		g_00894b28.fn_006a0770();
	} catch (...) {
	}
	try {
		fn_00469ed8();
	} catch (...) {
	}
	try {
		g_p00898c24->vfunction3();
	} catch (...) {
	}
	try {
		g_p008cc580->vfunction3();
	} catch (...) {
	}
	try {
		fn_00504399();
	} catch (...) {
	}
	try {
		m_field24.fn_0040147b();
	} catch (...) {
	}
	try {
		tApp3dBase::vfunction12();
	} catch (...) {
	}
	try {
		fn_0060c3fa();
	} catch (...) {
	}
	try {
		tThreadPool::Instance()->Shutdown(1);
	} catch (...) {
	}
}

// The recorded 37 bytes are not a function of their own: they are the third and fourth try
// blocks of tSfcApp3d::vfunction12 (EH states 4 and 6, written in fn_0040ab7e above), reached
// by falling through from the catch continuation of the second block. The row is written as
// those two steps so it scores; the try blocks and the frame belong to the enclosing routine.
// HYPOTHESIS: when the analysis rows are merged, this row disappears into fn_0040ab7e.
// FUNCTION: STARFLEETOP 0x0040ac19
void fn_0040ac19()
{
	try {
		g_p0096d5d8->fn_00510b1b();
	} catch (...) {
	}
	try {
		g_0090bf18.fn_00458928();
	} catch (...) {
	}
}

// The recorded 46 bytes are not a function of their own: they are the fifth and sixth try
// blocks of tSfcApp3d::vfunction12 (EH states 8 and 10, written in fn_0040ab7e above), reached
// by falling through from the catch continuation of the fourth block. The row is written as
// those two steps so it scores; the try blocks and the frame belong to the enclosing routine.
// HYPOTHESIS: when the analysis rows are merged, this row disappears into fn_0040ab7e.
// FUNCTION: STARFLEETOP 0x0040ac4a
void fn_0040ac4a()
{
	try {
		fn_004ecee3();
		g_008e0678.mDone();
		g_00894b28.fn_006a0770();
	} catch (...) {
	}
	try {
		fn_00469ed8();
	} catch (...) {
	}
}

// The recorded 38 bytes are not a function of their own: they are the seventh and eighth try
// blocks of tSfcApp3d::vfunction12 (written in fn_0040ab7e above), reached by falling through
// from the catch continuation of the sixth block. The row is written as those two steps so it
// scores; the try blocks and the frame belong to the enclosing routine.
// HYPOTHESIS: when the analysis rows are merged, this row disappears into fn_0040ab7e.
// FUNCTION: STARFLEETOP 0x0040ac84
void fn_0040ac84()
{
	try {
		g_p00898c24->vfunction3();
	} catch (...) {
	}
	try {
		g_p008cc580->vfunction3();
	} catch (...) {
	}
}

// The recorded 32 bytes are not a function of their own: they are the ninth and tenth try
// blocks of tSfcApp3d::vfunction12 (EH states 0x10 and 0x12, written in fn_0040ab7e above),
// reached by falling through from the catch continuation of the eighth block; `this` is read
// back from the enclosing frame. The row is written as those two steps so it scores.
// HYPOTHESIS: when the analysis rows are merged, this row disappears into fn_0040ab7e.
// FUNCTION: STARFLEETOP 0x0040acb6
void tSfcApp3d::fn_0040acb6()
{
	try {
		fn_00504399();
	} catch (...) {
	}
	try {
		m_field24.fn_0040147b();
	} catch (...) {
	}
}

// The recorded 36 bytes are not a function of their own: they are the thirteenth and last try
// block of tSfcApp3d::vfunction12 (EH state 0x18, written in fn_0040ab7e above), reached by
// falling through from the catch continuation of the twelfth block, followed by the routine's
// epilogue (the saved ebx, esi and edi and the exception frame belong to the enclosing routine).
// The row is written as that one step so it scores.
// HYPOTHESIS: when the analysis rows are merged, this row disappears into fn_0040ab7e.
// FUNCTION: STARFLEETOP 0x0040ad0b
void fn_0040ad0b()
{
	try {
		tThreadPool::Instance()->Shutdown(1);
	} catch (...) {
	}
}

// The view settings' defaults: windowed -1 (unset), Z-buffer on, an 800x600 view.
// HYPOTHESIS: every member is set in the initializer list, and the last three floats are one
// vector member built by its inline constructor; assignments in the body schedule the float
// stores differently.
// FUNCTION: STARFLEETOP 0x0040b080
fn_0040105e_Params::fn_0040105e_Params()
	: m_field00(-1),
	  m_field04(1),
	  m_field08(1),
	  m_field0C(0),
	  m_field0D(1),
	  m_field10(30.0f),
	  m_field14(1.0f),
	  m_field18(10000.0f),
	  m_field1C(800),
	  m_field20(600),
	  m_left(0),
	  m_top(0),
	  m_right(0),
	  m_bottom(0),
	  m_field34(1.0f, 0.0f, 10.0f)
{
}

// The startup routine: it starts the thread pool and the OS checks, loads the string table,
// reads the "3D" settings into the view parameters, opens the view through the base class and
// then starts the mouse, the zip manager and the remaining subsystems.
// FUNCTION: STARFLEETOP 0x0040ad35
bool tSfcApp3d::vfunction21(tApplication *param_1)
{
	tThreadPool::Instance()->SetMaxThreads(1);
	tThreadState::SetCurThreadState(fn_005d4c97());
	fn_007134c0()->vfunction2(0);
	if (!tOSVersion::Instance()->Init()) {
		return false;
	}
	m_field24.fn_004013f0(fn_006997b0());
	fn_0040cdb8();
	fn_0040b14a();
	BindPart(tStrManagerPart)->vfunction9("assets/strings/strings.txt", 0);
	m_field19 = (char)g_p008cc580->vfunction5("3D", "ZBuffer", 0);

	switch (g_p008cc580->vfunction5("3D", "lowres", 0)) {
	case 0:
		param_1->fn_0069a880(800, 600);
		break;
	case 2:
		param_1->fn_0069a880(960, 720);
		break;
	case 3:
		param_1->fn_0069a880(1024, 768);
		break;
	case 4:
		param_1->fn_0069a880(1152, 864);
		break;
	case 5:
		param_1->fn_0069a880(1280, 1024);
		break;
	case 6:
		param_1->fn_0069a880(1600, 1200);
		break;
	default:
		param_1->fn_0069a880(640, 480);
		break;
	}

	fn_0040105e_Params params;
	int bottom = param_1->fn_0069a970();
	params.m_right = param_1->fn_0069a940();
	params.m_left = 0;
	params.m_top = 0;
	params.m_bottom = bottom;
	params.m_field00 = g_p008cc580->vfunction5("3D", "windowed", 0) != 0;
	params.m_field08 = params.m_field04 = m_field19 != 0;
	params.m_field0C = g_p008cc580->vfunction5("3D", "wireframe", 0) != 0;
	g_b008584a2 = g_p008cc580->vfunction5("3D", "backdrop", 1) != 0;
	g_008584d8 = g_p008cc580->vfunction5("3D", "spacedust", 2);
	g_b0083c766 = g_p008cc580->vfunction5("3D", "shipstacking", 1) != 0;

	char buffer[50];
	g_p008cc580->vfunction9("3D", "ambientlighting", "0.30", buffer, sizeof(buffer));
	g_f0083e7ac = (float)atof(buffer);
	if (g_f0083e7ac > 0.5f) {
		g_f0083e7ac = 0.5f;
	} else if (g_f0083e7ac < 0.0f) {
		g_f0083e7ac = 0.0f;
	}
	params.m_field1C = param_1->fn_0069a940();
	params.m_field20 = param_1->fn_0069a970();
	if (!tApp3dBase::vfunction11(param_1, &params)) {
		return false;
	}
	g_p00898c24->vfunction2();
	tView3D *view = m_field10;
	int mouseMode = view->vfunction86(g_p008cc580->vfunction5("Mouse", "async", 1),
									  g_p008cc580->vfunction5("Mouse", "hwblit", 1));
	g_p00898c24->vfunction4(param_1->fn_0069aa00(), param_1->fn_006fe8a0(), mouseMode);
	g_p00898c24->vfunction7(1);
	g_p00898c24->vfunction10("Erik");
	g_00894b28.fn_006a0570(0);
	if (!fn_00469e75(param_1->fn_0069aa00(), param_1->fn_006fe8a0())) {
		return false;
	}
	fn_004ece00();
	if (!g_0090bf18.fn_0045884c()) {
		return false;
	}
	g_p0096d5d8->fn_00510af7();
	g_b00c38458 = (char)g_p008cc580->vfunction5("UI", "ASCIICharacterSet", 0);
	return true;
}

// HYPOTHESIS: four quadrant tables of 10001 entries at 0x00898ca8, filled with -11 (unset).
extern short g_00898ca8[4][10001];
// HYPOTHESIS: a function pointer at 0x008ac574 that stores the square root of its first
// argument through its second.
extern void (*g_p008ac574)(float, float *);
short fn_0069afc0(float, float);

// HYPOTHESIS: builds the four quadrant tables from the unit circle: entry i of each holds the
// value fn_0069afc0 gives for the point (+-x, +-sqrt(1 - x*x)), x = i / 10000; then counts
// the entries left unset (the count is not used).
// FUNCTION: STARFLEETOP 0x0040b14a
void fn_0040b14a()
{
	int i;
	int j;
	int count;
	short *entry;
	float x;
	float y;
	float negX;

	for (entry = &g_00898ca8[0][0]; entry < &g_00898ca8[4][0]; entry++) {
		*entry = -11;
	}
	for (i = 0; i <= 10000; i++) {
		x = i * 0.0001f;
		g_p008ac574(1.0f - x * x, &y);
		g_00898ca8[0][i] = fn_0069afc0(x, y);
		negX = -x;
		g_00898ca8[1][i] = fn_0069afc0(negX, y);
		g_00898ca8[2][i] = fn_0069afc0(negX, -y);
		g_00898ca8[3][i] = fn_0069afc0(x, -y);
	}
	count = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 10001; j++) {
			if (g_00898ca8[i][j] == -11) {
				count++;
			}
		}
	}
}

// HYPOTHESIS: an object at 0x00c7c120 updated first each frame through 0x006a0bb0.
class fn_00c7c120_Object {
public:
	void fn_006a0bb0();
};

extern fn_00c7c120_Object g_00c7c120;

// HYPOTHESIS: the object 0x0060b3f4 returns, updated through 0x005d80e0.
class fn_0060b3f4_Object {
public:
	void fn_005d80e0();
};

fn_0060b3f4_Object *fn_0060b3f4();

// The heartbeat timer (its SDK name, tHeartbeatTimer::Heartbeat).
class tHeartbeatTimer {
public:
	static void Heartbeat();
};

#include "unsorted/fn_0090bed4_Object.h"







// HYPOTHESIS: a global object at 0x0096e7c8 updated through 0x0051d7cf.
class fn_0096e7c8_Object {
public:
	void fn_0051d7cf();
};

extern fn_0096e7c8_Object g_0096e7c8;

void fn_004ecf67();
bool fn_0046aa94();

// HYPOTHESIS: the per-frame update: it runs the heartbeats of the subsystems, then the base
// class's frame (0x004012db); if that succeeds it updates the sound, network and game objects
// and returns false when 0x0046aa94 reports true (read as "keep running" by the caller).
// FUNCTION: STARFLEETOP 0x0040b241
bool tSfcApp3d::vfunction13()
{
	g_00c7c120.fn_006a0bb0();
	g_0090bf18.fn_0045a76a();
	fn_0060b3f4()->fn_005d80e0();
	tThreadPool::Instance()->Heartbeat();
	tHeartbeatTimer::Heartbeat();
	fn_004ecf67();
	if (!tApp3dBase::vfunction20()) {
		return false;
	}
	if (g_p0084b47c != 0) {
		g_p0084b47c->vfunction4();
	}
	g_p0096d5d8->fn_00510b55();
	g_p0090bed4->fn_00522681();
	g_0096e7c8.fn_0051d7cf();
	return !fn_0046aa94();
}

// HYPOTHESIS: an object at 0x0083b590 handed to the application; its type is unknown.
extern char g_0083b590[];
void fn_0046a574(tView3D *);

// HYPOTHESIS: passes 0x0083b590 to the application object (0x0069a9ad), then hands the view
// to 0x0046a574.
// FUNCTION: STARFLEETOP 0x0040b2c1
void tSfcApp3d::vfunction9(tView3D *param_1)
{
	m_fieldC->fn_0069a9ad(g_0083b590);
	fn_0046a574(param_1);
}

void fn_0046a588(tView3D *);

// HYPOTHESIS: hands the view to 0x0046a588, then sets the flag at +0x18 when a view was given.
// FUNCTION: STARFLEETOP 0x0040b2db
void tSfcApp3d::vfunction8(tView3D *param_1)
{
	fn_0046a588(param_1);
	if (param_1 != 0) {
		m_field18 = 1;
	}
}

void fn_00469e4d();
void fn_0046a428();
void fn_0046a534(tView3D *);

// HYPOTHESIS: calls 0x00469e4d and 0x0046a428, then hands the view to 0x0046a534 unless the
// base class's flag at +0x14 is set.
// FUNCTION: STARFLEETOP 0x0040b2f7
void tSfcApp3d::vfunction1(tView3D *param_1)
{
	fn_00469e4d();
	fn_0046a428();
	if (m_field14 == 0) {
		fn_0046a534(param_1);
	}
}

void fn_0046a558(tView3D *);

// HYPOTHESIS: hands the view to 0x0046a558 unless the base class's flag at +0x14 is set.
// FUNCTION: STARFLEETOP 0x0040b318
void tSfcApp3d::vfunction3(tView3D *param_1)
{
	if (m_field14 == 0) {
		fn_0046a558(param_1);
	}
}

void fn_0046a51f(tView3D *);
void fn_0046a4a8(tView3D *);

// HYPOTHESIS: brackets the view hand-off with the mouse object's +0x1c and +0x20; when the flag
// at +0x18 is set it clears it and hands the view to 0x0046a51f first, then always to 0x0046a4a8.
// FUNCTION: STARFLEETOP 0x0040b32b
void tSfcApp3d::vfunction2(tView3D *param_1)
{
	g_p00898c24->vfunction8();
	if (m_field18 != 0) {
		m_field18 = 0;
		fn_0046a51f(param_1);
	}
	fn_0046a4a8(param_1);
	g_p00898c24->vfunction9();
}

void fn_0046a4bd(tView3D *);
int fn_0040b3bf();
void fn_0050a710(int, tView3D *);

// HYPOTHESIS: brackets the frame with the mouse object's +0x1c and +0x20; when the view's +0x1c
// reports true it hands the view to 0x0046a4bd and calls the view's +0x20, then, once, while
// the flag at +0x1a is set and the value at +0x1c is nonzero, passes 0x0040b3bf's result and
// the view to 0x0050a710.
// FUNCTION: STARFLEETOP 0x0040b366
void tSfcApp3d::vfunction4(tView3D *param_1)
{
	g_p00898c24->vfunction8();
	if (param_1->vfunction8()) {
		fn_0046a4bd(param_1);
		param_1->vfunction9();
		if (m_field1A != 0 && m_field1C != 0) {
			m_field1A = 0;
			fn_0050a710(fn_0040b3bf(), param_1);
		}
	}
	g_p00898c24->vfunction9();
}
