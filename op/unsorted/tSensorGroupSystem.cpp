// Unit:      unsorted/tSensorGroupSystem.cpp
// Functions of tSensorGroupSystem (0x0043dfd4 to 0x0043f6c3); original file unknown.
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tEntity.h"
#include "Source/entity/entity_physical.h"
#include "unsorted/tMissionMap.h"

// HYPOTHESIS: the base is tRepairableShipSystem: the destructor stores vtable 0x00799598 (the
// one after tMinMaxFloat's) before calling tIndividualShipSystem's. Six 0x24-byte value
// objects follow at +0x8c; the fields between +0x80 (where the 8-aligned base ends) and +0x8c
// are not recovered.
class tSensorGroupSystem : public tRepairableShipSystem
{
public:
    virtual ~tSensorGroupSystem();

    // HYPOTHESIS: called first in the destructor; a cleanup step.
    void fn_0043e09e();
    // HYPOTHESIS: adds an entity ID to the list at +0x1e0.
    void fn_0043e0ed(int param_1);
    void fn_0043e201(float param_1);
    // HYPOTHESIS: a test on an entity ID; 0x0043e201 asks it for entities other than its owner.
    // HYPOTHESIS: returns the number of entries of field_1e0 in use.
    int fn_0043e351();
    void fn_0043e359(char param_1);
    // vftable slot 22; saves or loads the sensor fields.
    virtual unsigned int vfunction23(int param_1, unsigned int param_2, int param_3);
    void vfunction51();
    char fn_0043eb5f(int aID);
    void fn_0043e642(int param_1);
    void fn_0043e6ce(int param_1);
    float fn_0043e75a();
    float fn_0043e761();
    float fn_0043e768();
    float fn_0043e76f();
    float fn_0043e776();
    float fn_0043e77d();
    float fn_0043e78a(int param_1);
    void fn_0043e838();
    void fn_0043e853();
    // HYPOTHESIS: param_2 is not read.
    float fn_0043e870(float param_1, int param_2);
    char fn_0043e8da();
    char fn_0043e911(int aID);
    void fn_0043ec81();
    int fn_0043eca6(int param_1);
    void fn_0043eee8();
    // HYPOTHESIS: the result is an integer 0x0043eee8 converts to float.
    int fn_0043f023(struct vfunction33_result* owner, tObject_0042496f* target);
    // HYPOTHESIS: a value for one map cell; 0x0043f023 sums it along a line of cells.
    int fn_0043f618(int x, int y);
    virtual void vfunction39(float param_1);
    // HYPOTHESIS: param_2 is not read (the function pops two arguments).
    float fn_0043ed38(int param_1, int param_2);
    int fn_0043edd8();
    void fn_0043ee5a();

    // HYPOTHESIS: three objects 0x0043e366 asks a yes/no question (slot 9) and then calls at
    // slot 50; their type is not recovered.
    class fn_0043e366_part* field_80;
    class fn_0043e366_part* field_84;
    class fn_0043e366_part* field_88;
    tMinMaxFloat field_8c;
    tMinMaxFloat field_b0;
    tMinMaxFloat field_d4;
    tMinMaxFloat field_f8;
    tMinMaxFloat field_11c;
    tMinMaxFloat field_140;
    // HYPOTHESIS: the total ECM and ECCM 0x0043e642 shares between field_184 and field_188.
    int field_164;
    float field_168;
    float field_16c;
    float field_170;
    float field_174;
    float field_178;
    float field_17c;
    float field_180;
    int field_184;
    int field_188;
    // HYPOTHESIS: the answer 0x0043e201 uses for its owner's own entities.
    char field_18c;
    char pad18d[0x190 - 0x18d];
    float field_190;
    char pad194[0x198 - 0x194];
    // HYPOTHESIS: time accumulated by 0x0043f1c1 between sensor updates.
    double field_198;
    // HYPOTHESIS: the entity the owner's slot 11 names, as 0x0043eee8 last found it.
    tPhysicalEntity* field_1a0;
    char pad1a4[0x1d9 - 0x1a4];
    // HYPOTHESIS: the number of entries of field_1e0 in use (read with movsx).
    char field_1d9;
    char pad1da[0x1dc - 0x1da];
    // HYPOTHESIS: time accumulated by 0x0043e201.
    float field_1dc;
    // HYPOTHESIS: twelve entity IDs; 0x0043e09e looks each up in the entity table.
    int field_1e0[12];
};

