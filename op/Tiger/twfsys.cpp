// Tiger/twfsys.cpp

#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"

class twFileSystemPart {
public:
    virtual tVolIterator mGetVolIterator(tSession session);
    virtual tVolume mGetNamedVolume(tSession session, tString name);
};

// Claimed and scored as partial: the original body (about 1.5 KB) walks drive letters 'A' to
// 'Z' with GetVolumeInformation-style calls and builds the iterator from tString and tVolume
// temporaries. None of that is rebuilt yet, so only the signature is real.
// FUNCTION: STARFLEETOP 0x0062e1c0
tVolIterator twFileSystemPart::mGetVolIterator(tSession session)
{
    tVolIterator it;
    return it;
}

// HYPOTHESIS: only the first test of the original's loop is written; the second (the name of the
// volume the part's cookie manager resolves) needs tFSCookiePart and twVolume, which are declared
// only in tgfsys.cpp and cannot be repeated here.
// FUNCTION: STARFLEETOP 0x0062e7c0
tVolume twFileSystemPart::mGetNamedVolume(tSession session, tString name)
{
    tVolIterator it = mGetVolIterator(session);
    tVolume v;

    for (v = it.mFirst(); v.mValid(); v = it.mNext(v))
    {
        if (name.mStrICmp(v.mGetName()) == 0)
            return v;
    }
    return tVolume();
}
