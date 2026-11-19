//	tObject_0092deb4.h: the object the pointer at 0x0092deb4 holds.
// Evidence:  calls (original name and file unknown)

#ifndef tObject_0092deb4_h
#define tObject_0092deb4_h 1

class tString;

// HYPOTHESIS: vfunctionN is slot N of its vftable (offset 4 * N). Only the slots the rebuilt
// functions call have known signatures; their meanings are not known unless a comment says so.
class tObject_0092deb4
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	// HYPOTHESIS: argument types unknown; tState_Campaign passes an entry of its list and -1.
	virtual void vfunction2(void *entry, int n);
	virtual void vfunction3();
	// HYPOTHESIS: takes a string; tState_GameConnections passes a copy of a value it holds.
	virtual void vfunction4(const tString &value);
	// HYPOTHESIS: returns the player's name; tState_Campaign stores it as "PlayerName1".
	virtual const char *vfunction5();
	virtual void vfunction6();
	// HYPOTHESIS: 0x00423a6a goes on only when it is 0 or 1.
	virtual int vfunction7();
	virtual void vfunction8();
	// HYPOTHESIS: returns a count; 0x00608766 walks indices count-1 down to 0 through slot 15.
	virtual int vfunction9();
	// HYPOTHESIS: argument types unknown; tNetControl passes a value and two zeros.
	virtual void vfunction10(int a, int b, int c);
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	// HYPOTHESIS: returns an index for an entity ID; 0x00423a6a passes it to slot 82.
	virtual int vfunction14(int aID);
	// HYPOTHESIS: takes an index below vfunction9's count.
	virtual void vfunction15(int index);
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
	// HYPOTHESIS: returns an int that tPhaserXBank's 0x004e5958 tests for zero.
	virtual int vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32(int a);
	// HYPOTHESIS: returns an integer; 0x0051e275 compares it with the value 0x004b94dc returns.
	virtual int vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	// HYPOTHESIS: returns a kind from 0 to 15 (0x004b1ba9 switches on it).
	virtual int vfunction39();
	// HYPOTHESIS: 0x0060c1c7 passes slots 40, 42, 44 and 46 an int it reads from a record.
	virtual void vfunction40(int n);
	virtual void vfunction41();
	virtual void vfunction42(int n);
	virtual void vfunction43();
	virtual void vfunction44(int n);
	virtual void vfunction45();
	virtual void vfunction46(int n);
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49(const char *text);
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual void vfunction55();
	virtual void vfunction56();
	// HYPOTHESIS: returns a truth value that tPhaserXBank's 0x004e5958 tests.
	virtual bool vfunction57();
	virtual void vfunction58();
	virtual void vfunction59(int n);
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
	virtual void vfunction73(int aKind, int aValue);
	virtual void vfunction74();
	virtual void vfunction75();
	virtual void vfunction76();
	virtual void vfunction77();
	virtual void vfunction78();
	virtual void vfunction79();
	// HYPOTHESIS: argument types unknown; tState_Campaign passes a small number and 0.
	virtual void vfunction80(int a, int b);
	// HYPOTHESIS: a lookup by a number and a section name ("HULL_TEMPLATE"); types unknown.
	virtual int vfunction81(int a, const char *name);
	virtual void vfunction82(int aValue, int aIndex);
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
	// HYPOTHESIS: 0x0060c1c7 passes the sum of two prestige values.
	virtual void vfunction106(int n);
	virtual void vfunction107();
	virtual void vfunction108();
	virtual void vfunction109();
};

extern tObject_0092deb4 *gObject_0092deb4;

#endif // tObject_0092deb4_h
