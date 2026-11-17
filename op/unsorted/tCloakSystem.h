//	tCloakSystem.h: tCloakSystem, the ship's cloaking device (a repairable ship system).
// Evidence:  class (constructor 0x004381e4; original file unknown)

#ifndef tCloakSystem_h
#define tCloakSystem_h 1

#include "unsorted/tIndividualShipSystem.h"

class tCloakSystem : public tRepairableShipSystem
{
public:
    tCloakSystem(int param_1, int param_2, int param_3, int param_4);
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    virtual void vfunction39(float param_1);
    virtual void vfunction40(float param_1);
    // vftable slot 61; vfunction63 is slot 62 (vfunction39 dispatches to it through the
    // vftable, the constructor calls it directly).
    virtual void vfunction62();
    virtual void vfunction63();
    virtual void vfunction100(int param_1);
    virtual int vfunction102(bool* param_1);
    virtual void vfunction103(int param_1);
    virtual float vfunction72();
    virtual float vfunction76(float param_1, int param_2);
    bool fn_004382cb();
    float fn_004382e3();
    void fn_00438390();
    // HYPOTHESIS: a non-virtual member (this in ecx) taking the accumulated elapsed time.
    void fn_0043d696(float elapsed);
    float mDetermineCloakWeaponRange(float range, int param_2);
    float mDetermineCloakSeekingWeaponDamageModifier(int param_1, int param_2);

    bool field_80;
    int field_84;
    double field_88;
    double field_90;
    int field_98;
};

#endif	// tCloakSystem_h
