#include "unsorted/tBaseWeapon.h"
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
    case 1:
    case 8:
    case 21:
        field_cc += 60;
        return;
    case 5:
    case 58:
        field_cc += 300;
        return;
    case 3:
    case 42:
        field_cc += 260;
        return;
    case 0:
    case 29:
        field_cc += 20;
        return;
    case 23:
    case 37:
        field_cc += 220;
        return;
    case 9:
    case 15:
        field_cc += 100;
        return;
    case 11:
        field_cc += 40;
        return;
    case 10:
    case 40:
        field_cc += 120;
        return;
    case 25:
    case 39:
        field_cc += 180;
        return;
    case 26:
    case 38:
        field_cc += 160;
        return;
    case 12:
    case 30:
        field_cc += 80;
        return;
    case 4:
    case 59:
        field_cc += 280;
        return;
    case 57:
        field_cc += 320;
        return;
    case 2:
    case 44:
        field_cc += 140;
        return;
    case 19:
    case 43:
        field_cc += 240;
        return;
    case 24:
    case 41:
        field_cc += 200;
        return;
    }
    field_cc += 500;
}
