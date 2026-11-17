#include "unsorted/fn_0040ce21_type.h"
#include "unsorted/tBaseWeapon.h"
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tEntity.h"
#include "Source/entity/entity_physical.h"
#include "Tiger/tcglue.h"
#include "unsorted/fn_0090bed8_type.h"

extern char g_0090b750;
extern tEntity *g_008e1090[];
short fn_0069afc0(float x, float y);
extern float g_00795620;
extern double g_0079de48;
extern float g_00855984[];
// HYPOTHESIS: the list of child weapons at offset 0x24; slot 2 gives the first, slot 5 the next.
class fn_004d8219_list
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual tBaseWeapon *first();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual tBaseWeapon *next(tBaseWeapon *param_1);
};

// HYPOTHESIS: the slot at offset 0x1a4 returns the number of banks; the header declares it with
// an int parameter, so vfunction126 calls it through this reduced view of the vtable.
class fn_004d819c_view
{
public:
    virtual ~fn_004d819c_view();
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
    virtual void vfunction102();
    virtual void vfunction103();
    virtual void vfunction104();
    virtual char vfunction105();
};

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
float tBaseWeapon::vfunction115(float param_1)
{
    return vfunction116(field_148.fn_004d7099(param_1));
}

// FUNCTION: STARFLEETOP 0x004d7776
float tBaseWeapon::vfunction117(float param_1)
{
    return vfunction118(field_148.fn_004d7099(param_1));
}

// FUNCTION: STARFLEETOP 0x004d779d
int tBaseWeapon::vfunction121()
{
    return (int)&field_148;
}

// FUNCTION: STARFLEETOP 0x004d77a4
float tBaseWeapon::vfunction119()
{
    return g_00795620;
}

// FUNCTION: STARFLEETOP 0x004d77ec
void tBaseWeapon::vfunction122(fn_004d77ec_type* param_1, int param_2, int param_3)
{
    if (param_1 != 0)
    {
        unsigned char i = 0;
        if (param_1->fn_005da49d() > 0)
        {
            do
            {
                param_1->fn_004d6f77(i, vfunction114(i, param_2, param_3));
                param_1->fn_004d6f96(i, vfunction118(i));
                param_1->fn_004d6fb5(i, vfunction116(i));
                i++;
            } while (i < param_1->fn_005da49d());
        }
    }
}

// FUNCTION: STARFLEETOP 0x004d786e
float tBaseWeapon::fn_004d786e()
{
    short a = field_c8;
    short b = field_134;
    if (a > 0x1000)
        a -= 0x2000;
    if (b > 0x1000)
        b -= 0x2000;
    a = a - b;
    if (a < 0)
        a = -a;
    return a * g_0079de48;
}

// FUNCTION: STARFLEETOP 0x004d77ab
void tBaseWeapon::vfunction123(float* param_1)
{
    if (param_1 != 0)
    {
        *(char*)param_1 = vfunction106();
        for (int i = 0; i < *(unsigned char*)param_1; i++)
            param_1[i + 1] = vfunction135(i);
    }
}

// FUNCTION: STARFLEETOP 0x004d78b0
bool tBaseWeapon::vfunction128(short param_1, short param_2)
{
    if (field_130 >= 6)
        return false;
    field_118[field_130][0] = param_1;
    field_118[field_130][1] = param_2;
    field_130++;
    return true;
}

