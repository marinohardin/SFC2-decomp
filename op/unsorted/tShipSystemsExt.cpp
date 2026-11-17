// Unit:      unsorted/tShipSystemsExt.cpp
// Functions of tShipSystemsExt (0x00448235 to 0x00448bf1); original file unknown.
#include "unsorted/tShipSystems.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tIndividualShipSystemList.h"
#include "unsorted/tShipSystemManager.h"
#include "unsorted/tTacticalStoresManager.h"
#include "unsorted/tNetControl.h"
#include "Tiger/tcglue.h"
#include <string.h>

extern float g_00799ed0;
extern float g_0079d528;
extern float g_007977d0;
extern float g_007959cc;
extern float g_0079d524;
extern double g_0090bef0;
extern double g_00c7c148;

// HYPOTHESIS: a 0x10-byte element type; the constructor builds four of them at +0x9c4 with
// the constructor at 0x00401fe6 and the destructor at 0x005f762d. Neither is recovered.
class c_00401fe6
{
public:
    c_00401fe6();
    ~c_00401fe6();

    long m00[4];
};

// HYPOTHESIS: slot 1 of m918, a tValue<float>, takes a float (the exe pushes a float 0); the
// shared header types that slot as an integer, so this view gives it a float.
class fn_00448235_value
{
public:
    virtual void vfunction1();
    virtual void vfunction2( float param_1 );
};

// RTTI: tShipSystemsExt : tShipSystems (vtable 0x0079d530, 177 slots).
// HYPOTHESIS: the overrides are not declared yet; the layout is read from the members the
// constructor (0x00448235) builds and sets.
class tShipSystemsExt : public tShipSystems
{
public:
    tShipSystemsExt( unsigned long param_1 );
    virtual ~tShipSystemsExt();
    virtual tShipSystemManager* vfunction53();
    virtual tTacticalStoresManager* vfunction54();
    virtual char* vfunction57();
    virtual char* vfunction58();
    virtual int vfunction59();
    virtual int vfunction60();
    virtual int vfunction61();
    virtual int vfunction62();
    virtual int vfunction63();
    virtual int vfunction64();
    virtual int vfunction65();
    virtual int vfunction66();
    virtual int vfunction67();
    virtual int vfunction68();
    virtual int vfunction69();
    virtual int vfunction70();
    virtual int vfunction71();
    virtual int vfunction72();
    virtual int vfunction73();
    virtual int vfunction74();
    virtual int vfunction75();
    virtual int vfunction76();
    virtual int vfunction77();
    virtual int vfunction78();
    virtual int vfunction79();
    virtual int vfunction80();
    virtual int vfunction81();
    virtual int vfunction82();
    virtual int vfunction83();
    virtual int vfunction84();
    virtual int vfunction85();
    virtual int vfunction86();
    virtual int vfunction87();
    virtual int vfunction88();
    virtual int vfunction89();
    virtual int vfunction90();
    virtual int vfunction91();
    virtual int vfunction92();
    virtual int vfunction93();
    virtual int vfunction95();
    virtual int vfunction96();
    virtual int vfunction97();
    virtual int vfunction98();
    virtual int vfunction99();
    virtual void vfunction107( float param_1 );

    tIndividualShipSystemList m04;
    char m584[0x128];
    int m6ac;
    double m6b0;
    double m6b8;
    double m6c0;
    tMinMaxFloat m6c8;
    float m6ec;
    float m6f0;
    tMinMaxFloat m6f4;
    tMinMaxFloat m718;
    float m73c;
    int m740;
    tMinMaxFloat m744;
    tMinMaxFloat m768;
    tMinMaxFloat m78c;
    tMinMaxFloat m7b0;
    tMinMaxFloat m7d4;
    tMinMaxFloat m7f8;
    tMinMaxFloat m81c;
    tMinMaxFloat m840;
    tMinMaxFloat m864;
    tMinMaxFloat m888;
    tMinMaxFloat m8ac;
    tMinMaxFloat m8d0;
    tMinMaxFloat m8f4;
    tValue<float> m918;
    int m934;
    double m938;
    char m940;
    char m941;
    int m944;
    int m948;
    char m94c;
    char m94d;
    char m94e;
    int m950;
    int m954;
    int m958;
    int m95c;
    int m960;
    int m964;
    tShipSystemManager m968;
    tTacticalStoresManager m984;
    int m9a0;
    int m9a4[3];
    int m9b0;
    int m9b4;
    float m9b8;
    char m9bc[5];
    char m9c1;
    c_00401fe6 m9c4[4];
    char ma04;
    char ma05;
    char ma06;
    char ma07;
    char ma08;
    char ma09;
};

