// tShieldSystem: a ship's shield system (a repairable ship system).
// Original file unknown.
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tShieldGroup.h"
#include "unsorted/tShieldSystem.h"

// HYPOTHESIS: the object a ship system keeps at +0x28; vftable slot 95 takes the system's
// +0x0c value and returns an int bonus.
class fn_field_28_type
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
    // vftable slot 95
    virtual int vfunction96(int param_1);
};

// FUNCTION: STARFLEETOP 0x0043fa4e
void tShieldSystem::vfunction52()
{
    tIndividualShipSystem::vfunction52();
    if (field_45 != field_80) {
        field_82 = true;
    }
}

// FUNCTION: STARFLEETOP 0x0043fa6a
void tShieldSystem::vfunction54(int param_1)
{
    tIndividualShipSystem::vfunction54(param_1);
    if (field_45 != field_80) {
        field_82 = true;
    }
}

// FUNCTION: STARFLEETOP 0x0043fa8c
void tShieldSystem::vfunction47(int value)
{
    tIndividualShipSystem::vfunction47(value);
    if (field_45 != field_80) {
        field_82 = true;
    }
}

// FUNCTION: STARFLEETOP 0x0043faae
void tShieldSystem::vfunction62()
{
    if (field_80) {
        if (field_45 != true) {
            vfunction81();
        }
        field_45 = true;
    }
}

// HYPOTHESIS: field serializers (cdecl): the field, the stream and context passed through,
// and the field's name.
void __cdecl fn_0040eafc(bool* value, int param_1, int param_3, const char* name);
void __cdecl fn_0040eb35(float* value, int param_1, int param_3, const char* name);

// FUNCTION: STARFLEETOP 0x0043face
// HYPOTHESIS: saves or loads the shield's fields after the base class's; the two identical
// save branches are merged by the compiler. fSwitchingReinforcedTime is written twice in the
// original.
unsigned int tShieldSystem::vfunction23(int param_1, unsigned int param_2, int param_3)
{
    tRepairableShipSystem::vfunction23(param_1, param_2, param_3);
    if ((param_2 & 0x08) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_84, param_1, param_3, "fSwitchingOnLineTime");
        fn_0040eb35(&field_8c, param_1, param_3, "fSwitchingReinforcedTime");
        fn_0040eafc(&field_81, param_1, param_3, "fReinforced");
        fn_0040eafc(&field_82, param_1, param_3, "fSwitchingOnLineStatus");
        fn_0040eb35(&field_8c, param_1, param_3, "fSwitchingReinforcedTime");
    }
    else if ((param_2 & 0x20) > 0)
    {
    }
    else if ((param_2 & 0x80) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_84, param_1, param_3, "fSwitchingOnLineTime");
        fn_0040eb35(&field_8c, param_1, param_3, "fSwitchingReinforcedTime");
        fn_0040eafc(&field_81, param_1, param_3, "fReinforced");
        fn_0040eafc(&field_82, param_1, param_3, "fSwitchingOnLineStatus");
        fn_0040eb35(&field_8c, param_1, param_3, "fSwitchingReinforcedTime");
    }
    else if ((param_2 & 0x40) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eafc(&field_81, param_1, param_3, "fReinforced");
        fn_0040eafc(&field_45, param_1, param_3, "fOnLine");
    }
    return param_2;
}

void __cdecl fn_0040e9df(int* value, int param_1, int param_3, const char* name);

