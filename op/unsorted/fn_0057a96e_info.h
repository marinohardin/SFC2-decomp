#if !defined( fn_0057a96e_info_h )
#define fn_0057a96e_info_h

#include "Common/SmartPtr.h"

// HYPOTHESIS: fn_0057a96e_info stands in for nSwitch::tAddress::tStatusChangeInfo, which is
// not declared yet. It is 12 bytes: a smart pointer to an object whose tRefCount sits at +0xc,
// then two dwords.
class fn_0057a96e_base {
public:
    virtual ~fn_0057a96e_base();

    int field_4;
    int field_8;
};

class fn_0057a96e_target : public fn_0057a96e_base, public tRefCount {
};

struct fn_0057a96e_info {
    // HYPOTHESIS: a tConstSmartPtr; a tSmartPtr leaves the generated copy out of line.
    tConstSmartPtr<fn_0057a96e_target> field_0;
    int field_4;
    int field_8;
};

#endif
