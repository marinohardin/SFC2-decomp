//	tState_Tactical.h: tState_Namespace_Tactical::tState_Tactical, the tactical (in-mission) game state.
// Evidence:  class (RTTI; original file unknown)

#ifndef tState_Tactical_h
#define tState_Tactical_h 1

#include "Tiger/tvectormath.h"
#include "unsorted/tGameState.h"
#include "unsorted/tTacticalSubStateMan.h"

void __fastcall fn_004afa40( void *self );
void __fastcall fn_004bc256( void *self );

// HYPOTHESIS: the member type of the seven growable arrays: {data, count, capacity}, whose
// destructor is the function at 0x004afa40.
class fn_004afa40_Array
{
public:
	~fn_004afa40_Array() { fn_004afa40( this ); }
	unsigned char m_data[0xc];
};

// HYPOTHESIS: the member type of the list head at +0x1ac, whose destructor is 0x004bc256.
class fn_004bc256_List
{
public:
	~fn_004bc256_List() { fn_004bc256( this ); }
	unsigned char m_data[0xc];
};

namespace tState_Namespace_Tactical
{

// The state is one static object at 0x00917f10 (constructor 0x004aae0f, destructor 0x004ab015,
// reached through the accessor 0x004adc83). The next global starts at 0x009183c0, which makes
// the size 0x4b0, and the constructor's last store is at 0x4ac. Vtable 0x007a1344: 19 slots.
// Base: tGameState (constructor 0x00469cd7 called first, vtable 0x0079ec4c), 0xc bytes.
// Members are placed by the offsets the constructor and the slot bodies use; a member is typed
// only where a use proves it, the rest is left as bytes under its offset.
// The slots this state does not declare are tGameState's own. Slot names follow the symbol table
// (slot n is vfunction(n+1)); parameter types are the base's.
class tState_Tactical : public tGameState
{
public:
	tState_Tactical();
	virtual ~tState_Tactical();	// 0x004ab015
	virtual bool vfunction2();	// 0x004ab0f1
	virtual void vfunction3();	// 0x004abadd, the per-frame update (6690 bytes)
	virtual bool vfunction4();	// 0x004ab70a
	virtual void vfunction6(int param_1);	// 0x004b66a8
	virtual void vfunction7(int param_1);	// 0x004b714d: clears field_240, then calls 0x004b6642 on 0x00919f00
	virtual void vfunction8(int *param_1);	// 0x004b99f7: calls slots 26 and 29 of its argument
	virtual void vfunction9(int *param_1);	// 0x004b9a3a: copies the argument's matrix (slot 55) into field_294
	virtual void vfunction10(int param_1);	// 0x004b6647
	virtual void vfunction11(int param_1);	// 0x004b9dd6
	virtual char *vfunction13();	// 0x004adc4c: returns the state's name
	virtual void vfunction15(c_0046aa10_message *param_1);	// 0x004b2f8b
	virtual void vfunction17(int *param_1);	// 0x004b3809
	virtual void *vfunction19();	// 0x004af807: returns a constant address

	// HYPOTHESIS: 0x004b35db keeps ecx live up to its call of 0x004bc55a, which it makes
	// without loading ecx: both are members of one class, and the class is only a guess.
	void fn_004b35db(long *event);
	void fn_004b3758(long *event);
	void fn_004b3793(long *event);
	void fn_004b37ce(long *event);
	void fn_004bc55a(long x, long y, char *out);
	void fn_004ba05b(int *param_1);
	void fn_004ba5ea(int *param_1);
	void fn_004ba7c7(int *param_1);
	void fn_004ba9eb(int *param_1);
	unsigned char fn_004adc52();
	void fn_004aba4f();
	int *fn_004ad878();
	int *fn_004ad89d();

