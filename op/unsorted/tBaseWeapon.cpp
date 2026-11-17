#include "unsorted/fn_0040ce21_type.h"
#include "unsorted/tBaseWeapon.h"
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tEntity.h"

// HYPOTHESIS: this is tCloakSystem, declared in full only inside unsorted/tCloakSystem.cpp;
// this is the one member used here.
class fn_00438607_class
{
public:
    float mDetermineCloakSeekingWeaponDamageModifier(int param_1, int param_2);
};

// HYPOTHESIS: the entity fn_004249d3 returns; its slot at offset 0x80 gives the cloak system.
class fn_004d7402_entity
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
    virtual fn_00438607_class* vfunction33();
};

// FUNCTION: STARFLEETOP 0x004d7402
int tBaseWeapon::vfunction101(int param_1, int param_2)
{
    fn_004d7402_entity* entity = (fn_004d7402_entity*)gObject_008e1060.fn_004249d3(param_2);
    if (entity != 0)
    {
        fn_00438607_class* cloak = entity->vfunction33();
        if (cloak != 0)
        {
            return (int)(cloak->mDetermineCloakSeekingWeaponDamageModifier(0, 0) * param_1);
        }
    }
    return param_1;
}

// FUNCTION: STARFLEETOP 0x004d743f
void tBaseWeapon::vfunction102()
{
    if (vfunction138())
    {
        int bank = vfunction106();
        if (bank < 4)
        {
            if (bank < 3)
            {
                if (bank < 2)
                {
                    if (bank < 1)
                    {
                        field_3c = 0;
                    }
                    if (field_dc == field_38)
                    {
                        vfunction124(field_e8 - 1);
                    }
                    field_dc = 0;
                }
                if (field_e0 == field_38)
                {
                    vfunction124(field_e8 - 1);
                }
                field_e0 = 0;
            }
            if (field_e4 == field_38)
            {
                vfunction124(field_e8 - 1);
            }
            field_e4 = 0;
        }
    }
}

// FUNCTION: STARFLEETOP 0x004d74f6
void tBaseWeapon::fn_004d74f6()
{
    switch (field_f8)
    {
    case 0: field_cc += 20; break;
    case 1: field_cc += 60; break;
    case 2: field_cc += 140; break;
    case 3: field_cc += 260; break;
    case 4: field_cc += 280; break;
    case 5: field_cc += 300; break;
    case 8: field_cc += 60; break;
    case 9: field_cc += 100; break;
    case 10: field_cc += 120; break;
    case 11: field_cc += 40; break;
    case 12: field_cc += 80; break;
    case 15: field_cc += 100; break;
    case 19: field_cc += 240; break;
    case 21: field_cc += 60; break;
    case 23: field_cc += 220; break;
    case 24: field_cc += 200; break;
    case 25: field_cc += 180; break;
    case 26: field_cc += 160; break;
    case 29: field_cc += 20; break;
    case 30: field_cc += 80; break;
    case 37: field_cc += 220; break;
    case 38: field_cc += 160; break;
    case 39: field_cc += 180; break;
    case 40: field_cc += 120; break;
    case 41: field_cc += 200; break;
    case 42: field_cc += 260; break;
    case 43: field_cc += 240; break;
    case 44: field_cc += 140; break;
    case 57: field_cc += 320; break;
    case 58: field_cc += 300; break;
    case 59: field_cc += 280; break;
    default: field_cc += 500; break;
    }
}

// FUNCTION: STARFLEETOP 0x004d7648
void tBaseWeapon::vfunction100()
{
    vfunction55(0);
    vfunction63();
    vfunction80();
}

// FUNCTION: STARFLEETOP 0x004d766b
void tBaseWeapon::vfunction103()
{
    field_46 = 1;
    field_3c = 0.0f;
    if (vfunction144() > 0)
        vfunction145(vfunction144() - 1);
}

// FUNCTION: STARFLEETOP 0x004d76a0
int tBaseWeapon::vfunction107()
{
    return vfunction85() - field_e8;
}

// FUNCTION: STARFLEETOP 0x004d76b3
unsigned tBaseWeapon::mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode)
{
    static fn_0040ce21_type marker("tBaseWeapon::mStreamOutSystemSpecificData");
    // HYPOTHESIS: tBaseWeapon derives from tRepairableShipSystem; its declaration here has no base yet.
    return ((tRepairableShipSystem*)this)->tRepairableShipSystem::vfunction23((int)&s, aFlags, (int)mode);
}

// FUNCTION: STARFLEETOP 0x004d76f8
bool tBaseWeapon::fn_004d76f8()
{
    if (field_2c)
        return field_2c->vfunction143();
    return false;
}

// FUNCTION: STARFLEETOP 0x004d770a
void tBaseWeapon::vfunction112(float param_1)
{
    field_148.fn_004d7099(param_1);
}

// FUNCTION: STARFLEETOP 0x004d7720
float tBaseWeapon::vfunction113(float param_1, int param_2, int param_3)
{
    return vfunction114(field_148.fn_004d7099(param_1), param_2, param_3);
}

// FUNCTION: STARFLEETOP 0x004d774f
void tBaseWeapon::vfunction115(float param_1)
{
    vfunction116(field_148.fn_004d7099(param_1));
}

// FUNCTION: STARFLEETOP 0x004d7776
void tBaseWeapon::vfunction117(float param_1)
{
    vfunction118(field_148.fn_004d7099(param_1));
}
