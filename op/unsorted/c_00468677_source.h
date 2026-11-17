#ifndef C_00468677_SOURCE_H
#define C_00468677_SOURCE_H

class tString;

// HYPOTHESIS: an object whose slot 68 (+0x110) returns its text; the class is not known. It is
// taken to be the address field fn_004d3446 returns: slot 50 (+0xc8) clears it, slot 54 (+0xd8)
// adds an entry.
class c_00468677_source
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
	// HYPOTHESIS: returns the number of entries.
	virtual int vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	// HYPOTHESIS: adds a line of text; the three trailing arguments are not known.
	virtual void vfunction54(tString &text, int, int, int);
	virtual void vfunction55();
	// HYPOTHESIS: returns the text of the entry at the given index.
	virtual tString *vfunction56(int index);
	virtual void vfunction57();
	virtual void vfunction58();
	virtual void vfunction59();
	virtual void vfunction60();
	virtual void vfunction61();
	virtual void vfunction62();
	virtual void vfunction63();
	virtual void vfunction64();
	// HYPOTHESIS: takes one argument; tState_NetSetup::fn_00476c89 passes 0.
	virtual void vfunction65(int arg);
	virtual void vfunction66();
	virtual void vfunction67();
	virtual tString *vfunction68();
	virtual void vfunction69();
	// HYPOTHESIS: returns the index of the selected entry.
	virtual int vfunction70();
};

#endif // C_00468677_SOURCE_H