// FUNCTION: STARFLEETOP 0x00448235
tShipSystemsExt::tShipSystemsExt( unsigned long param_1 )
{
    // HYPOTHESIS: the argument; it is stored at +0x6ac and handed to both managers.
    m6ac = param_1;
    m6c0 = 0.0;

    tMinMaxFloat* value = &m7b0;
    value->vfunction3( 0.0f );
    value = &m744;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m768;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m78c;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m7b0;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m7d4;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m7f8;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m864;
    value->vfunction5( 0.0f );
    value->vfunction4( g_0079d528 );
    value->vfunction3( 0.0f );
    value = &m888;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m840;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m8ac;
    value->vfunction5( 0.0f );
    value->vfunction4( 1.0f );
    value->vfunction3( 0.0f );
    value = &m8d0;
    value->vfunction5( 0.0f );
    value->vfunction4( 1.0f );
    value->vfunction3( 0.0f );
    ((fn_00448235_value*)&m918)->vfunction2( 0.0f );
    value = &m8f4;
    value->vfunction5( 0.0f );
    value->vfunction4( 1.0f );
    value->vfunction3( 0.0f );

    m6ec = 0.0f;
    m6f0 = 0.0f;
    value = &m718;
    value->vfunction3( 0.0f );
    value = &m6c8;
    m73c = 0.0f;
    m6c8.vfunction5( 0.0f );
    value->vfunction4( 1.0f );
    value->vfunction3( 1.0f );
    value = &m6f4;
    value->vfunction5( 0.0f );
    value->vfunction4( 1.0f );
    value->vfunction3( 1.0f );
    m740 = -1;

    clib::memset( m584, 0, sizeof( m584 ) );
    m938 = g_0090bef0;
    m6b0 = g_00c7c148;

    // HYPOTHESIS: the call fills the value through the pointer it is given; the header types
    // the argument as an integer. The exe reuses the argument's slot for it and converts it
    // as unsigned.
    param_1 = 0;
    gNetControl.fn_00431808( (int)&param_1 );
    m6b8 = ( param_1 * g_007977d0 + g_007959cc ) * g_0079d524 + g_00c7c148;
    m944 = 1;
    m948 = 1;
    m940 = 0;
    m941 = 0;
    m9a0 = 0;
    memset( m9a4, 0, sizeof( m9a4 ) );
    m9b8 = 0.0f;
    m950 = -1;
    m94c = 0;
    m9b0 = 0;
    m9b4 = 0;
    m94d = 0;
    m94e = 0;
    m954 = 0;
    m95c = 0;
    m960 = 0;
    m964 = 0;
    m968.fn_005e4a72( m6ac );
    m984.fn_005e4a72( m6ac );
    memset( m9bc, 5, sizeof( m9bc ) );
    m9c1 = 1;
    ma04 = 0;
    ma05 = 0;
    ma06 = 0;
    ma07 = 0;
    ma08 = 0;
    ma09 = 0;
}

// FUNCTION: STARFLEETOP 0x00448847
void tShipSystemsExt::vfunction107( float param_1 )
{
    m7f8.vfunction3( param_1 );
}

