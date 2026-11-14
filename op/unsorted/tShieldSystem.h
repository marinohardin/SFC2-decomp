//	tShieldSystem.h: tShieldSystem, a ship's shield system (a repairable ship system).
// Evidence:  class (original file unknown)

#ifndef tShieldSystem_h
#define tShieldSystem_h 1

#include "unsorted/tIndividualShipSystem.h"
struct tShipSystemEntry;
class tShieldSystem : public tRepairableShipSystem
{
public:
    // vftable slot 22
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    // vftable slot 46
    virtual void vfunction47(int value);
    // vftable slot 51
    virtual void vfunction52();
    // vftable slot 53
    virtual void vfunction54(int param_1);
    // vftable slot 61
    virtual void vfunction62();

    // HYPOTHESIS: slots 64..80 belong to a base class; they are declared here, in slot order,
    // because the shared header stops at slot 63 and tCloakSystem declares its own slots
    // past it. Only slot 80 is called so far.
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
    // vftable slot 80
    virtual void vfunction81();
    // HYPOTHESIS: slots 81..107 are declared in slot order; their signatures are unknown
    // except slot 99 (vfunction100, returns a float) and slot 108 (vfunction109, yes/no).
    virtual void vfunction82();
    virtual void vfunction83();
    virtual void vfunction84();
    virtual void vfunction85();
    virtual void vfunction86();
    virtual void vfunction87();
    virtual void vfunction88();
    virtual void vfunction89();
    virtual void vfunction90();
    virtual void vfunction91();
    virtual void vfunction92();
    virtual void vfunction93();
    virtual void vfunction94();
    virtual void vfunction95();
    virtual void vfunction96();
    virtual void vfunction97();
    virtual void vfunction98();
    virtual void vfunction99();
    // vftable slot 99
    virtual float vfunction100();
    virtual void vfunction101();
    virtual void vfunction102();
    virtual void vfunction103();
    virtual void vfunction104();
    virtual void vfunction105();
    // vftable slot 105
    virtual int vfunction106();
    virtual int vfunction107();
    // HYPOTHESIS: vftable slot 107; yes/no, tested against true by tShieldGroup slot 100.
    virtual bool vfunction108();
    // vftable slot 108
    virtual bool vfunction109();

    // HYPOTHESIS: the per-turn update called by the group with the elapsed time, the group's
    // minimum shield level and its four counters.
    void fn_0043ff2d(float param_1, float param_2, int* param_3, int* param_4, int* param_5, int* param_6);
    // HYPOTHESIS: called when the shield goes off line.
    void fn_00440224();
    // HYPOTHESIS: called when the shield comes on line.
    void fn_0044036b();
    tShieldSystem(fn_owner_type* pOwner, tIndividualShipSystem* pGroup, const tShipSystemEntry* pEntry, int nFlag);
    // HYPOTHESIS: a state byte compared with tIndividualShipSystem's byte at +0x45.
    bool field_80;
    // HYPOTHESIS: saved as "fReinforced".
    bool field_81;
    // HYPOTHESIS: a changed flag, set when field_45 and field_80 differ; saved as
    // "fSwitchingOnLineStatus".
    bool field_82;
    char pad83[0x84 - 0x83];
    // HYPOTHESIS: saved as "fSwitchingOnLineTime".
    float field_84;
    // HYPOTHESIS: set when reinforcement switching starts; cleared while reinforced.
    bool field_88;
    char pad89[0x8c - 0x89];
    // HYPOTHESIS: saved as "fSwitchingReinforcedTime".
    float field_8c;
};

#endif
