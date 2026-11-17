//	tShuttleBaySystem.h: tShuttleBaySystem, the ship's shuttle bay.
// Evidence:  class (functions 0x0044e554 to 0x0044f16f; original file unknown)

#ifndef tShuttleBaySystem_h
#define tShuttleBaySystem_h 1

#include "unsorted/tIndividualShipSystem.h"

// HYPOTHESIS: the base is tRepairableShipSystem, as for the other ship systems; the fields
// between +0x7c and +0xa0 are not recovered.
class tShuttleBaySystem : public tRepairableShipSystem
{
public:
    virtual void vfunction100();
    virtual void vfunction101();
    // vftable slot 101; returns the address of field_a0.
    virtual int* vfunction102();
    // vftable slot 102; returns the address of field_c4.
    virtual int* vfunction103();
    // vftable slot 103; returns field_b8.
    virtual int vfunction104();
    // vftable slot 104; returns field_bc.
    virtual int vfunction105();
    // vftable slot 105; returns field_c0.
    virtual int vfunction106();
    // vftable slot 106; returns field_dc.
    virtual int vfunction107();
    // vftable slot 107; returns field_e8.
    virtual int vfunction108();
    // vftable slot 108; returns field_f4.
    virtual int vfunction109();

    char pad7c[0xa0 - 0x7c];
    // HYPOTHESIS: an object or array held by value; its type is not recovered.
    int field_a0;
    char pada4[0xb8 - 0xa4];
    int field_b8;
    int field_bc;
    int field_c0;
    // HYPOTHESIS: an object or array held by value; its type is not recovered.
    int field_c4;
    char padc8[0xdc - 0xc8];
    int field_dc;
    char pade0[0xe8 - 0xe0];
    int field_e8;
    char padec[0xf4 - 0xec];
    int field_f4;
};

#endif	// tShuttleBaySystem_h
