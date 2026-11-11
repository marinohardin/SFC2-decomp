// tShieldGroup: the group of a ship's shield systems.
// Original file unknown.
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tIndividualShipSystemList.h"
#include "unsorted/tShieldGroup.h"
#include "unsorted/tShieldSystem.h"

// HYPOTHESIS: the analysis splits this constructor after its exception-handling prolog. The
// prolog (mov eax, <unwind table>; call __EH_prolog) is at 0x0044054f, recorded as a 10-byte
// function of its own, and the body is recorded from 0x00440559. The source below is the
// whole constructor, so the rebuilt function carries those ten bytes in front of the scored
// range; the instructions after them follow the original one for one.
// HYPOTHESIS: four parameters (ret 0x10), as for the other ship systems; the last two go to
// vftable slot 108.
// FUNCTION: STARFLEETOP 0x00440559
tShieldGroup::tShieldGroup(int param_1, int param_2, int param_3, int param_4)
{
    field_7c = 0.0f;
    field_28 = param_2;
    field_74 = 0.0f;
    field_78 = 0.0f;
    field_80 = 0.0f;
    field_58 = false;
    field_59 = false;
    // HYPOTHESIS: param_1 is the owning ship.
    field_2c = (fn_owner_type*)param_1;
    field_0c = 0x36;
    field_1c = 0x3c;
    field_b0 = -1.0f;
    field_a0 = 0;
    field_a4 = 0;
    field_a8 = 0;
    field_ac = 0;
    field_b8 = false;
    field_84[0] = 64.0f;
    field_84[1] = 53.12f;
    field_84[2] = 45.44f;
    field_84[3] = 40.0f;
    field_84[4] = 35.84f;
    field_84[5] = 32.0f;
    field_24 = new tIndividualShipSystemList;
    field_5c = 0;
    field_60 = 0;
    field_64 = 0;
    field_68 = 0;
    field_6c = 0;
    field_70 = 0;
    vfunction109(param_3, param_4);
}

// FUNCTION: STARFLEETOP 0x00440664
float tShieldGroup::vfunction85()
{
    return field_7c;
}

// FUNCTION: STARFLEETOP 0x00440668
float tShieldGroup::vfunction91()
{
    return field_78;
}

// FUNCTION: STARFLEETOP 0x0044066c
float tShieldGroup::vfunction92()
{
    return field_b4;
}

// FUNCTION: STARFLEETOP 0x00440673
// HYPOTHESIS: returns the reinforcement capacitor (field_74).
float tShieldGroup::vfunction94()
{
    return field_74;
}

// FUNCTION: STARFLEETOP 0x00440677
// HYPOTHESIS: counts one more shield online or switching (field_a4).
void tShieldGroup::vfunction107()
{
    field_a4++;
}

// FUNCTION: STARFLEETOP 0x0044067e
// HYPOTHESIS: says whether the requested charge per turn (field_7c) reaches field_b0, the
// level the shield system raises the group to.
bool tShieldGroup::vfunction108()
{
    if (field_7c < field_b0)
    {
        return false;
    }
    return true;
}

// MATCH-ONLY: stands in for the compiler-generated scalar deleting destructor, which has no
// source line the harness could claim. The original declares no destructor for tShieldGroup,
// so the compiler wrote this body: the implicit destructor chain down to tListMember's, then
// operator delete when bit 0 of the flag is set.
// FUNCTION: STARFLEETOP 0x00440692
tShieldGroup* tShieldGroup::scalar_deleting_destructor(unsigned char flags)
{
    this->tIndividualShipSystem::~tIndividualShipSystem();
    if (flags & 1)
        operator delete(this);
    return this;
}

// FUNCTION: STARFLEETOP 0x004406ae
tShieldGroup::~tShieldGroup()
{
}

// FUNCTION: STARFLEETOP 0x004406b9
// HYPOTHESIS: says whether the group changed since the last call, and clears the flag.
bool tShieldGroup::fn_004406b9()
{
    if (field_b8)
    {
        field_b8 = false;
        return true;
    }
    return false;
}

// FUNCTION: STARFLEETOP 0x004406d1
void tShieldGroup::vfunction109(int param_1, int param_2)
{
}

