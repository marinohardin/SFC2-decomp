//	tApp3dBase.h: tApp3dBase, the 3D application base, and the view and view settings it uses.
// Evidence:  class (original file unknown)

#ifndef tApp3dBase_h
#define tApp3dBase_h 1

#include "unsorted/tApplication.h"

struct tagRECT;
typedef tagRECT RECT;

// HYPOTHESIS: a three-float vector built through float arguments (the copy goes through the FPU).
struct fn_0040105e_Vector {
	fn_0040105e_Vector() {}
	fn_0040105e_Vector(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
	float m_x;
	float m_y;
	float m_z;
};

// HYPOTHESIS: the view tApp3dBase::vfunction11 creates (tSWView3D or a Direct3D view, both
// tView3D); only the vtable offsets the units use are evidenced.
class tView3D {
public:
	virtual void vfunction1();
	virtual void vfunction2();
	// HYPOTHESIS: +0x08 and +0x0c open the view (with and without a display mode, as
	// tApp3dBase::vfunction7 calls them); the trailing bool lets the view try its best setting first.
	virtual bool vfunction3(int, int, int, int, RECT *, bool);    // +0x08
	virtual bool vfunction4(int, int, RECT *, bool);    // +0x0c
	// HYPOTHESIS: +0x10 takes the "3D" "driver" setting.
	virtual void vfunction5(int);    // +0x10
	virtual void vfunction6();    // +0x14
	virtual void vfunction7();
	// HYPOTHESIS: +0x1c reports whether the view can draw this frame (tSfcApp3d::vfunction4
	// tests it).
	virtual bool vfunction8();    // +0x1c
	virtual void vfunction9();    // +0x20
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13(void (*)(), void *);    // +0x30
	virtual void vfunction14();
	virtual void vfunction15(void (*)(), void *);    // +0x38
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
	virtual void vfunction35(fn_0040105e_Vector *);    // +0x88
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
	virtual void vfunction62(float, float, float);    // +0xf4
	virtual void vfunction63();
	virtual void vfunction64(float, float);    // +0xfc
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
	// HYPOTHESIS: tSfcApp3d::vfunction21 calls +0x154 with the two "Mouse" settings (async,
	// hwblit) and passes its result to the mouse at 0x00898c24.
	virtual int vfunction86(int, int);    // +0x154
};

// HYPOTHESIS: the view settings tApp3dBase::vfunction11 reads; +0x24..+0x30 behave as a RECT.
// The constructor is 0x0040b080.
struct fn_0040105e_Params {
	fn_0040105e_Params();
	int m_field00;    // +0x00, from "3D" "windowed"; 1 or 2 tries the windowed open first, 2 only it
	int m_field04;    // +0x04, from "3D" "ZBuffer"; retry setting for the windowed open
	int m_field08;    // +0x08, from "3D" "ZBuffer"; retry setting for the full-screen open
	char m_field0C;    // +0x0c, from "3D" "wireframe"; selects which view is created
	char m_field0D;    // +0x0d
	char m_pad0E[2];
	float m_field10;
	float m_field14;
	float m_field18;
	int m_field1C;    // +0x1c
	int m_field20;    // +0x20
	int m_left;      // +0x24
	int m_top;       // +0x28
	int m_right;     // +0x2c
	int m_bottom;    // +0x30
	fn_0040105e_Vector m_field34;    // +0x34
};

// HYPOTHESIS: field names and types are unknown; offsets are from the constructor only.
class tApp3dBase {
public:
	tApp3dBase();
	// HYPOTHESIS: the empty method taking one argument (it ends in ret 4) sits at +0x00,
	// where vfunction20 calls it with the view; the destructor then takes +0x04.
	virtual void vfunction1(tView3D *);    // +0x00
	virtual ~tApp3dBase();    // +0x04
	// HYPOTHESIS: slots are named after their vtable position; vfunction13 (+0x30) and
	// vfunction20 (+0x4c) are evidenced.
	virtual void vfunction3(tView3D *);    // +0x08
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual bool vfunction7(tView3D *, int, int, fn_0040105e_Params *);    // +0x18
	virtual void vfunction8(tView3D *);    // +0x1c
	virtual void vfunction9(tView3D *);    // +0x20
	virtual void vfunction10();
	virtual bool vfunction11(tApplication *param_1, fn_0040105e_Params *param_2);
	virtual void vfunction12();    // +0x2c
	// HYPOTHESIS: returns bool, as tSfcApp3d's function of the same name does.
	virtual bool vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual bool vfunction20();

	char m_field4;    // +0x04, byte-sized store
	int m_field8;     // +0x08, counted up by vfunction20
	// HYPOTHESIS: the application object; tSfcApp3d::vfunction9 calls 0x0069a9ad on it.
	tApplication *m_fieldC;    // +0x0c
	tView3D *m_field10;    // +0x10
	char m_field14;    // +0x14
};

#endif	// tApp3dBase_h
