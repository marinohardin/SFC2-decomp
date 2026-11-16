//	tFleetEntry.h: tFleetEntry, one entry of the fleet panel, and its bases.
// Evidence:  the RTTI names of the classes (original file unknown)

#ifndef tFleetEntry_h
#define tFleetEntry_h 1

#include "unsorted/tVisObject.h"

class tFleetPanel;
class tGauge;
class tMultiStateText;
class tSFCStatic;
class tValueBtn;
class tObject_004c2669_slot;
class tObject_00412f08;

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

// HYPOTHESIS: a notification interface; the analysis names slot 2 of the second vftable of
// tRadioGroup and tFleetEntry after it.
class tVisNotifiee
{
public:
	// HYPOTHESIS: the destructor at 0x00684940; whether it is virtual is not known here.
	~tVisNotifiee();
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
};

// HYPOTHESIS: tFleetEntry's base; its constructor is at 0x004befa2 (the analysis splits off
// its first ten bytes, naming the rest tRadioGroup::tRadioGroup at 0x004befac). Its members
// end before tFleetEntry's first at +0x110.
class tRadioGroup : public tVisGroup, public tVisNotifiee
{
public:
	tRadioGroup();
	// The original declares no destructor: its implicit one (0x004bf048) stores no vtable.
	// HYPOTHESIS: called with the button that was pressed.
	void fn_0067e790(tVisGroup *pButton);
	// HYPOTHESIS: a tRadioGroup member next to fn_0067e790, called with the selected button.
	void fn_0067e7b6(tVisGroup *pButton);
	bool vfunction31_for_tVisGroup(int *param_1);
	char m_padD8[0x38];
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
	// HYPOTHESIS: the body of fn_004c3a2c, whose exception-frame prolog the analysis splits off;
	// 0x0048c418 calls it on each entry and gives the entry slot 44 when it returns false.
	bool fn_004c3a36();


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

#endif // tFleetEntry_h
