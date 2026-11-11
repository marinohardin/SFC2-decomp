#include "unsorted/fn_0040ce21_type.h"
#include "unsorted/fn_004e20f4_field2c_type.h"
#include "unsorted/fn_004e2895_field148_type.h"
#include "unsorted/fn_0090bed8_type.h"
#include "unsorted/fn_0090be9c_type.h"
#include "unsorted/fn_004e2b02_ship_type.h"
#include "unsorted/c_0092deb4.h"
#include "unsorted/tBaseWeapon.h"
#include "unsorted/tEntity.h"

// HYPOTHESIS: the per-type table 0x004e558a passes with its entry count, of the same kind as
// tPhaserBank's tables.
extern const float g_00856fa0[];

// HYPOTHESIS: a float the constructor stores in each of the four recovery timers, as
// tPhaserBank's constructor does with its own constant.
extern float g_007a87b0;

// HYPOTHESIS: the limit 0x004e55af compares the value at 0xf4 with.
extern const float g_007a8218;

// HYPOTHESIS: a float constant read by 0x004e56dc, a margin below the maximum, as in
// tPhaserBank's 0x004e245e.
extern float g_007a166c;

// HYPOTHESIS: a float constant returned by 0x004e572e when the rate is zero, as in
// tPhaserBank's 0x004e24b0.
extern float g_0079d508;

// HYPOTHESIS: a float constant (1.0) that 0x004e572e divides by the rate.
extern float g_007959cc;

// HYPOTHESIS: a float constant 0x004e5868 passes to slot 67, as tPhaserBank's 0x004e20f4
// does for its fourth type.
extern float g_007977c0;

// HYPOTHESIS: a double constant 0x004e58ec compares each bank's recovery timer with; a
// timer below it means the bank is ready.
extern const double g_007a7028;

// HYPOTHESIS: constants 0x004e5958 reads: a double scale for the global's count, a range
// penalty and the floor below which the range is set to 20, and the damage multiplier used
// while field_499 is set.
extern double g_0079a148;
extern float g_007962f8;
extern float g_007962fc;
extern float g_00797228;

// HYPOTHESIS: the X bank's byte table, of the same shape as tPhaserBank's 0x008569fc, read by
// slot 114 (0x004e5b75).
extern unsigned char g_00856f5c[6][11];
// HYPOTHESIS: the float constant tPhaserBank's 0x004e25c0 scales its sum by.
extern float g_0079bc20;

// HYPOTHESIS: a flag at 0x0092eabd under which 0x004e5958 reports each shot through
// 0x004d8916, with a text at 0x0085702c.
extern bool g_0092eabd;
extern const char g_0085702c[];
void __cdecl fn_004d8916(int aShipID, int aTargetID, float aRange, int aDamage, const char* aText);

