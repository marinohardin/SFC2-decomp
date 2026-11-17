//	fn_0043665C.cpp: functions of 0x0043665c to 0x004368b7 with no unit or class evidence.
// Evidence:  page (original file unknown)

#include "unsorted/tIndividualShipSystem.h"
#include "Source/entity/entity_physical.h"

// HYPOTHESIS: layout from the accesses only; the vtable sits at +0x0 and the
// stored dword at +0xc.
class tETrait
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7(int value);

    int field_4;
    int field_8;
    int field_c;
};

// FUNCTION: STARFLEETOP 0x0043665c
int tIndividualShipSystem::vfunction11()
{
    return field_14 >> 7;
}

// FUNCTION: STARFLEETOP 0x00436663
void tETrait::vfunction7(int value)
{
    field_c = value;
}

// HYPOTHESIS: the game's own copy of a list getter that sits in slot 17 of the
// tShipSpecManager and tIndividualShipSystemList vtables.
// FUNCTION: STARFLEETOP 0x0043666d
long tDynamicMemoryFreeList::vfunction17()
{
    return field_0xc;
}

// FUNCTION: STARFLEETOP 0x00436671
bool tIndividualShipSystem::vfunction14()
{
    if (field_0c < 0 || field_0c > 0x35)
        return false;
    return true;
}

// FUNCTION: STARFLEETOP 0x00436683
bool tIndividualShipSystem::vfunction15()
{
    if (field_0c == 0x36)
        return true;
    return false;
}

// FUNCTION: STARFLEETOP 0x0043668b
int tIndividualShipSystem::vfunction16()
{
    return field_20;
}

// FUNCTION: STARFLEETOP 0x0043668f
void tIndividualShipSystem::vfunction18(int param_1)
{
    field_18 = param_1;
}

// HYPOTHESIS: the next free value for the dword at +0x20 of any ship system.
extern int g_0090b518;

// FUNCTION: STARFLEETOP 0x00436699
void tIndividualShipSystem::vfunction19(int param_1)
{
    field_20 = param_1;
    if (param_1 >= g_0090b518)
        g_0090b518 = param_1 + 1;
}

// FUNCTION: STARFLEETOP 0x004366b1
int tEntity::vfunction12()
{
    return fTargetID;
}

// FUNCTION: STARFLEETOP 0x004366b5
void tEntity::vfunction15(float aRadius)
{
    fRadius = aRadius;
}

// FUNCTION: STARFLEETOP 0x004366bf
fn_owner_type* tIndividualShipSystem::vfunction43()
{
    return field_2c;
}

// FUNCTION: STARFLEETOP 0x004366c3
void tIndividualShipSystem::vfunction45(int param_1)
{
    vfunction44(param_1);
}

// FUNCTION: STARFLEETOP 0x004366d2
bool tIndividualShipSystem::vfunction46()
{
    if (field_24 != 0)
        return true;
    return false;
}

// FUNCTION: STARFLEETOP 0x004366da
void tIndividualShipSystem::vfunction55(int param_1)
{
    vfunction47(param_1 - field_10);
}

// FUNCTION: STARFLEETOP 0x004366ed
int tIndividualShipSystem::vfunction56()
{
    return field_10;
}

// FUNCTION: STARFLEETOP 0x004366f1
void tIndividualShipSystem::vfunction57(int param_1)
{
    vfunction49(param_1 - field_14);
}

// FUNCTION: STARFLEETOP 0x00436704
void tIndividualShipSystem::vfunction59(int param_1)
{
    vfunction57(param_1);
    vfunction55(param_1);
}

// FUNCTION: STARFLEETOP 0x00436725
void tIndividualShipSystem::vfunction61(int param_1)
{
    field_1c = param_1;
}

// FUNCTION: STARFLEETOP 0x0043672f
bool tIndividualShipSystem::vfunction64()
{
    return field_45;
}

// FUNCTION: STARFLEETOP 0x00436733
void tIndividualShipSystem::vfunction65(char param_1)
{
    field_30 = param_1;
}

// FUNCTION: STARFLEETOP 0x0043673d
char tIndividualShipSystem::vfunction66()
{
    return field_30;
}

// FUNCTION: STARFLEETOP 0x00436741
void tIndividualShipSystem::vfunction67(float param_1)
{
    field_38 = param_1;
}

// FUNCTION: STARFLEETOP 0x0043674b
float tIndividualShipSystem::vfunction68()
{
    return field_38;
}

// FUNCTION: STARFLEETOP 0x0043674f
void tIndividualShipSystem::vfunction69(int param_1)
{
    field_34 = param_1;
}

// FUNCTION: STARFLEETOP 0x00436759
void tIndividualShipSystem::vfunction71(float param_1)
{
    field_40 = param_1;
}

// FUNCTION: STARFLEETOP 0x00436763
void tIndividualShipSystem::vfunction75(float param_1)
{
    vfunction74(param_1, 0.0f);
}

// FUNCTION: STARFLEETOP 0x0043677c
float tIndividualShipSystem::vfunction79()
{
    return field_3c;
}

// FUNCTION: STARFLEETOP 0x00436780
bool tIndividualShipSystem::vfunction83()
{
    return field_44;
}

// FUNCTION: STARFLEETOP 0x00436784
void tIndividualShipSystem::vfunction84(bool param_1)
{
    field_44 = param_1;
}

// FUNCTION: STARFLEETOP 0x0043678e
tRepairableShipSystem::tRepairableShipSystem()
{
    field_70 = 0.0f;
    field_6c = 0;
    field_74 = 0;
    field_60 = 0.0f;
    field_58 = 0;
    field_5c = 0;
    field_64 = 1.0f;
    field_78 = 0;
    field_68 = 0.0f;
}
