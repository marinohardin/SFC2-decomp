// Tiger/twfsys.cpp

#include "Tiger/tgfsys.h"

class twFileSystemPart {
public:
    virtual tVolIterator mGetVolIterator(tSession session);
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