// FUNCTION: STARFLEETOP 0x004d78ea
bool tBaseWeapon::vfunction127(int param_1)
{
    field_114 = param_1;
    if (field_130 >= 6)
        return false;
    vfunction18((int)(vfunction17() * g_00855984[param_1]));
    switch (param_1)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 0x15:
        vfunction128(0, 0x2000);
        field_134 = 0;
        field_cc = 4;
        break;
    case 8:
        vfunction128(0x1aaa, 0x555);
        field_134 = 0;
        field_cc = 0xc;
        break;
    case 9:
        vfunction128(0x1f1c, 0x10e3);
        field_134 = 0x800;
        field_cc = 8;
        break;
    case 0xa:
        vfunction128(0x1f1c, 0xaaa);
        field_134 = 0x555;
        field_cc = 0xc;
        break;
    case 0xb:
        vfunction128(0x1555, 0xe3);
        field_134 = 0x1aaa;
        field_cc = 0xc;
        break;
    case 0x22:
        vfunction128(0x1f1c, 0xaaa);
        vfunction128(0xf1c, 0x10e3);
        field_134 = 0x555;
        field_cc = 0xc;
        break;
    case 0x21:
        vfunction128(0x1555, 0xe3);
        vfunction128(0xf1c, 0x10e3);
        field_134 = 0x1aaa;
        field_cc = 0xc;
        break;
    case 0xc:
        vfunction128(0x1800, 0x800);
        field_134 = 0;
        field_cc = 8;
        break;
    case 0xd:
        vfunction128(0x800, 0x1800);
        field_134 = 0x1000;
        field_cc = 8;
        break;
    case 0xe:
        vfunction128(0xf1c, 0xe3);
        field_134 = 0x1800;
        field_cc = 8;
        break;
    case 0xf:
        vfunction128(0x1555, 0xaaa);
        field_134 = 0;
        field_cc = 6;
        break;
    case 0x10:
        vfunction128(0x555, 0x1aaa);
        field_134 = 0x1000;
        field_cc = 6;
        break;
    case 0x11:
        vfunction128(0x555, 0x10e3);
        field_134 = 0xaaa;
        field_cc = 0xc;
        break;
    case 0x12:
        vfunction128(0xf1c, 0x1aaa);
        field_134 = 0x1555;
        field_cc = 0xc;
        break;
    case 0x1d:
        vfunction128(0x1d55, 0xd55);
        field_134 = 0x555;
        field_cc = 8;
        break;
    case 0x13:
        vfunction128(0x1aaa, 0xaaa);
        field_134 = 0x2aa;
        field_cc = 8;
        break;
    case 0x1e:
        vfunction128(0x12aa, 0x2aa);
        field_134 = 0x1aaa;
        field_cc = 8;
        break;
    case 0x14:
        vfunction128(0x1555, 0x555);
        field_134 = 0x1d55;
        field_cc = 8;
        break;
    case 0x16:
        vfunction128(0x1f1c, 0xaaa);
        vfunction128(0xf1c, 0x1aaa);
        field_134 = 0x1555;
        field_cc = 6;
        break;
    case 0x17:
        vfunction128(0x1555, 0xe3);
        vfunction128(0x555, 0x10e3);
        field_134 = 0x1aaa;
        field_cc = 6;
        break;
    case 0x18:
        vfunction128(0x1800, 0xaaa);
        field_134 = 0x155;
        field_cc = 7;
        break;
    case 0x19:
        vfunction128(0x1555, 0x800);
        field_134 = 0x1eaa;
        field_cc = 7;
        break;
    case 0x1a:
        vfunction128(0xaaa, 0x1555);
        field_134 = 0x1000;
        field_cc = 0xc;
        break;
    case 0x1b:
        vfunction128(0x1f1c, 0x1555);
        field_134 = 0xaaa;
        field_cc = 6;
        break;
    case 0x1c:
        vfunction128(0xaaa, 0xe3);
        field_134 = 0x1555;
        field_cc = 6;
        break;
    case 0x1f:
        vfunction128(0x555, 0x1555);
        field_134 = 0xd55;
        field_cc = 8;
        break;
    case 0x20:
        vfunction128(0xaaa, 0x1aaa);
        field_134 = 0x12aa;
        field_cc = 8;
        break;
    case 0x24:
        vfunction128(0x1555, 0xaaa);
        vfunction128(0xf1c, 0x10e3);
        field_134 = 0;
        field_cc = 6;
        break;
    case 0x23:
        vfunction128(0x1aaa, 0x555);
        vfunction128(0xaaa, 0x1555);
        field_134 = 0;
        field_cc = 6;
        break;
    case 0x25:
        vfunction128(0x1f1c, 0xe3);
        field_134 = 0;
        field_cc = 0xc;
        break;
    case 0x26:
        vfunction128(0x1f8e, 0x71);
        field_134 = 0;
        field_cc = 0xc;
        break;
    case 0x27:
        vfunction128(0x555, 0xaaa);
        vfunction128(0x1555, 0x1aaa);
        field_134 = 0;
        field_cc = 8;
        break;
    case 0x29:
        vfunction128(0xd55, 0x1d55);
        field_134 = 0x1555;
        field_cc = 9;
        break;
    case 0x28:
        vfunction128(0x2aa, 0x12aa);
        field_134 = 0xaaa;
        field_cc = 9;
        break;
    case 0x2b:
        vfunction128(0x1000, 0x555);
        field_134 = 0x1aaa;
        field_cc = 6;
        break;
    case 0x2a:
        vfunction128(0x1aaa, 0x1000);
        field_134 = 0x555;
        field_cc = 6;
        break;
    case 0x2c:
        vfunction128(0x1555, 0x555);
        vfunction128(0xf1c, 0x10e3);
        field_134 = 0x1d55;
        field_cc = 7;
        break;
    case 0x2d:
        vfunction128(0x1aaa, 0xaaa);
        vfunction128(0xf1c, 0x10e3);
        field_134 = 0x2aa;
        field_cc = 7;
        break;
    case 0x2e:
        vfunction128(0x1555, 0x1aaa);
        field_134 = 0x1800;
        field_cc = 0xa;
        break;
    case 0x2f:
        vfunction128(0x555, 0xaaa);
        field_134 = 0x800;
        field_cc = 0xa;
        break;
    default:
        vfunction128(0, 0x2000);
        field_134 = 0;
        field_cc = 0xc;
        break;
    }
    return true;
}