// HYPOTHESIS: the return type, as in tShipSystemsFighter::vfunction58; the original returns
// the address +0x648, 0xc4 bytes into the block at m584 that the constructor clears as one.
// FUNCTION: STARFLEETOP 0x00448861
char* tShipSystemsExt::vfunction58()
{
    return m584 + 0xc4;
}

// HYPOTHESIS: the return type, as in tShipSystemsFighter::vfunction57; the start of the block
// at m584.
// FUNCTION: STARFLEETOP 0x00448868
char* tShipSystemsExt::vfunction57()
{
    return m584;
}

// The original returns the address +0x968, the member m968.
// FUNCTION: STARFLEETOP 0x0044886f
tShipSystemManager* tShipSystemsExt::vfunction53()
{
    return &m968;
}

// The original returns the address +0x984, the member m984.
// FUNCTION: STARFLEETOP 0x00448876
tTacticalStoresManager* tShipSystemsExt::vfunction54()
{
    return &m984;
}

// HYPOTHESIS: as tShipSystemsFighter::vfunction59; the block at m584 holds a pointer at +0x48
// (+0x5cc) to an object whose +0x58 is returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x0044887d
int tShipSystemsExt::vfunction59()
{
    char* p = *(char**)( m584 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x58 );
    return 0;
}

// HYPOTHESIS: as vfunction59, through the same pointer at +0x5cc; returns its +0x5c, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x0044888e
int tShipSystemsExt::vfunction60()
{
    char* p = *(char**)( m584 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x5c );
    return 0;
}

// HYPOTHESIS: as vfunction59, through the same pointer at +0x5cc; returns its +0x60, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x0044889f
int tShipSystemsExt::vfunction61()
{
    char* p = *(char**)( m584 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x60 );
    return 0;
}

// HYPOTHESIS: as vfunction59, through the same pointer at +0x5cc; returns its +0x64, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004488b0
int tShipSystemsExt::vfunction62()
{
    char* p = *(char**)( m584 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x64 );
    return 0;
}

// HYPOTHESIS: as vfunction59, through the same pointer at +0x5cc; returns its +0x68, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004488c1
int tShipSystemsExt::vfunction63()
{
    char* p = *(char**)( m584 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x68 );
    return 0;
}

// HYPOTHESIS: the block at m584 starts with a pointer to an object whose +0xb0 is returned,
// or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004488d2
int tShipSystemsExt::vfunction64()
{
    char* p = *(char**)m584;
    if( p != 0 )
        return *(int*)( p + 0xb0 );
    return 0;
}

// HYPOTHESIS: as vfunction64, returning the object's +0xb4.
// FUNCTION: STARFLEETOP 0x004488e6
int tShipSystemsExt::vfunction65()
{
    char* p = *(char**)m584;
    if( p != 0 )
        return *(int*)( p + 0xb4 );
    return 0;
}

// HYPOTHESIS: as vfunction64, returning the object's +0xb8.
// FUNCTION: STARFLEETOP 0x004488fa
int tShipSystemsExt::vfunction66()
{
    char* p = *(char**)m584;
    if( p != 0 )
        return *(int*)( p + 0xb8 );
    return 0;
}

// HYPOTHESIS: as vfunction64, returning the object's +0xbc.
// FUNCTION: STARFLEETOP 0x0044890e
int tShipSystemsExt::vfunction67()
{
    char* p = *(char**)m584;
    if( p != 0 )
        return *(int*)( p + 0xbc );
    return 0;
}

// HYPOTHESIS: the block at m584 holds a second pointer at +0x14 (+0x598) to an object whose
// +0x80 is returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x00448922
int tShipSystemsExt::vfunction68()
{
    char* p = *(char**)( m584 + 0x14 );
    if( p != 0 )
        return *(int*)( p + 0x80 );
    return 0;
}

// HYPOTHESIS: as vfunction68, through the same pointer at +0x598; returns its +0x84, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x00448936
int tShipSystemsExt::vfunction69()
{
    char* p = *(char**)( m584 + 0x14 );
    if( p != 0 )
        return *(int*)( p + 0x84 );
    return 0;
}

