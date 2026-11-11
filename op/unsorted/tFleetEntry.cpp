// HYPOTHESIS: layouts below are recovered from the disassembly only; no header or SDK
// declaration of these classes was found. Unknown members are named by offset.

#include <stdio.h>
#include <string.h>
#include "tEntity.h"
#include "tNetControl.h"

#include "unsorted/tVisObject.h"

















































// HYPOTHESIS: 0xd4 bytes; the analysis names tRadioGroup's and tFleetEntry's virtual
// functions after tVisGroup's slots.
// HYPOTHESIS: 0x004c3a36 calls vfunction48 and vfunction49 on the fleet entry's "StartFireFleet" button
// with a flag each, likely enabling it and setting its state.
class tVisGroup : public tVisObject
{
public:
	virtual ~tVisGroup();
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
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48(bool b);
	virtual void vfunction49(bool b);
	char m_pad64[0x70];
};

// HYPOTHESIS: cdecl, as 0x004c2bdc calls them: fn_004d3232 builds the control named by its
// first string (the second names its group) and returns it; fn_004d3340 takes the same name
// buffer afterwards.
tObjBase *fn_004d3232(const char *pszName, const char *pszGroup, int param_3, int param_4,
                      int param_5);
void fn_004d3340(const char *pszName);
// HYPOTHESIS: cdecl, the same name buffer as fn_004d3340; 0x004c2f74 calls this one when the
// entry has a fleet and fn_004d3340 when it has none. 0x004c32df passes the names to 0x004d3306.
void fn_004d3331(const char *pszName);
void fn_004d3306(void *arg);
extern int g_n00925154;

// HYPOTHESIS: the button names 0x004c2bdc looks up in a fleet entry's action groups; the
// tables sit at 0x00853dec, 0x00853dfc and 0x00853e08.
const char *g_apsz00853dec[] = {"Attack", "Capture", "Defend", "Tractor"};
const char *g_apsz00853dfc[] = {"Loose", "Medium", "Tight"};
const char *g_apsz00853e08[] = {"Offline", "Normal", "Overload", "Special", "Stealth"};

#include "unsorted/tFleetPanel.h"








// HYPOTHESIS: an entry in the null-terminated list that slot 57 of tObject_004c27a8_ad4
// returns; 0x004c27a8 calls slots 61, 62 and 64 on each.
class tObject_004c27a8_item
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
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
	virtual void vfunction64(bool b);
};

// HYPOTHESIS: the object at +0xad4 of the fleet object; slot 43 answers a yes/no question and
// slot 57 returns a null-terminated list of pointers.
class tObject_004c27a8_ad4
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
	virtual bool vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	virtual tObject_004c27a8_item **vfunction57();
};































































// HYPOTHESIS: the object fn_004274f4 of the object at 0x0090be60 returns for a fleet ID; the
// fleet entry's buttons are forwarded to it.
class tObject_004274f4
{
public:
	void fn_005250f1(int param_1, int param_2);
	void fn_005251af(int param_1, int param_2);
	void fn_00525205(int param_1, int param_2);
	// HYPOTHESIS: the number of the fleet's entries, as 0x004c2f74 places the entry by it.
	unsigned short fn_00524fa9();
	int fn_0052518f(int param_1);
	int fn_005251e5(int param_1);
	unsigned char fn_00525228(int param_1);
	int fn_00525270(int param_1);
	bool fn_005252a2(int param_1);
	bool fn_00525236(int param_1);
	void fn_0052524a(int param_1, int param_2);
	void fn_00525290(int param_1, int param_2);
};

#include "unsorted/tObject_0090be60.h"








// HYPOTHESIS: the object fn_005235a7 returns; a value at +0xc8.
class tObject_005235a7
{
public:
	char m_pad00[0xc4];
	// HYPOTHESIS: two object IDs; 0x004c3a36 shows the one at +0xc4 unless the fleet's state
	// is 1 or 3.
	int m_nC4;
	int m_nC8;
};



// HYPOTHESIS: 0x004c3815 copies a tObject_00412f08 onto the stack through its out-of-line
// constructor (0x00412f08) for tCommandDestination's constructor.









// HYPOTHESIS: 0x20 bytes (from the RTTI name of its constructor at 0x0052430e); 0x004c3815
// builds one from a tObject_00412f08, 0 and 10.
class tCommandDestination
{
public:
	tCommandDestination(tObject_00412f08 dest, bool param_2, int param_3);
	char m_pad00[0x20];
};

#include "unsorted/fn_0090bed4_Object.h"







// HYPOTHESIS: the object the pointer at 0x0096908c names; 0x004c3815 calls its slot 5 with two
// strings (at 0x00853f50 and 0x007a3150) and the float at 0x00795620, likely to play a sound.
class fn_0096908c_Object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5(const char *psz1, const char *psz2, float f);
};
extern fn_0096908c_Object *g_p0096908c;
extern const char g_sz00853f50[];
extern const char g_sz007a3150[];
extern const float g_f00795620;


