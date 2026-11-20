//	c_004280f1.h: the stores classes whose members unsorted/fn_00546000.cpp declares, for the
//	units outside it that call them.
// HYPOTHESIS: these three declarations repeat the ones that unsorted/fn_00546000.cpp still
// holds in its own body (the missile info and item rates are also declared inside
// Source/states/SupplyDockItem.cpp); that unit is to include this header in their place. The
// members 0x00569c83 needs are added: 0x0045147a returns the flag it tests, and 0x0045193a.

#ifndef c_004280f1_h
#define c_004280f1_h 1

#include <string>

#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: a 0x10-byte member of c_0045139e at +0x48; 0x0045275f destroys it.
class c_0045275f
{
public:
    ~c_0045275f();

    char field_0[0x10];
};

// HYPOTHESIS: tStoresState::tItemRates (RTTI); the object 0x0054606f takes by value (0x58
// bytes), and the one the request's rate table holds for each ship (0x00569c83).
class c_0045139e : public nDataStore::tStreamable
{
public:
    virtual void StreamOut(nDataStore::tOutput& output) const;
    virtual void StreamIn(nDataStore::tInput& input);

    double fn_0045139e(long arg1);
    double fn_004513a9(long arg1);
    // HYPOTHESIS: looks a store up by name and writes its size through the pointer; 0x00569c83
    // tests the result.
    bool fn_0045147a(const std::string& name, long* value);

    char field_c[0x3c];
    c_0045275f field_48;
};

// HYPOTHESIS: tStoresState::tMissileInfo (RTTI); 0x004280f1 copies one and it is destroyed as
// a tStreamable.
class c_004280f1 : public nDataStore::tStreamable
{
public:
    c_004280f1(const c_004280f1& other);
    virtual void StreamOut(nDataStore::tOutput& output) const;
    virtual void StreamIn(nDataStore::tInput& input);

    // HYPOTHESIS: 0x0045154e copies another missile info into this one.
    void fn_0045154e(c_004280f1* info);
    long fn_004517e1();
    long fn_00451a8b();
    void fn_0045189f(long count);
    // HYPOTHESIS: sets the missile count and returns the count it kept (0x00569c83).
    long fn_0045193a(long count);
    short fn_00451c45();
    double fn_00451aa6(c_0045139e* rates);

    char field_c[0xd8];
};

// HYPOTHESIS: the object behind 0x00546296 keeps a std::string at +0x6c, returned by copy
// (the allocator byte, the string's reset 0x005da20a and its assign 0x00403b49); its class
// is not known.
class c_00546296
{
public:
    // 0x00546296, the getter of this string, is tShip's.
    // HYPOTHESIS: 0x00546799 assigns the string at +0x6c; that it belongs to this class is a guess.
    void fn_00546799(const std::string& param_1);
    void fn_005467b0(const std::string& param_1);

    char field_0[0x6c];
    std::string field_6c;
    int field_7c;
    // HYPOTHESIS: 0x005467b0 assigns a second std::string here, likely the member whose
    // address 0x005462d0 returns.
    std::string field_80;
};

#endif