// FUNCTION: STARFLEETOP 0x004d7fa3
bool tBaseWeapon::vfunction130()
{
    for (int i = 0; i < field_130; i++)
    {
        short lo = field_118[i][0];
        short hi = field_118[i][1];
        if (lo < hi)
        {
            if (field_c8 >= lo && field_c8 <= hi)
                return true;
        }
        else
        {
            if (field_c8 >= lo && field_c8 <= 0x2000)
                return true;
            if (field_c8 >= 0 && field_c8 <= hi)
                return true;
        }
    }
    return false;
}

// FUNCTION: STARFLEETOP 0x004d8002
bool tBaseWeapon::vfunction131(short param_1)
{
    for (int i = 0; i < field_130; i++)
    {
        short lo = field_118[i][0];
        short hi = field_118[i][1];
        if (lo < hi)
        {
            if (param_1 >= lo && param_1 <= hi)
                return true;
        }
        else
        {
            if (param_1 >= lo && param_1 <= 0x2000)
                return true;
            if (param_1 >= 0 && param_1 <= hi)
                return true;
        }
    }
    return false;
}

// FUNCTION: STARFLEETOP 0x004d8057
// HYPOTHESIS: the table at 0x008e1090 holds the entity for an ID; param_1 is a
// target entity, or 0 for the one named by field_ec.
bool tBaseWeapon::vfunction132(int param_1)
{
    tPoint3 position;
    tPhysicalEntity *pShip = (tPhysicalEntity *)vfunction33();
    if (pShip != NULL)
    {
        position = pShip->fCurrPos;
        short heading = pShip->field_0x90;
        tEntity *pTarget = (tEntity *)param_1;
        if (pTarget == NULL)
        {
            if (field_ec < 0 || field_ec >= 0x4000)
                pTarget = NULL;
            else
                pTarget = g_008e1090[field_ec];
        }
        tPoint3 target(0.0f, 0.0f, 0.0f);
        if (pTarget != NULL)
        {
            target = pTarget->fCurrPos;
            tPoint3 direction;
            direction = target - position;
            direction.mNormalize();
            field_c8 = (heading - (fn_0069afc0(direction.x, direction.y) & 0x1fff)) & 0x1fff;
            if (vfunction130())
                return true;
        }
    }
    return false;
}