// FUNCTION: STARFLEETOP 0x0043fbb5
// HYPOTHESIS: saves or loads the group's fields; unlike tShieldSystem it does not call a base
// class's slot 22 first.
unsigned int tShieldGroup::vfunction23(int param_1, unsigned int param_2, int param_3)
{
    if ((param_2 & 0x08) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_74, param_1, param_3, "fReinforcementCapacitor");
        fn_0040eb35(&field_78, param_1, param_3, "fTotalDesiredEnergy");
        fn_0040eb35(&field_7c, param_1, param_3, "fRequestedChargePerTurn");
        fn_0040eb35(&field_9c, param_1, param_3, "fLastPowerAccepted");
        fn_0040e9df(&field_a0, param_1, param_3, "fNumberOnline");
        fn_0040e9df(&field_a4, param_1, param_3, "fNumberOnlineOrSwitching");
        fn_0040e9df(&field_a8, param_1, param_3, "fNumberReinforced");
        fn_0040e9df(&field_ac, param_1, param_3, "fNumberThatWillBeReinforced");
        fn_0040eafc(&field_58, param_1, param_3, "fTrueOnline");
        fn_0040eb35(&field_b0, param_1, param_3, "fMinShieldLevel");
        fn_0040eb35(&field_b4, param_1, param_3, "fDesiredReinforcementLevel");
    }
    else if ((param_2 & 0x20) > 0)
    {
    }
    else if ((param_2 & 0x80) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_74, param_1, param_3, "fReinforcementCapacitor");
        fn_0040eb35(&field_78, param_1, param_3, "fTotalDesiredEnergy");
        fn_0040eb35(&field_7c, param_1, param_3, "fRequestedChargePerTurn");
        fn_0040eb35(&field_9c, param_1, param_3, "fLastPowerAccepted");
        fn_0040e9df(&field_a0, param_1, param_3, "fNumberOnline");
        fn_0040e9df(&field_a4, param_1, param_3, "fNumberOnlineOrSwitching");
        fn_0040e9df(&field_a8, param_1, param_3, "fNumberReinforced");
        fn_0040e9df(&field_ac, param_1, param_3, "fNumberThatWillBeReinforced");
    }
    else if ((param_2 & 0x40) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040eb35(&field_74, param_1, param_3, "fReinforcementCapacitor");
        fn_0040eb35(&field_78, param_1, param_3, "fTotalDesiredEnergy");
        fn_0040eb35(&field_7c, param_1, param_3, "fRequestedChargePerTurn");
        fn_0040eafc(&field_58, param_1, param_3, "fTrueOnline");
        fn_0040eb35(&field_b0, param_1, param_3, "fMinShieldLevel");
        fn_0040eb35(&field_b4, param_1, param_3, "fDesiredReinforcementLevel");
    }
    return param_2;
}

// FUNCTION: STARFLEETOP 0x0043fdf9
// HYPOTHESIS: sets the desired reinforcement level and returns the total desired energy.
float tShieldGroup::vfunction93(float param_1)
{
    field_b4 = param_1;
    return field_78;
}

// FUNCTION: STARFLEETOP 0x0043fe09
// HYPOTHESIS: sets the minimum shield level and clamps the total desired energy to it.
float tShieldGroup::vfunction90(float param_1)
{
    if (param_1 < field_b0)
    {
        // The double constant at 0x0079c980 holds 0.0001 in the exe (read from its .rdata).
        if (field_78 < field_b0 && param_1 >= 0.0001)
        {
            field_78 = field_b0;
        }
        else
        {
            field_78 = 0.0f;
        }
    }
    else if (param_1 == field_b0)
    {
        if (field_78 > field_b0)
        {
            field_78 = field_b0;
        }
        else
        {
            field_78 = field_b0;
            if (field_a4 == 0)
            {
                vfunction105();
            }
        }
    }
    return field_78;
}

