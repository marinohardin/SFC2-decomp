#ifndef tPlayerRelayCCreator_h
#define tPlayerRelayCCreator_h

#include "unsorted/tMetaClientCreator.h"

// HYPOTHESIS: derives from tMetaClientCreator: it is 0x44 bytes and its users call the inherited
// function at 0x00607460 on it (0x006084e4).
class tPlayerRelayCCreator : public tMetaClientCreator {
public:
    tPlayerRelayCCreator();
    virtual ~tPlayerRelayCCreator();
    virtual void vfunction3();
};

#endif
