//	tView3DExt.h: tView3DExt, the extended view (name from RTTI).
// Evidence:  class (original file unknown)

#ifndef tView3DExt_h
#define tView3DExt_h 1

#include <stddef.h>

#include "Tiger/tvectormath.h"
#include "unsorted/tApp3dBase.h"

// HYPOTHESIS: a 64-byte vertex record; only its size is known (the vertex cache allocates
// 0x40 bytes per entry). The empty constructor is inferred from the null test the
// exe makes after operator new, which only a vector new with a constructor emits. Of its
// words, +0x00 is a flag word the view clears per entry (vfunction79) and sets once +0x04 holds
// a transformed point (vfunction71); +0x2c and +0x30 are copied from the view's +0x1ef4 and
// +0x1ef8, +0x34..+0x3c from its colour.
struct fn_006abe00_Vtx
{
	fn_006abe00_Vtx() {}

	unsigned int	field_0x00;    // flags; 0x10: field_0x20 holds 1 / field_0x10 (0x006abeb0)
	float			field_0x04;    // x
	float			field_0x08;    // y
	float			field_0x0c;
	float			field_0x10;    // w
	char			field_0x14[0x0c];
	float			field_0x20;    // 1 / w, cached
	char			field_0x24[0x08];
	float			field_0x2c;    // the view's +0x1ef4 (vfunction71)
	float			field_0x30;    // the view's +0x1ef8
	float			field_0x34;    // colour (vfunction71)
	float			field_0x38;
	float			field_0x3c;
};

typedef char fn_006abe00_Vtx_size_is_0x40[ sizeof( fn_006abe00_Vtx ) == 0x40 ? 1 : -1 ];

// HYPOTHESIS: the 0x40-byte member at the view's +0x1eec. 0x006a9540 calls 0x006ac780 with its
// address in ecx, so it is an object of its own; its class has no name in the exe. 0x006ac780
// sets +0x00 to -1, the next three words to 0 and the following eight to 1.0. Fields carry
// their offsets in this struct; the view's offset is 0x1eec more.
struct fn_006ac780_State
{
	void	fn_006ac780();

	int					field_0x00;    // -1, or 0..9 (vfunction68)
	int					field_0x04;    // the next cache entry, below 0x150
	float				field_0x08;    // vfunction74
	float				field_0x0c;
	// HYPOTHESIS: a point; vfunction73 takes a tPoint3 * and vfunction68 sets it with tPoint3::mSet
	tPoint3				field_0x10;    // vfunction73
	// HYPOTHESIS: a colour point; vfunction77 sets it with tPoint3::mSet (the store order shows it)
	tPoint3				field_0x1c;    // vfunction77, vfunction76
	float				field_0x28;
	float				field_0x2c;
	float				field_0x30;
	int					field_0x34;
	unsigned int		field_0x38;    // at most 0x1000, vfunction78
	int					field_0x3c;
};

typedef char fn_006ac780_State_size_is_0x40[ sizeof( fn_006ac780_State ) == 0x40 ? 1 : -1 ];
typedef void (*fn_006a9f20_Callback)();    // HYPOTHESIS: a no-argument callback, as vfunction13/15 keep it
// HYPOTHESIS: derived from tView3D (both carry the vtable slots the units call). Its size is
// 0x1f4c: tSWView3D and tD3DViewMgr (allocated as 0x8010 bytes) both start their own members
// there, and no method of this class touches a byte beyond it. No member is named in the exe,
// so the fields carry their offsets, and their types are what the uses prove.
//
// The constructor (0x006a9c60) inlines tView3D's, which writes the members from +0x55c up to
// +0x1778 before the vtable pointer is switched to this class: those probably belong to
// tView3D, whose declaration (tApp3dBase.h) is a stand-in that holds none. They are all
// declared here until tView3D is recovered, so tView3D stays 4 bytes (its vtable pointer).
//
// The vtable is 0x007b9388. Slots 0..85 override tView3D's (tApp3dBase.h declares them);
// slots 86 and up are new here. Slots 85 and 95 hold a shared "return 0" function
// (0x0069f470) and are left undeclared; slots 97 and up are not evidenced.
class tView3DExt : public tView3D {
public:
	tView3DExt();    // 0x006a9c60
	virtual ~tView3DExt();    // slot 0, 0x006a9e80 is its deleting destructor