// FUNCTION: STARFLEETOP 0x004d813c
// HYPOTHESIS: tBaseWeapon derives from tIndividualShipSystem in the original; the header lacks the base.
void tBaseWeapon::vfunction62()
{
    ((tIndividualShipSystem *)this)->tIndividualShipSystem::vfunction62();
    vfunction65(field_fc);
}

// FUNCTION: STARFLEETOP 0x004d8157
void tBaseWeapon::vfunction63()
{
    field_45 = 0;
    vfunction80();
}

// FUNCTION: STARFLEETOP 0x004d8163
void tBaseWeapon::vfunction80()
{
    ((tIndividualShipSystem *)this)->tIndividualShipSystem::vfunction80();
    field_e8 = 0;
    field_d4 = 0.0f;
    field_d8 = 0.0f;
    field_dc = 0.0f;
    field_e0 = 0.0f;
    field_e4 = 0.0f;
}

// FUNCTION: STARFLEETOP 0x004d819c
void tBaseWeapon::vfunction126(float param_1)
{
    int banks = (char)((fn_004d819c_view *)this)->vfunction105();
    if (param_1 > 0.0f && param_1 < 1.0f)
    {
        field_3c = param_1 * field_3c;
        if (banks > 1)
            field_dc = param_1 * field_dc;
        if (banks > 2)
            field_e0 = param_1 * field_e0;
        if (banks > 3)
            field_e4 = param_1 * field_e4;
    }
}

// FUNCTION: STARFLEETOP 0x004d8219
float tBaseWeapon::vfunction76(float param_1, float param_2)
{
    if (vfunction46() == 1)
    {
        tBaseWeapon *child;
        for (child = field_24->first(); child != 0; child = field_24->next(child))
            param_1 = child->vfunction76(param_1, param_2);
        return param_1;
    }
    else
    {
        if (vfunction64())
        {
            if (param_1 > field_40)
                return param_1 - field_40;
            vfunction124(0);
        }
    }
    return param_1;
}

