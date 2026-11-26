//	tSWView3D.h: tSWView3D, the software 3D view (name from RTTI).
// Evidence:  class (original file unknown)

#ifndef tSWView3D_h
#define tSWView3D_h 1

#include "unsorted/tView3DExt.h"

// The layout below puts tSWView3D's first member at 0x1f4c, where tView3DExt ends (its own code
// touches members up to 0x1f48, 0x006aa6e0 to 0x006aa790; the sibling tD3DViewMgr also starts
// its own members there). The vtable is 0x007b8f38, 100 slots: slots 0 to 96 are tView3D's and
// tView3DExt's, and 97 to 99 (vfunction98 to vfunction100, declared pure in tView3DExt) are
// implemented here. The allocation 0x0069fd44 (operator new of 0x5fc8 bytes, then the
// constructor 0x0069f3b0) gives the size. Slots 85 and 95 hold a shared "return 0" function
// (0x0069f470) in tView3DExt and slot 85 is this class's 0x0069f480 (see below).
//
// Slots this class overrides in the exe but this header cannot declare, because tView3D keeps
// the signature another unit already uses for the same slot:
//   vfunction8  (+0x1c) 0x0069fcf0 releases the shared instance and returns 0; tSfcApp3d tests
//                       the slot's result as a bool.
//   vfunction9  (+0x20) 0x0069fac0 returns a pointer to a string; tSfcApp3d calls it as a void.
//   vfunction86 (+0x154) 0x0069f480 takes no arguments and returns field_1f68; tSfcApp3d passes two.
// tD3DViewMgr's slots 6 to 8 (0x0069e2a0 and following) do the same as this class's
// 0x0069fce0, 0x0069fcf0 and 0x0069fac0, so the unit that tSfcApp3d passes is probably not a
// tView3D at those slots.
class tSWView3D : public tView3DExt {
public:
	tSWView3D();
	virtual ~tSWView3D();    // +0x00 0x0069f490, the body is 0x0069f4b0
	virtual bool vfunction2(int, int);    // +0x04 0x0069f510
	virtual bool vfunction3(int, int, int, int, RECT *, bool);    // +0x08 0x0069f850
	virtual bool vfunction4(int, int, RECT *, bool);    // +0x0c 0x0069f8a0
	virtual bool vfunction6();    // +0x14 0x0069f900
	virtual void vfunction7();    // +0x18 0x0069fce0, counts a reference to the shared instance
	virtual int vfunction10();    // +0x24 0x0069fad0, 0x1f when field_5fb4 is set, else 0x0e
	virtual bool vfunction11();    // +0x28 0x0069f990
	virtual bool vfunction12();    // +0x2c 0x0069fa10
	virtual void vfunction17(int, int, int, int);    // +0x40 0x0069fa70, sets the rectangle at +0x1f4c
	virtual int vfunction25(int);    // +0x60 0x0069fa50, returns 10
	virtual int vfunction26();    // +0x64 0x0069fa60, returns 10
	virtual bool vfunction27(unsigned char, unsigned char, unsigned char);    // +0x68 0x0069fb10
	virtual bool vfunction28(unsigned int);    // +0x6c 0x0069faf0
	virtual bool vfunction29(int, int, int, int, bool, bool);    // +0x70 0x0069fb90
	virtual bool vfunction30(bool, bool);    // +0x74 0x0069fb50
	virtual bool vfunction31();    // +0x78 0x0069fcc0
	virtual int vfunction43();    // +0xa8 0x0069fdc0, returns 2
	virtual bool vfunction48(bool);    // +0xbc 0x0069fde0, sets field_5fc4
	virtual bool vfunction49(bool);    // +0xc0 0x0069fe00, sets field_5fc5 and field_5fc6
	virtual bool vfunction50(bool);    // +0xc4 0x0069fe20, sets field_5fc5
	virtual bool vfunction51(bool);    // +0xc8 0x0069fe40, sets field_5fc6
	virtual bool vfunction82(void (__stdcall *)(void *, int), int);    // +0x144 0x006b2230
	virtual tTexInfo *vfunction83(int, void *, int, int, int);    // +0x148 0x006b2280
	virtual tTexInfo *vfunction84(int);    // +0x14c 0x006b2500
	virtual bool vfunction85(int, tBmpSurf *, char, int);    // +0x150 0x006b23c0
	virtual bool vfunction98(int, int, int, bool);    // +0x184 0x006b1050
	virtual bool vfunction99(int);    // +0x188 0x006b1660
	virtual bool vfunction100(int, int);    // +0x18c 0x006b1230

