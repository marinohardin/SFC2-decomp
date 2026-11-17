// Unit:      unsorted/fn_00546000.cpp
// Functions with no unit or class evidence on the 4 KB page 0x00546000; original file unknown.

#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseID.h"

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
    // HYPOTHESIS: 0x00450e41 is called on the member at +0x90; its return and parameter
    // types are unknown.
    void fn_00450e41(int param_1);
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
    // HYPOTHESIS: 0x005462de returns the address of +0x90, the member 0x00546027 calls through.
    c_00451045* fn_005462de();
    const c_00427db2& fn_005462e5();
    // HYPOTHESIS: 0x005462ec returns the address of +0x30c, the member after field_100;
    // its type and that it belongs to this class are guesses.
    char* fn_005462ec();
    void fn_00546325(int param_1);
    // HYPOTHESIS: 0x00410223 is called on the same object; it may belong to a base class.
    int fn_00410223();

    char field_0[0x90];
    c_00451045 field_90;
    char field_91[0x100 - 0x91];
    // HYPOTHESIS: the stores state 0x005462e5 returns, at +0x100.
    c_00427db2 field_100;
    char field_30c[0x430 - 0x30c];
    // HYPOTHESIS: the flag word at +0x430 that 0x00546264 tests bit 0 of.
    unsigned int field_430;
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

// HYPOTHESIS: the object behind 0x00546278 keeps a byte at +0x58; its class is not known.
class c_00546278
{
public:
    unsigned char fn_00546278();
    void fn_0054627c(unsigned char param_1);
    void fn_005462f3(const nDatabase::tID& param_1);

    char field_0[0x48];
    // HYPOTHESIS: the database ID 0x005462f3 copies in; 0x10 bytes up to field_58.
    nDatabase::tID field_48;
    unsigned char field_58;
};

// FUNCTION: STARFLEETOP 0x00546278
unsigned char c_00546278::fn_00546278()
{
    return field_58;
}

// FUNCTION: STARFLEETOP 0x0054627c
void c_00546278::fn_0054627c(unsigned char param_1)
{
    field_58 = param_1;
}

// HYPOTHESIS: the object behind 0x00546286 keeps a byte flag at +0xc3; its class is not known.
class c_00546286
{
public:
    void fn_00546286();

    char field_0[0xc3];
    unsigned char field_c3;
};

// FUNCTION: STARFLEETOP 0x00546286
void c_00546286::fn_00546286()
{
    field_c3 = 0;
}

// HYPOTHESIS: the object behind 0x0054628e keeps a dword at +0x5c; its class is not known.
class c_0054628e
{
public:
    int fn_0054628e();
    int fn_00546292();

    char field_0[0x5c];
    int field_5c;
    int field_60;
};

// FUNCTION: STARFLEETOP 0x0054628e
int c_0054628e::fn_0054628e()
{
    return field_5c;
}

// FUNCTION: STARFLEETOP 0x00546292
int c_0054628e::fn_00546292()
{
    return field_60;
}

// HYPOTHESIS: the object behind 0x00546296 keeps a std::string at +0x6c, returned by copy
// (the allocator byte, the string's reset 0x005da20a and its assign 0x00403b49); its class
// is not known.
class c_00546296
{
public:
    std::string fn_00546296() const;

    char field_0[0x6c];
    std::string field_6c;
};

// FUNCTION: STARFLEETOP 0x00546296
std::string c_00546296::fn_00546296() const
{
    return field_6c;
}

// HYPOTHESIS: the object behind 0x005462cc keeps a dword at +0x7c, just past the std::string
// at +0x6c of 0x00546296's object; nothing yet shows they are one class.
class c_005462cc
{
public:
    int fn_005462cc();

    char field_0[0x7c];
    int field_7c;
};

// FUNCTION: STARFLEETOP 0x005462cc
int c_005462cc::fn_005462cc()
{
    return field_7c;
}

// HYPOTHESIS: returns the address of a member at +0x80, likely of the same object as
// 0x005462cc's; the member's type is not known yet.
class c_005462d0
{
public:
    int* fn_005462d0();

    char field_0[0x80];
    int field_80;
};

// FUNCTION: STARFLEETOP 0x005462d0
int* c_005462d0::fn_005462d0()
{
    return &field_80;
}

// FUNCTION: STARFLEETOP 0x005462d7
unsigned int c_00546027::fn_005462d7()
{
    return field_430;
}

// FUNCTION: STARFLEETOP 0x005462de
c_00451045* c_00546027::fn_005462de()
{
    return &field_90;
}

// FUNCTION: STARFLEETOP 0x005462e5
const c_00427db2& c_00546027::fn_005462e5()
{
    return field_100;
}

// FUNCTION: STARFLEETOP 0x005462ec
char* c_00546027::fn_005462ec()
{
    return field_30c;
}

// FUNCTION: STARFLEETOP 0x005462f3
void c_00546278::fn_005462f3(const nDatabase::tID& param_1)
{
    field_48 = param_1;
    if (field_48.IsValid())
        fn_0054627c(0);
}

// FUNCTION: STARFLEETOP 0x00546325
void c_00546027::fn_00546325(int param_1)
{
    field_90.fn_00450e41(param_1);
}

// HYPOTHESIS: the object behind 0x00546337 keeps two pairs of 25-byte arrays from +0x9c,
// the second of each pair the first clamped against the argument's arrays at +0x25 and
// +0x57; neither class is known.
class c_00546337_arg
{
public:
    char field_0[0x25];
    char field_25[0x19];
    char field_3e[0x57 - 0x3e];
    char field_57[0x19];
};

class c_00546337
{
public:
    void fn_00546337(const c_00546337_arg& param_1);

    char field_0[0x9c];
    char field_9c[0x19];
    char field_b5[0x19];
    char field_ce[0x19];
    char field_e7[0x19];
};

// FUNCTION: STARFLEETOP 0x00546337
void c_00546337::fn_00546337(const c_00546337_arg& param_1)
{
    int i;
    for (i = 0; i < 0x19; i++)
    {
        // HYPOTHESIS: the minimum is computed twice, as a max(0, min(a, b)) macro expands.
        field_b5[i] = 0 > (field_9c[i] < param_1.field_25[i] ? field_9c[i] : param_1.field_25[i]) ? 0 : (field_9c[i] < param_1.field_25[i] ? field_9c[i] : param_1.field_25[i]);
    }
    for (i = 0; i < 0x19; i++)
    {
        field_e7[i] = 0 > (field_ce[i] < param_1.field_57[i] ? field_ce[i] : param_1.field_57[i]) ? 0 : (field_ce[i] < param_1.field_57[i] ? field_ce[i] : param_1.field_57[i]);
    }
}
