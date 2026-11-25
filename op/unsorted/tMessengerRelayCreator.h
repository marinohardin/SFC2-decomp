#ifndef tMessengerRelayCreator_h
#define tMessengerRelayCreator_h

#include "unsorted/tMetaClientCreator.h"

// HYPOTHESIS: layout unknown; 0xd0 is the operator new size in tMessengerRelayCreator::vfunction3.
class tMessengerRelayC {
public:
    tMessengerRelayC();

    char pad00[0xd0];
};

// HYPOTHESIS: derives from tMetaClientCreator: it is 0x44 bytes and its users call the inherited
// function at 0x00607460 on it (0x006084e4).
class tMessengerRelayCreator : public tMetaClientCreator {
public:
    tMessengerRelayCreator();
    virtual ~tMessengerRelayCreator();
    virtual void vfunction3();
};

typedef char tMessengerRelayCreator_size_is_0x44[ sizeof( tMessengerRelayCreator ) == 0x44 ? 1 : -1 ];

#endif
