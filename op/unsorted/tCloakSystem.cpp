// tCloakSystem: the ship's cloaking device (a repairable ship system).
// Original file unknown; class layout reconstructed from the constructor only.

#include "unsorted/tCloakSystem.h"

// FUNCTION: STARFLEETOP 0x004381e4
// HYPOTHESIS: four parameters (ret 0x10); only the first two are used here.
tCloakSystem::tCloakSystem(int param_1, int param_2, int param_3, int param_4)
{
    field_28 = param_2;
    // HYPOTHESIS: param_1 is the owning ship.
    field_2c = (fn_owner_type*)param_1;
    field_0c = 0x3d;
    field_1c = 0x46;
    field_78 = 0xd7;
    vfunction94(0, -1);
    // HYPOTHESIS: dispatched through the vftable in the original, so not called on the
    // constructed object's static type.
    tIndividualShipSystem* system = this;
    system->vfunction49(0x400 - field_14);
    system->vfunction47(0x400 - field_10);
    field_78 = 0;
    vfunction94(1000, -1);
    field_88 = 0.0;
    field_84 = 0;
    field_90 = 0.0;
    field_18 = fn_00453c60("Cloak", 0, 0);
    field_98 = 0;
    field_80 = false;
    vfunction63();
    field_50 = 0.0f;
    field_48 = 0.0;
}

// FUNCTION: STARFLEETOP 0x004382be
void tCloakSystem::vfunction100(int param_1)
{
    field_84 = param_1;
}

// FUNCTION: STARFLEETOP 0x004382cb
// HYPOTHESIS: a member of tCloakSystem (this in ecx); returns and clears field_80.
bool tCloakSystem::fn_004382cb()
{
    if (field_80)
    {
        field_80 = false;
        return true;
    }
    return false;
}

// HYPOTHESIS: a float global (or a float literal) at 0x0079a118.
extern float g_0079a118;

// FUNCTION: STARFLEETOP 0x004382e3
// HYPOTHESIS: a member of tCloakSystem; this is not used.
float tCloakSystem::fn_004382e3()
{
    return g_0079a118;
}

// FUNCTION: STARFLEETOP 0x004382ea
// HYPOTHESIS: sets the cloak state (field_98); state 0 -> other reports event 0x31,
// state 6 -> other reports event 0x32, through the ship returned by vftable slot 32.
void tCloakSystem::vfunction103(int param_1)
{
    if (field_98 == param_1)
    {
        return;
    }
    if (field_98 == 0)
    {
        if (vfunction33() != 0)
        {
            vfunction33_result* result = vfunction33();
            g_0084b47c->vfunction7(0x31, result->field_1c);
        }
    }
    else if (field_98 == 6)
    {
        if (vfunction33() != 0)
        {
            vfunction33_result* result = vfunction33();
            g_0084b47c->vfunction7(0x32, result->field_1c);
        }
    }
    field_98 = param_1;
}

// FUNCTION: STARFLEETOP 0x0043835d
// HYPOTHESIS: reports through param_1 whether field_90 is positive while the cloak state is
// not 0, and returns the cloak state.
int tCloakSystem::vfunction102(bool* param_1)
{
    if (field_90 > 0.0 && field_98 != 0)
    {
        *param_1 = true;
    }
    else
    {
        *param_1 = false;
    }
    return field_98;
}

// HYPOTHESIS: a double global (or a double literal) at 0x0079a120.
extern double g_0079a120;

// FUNCTION: STARFLEETOP 0x00438390
// HYPOTHESIS: a member of tCloakSystem; resets field_90 from a double constant.
void tCloakSystem::fn_00438390()
{
    field_90 = g_0079a120;
}

// HYPOTHESIS: the object returned by fn_004249d3; vftable slot 33 returns a count.
class fn_004249d3_result
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
    virtual int vfunction34(); // slot 33
};

// HYPOTHESIS: a global object at 0x008e1060 with a lookup by id.
class fn_008e1060_type
{
public:
    fn_004249d3_result* fn_004249d3(int id);
};
extern fn_008e1060_type g_008e1060;

// HYPOTHESIS: a double global (or a double literal) at 0x0079a128.
extern double g_0079a128;