	void	mAssertVtxCache( int count );
	void	fn_006a9540();    // HYPOTHESIS: a member (it runs on this), resets the view's state
	float	fn_006abeb0();    // HYPOTHESIS: a member; the screen-space cross product of the first three indexed vertices
	void	fn_006ac900( int );    // HYPOTHESIS: a member (vfunction69 calls it on this with 1)
	void	fn_006ac830( int, int, int );    // HYPOTHESIS: a member; counts a primitive by type and draws it through vfunction98
	tPoint3	fn_006a9790( tPoint3 *, tPoint3 * );    // HYPOTHESIS: a member (vfunction73 calls it on this), returns a point by value

	// Overrides, by tView3D's names (slot = number - 1). HYPOTHESIS: 0x006a95f0, the function
	// in slot 5, is this class's (it frees the vertex cache); the symbol table now says so too,
	// as a placeholder name.
	virtual void vfunction5(int);    // 0x006aa6d0, stores its argument at +0x774
	virtual bool vfunction6();
	virtual fn_006a9f20_Callback vfunction13(fn_006a9f20_Callback, void *);    // 0x006a9f20, returns the old callback
	virtual fn_006a9f20_Callback vfunction14();    // 0x006a9f40
	virtual fn_006a9f20_Callback vfunction15(fn_006a9f20_Callback, void *);    // 0x006a9f50, likewise
	virtual fn_006a9f20_Callback vfunction16();    // 0x006a9f70
	virtual void vfunction17(int, int, int, int);    // 0x006aa000, sets the viewport
	virtual void vfunction18(int *, int *, int *, int *);    // 0x006aa080, reads it back
	virtual int vfunction19(int);    // 0x006a9670, returns the old setting
	virtual int vfunction20();    // 0x006a96d0
	virtual int vfunction21(int);    // 0x006a96e0, accepts 3..5
	virtual int vfunction22();    // 0x006a9700
	virtual int vfunction23(int);    // 0x006a9620, accepts 6 and 7
	virtual int vfunction24();    // 0x006a9660
	virtual int vfunction32();    // 0x006a9aa0, returns 0x40 (the number of lights)
	virtual void vfunction33(unsigned int, const void *);    // 0x006a9ab0, sets a 0x40-byte light
	virtual void vfunction34(unsigned int, void *);    // 0x006a9ba0, reads one back
	virtual void vfunction35(fn_0040105e_Vector *);    // 0x006a9f80, normalizes into +0x738
	virtual fn_0040105e_Vector *vfunction36();    // 0x006a9ff0, the vector at +0x738
	virtual bool vfunction37(bool);    // 0x006a9c20, returns the old flag
	virtual bool vfunction38();    // 0x0069f190
	virtual bool vfunction39(unsigned int, bool);    // 0x006a9c40, light enable, false past 0x40
	virtual void vfunction45(float);    // 0x006a9710, clamps to 0..1 and keeps a byte copy
	virtual float vfunction46();    // 0x006a9780
	virtual bool vfunction47(bool);    // 0x0069fda0, returns the old flag
	virtual void vfunction55(tMatrix *);    // 0x006aa4a0
	virtual tMatrix *vfunction56();    // 0x006aa4d0
	virtual void vfunction57(tPoint3 *);    // 0x006aa4e0
	virtual void vfunction58(tMatrix *);    // 0x006aa550
	virtual tMatrix *vfunction59();    // 0x006aa6c0
	virtual void vfunction60(tMatrix *);    // 0x006aa3f0
	virtual tMatrix *vfunction61();    // 0x006aa490
	virtual void vfunction62(float, float, float);    // 0x006aa1c0, builds through vfunction65 and sets through vfunction60
	virtual void vfunction63(float, float, float, float, float, float);    // 0x006aa210
	virtual void vfunction64(float, float);    // 0x006aa270
	virtual void vfunction65(tMatrix *, float, float, float);    // 0x006aa0e0
	virtual void vfunction66(tMatrix *, float, float);    // 0x006aa390
	virtual void vfunction67(tMatrix *, float, float, float, float, float, float);    // 0x006aa2c0
	virtual bool vfunction68(int);    // 0x006ac7c0
	virtual void vfunction69();    // 0x006ac810
	virtual void vfunction70(float, float, float);    // 0x006ad230
	virtual void vfunction71(tPoint3 *);    // 0x006ad130
	virtual void vfunction72(float, float, float);    // 0x006ad2d0
	virtual void vfunction73(tPoint3 *);    // 0x006ad260
	virtual void vfunction74(float, float);    // 0x006ad320
	virtual void vfunction75(tPoint2 *);    // 0x006ad300
	virtual void vfunction76(unsigned int);    // 0x006ad370
	virtual void vfunction77(float, float, float);    // 0x006ad340
	virtual void vfunction78(unsigned int);    // 0x006ad3e0
	virtual void vfunction79(int, int);    // 0x006abf80
	virtual void vfunction80(void *);    // 0x006abfb0
	virtual void vfunction81(int, void **, bool);    // 0x006ac5a0