// HYPOTHESIS: an entity found by ID whose vftable slot 26 takes one value.
class fn_0043e09e_entity
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
    virtual int vfunction12(); // slot 11
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
    virtual void vfunction27(int param_1); // slot 26
};

// FUNCTION: STARFLEETOP 0x0043dfd4
// FUNCTION: STARFLEETOP 0x0043dff0
tSensorGroupSystem::~tSensorGroupSystem()
{
    fn_0043e09e();
}

// FUNCTION: STARFLEETOP 0x0043e09e
void tSensorGroupSystem::fn_0043e09e()
{
    for (int i = 0; i < 12; i++)
    {
        // HYPOTHESIS: the entity looked up takes one argument at vftable slot 26, which
        // tPhysicalEntity's declaration does not show; the cast stands for that type.
        fn_0043e09e_entity* entity = (fn_0043e09e_entity*)gObject_008e1060.fn_004249d3(field_1e0[i]);
        if (entity != 0)
            entity->vfunction27(vfunction37(0, 100, 0, 0));
    }
}

// HYPOTHESIS: the object fn_004274f4 returns; tFleetEntry.cpp declares it as tObject_004274f4
// (only in that unit), whose fn_005254c2 this function calls with an entity ID.
class fn_004274f4_result
{
public:
    void fn_005254c2(int param_1);
};

// HYPOTHESIS: the object the pointer at 0x0096908c names; tFleetEntry.cpp and tFleetPanel.cpp
// declare it in their own units. Slot 5 plays a sound by name with a volume.
class fn_0096908c_type
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6(const char* name, const char* param_2, float param_3); // slot 5
};
extern fn_0096908c_type* g_0096908c;
extern const char g_sz0079bfbc[];
extern float g_00795bf0;

// FUNCTION: STARFLEETOP 0x0043e0ed
void tSensorGroupSystem::fn_0043e0ed(int param_1)
{
    vfunction33_result* owner = field_2c->vfunction108();
    if (gObject_008e1060.fn_004235f0(owner->field_1c))
    {
        fn_004274f4_result* orders = (fn_004274f4_result*)gObject_0090be60->fn_004274f4(owner->field_60);
        if (orders != 0)
        {
            orders->fn_005254c2(param_1);
            return;
        }
        int i;
        for (i = 0; i < field_1d9; i++)
        {
            if (field_1e0[i] == -1)
            {
                field_1e0[i] = param_1;
                return;
            }
        }
        int id = field_2c->vfunction108()->field_1c;
        if (gObject_008e1060.fn_00525081() == id)
            g_0096908c->vfunction6("MissileLost", g_sz0079bfbc, g_00795bf0);
        fn_0043e09e_entity* entity = (fn_0043e09e_entity*)gObject_008e1060.fn_004249d3(field_1e0[0]);
        if (entity != 0)
            entity->vfunction27(vfunction37(0, 100, 0, 0));
        for (i = 0; i < field_1d9 - 1; i++)
            field_1e0[i] = field_1e0[i + 1];
        field_1e0[i] = param_1;
    }
}

