// Tiger/tstringtag.cpp

#include "Tiger/tcglue.h"
#include "Tiger/tsay.h"
#include "Tiger/tstringtag.h"
#include "Tiger/tStrManagerPart.h"

// FUNCTION: STARFLEETOP 0x0062de40
tStringTag::tStringTag(char* param_1)
{
    clib::strcpy(m00, param_1);
    m10 = -1;
}

// FUNCTION: STARFLEETOP 0x0062de60
const tString& tStringTag::mGetString()
{
    if (m10 == -1)
        m10 = BindPart(tStrManagerPart)->mGetStringIndex(m00);
    return BindPart(tStrManagerPart)->mGetString(m10);
}

// FUNCTION: STARFLEETOP 0x0062df70
tStringGroupTag::tStringGroupTag(char* spec)
{
    m00[0] = 0;
    m10 = -1;
    clib::strcpy(m00, spec);
}

// FUNCTION: STARFLEETOP 0x0062dfa0
long tStringGroupTag::mGetNumElements()
{
    if (m10 == -1)
        m10 = BindPart(tStrManagerPart)->mGetStringIndex(m00);
    return BindPart(tStrManagerPart)->mGetNumElements(m10);
}

// FUNCTION: STARFLEETOP 0x0062e0b0
const tString& tStringGroupTag::mGetString(long index)
{
    if (m10 == -1)
        m10 = BindPart(tStrManagerPart)->mGetStringIndex(m00);
    return BindPart(tStrManagerPart)->mGetGrpStr(m10, index);
}
