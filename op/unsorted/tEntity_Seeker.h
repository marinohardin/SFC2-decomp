//	tEntity_Seeker.h: tEntity_Seeker, the seeking weapon entity (RTTI, vtable 0x007978fc), and
//	the base of tEntity_DriftingSeeker and tEntity_HellboreSeeker.
// Evidence:  class (RTTI: tEntity_Seeker is a tEntity_Weapon; original file unknown)

#ifndef tEntity_Seeker_h
#define tEntity_Seeker_h 1

#include "unsorted/tEntity_Weapon.h"

// HYPOTHESIS: the size is 0x288: the constructor (0x0041e042) calls tEntity_Weapon's, then
// writes its own members from +0x260 to the double at +0x280. The vtable has 71 slots; this
// class adds two (slots 69 and 70) and overrides, among others, slots 0, 2, 4, 5, 10, 13, 17,
// 20 to 23, 25, 30, 62, 65 and 66 (0x0041e0ad, 0x0041f5f5, 0x0041e74f, 0x0041eaf9, 0x0041e64f,
// 0x004203d5, 0x0041e0a9, 0x0042167b, 0x00420447, 0x0041e93f, 0x0041e94d, 0x0041e79a,
// 0x0041e627, 0x0041e09b, 0x0041e95a, 0x0041e0c9); their declarations are written with the
// functions; slot 62 is declared, as its type is now known.
class tEntity_Seeker : public tEntity_Weapon
{
public:
	tEntity_Seeker();

	virtual void vfunction11(tSFCStream &s, int32u mode);
	virtual void vfunction26(short a1, int a2, int a3, int a4, int a5);

	// HYPOTHESIS: slot 62, returns the float at +0x260 (0x0041e09b).
	virtual float vfunction63();
	// HYPOTHESIS: returns a one-byte member at +0x275.
	virtual bool vfunction70();
	// HYPOTHESIS: returns the int at +0x270.
	virtual int vfunction71();

	// HYPOTHESIS: the member 0x0041eeaf and 0x0041ef62 call; it fills a kind byte, a
	// byte and a point of the create message they send.
	void fn_0041ee42( unsigned char *a, char *b, char *c );
	void fn_0041eeaf();
	void fn_0041ef62();

	char field_0x248[0x18];
	// HYPOTHESIS: a float; the constructor sets it to 0.0 and slot 62 returns it.
	float field_0x260;
	float field_0x264;
	// HYPOTHESIS: two ints, both 0x100 after construction; slot 23 divides the first by the
	// second as floats (fild, fidiv) and slot 22 returns the first.
	int field_0x268;
	int field_0x26c;
	int field_0x270;
	char field_0x274;
	char field_0x275;
	char field_0x276[0x2];
	int field_0x278;
	int field_0x27c;
	double field_0x280;
};

typedef char tEntity_Seeker_size_is_0x288[ sizeof( tEntity_Seeker ) == 0x288 ? 1 : -1 ];

#endif	// tEntity_Seeker_h
