// HYPOTHESIS: layouts below are recovered from the disassembly only; no header or SDK
// declaration of these classes was found. Unknown members are named by offset.

#include <vector>
#include "tEntity.h"


#include "unsorted/tObject_0090be60.h"









#include "unsorted/fn_0090be9c_type.h"


















// HYPOTHESIS: the current time.
extern double g_00c7c148;

// HYPOTHESIS: a 16-byte record at +0x1f0 of the panel constructor's argument, handed to 0x004b97cc; only its
// last two fields (+0x8, +0xc) are seen.
struct tObject_004b97cc_arg
{
	int m_n00;
	int m_n04;
	int m_n08;
	int m_n0c;
};

class tObject_0048bd57_arg
{
public:
	void fn_004b97cc(tObject_004b97cc_arg *pArg);

	char m_pad00[0x1f0];
	tObject_004b97cc_arg m_1f0;
};

// HYPOTHESIS: the panel's child widgets share a base with a short rectangle at +0x34 and
// virtual slots 6 (move to x, y) and 43 (no arguments) used here.
class tObject_0048c1b5_wnd
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6(int x, int y);
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
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	// HYPOTHESIS: no arguments; called by 0x0048c418 on each button not yet done.
	virtual void vfunction44();
	virtual void vfunction45();
	// HYPOTHESIS: one argument, the message code passed through from 0x0048c3a6.
	virtual void vfunction46(int n);
	// HYPOTHESIS: one argument, passed through from 0x0048c36d.
	virtual void vfunction47(int n);
	virtual void vfunction48();
	// HYPOTHESIS: no arguments; called on each fleet button by 0x0048c34e.
	virtual void vfunction49();
	// HYPOTHESIS: a float and two further arguments, passed through from 0x0048c3e7.
	virtual void vfunction50(float f, int param_2, int param_3);

	// HYPOTHESIS: a truth value; 0x0048c418 skips slot 44 for a button where it is true.
	bool fn_004c3a2c();

	char m_pad04[0x30];
	short m_s34;
	short m_s36;
	short m_s38;
	short m_s3a;
	char m_pad3c[0x104];
	tObject_0048c1b5_wnd *m_p140;
};

// HYPOTHESIS: a 0x158-byte object the panel creates once; its constructor is 0x0048ba83.
class tObject_0048ba83
{
public:
	tObject_0048ba83();
	~tObject_0048ba83();	// 0x0048bbac
	// HYPOTHESIS: no arguments; called once per interval by 0x0048c418.
	void fn_0048bc40();
	char m_pad00[0x158];
};

void fn_004d3331(const char *psz);
// HYPOTHESIS: cdecl, two strings and a zero, as called by 0x0048c36d.
void fn_004d3564(const char *psz1, const char *psz2, int n);
// HYPOTHESIS: cdecl, two strings, returns a 1-based index, as called by 0x0048c3a6.
int fn_004d3551(const char *psz1, const char *psz2);
extern "C" int sprintf(char *, const char *, ...);
extern bool g_b0090e004;
extern const char *g_apsz0084c5e0[6];
void fn_004d3306(const char *psz);
// HYPOTHESIS: cdecl, no arguments; the values each returns (a width, a height) from 0x0040c28c.
int fn_0046abb9();
int fn_0046abc5();
// HYPOTHESIS: the two objects at 0x0096908c and 0x00969088: slot 9 takes four integers (a
// rectangle) and slot 10 a float.
class c_0096908c
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9(int x, int y, int w, int h);
	virtual void vfunction10(float f);
};
extern c_0096908c *g_0096908c;
extern c_0096908c *g_00969088;
// HYPOTHESIS: flags for the names at +0x58 and +0x98, set while each is in use.
extern int g_00916f4c;
extern int g_00916f50;

#include "unsorted/tArea3Panel.h"













#include "unsorted/tFleetPanel.h"

