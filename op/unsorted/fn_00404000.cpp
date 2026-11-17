// HYPOTHESIS: a counter pair held through pointers at +0x1c and +0x2c; class and name unknown.
class fn_00404014_Counters
{
public:
    char pad_00[0x1c];
    int* m_pDown;
    char pad_20[0x0c];
    int* m_pUp;
    int fn_00404014();
};

// FUNCTION: STARFLEETOP 0x00404014
int fn_00404014_Counters::fn_00404014()
{
    *m_pUp += 1;
    *m_pDown -= 1;
    return *m_pDown;
}

// HYPOTHESIS: another counter pair, held through pointers at +0x20 and +0x30; class and name unknown.
class fn_00404034_Counters
{
public:
    char pad_00[0x20];
    int* m_pUp;
    char pad_24[0x0c];
    int* m_pDown;
    int fn_00404034();
};

// FUNCTION: STARFLEETOP 0x00404034
int fn_00404034_Counters::fn_00404034()
{
    *m_pDown = *m_pDown - 1;
    return (*m_pUp)++;
}

#include <string>
#include <iostream>

// HYPOTHESIS: std::vector<std::string>::erase(first, last); the vector's end pointer sits at +8.
class fn_00404044_Vector
{
public:
    char pad_00[0x08];
    std::string* _Last;
    std::string* fn_00404044(std::string* _F, std::string* _L);
};

// MATCH-ONLY: the library's copy and _Destroy, inlined by the original.
inline std::string* fn_00404044_Copy(std::string* _F, std::string* _L, std::string* _X)
{
    for (; _F != _L; ++_X, ++_F)
        _X->assign(*_F, 0, std::string::npos);
    return _X;
}

inline void fn_00404044_Destroy(std::string* _F, std::string* _L)
{
    for (; _F != _L; ++_F)
        _F->~basic_string();
}

// FUNCTION: STARFLEETOP 0x00404044
std::string* fn_00404044_Vector::fn_00404044(std::string* _F, std::string* _L)
{
    std::string* _S = fn_00404044_Copy(_L, _Last, _F);
    fn_00404044_Destroy(_S, _Last);
    _Last = _S;
    return _F;
}

// HYPOTHESIS: the library's std::iostream constructor taking a streambuf, written out as a
// stand-in class with the same bases.
class fn_004040a4_Iostream : public std::istream, public std::ostream
{
public:
    fn_004040a4_Iostream(std::streambuf* _S);
};

// FUNCTION: STARFLEETOP 0x004040a4
fn_004040a4_Iostream::fn_004040a4_Iostream(std::streambuf* _S)
    : std::basic_istream<char, std::char_traits<char> >(_S), std::basic_ostream<char, std::char_traits<char> >(_S, false, false)
{
}

// HYPOTHESIS: the out-of-line copy of the library's inline std::locale default constructor (the
// header defines it in the class, so it cannot be redefined); written out as a stand-in class.
// m0 is the implementation pointer, taken from std::locale::_Init at 0x0074c431; the global at
// 0x00c8032c is std::locale::_Locimp::_Global.
class fn_00404184_Locale
{
public:
    fn_00404184_Locale();
    std::locale::_Locimp* m0;
};

// HYPOTHESIS: the reference-count increment at 0x00402b90 (defined in fn_00402000.cpp as
// fn_00402b90_Obj::fn_00402b90), seen here as a no-throw call.
class fn_00402b90_Obj
{
public:
    void fn_00402b90() throw();
    unsigned long m0;
    unsigned long m4;
};

extern fn_00402b90_Obj* g_00c8032c;
std::locale::_Locimp* fn_0074c431() throw();

// FUNCTION: STARFLEETOP 0x00404184
fn_00404184_Locale::fn_00404184_Locale()
{
    m0 = fn_0074c431();
    std::_Lockit lock;
    g_00c8032c->fn_00402b90();
}

// HYPOTHESIS: a locked-state owner; m0c points to a pointer to the lock object released by the
// call at 0x0072eec0, m3c is cleared and bit 0 of m44 marks "held". The real class is unknown.
class fn_004041b2_Obj
{
public:
    void fn_004041b2();
    char pad0[0xc];
    void** m0c;
    char pad10[0x2c];
    unsigned long m3c;
    unsigned long m40;
    unsigned long m44;
};

// FUNCTION: STARFLEETOP 0x004041b2
void fn_004041b2_Obj::fn_004041b2()
{
    if (m44 & 1)
        operator delete(*m0c);
    m3c = 0;
    m44 &= 0xfffffffe;
}

// HYPOTHESIS: unknown class; m50 is a state word tested before the lookup.
class fn_00404230_Item;

class fn_00404230_Obj
{
public:
    fn_00404230_Obj* fn_00404230(unsigned long a, unsigned long b);
    void fn_0040499a(fn_00404230_Item* item, int n);
    void fn_00404a0c();
    char pad0[0x50];
    unsigned long m50;
};

fn_00404230_Item* fn_0074c69d(unsigned long a, unsigned long b);

// FUNCTION: STARFLEETOP 0x00404230
fn_00404230_Obj* fn_00404230_Obj::fn_00404230(unsigned long a, unsigned long b)
{
    if (m50 == 0) {
        fn_00404230_Item* p = fn_0074c69d(a, b);
        if (p != 0) {
            this->fn_0040499a(p, 1);
            this->fn_00404a0c();
            return this;
        }
    }
    return 0;
}
