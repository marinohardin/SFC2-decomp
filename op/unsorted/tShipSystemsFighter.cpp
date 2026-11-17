// Unit:      unsorted/tShipSystemsFighter.cpp
// Functions of tShipSystemsFighter (0x00444004 to 0x00444723); original file unknown.
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tShipSystems.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tIndividualShipSystemList.h"
#include "Tiger/tcglue.h"

extern float g_00799ed0;
extern double g_0090bef0;
extern float g_007959cc;

// HYPOTHESIS: the slots before the destructor are not recovered; 0x00444004 calls the
// deleting destructor through slot 169 (+0x2a4), so it is declared after 168 others.
class tShipSystemsFighter : public tShipSystems
{
public:
    virtual void vfunction0();
    virtual int vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual int vfunction11( int param_1, int param_2, int param_3 );
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual void vfunction17();
    virtual void vfunction18();
    virtual void vfunction19();
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    virtual void vfunction23();
    virtual void vfunction24();
    virtual void vfunction25();
    virtual void vfunction26( float param_1 );
    virtual tMinMaxFloat* vfunction27();
    virtual tMinMaxFloat* vfunction28();
    virtual tMinMaxFloat* vfunction29();
    virtual tMinMaxFloat* vfunction30();
    virtual tMinMaxFloat* vfunction31();
    virtual void vfunction32();
    virtual void vfunction33();
    virtual void vfunction34();
    virtual tMinMaxFloat* vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual void vfunction41();
    virtual tValue<float>* vfunction42();
    virtual void vfunction43();
    virtual void vfunction44();
    virtual void vfunction45();
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual float vfunction51( int param_1 );
    virtual void vfunction52();
    virtual void vfunction53();
    virtual void vfunction54();
    virtual void vfunction55();
    virtual void vfunction56();
    virtual char* vfunction57();
    virtual char* vfunction58();
    virtual int vfunction59();
    virtual void vfunction60();
    virtual void vfunction61();
    virtual void vfunction62();
    virtual void vfunction63();
    virtual void vfunction64();
    virtual void vfunction65();
    virtual void vfunction66();
    virtual void vfunction67();
    virtual void vfunction68();
    virtual void vfunction69();
    virtual void vfunction70();
    virtual void vfunction71();
    virtual void vfunction72();
    virtual void vfunction73();
    virtual void vfunction74();
    virtual void vfunction75();
    virtual void vfunction76();
    virtual void vfunction77();
    virtual void vfunction78();
    virtual void vfunction79();
    virtual void vfunction80();
    virtual void vfunction81();
    virtual void vfunction82();
    virtual void vfunction83();
    virtual void vfunction84();
    virtual void vfunction85();
    virtual void vfunction86();
    virtual void vfunction87();
    virtual void vfunction88();
    virtual void vfunction89();
    virtual void vfunction90();
    virtual void vfunction91();
    virtual void vfunction92();
    virtual void vfunction93();
    virtual void vfunction94();
    virtual void vfunction95();
    virtual void vfunction96();
    virtual void vfunction97();
    virtual void vfunction98();
    virtual void vfunction99();
    virtual void vfunction100();
    virtual void vfunction101();
    virtual void vfunction102();
    virtual void vfunction103();
    virtual void vfunction104();
    virtual void vfunction105();
    virtual void vfunction106();
    virtual void vfunction107();
    virtual void vfunction108();
    virtual void vfunction109();
    virtual void vfunction110();
    virtual void vfunction111();
    virtual void vfunction112();
    virtual void vfunction113();
    virtual void vfunction114();
    virtual void vfunction115();
    virtual void vfunction116();
    virtual void vfunction117();
    virtual void vfunction118();
    virtual void vfunction119();
    virtual void vfunction120();
    virtual void vfunction121();
    virtual void vfunction122();
    virtual void vfunction123();
    virtual void vfunction124();
    virtual void vfunction125();
    virtual void vfunction126();
    virtual void vfunction127();
    virtual void vfunction128();
    virtual void vfunction129();
    virtual void vfunction130();
    virtual void vfunction131();
    virtual void vfunction132();
    virtual void vfunction133();
    virtual void vfunction134();
    virtual void vfunction135();
    virtual void vfunction136();
    virtual void vfunction137();
    virtual void vfunction138();
    virtual void vfunction139();
    virtual void vfunction140();
    virtual void vfunction141();
    virtual void vfunction142();
    virtual void vfunction143();
    virtual void vfunction144();
    virtual void vfunction145();
    virtual void vfunction146();
    virtual void vfunction147();
    virtual void vfunction148();
    virtual void vfunction149();
    virtual void vfunction150();
    virtual void vfunction151();
    virtual void vfunction152();
    virtual void vfunction153();
    virtual void vfunction154();
    virtual void vfunction155();
    virtual void vfunction156();
    virtual void vfunction157();
    virtual void vfunction158();
    virtual void vfunction159();
    virtual void vfunction160();
    virtual void vfunction161();
    virtual void vfunction162();
    virtual void vfunction163();
    virtual void vfunction164();
    virtual void vfunction165();
    virtual void vfunction166();
    virtual void vfunction167();
    virtual void vfunction168();
    tShipSystemsFighter( int param_1 );
    virtual ~tShipSystemsFighter();

