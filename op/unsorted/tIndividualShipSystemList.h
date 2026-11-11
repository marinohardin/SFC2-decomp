//	tIndividualShipSystemList.h: tIndividualShipSystemList, a list of a ship's systems.
// Evidence:  class (original file unknown)

#ifndef tIndividualShipSystemList_h
#define tIndividualShipSystemList_h 1

class tIndividualShipSystemList
{
public:
    // HYPOTHESIS: the constructor starts at 0x00437153 (mov eax, <unwind table>;
    // call __EH_prolog); the analysis records its body separately at 0x0043715d.
    tIndividualShipSystemList();

    // HYPOTHESIS: layout unknown; tShieldGroup's constructor allocates 0x580 bytes for one.
    char pad00[0x580];
};

#endif
