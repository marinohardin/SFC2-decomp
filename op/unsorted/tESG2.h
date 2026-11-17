//	tESG2.h: tESG2, the ESG weapon system, as other units use it.
// Evidence:  class (RTTI; original file unknown)

#ifndef tESG2_h
#define tESG2_h 1

#include "unsorted/tBaseWeapon.h"

// HYPOTHESIS: tESG2 derives from tBaseWeapon; tShipSystemsExt's 0x0044cb6f casts a weapon to
// it with dynamic_cast.
class tESG2 : public tBaseWeapon
{
public:
    // HYPOTHESIS: a count 0x0044cb6f sums over the ship's ESGs.
    int fn_004dd414();
};

#endif // tESG2_h
