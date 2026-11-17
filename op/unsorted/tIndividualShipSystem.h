//	tIndividualShipSystem.h: tIndividualShipSystem and tRepairableShipSystem, the ship system
//	bases shared by tCloakSystem, tShieldSystem and the other ship systems.
// Evidence:  class (original file unknown)

#ifndef tIndividualShipSystem_h
#define tIndividualShipSystem_h 1

// HYPOTHESIS: returns a handle stored at +0x18; called with the system's display name.
int __cdecl fn_00453c60(const char* name, int a2, int a3);

struct vfunction33_result
{
    char pad00[0x1c];
    int field_1c;
    char pad20[0x110 - 0x20];
    bool field_110;
    char pad111[0x86c - 0x111];
    // HYPOTHESIS: a kind or index read by tShieldSystem to pick a sound event.
    int field_86c;
    // HYPOTHESIS: a kind; tShieldGroup fixes its reinforcement capacitor at 100 when it is 0x13.
    int field_870;
    char pad874[0x944 - 0x874];
    // HYPOTHESIS: a 1-based index read by tShieldGroup to pick its minimum shield level.
    int field_944;
};

// HYPOTHESIS: a global object pointer at 0x00967af4 whose vftable slot 3 answers a yes/no
// question (the cloak is not engaged while it says yes). Read by tCloakSystem and tShieldGroup.
class fn_00967af4_type
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual bool vfunction4(); // slot 3
};
extern fn_00967af4_type* g_00967af4;

// HYPOTHESIS: the owning ship; vftable slot 43 answers a yes/no question and slot 107 returns
// an object with an id at +0x1c.
class fn_owner_type
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual void vfunction17();
    virtual void vfunction18();
    virtual void vfunction19();
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    virtual void vfunction23();
    virtual void vfunction24();
    virtual void vfunction25();
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    virtual void vfunction33();
    virtual void vfunction34();
    virtual void vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual void vfunction41();
    virtual void vfunction42();
    virtual void vfunction43();
    virtual bool vfunction44(); // slot 43
    virtual void vfunction45();
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual void vfunction51();
    virtual void vfunction52();
    virtual void vfunction53();
    virtual void vfunction54();
    virtual void vfunction55();
    virtual void vfunction56();
    virtual void vfunction57();
    virtual void vfunction58();
    virtual void vfunction59();
    virtual void vfunction60();
    virtual void vfunction61();
    virtual void vfunction62();
    virtual void vfunction63();
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
    virtual void vfunction100();
    virtual void vfunction101();
    virtual void vfunction102();
    virtual void vfunction103();
    virtual void vfunction104();
    virtual void vfunction105();
    virtual void vfunction106();
    // HYPOTHESIS: slot 106 takes one float; tShieldGroup::vfunction106 passes it 0.
    virtual void vfunction107(float param_1);
    virtual struct vfunction33_result* vfunction108(); // slot 107
};

// HYPOTHESIS: a global object at 0x0090be9c; vftable slot 6 returns an unsigned count and
// +0x20 holds an unsigned total.
#include "unsorted/fn_0090be9c_type.h"















// HYPOTHESIS: a double global at 0x00c7c148 holding the current time.
extern double g_00c7c148;

// HYPOTHESIS: a global object pointer at 0x0084b47c whose vftable slot 6 takes an event id
// and a value.
class fn_0084b47c_type
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7(int event, int value); // slot 6
};
extern fn_0084b47c_type* g_0084b47c;

#include "unsorted/tListMember.h"



class tShieldSystem;

