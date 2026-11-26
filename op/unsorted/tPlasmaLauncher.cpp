#include "unsorted/tPlasmaLauncher.h"

// HYPOTHESIS: the object 0x004e824a returns has a message slot at vtable offset 0x18
// that takes a text and a level.
class tMessageSink
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6(const char *text, int level);
};

tMessageSink *fn_004e824a();

// FUNCTION: STARFLEETOP 0x004e80b0
void tPlasmaLauncher::vfunction125(int aValue)
{
    unsigned param_1 = aValue;
    float local_8 = 0.0f;
    switch (field_f8)
    {
    case 0:
    case 6:
    case 7:
        *(int *)((char *)this + 0x10c) = param_1;
        break;
    case 1:
        if (param_1 < 1)
            fn_004e824a()->vfunction6("Plasma Larger than an S attempted to be loaded into an S", 2);
        else
            *(int *)((char *)this + 0x10c) = param_1;
        break;
    case 2:
        if (param_1 < 2)
            fn_004e824a()->vfunction6("Plasma Larger than an G attempted to be loaded into an G", 2);
        else
            *(int *)((char *)this + 0x10c) = param_1;
        break;
    case 3:
        if (param_1 < 3)
            fn_004e824a()->vfunction6("Plasma Larger than an F attempted to be loaded into an F", 2);
        else
            *(int *)((char *)this + 0x10c) = param_1;
        break;
    case 4:
        if (param_1 < 4)
            fn_004e824a()->vfunction6("Plasma Larger than an I attempted to be loaded into an I", 2);
        else
            *(int *)((char *)this + 0x10c) = param_1;
        break;
    case 5:
        if (param_1 < 5)
            fn_004e824a()->vfunction6("Plasma Larger than an I attempted to be loaded into an I", 2);
        else
            *(int *)((char *)this + 0x10c) = param_1;
        break;
    }
    if (field_fc && field_4a8 == 0 && param_1 == 3)
        local_8 = 0.35714287f;
    else if (field_fc && field_4a8 == 1 && param_1 == 3)
        local_8 = 0.41666666f;
    else if (field_fc && field_4a8 == 2 && param_1 == 3)
        local_8 = 0.5f;
    else
    {
        // MATCH-ONLY: mixing the local and the member in the tests keeps the original's repeated compares.
        int iVar4 = field_4a8;
        if (iVar4 == 0 && param_1 == 1)
            local_8 = 0.8888889f;
        else if (field_4a8 == 0 && param_1 == 2)
            local_8 = 0.7777778f;
        else if (field_4a8 == 0 && param_1 == 3)
            local_8 = 0.5555556f;
        else if (iVar4 == 1 && param_1 == 2)
            local_8 = 0.875f;
        else if (field_4a8 == 1 && param_1 == 3)
            local_8 = 0.625f;
        else if (iVar4 == 2 && param_1 == 3)
            local_8 = 0.71428573f;
    }
    vfunction65(field_fc);
    field_4a8 = param_1;
    vfunction126(local_8);
}