// FUNCTION: STARFLEETOP 0x0043fe84
// HYPOTHESIS: returns the shield's strength: none while slot 108 says no or below 0x40,
// plus the bonus answered by the object at +0x28 when reinforced.
float tShieldSystem::vfunction100()
{
    if (vfunction109() && field_10 >= 0x40)
    {
        // HYPOTHESIS: field_28 holds a pointer to the object that answers the bonus.
        if (field_81 == true && field_28 != 0)
        {
            return (float)((fn_field_28_type*)field_28)->vfunction96(field_0c) + field_10;
        }
        return (float)field_10;
    }
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x0043fed6
// HYPOTHESIS: starts the reinforcement switch timer unless already reinforced.
void tShieldSystem::vfunction101()
{
    if (field_81 == true)
    {
        field_88 = false;
        return;
    }
    if (field_88 == false)
    {
        field_88 = true;
        field_8c = 0.0f;
    }
}

// FUNCTION: STARFLEETOP 0x0043ff02
// HYPOTHESIS: starts the reinforcement switch timer unless already unreinforced; the
// counterpart of vfunction101.
void tShieldSystem::vfunction102()
{
    if (field_81 == false)
    {
        field_88 = false;
        return;
    }
    if (field_88 == false)
    {
        field_88 = true;
        field_8c = 0.0f;
    }
}

// FUNCTION: STARFLEETOP 0x0043ff2d
// HYPOTHESIS: accumulates the elapsed time, sets field_48 to the next interval's end without
// testing it (the cloak's loop tests it first), finishes the on-line and reinforcement switches
// whose timers have run out, then counts this shield into the group's four totals.
void tShieldSystem::fn_0043ff2d(float param_1, float param_2, int* param_3, int* param_4,
                                int* param_5, int* param_6)
{
    field_50 += param_1;
    double now = g_00c7c148;
    fn_0090be9c_type* source = g_0090be9c;
    field_48 = (double)source->vfunction7() / (double)source->field_20 * 100000.0 + now;
    param_1 = field_50;
    field_50 = 0.0f;
    // HYPOTHESIS: field_28 holds the owning tShieldGroup.
    tShieldGroup* group = (tShieldGroup*)field_28;
    if (field_45 == true && field_10 == 0)
    {
        field_45 = false;
        field_82 = false;
        fn_00440224();
        if (group != 0)
        {
            group->field_b8 = true;
        }
    }
    if (field_82 == true)
    {
        field_84 += param_1;
        if (field_80 == false && (group->field_b0 > param_2 || field_84 > 0.0))
        {
            field_45 = false;
            field_82 = false;
            vfunction81();
            fn_00440224();
            if (group != 0)
            {
                group->field_b8 = true;
            }
        }
        else if (field_80 == true && field_10 != 0 && group->field_b0 <= param_2 &&
                 field_84 > 0.25)
        {
            field_45 = field_80;
            fn_0044036b();
            field_82 = false;
            if (group != 0)
            {
                group->field_b8 = true;
            }
            vfunction81();
        }
    }
    if (field_88 == true)
    {
        field_8c += param_1;
        if (field_81 == true && field_8c > 0.25)
        {
            field_81 = false;
            vfunction81();
            field_88 = false;
        }
        else if (field_81 == false && field_8c > 0.25)
        {
            field_81 = true;
            vfunction81();
            field_88 = false;
        }
    }
    if (field_45 == true)
    {
        (*param_3)++;
        if (field_82 == false)
        {
            (*param_4)++;
        }
    }
    else if (field_82 == true)
    {
        (*param_4)++;
    }
    if (field_81 == true)
    {
        (*param_5)++;
        if (field_88 == false)
        {
            (*param_6)++;
        }
    }
    else if (field_88 == true)
    {
        (*param_6)++;
    }
}

// FUNCTION: STARFLEETOP 0x00440116
// HYPOTHESIS: asks the shield to come on line: starts the on-line switch timer when the shield
// is off line and not destroyed, and raises the group's level to its minimum when it is below.
void tShieldSystem::vfunction103()
{
    field_80 = true;
    if (field_45 != true && field_10 != 0)
    {
        if (field_82 == false)
        {
            field_82 = true;
            field_84 = 0.0f;
            // HYPOTHESIS: field_28 holds the owning tShieldGroup.
            tShieldGroup* group = (tShieldGroup*)field_28;
            if (group != 0)
            {
                float minLevel = group->field_b0;
                if (group->vfunction91() < minLevel)
                {
                    group->vfunction107();
                    // MATCH-ONLY: the local orders the argument load before the this load.
                    float level = group->field_b0;
                    group->vfunction90(level);
                    group->field_58 = true;
                }
            }
        }
    }
    else
    {
        field_82 = false;
    }
}

// FUNCTION: STARFLEETOP 0x00440199
// HYPOTHESIS: asks the shield to go off line: clears the request and, when the shield is on
// line, starts the on-line switch timer; the counterpart of vfunction103.
void tShieldSystem::vfunction104()
{
    field_80 = false;
    vfunction81();
    if (field_45 == false)
    {
        field_82 = false;
        return;
    }
    if (field_82 == false)
    {
        field_82 = true;
        field_84 = 0.0f;
    }
}

// FUNCTION: STARFLEETOP 0x004401d6
// HYPOTHESIS: takes the shield off line at once, without the switch timer, when it is on line
// and not already switching, then asks it to come back on line through vfunction103.
void tShieldSystem::vfunction105()
{
    vfunction81();
    if (field_45 == false)
    {
        field_82 = false;
        return;
    }
    if (field_82 == false)
    {
        // HYPOTHESIS: field_28 holds the owning tShieldGroup.
        tShieldGroup* group = (tShieldGroup*)field_28;
        if (group != 0)
        {
            group->field_b8 = true;
        }
        field_45 = false;
        field_82 = false;
        fn_00440224();
        vfunction103();
    }
}

// FUNCTION: STARFLEETOP 0x00440224
// HYPOTHESIS: reports the shield going off line to the object at 0x0084b47c, with an event id
// chosen by the field at +0x86c of the object the group's vftable slot 32 returns.
void tShieldSystem::fn_00440224()
{
    // HYPOTHESIS: field_28 holds the owning tShieldGroup.
    vfunction33_result* result;
    switch (((tShieldGroup*)field_28)->vfunction33()->field_86c)
    {
    case 5:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x2b, result->field_1c);
        break;
    case 6:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x43, result->field_1c);
        break;
    case 7:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x45, result->field_1c);
        break;
    case 0:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x21, result->field_1c);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x91, result->field_1c);
        break;
    case 4:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x27, result->field_1c);
        break;
    case 1:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x23, result->field_1c);
        break;
    case 2:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x25, result->field_1c);
        break;
    case 3:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x29, result->field_1c);
        break;
    default:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x21, result->field_1c);
        break;
    }
}