// FUNCTION: STARFLEETOP 0x0044089f
void tShieldGroup::vfunction87(float param_1, float param_2, float param_3, float param_4, float param_5, float param_6)
{
    if (field_5c)
    {
        int level = (int)param_1;
        field_5c->vfunction57(level);
        field_5c->vfunction55(level);
    }
    if (field_60)
    {
        int level = (int)param_2;
        field_60->vfunction57(level);
        field_60->vfunction55(level);
    }
    if (field_64)
    {
        int level = (int)param_3;
        field_64->vfunction57(level);
        field_64->vfunction55(level);
    }
    if (field_68)
    {
        int level = (int)param_4;
        field_68->vfunction57(level);
        field_68->vfunction55(level);
    }
    if (field_6c)
    {
        int level = (int)param_5;
        field_6c->vfunction57(level);
        field_6c->vfunction55(level);
    }
    if (field_70)
    {
        int level = (int)param_6;
        field_70->vfunction57(level);
        field_70->vfunction55(level);
    }
}

// HYPOTHESIS: reads back each shield's level (slot 55) when its slot 108 says yes, and 0
// for a missing shield.
// FUNCTION: STARFLEETOP 0x0044099e
void tShieldGroup::vfunction88(float* param_1, float* param_2, float* param_3, float* param_4, float* param_5, float* param_6)
{
    if (field_5c && field_5c->vfunction109() == true)
        *param_1 = (float)field_5c->vfunction56();
    else
        *param_1 = 0.0f;
    if (field_60 && field_60->vfunction109() == true)
        *param_2 = (float)field_60->vfunction56();
    else
        *param_2 = 0.0f;
    if (field_64 && field_64->vfunction109() == true)
        *param_3 = (float)field_64->vfunction56();
    else
        *param_3 = 0.0f;
    if (field_68 && field_68->vfunction109() == true)
        *param_4 = (float)field_68->vfunction56();
    else
        *param_4 = 0.0f;
    if (field_6c && field_6c->vfunction109() == true)
        *param_5 = (float)field_6c->vfunction56();
    else
        *param_5 = 0.0f;
    if (field_70 && field_70->vfunction109() == true)
        *param_6 = (float)field_70->vfunction56();
    else
        *param_6 = 0.0f;
}

// HYPOTHESIS: six byte-sized levels at 0x008405d8, indexed by the value at +0x944 of the
// owner's slot-107 object less one.
extern unsigned char g_008405d8[6];

// HYPOTHESIS: a float of unknown value at 0x0079cb50, the level every shield starts at.
extern float g_0079cb50;

// HYPOTHESIS: returns the minimum shield level (field_b0), filling it from the table above the
// first time it is asked (it starts at -1).
// FUNCTION: STARFLEETOP 0x00440abc
float tShieldGroup::vfunction95()
{
    if (field_b0 < 0.0f && field_2c)
    {
        int index = field_2c->vfunction108()->field_944 - 1;
        field_b0 = (index >= 0 && index < 6) ? g_008405d8[index] : 0.0f;
    }
    return field_b0;
}