	// New slots.
	virtual void vfunction87();    // slot 86, 0x006aa6e0 clears the counters
	virtual int vfunction88();    // slot 87..94, 0x006aa720..0x006aa790 return the counters
	virtual int vfunction89();
	virtual int vfunction90();
	virtual int vfunction91();
	virtual int vfunction92();
	virtual int vfunction93();
	virtual int vfunction94();
	virtual int vfunction95();
	virtual int vfunction96();    // slot 95, not this class's (0x0069f470)
	virtual bool vfunction97(int, int, void **, bool);    // slot 96, 0x006abfc0 draws a list
	virtual bool vfunction98(int, int, int, bool) = 0;    // slot 97, pure here (+0x184); both implementations return a bool

	fn_006abe00_Vtx		*mVtxCache;    // +0x04
	int					mVtxCacheSize;    // +0x08
	fn_006abe00_Vtx		*mVtxCacheEnd;    // +0x0c HYPOTHESIS: the first of the 16 spare entries
	int					field_0x10;
	// HYPOTHESIS: indices into the cache, written two bytes apart and read three at a time.
	unsigned short		field_0x14[0x2a0];
	int					field_0x554;    // 0x006abf80's second argument
	void				*field_0x558;    // 0x006abfb0's argument, read with a 0x24-byte stride
	// The constructor builds these seven with tMatrix's constructor (0x0069c250).
	tMatrix				field_0x55c;    // vfunction61, vfunction60
	tMatrix				field_0x5a0;
	tMatrix				field_0x5e4;
	tMatrix				field_0x628;    // vfunction56, vfunction55
	tMatrix				field_0x66c;
	tMatrix				field_0x6b0;
	tMatrix				field_0x6f4;    // vfunction59, vfunction58
	float				field_0x738[3];    // vfunction36, starts as (1, 0, 0)
	float				field_0x744[3];
	float				field_0x750[3];
	float				field_0x75c;    // viewport, set from integers by vfunction17
	float				field_0x760;
	float				field_0x764;
	float				field_0x768;
	float				field_0x76c;
	float				field_0x770;
	int					field_0x774;    // vfunction5's argument
	// 0x40 lights of 0x40 bytes; the constructor calls a constructor that does nothing (0x006a9ea0).
	// +0x00 is the type, +0x28 a range.
	char				field_0x778[0x40][0x40];
	char				field_0x1778[0x40];    // vfunction39
	float				field_0x17b8[0x40][3];
	float				field_0x1ab8[0x40][3];
	float				field_0x1db8[0x40];    // 1 / range, set by vfunction33
	bool				field_0x1eb8;   // vfunction37, vfunction38
	int					field_0x1ebc;    // 6 or 7, vfunction23
	int					field_0x1ec0;
	int					field_0x1ec4;    // vfunction19
	int					field_0x1ec8;
	int					field_0x1ecc;    // 3..5, vfunction21
	char				field_0x1ed0;
	float				field_0x1ed4;    // vfunction45
	bool				field_0x1ed8;    // vfunction47
	unsigned char		field_0x1ed9;
	fn_006a9f20_Callback	field_0x1edc;    // vfunction13
	void				*field_0x1ee0;
	fn_006a9f20_Callback	field_0x1ee4;    // vfunction15
	void				*field_0x1ee8;
	fn_006ac780_State	field_0x1eec;    // +0x1eec..+0x1f2b
	int					field_0x1f2c[8];    // counters, cleared by vfunction87, read by vfunction88..95
};