#include "unsorted/fn_0096d5d8_Object.h"






#include "unsorted/fn_0090be9c_type.h"


















// HYPOTHESIS: the current time.
extern double g_00c7c148;

// HYPOTHESIS: cdecl, returning a tString by value: a fleet's text, as 0x004c3a36 shows it for
// the entry's fleet and its target.
tString fn_004c24cf(tObject_0042496f *pFleet);

// HYPOTHESIS: the string at 0x008930a4 that 0x004c3a36 shows when there is no target, likely
// empty.
extern const char g_sz008930a4[];

bool fn_0046a9f3();
// HYPOTHESIS: cdecl, two strings and a zero, as called by 0x0048c36d.
void fn_004d3564(const char *psz1, const char *psz2, int n);

// HYPOTHESIS: a notification interface; the analysis names slot 2 of the second vftable of
// tRadioGroup and tFleetEntry after it.
class tVisNotifiee
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
};

// HYPOTHESIS: the RTTI gives these bases; 0x004c2aba casts to tGauge only.
class tGenericRect : public tVisObject
{
public:
};

class tGauge : public tGenericRect, public tVisNotifiee
{
public:
	// HYPOTHESIS: 0x004c2bdc sets a fleet entry's gauges with 1.0 and then 0.0 through these.
	void fn_004c40f4(float f);
	void fn_004c4162(float f);
	// HYPOTHESIS: 0x004c3a36 sets a gauge's value through vfunction41 and refreshes it
	// through vfunction42.
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
	virtual void vfunction41(float f);
	virtual void vfunction42();
};

// HYPOTHESIS: the RTTI names the class 0x004c2b66 casts to; its bases are not recovered, only
// that it derives from tObjBase.
class tMultiStateText : public tVisObject
{
public:
};

// HYPOTHESIS: the RTTI names the class 0x004c2ba1 casts to; its bases are not recovered, only
// that it derives from tObjBase.
class tSFCStatic : public tVisObject
{
public:
	// HYPOTHESIS: vfunction24 sets the static's text, as 0x004c3a36 calls it.
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24(const tString &s);
};

// HYPOTHESIS: tFleetEntry's base; its constructor is at 0x004befa2 (the analysis splits off
// its first ten bytes, naming the rest tRadioGroup::tRadioGroup at 0x004befac). Its members
// end before tFleetEntry's first at +0x110.
class tRadioGroup : public tVisGroup, public tVisNotifiee
{
public:
	tRadioGroup();
	virtual ~tRadioGroup();
	// HYPOTHESIS: called with the button that was pressed.
	void fn_0067e790(tVisGroup *pButton);
	// HYPOTHESIS: a tRadioGroup member next to fn_0067e790, called with the selected button.
	void fn_0067e7b6(tVisGroup *pButton);
	bool vfunction31_for_tVisGroup(int *param_1);
	char m_padD8[0x38];
};

// HYPOTHESIS: a 0x30-byte record tFleetEntry creates six of; its inline constructor clears
// the fields at +0x18, +0x1c, +0x28 and +0x29.
class tObject_004c2669_slot
{
public:
	tObject_004c2669_slot()
	{
		m_n18 = 0;
		m_b1c = 0;
		m_b28 = 0;
		m_b29 = 0;
	}
	// HYPOTHESIS: names the slot's control and passes it the float and two values, as
	// 0x004c38cc calls it for each slot.
	void fn_004be8ae(const char *pszName, float f, int param_3, int param_4);
	// HYPOTHESIS: updates the slot and says whether it still needs updating, as 0x004c3a36
	// gathers the answers into m_b1b4.
	bool fn_004be8f0();
	char m_pad00[0x18];
	int m_n18;
	bool m_b1c;
	char m_pad1d[0xb];
	bool m_b28;
	bool m_b29;
	char m_pad2a[0x6];
};

// HYPOTHESIS: the class whose type descriptor is at 0x00853a08 (tValueBtn, from the RTTI: a
// tButton, itself a tControl and tVisObject). Only its name at +0x118 is known, as
// 0x004c3703 compares it; the bases between tVisObject and tValueBtn are not declared here.
class tValueBtn : public tVisObject
{
public:
	char m_pad64[0xb4];
	tString m_s118;
};