// HYPOTHESIS: the group's per-turn update: param_1 is the energy available, param_2 the time
// step. It sets every shield to a starting level the first time, takes the minimum shield
// level, lets each shield count itself into the four counters, then moves the requested charge
// (field_7c) toward the total desired energy plus the reinforcement level.
// FUNCTION: STARFLEETOP 0x00440b11
void tShieldGroup::vfunction86(float param_1, float param_2)
{
    vfunction33_result* ship = field_2c->vfunction108();
    if (!field_59 && g_00967af4->vfunction4())
    {
        // HYPOTHESIS: the starting level, from the constant at 0x0079cb50.
        vfunction87(g_0079cb50, g_0079cb50, g_0079cb50, g_0079cb50, g_0079cb50, g_0079cb50);
    }
    field_59 = true;
    if (field_b0 < 0.0f && field_2c)
    {
        int index = field_2c->vfunction108()->field_944 - 1;
        field_b0 = (index >= 0 && index < 6) ? g_008405d8[index] : 0.0f;
        if (g_00967af4->vfunction4() && field_b0 > 1.0f)
        {
            field_b0 = 1.0f;
        }
    }
    field_a0 = 0;
    field_a4 = 0;
    field_a8 = 0;
    field_ac = 0;
    if (field_5c)
        field_5c->fn_0043ff2d(param_2, field_7c, &field_a0, &field_a4, &field_a8, &field_ac);
    if (field_70)
        field_70->fn_0043ff2d(param_2, field_7c, &field_a0, &field_a4, &field_a8, &field_ac);
    if (field_60)
        field_60->fn_0043ff2d(param_2, field_7c, &field_a0, &field_a4, &field_a8, &field_ac);
    if (field_68)
        field_68->fn_0043ff2d(param_2, field_7c, &field_a0, &field_a4, &field_a8, &field_ac);
    if (field_6c)
        field_6c->fn_0043ff2d(param_2, field_7c, &field_a0, &field_a4, &field_a8, &field_ac);
    if (field_64)
        field_64->fn_0043ff2d(param_2, field_7c, &field_a0, &field_a4, &field_a8, &field_ac);
    if (field_a4 == 0 && field_78 > 0.0f)
    {
        field_58 = false;
        vfunction90(0.0f);
    }
    else if (field_a4 > 0 && field_b0 > field_78)
    {
        field_58 = true;
        vfunction90(field_b0);
    }
    else if (field_a4 > 0 && field_ac == 0 && field_b4 > 0.0f)
    {
        vfunction90(field_b0);
        vfunction93(0.0f);
    }
    if (field_7c > param_1)
    {
        field_7c = param_1;
    }
    else
    {
        // The rate factor is the compiler constant at 0x00795bf4, which holds 4.0 in the exe
        // (read from its .rdata; the byte match does not check constant values).
        float target = field_78 + field_b4;
        if (target < field_7c)
        {
            field_7c -= param_2 * field_b0 * 4.0f;
            if (field_7c < target)
                field_7c = target;
        }
        else if (target > field_7c)
        {
            if (field_b0 * 4.0f < field_b4)
                field_7c += param_2 * field_b4;
            else
                field_7c += param_2 * field_b0 * 4.0f;
            if (target < field_7c)
                field_7c = target;
        }
    }
    if (ship->field_870 == 0x13)
        field_74 = 100.0f;
    else
        field_74 = param_1 - field_b0;
}

// HYPOTHESIS: param_1 is a shield's system type (0x37..0x3c, one per facing, after the group's
// own 0x36); the call goes to that shield's vftable slot 100.
// FUNCTION: STARFLEETOP 0x00440f12
void tShieldGroup::vfunction99(int param_1)
{
    tShieldSystem* shield;
    switch (param_1)
    {
    case 0x37:
        shield = field_5c;
        break;
    case 0x38:
        shield = field_60;
        break;
    case 0x39:
        shield = field_64;
        break;
    case 0x3a:
        shield = field_68;
        break;
    case 0x3b:
        shield = field_6c;
        break;
    case 0x3c:
        shield = field_70;
        break;
    default:
        return;
    }
    if (shield)
        shield->vfunction101();
}

// HYPOTHESIS: the counterpart of vfunction99; the call goes to that shield's vftable slot 101.
// FUNCTION: STARFLEETOP 0x00440f55
void tShieldGroup::vfunction100(int param_1)
{
    tShieldSystem* shield;
    switch (param_1)
    {
    case 0x37:
        shield = field_5c;
        break;
    case 0x38:
        shield = field_60;
        break;
    case 0x39:
        shield = field_64;
        break;
    case 0x3a:
        shield = field_68;
        break;
    case 0x3b:
        shield = field_6c;
        break;
    case 0x3c:
        shield = field_70;
        break;
    default:
        return;
    }
    if (shield)
        shield->vfunction102();
}

// HYPOTHESIS: counts the shields that answer yes to slot 107 and whose level (slot 55) is at
// least 64.
// FUNCTION: STARFLEETOP 0x00440e2a
int tShieldGroup::vfunction101()
{
    int count = 0;
    if (field_5c && field_5c->vfunction108() == true && field_5c->vfunction56() >= 64)
        count++;
    if (field_70 && field_70->vfunction108() == true && field_70->vfunction56() >= 64)
        count++;
    if (field_60 && field_60->vfunction108() == true && field_60->vfunction56() >= 64)
        count++;
    if (field_68 && field_68->vfunction108() == true && field_68->vfunction56() >= 64)
        count++;
    if (field_64 && field_64->vfunction108() == true && field_64->vfunction56() >= 64)
        count++;
    if (field_6c && field_6c->vfunction108() == true && field_6c->vfunction56() >= 64)
        count++;
    return count;
}