    // The layout is read from the members the constructor (0x00444046) builds.
    int m04;
    int m08;
    int m0c;
    int m10;
    int m14;
    tMinMaxFloat m18;
    float m3c;
    float m40;
    tIndividualShipSystemList m44;
    char m5c4[0x128];
    int m6ec;
    tMinMaxFloat m6f0;
    tValue<float> m714;
    tMinMaxFloat m730;
    tMinMaxFloat m754;
    tMinMaxFloat m778;
    tMinMaxFloat m79c;
    tMinMaxFloat m7c0;
    tMinMaxFloat m7e4;
    int m808;
    int m80c;
    int m810;
    int m814;
    double m818;
    int m820;
    char m824;
    int m828;
    short m82c;
    double m830;
};

// HYPOTHESIS: the return type; the original only clears eax after deleting the object.
// FUNCTION: STARFLEETOP 0x00444004
int tShipSystemsFighter::vfunction1()
{
    static tObject_0040ce21 marker("Eeeekkk! The Figher Goes Down!");
    delete this;
    return 0;
}

// FUNCTION: STARFLEETOP 0x00444046
tShipSystemsFighter::tShipSystemsFighter( int param_1 )
{
    // HYPOTHESIS: the argument; it is stored at +0x6ec.
    m6ec = param_1;
    clib::memset( m5c4, 0, sizeof( m5c4 ) );
    m820 = 0;
    m824 = 0;
    m80c = 8;
    m810 = 8;

    tMinMaxFloat* value = &m730;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m754;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m778;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m79c;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m7c0;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m7e4;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m18;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    value = &m6f0;
    value->vfunction5( 0.0f );
    value->vfunction4( g_00799ed0 );
    value->vfunction3( 0.0f );
    tValue<float>* single = &m714;
    // HYPOTHESIS: the exe passes a float 0 here; the shared header types the slot's argument
    // as an integer.
    single->vfunction2( 0 );

    m04 = 0;
    m08 = 0;
    m0c = 0;
    m3c = 0.0f;
    m10 = 0;
    m14 = 0;
    m40 = 0.0f;
    m828 = -1;
    m82c = 1;
    m818 = 0.0;
    m830 = g_0090bef0;
}

// HYPOTHESIS: the types; the original clears eax and pops three dword arguments.
// FUNCTION: STARFLEETOP 0x00444391
int tShipSystemsFighter::vfunction11( int, int, int )
{
    return 0;
}

// HYPOTHESIS: the exe pushes a float to slot 2 of m754, a tMinMaxFloat; the shared header
// types that slot as an integer (0x0043e642 passes an int), so this view gives it a float.
class fn_00444396_value
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3( float param_1 ); // slot 2
};

// HYPOTHESIS: the parameter is a float; it is loaded with fld and forwarded.
// FUNCTION: STARFLEETOP 0x00444396
void tShipSystemsFighter::vfunction26( float param_1 )
{
    ((fn_00444396_value*)&m754)->vfunction3( param_1 );
}

// HYPOTHESIS: the return type; the original returns the address of m754.
// FUNCTION: STARFLEETOP 0x004443b0
tMinMaxFloat* tShipSystemsFighter::vfunction27()
{
    return &m754;
}

// HYPOTHESIS: the return type; the original returns the address of m730.
// FUNCTION: STARFLEETOP 0x004443b7
tMinMaxFloat* tShipSystemsFighter::vfunction28()
{
    return &m730;
}

// HYPOTHESIS: the return type; the original returns the address of m778.
// FUNCTION: STARFLEETOP 0x004443be
tMinMaxFloat* tShipSystemsFighter::vfunction29()
{
    return &m778;
}

// HYPOTHESIS: the return type; the original returns the address of m79c.
// FUNCTION: STARFLEETOP 0x004443c5
tMinMaxFloat* tShipSystemsFighter::vfunction30()
{
    return &m79c;
}

// HYPOTHESIS: the return type; the original returns the address of m7c0.
// FUNCTION: STARFLEETOP 0x004443cc
tMinMaxFloat* tShipSystemsFighter::vfunction35()
{
    return &m7c0;
}

// HYPOTHESIS: the return type; the original returns the address of m6f0.
// FUNCTION: STARFLEETOP 0x004443d3
tMinMaxFloat* tShipSystemsFighter::vfunction31()
{
    return &m6f0;
}

// HYPOTHESIS: the return type; the original returns the address of m714.
// FUNCTION: STARFLEETOP 0x004443da
tValue<float>* tShipSystemsFighter::vfunction42()
{
    return &m714;
}

// HYPOTHESIS: the types; the original loads the float at 0x007959cc and pops one dword
// argument it does not read.
// FUNCTION: STARFLEETOP 0x004443e1
float tShipSystemsFighter::vfunction51( int )
{
    return g_007959cc;
}

// HYPOTHESIS: the return type; the original returns the address of m5c4.
// FUNCTION: STARFLEETOP 0x004443ea
char* tShipSystemsFighter::vfunction57()
{
    return m5c4;
}

// HYPOTHESIS: the return type; the original returns the address +0x688, 0xc4 bytes into the
// block at m5c4 that the constructor clears as one, so m5c4 is likely a structure with a
// member there.
// FUNCTION: STARFLEETOP 0x004443f1
char* tShipSystemsFighter::vfunction58()
{
    return m5c4 + 0xc4;
}

// HYPOTHESIS: the block at m5c4 holds a pointer at +0x48 (+0x60c) to an object whose +0x58
// is returned, or 0 when there is none; neither type is recovered.
// FUNCTION: STARFLEETOP 0x004443f8
int tShipSystemsFighter::vfunction59()
{
    char* p = *(char**)( m5c4 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x58 );
    return 0;
}