// FUNCTION: STARFLEETOP 0x0043e201
void tSensorGroupSystem::fn_0043e201(float param_1)
{
    field_1dc += param_1;
    vfunction33_result* owner = field_2c->vfunction108();
    if (gObject_008e1060.fn_004235f0(owner->field_1c) && gObject_0090be60->fn_004274f4(owner->field_60) == 0)
    {
        // HYPOTHESIS: the owner is an entity; its slot 11 returns its ID.
        int ownerID = ((tEntity*)owner)->vfunction12();
        int i;
        for (i = 0; i < field_1d9 - 1; i++)
        {
            fn_0043e09e_entity* entity = (fn_0043e09e_entity*)gObject_008e1060.fn_004249d3(field_1e0[i]);
            if (entity != 0)
            {
                bool known = entity->vfunction12() == ownerID ? field_18c != 0 : fn_0043eb5f(entity->vfunction12()) != 0;
                if (!known && field_1dc > 0.05f && g_0090be9c->fn_004105b4() <= 0.02f)
                {
                    field_1dc = 0.0f;
                    entity->vfunction27(vfunction37(0, 100, 0, 0));
                    field_1e0[i] = field_1e0[i + 1];
                    field_1e0[i + 1] = -1;
                }
            }
            else
            {
                field_1e0[i] = field_1e0[i + 1];
                field_1e0[i + 1] = -1;
            }
        }
        if (gObject_008e1060.fn_entity(field_1e0[i]) == 0)
            field_1e0[i] = -1;
    }
}

// FUNCTION: STARFLEETOP 0x0043e351
int tSensorGroupSystem::fn_0043e351()
{
    return field_1d9;
}

// FUNCTION: STARFLEETOP 0x0043e359
void tSensorGroupSystem::fn_0043e359(char param_1)
{
    field_1d9 = param_1;
}

// HYPOTHESIS: the type of field_80, field_84 and field_88.
class fn_0043e366_part
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
    virtual int vfunction10(); // slot 9
    virtual int vfunction11(); // slot 10
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
    virtual void vfunction44();
    virtual void vfunction45();
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual void vfunction51(); // slot 50

    int fn_0043da08();
};

// FUNCTION: STARFLEETOP 0x0043e366
void tSensorGroupSystem::vfunction51()
{
    if (field_80 != 0 && field_80->vfunction10())
        field_80->vfunction51();
    if (field_84 != 0 && field_84->vfunction10())
        field_84->vfunction51();
    if (field_88 != 0 && field_88->vfunction10())
        field_88->vfunction51();
}

// HYPOTHESIS: field serializers (cdecl): the field, the stream and context passed through,
// and the field's name; the last two take a value object.
void __cdecl fn_0040e9df(int* value, int param_1, int param_3, const char* name);
void __cdecl fn_0040eb35(float* value, int param_1, int param_3, const char* name);
void __cdecl fn_0040d11a(tMinMaxFloat* value, int param_1, int param_3, const char* name);
void __cdecl fn_0040d372(tMinMaxFloat* value, int param_1, int param_3, const char* name);

// FUNCTION: STARFLEETOP 0x0043e3d6
unsigned int tSensorGroupSystem::vfunction23(int param_1, unsigned int param_2, int param_3)
{
    if ((param_2 & 0x08) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040e9df(&field_184, param_1, param_3, "fDesiredECM");
        fn_0040e9df(&field_188, param_1, param_3, "fDesiredECCM");
        fn_0040d11a(&field_d4, param_1, param_3, "fDesiredECMStatus");
        fn_0040d11a(&field_f8, param_1, param_3, "fDesiredECCMStatus");
        fn_0040d372(&field_8c, param_1, param_3, "fECMStatus");
        fn_0040d372(&field_b0, param_1, param_3, "fECCMStatus");
        fn_0040d372(&field_11c, param_1, param_3, "fSensorEffectiveness");
        fn_0040d372(&field_140, param_1, param_3, "fTargetLock");
        fn_0040eb35(&field_190, param_1, param_3, "fTotalSensorHealthMultiplier");
        fn_0040eb35(&field_180, param_1, param_3, "fExternalECCM");
        fn_0040eb35(&field_17c, param_1, param_3, "fExternalECM");
        fn_0040eb35(&field_174, param_1, param_3, "fTargetECCM");
        fn_0040eb35(&field_178, param_1, param_3, "fEnviromentalECM");
        fn_0040eb35(&field_170, param_1, param_3, "fTargetECM");
        fn_0040eb35(&field_16c, param_1, param_3, "fECCM");
        fn_0040eb35(&field_168, param_1, param_3, "fECM");
    }
    else if ((param_2 & 0x20) > 0)
    {
    }
    else if ((param_2 & 0x80) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_168, param_1, param_3, "fECM");
        fn_0040eb35(&field_16c, param_1, param_3, "fECCM");
        fn_0040d11a(&field_d4, param_1, param_3, "fDesiredECMStatus");
        fn_0040d11a(&field_f8, param_1, param_3, "fDesiredECCMStatus");
    }
    else if ((param_2 & 0x40) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_168, param_1, param_3, "fECM");
        fn_0040eb35(&field_16c, param_1, param_3, "fECCM");
        fn_0040d372(&field_8c, param_1, param_3, "fECMStatus");
        fn_0040d372(&field_b0, param_1, param_3, "fECCMStatus");
        fn_0040eb35(&field_180, param_1, param_3, "fExternalECCM");
        fn_0040eb35(&field_17c, param_1, param_3, "fExternalECM");
        fn_0040d11a(&field_d4, param_1, param_3, "fDesiredECMStatus");
        fn_0040d11a(&field_f8, param_1, param_3, "fDesiredECCMStatus");
    }
    return param_2;
}