// FUNCTION: STARFLEETOP 0x0044036b
// HYPOTHESIS: reports the shield coming on line, the counterpart of fn_00440224; each event id
// is one below the one fn_00440224 sends for the same value.
void tShieldSystem::fn_0044036b()
{
    // HYPOTHESIS: field_28 holds the owning tShieldGroup.
    vfunction33_result* result;
    switch (((tShieldGroup*)field_28)->vfunction33()->field_86c)
    {
    case 5:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x2a, result->field_1c);
        break;
    case 6:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x42, result->field_1c);
        break;
    case 7:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x44, result->field_1c);
        break;
    case 0:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x20, result->field_1c);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x90, result->field_1c);
        break;
    case 4:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x26, result->field_1c);
        break;
    case 1:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x22, result->field_1c);
        break;
    case 2:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x24, result->field_1c);
        break;
    case 3:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x28, result->field_1c);
        break;
    default:
        result = ((tShieldGroup*)field_28)->vfunction33();
        g_0084b47c->vfunction7(0x20, result->field_1c);
        break;
    }
}

// FUNCTION: STARFLEETOP 0x004404b2
// HYPOTHESIS: grades the reinforcement bonus the object at +0x28 answers: 0 when the shield is
// not working, below 0x40 or not reinforced, or the bonus is zero; then 1 below 0x100, 2 below
// 0x280 and 3 above.
int tShieldSystem::vfunction106()
{
    if (vfunction109() && field_10 >= 0x40 && field_81)
    {
        int bonus = ((fn_field_28_type*)field_28)->vfunction96(field_0c);
        if (bonus != 0)
        {
            if (bonus < 0x100)
            {
                return 1;
            }
            return (bonus >= 0x280) + 2;
        }
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00440502
// HYPOTHESIS: grades the shield strength from 0 to 10: 0 when the shield is not working or
// the strength is below 0.00001, otherwise the strength times 1/64, times 0.2, plus 1, capped
// at 10; the constants are the exe's (0x0079c990, 0x0079c98c, 0x0079c988 and 0x007959cc).
// The single exit through one result variable and the two-step scaling are the original's shape.
int tShieldSystem::vfunction107()
{
    int grade;
    if (!vfunction109())
    {
        grade = 0;
    }
    else
    {
        float level = vfunction100();
        if (level < 0.00001)
        {
            grade = 0;
        }
        else
        {
            level *= 0.015625f;
            grade = (int)(level * 0.2f + 1.0f);
            if (grade > 10)
            {
                grade = 10;
            }
        }
    }
    return grade;
}
