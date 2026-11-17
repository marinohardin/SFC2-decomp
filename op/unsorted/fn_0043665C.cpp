//	fn_0043665C.cpp: functions of 0x0043665c to 0x004368b7 with no unit or class evidence.
// Evidence:  page (original file unknown)

#include "unsorted/tIndividualShipSystem.h"

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