// HYPOTHESIS: counts the shields that answer yes to slot 108 and whose level (slot 55) is above
// zero.
// FUNCTION: STARFLEETOP 0x00440f98
int tShieldGroup::vfunction102()
{
    int count = 0;
    if (field_5c && field_5c->vfunction109() == true && field_5c->vfunction56() > 0)
        count++;
    if (field_70 && field_70->vfunction109() == true && field_70->vfunction56() > 0)
        count++;
    if (field_60 && field_60->vfunction109() == true && field_60->vfunction56() > 0)
        count++;
    if (field_68 && field_68->vfunction109() == true && field_68->vfunction56() > 0)
        count++;
    if (field_64 && field_64->vfunction109() == true && field_64->vfunction56() > 0)
        count++;
    if (field_6c && field_6c->vfunction109() == true && field_6c->vfunction56() > 0)
        count++;
    return count;
}

// HYPOTHESIS: passes the call on to every shield's vftable slot 100 (vfunction101), as
// vfunction99 does for one shield.
// FUNCTION: STARFLEETOP 0x0044107b
void tShieldGroup::vfunction103()
{
    if (field_5c)
        field_5c->vfunction101();
    if (field_60)
        field_60->vfunction101();
    if (field_70)
        field_70->vfunction101();
    if (field_68)
        field_68->vfunction101();
    if (field_64)
        field_64->vfunction101();
    if (field_6c)
        field_6c->vfunction101();
}

// HYPOTHESIS: passes the call on to every shield's vftable slot 101 (vfunction102), as
// vfunction100 does for one shield.
// FUNCTION: STARFLEETOP 0x004410da
void tShieldGroup::vfunction104()
{
    if (field_5c)
        field_5c->vfunction102();
    if (field_60)
        field_60->vfunction102();
    if (field_70)
        field_70->vfunction102();
    if (field_68)
        field_68->vfunction102();
    if (field_64)
        field_64->vfunction102();
    if (field_6c)
        field_6c->vfunction102();
}

// HYPOTHESIS: sets the group's true-online flag (field_58) and passes the call on to every
// shield's vftable slot 102 (vfunction103).
// FUNCTION: STARFLEETOP 0x00441139
void tShieldGroup::vfunction105()
{
    field_58 = true;
    if (field_5c)
        field_5c->vfunction103();
    if (field_60)
        field_60->vfunction103();
    if (field_70)
        field_70->vfunction103();
    if (field_68)
        field_68->vfunction103();
    if (field_64)
        field_64->vfunction103();
    if (field_6c)
        field_6c->vfunction103();
}

// HYPOTHESIS: the counterpart of vfunction105: clears the group's flag (field_58), passes the
// call on to every shield's vftable slot 103 (vfunction104), then gives the owner's slot 106
// and the group's minimum shield level (vfunction90) zero.
// FUNCTION: STARFLEETOP 0x0044119c
void tShieldGroup::vfunction106()
{
    field_58 = false;
    if (field_5c)
        field_5c->vfunction104();
    if (field_60)
        field_60->vfunction104();
    if (field_70)
        field_70->vfunction104();
    if (field_68)
        field_68->vfunction104();
    if (field_64)
        field_64->vfunction104();
    if (field_6c)
        field_6c->vfunction104();
    if (field_2c)
        field_2c->vfunction107(0.0f);
    vfunction90(0.0f);
}

// HYPOTHESIS: returns the member shield for a facing index (0..5), or null for any other index.
// FUNCTION: STARFLEETOP 0x00441226
tShieldSystem* tShieldGroup::fn_00441226(int param_1)
{
    switch (param_1)
    {
    case 0:
        return field_5c;
    case 1:
        return field_60;
    case 2:
        return field_64;
    case 3:
        return field_68;
    case 4:
        return field_6c;
    case 5:
        return field_70;
    }
    return 0;
}