// FUNCTION: STARFLEETOP 0x0043839d
// HYPOTHESIS: tries to engage the cloak (field_84 is its cost or delay); state 0 -> 1 and
// states 7..11 -> 1..5.
void tCloakSystem::vfunction62()
{
    fn_004249d3_result* other = 0;
    if (field_2c->vfunction108() != 0)
    {
        other = g_008e1060.fn_004249d3(field_2c->vfunction108()->field_1c);
    }
    if (field_84 == 0 || g_00967af4->vfunction4() || field_2c->vfunction44())
    {
        return;
    }
    field_2c->vfunction108();
    if (other != 0 && other->vfunction34() > 0)
    {
        return;
    }
    if (field_98 == 0)
    {
        field_88 = g_0079a128;
        field_40 = (float)field_84;
        field_98 = 1;
        if (vfunction33() != 0)
        {
            vfunction33_result* result = vfunction33();
            g_0084b47c->vfunction7(0x31, result->field_1c);
        }
    }
    if (field_98 >= 7 && field_98 <= 11)
    {
        field_88 = g_0079a128;
        field_40 = (float)field_84;
        field_98 -= 6;
    }
    field_45 = true;
}

// FUNCTION: STARFLEETOP 0x004384a5
// HYPOTHESIS: disengages the cloak; state 6 -> 11 and states 1..5 -> 7..11.
void tCloakSystem::vfunction63()
{
    if (field_98 == 6)
    {
        field_88 = g_0079a128;
        field_98 = 11;
        field_40 = 0.0f;
        if (vfunction33() != 0)
        {
            vfunction33_result* result = vfunction33();
            g_0084b47c->vfunction7(0x32, result->field_1c);
        }
    }
    if (field_98 >= 1 && field_98 <= 5)
    {
        field_88 = g_0079a128;
        field_40 = 0.0f;
        field_98 += 6;
    }
    field_45 = false;
}

// HYPOTHESIS: a named marker object (constructor at 0x0040ce21 takes the function's name);
// its destructor is empty, since the original's atexit thunk is a bare ret.
#include "unsorted/fn_0040ce21_type.h"






// FUNCTION: STARFLEETOP 0x00438522
// The name is the string the function hands to its marker object.
// HYPOTHESIS: param_2 is unused (ret 8); the range is doubled while fully cloaked (state 6)
// with field_90 not positive, then widened by 10..50 by cloak state unless the ship's flag at
// +0x110 is set.
float tCloakSystem::mDetermineCloakWeaponRange(float range, int param_2)
{
    static fn_0040ce21_type marker("tCloakSystem::mDetermineCloakWeaponRange");
    vfunction33_result* ship = field_2c->vfunction108();
    float result;
    if (field_90 <= 0.0 && field_98 == 6)
    {
        result = range + range;
    }
    else
    {
        result = range;
    }
    if (ship != 0 && !ship->field_110)
    {
        switch (field_98)
        {
        case 5:
        case 6:
        case 11:
            result += 50.0;
            break;
        case 4:
        case 10:
            result += 40.0;
            break;
        case 3:
        case 9:
            result += 30.0;
            break;
        case 2:
        case 8:
            result += 20.0;
            break;
        case 1:
        case 7:
            result += 10.0;
            break;
        }
    }
    return result;
}

// HYPOTHESIS: double and float constants in the original's read-only data.
extern double g_0079a148;
extern float g_007959c8;
extern float g_00795bf0;
extern float g_007959cc;

// FUNCTION: STARFLEETOP 0x00438607
// The name is the string the function hands to its marker object.
// HYPOTHESIS: both parameters are unused (ret 8); while fully cloaked (state 6) with field_90
// not positive, the modifier is stepped by a value derived from the global's count and total.
float tCloakSystem::mDetermineCloakSeekingWeaponDamageModifier(int param_1, int param_2)
{
    static fn_0040ce21_type marker("tCloakSystem::mDetermineCloakSeekingWeaponDamageModifier");
    if (field_90 <= 0.0 && field_98 == 6)
    {
        fn_0090be9c_type* source = g_0090be9c;
        int step = 1 - (int)((double)source->vfunction7() * g_0079a148 /
                             (double)source->field_20);
        if (step >= 3)
        {
            if (step < 5)
            {
                return g_007959c8;
            }
            if (step < 7)
            {
                return g_00795bf0;
            }
            return 0.0f;
        }
    }
    return g_007959cc;
}

// FUNCTION: STARFLEETOP 0x004386bc
// HYPOTHESIS: counts field_90 down by the elapsed time, stopping at zero.
void tCloakSystem::vfunction40(float param_1)
{
    if (field_90 > 0.0)
    {
        field_90 -= param_1;
        if (field_90 < 0.0)
        {
            field_90 = 0.0;
        }
    }
}

