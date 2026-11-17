//	tShipSystemsExt.h: tShipSystemsExt, a ship's full system set.
// Evidence:  class (RTTI: tShipSystemsExt : tShipSystems, vtable 0x0079d530; original file unknown)

#ifndef tShipSystemsExt_h
#define tShipSystemsExt_h 1

#include "unsorted/tShipSystems.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tIndividualShipSystemList.h"
#include "unsorted/tShipSystemManager.h"
#include "unsorted/tTacticalStoresManager.h"

// HYPOTHESIS: a 0x10-byte element type; the constructor builds four of them at +0x9c4 with
// the constructor at 0x00401fe6 and the destructor at 0x005f762d. Neither is recovered.
class c_00401fe6
{
public:
    c_00401fe6();
    ~c_00401fe6();

    long m00[4];
};

// HYPOTHESIS: the ship's shield group (tShieldGroup, whose slot 84 at 0x00440664 returns a
// float). Its shared header does not build against the current tIndividualShipSystem.h, so
// this view declares only the slots up to the one 0x00448baa calls.
class fn_00448baa_shields
{
public:
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
    virtual void vfunction84();
    virtual float vfunction85();
    virtual void vfunction86();
    virtual float vfunction87();
};

// RTTI: tShipSystemsExt : tShipSystems (vtable 0x0079d530, 177 slots).
// HYPOTHESIS: the overrides are not all declared yet; the layout is read from the members the
// constructor (0x00448235) builds and sets.
class tShipSystemsExt : public tShipSystems
{
public:
    tShipSystemsExt( unsigned long param_1 );
    virtual ~tShipSystemsExt();
    virtual float vfunction2();
    virtual float vfunction3();
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
    virtual tMinMaxFloat* vfunction41();
    virtual tValue<float>* vfunction42();
    virtual bool vfunction44();
    virtual void vfunction45( bool param_1 );
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
    virtual int vfunction100();
    virtual int vfunction101();
    virtual int vfunction102();
    virtual int vfunction103();
    virtual int vfunction104();
    virtual int vfunction105();
    virtual int vfunction106();
    virtual void vfunction107( float param_1 );
    virtual int vfunction108();
    virtual float vfunction113();
    virtual void vfunction114( float param_1 );
    virtual void vfunction115( float param_1 );
    virtual void vfunction116( float param_1 );
    virtual void vfunction117( float param_1 );
    virtual float vfunction118();
    virtual void vfunction119();
    virtual float vfunction120();
    virtual float vfunction124();
    virtual float vfunction125();
    virtual tMinMaxFloat* vfunction126();
    virtual void vfunction127( float param_1 );
    virtual void vfunction128( float param_1 );
    virtual void vfunction129( float param_1 );
    virtual void vfunction130( float param_1 );
    virtual tMinMaxFloat* vfunction131();
    virtual float vfunction132();
    virtual void vfunction133();
    virtual float vfunction134();
    virtual float vfunction138();
    virtual int vfunction139();
    virtual int vfunction140();
    virtual int vfunction148( int param_1 );
    virtual int vfunction149( int param_1 );
    virtual void vfunction153();
    virtual void vfunction154( int param_1 );
    // HYPOTHESIS: a slot this class adds after the destructor; it returns the member list m04.
    virtual tIndividualShipSystemList* vfunction171();

    tIndividualShipSystemList m04;
    char m584[0x48];
    fn_00448baa_shields* m5cc;
    char m5d0[0xdc];
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
    bool m941;
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

#endif	// tShipSystemsExt_h