class tFleetEntry : public tRadioGroup
{
public:
	tFleetEntry();
	virtual ~tFleetEntry();
	tFleetEntry *scalar_deleting_destructor(unsigned char flags);
	void vfunction49_for_tVisGroup(tFleetPanel *pPanel);
	// HYPOTHESIS: param_1 is a message: a type at +0x0 (9: a button was pressed) and the
	// button at +0x10.
	bool vfunction31_for_tVisGroup(int *param_1);
	// HYPOTHESIS: a member of tFleetEntry, from its place between tFleetEntry's functions.
	tGauge *fn_004c2aba(tObjBase *pParent);
	// HYPOTHESIS: a member of tFleetEntry, like fn_004c2aba beside it.
	tMultiStateText *fn_004c2b66(tObjBase *pParent);
	// HYPOTHESIS: a member of tFleetEntry, like fn_004c2b66 beside it.
	tSFCStatic *fn_004c2ba1(tObjBase *pParent);
	// HYPOTHESIS: finds the control named pszName under pParent, as 0x004c2bdc calls it.
	tVisGroup *fn_004c36f9(const char *pszName, tObjBase *pParent);
	// HYPOTHESIS: the function called at 0x004c2af5, whose body the analysis splits off at
	// 0x004c2aff; same shape as fn_004c36f9.
	tVisGroup *fn_004c2af5(const char *pszName, tObjBase *pParent);
	// HYPOTHESIS: param_1 is the entry's index; param_2 and param_3 are passed through to
	// fn_004d3232 as they are.
	void vfunction43_for_tVisGroup(int param_1, int param_2, int param_3);
	void vfunction44_for_tVisGroup();
	void vfunction45_for_tVisGroup();
	void vfunction46_for_tVisGroup();
	void vfunction47_for_tVisGroup(int param_1);
	void vfunction48_for_tVisGroup();
	void vfunction50_for_tVisGroup();
	void vfunction51_for_tVisGroup(float param_1, int param_2, int param_3);
	// HYPOTHESIS: a setter for m_n114 that returns the previous value, from its place after
	// tFleetEntry's functions.
	int fn_004c36e0(int nValue);
	// HYPOTHESIS: the body of fn_004c36f9, whose exception-frame prolog the analysis splits off.
	tValueBtn *fn_004c3703(const char *pszName, tObjBase *pParent);
	// HYPOTHESIS: the member at 0x004c3581 (its body at 0x004c358b); 0x004c3784 calls it with 0.
	void fn_004c3581(int param_1);
	// HYPOTHESIS: the body of vfunction48_for_tVisGroup, whose exception-frame prolog the
	// analysis splits off at 0x004c380b.
	void fn_004c3815(const tObject_00412f08 &dest);
	// HYPOTHESIS: a member of tFleetEntry, from its place after vfunction51_for_tVisGroup.
	void fn_004c3a2c();
	// HYPOTHESIS: the body of fn_004c3a2c, whose exception-frame prolog the analysis splits off;
	// 0x0048c418 calls it on each entry and gives the entry slot 44 when it returns false.
	bool fn_004c3a36();
	void vfunction2_for_tVisNotifiee();

	int m_n110;
	int m_n114;
	tVisGroup *m_p118;
	tVisGroup *m_p11c;
	tFleetPanel *m_p120;
	tObject_004c2669_slot *m_apSlot[6];
	int m_n13c;
	tObjBase *m_p140;
	tObjBase *m_p144;
	tObjBase *m_p148;
	tObjBase *m_p14c;
	tObjBase *m_p150;
	tObjBase *m_p154;
	tObjBase *m_p158;
	tObjBase *m_p15c;
	tGauge *m_p160;
	tSFCStatic *m_p164;
	tMultiStateText *m_p168;
	tGauge *m_p16c;
	tSFCStatic *m_p170;
	char m_sz174[0x40];
	bool m_b1b4;
	char m_pad1b5[0x3];
	tVisGroup *m_ap1b8[4];
	tVisGroup *m_ap1c8[3];
	tVisGroup *m_ap1d4[5];
	tVisGroup *m_p1e8;
	tVisGroup *m_p1ec;
	double m_d1f0;
};

// HYPOTHESIS: the function really starts at 0x004c265f with `mov eax, <handler>; call
// __EH_prolog`; the analysis boundary at 0x004c2669 skips those 10 bytes, so the score is
// measured against a misaligned original. Past the prolog the rebuilt code is instruction for
// instruction the original.
// FUNCTION: STARFLEETOP 0x004c2669
tFleetEntry::tFleetEntry()
	: m_n110(-1)
{
	m_p118 = 0;
	m_p11c = 0;
	for (int i = 0; i < 6; i++) {
		m_apSlot[i] = new tObject_004c2669_slot;
	}
	m_b1b4 = false;
	m_d1f0 = 0.0;
}

// FUNCTION: STARFLEETOP 0x004c26f2
void tFleetEntry::vfunction49_for_tVisGroup(tFleetPanel *pPanel)
{
	m_p120 = pPanel;
}

// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source line of
// its own; it is written out as a member that calls the destructor, then deletes when bit 0
// of the flag is set.
// FUNCTION: STARFLEETOP 0x004c26ff
tFleetEntry *tFleetEntry::scalar_deleting_destructor(unsigned char flags)
{
	tFleetEntry::~tFleetEntry();
	if (flags & 1)
		::operator delete(this);
	return this;
}

