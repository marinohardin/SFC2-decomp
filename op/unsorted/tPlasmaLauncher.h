//	tPlasmaLauncher.h: tPlasmaLauncher, the plasma torpedo launcher, a weapon system.
// Evidence:  class (RTTI; original file unknown). vftable 0x007a8d08 (149 slots); constructor
//            0x004e7949 calls tBaseWeapon's (0x004d70eb) and is called from 0x004e9285 with an
//            allocation of 0x4b0 bytes.

#ifndef tPlasmaLauncher_h
#define tPlasmaLauncher_h 1

#include "unsorted/tBaseWeapon.h"

// HYPOTHESIS: only the slots the launcher overrides are declared; each signature is read from
// its body (the arguments it takes from the stack and the register it returns in) and is
// checked against tBaseWeapon's declaration of the same slot, since a mismatch would add a
// virtual instead of overriding one. Slot 124 (vfunction125, 0x004e80b0, one int argument),
// slot 141 (vfunction142, 0x004e779b, two signed bytes, returns a float) are not declared
// either: tBaseWeapon.h declares both as void with no arguments (placeholders, unclaimed);
// fixing them is a tBaseWeapon.h edit left to the integrator. Slot 142 (0x004e7869, vfunction143) is not declared:
// tPhaserBank's exact override of it takes two floats, while this one reads a signed byte as
// its first argument, so the base's signature is not settled.
class tPlasmaLauncher : public tBaseWeapon
{
public:
    // 0x004e7949: (owner ship, a four-integer value); tBaseWeapon takes the first, the second is
    // copied to field_f8 (the kind, 0 to 7), field_100, field_104 and field_108.
    tPlasmaLauncher(int param_1, fn_004e1e99_arg* param_2);

    virtual unsigned mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode);
    virtual void vfunction39(float param_1);
    virtual void vfunction65(char param_1);
    virtual float vfunction74(float param_1, float param_2);
    virtual void vfunction80();
    virtual void vfunction105(int param_1);
    virtual float vfunction114(unsigned char param_1, int param_2, int param_3);
    virtual float vfunction118(unsigned char param_1);
    virtual void vfunction124(int param_1);
    virtual bool vfunction134();
    virtual int vfunction136(char param_1);
    virtual void vfunction137();
    // The two slots past the end of tBaseWeapon's vftable: 0x004e83f3 resets the part at 0x148
    // and fills it with a table of one entry (300.0f); 0x004e88c6 takes a flag byte and a value
    // and returns an int, a count or 0x20, 4 or 8.
    virtual void vfunction148();
    virtual int vfunction149(char param_1, int param_2);

    // Offsets 0x04 to 0x14b are tBaseWeapon's members.
    char pad14c[0x470 - 0x14c];
    // HYPOTHESIS: a flag, cleared by the constructor.
    bool field_470;
    char pad471[0x488 - 0x471];
    float field_488;
    float field_48c;
    char pad490[0x494 - 0x490];
    float field_494;
    float field_498;
    char pad49c[0x4a0 - 0x49c];
    double field_4a0;
    // HYPOTHESIS: a dword 0x004e80b0 tests against 0; the constructor does not set it.
    int field_4a8;
    char pad4ac[0x4b0 - 0x4ac];
};

typedef char tPlasmaLauncher_size_is_0x4b0[ sizeof( tPlasmaLauncher ) == 0x4b0 ? 1 : -1 ];

#endif	// tPlasmaLauncher_h