// HYPOTHESIS: the stream helpers tPhaserBank's mStreamOutSystemSpecificData (0x004e1f61) calls,
// in the same form: (value, stream, mode, field name).
void fn_0040e9df(int &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eafc(bool &aValue, tSFCStream &s, unsigned mode, const char *aText);
void fn_0040eb35(float &aValue, tSFCStream &s, unsigned mode, const char *aText);

class tPhaserXBank : public tBaseWeapon
{
public:
    tPhaserXBank(int param_1, fn_004e1e99_arg* param_2);
    // HYPOTHESIS: the constructor calls these two as tPhaserBank's calls 0x004e20f4 and
    // 0x004e2895; their roles are not known.
    void fn_004e5868();
    // HYPOTHESIS: 0x004e5868 calls the helper at 0x004e20d1, defined with tPhaserBank, on this
    // object; it is probably a shared base-class member whose class is not yet known.
    void fn_004e20d1(const char* name, int a2);
    void fn_004e558a();
    int fn_004e5958(int param_1);
    // HYPOTHESIS: turns a step and a range into damage; 0x004e5958 calls it.
    int fn_004e5c2f(int param_1, int param_2);
    virtual int vfunction101(int param_1, int param_2);
    virtual unsigned mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode);
    virtual void vfunction105(int param_1);
    virtual int vfunction133();
    virtual bool vfunction134();
    virtual void vfunction65(char param_1);
    virtual float vfunction74(float param_1, float param_2);
    virtual float vfunction79();
    virtual void vfunction80();
    virtual float vfunction109();
    virtual float vfunction110();
    virtual float vfunction111();
    virtual float vfunction114(unsigned char param_1, int param_2, int param_3);
    // HYPOTHESIS: slots 135 to 148 are placeholders that only fix the position of slot 149
    // (offset 0x250), which 0x004e55e9 calls with 0; slot 148 is the one 0x004e558a fills.
    virtual float vfunction135(unsigned char param_1);
    virtual void vfunction136();
    virtual void vfunction137();
    virtual void vfunction138();
    virtual void vfunction139();
    virtual void vfunction140();
    virtual void vfunction141();
    virtual void vfunction142();
    virtual float vfunction143(float param_1, float param_2);
    virtual void vfunction144();
    virtual void vfunction145();
    virtual void vfunction146();
    virtual void vfunction147();
    virtual void vfunction148();
    // HYPOTHESIS: the recovery time left for one bank, as tPhaserBank's 0x004e2243.
    virtual float vfunction149(int param_1);
    // HYPOTHESIS: slot 116, which tBaseWeapon.h declares as void(); its form here follows
    // tPhaserBank's slot 116 (0x004e2711). Declared last so it does not move the slots above.
    virtual float vfunction116(unsigned char param_1);
    virtual float vfunction118(unsigned char param_1);

    char pad04[0x10 - 0x04];
    // HYPOTHESIS: a signed state that 0x004e58ec requires to be non-negative.
    int field_10;
    char pad14[0x2c - 0x14];
    fn_004e20f4_field2c_type* field_2c;
    char pad30[0x38 - 0x30];
    // HYPOTHESIS: a float the constructor sets to 2.0.
    float field_38;
    char pad3c[0xec - 0x3c];
    // HYPOTHESIS: the target's ID, as tPhaserBank's field_ec.
    int field_ec;
    char padf0[0xf4 - 0xf0];
    float field_f4;
    int field_f8;
    // HYPOTHESIS: a mode byte set by vfunction65 (0x004e562b); 12 is a mode it does not leave.
    char field_fc;
    char padfd[0x100 - 0xfd];
    int field_100;
    int field_104;
    int field_108;
    char pad10c[0x148 - 0x10c];
    fn_004e2895_field148_type field_148;
    char pad149[0x470 - 0x149];
    float field_470[4];
    bool field_480;
    char pad481[0x484 - 0x481];
    // HYPOTHESIS: the number of banks, as tPhaserBank's field_4c4.
    int field_484;
    int field_488;
    bool field_48c;
    char pad48d[0x490 - 0x48d];
    double field_490;
    bool field_498;
    bool field_499;
    char pad49a[0x49c - 0x49a];
    float field_49c;
    bool field_4a0;
};

// FUNCTION: STARFLEETOP 0x004e5325
tPhaserXBank::tPhaserXBank(int param_1, fn_004e1e99_arg* param_2)
    : tBaseWeapon(param_1)
{
    static fn_0040ce21_type marker("tPhaserXBank::tPhaserXBank");
    field_f8 = param_2->field_00;
    field_100 = param_2->field_04;
    field_104 = param_2->field_08;
    field_108 = param_2->field_0c;
    for (int i = 0; i < 4; i++)
    {
        field_470[i] = g_007a87b0;
    }
    field_488 = 0;
    field_480 = false;
    field_48c = false;
    fn_004e5868();
    field_498 = false;
    field_490 = 0.0;
    fn_004e558a();
    field_499 = false;
    field_38 = 2.0f;
    field_4a0 = false;
    field_49c = 1.0f;
}