	// Members the virtuals above call: not virtual, read and write this class's fields, and have
	// no symbol.
	bool fn_0069f600(int, int, int, int, RECT *, bool, bool);    // 0x0069f600, opens the view for vfunction3 and vfunction4
	void fn_006adf80(int, void *, void *, void *);    // 0x006adf80, draws a triangle: a texture slot and three vertices
	void fn_006b11a0(void *);    // 0x006b11a0, copies a tGLPixFmt into field_5f6c

	// +0x1f4c to +0x1f58 are the clip rectangle (left, top, right, bottom as 0x0069fb90 clamps).
	int field_1f4c;
	int field_1f50;
	int field_1f54;
	int field_1f58;
	unsigned short field_1f5c;    // a packed 5-6-5 colour (0x0069fb10)
	unsigned char field_1f5e[2];
	int field_1f60;
	int field_1f64;
	void *field_1f68;    // an object with a vtable (the surface or device, built by 0x006b3260 or 0x006b4fe0)
	// The texture slots, indexed from 1 by 0x006b2280, 0x006b23c0 and 0x006b2500; the constructor
	// clears them and 0x0069f900 deletes each one through its virtual destructor.
	tTexInfo *field_1f6c[0x1000];
	// HYPOTHESIS: a tGLPixFmt (0x3c bytes; constructor 0x006a0ee0, vtable 0x007959d8, copied by
	// 0x006b11a0). Kept as bytes: its class is declared only inside its own unit. The shifts
	// 0x006b1230 reads are its members at +0x5f74, +0x5f78, +0x5f80, +0x5f84, +0x5f8c,
	// +0x5f90, +0x5f98 and +0x5f9c, and its mode is the int at +0x5fa0.
	unsigned char field_5f6c[0x3c];
	void *field_5fa8;    // an object with a vtable, from field_1f68's slot +0x2c (0x0069f990)
	int field_5fac;    // the row pitch of the surface, from field_5fa8's slot +0x28
	unsigned char *field_5fb0;    // the surface bits, from field_5fa8's slot +0x2c
	unsigned short *field_5fb4;    // the depth buffer, allocated by 0x0069f600 (2 bytes a pixel)
	int field_5fb8;    // its width
	int field_5fbc;    // its height
	int field_5fc0;    // its row size in bytes
	bool field_5fc4;
	bool field_5fc5;
	bool field_5fc6;
};

typedef char tSWView3D_size_is_0x5fc8[ sizeof( tSWView3D ) == 0x5fc8 ? 1 : -1 ];
typedef char tSWView3D_field_1f4c_is_0x1f4c[ (int)&((tSWView3D *)0)->field_1f4c == 0x1f4c ? 1 : -1 ];
typedef char tSWView3D_field_1f68_is_0x1f68[ (int)&((tSWView3D *)0)->field_1f68 == 0x1f68 ? 1 : -1 ];
typedef char tSWView3D_field_1f6c_is_0x1f6c[ (int)&((tSWView3D *)0)->field_1f6c == 0x1f6c ? 1 : -1 ];
typedef char tSWView3D_field_5f6c_is_0x5f6c[ (int)&((tSWView3D *)0)->field_5f6c == 0x5f6c ? 1 : -1 ];
typedef char tSWView3D_field_5fa8_is_0x5fa8[ (int)&((tSWView3D *)0)->field_5fa8 == 0x5fa8 ? 1 : -1 ];
typedef char tSWView3D_field_5fc4_is_0x5fc4[ (int)&((tSWView3D *)0)->field_5fc4 == 0x5fc4 ? 1 : -1 ];

#endif
