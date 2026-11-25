#include "Common/Thread/Optex.h"
#include "unsorted/tPlayerRelayCCreator.h"

// MATCH-ONLY: inline in the SDK, as in unsorted/fn_0042A000.cpp.
inline tOptex::tGuard::~tGuard()
{
    if (fNeedToLeave) {
        fOptex->Leave();
    }
}

// HYPOTHESIS: the owner class is not recovered; it holds a tOptex at +0xb8.
class fn_00600035_owner {
public:
    char pad[0xb8];
    tOptex m_b8;
    void fn_00600035(tMetaClientCreator **out);
};

// FUNCTION: STARFLEETOP 0x00600035
void fn_00600035_owner::fn_00600035(tMetaClientCreator **out)
{
    tOptex::tGuard guard(&m_b8);
    *out = new tPlayerRelayCCreator;
}
