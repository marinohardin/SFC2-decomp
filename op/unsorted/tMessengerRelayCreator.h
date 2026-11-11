#ifndef tMessengerRelayCreator_h
#define tMessengerRelayCreator_h

#include "unsorted/tMetaClientCreator.h"

// HYPOTHESIS: derives from tMetaClientCreator: it is 0x44 bytes and its users call the inherited
// function at 0x00607460 on it (0x006084e4).
class tMessengerRelayCreator : public tMetaClientCreator {
public:
    tMessengerRelayCreator();
    virtual ~tMessengerRelayCreator();
    virtual void vfunction3();
};

#endif
