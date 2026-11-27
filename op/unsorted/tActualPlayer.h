//	tActualPlayer.h: tActualPlayer, the player's shadow of the game state that the interface
//	reads (RTTI, vtable 0x007a0d4c), and its allocation.
// Evidence:  class (RTTI: tActualPlayer is a tPlayerShadow, a tConnector; original file unknown)

#ifndef tActualPlayer_h
#define tActualPlayer_h 1

#include "unsorted/tPlayerShadow.h"
#include "unsorted/tValueBase.h"

// The object is 0x201c bytes: 0x0049f480 pushes it to operator new before the constructor
// (0x0049f4b2) and stores the result in the global at 0x00917cdc, which the destructor
// (0x004a04a9) resets. The constructor builds every member in place, in offset order, so the
// members below are the original's tiling of the object: each starts where the constructor
// calls its constructor or stores its vtable, and runs to the next. Members this file
// cannot type are bytes. A member named after an RTTI class (a comment says which) is that
// class's object; their layouts are not declared here. The vtable has 27 slots; 0x0049f4b2
// also holds tPlayerShadow's constructor inline. vfunctionN is slot N-1; the one-argument
// functions take a pointer to an object whose slots 0x200 and 0x228 they call (a ship), so
// the argument is a void pointer here.

// tShadowFloat (RTTI, vtable 0x007a0e44, constructor 0x004a03bc): a 12-byte shadow of one value.
// Its slot names are not known; vfunctionN is slot N-1.
class tShadowFloat
{
public:
	tShadowFloat();
	virtual void vfunction1();
	virtual void vfunction2();
	// HYPOTHESIS: slot 2 sets the shadowed value; tActualPlayer::vfunction27 passes 0 to it.
	virtual void vfunction3(int aValue);

	int field_0x4;
	int field_0x8;
};

// tShadowDragBar (RTTI, vtable 0x007a0e34): a tShadowFloat of the same size.
class tShadowDragBar : public tShadowFloat
{
};

class tActualPlayer : public tPlayerShadow
{
public:
	tActualPlayer();
	virtual ~tActualPlayer();
	virtual void vfunction4(int aArg);
	virtual void vfunction5(void *aObject);
	virtual void vfunction6(void *aObject);
	virtual void vfunction7(void *aObject);
	virtual void vfunction8(void *aObject);
	virtual void vfunction9(void *aObject);
	virtual void vfunction10();
	virtual void vfunction11(void *aObject);
	virtual void vfunction12(void *aObject);
	virtual void vfunction13(void *aObject);
	virtual void vfunction14(void *aObject);
	virtual void vfunction15(void *aObject);
	virtual void vfunction16(void *aObject);
	virtual void vfunction17(void *aObject);
	virtual void vfunction18(void *aObject);
	virtual void vfunction19(void *aObject);
	virtual void vfunction20(void *aObject);
	virtual void vfunction21(void *aObject);
	virtual void vfunction22(void *aObject);
	virtual void vfunction23(void *aObject);
	virtual void vfunction24(void *aObject);
	virtual void vfunction25(void *aObject);
	virtual void vfunction26();
	virtual void vfunction27();

