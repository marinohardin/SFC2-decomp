#include "unsorted/tFighterSpec.h"

// FUNCTION: STARFLEETOP 0x00401862
tFighterSpec::tFighterSpec()
{
    m04 = 0;
    m08 = "";
    m18 = 0;
    m1c = 1;
    m20 = 0;
    m24 = 0;
    m28 = 0;
    m2c = 0;
    m30 = 1;
    m34 = 0;
    m38 = 0;
    m3c = 999;
    m40 = 1;
    m44 = 0;
    m48 = "";
    m58 = "";
    for (int i = 0; i < 5; i++)
    {
        m68[i].m00 = 0;
        m68[i].m0c = -1;
        m68[i].m08 = -1;
        m68[i].m04 = -1;
    }
}

// FUNCTION: STARFLEETOP 0x00401974
tFighterSpec::~tFighterSpec()
{
}