// HYPOTHESIS: forwards a per-facing query to the shield fn_00441226 returns, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x00441261
int tShieldGroup::vfunction98(int param_1)
{
    tShieldSystem* shield = fn_00441226(param_1);
    if (shield)
        return shield->vfunction107();
    return 0;
}

// FUNCTION: STARFLEETOP 0x0044127f
bool tShieldGroup::vfunction97(int param_1)
{
    switch (param_1)
    {
    case 60:
        if (field_70 && field_70->vfunction56() >= 64 && field_70->vfunction109() &&
            field_70->vfunction108())
            return true;
        break;
    case 59:
        if (field_6c && field_6c->vfunction56() >= 64 && field_6c->vfunction109() &&
            field_6c->vfunction108())
            return true;
        break;
    case 58:
        if (field_68 && field_68->vfunction56() >= 64 && field_68->vfunction109() &&
            field_68->vfunction108())
            return true;
        break;
    case 57:
        if (field_64 && field_64->vfunction56() >= 64 && field_64->vfunction109() &&
            field_64->vfunction108())
            return true;
        break;
    case 56:
        if (field_60 && field_60->vfunction56() >= 64 && field_60->vfunction109() &&
            field_60->vfunction108())
            return true;
        break;
    case 55:
        if (field_5c && field_5c->vfunction56() >= 64 && field_5c->vfunction109() &&
            field_5c->vfunction108())
            return true;
        break;
    }
    return false;
}

