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

    char pad7c[0xa0 - 0x7c];
    // HYPOTHESIS: an object or array held by value; its type is not recovered.
    int field_a0;
    char pada4[0xc4 - 0xa4];
    // HYPOTHESIS: an object or array held by value; its type is not recovered.
    int field_c4;
};

#endif	// tShuttleBaySystem_h