// HYPOTHESIS: as vfunction68, through the same pointer at +0x598; returns its +0x88, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x0044894a
int tShipSystemsExt::vfunction70()
{
    char* p = *(char**)( m584 + 0x14 );
    if( p != 0 )
        return *(int*)( p + 0x88 );
    return 0;
}

// HYPOTHESIS: the block at m584 holds a pointer at +0x94 (+0x618) to an object whose +0x58 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x0044895e
int tShipSystemsExt::vfunction71()
{
    char* p = *(char**)( m584 + 0x94 );
    if( p != 0 )
        return *(int*)( p + 0x58 );
    return 0;
}

// HYPOTHESIS: as vfunction71, through the same pointer at +0x618; returns its +0x5c, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x0044896f
int tShipSystemsExt::vfunction72()
{
    char* p = *(char**)( m584 + 0x94 );
    if( p != 0 )
        return *(int*)( p + 0x5c );
    return 0;
}

// HYPOTHESIS: the block at m584 holds a pointer at +0x20 (+0x5a4) to an object whose +0x58 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x00448980
int tShipSystemsExt::vfunction73()
{
    char* p = *(char**)( m584 + 0x20 );
    if( p != 0 )
        return *(int*)( p + 0x58 );
    return 0;
}

// HYPOTHESIS: as vfunction73, through the same pointer at +0x5a4; returns its +0x5c, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x00448991
int tShipSystemsExt::vfunction74()
{
    char* p = *(char**)( m584 + 0x20 );
    if( p != 0 )
        return *(int*)( p + 0x5c );
    return 0;
}

// HYPOTHESIS: as vfunction73, through the same pointer at +0x5a4; returns its +0x60, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004489a2
int tShipSystemsExt::vfunction75()
{
    char* p = *(char**)( m584 + 0x20 );
    if( p != 0 )
        return *(int*)( p + 0x60 );
    return 0;
}

// HYPOTHESIS: the block at m584 holds a pointer at +0xa0 (+0x624) to an object whose +0x58 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004489b3
int tShipSystemsExt::vfunction77()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x58 );
    return 0;
}

// HYPOTHESIS: as vfunction77, through the same pointer at +0x624; returns its +0x60, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004489c4
int tShipSystemsExt::vfunction78()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x60 );
    return 0;
}

// HYPOTHESIS: as vfunction77, through the same pointer at +0x624; returns its +0x64, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004489d5
int tShipSystemsExt::vfunction79()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x64 );
    return 0;
}

// HYPOTHESIS: as vfunction77, through the same pointer at +0x624; returns its +0x68, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004489e6
int tShipSystemsExt::vfunction80()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x68 );
    return 0;
}

// HYPOTHESIS: as vfunction77, through the same pointer at +0x624; returns its +0x6c, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x004489f7
int tShipSystemsExt::vfunction81()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x6c );
    return 0;
}

// HYPOTHESIS: as vfunction77, through the same pointer at +0x624; returns its +0x70, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x00448a08
int tShipSystemsExt::vfunction82()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x70 );
    return 0;
}

// HYPOTHESIS: as vfunction77, through the same pointer at +0x624; returns its +0x5c, or 0
// when there is none.
// FUNCTION: STARFLEETOP 0x00448a19
int tShipSystemsExt::vfunction83()
{
    char* p = *(char**)( m584 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x5c );
    return 0;
}

// HYPOTHESIS: returns the first dword of the block at m584, the pointer vfunction71..75 read
// through, as an integer.
// FUNCTION: STARFLEETOP 0x00448a2a
int tShipSystemsExt::vfunction84()
{
    return *(int*)m584;
}

// HYPOTHESIS: returns the pointer at +0x14 of the block at m584 (+0x598), the one
// vfunction68..70 read through, as an integer.
// FUNCTION: STARFLEETOP 0x00448a31
int tShipSystemsExt::vfunction85()
{
    return *(int*)( m584 + 0x14 );
}