class tIndividualShipSystem : public tListMember
{
public:
    // body at 0x004533e2
    tIndividualShipSystem();
    // 0x00453471: stores this class's vftable (0x0079e124), as the constructor does.
    virtual ~tIndividualShipSystem();
    // vftable slots 1..45, not yet identified
    virtual void vfunction2();
    virtual void vfunction3();
    virtual tListMember *vfunction4();
    virtual tListMember *vfunction5();
    virtual void vfunction6(tListMember *aMember);
    virtual void vfunction7(tListMember *aMember);
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual int vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    // HYPOTHESIS: slot 13; says whether the type index at +0xc is in the range 0..0x35.
    virtual bool vfunction14();
    // HYPOTHESIS: slot 14; says whether the type index at +0xc is 0x36.
    virtual bool vfunction15();
    // HYPOTHESIS: slot 15; returns the dword at +0x20.
    virtual int vfunction16();
    virtual void vfunction17();
    // HYPOTHESIS: slot 17; stores its argument in the dword at +0x18.
    virtual void vfunction18(int param_1);
    // HYPOTHESIS: slot 18; stores its argument in the dword at +0x20 (read by slot 15) and keeps the global
    // at 0x0090b518 above every value stored.
    virtual void vfunction19(int param_1);
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    // HYPOTHESIS: slots 22 and 23 take a stream, a flag word and a context and return the
    // flag word; the flags select saving (0x08, or 0x80 without 0x20) or loading (0x40).
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    virtual unsigned int vfunction24(int param_1, unsigned int param_2, int param_3);
    virtual void vfunction25();
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    // HYPOTHESIS: slot 32 returns the owning object (an int at +0x1c is read from it).
    virtual struct vfunction33_result* vfunction33();
    virtual void vfunction34();
    virtual void vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual void vfunction41();
    virtual void vfunction42();
    // HYPOTHESIS: slot 42; returns the owning ship at +0x2c.
    virtual fn_owner_type* vfunction43();
    virtual void vfunction44(int param_1);
    // HYPOTHESIS: slot 44; forwards its argument to slot 43.
    virtual void vfunction45(int param_1);
    // HYPOTHESIS: slot 45; says whether the system is in a list (pointer at +0x24 set).
    virtual bool vfunction46();
    virtual void vfunction47(int value); // slot 46
    // HYPOTHESIS: vftable slot 47; takes an amount of damage off the system's level
    // (tShieldGroup slot 88 passes each shield at most its current level).
    virtual void vfunction48(int param_1);
    virtual void vfunction49(int value); // slot 48
    virtual void vfunction50();
    virtual void vfunction51();
    // vftable slot 51; tIndividualShipSystem's own body is at 0x0043d66b.
    virtual void vfunction52();
    virtual void vfunction53();
    // vftable slot 53; tIndividualShipSystem's own body is at 0x0043d68f.
    virtual void vfunction54(int param_1);
    // HYPOTHESIS: vftable slots 54 and 56 take a level; tShieldGroup passes each shield the
    // same integer to both.
    virtual void vfunction55(int param_1);
    // HYPOTHESIS: vftable slot 55; returns the system's current level as an int (tShieldGroup
    // reads each shield's through it).
    virtual int vfunction56();
    virtual void vfunction57(int param_1);
    virtual void vfunction58();
    virtual void vfunction59(int param_1);
    virtual void vfunction60();
    // HYPOTHESIS: slot 60; stores its argument in the dword at +0x1c.
    virtual void vfunction61(int param_1);
    virtual void vfunction62(); // slot 61
    virtual void vfunction63(); // slot 62
    // HYPOTHESIS: slot 63; returns the flag byte at +0x45.
    virtual bool vfunction64();
    virtual void vfunction65(char param_1);
    // HYPOTHESIS: slot 65; returns the byte at +0x30 set by slot 64.
    virtual char vfunction66();
    // HYPOTHESIS: slot 66; stores its argument in the dword at +0x38.
    virtual void vfunction67(float param_1);
    // HYPOTHESIS: slot 67; returns the float at +0x38 stored by slot 66.
    virtual float vfunction68();
    // HYPOTHESIS: slot 68; stores its argument in the dword at +0x34.
    virtual void vfunction69(int param_1);
    virtual void vfunction70();
    // HYPOTHESIS: slot 70; stores its argument in the float at +0x40 (tCloakSystem writes it as a float).
    virtual void vfunction71(float param_1);
    virtual void vfunction72();
    virtual void vfunction73();
    // HYPOTHESIS: slot 73 takes two floats; slot 74 passes its float and 0.0.
    virtual void vfunction74(float param_1, float param_2);
    virtual void vfunction75(float param_1);
    virtual void vfunction76();
    virtual void vfunction77();
    virtual void vfunction78();
    virtual float vfunction79();
    virtual void vfunction80();
    virtual void vfunction81();
    virtual void vfunction82();
    // HYPOTHESIS: slot 82; returns the byte at +0x44.
    virtual bool vfunction83();
    // HYPOTHESIS: slot 83; stores its argument in the byte at +0x44 read by slot 82.
    virtual void vfunction84(bool param_1);

    char pad04[0x0c - 0x04];
    int field_0c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    // HYPOTHESIS: the group's list of member systems; tShieldGroup's constructor allocates it.
    class tIndividualShipSystemList* field_24;
    int field_28;
    // HYPOTHESIS: the owning ship (set from the constructor's first argument).
    class fn_owner_type* field_2c;
    // HYPOTHESIS: a byte set by slot 64 (vfunction65).
    char field_30;
    char pad31[0x34 - 0x31];
    int field_34;
    // HYPOTHESIS: a float; slot 67 returns it with fld, and tBaseWeapon's slot 66 takes a float.
    float field_38;
    // HYPOTHESIS: a float; slot 78 returns it with fld.
    float field_3c;
    float field_40;
    bool field_44;
    bool field_45;
    char pad46[0x48 - 0x46];
    double field_48;
    float field_50;
    // The double at +0x48 aligns the class to 8, so derived members start at +0x58:
    // tRepairableShipSystem's constructor stores its own fields from +0x58 on.
    char pad54[0x58 - 0x54];
};

class tRepairableShipSystem : public tIndividualShipSystem
{
public:
    tRepairableShipSystem();
    // vftable slot 22; saves or loads the repair fields (body at 0x0043d8b4).
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    // HYPOTHESIS: slots 84, 85 and 87..90 are declared here, in slot order, so that slot 93
    // is vfunction94; their signatures are unknown.
    virtual void vfunction85();
    virtual void vfunction86();
    // vftable slot 86
    virtual float vfunction87();
    virtual void vfunction88();
    virtual void vfunction89();
    virtual void vfunction90();
    virtual void vfunction91();
    // HYPOTHESIS: vftable slot 91; stores its argument into field_78.
    virtual void vfunction92(int param_1);
    // HYPOTHESIS: vftable slot 92; returns field_78.
    virtual int vfunction93();
    // vftable slot 93 (the compiler calls it directly inside a constructor).
    virtual void vfunction94(int param_1, int param_2);
    // vftable slot 94
    virtual float vfunction95();

    // HYPOTHESIS: types follow the constructor's stores (fstp for floats, a register for the
    // rest); the meanings are not known.
    int field_58;
    int field_5c;
    float field_60;
    // HYPOTHESIS: a factor; the constructor sets it to 1.0.
    float field_64;
    float field_68;
    int field_6c;
    float field_70;
    int field_74;
    int field_78;
};

#endif