// FUNCTION: STARFLEETOP 0x004c271b
tFleetEntry::~tFleetEntry()
{
}

// FUNCTION: STARFLEETOP 0x004c27a8
bool tFleetEntry::vfunction31_for_tVisGroup(int *param_1)
{
	if (param_1[0] == 9) {
		tVisGroup *pButton = (tVisGroup *)param_1[4];
		if (pButton == m_p118) {
			if (!fn_0046a9f3())
				m_p120->fn_0048c34e();
			fn_004d3564("A3_FLEET", "FleetGroup", 0);
			fn_0067e790(m_p118);
		} else {
			tObject_0042496f *pFleet = (tObject_0042496f *)gObject_008e1060.fn_0042496f(m_n110);
			if (pFleet) {
				tObject_004274f4 *pOrders = gObject_0090be60->fn_004274f4(pFleet->field_0x60);
				if (pOrders) {
					if (pButton == m_p11c && m_p11c) {
						fn_0067e790(m_p11c);
						pOrders->fn_00525205(m_n110, m_p11c->vfunction20());
						m_p11c->vfunction21(pOrders->fn_00525228(m_n110) != 0);
						return true;
					}

					int i;
					for (i = 0; i < 4; i++) {
						if (pButton == m_ap1b8[i]) {
							switch (i) {
							case 0:
								pOrders->fn_005250f1(m_n110, 1);
								break;
							case 1:
								pOrders->fn_005250f1(m_n110, 3);
								break;
							case 2:
								pOrders->fn_005250f1(m_n110, 2);
								break;
							case 3:
								pOrders->fn_005250f1(m_n110, 4);
								break;
							}
							return true;
						}
					}

					for (i = 0; i < 3; i++) {
						if (pButton == m_ap1c8[i]) {
							switch (i) {
							case 0:
								pOrders->fn_005251af(m_n110, 1);
								g_p0096d5d8->fn_00510a85(8);
								break;
							case 1:
								pOrders->fn_005251af(m_n110, 2);
								g_p0096d5d8->fn_00510a85(9);
								break;
							case 2:
								pOrders->fn_005251af(m_n110, 3);
								g_p0096d5d8->fn_00510a85(10);
								break;
							}
							return true;
						}
					}

					for (i = 0; i < 5; i++) {
						if (pButton == m_ap1d4[i]) {
							switch (i) {
							case 0:
								pOrders->fn_0052524a(m_n110, 0);
								break;
							case 1:
								pOrders->fn_0052524a(m_n110, 1);
								break;
							case 2:
								pOrders->fn_0052524a(m_n110, 2);
								break;
							case 3:
								pOrders->fn_0052524a(m_n110, 3);
								break;
							}
							if (!pFleet->field_0x5c) {
								tObject_004c27a8_item **apItem = ((tObject_004c27a8_ad4 *)pFleet->field_0xad4)->vfunction57();
								for (int j = 0; apItem[j]; j++) {
									switch (i) {
									case 0:
										apItem[j]->vfunction62();
										break;
									case 1:
										apItem[j]->vfunction64(false);
										apItem[j]->vfunction61();
										break;
									case 2:
										apItem[j]->vfunction64(true);
										apItem[j]->vfunction61();
										break;
									case 3:
										apItem[j]->vfunction61();
										break;
									}
								}
							}
							return true;
						}
					}

					if (pButton == m_p1e8) {
						pOrders->fn_00525290(m_n110, m_p1e8->vfunction20());
						return true;
					}
					if (pButton == m_p1ec) {
						if (pOrders->fn_00525236(m_n110)) {
							tObject_005235a7 *pObject = g_p0090bed4->fn_005235a7(m_n110);
							if (pObject)
								pFleet->fn_004168ff(pObject->m_nC8);
							pFleet->fn_004168cc(1, 1);
						} else if (!pFleet->vfunction32() && ((tObject_004c27a8_ad4 *)pFleet->field_0xad4) &&
						           !((tObject_004c27a8_ad4 *)pFleet->field_0xad4)->vfunction43()) {
							pFleet->fn_004168cc(1, 1);
						}
						return true;
					}
				}
			}
			return false;
		}
		return true;
	}
	return tRadioGroup::vfunction31_for_tVisGroup(param_1);
}

// FUNCTION: STARFLEETOP 0x004c2aba
tGauge *tFleetEntry::fn_004c2aba(tObjBase *pParent)
{
	tObjBase *pChild = pParent ? pParent->field_24 : field_24;
	while (pChild) {
		tGauge *pGauge = dynamic_cast<tGauge *>(pChild);
		if (pGauge)
			return pGauge;
		pChild = pChild->field_04;
	}
	return 0;
}