// FUNCTION: STARFLEETOP 0x004d82fb
float tBaseWeapon::vfunction74(float param_1, float param_2)
{
    int banks;
    if (field_45)
    {
        float avail = param_1 * param_2;
        if ((char)vfunction106() > 0)
        {
            field_d8 = g_0090bed8.fn_00518603() * field_d4 * param_2;
            float thresh = g_0090b750 ? field_d8 : 0.0f;
            banks = (char)vfunction106();
            float first;
            if (avail > field_d8)
                first = field_d8;
            else
                first = avail;
            if (vfunction144() > 0)
                first = (float)vfunction146() * first;
            if (vfunction144() == 0)
                first = 0.0f;
            if (field_e8 < 1 && avail >= thresh)
            {
                float *p = &field_3c;
                *p = first + field_3c;
                avail = avail - first;
                param_1 = param_1 - field_d4;
                fn_004d82e3(&param_1);
                fn_004d82cd(p);
            }
            else
            {
                if (field_e8 < 1)
                {
                    field_3c -= param_2;
                    if (field_3c < 0.0f)
                        field_3c = 0.0f;
                }
            }
            if (banks > 1 && avail >= thresh)
            {
                if (field_e8 < 2)
                {
                    float m;
                    if (avail > field_d8)
                        m = field_d8;
                    else
                        m = avail;
                    float *p = &field_dc;
                    *p = m + *p;
                    avail = avail - m;
                    param_1 = param_1 - field_d4;
                    fn_004d82e3(&param_1);
                    fn_004d82cd(p);
                }
            }
            else
            {
                if (field_e8 < 2)
                {
                    field_dc -= param_2;
                    if (field_dc < 0.0f)
                        field_dc = 0.0f;
                }
            }
            if (banks > 2 && avail >= thresh)
            {
                if (field_e8 < 3)
                {
                    float m;
                    if (avail > field_d8)
                        m = field_d8;
                    else
                        m = avail;
                    float *p = &field_e0;
                    *p = m + *p;
                    avail = avail - m;
                    param_1 = param_1 - field_d4;
                    fn_004d82e3(&param_1);
                    fn_004d82cd(p);
                }
            }
            else
            {
                if (field_e8 < 3)
                {
                    field_e0 -= param_2;
                    if (field_e0 < 0.0f)
                        field_e0 = 0.0f;
                }
            }
            // MATCH-ONLY: the original tests banks > 3 (cmp 3; jle) yet keeps 4 in ebx; ">= 4"
            // is the only form found that makes the compiler keep 4 in a register.
            if (banks >= 4 && avail >= thresh)
            {
                if (field_e8 < 4)
                {
                    float m;
                    if (avail > field_d8)
                        m = field_d8;
                    else
                        m = avail;
                    float *p = &field_e4;
                    *p = m + *p;
                    avail = avail - m;
                    param_1 = param_1 - field_d4;
                    fn_004d82e3(&param_1);
                    fn_004d82cd(p);
                }
            }
            else
            {
                if (field_e8 < 4)
                {
                    field_e4 -= param_2;
                    if (field_e4 < 0.0f)
                        field_e4 = 0.0f;
                }
            }
            vfunction147();
            if (field_e4 == field_38)
            {
                if ((char)field_e8 != 4)
                    vfunction124(4);
            }
            else if (field_e0 == field_38)
            {
                if ((char)field_e8 != 3)
                    vfunction124(3);
            }
            else if (field_dc == field_38)
            {
                if ((char)field_e8 != 2)
                    vfunction124(2);
            }
            else if (field_3c == field_38)
            {
                if ((char)field_e8 != 1)
                {
                    field_46 = 1;
                    vfunction124(1);
                }
            }
            else
            {
                if ((char)field_e8 != 0)
                    vfunction124(0);
            }
        }
        else
        {
            field_d8 = 0.0f;
        }
    }
    return param_1;
}

// FUNCTION: STARFLEETOP 0x004d829b
float tBaseWeapon::vfunction70()
{
    field_34 = 0.0f;
    if (field_45)
        field_34 = (float)(char)vfunction107() * field_d4;
    return field_34;
}

// FUNCTION: STARFLEETOP 0x004d82cd
void tBaseWeapon::fn_004d82cd(float *param_1)
{
    if (*param_1 > field_38)
        *param_1 = field_38;
}

// FUNCTION: STARFLEETOP 0x004d82e3
void tBaseWeapon::fn_004d82e3(float *param_1)
{
    if (*param_1 < 0.0f)
        *param_1 = 0.0f;
}

// FUNCTION: STARFLEETOP 0x004d867a
void tBaseWeapon::vfunction147()
{
    if (field_3c < field_dc)
        fn_004d871f(&field_3c, &field_dc);
    if (field_3c < field_e0)
        fn_004d871f(&field_3c, &field_e0);
    if (field_3c < field_e4)
        fn_004d871f(&field_3c, &field_e4);
    if (field_dc < field_e0)
        fn_004d871f(&field_dc, &field_e0);
    if (field_dc < field_e4)
        fn_004d871f(&field_dc, &field_e4);
    if (field_e0 < field_e4)
        fn_004d871f(&field_e0, &field_e4);
}

// FUNCTION: STARFLEETOP 0x004d871f
void tBaseWeapon::fn_004d871f(float *param_1, float *param_2)
{
    float tmp = *param_1;
    *param_1 = *param_2;
    *param_2 = tmp;
}

