//	tShuttleBaySystem.h: tShuttleBaySystem, the ship's shuttle bay.
// Evidence:  class (functions 0x0044e554 to 0x0044f16f; original file unknown)

#ifndef tShuttleBaySystem_h
#define tShuttleBaySystem_h 1

#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"
#include <string>

// HYPOTHESIS: the base is tRepairableShipSystem, as for the other ship systems; the fields
// between +0x7c and +0xa0 are not recovered.
class tShuttleBaySystem : public tRepairableShipSystem
{
public:
    virtual ~tShuttleBaySystem();
    virtual void vfunction100();
    virtual void vfunction101();
    // vftable slot 101; returns the address of field_a0.
    virtual int* vfunction102();
    // vftable slot 102; returns the address of field_c4.
    virtual tValue<long>* vfunction103();
    // vftable slot 103; returns field_b8.
    virtual int vfunction104();
    // vftable slot 104; returns field_bc.
    virtual int vfunction105();
    // vftable slot 105; returns field_c0.
    virtual int vfunction106();
    // vftable slot 106; returns field_c4's value.
    virtual int vfunction107();
    // vftable slot 107; returns field_e8.
    virtual int vfunction108();
    // vftable slot 108; returns field_f4.
    virtual int vfunction109();
    // vftable slot 109; returns element param_1 of field_2c8.
    virtual int vfunction110(int param_1);
    // vftable slot 110; stores param_2 into element param_1 of field_2c8.
    virtual void vfunction111(int param_1, int param_2);
    // Returns a copy of element param_1 of field_2d8.
    virtual std::string vfunction112(short param_1);
    // HYPOTHESIS: the slots between 111 and 116 are not recovered; vfunction116 is the
    // analysis's name. Returns element param_1 of an int array.
    virtual int vfunction116(int param_1);
    // vftable slot 117; returns element param_1 of an int array.
    virtual int vfunction117(int param_1);
    // HYPOTHESIS: the slots between 117 and 126 are not recovered; vfunction126 is the
    // analysis's name. Stores param_1 into a member.
    virtual void vfunction126(int param_1);
    // vftable slot 127; returns the member vfunction126 sets.
    virtual int vfunction127();
    // vftable slot 128; sets field_c4's value.
    virtual void vfunction128(long param_1);
    // vftable slot 129; stores param_1 into field_e8.
    virtual void vfunction129(int param_1);
    // vftable slot 130; stores param_1 into field_f4.
    virtual void vfunction130(int param_1);
    // vftable slot 131; stores param_2 into element param_1 of an int array.
    virtual void vfunction131(int param_1, int param_2);
    // vftable slot 132; stores param_2 into element param_1 of an int array.
    virtual void vfunction132(int param_1, int param_2);
    // vftable slot 133; stores param_2 into element param_1 of an int array.
    virtual void vfunction133(int param_1, int param_2);
    // vftable slot 134; returns element param_1 of the array vfunction131 stores into.
    virtual int vfunction134(int param_1);
    // vftable slot 135; returns element param_1 of the array vfunction132 stores into.
    virtual int vfunction135(int param_1);
    // vftable slot 136; returns element param_1 of the array vfunction133 stores into.
    virtual int vfunction136(int param_1);

    char pad7c[0xa0 - 0x7c];
    // HYPOTHESIS: an object or array held by value; its type is not recovered.
    int field_a0;
    char pada4[0xb8 - 0xa4];
    int field_b8;
    int field_bc;
    int field_c0;
    // HYPOTHESIS: a tValue<long>: vfunction128 sets its value at +0x18 and calls the notify
    // step (0x006712c0) with (3, 0), the shape of tValue's assignment.
    tValue<long> field_c4;
    char pade0[0xe8 - 0xe0];
    int field_e8;
    char padec[0xf4 - 0xec];
    int field_f4;
    // The names above sit 4 bytes below the compiled offsets (field_f4 compiles to +0xf8);
    // from here on names give the compiled offset.
    char padfc[0x104 - 0xfc];
    // Set by vfunction126.
    int field_104;
    // HYPOTHESIS: an int array indexed by vfunction131; its length is not recovered.
    int field_108[1];
    char pad10c[0x118 - 0x10c];
    // HYPOTHESIS: an int array indexed by vfunction132; its length is not recovered.
    int field_118[1];
    char pad11c[0x128 - 0x11c];
    // HYPOTHESIS: an int array indexed by vfunction133; its length is not recovered.
    int field_128[1];
    char pad12c[0x138 - 0x12c];
    // HYPOTHESIS: an int array indexed by vfunction116; its length is not recovered.
    int field_138[1];
    char pad13c[0x248 - 0x13c];
    // HYPOTHESIS: an int array indexed by vfunction117; its length is not recovered.
    int field_248[1];
    char pad24c[0x2c8 - 0x24c];
    // HYPOTHESIS: an int array indexed by vfunction110; its length is not recovered.
    int field_2c8[1];
    char pad2cc[0x2d8 - 0x2cc];
    // HYPOTHESIS: a std::string array indexed by vfunction112; its length is not recovered.
    std::string field_2d8[1];
};

#endif	// tShuttleBaySystem_h