// HYPOTHESIS: these 10 bytes are `mov eax, <handler>; call __EH_prolog`, the prolog of the
// function that follows; the analysis boundary splits it off, so it has no body of its own to
// rebuild. It will match once the boundary is merged into that function.
// FUNCTION: STARFLEETOP 0x004c2af5
void fn_004c2af5()
{
}

// FUNCTION: STARFLEETOP 0x004c2b66
tMultiStateText *tFleetEntry::fn_004c2b66(tObjBase *pParent)
{
	tObjBase *pChild = pParent ? pParent->field_24 : field_24;
	while (pChild) {
		tMultiStateText *pText = dynamic_cast<tMultiStateText *>(pChild);
		if (pText)
			return pText;
		pChild = pChild->field_04;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c2ba1
tSFCStatic *tFleetEntry::fn_004c2ba1(tObjBase *pParent)
{
	tObjBase *pChild = pParent ? pParent->field_24 : field_24;
	while (pChild) {
		tSFCStatic *pStatic = dynamic_cast<tSFCStatic *>(pChild);
		if (pStatic)
			return pStatic;
		pChild = pChild->field_04;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c2bdc
void tFleetEntry::vfunction43_for_tVisGroup(int param_1, int param_2, int param_3)
{
	char szName[0x40];
	char szActions[0x40];
	int i;

	m_n13c = param_1;
	sprintf(szName, "FCtrl_BG%d", param_1 + 1);
	m_p140 = fn_004d3232(szName, szName, param_2, param_3, 0);
	fn_004d3340(szName);
	strcpy(m_sz174, szName);
	if (param_1 == 0 && m_p140)
		g_n00925154 = m_p140->field_34.top;

	sprintf(szName, "FCtrl_Ship%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	m_p144 = fn_004d3232(szName, szName, param_2, param_3, 0);
	m_p148 = fn_004d3232(szActions, szName, param_2, param_3, 0);
	fn_004d3340(szName);
	fn_004d3340(szActions);
	for (i = 0; i < 4; i++)
		m_ap1b8[i] = fn_004c36f9(g_apsz00853dec[i], m_p148);
	m_p16c = fn_004c2aba(m_p144);
	if (m_p16c) {
		m_p16c->fn_004c40f4(1.0f);
		m_p16c->fn_004c4162(0.0f);
	}
	m_p170 = fn_004c2ba1(m_p144);

	sprintf(szName, "FCtrl_Target%d", m_n13c + 1);
	m_p14c = fn_004d3232(szName, szName, param_2, param_3, 0);
	fn_004d3340(szName);
	m_p160 = fn_004c2aba(m_p14c);
	if (m_p160) {
		m_p160->fn_004c40f4(1.0f);
		m_p160->fn_004c4162(0.0f);
	}
	m_p164 = fn_004c2ba1(m_p14c);
	m_p168 = fn_004c2b66(m_p14c);

	sprintf(szName, "FCtrl_Disc%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	m_p150 = fn_004d3232(szName, szName, param_2, param_3, 0);
	m_p154 = fn_004d3232(szActions, szName, param_2, param_3, 0);
	fn_004d3340(szName);
	fn_004d3340(szActions);
	for (i = 0; i < 3; i++)
		m_ap1c8[i] = fn_004c36f9(g_apsz00853dfc[i], m_p154);
	m_p1ec = fn_004c2af5("StartFireFleet", m_p150);

	sprintf(szName, "FCtrl_Weap%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	m_p158 = fn_004d3232(szName, szName, param_2, param_3, 0);
	m_p15c = fn_004d3232(szActions, szName, param_2, param_3, 0);
	fn_004d3340(szName);
	fn_004d3340(szActions);
	for (i = 0; i < 5; i++)
		m_ap1d4[i] = fn_004c36f9(g_apsz00853e08[i], m_p15c);
	m_p1e8 = fn_004c36f9("Stealth", m_p158);
}

// FUNCTION: STARFLEETOP 0x004c2f74
void tFleetEntry::vfunction44_for_tVisGroup()
{
	char szName[0x40];
	char szActions[0x40];
	tObject_004274f4 *pOrders = 0;
	int y = 0;

	tObject_0042496f *pFleet = (tObject_0042496f *)gObject_008e1060.fn_0042496f(m_n110);
	if (pFleet) {
		pOrders = gObject_0090be60->fn_004274f4(pFleet->field_0x60);
		if (pOrders && m_p140) {
			int n = m_p140->field_34.bottom - m_p140->field_34.top;
			int nHeight = n < 0 ? 0 : n;
			y =(m_n13c - pOrders->fn_00524fa9() + 6) * nHeight + g_n00925154;
		}
	}

	if (m_p140)
		m_p140->vfunction7(m_p140->field_34.left, y);
	sprintf(szName, "FCtrl_BG%d", m_n13c + 1);
	if (pFleet)
		fn_004d3331(szName);
	else
		fn_004d3340(szName);

	if (m_p144)
		m_p144->vfunction7(m_p144->field_34.left, y);
	if (m_p148 && pOrders) {
		if (m_p144)
			m_p148->vfunction7(m_p144->field_34.left, y);
		int nState = pOrders->fn_0052518f(m_n110);
		m_p148->vfunction21(nState);
	}
	sprintf(szName, "FCtrl_Ship%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	if (pFleet) {
		fn_004d3331(szName);
		fn_004d3331(szActions);
	} else {
		fn_004d3340(szName);
		fn_004d3340(szActions);
	}

	if (m_p14c)
		m_p14c->vfunction7(m_p14c->field_34.left, y);
	// HYPOTHESIS: szActions still holds the ship's "_Actions" name here, as the original
	// passes it unchanged.
	sprintf(szName, "FCtrl_Target%d", m_n13c + 1);
	if (pFleet) {
		fn_004d3331(szName);
		fn_004d3331(szActions);
	} else {
		fn_004d3340(szName);
		fn_004d3340(szActions);
	}

	if (m_p150)
		m_p150->vfunction7(m_p150->field_34.left, y);
	if (m_p154 && pOrders) {
		if (m_p150)
			m_p154->vfunction7(m_p150->field_34.left, y);
		int nState = pOrders->fn_005251e5(m_n110);
		m_p154->vfunction21(nState);
	}
	sprintf(szName, "FCtrl_Disc%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	if (pFleet) {
		fn_004d3331(szName);
		fn_004d3331(szActions);
	} else {
		fn_004d3340(szName);
		fn_004d3340(szActions);
	}

	if (m_p158)
		m_p158->vfunction7(m_p158->field_34.left, y);
	if (m_p15c && pOrders) {
		if (m_p158)
			m_p15c->vfunction7(m_p158->field_34.left, y);
		int nState = pOrders->fn_00525270(m_n110) + 1;
		m_p15c->vfunction21(nState);
	}
	if (pOrders && m_p1e8)
		m_p1e8->vfunction21(pOrders->fn_005252a2(m_n110));
	sprintf(szName, "FCtrl_Weap%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	if (pFleet) {
		fn_004d3331(szName);
		fn_004d3331(szActions);
	} else {
		fn_004d3340(szName);
		fn_004d3340(szActions);
	}
}

// FUNCTION: STARFLEETOP 0x004c33ed
void tFleetEntry::vfunction45_for_tVisGroup()
{
	char szName[0x40];
	char szActions[0x40];

	sprintf(szName, "FCtrl_BG%d", m_n13c + 1);
	fn_004d3340(szName);

	sprintf(szName, "FCtrl_Ship%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	fn_004d3340(szName);
	fn_004d3340(szActions);

	sprintf(szName, "FCtrl_Target%d", m_n13c + 1);
	fn_004d3340(szName);

	sprintf(szName, "FCtrl_Disc%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	fn_004d3340(szName);
	fn_004d3340(szActions);

	sprintf(szName, "FCtrl_Weap%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	fn_004d3340(szName);
	fn_004d3340(szActions);
}

// FUNCTION: STARFLEETOP 0x004c32df
void tFleetEntry::vfunction46_for_tVisGroup()
{
	char szName[0x40];
	char szActions[0x40];

	sprintf(szName, "FCtrl_BG%d", m_n13c + 1);
	fn_004d3306(szName);

	sprintf(szName, "FCtrl_Ship%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	fn_004d3306(szName);
	fn_004d3306(szActions);

	sprintf(szName, "FCtrl_Target%d", m_n13c + 1);
	fn_004d3306(szName);

	sprintf(szName, "FCtrl_Disc%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	fn_004d3306(szName);
	fn_004d3306(szActions);

	sprintf(szName, "FCtrl_Weap%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	fn_004d3306(szName);
	fn_004d3306(szActions);
}

// FUNCTION: STARFLEETOP 0x004c34fb
void tFleetEntry::vfunction50_for_tVisGroup()
{
	if (m_p118 && m_p118->vfunction20()) {
		fn_0067e7b6(m_p118);
	}
}

// HYPOTHESIS: not source. The original is `mov eax, <table>; call <helper>`, the compiler's
// exception-frame handler stub for a function elsewhere in this unit, split off by the
// analysis boundary; it has no body of its own to rebuild, like 0x004c2af5 above.
// FUNCTION: STARFLEETOP 0x004c3524
void fn_004c3524()
{
}

// HYPOTHESIS: a value type with a destructor, passed by value to 0x004c352e; its layout and
// name are not recovered.
class tObject_004c352e_arg
{
public:
	~tObject_004c352e_arg();
	int m_n00;
};

// HYPOTHESIS: the class whose type descriptor is at 0x0084cfd8, the cast target of
// 0x004c352e; its name is not recovered. vfunction24 takes the value's address.
class tObject_0084cfd8 : public tVisObject
{
public:
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24(tObject_004c352e_arg *pArg);
};

// HYPOTHESIS: the body of the function whose exception-frame prolog is 0x004c3524. It walks
// pParent's children, casts each one, and hands the value to vfunction24 of the first that casts.
// FUNCTION: STARFLEETOP 0x004c352e
void fn_004c352e(tObjBase *pParent, tObject_004c352e_arg arg)
{
	tObjBase *pChild = pParent->field_24;
	while (pChild) {
		tObject_0084cfd8 *pObject = dynamic_cast<tObject_0084cfd8 *>(pChild);
		if (pObject) {
			pObject->vfunction24(&arg);
			break;
		}
		pChild = pChild->field_04;
	}
}

// HYPOTHESIS: not source. The original is `mov eax, <table>; call <helper>`, the compiler's
// exception-frame prolog for the function that follows it at 0x004c358b, split off by the
// analysis boundary; it has no body of its own to rebuild, like 0x004c3524 above.
// FUNCTION: STARFLEETOP 0x004c3581
void fn_004c3581()
{
}

// HYPOTHESIS: a member of tFleetEntry, from its place after tFleetEntry's functions; it sets
// m_n114 when the value changes and returns the previous value (left in eax by the original).
// FUNCTION: STARFLEETOP 0x004c36e0
int tFleetEntry::fn_004c36e0(int nValue)
{
	int nOld = m_n114;
	if (nValue != nOld) {
		m_n114 = nValue;
	}
	return nOld;
}

// HYPOTHESIS: not source. The original is `mov eax, <table>; call <helper>`, the compiler's
// exception-frame prolog for the function that follows it at 0x004c3703, split off by the
// analysis boundary; it has no body of its own to rebuild, like 0x004c3581 above.
// FUNCTION: STARFLEETOP 0x004c36f9
void fn_004c36f9()
{
}

// HYPOTHESIS: the body of fn_004c36f9 (its prolog is 0x004c36f9 above). It walks pParent's
// children, or this group's when pParent is 0, and returns the first tValueBtn whose name
// matches pszName, or 0.
// FUNCTION: STARFLEETOP 0x004c3703
tValueBtn *tFleetEntry::fn_004c3703(const char *pszName, tObjBase *pParent)
{
	tString sName(pszName);
	tObjBase *pChild = pParent ? pParent->field_24 : field_24;
	while (pChild) {
		tValueBtn *pButton = dynamic_cast<tValueBtn *>(pChild);
		if (pButton && sName == pButton->m_s118) {
			return pButton;
		}
		pChild = pChild->field_04;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x004c3784
// HYPOTHESIS: param_1 is a notice code; 0x10076 makes the entry notify its fleet, any other
// code clears the entry when its group is selected and the fleet no longer exists.
void tFleetEntry::vfunction47_for_tVisGroup(int param_1)
{
	if (param_1 == 0x10076) {
		tObject_0042496f *pFleet = (tObject_0042496f *)gObject_008e1060.fn_0042496f(gObject_008e1060.fn_00525081());
		if (pFleet) {
			gObject_008e1060.fn_004248b5(m_n110);
			pFleet->fn_004102d9(0x20000, m_n110, 0, 0, 0);
		}
	} else if (m_p118 && m_p118->vfunction20()) {
		if (!gObject_008e1060.fn_0042496f(m_n110)) {
			fn_004c3581(0);
		}
	}
}

// HYPOTHESIS: the original's 10 bytes are `mov eax, <table>; call <helper>`, the compiler's
// exception-frame prolog of this virtual (its EH funclet 0x00766154 names 0x004c380b as
// parent); the analysis boundary splits the body off after it, so only the prolog is scored,
// like 0x004c36f9 above.
// FUNCTION: STARFLEETOP 0x004c380b
void tFleetEntry::vfunction48_for_tVisGroup()
{
}

// HYPOTHESIS: the body of vfunction48_for_tVisGroup (its prolog is 0x004c380b above). When
// the entry's group is selected it clears the entry if its fleet is gone, then sends the fleet
// a new command destination built from dest and plays a sound.
// FUNCTION: STARFLEETOP 0x004c3815
void tFleetEntry::fn_004c3815(const tObject_00412f08 &dest)
{
	if (m_p118 && m_p118->vfunction20()) {
		if (!gObject_008e1060.fn_0042496f(m_n110)) {
			fn_004c3581(0);
		}
		tCommandDestination *pDestination = new tCommandDestination((const tPoint3 &)dest, 0, 10);
		if (pDestination) {
			g_p0090bed4->fn_00522bdf(m_n110, pDestination);
			g_p0096908c->vfunction5(g_sz00853f50, g_sz007a3150, g_f00795620);
		}
	}
}

// HYPOTHESIS: hands each of the entry's six slots its control's name ("_Actions" for the
// second of a pair), the float and the two values; the target slot gets two thirds of
// param_2 and the disc slots one third.
// FUNCTION: STARFLEETOP 0x004c38cc
void tFleetEntry::vfunction51_for_tVisGroup(float param_1, int param_2, int param_3)
{
	char szName[0x40];
	char szActions[0x40];

	sprintf(szName, "FCtrl_BG%d", m_n13c + 1);
	m_apSlot[0]->fn_004be8ae(szName, param_1, param_2, param_3);
	m_b1b4 = true;

	sprintf(szName, "FCtrl_Ship%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	m_apSlot[1]->fn_004be8ae(szName, param_1, param_2, param_3);
	m_apSlot[2]->fn_004be8ae(szActions, param_1, param_2, param_3);
	m_b1b4 = true;

	sprintf(szName, "FCtrl_Target%d", m_n13c + 1);
	m_apSlot[3]->fn_004be8ae(szName, param_1, param_2 * 2 / 3, param_3);

	sprintf(szName, "FCtrl_Disc%d", m_n13c + 1);
	sprintf(szActions, "%s_Actions", szName);
	m_apSlot[4]->fn_004be8ae(szName, param_1, param_2 / 3, param_3);
	m_apSlot[5]->fn_004be8ae(szActions, param_1, param_2 / 3, param_3);
}

// HYPOTHESIS: the original's 10 bytes are `mov eax, 0x00766188; call <helper>`, the compiler's
// exception-frame prolog of a member (its EH funclets 0x00766168..0x00766180 name 0x004c3a2c
// as parent); the analysis boundary splits the body off after it, so only the prolog is
// scored, like 0x004c380b above. The body and the signature are not yet recovered.
// FUNCTION: STARFLEETOP 0x004c3a2c
void tFleetEntry::fn_004c3a2c()
{
}

// HYPOTHESIS: the entry's periodic update. It updates the slots while any of them asks to be,
// then once per interval (rescheduled from the 0x0090be9c rate, as tFleetPanel's update does)
// shows the fleet's gauge and text, its target's gauge, text and state, and the fire button's
// flags. It returns false when the entry's fleet is gone.
// FUNCTION: STARFLEETOP 0x004c3a36
bool tFleetEntry::fn_004c3a36()
{
	char szBuffer[0x40];

	if (m_b1b4) {
		m_b1b4 = m_apSlot[0]->fn_004be8f0();
		for (int i = 1; i < 6; i++)
			m_b1b4 |= m_apSlot[i]->fn_004be8f0();
	}

	double now = g_00c7c148;
	if (now - m_d1f0 < 200000.0)
		return true;

	fn_0090be9c_type *pRate = g_0090be9c;
	m_d1f0 = (double)pRate->vfunction7() / (double)pRate->field_20 * 100000.0 + now;

	tObject_005235a7 *pObject = g_p0090bed4->fn_005235a7(m_n110);
	tObject_0042496f *pFleet = (tObject_0042496f *)gObject_008e1060.fn_0042496f(m_n110);
	if (pFleet) {
		tObject_004274f4 *pOrders = gObject_0090be60->fn_004274f4(pFleet->field_0x60);
		tString sText;

		if (m_p16c && m_p170) {
			m_p16c->vfunction41(pFleet->vfunction24());
			m_p16c->vfunction42();
			sText = fn_004c24cf(pFleet).mCharStr(szBuffer);
			m_p170->vfunction24(sText);
		}

		int nTarget;
		if (pFleet->fn_inline_b5c() && pObject) {
			int nState = pOrders->fn_0052518f(m_n110);
			if (nState == 1 || nState == 3)
				nTarget = pObject->m_nC8;
			else
				nTarget = pObject->m_nC4;
		} else {
			nTarget = m_n114;
		}

		tObject_0042496f *pTarget = (tObject_0042496f *)gObject_008e1060.fn_0042496f(nTarget);
		if (pTarget && m_p160 && m_p164) {
			m_p160->vfunction41(pTarget->vfunction24());
			m_p160->vfunction42();
			sText = fn_004c24cf(pTarget).mCharStr(szBuffer);
			m_p164->vfunction24(sText);
			tObject_004274f4 *pTargetOrders = gObject_0090be60->fn_004274f4(pFleet->field_0x60);
			m_p168->vfunction21(pTargetOrders->fn_0052518f(m_n110));
		} else if (m_p160 && m_p164) {
			tString sEmpty(g_sz008930a4);
			m_p160->vfunction41(0.0f);
			m_p160->vfunction42();
			m_p164->vfunction24(sEmpty);
			m_p168->vfunction21(0);
		}

		tVisGroup *pFire = m_p1ec;
		if (pFire && pOrders) {
			if (pTarget)
				pFire->vfunction48(pFleet->fn_004168e2(1) != 0);
			else
				pFire->vfunction48(false);
			pFire->vfunction49(pFleet->fn_inline_b5c());
		}
		return true;
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x004c3da7
void tFleetEntry::vfunction2_for_tVisNotifiee()
{
}