// HYPOTHESIS: returns the pointer at +0x20 of the block at m584 (+0x5a4), the one
// vfunction73..75 read through, as an integer.
// FUNCTION: STARFLEETOP 0x00448a38
int tShipSystemsExt::vfunction86()
{
    return *(int*)( m584 + 0x20 );
}

// HYPOTHESIS: returns the dword at +0x30 of the block at m584 (+0x5b4) as an integer.
// FUNCTION: STARFLEETOP 0x00448a3f
int tShipSystemsExt::vfunction87()
{
    return *(int*)( m584 + 0x30 );
}

// HYPOTHESIS: returns the dword at +0x34 of the block at m584 (+0x5b8) as an integer.
// FUNCTION: STARFLEETOP 0x00448a46
int tShipSystemsExt::vfunction88()
{
    return *(int*)( m584 + 0x34 );
}

// HYPOTHESIS: returns the dword at +0x40 of the block at m584 (+0x5c4) as an integer.
// FUNCTION: STARFLEETOP 0x00448a4d
int tShipSystemsExt::vfunction89()
{
    return *(int*)( m584 + 0x40 );
}

// FUNCTION: STARFLEETOP 0x00448a54
int tShipSystemsExt::vfunction76()
{
    return *(int*)( m584 + 0x38 );
}

// HYPOTHESIS: returns the pointer at +0x48 of the block at m584 (+0x5cc), the one
// vfunction59..63 read through, as an integer.
// FUNCTION: STARFLEETOP 0x00448a5b
int tShipSystemsExt::vfunction90()
{
    return *(int*)( m584 + 0x48 );
}

// HYPOTHESIS: returns the dword at +0x60 of the block at m584 (+0x5e4) as an integer.
// FUNCTION: STARFLEETOP 0x00448a62
int tShipSystemsExt::vfunction91()
{
    return *(int*)( m584 + 0x60 );
}

// HYPOTHESIS: returns the dword at +0x64 of the block at m584 (+0x5e8) as an integer.
// FUNCTION: STARFLEETOP 0x00448a69
int tShipSystemsExt::vfunction92()
{
    return *(int*)( m584 + 0x64 );
}

// HYPOTHESIS: returns the dword at +0x6c of the block at m584 (+0x5f0) as an integer.
// FUNCTION: STARFLEETOP 0x00448a70
int tShipSystemsExt::vfunction93()
{
    return *(int*)( m584 + 0x6c );
}

// HYPOTHESIS: returns the dword at +0x8c of the block at m584 (+0x610) as an integer.
// FUNCTION: STARFLEETOP 0x00448a77
int tShipSystemsExt::vfunction95()
{
    return *(int*)( m584 + 0x8c );
}

// HYPOTHESIS: returns the dword at +0x90 of the block at m584 (+0x614) as an integer.
// FUNCTION: STARFLEETOP 0x00448a7e
int tShipSystemsExt::vfunction96()
{
    return *(int*)( m584 + 0x90 );
}

// HYPOTHESIS: returns the pointer at +0x94 of the block at m584 (+0x618), the one
// vfunction79..80 read through.
// FUNCTION: STARFLEETOP 0x00448a85
int tShipSystemsExt::vfunction97()
{
    return *(int*)( m584 + 0x94 );
}

// HYPOTHESIS: returns the pointer at +0xa0 of the block at m584 (+0x624), the one
// vfunction85..91 read through.
// FUNCTION: STARFLEETOP 0x00448a8c
int tShipSystemsExt::vfunction98()
{
    return *(int*)( m584 + 0xa0 );
}

// HYPOTHESIS: returns the dword at +0xc0 of the block at m584 (+0x644) as an integer.
// FUNCTION: STARFLEETOP 0x00448a93
int tShipSystemsExt::vfunction99()
{
    return *(int*)( m584 + 0xc0 );
}
