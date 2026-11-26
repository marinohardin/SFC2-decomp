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

// FUNCTION: STARFLEETOP 0x004e770a
int tPlasmaLauncher::fn_004e770a()
{
    int count = 0;
    if (field_3c >= field_38)
        count = 1;
    if (field_dc >= field_38)
        count++;
    if (field_e0 >= field_38)
        count++;
    if (field_e4 >= field_38)
        count++;
    char banks = vfunction106();
    if (count <= banks)
        return count;
    return banks;
}

// FUNCTION: STARFLEETOP 0x004e775b
unsigned tPlasmaLauncher::mStreamOutSystemSpecificData(tSFCStream &s, unsigned aFlags, unsigned mode)
{
    tBaseWeapon::mStreamOutSystemSpecificData(s, aFlags, mode);
    if ((aFlags & 8) > 0 || ((aFlags & 0x20) <= 0 && (aFlags & 0x80) > 0))
        aFlags = vfunction24(s, aFlags, mode);
    return aFlags;
}

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

// FUNCTION: STARFLEETOP 0x004e8a69
void tPlasmaLauncher::vfunction65(char param_1)
{
    char cVar1 = field_fc;
    float local_c = 0.0f;
    if (param_1 == 1)
        param_1 = 8;
    int iVar2 = field_f8;
    if (iVar2 == 6 || iVar2 == 7)
        field_fc = 0;
    else
        field_fc = param_1;
    int uVar3 = *(int *)((char *)this + 0x10c);
    field_d4 = 0.0f;
    if (param_1 == 0 || param_1 == 10)
        vfunction141(0);
    else
        vfunction141(3);
    switch (uVar3)
    {
    case 0:
        if (param_1 == 0)
        {
            if (cVar1 != 0)
                local_c = 0.64285713f;
            goto L_case6;
        }
        if (cVar1 == 0)
            local_c = 1.5555556f;
        field_38 = 14.0f;
        field_34 = 4.66f;
        break;
    case 6:
    L_case6:
        field_38 = 9.0f;
        field_34 = 3.0f;
        break;
    case 1:
        if (param_1 == 0)
        {
            if (cVar1 != 0)
                local_c = 0.6666667f;
            field_38 = 8.0f;
            field_34 = 2.66f;
        }
        else
        {
            if (cVar1 == 0)
                local_c = 1.5f;
            field_38 = 12.0f;
            field_34 = 4.0f;
        }
        break;
    case 2:
        if (param_1 == 0)
        {
            if (cVar1 != 0)
                local_c = 0.7f;
            field_38 = 7.0f;
            field_34 = 2.33f;
        }
        else
        {
            if (cVar1 == 0)
                local_c = 1.4285715f;
            field_38 = 10.0f;
            field_34 = 3.33f;
        }
        break;
    case 3:
        vfunction141(0);
        field_fc = 0;
        field_38 = 5.0f;
        if (field_f8 != 3)
        {
            field_34 = 2.5f;
            break;
        }
        goto L_004e8c01;
    case 4:
    case 5:
        vfunction141(0);
        if (param_1 != 8 && param_1 != 10)
            field_fc = 0;
        field_38 = 5.0f;
    L_004e8c01:
        field_34 = 1.67f;
        break;
    case 7:
        field_34 = 2.5f;
        field_38 = 7.5f;
    }
    vfunction126(local_c);
    field_d4 = field_34;
    vfunction124(field_e8);
}