	char field_0x2c[0x8];  // after tConnector (0x2c bytes); the first member starts at 0x34
	char field_0x34[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x40[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tShadowDragBar field_0x4c;
	char field_0x58[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0xac[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tShadowFloat field_0xb8;
	char field_0xc4[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0xd0[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tShadowFloat field_0xdc;
	tShadowFloat field_0xe8;
	char field_0xf4[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x148[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x154[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x1a8[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tShadowFloat field_0x1b4;
	char field_0x1c0[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x214[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x268[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x2bc[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x310[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x364[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x370[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x37c[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x388[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x394[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x3e8[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tShadowFloat field_0x3f4;
	char field_0x400[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tShadowFloat field_0x40c;
	char field_0x418[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x424[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x430[0x54];  // tShadowGaugeStr (RTTI, vtable 0x007a0e20; a tShadowFloat)
	char field_0x484[0xc];  // tShadowFloatV (RTTI, vtable 0x007a0e10)
	char field_0x490[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0x4e0[0x14];
	char field_0x4f4[0x14];
	char field_0x508[0x14];
	char field_0x51c[0x10];
	char field_0x52c[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	char field_0x548[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x554[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x560[0xa0];  // tRadioShadow (RTTI, vtable 0x007a0ed8, constructor 0x004a041e)
	int field_0x600;
	char field_0x604[0xa0];  // tRadioShadow (RTTI, vtable 0x007a0ed8, constructor 0x004a041e)
	int field_0x6a4;
	char field_0x6a8[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x6b4[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x6c0[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tMinMaxFloat field_0x6cc;
	tMinMaxFloat field_0x6f0;
	char field_0x714[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	char field_0x730[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	// HYPOTHESIS: the object vfunction9 is given; it reads ints at +0x47c and +0x480 through it.
	void *field_0x73c;
	char field_0x740[0x8];
	char field_0x748[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0x798[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x7a4[0xc];  // tShadowInt (RTTI, vtable 0x007a0df0)
	char field_0x7b0[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	tMinMaxFloat field_0x7bc;
	char field_0x7e0[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x7ec[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x7f8[0xb4];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	char field_0x8ac[0x50];
	char field_0x8fc[0x50];
	char field_0x94c[0x50];
	char field_0x99c[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0x9ec[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0xa3c[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0xa8c[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0xadc[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0xb2c[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0xb7c[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0xbcc[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	// HYPOTHESIS: the object the vfunction5 to vfunction9 family is given; the next four are set
	// to the addresses of tMinMaxFloat members by vfunction5.
	void *field_0xbe8;
	tMinMaxFloat *field_0xbec;
	tMinMaxFloat *field_0xbf0;
	tMinMaxFloat *field_0xbf4;
	tMinMaxFloat *field_0xbf8;
	// HYPOTHESIS: tValue<long>: vfunction5 stores at +0x18 and then notifies with (3, 0).
	tValue<long> field_0xbfc;
	tValue<long> field_0xc18;
	tMinMaxFloat field_0xc34;
	tMinMaxFloat field_0xc58;
	tMinMaxFloat field_0xc7c;
	tMinMaxFloat field_0xca0;
	tMinMaxFloat field_0xcc4;
	tMinMaxFloat field_0xce8;
	tMinMaxFloat field_0xd0c;
	tMinMaxFloat field_0xd30;
	tMinMaxFloat field_0xd54;
	tMinMaxFloat field_0xd78;
	tMinMaxFloat field_0xd9c;
	tMinMaxFloat field_0xdc0;
	tMinMaxFloat field_0xde4;
	tMinMaxFloat field_0xe08;
	tMinMaxFloat field_0xe2c;
	tMinMaxFloat field_0xe50;
	tMinMaxFloat field_0xe74;
	tMinMaxFloat field_0xe98;
	tMinMaxFloat field_0xebc;
	tMinMaxFloat field_0xee0;
	tMinMaxFloat field_0xf04;
	tMinMaxFloat field_0xf28;
	tMinMaxFloat field_0xf4c;
	tMinMaxFloat field_0xf70;
	tMinMaxFloat field_0xf94;
	tMinMaxFloat field_0xfb8;
	tMinMaxFloat field_0xfdc;
	tMinMaxFloat field_0x1000;
	tMinMaxFloat field_0x1024;
	tMinMaxFloat field_0x1048;
	tMinMaxFloat field_0x106c;
	tMinMaxFloat field_0x1090;
	tMinMaxFloat field_0x10b4;
	tMinMaxFloat field_0x10d8;
	tMinMaxFloat field_0x10fc;
	tMinMaxFloat field_0x1120;
	tMinMaxFloat field_0x1144;
	tMinMaxFloat field_0x1168;
	tMinMaxFloat field_0x118c;
	tMinMaxFloat field_0x11b0;
	tMinMaxFloat field_0x11d4;
	char field_0x11f8[0x28];
	char field_0x1220[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	char field_0x123c[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	char field_0x1258[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	char field_0x12a8[0x50];  // tShadowTStr (RTTI, vtable 0x007a0dbc, constructor 0x004a03cd)
	tMinMaxFloat field_0x12f8;
	tMinMaxFloat field_0x131c;
	tMinMaxFloat field_0x1340;
	tMinMaxFloat field_0x1364;
	tMinMaxFloat field_0x1388;
	tMinMaxFloat field_0x13ac;
	tMinMaxFloat field_0x13d0;
	tMinMaxFloat field_0x13f4;
	tMinMaxFloat field_0x1418;
	tMinMaxFloat field_0x143c;
	tMinMaxFloat field_0x1460;
	tMinMaxFloat field_0x1484;
	tMinMaxFloat field_0x14a8;
	tMinMaxFloat field_0x14cc;
	tMinMaxFloat field_0x14f0;
	tMinMaxFloat field_0x1514;
	tMinMaxFloat field_0x1538;
	tMinMaxFloat field_0x155c;
	tMinMaxFloat field_0x1580;
	tMinMaxFloat field_0x15a4;
	tMinMaxFloat field_0x15c8;
	tMinMaxFloat field_0x15ec;
	tMinMaxFloat field_0x1610;
	tMinMaxFloat field_0x1634;
	tMinMaxFloat field_0x1658;
	tMinMaxFloat field_0x167c;
	tMinMaxFloat field_0x16a0;
	tMinMaxFloat field_0x16c4;
	tMinMaxFloat field_0x16e8;
	tMinMaxFloat field_0x170c;
	tMinMaxFloat field_0x1730;
	char field_0x1754[0x28];
	char field_0x177c[0xc];  // tShadowFloat (RTTI, vtable 0x007a0e44; stored inline, 12 bytes)
	// HYPOTHESIS: tValue<long>, the next three: vfunction9 stores at +0x18 and notifies with (3, 0).
	tValue<long> field_0x1788;
	tValue<long> field_0x17a4;
	tValue<long> field_0x17c0;
	char field_0x17dc[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	char field_0x17f8[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	char field_0x1814[0x1c];  // a value object, constructed by tValueBase (0x00436633) and given its own vtable
	tValue<long> field_0x1830;
	tValue<long> field_0x184c;
	tValue<long> field_0x1868;
	tValue<long> field_0x1884;
	tValue<long> field_0x18a0;
	tMinMaxFloat field_0x18bc;
	char field_0x18e0[0x24];  // tMinMaxInt (RTTI, constructor 0x0043dfb3)
	tMinMaxFloat field_0x1904;
	tMinMaxFloat field_0x1928;
	tMinMaxFloat field_0x194c;
	tMinMaxFloat field_0x1970;
	tMinMaxFloat field_0x1994;
	tMinMaxFloat field_0x19b8;
	tMinMaxFloat field_0x19dc;
	tMinMaxFloat field_0x1a00;
	tMinMaxFloat field_0x1a24;
	tMinMaxFloat field_0x1a48;
	tMinMaxFloat field_0x1a6c;
	tMinMaxFloat field_0x1a90;
	tMinMaxFloat field_0x1ab4;
	tMinMaxFloat field_0x1ad8;
	tMinMaxFloat field_0x1afc;
	tMinMaxFloat field_0x1b20;
	tMinMaxFloat field_0x1b44;
	tMinMaxFloat field_0x1b68;
	tMinMaxFloat field_0x1b8c;
	tMinMaxFloat field_0x1bb0;
	tMinMaxFloat field_0x1bd4;
	tMinMaxFloat field_0x1bf8;
	tMinMaxFloat field_0x1c1c;
	tMinMaxFloat field_0x1c40;
	tValue<float> field_0x1c64;
	tValue<long> field_0x1c80;
	tValue<long> field_0x1c9c;
	tValue<long> field_0x1cb8;
	tValue<long> field_0x1cd4;
	tValue<long> field_0x1cf0;
	tValue<long> field_0x1d0c;
	tValue<long> field_0x1d28;
	tMinMaxFloat field_0x1d44;
	tMinMaxFloat field_0x1d68;
	tMinMaxFloat field_0x1d8c;
	tMinMaxFloat field_0x1db0;
	tMinMaxFloat field_0x1dd4;
	tMinMaxFloat field_0x1df8;
	tValue<long> field_0x1e1c;
	tMinMaxFloat field_0x1e38;
	tValue<long> field_0x1e5c;
	char field_0x1e78[0x114];
	tMinMaxFloat field_0x1f8c;
	tMinMaxFloat field_0x1fb0;
	tMinMaxFloat field_0x1fd4;
	tMinMaxFloat field_0x1ff8;
};

typedef char tActualPlayer_size_is_0x201c[ sizeof( tActualPlayer ) == 0x201c ? 1 : -1 ];

// HYPOTHESIS: a cdecl free function of the object tActualPlayer::vfunction5 is given; returns
// the value vfunction5 stores in field_0xc18.
int fn_004a6f3d(void *aObject);

#endif	// tActualPlayer_h
