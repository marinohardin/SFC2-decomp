//	tShieldGroup.h: tShieldGroup, the group of a ship's shield systems.
// Evidence:  class (original file unknown)

#ifndef tShieldGroup_h
#define tShieldGroup_h 1

#include "unsorted/tIndividualShipSystem.h"
struct tShipSystemEntry;
class tShieldGroup : public tIndividualShipSystem
{
public:
    tShieldGroup(int param_1, int param_2, int param_3, int param_4);
    ~tShieldGroup();
    // MATCH-ONLY: stands in for the compiler-generated scalar deleting destructor (vftable slot
    // 0); the class declares no destructor of its own.
    tShieldGroup* scalar_deleting_destructor(unsigned char flags);
    // vftable slot 22; saves or loads the group's fields.
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    // HYPOTHESIS: slots 63..103 belong to this class or a base class; they are declared here,
    // in slot order, because the shared header stops at slot 62. Their signatures are unknown.
    virtual void vfunction64();
    virtual void vfunction65();
    virtual void vfunction66();
    virtual void vfunction67();
    virtual void vfunction68();
    virtual void vfunction69();
    virtual void vfunction70();
    virtual void vfunction71();
    virtual void vfunction72();
    virtual void vfunction73();
    virtual void vfunction74();
    virtual void vfunction75();
    virtual void vfunction76();
    virtual void vfunction77();
    virtual void vfunction78();
    virtual void vfunction79();
    virtual void vfunction80();
    virtual void vfunction81();
    virtual void vfunction82();
    virtual void vfunction83();
    virtual void vfunction84();
    // vftable slot 84; returns field_7c (HYPOTHESIS: the requested charge per turn).
    virtual float vfunction85();
    virtual void vfunction86(float param_1, float param_2);
    // HYPOTHESIS: vftable slot 86; sets a level on each of the six shields, one float each.
    virtual void vfunction87(float param_1, float param_2, float param_3, float param_4, float param_5, float param_6);
    // HYPOTHESIS: vftable slot 87; reads back a level from each of the six shields.
    virtual void vfunction88(float* param_1, float* param_2, float* param_3, float* param_4, float* param_5, float* param_6);
    // vftable slot 88
    virtual int vfunction89(unsigned int param_1, int param_2);
    // HYPOTHESIS: vftable slot 89; sets the minimum shield level and returns the total
    // desired energy.
    virtual float vfunction90(float param_1);
    // HYPOTHESIS: vftable slot 90; returns the group's current shield level.
    virtual float vfunction91();
    // HYPOTHESIS: vftable slot 91; returns the desired reinforcement level (field_b4).
    virtual float vfunction92();
    // HYPOTHESIS: vftable slot 92, by its name; sets the desired reinforcement level.
    virtual float vfunction93(float param_1);
    virtual float vfunction94();
    virtual float vfunction95();
    // HYPOTHESIS: vftable slot 95; the reinforcement a facing's shield gets, scaled by the
    // table entry for the number of shields at level 64 or more, or 0 when that shield is not up.
    virtual int vfunction96(int param_1);
    // HYPOTHESIS: vftable slot 96; whether the shield for a facing id (55..60) is up at level 64
    // or more.
    virtual bool vfunction97(int param_1);
    virtual int vfunction98(int param_1);
    virtual void vfunction99(int param_1);
    virtual void vfunction100(int param_1);
    // HYPOTHESIS: vftable slot 100; counts the shields at level 64 or more.
    virtual int vfunction101();
    // HYPOTHESIS: vftable slot 101; counts the shields with a level above zero.
    virtual int vfunction102();
    virtual void vfunction103();
    virtual void vfunction104();
    // vftable slot 104
    virtual void vfunction105();
    virtual void vfunction106();
    // vftable slot 106
    virtual void vfunction107();
    virtual bool vfunction108();
    // HYPOTHESIS: vftable slot 108; adds the member shields from a table of entries; the
    // constructor calls it directly with its last two arguments.
    virtual void vfunction109(const tShipSystemEntry* param_1, int param_2);
    // HYPOTHESIS: reads and clears the changed flag (field_b8); whether it is virtual, and
    // its slot, are not known.
    bool fn_004406b9();
    // HYPOTHESIS: returns the member shield for a facing index; whether it is virtual, and its
    // slot, are not known.
    tShieldSystem* fn_00441226(int param_1);

    // HYPOTHESIS: saved as "fTrueOnline".
    bool field_58;
    bool field_59;
    char pad5a[0x5c - 0x5a];
    // HYPOTHESIS: the six member shields, one per facing; null when the facing has none.
    tShieldSystem* field_5c;
    tShieldSystem* field_60;
    tShieldSystem* field_64;
    tShieldSystem* field_68;
    tShieldSystem* field_6c;
    tShieldSystem* field_70;
    // HYPOTHESIS: saved as "fReinforcementCapacitor".
    float field_74;
    // HYPOTHESIS: saved as "fTotalDesiredEnergy".
    float field_78;
    // HYPOTHESIS: saved as "fRequestedChargePerTurn".
    float field_7c;
    float field_80;
    // HYPOTHESIS: a table of six levels (64.0 down to 32.0) set by the constructor.
    float field_84[6];
    // HYPOTHESIS: saved as "fLastPowerAccepted".
    float field_9c;
    // HYPOTHESIS: saved as "fNumberOnline".
    int field_a0;
    // HYPOTHESIS: saved as "fNumberOnlineOrSwitching".
    int field_a4;
    // HYPOTHESIS: saved as "fNumberReinforced".
    int field_a8;
    // HYPOTHESIS: saved as "fNumberThatWillBeReinforced".
    int field_ac;
    // HYPOTHESIS: saved as "fMinShieldLevel".
    float field_b0;
    // HYPOTHESIS: saved as "fDesiredReinforcementLevel".
    float field_b4;
    // HYPOTHESIS: a changed flag, set by a member shield when it goes on or off line.
    bool field_b8;
};

#endif
