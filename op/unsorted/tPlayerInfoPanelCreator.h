#ifndef tPlayerInfoPanelCreator_h
#define tPlayerInfoPanelCreator_h

#include "unsorted/tMetaClientCreator.h"

// HYPOTHESIS: derives from tMetaClientCreator: it is 0x44 bytes and its users call the inherited
// function at 0x00607460 on it (0x006085bf).
class tPlayerInfoPanelCreator : public tMetaClientCreator {
public:
    tPlayerInfoPanelCreator();
    virtual ~tPlayerInfoPanelCreator();
    virtual void vfunction3();
};

#endif
