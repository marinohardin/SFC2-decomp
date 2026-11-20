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
// FUNCTION: STARFLEETOP 0x00401990
tFighterSpec::~tFighterSpec()
{
}

// FUNCTION: STARFLEETOP 0x004019f7
tFighterSpec::tFighterSpec(const tFighterSpec& other)
{
    m04 = other.m04;
    m08 = other.m08;
    m18 = other.m18;
    m1c = other.m1c;
    m20 = other.m20;
    m24 = other.m24;
    m28 = other.m28;
    m2c = other.m2c;
    m30 = other.m30;
    m34 = other.m34;
    m38 = other.m38;
    m3c = other.m3c;
    m40 = other.m40;
    m44 = other.m44;
    m48 = other.m48;
    m58 = other.m58;
    for (int i = 0; i < 5; i++)
    {
        m68[i].m00 = other.m68[i].m00;
        m68[i].m0c = other.m68[i].m0c;
        m68[i].m08 = other.m68[i].m08;
        m68[i].m04 = other.m68[i].m04;
    }
}
