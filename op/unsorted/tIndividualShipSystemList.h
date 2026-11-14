//	tIndividualShipSystemList.h: tIndividualShipSystemList, a list of a ship's systems.
// Evidence:  class (original file unknown)

#ifndef tIndividualShipSystemList_h
#define tIndividualShipSystemList_h 1

class tIndividualShipSystemList
{
public:
    // HYPOTHESIS: the constructor starts at 0x00437153; its body is recorded at 0x0043715d.
    tIndividualShipSystemList();
    // HYPOTHESIS: slots 0..21 are not recovered; slot 22 adds a member (tShieldGroup slot 108).
    virtual void vfunction0(); virtual void vfunction1(); virtual void vfunction2(); virtual void vfunction3(); virtual void vfunction4(); virtual void vfunction5(); virtual void vfunction6(); virtual void vfunction7(); virtual void vfunction8(); virtual void vfunction9(); virtual void vfunction10();
    virtual void vfunction11(); virtual void vfunction12(); virtual void vfunction13(); virtual void vfunction14(); virtual void vfunction15(); virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18(); virtual void vfunction19(); virtual void vfunction20(); virtual void vfunction21(); virtual void vfunction22(class tIndividualShipSystem* pSystem);
    char pad04[0x580 - 4];	// HYPOTHESIS: layout unknown; tShieldGroup allocates 0x580 bytes
};

#endif
