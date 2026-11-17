// Unit:      unsorted/fn_00546000.cpp
// Functions with no unit or class evidence on the 4 KB page 0x00546000; original file unknown.

#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: a 0x10-byte member of c_0045139e at +0x48; 0x0045275f destroys it.
class c_0045275f
{
public:
    ~c_0045275f();

    char field_0[0x10];
};

// HYPOTHESIS: the object 0x0054606f takes by value (0x58 bytes); its implicit destructor
// destroys the member at +0x48 and then the tStreamable base. It is the same object
// Source/states/SupplyDockItem.cpp declares as fn_0045139e_object; that declaration lives
// only in that .cpp, so this unit names its own.
class c_0045139e : public nDataStore::tStreamable
{
public:
    virtual void StreamOut(nDataStore::tOutput& output) const;
    virtual void StreamIn(nDataStore::tInput& input);

    double fn_0045139e(long arg1);
    double fn_004513a9(long arg1);
    void fn_0045147a(const std::string& name, long* value);

    char field_c[0x3c];
    c_0045275f field_48;
};

// HYPOTHESIS: tStoresState::tMissileInfo (declared only in Source/states/SupplyDockItem.cpp);
// 0x004280f1 copies one and it is destroyed as a tStreamable.
class c_004280f1 : public nDataStore::tStreamable
{
public:
    c_004280f1(const c_004280f1& other);
    virtual void StreamOut(nDataStore::tOutput& output) const;
    virtual void StreamIn(nDataStore::tInput& input);

    long fn_004517e1();
    long fn_00451a8b();
    double fn_00451aa6(c_0045139e* rates);

    char field_c[0xd8];
};

// HYPOTHESIS: tStoresState (declared only in Source/states/SupplyDockItem.cpp); 0x00427db2
// copies one, 0x00451f47 destroys it; 0x20c bytes.
class c_00427db2
{
public:
    c_00427db2(const c_00427db2& other);
    ~c_00427db2();
    c_004280f1* fn_0055e60f();

    char field_0[0xf0];
    char field_f0;
    char field_f1;
    char field_f2;
    char field_f3[0x1c];
    char field_10f[3];
    char field_112[3];
    char field_115[0x69];
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
};

// HYPOTHESIS: the class that owns 0x00546027; only the member at +0x90 is known.
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
    const c_00427db2& fn_005462e5();
    // HYPOTHESIS: 0x00410223 is called on the same object; it may belong to a base class.
    int fn_00410223();

    char field_0[0x90];
    c_00451045 field_90;
};

// FUNCTION: STARFLEETOP 0x00546027
double c_00546027::fn_00546027(int param_1, int param_2)
{
    return field_90.fn_00451045(param_1, param_2);
}

// FUNCTION: STARFLEETOP 0x0054603d
int c_00546027::fn_0054603d(double param_1)
{
    return fn_00546027(0, 0) * fn_00410223() * param_1;
}

// FUNCTION: STARFLEETOP 0x0054606f
int c_00546027::fn_0054606f(double param_1, double param_2, c_0045139e param_3)
{
    c_00427db2 stores(fn_005462e5());
    c_004280f1 info(*stores.fn_0055e60f());

    int local_20 = fn_00410223() * param_1;
    int local_24 = fn_0054603d(param_2);

    int diff = stores.field_f2 - stores.field_f0;
    int local_28 = param_3.fn_0045139e(3) * (diff > 0 ? 0 : diff);

    int spare;
    if (info.fn_00451a8b() - info.fn_004517e1() * 2 > 0)
        spare = 0;
    else
        spare = info.fn_00451a8b() - info.fn_004517e1() * 2;
    int local_1c = info.fn_00451aa6(&param_3) * spare;

    int sum = 0;
    for (int i = 0; i < 4; i++) {
        long have = 0;
        long need = 0;
        param_3.fn_0045147a(stores.field_18c[i], &have);
        param_3.fn_0045147a(stores.field_1cc[i], &need);
        int d = stores.field_182[i] * have - stores.field_17e[i] * need;
        if (d > 0)
            d = 0;
        sum += d;
    }

    int total = 0;
    for (int j = 0; j < 3; j++) {
        int d = stores.field_112[j] - stores.field_10f[j];
        total += param_3.fn_004513a9(j) * (d > 0 ? 0 : d);
    }

    return total - local_24 + sum + local_1c + local_28 + local_20;
}

// FUNCTION: STARFLEETOP 0x00546264
unsigned int c_00546027::fn_00546264()
{
    return fn_005462d7() & 1;
}

// HYPOTHESIS: the object behind 0x0054626d keeps a flag word at +0x430; its class is not known.
class c_0054626d
{
public:
    bool fn_0054626d();

    char field_0[0x430];
    struct tFlags430
    {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char pad[3];
    } field_430;
};

// FUNCTION: STARFLEETOP 0x0054626d
bool c_0054626d::fn_0054626d()
{
    // MATCH-ONLY: the whole-dword copy gives the original's dword load before the byte-wide shift and mask.
    unsigned int word = *(unsigned int*)&field_430;
    return ((tFlags430*)&word)->b1;
}