// HYPOTHESIS: applies param_2 points of damage to the shields and returns what gets through.
// param_1 picks the facings: one bit per shield (0x01..0x20), 0x3f for all six at once (the
// reinforcement absorbs first), or 0x40 for the weakest shields first, which hands out the
// damage a little at a time through the one-bit form.
// FUNCTION: STARFLEETOP 0x0044140c
int tShieldGroup::vfunction89(unsigned int param_1, int param_2)
{
    int remaining = param_2;
    int absorbed = 0;
    if (param_1 == 0x3f)
    {
        // Damage to every facing: split it six ways; reinforcement takes what the up shields
        // absorb.
        int count = vfunction101();
        int reinforcement = (int)(field_74 * field_84[count - 1]);
        if (reinforcement < 0)
            reinforcement = 0;
        int share = param_2 / 6;
        remaining = 0;
        if (!vfunction97(0x37))
        {
            if (field_5c && field_5c->vfunction109())
            {
                int level = field_5c->vfunction56();
                if (level >= share)
                    field_5c->vfunction48(share);
                else
                {
                    field_5c->vfunction48(level);
                    remaining += share - level;
                }
            }
            else
                remaining += share;
        }
        else
            absorbed += share;
        if (!vfunction97(0x38))
        {
            if (field_60 && field_60->vfunction109())
            {
                int level = field_60->vfunction56();
                if (level >= share)
                    field_60->vfunction48(share);
                else
                {
                    field_60->vfunction48(level);
                    remaining += share - level;
                }
            }
            else
                remaining += share;
        }
        else
            absorbed += share;
        if (!vfunction97(0x39))
        {
            if (field_64 && field_64->vfunction109())
            {
                int level = field_64->vfunction56();
                if (level >= share)
                    field_64->vfunction48(share);
                else
                {
                    field_64->vfunction48(level);
                    remaining += share - level;
                }
            }
            else
                remaining += share;
        }
        else
            absorbed += share;
        if (!vfunction97(0x3a))
        {
            if (field_68 && field_68->vfunction109())
            {
                int level = field_68->vfunction56();
                if (level >= share)
                    field_68->vfunction48(share);
                else
                {
                    field_68->vfunction48(level);
                    remaining += share - level;
                }
            }
            else
                remaining += share;
        }
        else
            absorbed += share;
        if (!vfunction97(0x3b))
        {
            if (field_6c && field_6c->vfunction109())
            {
                int level = field_6c->vfunction56();
                if (level >= share)
                    field_6c->vfunction48(share);
                else
                {
                    field_6c->vfunction48(level);
                    remaining += share - level;
                }
            }
            else
                remaining += share;
        }
        else
            absorbed += share;
        if (!vfunction97(0x3c))
        {
            if (field_70 && field_70->vfunction109())
            {
                int level = field_70->vfunction56();
                if (level >= share)
                    field_70->vfunction48(share);
                else
                {
                    field_70->vfunction48(level);
                    remaining += share - level;
                }
            }
            else
                remaining += share;
        }
        else
            absorbed += share;

        if (reinforcement < absorbed)
        {
            // The reinforcement is spent; the reinforced shields take the rest.
            int excess = absorbed - reinforcement;
            field_74 = 0.0f;
            float minLevel = field_b0;
            if (minLevel < field_7c)
                field_7c = minLevel;
            share = excess / count;
            if (vfunction97(0x37) == true)
            {
                int level = field_5c->vfunction56();
                if (level >= share)
                    field_5c->vfunction48(share);
                else
                {
                    field_5c->vfunction48(level);
                    remaining += share - level;
                }
            }
            if (vfunction97(0x38) == true)
            {
                int level = field_60->vfunction56();
                if (level >= share)
                    field_60->vfunction48(share);
                else
                {
                    field_60->vfunction48(level);
                    remaining += share - level;
                }
            }
            if (vfunction97(0x39) == true)
            {
                int level = field_64->vfunction56();
                if (level >= share)
                    field_64->vfunction48(share);
                else
                {
                    field_64->vfunction48(level);
                    remaining += share - level;
                }
            }
            if (vfunction97(0x3a) == true)
            {
                int level = field_68->vfunction56();
                if (level >= share)
                    field_68->vfunction48(share);
                else
                {
                    field_68->vfunction48(level);
                    remaining += share - level;
                }
            }
            if (vfunction97(0x3b) == true)
            {
                int level = field_6c->vfunction56();
                if (level >= share)
                    field_6c->vfunction48(share);
                else
                {
                    field_6c->vfunction48(level);
                    remaining += share - level;
                }
            }
            if (vfunction97(0x3c) == true)
            {
                int level = field_70->vfunction56();
                if (level >= share)
                    field_70->vfunction48(share);
                else
                {
                    field_70->vfunction48(level);
                    remaining += share - level;
                }
            }
        }
        else
        {
            field_74 -= absorbed / field_84[count - 1];
            if (field_74 + field_b0 < field_7c)
                field_7c = field_74 + field_b0;
        }
        return remaining;
    }
    else if (param_1 == 0x40)
    {
        // Damage to the weakest shields first: find the lowest level and how many share it.
        int numLowest = 0;
        int lowest = 999999999;
        int numShields = 0;
        if (field_5c)
        {
            numShields++;
            int level = (int)field_5c->vfunction100();
            if (level < lowest)
            {
                numLowest = 1;
                lowest = level;
            }
            else if (level == lowest)
                numLowest++;
        }
        if (field_60)
        {
            numShields++;
            int level = (int)field_60->vfunction100();
            if (level < lowest)
            {
                lowest = level;
                numLowest = 1;
            }
            else if (level == lowest)
                numLowest++;
        }
        if (field_64)
        {
            numShields++;
            int level = (int)field_64->vfunction100();
            if (level < lowest)
            {
                lowest = level;
                numLowest = 1;
            }
            else if (level == lowest)
                numLowest++;
        }
        if (field_68)
        {
            numShields++;
            int level = (int)field_68->vfunction100();
            if (level < lowest)
            {
                lowest = level;
                numLowest = 1;
            }
            else if (level == lowest)
                numLowest++;
        }
        if (field_6c)
        {
            numShields++;
            int level = (int)field_6c->vfunction100();
            if (level < lowest)
            {
                lowest = level;
                numLowest = 1;
            }
            else if (level == lowest)
                numLowest++;
        }
        if (field_70)
        {
            numShields++;
            int level = (int)field_70->vfunction100();
            if (level < lowest)
            {
                lowest = level;
                numLowest = 1;
            }
            else if (level == lowest)
                numLowest++;
        }
        if (numShields == 0)
            return param_2;
        if (numShields <= numLowest + 1)
            return vfunction89(0x3f, param_2);

        int weight1 = 0;
        int weight2 = 0;
        int weight20 = 0;
        int weight10 = 0;
        int weight4 = 0;
        int weight8 = 0;
        if (field_5c)
        {
            if ((int)field_5c->vfunction100() == lowest)
                weight1 = numShields - numLowest;
            else
                weight1 = 1;
        }
        if (field_60)
        {
            if ((int)field_60->vfunction100() == lowest)
                weight2 = numShields - numLowest;
            else
                weight2 = 1;
        }
        if (field_64)
        {
            if ((int)field_64->vfunction100() == lowest)
                weight4 = numShields - numLowest;
            else
                weight4 = 1;
        }
        if (field_68)
        {
            if ((int)field_68->vfunction100() == lowest)
                weight8 = numShields - numLowest;
            else
                weight8 = 1;
        }
        if (field_6c)
        {
            if ((int)field_6c->vfunction100() == lowest)
                weight10 = numShields - numLowest;
            else
                weight10 = 1;
        }
        if (field_70)
        {
            if ((int)field_70->vfunction100() == lowest)
                weight20 = numShields - numLowest;
            else
                weight20 = 1;
        }

        int left = 0;
        while (remaining > 0)
        {
            if (remaining > weight1)
            {
                left += vfunction89(0x1, weight1);
                remaining -= weight1;
            }
            else
            {
                left += vfunction89(0x1, remaining);
                remaining = 0;
            }
            if (remaining > weight2)
            {
                left += vfunction89(0x2, weight2);
                remaining -= weight2;
            }
            else if (remaining > 0)
            {
                left += vfunction89(0x2, remaining);
                remaining = 0;
            }
            if (remaining > weight4)
            {
                left += vfunction89(0x4, weight4);
                remaining -= weight4;
            }
            else if (remaining > 0)
            {
                left += vfunction89(0x4, remaining);
                remaining = 0;
            }
            if (remaining > weight8)
            {
                left += vfunction89(0x8, weight8);
                remaining -= weight8;
            }
            else if (remaining > 0)
            {
                left += vfunction89(0x8, remaining);
                remaining = 0;
            }
            if (remaining > weight10)
            {
                left += vfunction89(0x10, weight10);
                remaining -= weight10;
            }
            else if (remaining > 0)
            {
                left += vfunction89(0x10, remaining);
                remaining = 0;
            }
            if (remaining > weight20)
            {
                left += vfunction89(0x20, weight20);
                remaining -= weight20;
            }
            else if (remaining > 0)
            {
                left += vfunction89(0x20, remaining);
                remaining = 0;
            }
        }
        remaining = left;
    }
    else
    {
        // Damage to one facing, one bit per shield.
        tShieldSystem* shield;
        switch (param_1)
        {
        case 0x1:
            shield = field_5c;
            break;
        case 0x2:
            shield = field_60;
            break;
        case 0x4:
            shield = field_64;
            break;
        case 0x8:
            shield = field_68;
            break;
        case 0x10:
            shield = field_6c;
            break;
        case 0x20:
            shield = field_70;
            break;
        default:
            shield = 0;
            break;
        }
        if (shield && shield->vfunction109() == true && shield->vfunction108() == true &&
            shield->vfunction56() >= 64 && field_7c >= field_b0)
        {
            // A reinforced shield: the reinforcement takes the damage first.
            int count = vfunction101();
            float* factor = &field_84[count - 1];
            int reinforcement = (int)(field_74 * *factor);
            if (reinforcement < param_2)
            {
                field_74 = 0.0f;
                remaining = param_2 - reinforcement / 64 * 64;
            }
            else
            {
                remaining = 0;
                field_74 -= param_2 / *factor;
            }
            if (field_58)
            {
                if (field_b0 + field_74 < field_7c)
                    field_7c = field_b0 + field_74;
            }
        }
        if (remaining > 0 && shield && shield->vfunction109() == true)
        {
            int level = shield->vfunction56();
            if (level >= remaining)
            {
                shield->vfunction48(remaining);
                remaining = 0;
            }
            else
            {
                shield->vfunction48(level);
                remaining += level / -64 * 64;
            }
        }
    }
    return remaining;
}

// FUNCTION: STARFLEETOP 0x004413d8
int tShieldGroup::vfunction96(int param_1)
{
    if (vfunction97(param_1))
    {
        int count = vfunction101();
        float value = field_74;
        value *= field_84[count - 1];
        return (int)value;
    }
    return 0;
}