// FUNCTION: STARFLEETOP 0x004d8732
void tBaseWeapon::fn_004d8732(int *param_1, int *param_2)
{
    int tmp = *param_1;
    *param_1 = *param_2;
    *param_2 = tmp;
}

// FUNCTION: STARFLEETOP 0x004d8747
void tBaseWeapon::vfunction105(int param_1)
{
    field_104 = param_1;
    vfunction57((field_104 + 1) * 0x80);
    vfunction55((field_104 + 1) * 0x80);
}



// FUNCTION: STARFLEETOP 0x004d877a
float tBaseWeapon::vfunction135(unsigned char param_1)
{
    // HYPOTHESIS: the original leaves value unset for an index above 3; VC6 gives it the
    // argument's stack slot, which the original reads as a float in that case.
    float value;
    if (param_1 == 0) {
        value = field_38 - field_3c;
    } else if (param_1 == 1) {
        value = field_38 - field_dc;
    } else if (param_1 == 2) {
        value = field_38 - field_e0;
    } else if (param_1 == 3) {
        value = field_38 - field_e4;
    }
    if (field_d4 == 0.0f) {
        return 0.0f;
    }
    return value / field_d4;
}

#include "unsorted/tObject_0090be60.h"
#include "unsorted/fn_004e2b02_ship_type.h"

// HYPOTHESIS: the entity the weapon's target id names, as 0x004d87e3 reads it.
struct fn_004d87e3_target
{
    char pad_00[0x18];
    unsigned char field_18;
    char pad_19[0x60 - 0x19];
    int field_60;
    char pad_64[0x1bc - 0x64];
    int field_1bc;
};

#include "unsorted/tObject_008f5170.h"

extern float g_0079561c;

// FUNCTION: STARFLEETOP 0x004d87e3
int tBaseWeapon::fn_004d87e3()
{
    fn_004e2b02_ship_type *ship = vfunction33();
    int result;
    if (ship == NULL || field_68 > g_0079561c)
    {
        result = 0x20;
    }
    else
    {
      if ((field_f8 >= 8 && field_f8 <= 0xe) || !fn_004d76f8())
      {
        fn_004d87e3_target *target;
        if (field_ec < 0 || field_ec >= 0x4000)
            target = NULL;
        else
            target = (fn_004d87e3_target *)g_008e1090[field_ec];
        if (ship->vfunction32() || target == NULL)
            return 0x100;
        if ((!(target->field_18 & 4) || target->field_1bc < 0 || target->field_1bc > 4 ||
             (field_f8 >= 8 && field_f8 <= 0xc)) &&
            gObject_008f5170.fn_00425a18(target, ship, 1))
        {
            if (ship->field_60 == target->field_60 ||
                gObject_0090be60->fn_00427581(ship->field_60, target->field_60) == 2 ||
                gObject_0090be60->fn_00427581(ship->field_60, target->field_60) == 3)
                return 0x80;
            return 0x200;
        }
      }
      result = 0x40;
    }
    return result;
}

#include <stdio.h>

// HYPOTHESIS: the mode string at 0x0083bc38 and the second call as fprintf are guesses.
extern char g_0083bc38[];

// FUNCTION: STARFLEETOP 0x004d88e9
void fn_004d88e9(const char *param_1)
{
    FILE *file = fopen("WeaponDamage.txt", g_0083bc38);
    if (file != NULL) {
        fprintf(file, param_1);
        fclose(file);
    }
}

// FUNCTION: STARFLEETOP 0x004d8916
void fn_004d8916(int param_1, int param_2, float param_3, int param_4, int param_5)
{
    FILE *file = fopen("WeaponDamage.txt", g_0083bc38);
    if (file != NULL) {
        fprintf(file, "%s fired from # %d at # %d at %f ticks away and did %d points of damage \n", param_5, param_1, param_2, (double)param_3, param_4);
        fclose(file);
    }
}
