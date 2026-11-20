//	c_00546027.h: the object whose members unsorted/fn_00546000.cpp rebuilds, shared with the
//	units that call them.
// Evidence:  calls (original names and file unknown)

#ifndef c_00546027_h
#define c_00546027_h 1

#include <string>

#include "unsorted/c_004526cb.h"

class c_0045139e;
class c_004280f1;

// HYPOTHESIS: tStoresState (declared only in Source/states/SupplyDockItem.cpp); 0x00427db2
// copies one, 0x00451f47 destroys it; 0x20c bytes.
class c_00427db2
{
public:
    c_00427db2(const c_00427db2& other);
    ~c_00427db2();
    c_004280f1* fn_0055e60f();
    // HYPOTHESIS: 0x004457b7 takes one pointer-sized argument; its type is unknown.
    void fn_004457b7(int param_1);

    char field_0[0xf0];
    char field_f0;
    char field_f1;
    char field_f2;
    char field_f3[0x19];
    char field_10c[3];
    char field_10f[3];
    char field_112[3];
    char field_115[0x65];
    char field_17a[4];
    char field_17e[4];
    char field_182[4];
    char field_186[6];
    std::string field_18c[4];
    std::string field_1cc[4];
};

// HYPOTHESIS: the class that owns 0x00451045; name and parameter types unknown.
class c_00451045
{
public:
    double fn_00451045(int param_1, int param_2);
    // HYPOTHESIS: 0x00450e41 is called on the member at +0x90; its return and parameter
    // types are unknown.
    void fn_00450e41(int param_1);
};

// HYPOTHESIS: the class that owns 0x00546027; only the member at +0x90 is known.
// HYPOTHESIS: 0x00568b2e calls 0x00546027 with the addresses of two ints, so its parameters
// are probably pointers to results; 0x0054603d passes zero for both.
class c_00546027
{
public:
    double fn_00546027(int param_1, int param_2);
    int fn_0054603d(double param_1);
    int fn_0054606f(double param_1, double param_2, c_0045139e param_3);
    // HYPOTHESIS: 0x00546264 and 0x005462d7 sit next to 0x005462e5 and take this in ecx;
    // that they belong to this class is a guess.
    unsigned int fn_00546264();
    unsigned int fn_005462d7();
    // HYPOTHESIS: 0x005462de returns the address of +0x90, the member 0x00546027 calls through.
    c_00451045* fn_005462de();
    const c_00427db2& fn_005462e5();
    // HYPOTHESIS: 0x005462ec returns the address of +0x30c, the member after field_100;
    // that it belongs to this class is a guess.
    c_004526cb* fn_005462ec();
    void fn_00546325(int param_1);
    void fn_00546480(int param_1);
    void fn_00546492(c_00427db2& param_1);
    void fn_005465d5(float param_1, int param_2);
    void fn_00546787(int param_1);
    void fn_005467ca(unsigned int param_1);
    // HYPOTHESIS: 0x00410223 is called on the same object; it may belong to a base class.
    int fn_00410223();

    char field_0[0x90];
    c_00451045 field_90;
    char field_91[0x100 - 0x91];
    // HYPOTHESIS: the stores state 0x005462e5 returns, at +0x100.
    c_00427db2 field_100;
    c_004526cb field_30c;
    // HYPOTHESIS: the flag word at +0x430 that 0x00546264 tests bit 0 of.
    unsigned int field_430;
};

#endif