	int field_0c;	// 0x1e
	float field_10;
	float field_14;
	float field_18;
	float field_1c;
	float field_20;
	float field_24;	// 1.0
	unsigned char field_28[0x1ac - 0x28];
	// 3 dwords zeroed by 0x004230b9, released by 0x004bc256 (a list head).
	fn_004bc256_List field_1ac;
	unsigned char field_1b8[0x1e0 - 0x1b8];
	// Two screen rectangles (left, top, right, bottom): the slots compare a point against them.
	int field_1e0[4];
	int field_1f0[4];
	unsigned char field_200[0x240 - 0x200];
	unsigned char field_240;
	unsigned char field_241[3];
	int field_244;
	int field_248;
	float field_24c[3];
	unsigned char field_258;
	unsigned char field_259[3];
	int field_25c[4];
	unsigned char field_26c[8];
	int field_274;	// -1
	int field_278[4];
	unsigned char field_288[8];
	int field_290;	// -1
	// Three matrices, identity at construction, the flag at +0x41 set to 1 on the first two.
	tMatrix field_294;
	tMatrix field_2d8;
	tMatrix field_31c;
	// 0xb0 bytes built by 0x004afd4d: floats from +0x10 to +0x2c, 4 dwords at +0x30, three
	// blocks cleared by memset at +0x40 (0x30), +0x70 (0x30) and +0xa0 (0x10).
	unsigned char field_360[0xb0];
	// Seven growable arrays, each {data, count, capacity}, started with room for 40 and freed by
	// 0x004afa40. Element sizes from the allocating constructors: 0x1c at +0x410 and +0x41c
	// (0x004afa1c), 4 at +0x428, +0x434 and +0x458 (0x004afb43), 8 at +0x440 (0x004afc63),
	// 0x14 at +0x44c (0x004afbc4).
	fn_004afa40_Array field_410;
	fn_004afa40_Array field_41c;
	fn_004afa40_Array field_428;
	fn_004afa40_Array field_434;
	fn_004afa40_Array field_440;
	fn_004afa40_Array field_44c;
	fn_004afa40_Array field_458;
	int field_464;
	// HYPOTHESIS: names of states (strings), as 0x004aba4f stores them.
	const char *field_468;
	const char *field_46c;
	int field_470;
	unsigned char field_474;
	unsigned char field_475[3];
	// A tTacticalSubStateMan (constructor 0x004bc911): vtable pointer and one pointer, 8 bytes.
	tTacticalSubStateMan field_478;
	unsigned char field_480;
	unsigned char field_481[3];
	int field_484;
	int field_488;
	unsigned char field_48c;
	unsigned char field_48d;
	unsigned char field_48e;
	unsigned char field_48f;
	float field_490;
	float field_494;
	int field_498[4];	// -1
	int field_4a8;
	int field_4ac;
};

typedef char tState_Tactical_size_is_0x4b0[ sizeof( tState_Tactical ) == 0x4b0 ? 1 : -1 ];

// A camera (RTTI); the base of the five camera kinds. Destructor 0x004af2cd.
// HYPOTHESIS: only the vtable pointer is known; the size of 0x10 is the smallest gap between two
// cameras in tCameraMgrEx.
class tCamera
{
public:
	virtual ~tCamera();
	unsigned char field_04[0xc];
};

inline tCamera::~tCamera()
{
}

// The base of tCameraMgrEx (RTTI). Destructor 0x004af1bc.
// HYPOTHESIS: the layout up to +0x50 is not recovered yet.
class tCameraMgr
{
public:
	virtual ~tCameraMgr();
	unsigned char field_04[0x50 - 0x04];
};

inline tCameraMgr::~tCameraMgr()
{
}

// The five camera kinds (RTTI). Each derives from tCamera: the destructor 0x004add41 leaves
// tCamera's vtable in every camera member, which needs a derived destructor inlined before it.
// HYPOTHESIS: each derives from tCamera directly; their own members are not recovered yet.
class tCamera_CenteredShip : public tCamera
{
public:
	virtual ~tCamera_CenteredShip();
};

inline tCamera_CenteredShip::~tCamera_CenteredShip()
{
}

class tCamera_TopDownAutoZoom : public tCamera
{
public:
	virtual ~tCamera_TopDownAutoZoom();
};

inline tCamera_TopDownAutoZoom::~tCamera_TopDownAutoZoom()
{
}

class tCamera_OverTheShoulder : public tCamera
{
public:
	virtual ~tCamera_OverTheShoulder();
};

inline tCamera_OverTheShoulder::~tCamera_OverTheShoulder()
{
}

class tCamera_ReverseOverTheShoulder : public tCamera
{
public:
	virtual ~tCamera_ReverseOverTheShoulder();
};

inline tCamera_ReverseOverTheShoulder::~tCamera_ReverseOverTheShoulder()
{
}

class tCamera_TopDownOrthographic : public tCamera
{
public:
	virtual ~tCamera_TopDownOrthographic();
};

inline tCamera_TopDownOrthographic::~tCamera_TopDownOrthographic()
{
}

// The camera manager (RTTI). Constructor 0x004aefd4, deleting destructor 0x004af2d4; one static
// object at 0x00918890. Its destructor 0x004add41 resets five cameras at +0x50, +0x60, +0x8c,
// +0xc8 and +0xf4. HYPOTHESIS: which kind sits at which offset follows the order of the kinds'
// functions in the exe; the bytes after each camera's tCamera part are not recovered yet.
class tCameraMgrEx : public tCameraMgr
{
public:
	tCameraMgrEx();
	virtual ~tCameraMgrEx();
	tCamera_CenteredShip field_50;
	tCamera_TopDownAutoZoom field_60;
	unsigned char field_70[0x8c - 0x70];
	tCamera_OverTheShoulder field_8c;
	unsigned char field_9c[0xc8 - 0x9c];
	tCamera_ReverseOverTheShoulder field_c8;
	unsigned char field_d8[0xf4 - 0xd8];
	tCamera_TopDownOrthographic field_f4;
};

}	// namespace tState_Namespace_Tactical

void __fastcall fn_004c5b95( void *self );

// HYPOTHESIS: the type of the object at 0x00919b44, whose destructor is the function at 0x004c5b95.
// 0x004c5d2e, called on it with a binding id, gives that binding's text or null.
class fn_004c5b95_Object
{
public:
	~fn_004c5b95_Object() { fn_004c5b95( this ); }
	const char *fn_004c5d2e( long id, long param_2 );
	unsigned char m_data[4];
};

extern fn_004c5b95_Object *g_00919b44;

// HYPOTHESIS: the mouse at 0x00898c24: slot 9 selects a cursor by name, slot 10 a race's cursor.
class fn_00898c24_object
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
	virtual void vfunction9(const char *name);
	virtual void vfunction10(int kind);
};

extern fn_00898c24_object *g_00898c24;

#endif	// tState_Tactical_h