// FUNCTION: STARFLEETOP 0x0048bd4d
tFleetPanel::tFleetPanel(tObject_0048bd57_arg *pCharacter)
{
	m_pD8 = 0;
	m_pCharacter = pCharacter;
	if (pCharacter) {
		m_n14 = pCharacter->m_1f0.m_n08;
		m_n18 = pCharacter->m_1f0.m_n0c;
	}
	// HYPOTHESIS: the rest of the construction (resource loading through the
	// globals at 0x916f48..0x916f50, the six button images, the 0x84c5e0 string
	// table loop, the final screen setup) is not yet recovered.
}

// FUNCTION: STARFLEETOP 0x0048c05c
tFleetPanel::~tFleetPanel()
{
	int w = fn_0046abb9();
	fn_0046abc5();
	g_0096908c->vfunction9(240, 70, w - 10, 300);
	g_0096908c->vfunction10(25.0f);
	g_00969088->vfunction10(25.0f);
	if (m_pCharacter) {
		tObject_004b97cc_arg *pArg = &m_pCharacter->m_1f0;
		pArg->m_n08 = m_n14;
		pArg->m_n0c = m_n18;
		m_pCharacter->fn_004b97cc(pArg);
	}
	int i;
	for (i = 0; i < 6; i++)
		fn_004d3306(g_apsz0084c5e0[i]);
	fn_004d3306("FCtrl_FormBG");
	for (i = 0; i < 6; i++) {
		char buf[64];
		sprintf(buf, "FCtrl_Form%d", i + 1);
		fn_004d3306(buf);
	}
	if (m_pD8) {
		delete m_pD8;
		m_pD8 = 0;
	}
	if (g_00916f4c) {
		g_00916f4c = 0;
		fn_004d3306(m_sz58);
	}
	if (g_00916f50) {
		g_00916f50 = 0;
		fn_004d3306(m_sz98);
	}
}

// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line,
// so the harness cannot claim it directly; it is written out as a member that runs the
// destructor, then deletes when bit 0 of the flag is set.
// FUNCTION: STARFLEETOP 0x0048c040
tFleetPanel *tFleetPanel::scalar_deleting_destructor(unsigned char flags)
{
	tFleetPanel::~tFleetPanel();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// HYPOTHESIS: the whole of tFleetPanel::vfunction2, which the analysis splits into its
// exception prolog (0x0048c1ab, 10 bytes) and its body (0x0048c1b5, 409 bytes). Neither
// address can claim it until the boundary is corrected: compiled whole, it is 415 bytes that
// start with the prolog. Left unclaimed so the recovered body is kept. The strings are the
// original's at 0x0084cc48 and 0x0084cbe4; the latter is the format of the sprintf of i + 1
// into a 64-byte buffer.
void tFleetPanel::vfunction2()
{
	if (g_b0090e004)
		return;

	fn_004d3331(m_psz04);
	int i;
	for (i = 0; i < 6; i++) {
		if (m_apButton[i])
			m_apButton[i]->vfunction43();
	}

	int y;
	int gap;
	if (m_apLabel[0]) {
		y = m_apLabel[0]->m_s36;
		gap = m_apLabel[0]->m_s3a - m_apLabel[0]->m_s36;
		if (gap < 0)
			gap = 0;
	} else {
		y = 0;
		gap = 0;
	}
	int bottom = (m_apButton[0] && m_apButton[0]->m_p140) ? m_apButton[0]->m_p140->m_s36 : 0;
	int step = (bottom - gap * 6 - y) / 5 + gap;

	fn_004d3331("FCtrl_FormBG");
	for (i = 0; i < 6; i++) {
		if (m_apLabel[i]) {
			if (step > 0) {
				m_apLabel[i]->vfunction6(m_apLabel[i]->m_s34, y);
				y += step;
			}
			char buf[64];
			sprintf(buf, "FCtrl_Form%d", i + 1);
			fn_004d3331(buf);
		}
	}

	if (!m_pD8)
		m_pD8 = new tObject_0048ba83;

	for (i = 0; i < 6; i++)
		fn_004d3331(g_apsz0084c5e0[i]);

	m_p4c->m_s3a = m_apButton[0]->m_p140 ? m_apButton[0]->m_p140->m_s36 : 0;

	if (m_pCharacter && m_apLabel[0] && m_apButton[0]) {
		tObject_004b97cc_arg *pArg = &m_pCharacter->m_1f0;
		pArg->m_n08 = m_apLabel[0]->m_s34;
		pArg->m_n0c = m_apButton[0]->m_p140 ? m_apButton[0]->m_p140->m_s36 : 0;
		m_pCharacter->fn_004b97cc(pArg);
	}
}

// FUNCTION: STARFLEETOP 0x0048c34e
void tFleetPanel::fn_0048c34e()
{
	for (int i = 0; i < 6; i++) {
		if (m_apButton[i])
			m_apButton[i]->vfunction49();
	}
}

// The two strings are the original's at 0x0084cc3c and 0x0084cc14.

// FUNCTION: STARFLEETOP 0x0048c36d
void tFleetPanel::fn_0048c36d(int n)
{
	for (int i = 0; i < 6; i++) {
		if (m_apButton[i])
			m_apButton[i]->vfunction47(n);
	}
	fn_004d3564("A3_FLEET", "FleetGroup", 0);
}

// FUNCTION: STARFLEETOP 0x0048c3a6
// The string is the original's at 0x0084cc84.
void tFleetPanel::vfunction3(int *param_1)
{
	if (*param_1 != 0x10076) {
		tArea3Panel::vfunction3(param_1);
		return;
	}
	int i = fn_004d3551(m_psz04, "ActiveShip") - 1;
	tObject_0048c1b5_wnd *pButton = m_apButton[i];
	if (pButton)
		pButton->vfunction46(*param_1);
}

// FUNCTION: STARFLEETOP 0x0048c3e7
void tFleetPanel::vfunction6(float f, int param_2, int param_3)
{
	for (int i = 0; i < 6; i++) {
		if (m_apButton[i])
			m_apButton[i]->vfunction50(f, param_2, param_3);
	}
}

// HYPOTHESIS: the panel's periodic update: buttons that are not done get slot 44; then, once
// per interval (201234 time units, rescheduled from the 0x0090be9c rate), the fleet list is
// refreshed when the selected object or its member count changed. The strings at 0x0084cc3c
// and 0x0084cc14 are the same two that 0x0048c36d passes.
// FUNCTION: STARFLEETOP 0x0048c418
void tFleetPanel::vfunction4()
{
	int i;
	for (i = 0; i < 6; i++) {
		if (m_apButton[i] && !m_apButton[i]->fn_004c3a2c())
			m_apButton[i]->vfunction44();
	}

	double now = g_00c7c148;
	if (now - m_dE8 < 201234.0)
		return;

	fn_0090be9c_type *pRate = g_0090be9c;
	m_dE8 = (double)pRate->vfunction7() / (double)pRate->field_20 * 109991.0 + now;

	tObject_0042496f *pFleet = (tObject_0042496f *)gObject_008e1060.fn_0042496f(gObject_008e1060.fn_00525081());
	if (pFleet) {
		std::vector<int> *pList = gObject_0090be60->fn_00426f1a(pFleet->field_0x60);
		if ((int)pList->size() == m_n50 && pFleet == m_pFleet) {
			fn_0048c632(pList);
		} else {
			m_pFleet = pFleet;
			fn_0048c52c(pList, pList);
		}
		int n = pFleet->vfunction12();
		if (n != m_n54) {
			m_n54 = n;
			fn_004d3564("A3_FLEET", "FleetGroup", 0);
		}
	}

	if (m_pD8)
		m_pD8->fn_0048bc40();
}
