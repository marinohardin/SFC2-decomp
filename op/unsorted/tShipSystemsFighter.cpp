// Unit:      unsorted/tShipSystemsFighter.cpp
// Functions of tShipSystemsFighter (0x00444004 to 0x00444723); original file unknown.
#include "unsorted/tObject_0040ce21.h"
#include "unsorted/tShipSystems.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tIndividualShipSystemList.h"
#include "Tiger/tcglue.h"
#include <string>

extern float g_00799ed0;
extern double g_0090bef0;
extern float g_007959cc;
extern int g_008e0cd0;
extern double g_00c7c148;

// HYPOTHESIS: the slots before the destructor are not recovered; 0x00444004 calls the
// deleting destructor through slot 169 (+0x2a4), so it is declared after 168 others.
class tShipSystemsFighter : public tShipSystems
{
public:
    virtual int vfunction1();
    virtual float vfunction2();
    virtual float vfunction3();
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
    virtual float vfunction22( float param_1 );
    virtual float vfunction23( float param_1 );
    virtual void vfunction24();
    virtual void vfunction25();
    virtual void vfunction26( float param_1 );
    virtual tMinMaxFloat* vfunction27();
    virtual tMinMaxFloat* vfunction28();
    virtual tMinMaxFloat* vfunction29();
    virtual tMinMaxFloat* vfunction30();
    virtual tMinMaxFloat* vfunction31();
    virtual tMinMaxFloat* vfunction32();
    virtual tMinMaxFloat* vfunction33();
    virtual tMinMaxFloat* vfunction34();
    virtual tMinMaxFloat* vfunction35();
    virtual tMinMaxFloat* vfunction36();
    virtual tMinMaxFloat* vfunction37();
    virtual tMinMaxFloat* vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual tMinMaxFloat* vfunction41();
    virtual tValue<float>* vfunction42();
    virtual bool vfunction43();
    virtual bool vfunction44();
    virtual void vfunction45( bool param_1 );
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual float vfunction51( int param_1 );
    virtual void vfunction52();
    virtual tShipSystemManager* vfunction53();
    virtual tTacticalStoresManager* vfunction54();
    virtual void vfunction55();
    virtual void vfunction56();
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
    virtual void vfunction94();
    virtual int vfunction95();
    virtual int vfunction96();
    virtual int vfunction97();
    virtual int vfunction98();
    virtual int vfunction99();
    virtual int vfunction100();
    virtual int vfunction101();
    virtual int vfunction102();
    virtual int vfunction103();
    virtual int vfunction104();
    virtual int vfunction105();
    virtual int vfunction106();
    virtual void vfunction107( float param_1 );
    virtual int vfunction108();
    virtual float vfunction109( float param_1, int param_2 );
    virtual float vfunction110( int param_1, int param_2 );
    virtual void vfunction111( int param_1 );
    virtual void vfunction112();
    virtual float vfunction113();
    virtual void vfunction114( float param_1 );
    virtual void vfunction115( float param_1 );
    virtual void vfunction116( float param_1 );
    virtual void vfunction117( float param_1 );
    virtual float vfunction118();
    virtual void vfunction119();
    virtual float vfunction120();
    virtual void vfunction121();
    virtual void vfunction122();
    virtual tMinMaxFloat* vfunction123();
    virtual float vfunction124();
    virtual float vfunction125();
    virtual tMinMaxFloat* vfunction126();
    virtual void vfunction127( float param_1 );
    virtual void vfunction128();
    virtual void vfunction129();
    virtual void vfunction130();
    virtual tMinMaxFloat* vfunction131();
    virtual float vfunction132();
    virtual void vfunction133();
    virtual float vfunction134();
    virtual void vfunction135();
    virtual void vfunction136();
    virtual void vfunction137();
    virtual float vfunction138();
    virtual int vfunction139();
    virtual int vfunction140();
    virtual void vfunction141();
    virtual void vfunction142();
    virtual void vfunction143();
    virtual void vfunction144();
    virtual void vfunction145();
    virtual int vfunction146(int, int);
    virtual tIndividualShipSystemList* vfunction147();
    virtual int vfunction148( int param_1 );
    virtual int vfunction149( int param_1 );
    virtual void vfunction150();
    virtual void vfunction151();
    virtual int* vfunction152( int* param_1 );
    virtual void vfunction153();
    virtual void vfunction154( int param_1 );
    virtual void vfunction155();
    virtual void vfunction156();
    virtual int* vfunction157( int );
    virtual void vfunction158();
    virtual char vfunction159(int);
    virtual void vfunction160();
    virtual void vfunction161();
    virtual void vfunction162();
    virtual void vfunction163();
    virtual void vfunction164();
    virtual int vfunction165();
    virtual void vfunction166( int param_1 );
    virtual void vfunction167();
    virtual void vfunction168();
    virtual void vfunction169();
    tShipSystemsFighter( int param_1 );
    virtual ~tShipSystemsFighter();
    virtual void vfunction170();
    virtual void vfunction171();
    virtual void vfunction172( int param_1, std::string param_2 );

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

// FUNCTION: STARFLEETOP 0x0044453c
// FUNCTION: STARFLEETOP 0x00444558
tShipSystemsFighter::~tShipSystemsFighter()
{
    tIndividualShipSystemList* list = &m44;
    list->vfunction24();
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

// HYPOTHESIS: the same object as vfunction59 (pointer at m5c4 + 0x48); its +0x5c is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x00444409
int tShipSystemsFighter::vfunction60()
{
    char* p = *(char**)( m5c4 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x5c );
    return 0;
}

// HYPOTHESIS: the same object as vfunction59 (pointer at m5c4 + 0x48); its +0x60 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x0044441a
int tShipSystemsFighter::vfunction61()
{
    char* p = *(char**)( m5c4 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x60 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction59 (pointer at m5c4 + 0x48); its +0x64 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x0044442b
int tShipSystemsFighter::vfunction62()
{
    char* p = *(char**)( m5c4 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x64 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction59 (pointer at m5c4 + 0x48); its +0x68 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x0044443c
int tShipSystemsFighter::vfunction63()
{
    char* p = *(char**)( m5c4 + 0x48 );
    if( p != 0 )
        return *(int*)( p + 0x68 );
    return 0;
}

// HYPOTHESIS: the object whose pointer sits at m5c4 + 0x48 (+0x60c); vfunction2 calls its
// slot 84 (+0x150) for a float. Neither the type nor its other slots are recovered.
class fn_0044444d_field60c_type
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
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
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    virtual void vfunction33();
    virtual void vfunction34();
    virtual void vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual void vfunction41();
    virtual void vfunction42();
    virtual void vfunction43();
    virtual void vfunction44();
    virtual void vfunction45();
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual void vfunction51();
    virtual void vfunction52();
    virtual void vfunction53();
    virtual void vfunction54();
    virtual void vfunction55();
    virtual void vfunction56();
    virtual void vfunction57();
    virtual void vfunction58();
    virtual void vfunction59();
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
    virtual float vfunction84();
    virtual void vfunction85();
    virtual float vfunction86();
};

// HYPOTHESIS: the return type; the original returns the float from slot 84 of the object at
// m5c4 + 0x48, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x0044444d
float tShipSystemsFighter::vfunction2()
{
    fn_0044444d_field60c_type* p = *(fn_0044444d_field60c_type**)( m5c4 + 0x48 );
    if( p != 0 )
        return p->vfunction84();
    return 0.0f;
}

// HYPOTHESIS: the return type; the same object as vfunction2, whose slot 86 (+0x158) is taken
// to return a float.
// FUNCTION: STARFLEETOP 0x00444466
float tShipSystemsFighter::vfunction3()
{
    fn_0044444d_field60c_type* p = *(fn_0044444d_field60c_type**)( m5c4 + 0x48 );
    if( p != 0 )
        return p->vfunction86();
    return 0.0f;
}

// HYPOTHESIS: the block at m5c4 holds another pointer at +0xa0 (+0x664) to an object whose
// +0x58 is returned, or 0 when there is none; neither type is recovered.
// FUNCTION: STARFLEETOP 0x0044447f
int tShipSystemsFighter::vfunction77()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x58 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction77 (pointer at m5c4 + 0xa0); its +0x60 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x00444490
int tShipSystemsFighter::vfunction78()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x60 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction77 (pointer at m5c4 + 0xa0); its +0x64 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004444a1
int tShipSystemsFighter::vfunction79()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x64 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction77 (pointer at m5c4 + 0xa0); its +0x68 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004444b2
int tShipSystemsFighter::vfunction80()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x68 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction77 (pointer at m5c4 + 0xa0); its +0x6c is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004444c3
int tShipSystemsFighter::vfunction81()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x6c );
    return 0;
}

// HYPOTHESIS: the same object as vfunction77 (pointer at m5c4 + 0xa0); its +0x70 is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004444d4
int tShipSystemsFighter::vfunction82()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x70 );
    return 0;
}

// HYPOTHESIS: the same object as vfunction77 (pointer at m5c4 + 0xa0); its +0x5c is
// returned, or 0 when there is none.
// FUNCTION: STARFLEETOP 0x004444e5
int tShipSystemsFighter::vfunction83()
{
    char* p = *(char**)( m5c4 + 0xa0 );
    if( p != 0 )
        return *(int*)( p + 0x5c );
    return 0;
}

// HYPOTHESIS: the return type; the original returns the dword at +0x5d8, 0x14 bytes into
// the block at m5c4.
// FUNCTION: STARFLEETOP 0x004444f6
int tShipSystemsFighter::vfunction85()
{
    return *(int*)( m5c4 + 0x14 );
}

// HYPOTHESIS: the return type; the original returns the dword at +0x5f4, 0x30 bytes into
// the block at m5c4.
// FUNCTION: STARFLEETOP 0x004444fd
int tShipSystemsFighter::vfunction87()
{
    return *(int*)( m5c4 + 0x30 );
}

// HYPOTHESIS: the return type; the original returns the constructor's argument (+0x6ec).
// FUNCTION: STARFLEETOP 0x00444504
int tShipSystemsFighter::vfunction108()
{
    return m6ec;
}

// FUNCTION: STARFLEETOP 0x0044450b
tMinMaxFloat* tShipSystemsFighter::vfunction123()
{
    return &m18;
}

// HYPOTHESIS: the return and parameter types; the original returns -1 and pops two
// unused dword arguments.
// FUNCTION: STARFLEETOP 0x0044450f
int tShipSystemsFighter::vfunction146(int, int)
{
    return -1;
}

// FUNCTION: STARFLEETOP 0x00444515
tIndividualShipSystemList* tShipSystemsFighter::vfunction147()
{
    return &m44;
}

// HYPOTHESIS: the return and parameter types; the original stores 3 through the argument
// and returns the address of the first member.
// FUNCTION: STARFLEETOP 0x00444519
int* tShipSystemsFighter::vfunction152( int* param_1 )
{
    *param_1 = 3;
    return &m04;
}

// HYPOTHESIS: the return and parameter types; the original returns 5 in al and pops one
// unused dword argument.
// FUNCTION: STARFLEETOP 0x00444529
char tShipSystemsFighter::vfunction159(int)
{
    return 5;
}

// HYPOTHESIS: the types; the original pops five dwords and only destroys the by-value string
// that follows the first argument (std::string::_Tidy(true) on the stack copy).
// FUNCTION: STARFLEETOP 0x0044452e
void tShipSystemsFighter::vfunction172( int, std::string )
{
}

// HYPOTHESIS: the types; the original returns the address of the global at 0x008e0cd0 and
// pops one unused dword argument.
// FUNCTION: STARFLEETOP 0x00444638
int* tShipSystemsFighter::vfunction157( int )
{
    return &g_008e0cd0;
}

// HYPOTHESIS: the return type; the original returns the dword at +0x828.
// FUNCTION: STARFLEETOP 0x00444640
int tShipSystemsFighter::vfunction165()
{
    return m828;
}

// FUNCTION: STARFLEETOP 0x00444647
void tShipSystemsFighter::vfunction166( int param_1 )
{
    m828 = param_1;
}

// HYPOTHESIS: the types; the request is clamped to the whole part of vfunction3's value, plus
// one when vfunction2's whole part is higher, then handed to m7e4 and its value returned.
// FUNCTION: STARFLEETOP 0x00444654
float tShipSystemsFighter::vfunction22( float param_1 )
{
    static tObject_0040ce21 marker("I want it this much! Fighter Style");
    float level = (float)(int)vfunction3();
    if( (float)(int)vfunction2() > level )
        level++;
    if( param_1 > level )
        param_1 = level;
    ((fn_00444396_value*)&m7e4)->vfunction3( param_1 );
    if( (float)m7e4 == (float)m778 &&(float)m778 != param_1 )
        m818 = g_00c7c148;
    return m7e4.m18;
}

// HYPOTHESIS: the object stored at +0x6ec by the constructor; vfunction23 calls its slot 54
// (+0xd8) for a float. Neither the type nor its other slots are recovered.
class fn_00444723_field6ec_type
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
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
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    virtual void vfunction33();
    virtual void vfunction34();
    virtual void vfunction35();
    virtual void vfunction36();
    virtual void vfunction37();
    virtual void vfunction38();
    virtual void vfunction39();
    virtual void vfunction40();
    virtual void vfunction41();
    virtual void vfunction42();
    virtual void vfunction43();
    virtual void vfunction44();
    virtual void vfunction45();
    virtual void vfunction46();
    virtual void vfunction47();
    virtual void vfunction48();
    virtual void vfunction49();
    virtual void vfunction50();
    virtual void vfunction51();
    virtual void vfunction52();
    virtual void vfunction53();
    virtual float vfunction54();
};

// HYPOTHESIS: the types; the request is capped at 31 (0x00799ed0), handed to m730, m730 is
// set again to its own value, and the capped request scaled by slot 54 of the object at
// +0x6ec (1 when that is 0) is passed on to vfunction22.
// FUNCTION: STARFLEETOP 0x00444723
float tShipSystemsFighter::vfunction23( float param_1 )
{
    static tObject_0040ce21 marker("I want to go this fast Fighter Style!");
    if( param_1 > g_00799ed0 )
        param_1 = g_00799ed0;
    ((fn_00444396_value*)&m730)->vfunction3( param_1 );
    ((fn_00444396_value*)&m730)->vfunction3( (float)m730 );
    float scale = ((fn_00444723_field6ec_type*)m6ec)->vfunction54();
    if( scale == 0.0f )
        scale = 1;
    vfunction22( scale * param_1 );
    return m730.m18;
}
