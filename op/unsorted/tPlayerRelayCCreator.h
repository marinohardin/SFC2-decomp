#ifndef tPlayerRelayCCreator_h
#define tPlayerRelayCCreator_h

#include "unsorted/tMetaClientCreator.h"

// HYPOTHESIS: the class is 0xd0 bytes (operator new size in tPlayerRelayCCreator::vfunction3); its layout is not recovered.
class tPlayerRelayC {
public:
    tPlayerRelayC();

    char m_0x00[0xd0];
};

// HYPOTHESIS: derives from tMetaClientCreator: it is 0x44 bytes and its users call the inherited
// function at 0x00607460 on it (0x006084e4).
class tPlayerRelayCCreator : public tMetaClientCreator {
public:
    tPlayerRelayCCreator();
    virtual ~tPlayerRelayCCreator();
    virtual void vfunction3();
};

typedef char tPlayerRelayCCreator_size_is_0x44[ sizeof( tPlayerRelayCCreator ) == 0x44 ? 1 : -1 ];

#endif
