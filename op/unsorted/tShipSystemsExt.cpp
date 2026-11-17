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
