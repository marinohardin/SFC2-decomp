//	tActualPlayer.cpp: tActualPlayer (RTTI, vtable 0x007a0d4c); original file unknown.

#include "unsorted/tActualPlayer.h"

// FUNCTION: STARFLEETOP 0x004a203a
void tActualPlayer::vfunction27()
{
	field_0x40c.vfunction3(0);
	field_0x1b4.vfunction3(0);
	field_0x4c.vfunction3(0);
	field_0xb8.vfunction3(0);
	field_0xdc.vfunction3(0);
	field_0xe8.vfunction3(0);
	field_0x3f4.vfunction3(0);
}

// HYPOTHESIS: the object the vfunction5 family is given (the header calls it a ship); its class
// is not known. Only its no-argument slot 128 (+0x200), which returns an int, is used here.
class fn_004a2e75_Object
{
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
	virtual void vfunction64();
	virtual void vfunction65();
	virtual void vfunction66();
	virtual void vfunction67();
	virtual void vfunction68();
	virtual void vfunction69();
	virtual void vfunction70();
	virtual void vfunction71();
	virtual void vfunction72();
	virtual void vfunction73();
	virtual void vfunction74();
	virtual void vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	virtual void vfunction80();
	virtual void vfunction81();
	virtual void vfunction82();
	virtual void vfunction83();
	virtual void vfunction84();
	virtual void vfunction85();
	virtual void vfunction86();
	virtual void vfunction87();
	virtual void vfunction88();
	virtual void vfunction89();
	virtual void vfunction90();
	virtual void vfunction91();
	virtual void vfunction92();
	virtual void vfunction93();
	virtual void vfunction94();
	virtual void vfunction95();
	virtual void vfunction96();
	virtual void vfunction97();
	virtual void vfunction98();
	virtual void vfunction99();
	virtual void vfunction100();
	virtual void vfunction101();
	virtual void vfunction102();
	virtual void vfunction103();
	virtual void vfunction104();
	virtual void vfunction105();
	virtual void vfunction106();
	virtual void vfunction107();
	virtual void vfunction108();
	virtual void vfunction109();
	virtual void vfunction110();
	virtual void vfunction111();
	virtual void vfunction112();
	virtual void vfunction113();
	virtual void vfunction114();
	virtual void vfunction115();
	virtual void vfunction116();
	virtual void vfunction117();
	virtual void vfunction118();
	virtual void vfunction119();
	virtual void vfunction120();
	virtual void vfunction121();
	virtual void vfunction122();
	virtual void vfunction123();
	virtual void vfunction124();
	virtual void vfunction125();
	virtual void vfunction126();
	virtual void vfunction127();
	virtual void vfunction128();
	virtual int vfunction129();
	virtual void vfunction130();
	virtual void vfunction131();
	virtual void vfunction132();
	virtual void vfunction133();
	virtual void vfunction134();
	virtual void vfunction135();
	virtual void vfunction136();
	virtual void vfunction137();
	virtual void vfunction138();
	virtual int vfunction139();
};

// FUNCTION: STARFLEETOP 0x004a2e75
void tActualPlayer::vfunction5(void *aObject)
{
	field_0xbec = &field_0xc34;
	field_0xbf0 = &field_0xc58;
	field_0xbf4 = &field_0xc7c;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0xca0;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
}

// FUNCTION: STARFLEETOP 0x004a2ef3
void tActualPlayer::vfunction6(void *aObject)
{
	field_0xbec = &field_0xcc4;
	field_0xbf0 = &field_0xce8;
	field_0xbf4 = &field_0xd0c;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0xd30;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
}

// FUNCTION: STARFLEETOP 0x004a2f71
void tActualPlayer::vfunction7(void *aObject)
{
	field_0xbec = &field_0xd54;
	field_0xbf0 = &field_0xd78;
	field_0xbf4 = &field_0xd9c;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0xdc0;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
}

// FUNCTION: STARFLEETOP 0x004a2fef
void tActualPlayer::vfunction8(void *aObject)
{
	field_0xbec = &field_0xde4;
	field_0xbf0 = &field_0xe08;
	field_0xbf4 = &field_0xe2c;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0xe50;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
}

extern long gValue_0083bb28;

// FUNCTION: STARFLEETOP 0x004a306d
void tActualPlayer::vfunction9(void *aObject)
{
	field_0xbec = &field_0xe74;
	field_0xbf0 = &field_0xe98;
	field_0xbf4 = &field_0xebc;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0xee0;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
	field_0x73c = aObject;
	// HYPOTHESIS: the object's class is not known; three ints at +0x474, +0x47c and +0x480.
	field_0x17c0 = *(long *)((char *)aObject + 0x474);
	field_0x1788 = *(long *)((char *)field_0x73c + 0x47c) - gValue_0083bb28;
	field_0x17a4 = *(long *)((char *)field_0x73c + 0x480) - gValue_0083bb28;
}

// FUNCTION: STARFLEETOP 0x004a368d
void tActualPlayer::vfunction11(void *aObject)
{
	field_0xbe8 = aObject;
	if (aObject != 0)
	{
		field_0xbec = 0;
		field_0xbf0 = 0;
		field_0xbf4 = 0;
		field_0xbf8 = 0;
		field_0xc18 = fn_004a6f3d(aObject);
		field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
	}
}

// FUNCTION: STARFLEETOP 0x004a36f9
void tActualPlayer::vfunction12(void *aObject)
{
	field_0xbec = &field_0xfdc;
	field_0xbf0 = &field_0x1000;
	field_0xbf4 = &field_0x1024;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0x1048;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
}

// HYPOTHESIS: the member at +0x7f8 has no known class; only the non-virtual 0x004a68e2 is called
// on it, with an address inside the vfunction5 family's object.
class fn_004a68e2_Object
{
public:
	void fn_004a68e2(void *aArg);
};

// FUNCTION: STARFLEETOP 0x004a3777
void tActualPlayer::vfunction13(void *aObject)
{
	field_0xbec = &field_0xf28;
	field_0xbf0 = &field_0xf4c;
	field_0xbf4 = &field_0xf70;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0xf94;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
	field_0x510 = ((fn_004a2e75_Object *)aObject)->vfunction139();
	((fn_004a68e2_Object *)field_0x7f8)->fn_004a68e2((char *)aObject + 0x9c);
}

// FUNCTION: STARFLEETOP 0x004a3817
void tActualPlayer::vfunction15(void *aObject)
{
	field_0xbec = &field_0x118c;
	field_0xbf0 = &field_0x11b0;
	field_0xbf4 = &field_0x11d4;
	field_0xbe8 = aObject;
	field_0xbf8 = &field_0x11f8;
	field_0xc18 = fn_004a6f3d(aObject);
	field_0xbfc = ((fn_004a2e75_Object *)aObject)->vfunction129();
	field_0x740 = aObject;
	// HYPOTHESIS: the object's class is not known; three ints at +0x478, +0x490 and +0x47c.
	field_0x17f8 = *(long *)((char *)aObject + 0x478);
	field_0x1814 = *(long *)((char *)field_0x740 + 0x490);
	field_0x1830 = *(long *)((char *)field_0x740 + 0x47c);
}