typedef char tView3DExt_size_is_0x1f4c[ sizeof( tView3DExt ) == 0x1f4c ? 1 : -1 ];
typedef char tView3DExt_mVtxCache_is_0x4[ offsetof( tView3DExt, mVtxCache ) == 0x4 ? 1 : -1 ];
typedef char tView3DExt_field_0x14_is_0x14[ offsetof( tView3DExt, field_0x14 ) == 0x14 ? 1 : -1 ];
typedef char tView3DExt_field_0x55c_is_0x55c[ offsetof( tView3DExt, field_0x55c ) == 0x55c ? 1 : -1 ];
typedef char tView3DExt_field_0x6f4_is_0x6f4[ offsetof( tView3DExt, field_0x6f4 ) == 0x6f4 ? 1 : -1 ];
typedef char tView3DExt_field_0x738_is_0x738[ offsetof( tView3DExt, field_0x738 ) == 0x738 ? 1 : -1 ];
typedef char tView3DExt_field_0x774_is_0x774[ offsetof( tView3DExt, field_0x774 ) == 0x774 ? 1 : -1 ];
typedef char tView3DExt_field_0x778_is_0x778[ offsetof( tView3DExt, field_0x778 ) == 0x778 ? 1 : -1 ];
typedef char tView3DExt_field_0x1778_is_0x1778[ offsetof( tView3DExt, field_0x1778 ) == 0x1778 ? 1 : -1 ];
typedef char tView3DExt_field_0x1ab8_is_0x1ab8[ offsetof( tView3DExt, field_0x1ab8 ) == 0x1ab8 ? 1 : -1 ];
typedef char tView3DExt_field_0x1db8_is_0x1db8[ offsetof( tView3DExt, field_0x1db8 ) == 0x1db8 ? 1 : -1 ];
typedef char tView3DExt_field_0x1eb8_is_0x1eb8[ offsetof( tView3DExt, field_0x1eb8 ) == 0x1eb8 ? 1 : -1 ];
typedef char tView3DExt_field_0x1ed4_is_0x1ed4[ offsetof( tView3DExt, field_0x1ed4 ) == 0x1ed4 ? 1 : -1 ];
typedef char tView3DExt_field_0x1edc_is_0x1edc[ offsetof( tView3DExt, field_0x1edc ) == 0x1edc ? 1 : -1 ];
typedef char tView3DExt_field_0x1eec_is_0x1eec[ offsetof( tView3DExt, field_0x1eec ) == 0x1eec ? 1 : -1 ];
typedef char tView3DExt_field_0x1f2c_is_0x1f2c[ offsetof( tView3DExt, field_0x1f2c ) == 0x1f2c ? 1 : -1 ];

// HYPOTHESIS: tView3D's destructor is empty and inline; ~tView3DExt (0x006a9eb0) ends with only the
// base vtable store. It belongs after tView3D in tApp3dBase.h, which is outside this lane.
inline tView3D::~tView3D()
{
}

#endif