// FUNCTION: STARFLEETOP 0x004386f3
// HYPOTHESIS: per-frame update; accumulates elapsed time in field_50 and, once per interval,
// hands it to fn_0043d696 and steps the cloak state (field_98) one stage towards 0 or 6 each
// time the stage timer field_88 runs out.
void tCloakSystem::vfunction39(float param_1)
{
    vfunction33();
    fn_004249d3_result* other = 0;
    if (field_2c->vfunction108() != 0)
    {
        other = g_008e1060.fn_004249d3(field_2c->vfunction108()->field_1c);
    }
    field_50 += param_1;
    double now = g_00c7c148;
    field_2c->vfunction108();
    if (other != 0 && other->vfunction34() > 0)
    {
        vfunction63();
    }
    if (now - field_48 >= 100000.0)
    {
        fn_0090be9c_type* source = g_0090be9c;
        field_48 = (double)source->vfunction7() / (double)source->field_20 * 100000.0 + now;
        param_1 = field_50;
        field_50 = 0.0f;
        fn_0043d696(param_1);
        if (field_98 != 0 && field_98 != 6)
        {
            field_88 -= param_1;
            if (field_88 > 0.0)
            {
                return;
            }
            switch (field_98)
            {
            case 1:
            case 2:
            case 3:
            case 4:
                field_88 += g_0079a128;
                field_80 = true;
                field_98++;
                break;
            case 5:
                field_80 = true;
                field_98 = 6;
                field_88 = 0.0;
                break;
            case 7:
                field_98 = 0;
                field_80 = true;
                field_88 = 0.0;
                break;
            case 8:
            case 9:
            case 10:
            case 11:
                field_88 += g_0079a128;
                field_80 = true;
                field_98--;
                break;
            default:
                field_88 = 0.0;
                break;
            }
        }
    }
}

// HYPOTHESIS: field serializers (cdecl): the field, the stream and context passed through,
// and the field's name.
void __cdecl fn_0040e9df(int* value, int param_1, int param_3, const char* name);
void __cdecl fn_0040ec4c(double* value, int param_1, int param_3, const char* name);

// FUNCTION: STARFLEETOP 0x00438877
// HYPOTHESIS: saves or loads the cloak's fields after the slot 23 handler; the two identical
// save branches are merged by the compiler. The field names are the strings the original
// passes (field_98's is at 0x0083fe2c).
unsigned int tCloakSystem::vfunction23(int param_1, unsigned int param_2, int param_3)
{
    if ((param_2 & 0x08) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040e9df(&field_84, param_1, param_3, "fCloakCost");
        fn_0040ec4c(&field_88, param_1, param_3, "fUpDownTimer");
        fn_0040ec4c(&field_90, param_1, param_3, "fTemporarilyDownTimeRemaining");
        fn_0040e9df(&field_98, param_1, param_3, "fState");
    }
    else if ((param_2 & 0x20) > 0)
    {
    }
    else if ((param_2 & 0x80) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040e9df(&field_84, param_1, param_3, "fCloakCost");
        fn_0040ec4c(&field_88, param_1, param_3, "fUpDownTimer");
        fn_0040ec4c(&field_90, param_1, param_3, "fTemporarilyDownTimeRemaining");
        fn_0040e9df(&field_98, param_1, param_3, "fState");
    }
    else if ((param_2 & 0x40) > 0)
    {
        param_2 = vfunction24(param_1, param_2, param_3);
        fn_0040e9df(&field_98, param_1, param_3, "fState");
        fn_0040ec4c(&field_90, param_1, param_3, "fTemporarilyDownTimeRemaining");
    }
    return param_2;
}

// FUNCTION: STARFLEETOP 0x0043894d
// HYPOTHESIS: the cloak's running cost (field_84) while the cloak is not fully down
// (field_98 nonzero), otherwise nothing.
float tCloakSystem::vfunction72()
{
    if (field_84 != 0 && field_98 != 0)
    {
        return (float)field_84;
    }
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x00438974
// HYPOTHESIS: pays the cloak's running cost (field_84) out of the power offered in param_1
// while the cloak is not down; when the power is short the cloak drops (vfunction63, slot 62).
// param_2 is unused (ret 8). The else-if re-tests field_98 when the power is short and the
// cloak is already down; the compiler threads that jump straight to the exit.
float tCloakSystem::vfunction76(float param_1, int param_2)
{
    if (field_84 > param_1 && field_98 != 0)
    {
        vfunction63();
    }
    else if (field_98 != 0)
    {
        param_1 -= field_84;
    }
    return param_1;
}