// FUNCTION: STARFLEETOP 0x004e5403
unsigned tPhaserXBank::mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode)
{
    static fn_0040ce21_type marker("tPhaserXBank::mStreamOutSystemSpecificData");
    unsigned result;
    int i;

    result = tBaseWeapon::mStreamOutSystemSpecificData(s, aFlags, mode);
    if ((result & 8) > 0)
    {
        result = vfunction24(s, result, mode);
        fn_0040e9df(field_484, s, mode, "fNumberofBanks");
        fn_0040e9df(field_488, s, mode, "fCurrentBank");
        fn_0040eafc(field_480, s, mode, "fBankUsedthisTimeSlice");
        for (i = 0; i < field_484; i++)
            fn_0040eb35(field_470[i], s, mode, "fRecoverTimer[i]");
    }
    else if ((result & 0x20) <= 0 && (result & 0x80) > 0)
    {
        result = vfunction24(s, result, mode);
        fn_0040e9df(field_484, s, mode, "fNumberofBanks");
        fn_0040e9df(field_488, s, mode, "fCurrentBank");
        fn_0040eafc(field_480, s, mode, "fBankUsedthisTimeSlice");
        for (i = 0; i < field_484; i++)
            fn_0040eb35(field_470[i], s, mode, "fRecoverTimer[i]");
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x004e5573
void tPhaserXBank::vfunction105(int param_1)
{
    tBaseWeapon::vfunction105(param_1);
    fn_004e5868();
}

// FUNCTION: STARFLEETOP 0x004e558a
void tPhaserXBank::fn_004e558a()
{
    // HYPOTHESIS: clears the object at 0x148 and fills it from the X bank's single table,
    // as tPhaserBank's 0x004e2895 does from its per-type tables.
    field_148.fn_0050bfaa();
    field_148.fn_004d6fd4();
    field_148.fn_004d6f22(g_00856fa0, 11);
}

// FUNCTION: STARFLEETOP 0x004e55af
int tPhaserXBank::vfunction133()
{
    field_f4 = gObject_008e1060.fn_00424705(vfunction33(), vfunction34());
    if (field_f4 > g_007a8218)
        return 1;
    return 0;
}

// FUNCTION: STARFLEETOP 0x004e57c0
float tPhaserXBank::vfunction74(float param_1, float param_2)
{
    // HYPOTHESIS: draws up to param_1 (limited by slot 70) into the object at 0x2c, clamps it
    // to its maximum, and returns the part of param_1 not drawn.
    float max = field_2c->vfunction123();
    float limit = vfunction70();
    float amount = limit < param_1 ? limit : param_1;
    field_2c->vfunction114(amount * g_0090bed8.fn_00518603() * param_2);
    if (field_2c->vfunction117() >= max)
        field_2c->vfunction116(max);
    return param_1 - amount;
}

// FUNCTION: STARFLEETOP 0x004e55e9
float tPhaserXBank::vfunction79()
{
    // HYPOTHESIS: the recovery time left for the first bank, as tPhaserBank's 0x004e22d5.
    return vfunction149(0);
}

// FUNCTION: STARFLEETOP 0x004e5849
void tPhaserXBank::vfunction80()
{
    // HYPOTHESIS: after the base reset, passes the multiplier vfunction65 sets (1.0 or 2.0)
    // on to the object at 0x2c.
    tBaseWeapon::vfunction80();
    field_2c->vfunction115(field_49c);
}

// FUNCTION: STARFLEETOP 0x004e5868
void tPhaserXBank::fn_004e5868()
{
    // HYPOTHESIS: the X bank's single-type form of tPhaserBank's 0x004e20f4.
    vfunction65(0);
    fn_004e20d1("PhaserX", 0x8480);
    field_484 = field_104 + 1;
    vfunction57(field_484 * 128);
    vfunction55(field_484 * 128);
    vfunction67(g_007977c0);
    vfunction69(field_484 * 2.0f);
    field_2c->vfunction118();
}

// FUNCTION: STARFLEETOP 0x004e55f4
float tPhaserXBank::vfunction109()
{
    // HYPOTHESIS: the recovery time left for the second bank, as tPhaserBank's 0x004e22dd.
    return vfunction149(1);
}

// FUNCTION: STARFLEETOP 0x004e55ff
float tPhaserXBank::vfunction110()
{
    // HYPOTHESIS: the recovery time left for the third bank, as tPhaserBank's 0x004e22e5.
    return vfunction149(2);
}

// FUNCTION: STARFLEETOP 0x004e560a
float tPhaserXBank::vfunction111()
{
    // HYPOTHESIS: the recovery time left for the fourth bank, as tPhaserBank's 0x004e22ed.
    return vfunction149(3);
}

// FUNCTION: STARFLEETOP 0x004e5615
float tPhaserXBank::vfunction149(int param_1)
{
    // HYPOTHESIS: the time left on one bank's recovery timer, doubled; g_007a87b0 is the
    // value the constructor starts each timer at.
    return (g_007a87b0 - field_470[param_1]) * 2.0f;
}

// FUNCTION: STARFLEETOP 0x004e562b
void tPhaserXBank::vfunction65(char param_1)
{
    if (param_1 == 13)
    {
        if (field_499)
            field_fc = 1;
        else
            field_fc = 0;
    }
    else
    {
        if (field_fc != 12)
            field_fc = param_1;
        if (param_1 == 1)
        {
            field_499 = true;
            field_49c = 2.0f;
        }
        else if (param_1 == 0)
        {
            field_499 = false;
            field_49c = 1.0f;
        }
    }
}

// FUNCTION: STARFLEETOP 0x004e56dc
float tPhaserXBank::vfunction143(float param_1, float param_2)
{
    // HYPOTHESIS: the same test as tPhaserBank's slot 143 (0x004e245e); the two parameters
    // (ret 8) are unused and their types are guessed from slot 74.
    float max = field_2c->vfunction123();
    float cur = field_2c->vfunction117();
    float rate = field_2c->vfunction112();
    float room = max - g_007a166c;
    if (cur < room)
        return rate * max;
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x004e572e
float tPhaserXBank::vfunction135(unsigned char param_1)
{
    // HYPOTHESIS: the same as tPhaserBank's slot 135 (0x004e24b0): the time until bank
    // param_1 can fire, given the energy held by the object at 0x2c against its rate.
    if (field_2c->vfunction117() > 1.0f)
        return field_470[param_1];
    if (field_2c->vfunction112() == 0.0f)
        return g_0079d508;
    // HYPOTHESIS: the original also stores this pointer to the stack slot that t uses later,
    // as in tPhaserBank's 0x004e24b0, which suggests an inline helper not yet identified.
    fn_004e20f4_field2c_type* p = field_2c;
    float t = g_007959cc / p->vfunction112();
    t *= p->vfunction119();
    if (t > field_470[param_1])
        return t;
    return field_470[param_1];
}

// FUNCTION: STARFLEETOP 0x004e58ec
bool tPhaserXBank::vfunction134()
{
    // HYPOTHESIS: whether any bank is ready to fire: no bank used this time slice, the
    // object at 0x2c holds at least the multiplier's worth of energy, slot 64 allows it, and
    // one bank's recovery timer has run out.
    bool result = false;
    int i = 0;
    if (!field_480 && field_10 >= 0 && field_2c->vfunction117() >= field_49c && vfunction64())
    {
        int count = vfunction10();
        for (; i < count; i++)
        {
            if (field_470[i] < g_007a7028)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x004e5b75
float tPhaserXBank::vfunction114(unsigned char param_1, int param_2, int param_3)
{
    // HYPOTHESIS: the X bank's form of tPhaserBank's slot 114 (0x004e25c0): sums six entries
    // of its one table from column param_3 and row param_1, doubled while field_499 is set.
    float sum = 0.0f;
    for (int i = 1; i <= 6; i++)
    {
        unsigned char row = param_1;
        unsigned char col = i + param_3 - 1;
        if (col >= 6)
        {
            row = col + param_1 - 5;
            col = 5;
        }
        if (param_1 < 11)
            sum += g_00856f5c[col][row];
    }
    if (field_499)
    {
        // MATCH-ONLY: the copy reproduces the original's fld st(0)/fstp st(1); the original
        // probably multiplied through an inline helper taking the value by copy, not yet known.
        float doubled = sum;
        doubled *= g_00797228;
        sum = doubled;
    }
    return sum * g_0079bc20;
}

// FUNCTION: STARFLEETOP 0x004e5958
int tPhaserXBank::fn_004e5958(int param_1)
{
    // HYPOTHESIS: fires the current bank at the target param_1 (or, when it is negative, at
    // the range already stored at 0xf4) and returns the damage: a step from the global's
    // count and total, lowered by the conditions below, and the range, go to 0x004e5c2f.
    fn_0090be9c_type* source = g_0090be9c;
    int step = 1 - (int)((double)source->vfunction7() * g_0079a148 /
                         (double)source->field_20);
    float range;
    if (param_1 < 0)
        range = field_2c->vfunction108(field_f4, 0);
    else
    {
        int fromID = vfunction33()->field_1c;
        range = field_2c->vfunction108(gObject_008e1060.fn_004246ab(fromID, param_1), 0);
    }
    fn_004e5958_slot78_type* slot78 = field_2c->vfunction78();
    if (step > 1 && slot78 != 0 && slot78->vfunction100() == 5)
        step--;
    if (vfunction35() && !g_0092deb4->fn_vtbl_E4() && g_0092deb4->fn_vtbl_6C() == 0 && step > 1)
        step--;
    switch (field_100)
    {
    case 1:
        step--;
        if (step < 1)
            step = 1;
        break;
    case 2:
        step--;
        if (step < 1)
            step = 1;
        range -= g_007962f8;
        if (range < g_007962fc)
            range = 20.0f;
        break;
    }
    int shipID = -1;
    if (g_0092eabd)
    {
        fn_004e2b02_ship_type* ship = vfunction33();
        if (ship != 0)
            shipID = ship->field_1c;
    }
    step += field_2c->vfunction110(0);
    field_2c->vfunction115(field_49c);
    int damage = fn_004e5c2f(step, (int)range);
    if (field_499)
        damage = (int)(damage * g_00797228);
    if (g_0092eabd)
        fn_004d8916(shipID, field_ec, range, damage, g_0085702c);
    field_470[field_488] = g_007a87b0;
    field_480 = true;
    fn_004e2b02_ship_type* target = gObject_008e1060.fn_0042496f(field_ec);
    if (target != 0 && target->vfunction32())
        damage = vfunction101(damage, field_ec);
    return damage;
}

// FUNCTION: STARFLEETOP 0x004e5bd9
float tPhaserXBank::vfunction116(unsigned char param_1)
{
    // HYPOTHESIS: the value in the last column of the X bank's table for row param_1, the
    // column that slot 114 (0x004e5b75) clamps to, doubled while field_499 is set.
    float value = g_00856f5c[5][param_1];
    if (field_499)
    {
        // MATCH-ONLY: the copy reproduces the original's fld st(0)/fstp st(1), as in slot 114.
        float doubled = value;
        doubled *= g_00797228;
        value = doubled;
    }
    return value;
}

// FUNCTION: STARFLEETOP 0x004e5c04
float tPhaserXBank::vfunction118(unsigned char param_1)
{
    // HYPOTHESIS: the value in the first column of the X bank's table for row param_1, the
    // column slot 114 (0x004e5b75) starts from, doubled while field_499 is set.
    float value = g_00856f5c[0][param_1];
    if (field_499)
    {
        // MATCH-ONLY: the copy reproduces the original's fld st(0)/fstp st(1), as in slot 114.
        float doubled = value;
        doubled *= g_00797228;
        value = doubled;
    }
    return value;
}
