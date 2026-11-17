// Unit:      unsorted/tSensorGroupSystem.cpp
// Functions of tSensorGroupSystem (0x0043dfd4 to 0x0043f6c3); original file unknown.
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tEntity.h"

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
    char pad194[0x1d9 - 0x194];
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
    virtual void vfunction51(); // slot 50
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
