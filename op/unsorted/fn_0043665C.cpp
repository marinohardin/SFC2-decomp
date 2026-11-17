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
