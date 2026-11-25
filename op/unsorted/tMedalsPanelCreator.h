#ifndef tMedalsPanelCreator_h
#define tMedalsPanelCreator_h

#include "unsorted/tMetaClientCreator.h"

class tMedalsPanelCreator : public tMetaClientCreator {
public:
    tMedalsPanelCreator();
    virtual ~tMedalsPanelCreator();
    virtual void vfunction3();
};

typedef char tMedalsPanelCreator_size_is_0x44[ sizeof( tMedalsPanelCreator ) == 0x44 ? 1 : -1 ];

#endif