// FUNCTION: STARFLEETOP 0x0043e642
void tSensorGroupSystem::fn_0043e642(int param_1)
{
    if (param_1 != field_184)
    {
        field_184 = param_1;
        field_d4.vfunction2(param_1);
        if (field_184 + field_188 > field_164)
        {
            if (field_184 <= field_164)
            {
                field_188 = field_164 - field_184;
                field_f8.vfunction2(field_188);
            }
            else
            {
                field_188 = 0;
                field_f8.vfunction2(0);
                field_184 = field_164;
                field_d4.vfunction2(field_184);
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x0043e6ce
void tSensorGroupSystem::fn_0043e6ce(int param_1)
{
    if (param_1 != field_188)
    {
        field_188 = param_1;
        field_f8.vfunction2(param_1);
        if (field_184 + field_188 > field_164)
        {
            if (field_188 <= field_164)
            {
                field_184 = field_164 - field_188;
                field_d4.vfunction2(field_184);
            }
            else
            {
                field_184 = 0;
                field_d4.vfunction2(0);
                field_188 = field_164;
                field_f8.vfunction2(field_188);
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x0043e75a
float tSensorGroupSystem::fn_0043e75a()
{
    return field_168;
}

// FUNCTION: STARFLEETOP 0x0043e761
float tSensorGroupSystem::fn_0043e761()
{
    return field_16c;
}

// FUNCTION: STARFLEETOP 0x0043e768
float tSensorGroupSystem::fn_0043e768()
{
    return field_170;
}

// FUNCTION: STARFLEETOP 0x0043e76f
float tSensorGroupSystem::fn_0043e76f()
{
    return field_17c;
}

// FUNCTION: STARFLEETOP 0x0043e776
float tSensorGroupSystem::fn_0043e776()
{
    return field_180;
}

// FUNCTION: STARFLEETOP 0x0043e77d
float tSensorGroupSystem::fn_0043e77d()
{
    return field_168 - field_174;
}

// HYPOTHESIS: the object the owner's slot 78 returns; slot 100 returns a level from 1 to 5.
class fn_owner_slot78_result
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
    virtual void vfunction44();
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
    virtual int vfunction101(); // slot 100
};

// HYPOTHESIS: the object the owner's slot 79 returns; only its method at 0x0043c019 is known.
class fn_owner_slot79_result
{
public:
    char fn_0043c019();
};

// FUNCTION: STARFLEETOP 0x0043e78a
float tSensorGroupSystem::fn_0043e78a(int param_1)
{
    float value;
    if (param_1 == 0)
        value = field_16c - field_170 + field_178;
    else
        value = field_16c - field_178;
    fn_owner_slot78_result* object = field_2c->vfunction79();
    fn_owner_slot79_result* other = field_2c->vfunction80();
    if (object != 0 && other != 0 && other->fn_0043c019())
    {
        switch (object->vfunction101())
        {
        case 1:
            return value - 6.0f;
        case 2:
            return value - 5.0f;
        case 3:
            return value - 4.0f;
        case 4:
            return value - 3.0f;
        case 5:
            return value - 1.0f;
        }
    }
    return value;
}

// FUNCTION: STARFLEETOP 0x0043e838
void tSensorGroupSystem::fn_0043e838()
{
    tMinMaxFloat* value = &field_11c;
    value->vfunction3(fn_0043e77d());
}

// FUNCTION: STARFLEETOP 0x0043e853
void tSensorGroupSystem::fn_0043e853()
{
    tMinMaxFloat* value = &field_140;
    value->vfunction3(fn_0043e78a(0));
}

// HYPOTHESIS: a byte table of 4-byte entries, twelve to a row; 0x0043e870 reads the first
// byte of an entry.
extern char g_0079c080[][12][4];

// FUNCTION: STARFLEETOP 0x0043e870
float tSensorGroupSystem::fn_0043e870(float param_1, int param_2)
{
    int column = field_84->fn_0043da08();
    fn_0043e366_part* part = field_84;
    char value = g_0079c080[part->vfunction11() - part->vfunction10()][column][0];
    if (!field_18c)
        param_1 += param_1;
    return value * 10 + param_1;
}

// HYPOTHESIS: a byte table laid out like g_0079c080; 0x0043e8da reads the first byte of an
// entry.
extern char g_0079bfc0[][12][4];

// FUNCTION: STARFLEETOP 0x0043e8da
char tSensorGroupSystem::fn_0043e8da()
{
    int column = field_80->fn_0043da08();
    fn_0043e366_part* part = field_80;
    return g_0079bfc0[part->vfunction11() - part->vfunction10()][column][0];
}

// HYPOTHESIS: what slot 95 (offset 0x17c) of a ship's object at 0xad4 returns; tAICaptain.cpp
// declares it in its own unit as tObject_0042496f_member96. Slot 101 (offset 0x194) returns a
// state and sets the flag it is given.
class fn_0043e911_state
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
    virtual void vfunction44();
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
    virtual int vfunction102(bool* flag); // slot 101
};

// FUNCTION: STARFLEETOP 0x0043e911
char tSensorGroupSystem::fn_0043e911(int aID)
{
    tObject_0042496f* ship = (tObject_0042496f*)gObject_008e1060.fn_0042496f(aID);
    vfunction33_result* owner = field_2c->vfunction108();
    float range = 1000.0f;
    float strength = 0.0f;
    int level = field_80->vfunction10() / field_80->vfunction11() * 6;
    if (ship != 0)
    {
        strength = ship->vfunction63() * 0.1f;
        if (owner != 0)
            range = gObject_008e1060.fn_004246e4((tEntity*)owner, (tEntity*)ship) * 0.1f;
    }
    tPhysicalEntity* found = gObject_008e1060.fn_004249c0((tPhysicalEntity*)gObject_008e1060.fn_lookup(aID));
    if (found != 0)
    {
        tObject_0042496f* target = (tObject_0042496f*)found;
        fn_0043e911_state* state = (fn_0043e911_state*)target->field_0xad4->vfunction96();
        tPhysicalEntity* self = gObject_008e1060.fn_004249d3(field_2c->vfunction108()->field_1c);
        if (self != 0 && self->vfunction35(found->fID))
            return 1;
        if (state != 0)
        {
            bool flag;
            int mode = state->vfunction102(&flag);
            if (!flag && mode == 6)
            {
                int rangeLevel;
                if (range < 1.0f)
                    rangeLevel = -1;
                else if (range < 5.0f)
                    rangeLevel = 0;
                else if (range < 11.0f)
                    rangeLevel = 1;
                else if (range < 16.0f)
                    rangeLevel = 2;
                else if (range < 21.0f)
                    rangeLevel = 3;
                else if (range < 31.0f)
                    rangeLevel = 4;
                else if (range < 41.0f)
                    rangeLevel = 5;
                else
                    rangeLevel = 6;
                int strengthLevel;
                if (strength < 1.0f)
                    strengthLevel = -2;
                else if (strength < 5.0f)
                    strengthLevel = 0;
                else if (strength < 9.0f)
                    strengthLevel = 1;
                else if (strength < 13.0f)
                    strengthLevel = 2;
                else if (strength < 16.0f)
                    strengthLevel = 3;
                else if (strength < 18.0f)
                    strengthLevel = 4;
                else if (strength < 19.0f)
                    strengthLevel = 5;
                else
                    strengthLevel = 6;
                if (level - rangeLevel - 4 + strengthLevel >= 2)
                    return 1;
            }
        }
    }
    return 0;
}

extern double g_00799f68;

// FUNCTION: STARFLEETOP 0x0043eb5f
char tSensorGroupSystem::fn_0043eb5f(int aID)
{
    int column = field_80->fn_0043da08();
    fn_0043e366_part* part = field_80;
    char value = g_0079bfc0[part->vfunction11() - part->vfunction10()][column][0];
    tPhysicalEntity* found = gObject_008e1060.fn_004249c0((tPhysicalEntity*)gObject_008e1060.fn_lookup(aID));
    bool hidden = false;
    if (found != 0)
    {
        tObject_0042496f* target = (tObject_0042496f*)found;
        fn_0043e911_state* state = (fn_0043e911_state*)target->field_0xad4->vfunction96();
        tPhysicalEntity* self = gObject_008e1060.fn_004249d3(field_2c->vfunction108()->field_1c);
        if (self != 0)
        {
            int id = found->fID;
            if (self->vfunction35(id))
                return 1;
        }
        if (state != 0)
        {
            bool flag;
            int mode = state->vfunction102(&flag);
            if (!flag && mode == 6)
                hidden = true;
        }
    }
    fn_0090be9c_type* source = g_0090be9c;
    char result = value >= (unsigned int)((double)source->vfunction7() * g_00799f68 / (double)source->field_20);
    if (hidden == true)
        result = 0;
    return result;
}

// FUNCTION: STARFLEETOP 0x0043ec81
void tSensorGroupSystem::fn_0043ec81()
{
    // HYPOTHESIS: the owner is an entity; its slot 11 returns its ID.
    field_18c = fn_0043eb5f(((tEntity*)field_2c->vfunction108())->vfunction12());
}

// HYPOTHESIS: a free function taking a float; its result type is not recovered.
int fn_0043ecde(float param_1);

// FUNCTION: STARFLEETOP 0x0043eca6
int tSensorGroupSystem::fn_0043eca6(int param_1)
{
    fn_0043eee8();
    float value = fn_0043e78a(param_1);
    int result = 0;
    if (value < 0.0f)
        result = fn_0043ecde(-value);
    return result;
}

extern float g_0079bfb8;

// FUNCTION: STARFLEETOP 0x0043ecde
int fn_0043ecde(float param_1)
{
    // 1.5 * 2^23: adding it leaves the rounded value in the low bits.
    float rounded = param_1 + g_0079bfb8;
    int n = (short&)rounded;
    int result = 0;
    int i = 2;
    if (n >= 1)
    {
        result = 1;
        while (n >= i * i)
        {
            i++;
            result++;
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0043ed12
int fn_0043ed12(int param_1)
{
    int result = 0;
    int i = 2;
    if (param_1 > 0)
    {
        param_1--;
        result = 1;
        int square = 4;
        for (; param_1 > 0; param_1--)
        {
            i++;
            result = square;
            square = i * i;
        }
    }
    return result;
}

extern double g_0079a148;
extern float g_007959cc;
extern float g_007959c8;
extern float g_00795bf0;

// FUNCTION: STARFLEETOP 0x0043ed38
float tSensorGroupSystem::fn_0043ed38(int param_1, int param_2)
{
    fn_0043eee8();
    int value = (int)-(fn_0043e78a(0) + param_1);
    int level = 0;
    int limit = 7;
    int step = 6;
    if (value > 0)
    {
        level = 1;
        while (value > limit)
        {
            limit += step;
            step += 4;
            level++;
        }
        fn_0090be9c_type* source = g_0090be9c;
        level -= (int)((double)source->vfunction7() * g_0079a148 / (double)source->field_20);
    }
    if (level <= 5)
        return g_007959cc;
    if (level <= 7)
        return g_007959c8;
    if (level <= 9)
        return g_00795bf0;
    return 0.0f;
}

// HYPOTHESIS: the owner slot 32 returns, seen as an object whose slot 58 (offset 0xe8) returns
// a count; 0x0043edd8 adds 6 while it is positive. The shared vfunction33_result has no vftable.
class fn_0043edd8_owner
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
    virtual void vfunction44();
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
    virtual int vfunction59(); // slot 58
};

// FUNCTION: STARFLEETOP 0x0043edd8
int tSensorGroupSystem::fn_0043edd8()
{
    fn_0043eee8();
    int low = (int)field_174;
    int high = (int)(field_17c + field_168);
    if (((fn_0043edd8_owner*)vfunction33())->vfunction59() > 0)
        high += 6;
    int diff = high - low;
    if (high <= 0)
        return 0;
    if (diff >= 25)
        return 5;
    else if (diff >= 16)
        return 4;
    else if (diff >= 9)
        return 3;
    else if (diff >= 4)
        return 2;
    else if (diff >= 1)
        return 1;
    return 0;
}

// HYPOTHESIS: 0x0043ee5a pushes a float to slot 2 of field_8c and field_b0, while 0x0043e642
// pushes an int to the same slot of field_d4; all six share tMinMaxFloat's vftable, so the
// slot's real parameter type is not settled. This view gives slot 2 a float.
class fn_0043ee5a_value
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3(float param_1); // slot 2
};

// FUNCTION: STARFLEETOP 0x0043ee5a
void tSensorGroupSystem::fn_0043ee5a()
{
    if (field_184 + field_188 > field_164)
    {
        if (field_184 <= field_164)
        {
            field_188 = (int)(field_164 - field_168);
            ((fn_0043ee5a_value*)&field_b0)->vfunction3(field_16c);
        }
        else
        {
            field_188 = 0;
            ((fn_0043ee5a_value*)&field_b0)->vfunction3(field_16c);
            field_184 = field_164;
            ((fn_0043ee5a_value*)&field_8c)->vfunction3(field_168);
        }
    }
}

// HYPOTHESIS: the object at 0xad4 of a ship, seen from 0x0043eee8: slot 84 (offset 0x150)
// returns the ship's sensor system, on which it calls this class's readers.
class fn_0043eee8_member
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
    virtual void vfunction44();
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
    virtual void vfunction80(); // slot 79
    virtual void vfunction81();
    virtual void vfunction82();
    virtual void vfunction83();
    virtual void vfunction84();
    virtual tSensorGroupSystem* vfunction85(); // slot 84
};

// FUNCTION: STARFLEETOP 0x0043eee8
void tSensorGroupSystem::fn_0043eee8()
{
    vfunction33_result* owner = field_2c->vfunction108();
    tPhysicalEntity* found = gObject_008e1060.fn_004249c0((tPhysicalEntity*)gObject_008e1060.fn_lookup(((tEntity*)owner)->vfunction12()));
    if (field_1a0 != found)
        fn_0043ec81();
    field_1a0 = found;
    if (found != 0)
    {
        tObject_0042496f* target = (tObject_0042496f*)found;
        tSensorGroupSystem* sensors = ((fn_0043eee8_member*)target->field_0xad4)->vfunction85();
        ((fn_0043eee8_member*)target->field_0xad4)->vfunction80();
        if (!target->fn_00419c87() && sensors != 0)
        {
            field_170 = sensors->fn_0043e76f() + sensors->fn_0043e75a();
            if (((fn_0043edd8_owner*)target)->vfunction59() != -1)
                field_170 += 6.0f;
            field_174 = sensors->fn_0043e776() + sensors->fn_0043e761();
            field_178 = (float)fn_0043f023(owner, target);
        }
        else
        {
            if (target->field_0x870 == 0x16)
            {
                field_170 = 2.0f;
                field_174 = 2.0f;
            }
            else
            {
                field_170 = 0.0f;
                field_174 = 0.0f;
            }
            field_178 = (float)fn_0043f023(owner, target);
        }
    }
}

// HYPOTHESIS: sums fn_0043f618 over the map cells on a line from the owner's cell to the
// target's; 0x0043eee8 stores the result as a float.
// FUNCTION: STARFLEETOP 0x0043f023
int tSensorGroupSystem::fn_0043f023(vfunction33_result* owner, tObject_0042496f* target)
{
    int sum = 0;
    if (owner == 0 || target == 0 || (void*)owner == (void*)target)
        return 0;
    tPoint3 from = ((tEntity*)owner)->fCurrPos;
    tPoint3 to = target->field_0x34;
    unsigned int x1, y1, x2, y2, t;
    tMapPoint cell = g_0090b734->m3DToMapPoint(from);
    x1 = cell.x;
    y1 = cell.y;
    cell = g_0090b734->m3DToMapPoint(to);
    y2 = cell.y;
    x2 = cell.x;
    if (y2 < y1)
    {
        t = x1;
        x1 = x2;
        x2 = t;
        t = y1;
        y1 = y2;
        y2 = t;
    }
    int dy = y2 - y1;
    int dx = x2 - x1;
    int i;
    if (dx == 0)
    {
        for (i = 0; i < dy + 1; i++)
        {
            sum += fn_0043f618(x1, y1);
            y1++;
        }
    }
    else if (dy == 0)
    {
        if (x1 > x2)
        {
            t = x1;
            x1 = x2;
            x2 = t;
        }
        int count = x2 - x1 + 1;
        for (i = 0; i < count; i++)
        {
            sum += fn_0043f618(x1, y1);
            x1++;
        }
    }
    else
    {
        int adx = dx;
        int stepX;
        if (adx > 0)
            stepX = 1;
        else
        {
            stepX = -1;
            adx = -adx;
        }
        int steps;
        if (dy > adx)
            steps = dy;
        else
            steps = adx;
        int errX = steps >> 1;
        int errY = errX;
        i = steps + 1;
        while (i-- > 0)
        {
            errX += adx;
            if (errX > steps)
            {
                errX -= steps;
                x1 += stepX;
            }
            errY += dy;
            if (errY > steps)
            {
                errY -= steps;
                y1++;
            }
            sum += fn_0043f618(x1, y1);
        }
    }
    return sum;
}

// FUNCTION: STARFLEETOP 0x0043f1c1
void tSensorGroupSystem::vfunction39(float param_1)
{
    field_50 += param_1;
    double now = g_00c7c148;
    if (now - field_48 >= 100000.0)
    {
        fn_0090be9c_type* source = g_0090be9c;
        field_48 = (double)source->vfunction7() / (double)source->field_20 * 100000.0 + now;
        param_1 = field_50;
        field_50 = 0.0f;
        int column = field_80->fn_0043da08();
        fn_0043e366_part* part = field_80;
        int value = g_0079bfc0[part->vfunction11() - part->vfunction10()][column][0];
        field_198 += param_1;
        field_164 = value;
        if (field_198 > 0.025f)
        {
            field_198 = 0.0;
            fn_0043ec81();
        }
        tObject_0042496f* owner = (tObject_0042496f*)field_2c->vfunction108();
        if (owner != 0 && owner->vfunction32() == true)
            fn_0043e09e();
        fn_0043e201(param_1);
    }
}
