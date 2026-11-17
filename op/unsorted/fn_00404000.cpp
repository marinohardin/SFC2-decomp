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
    fn_00404184_Locale() throw();
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
fn_00404184_Locale::fn_00404184_Locale() throw()
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
    fn_00404230_Obj* fn_00404267();
    std::locale getloc() const;
    char pad0[0x38];
    std::codecvt<char, char, int>* m38;
    char pad3c[0x08];
    std::string* m44;
    char pad48[0x04];
    std::locale m4c;
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

// FUNCTION: STARFLEETOP 0x00404267
fn_00404230_Obj* fn_00404230_Obj::fn_00404267()
{
    if (m50 != 0) {
        if (fclose((FILE*)m50) == 0) {
            this->fn_0040499a(0, 2);
            return this;
        }
    }
    return 0;
}

#include <string>

// FUNCTION: STARFLEETOP 0x0040428e
std::string& std::string::assign(const char* _S, size_type _N)
{
    if (_Grow(_N, true))
    {
        std::char_traits<char>::copy(_Ptr, _S, _N);
        _Eos(_N);
    }
    return (*this);
}

// HYPOTHESIS: the out-of-line copy of the library's std::ostream constructor, which the
// header defines in the class. MATCH-ONLY: written as a specialization with the header's
// body, since the copy the header instantiates carries no line of this unit.
// FUNCTION: STARFLEETOP 0x00404374
template<> std::basic_ostream<char, std::char_traits<char> >::basic_ostream(std::streambuf* _S, bool _Isstd, bool _Doinit)
{
    if (_Doinit)
        init(_S, _Isstd);
}

// HYPOTHESIS: the out-of-line copy of the library's std::ios::init, written as a
// specialization with the header's body.
// FUNCTION: STARFLEETOP 0x004043de
template<> void std::basic_ios<char, std::char_traits<char> >::init(std::streambuf* _S, bool _Isstd)
{
    _Sb = _S;
    _Tiestr = 0;
    _Fillch = ' ';
    _Init();
    if (_Sb == 0)
        setstate(badbit);
    if (_Isstd)
        _Addstd();
}

// HYPOTHESIS: std::vector<element>::insert(pos, n, x), the out-of-line copy of the library's
// member, written as a specialization with the header's body. The element is 0xb8 bytes with a
// virtual destructor, a copy constructor (0x004064a8) and an assignment (0x00401b2d); the
// analysis calls it tBmpSurf, but there is no evidence for the name yet.
#include <vector>

class fn_0040441e_Elem
{
public:
    virtual ~fn_0040441e_Elem();
    fn_0040441e_Elem(const fn_0040441e_Elem&);
    fn_0040441e_Elem& operator=(const fn_0040441e_Elem&);
    char pad_04[0xb4];
};

// FUNCTION: STARFLEETOP 0x0040441e
template<> void std::vector<fn_0040441e_Elem>::insert(iterator _P, size_type _M, const fn_0040441e_Elem& _X)
{
    if (_End - _Last < _M)
    {
        size_type _N = size() + (_M < size() ? size() : _M);
        iterator _S = allocator.allocate(_N, (void *)0);
        iterator _Q = _Ucopy(_First, _P, _S);
        _Ufill(_Q, _M, _X);
        _Ucopy(_P, _Last, _Q + _M);
        _Destroy(_First, _Last);
        allocator.deallocate(_First, _End - _First);
        _End = _S + _N;
        _Last = _S + size() + _M;
        _First = _S;
    }
    else if (_Last - _P < _M)
    {
        _Ucopy(_P, _Last, _P + _M);
        _Ufill(_Last, _M - (_Last - _P), _X);
        fill(_P, _Last, _X);
        _Last += _M;
    }
    else if (0 < _M)
    {
        _Ucopy(_Last - _M, _Last, _Last);
        copy_backward(_P, _Last - _M, _Last);
        fill(_P, _P + _M, _X);
        _Last += _M;
    }
}

// MATCH-ONLY: the original's callers take this out-of-line copy; nothing in this unit calls it.
void (std::vector<fn_0040441e_Elem>::*fn_0040441e_Insert)(std::vector<fn_0040441e_Elem>::iterator, std::vector<fn_0040441e_Elem>::size_type, const fn_0040441e_Elem&) = &std::vector<fn_0040441e_Elem>::insert;

// HYPOTHESIS: std::vector<std::string>::insert(pos, n, x), the out-of-line copy of the library's
// member, written as a specialization with the header's body.
// FUNCTION: STARFLEETOP 0x00404695
template<> void std::vector<std::string>::insert(iterator _P, size_type _M, const std::string& _X)
{
    if (_End - _Last < _M)
    {
        size_type _N = size() + (_M < size() ? size() : _M);
        iterator _S = allocator.allocate(_N, (void *)0);
        iterator _Q = _Ucopy(_First, _P, _S);
        _Ufill(_Q, _M, _X);
        _Ucopy(_P, _Last, _Q + _M);
        _Destroy(_First, _Last);
        allocator.deallocate(_First, _End - _First);
        _End = _S + _N;
        _Last = _S + size() + _M;
        _First = _S;
    }
    else
    {
        if (_Last - _P < _M)
        {
            _Ucopy(_P, _Last, _P + _M);
            _Ufill(_Last, _M - (_Last - _P), _X);
            fill(_P, _Last, _X);
        }
        else if (0 < _M)
        {
            _Ucopy(_Last - _M, _Last, _Last);
            copy_backward(_P, _Last - _M, _Last);
            fill(_P, _P + _M, _X);
        }
        else
            return;
        _Last += _M;
    }
}

// MATCH-ONLY: the original's callers take this out-of-line copy; nothing in this unit calls it.
void (std::vector<std::string>::*fn_00404695_Insert)(std::vector<std::string>::iterator, std::vector<std::string>::size_type, const std::string&) = &std::vector<std::string>::insert;

// HYPOTHESIS: the buffer setup of a string-stream buffer; the pointers at +0x0c to +0x30 are the
// streambuf's get and put area pointers, held by address.
class fn_004048dd_Buf
{
public:
    void fn_004048dd(char* _G, int _N, int _Mode);
    char pad0[0x0c];
    char** m0c;
    char** m10;
    char pad14[0x08];
    char** m1c;
    char** m20;
    char pad24[0x08];
    char** m2c;
    char** m30;
    char pad34[0x04];
    char* m38;
    char* m3c;
    int m40;
    int m44;
};

void* fn_007365b0(void* dst, const void* src, unsigned int n);

// FUNCTION: STARFLEETOP 0x004048dd
void fn_004048dd_Buf::fn_004048dd(char* _G, int _N, int _Mode)
{
    m38 = 0;
    m3c = 0;
    m40 = 0x20;
    m44 = _Mode;
    if (_N == 0 || (_Mode & 6) == 6)
    {
        *m0c = 0;
        *m1c = 0;
        *m2c = 0;
        *m10 = 0;
        *m20 = 0;
        *m30 = 0;
    }
    else
    {
        int n = _N;
        if (n < 0)
            n = 0;
        char* p = (char*)operator new(n);
        fn_007365b0(p, _G, _N);
        m3c = p + _N;
        if ((m44 & 4) == 0)
        {
            *m0c = p;
            *m1c = p;
            *m2c = (char*)_N;
        }
        if ((m44 & 2) == 0)
        {
            *m10 = p;
            *m20 = p;
            *m30 = (char*)_N;
            if (*m1c == 0)
            {
                *m0c = p;
                *m1c = p;
                *m2c = 0;
            }
        }
        m44 |= 1;
    }
}

// HYPOTHESIS: a stream buffer's area-pointer setup; +0x4c holds a locale object (constructed by
// 0x00404184, released by 0x00402b22), +0x50 the area base, 0x00404b1c resets the buffer state.
class fn_0040499a_Obj
{
public:
    void fn_0040499a(char** _P, int _M);
    void fn_00404b1c();
    char pad0[0x0c];
    char** m0c;
    char** m10;
    char pad14[0x08];
    char** m1c;
    char** m20;
    char pad24[0x08];
    char** m2c;
    char** m30;
    char pad34[0x04];
    int m38;
    int m3c;
    int m40;
    int m44;
    bool m48;
    char pad49[0x03];
    std::locale m4c;
    char** m50;
};

extern int g_00c81934;

// FUNCTION: STARFLEETOP 0x0040499a
void fn_0040499a_Obj::fn_0040499a(char** _P, int _M)
{
    m48 = _M == 1;
    if (_M == 0)
    {
        m4c.~locale();
        if (&m4c != 0)
            new (&m4c) std::locale;
        m44 = 0;
    }
    fn_00404b1c();
    if (_P != 0)
    {
        m1c = _P;
        m0c = _P + 2;
        m10 = _P + 2;
        m20 = _P;
        m2c = _P + 1;
        m30 = _P + 1;
    }
    m50 = _P;
    m40 = g_00c81934;
    int t = g_00c81934;
    m38 = 0;
    m3c = t;
}

// HYPOTHESIS: the stream buffer's codecvt setup (basic_filebuf::_Initcvt in shape); the same
// object as fn_00404230_Obj. It is a template member: compiled outside a template, VC6 adds an
// unwind state for the new-expression and tests the bool with cmp, which the original lacks.
template<class _E> class fn_00404230_ObjT
{
public:
    void fn_00404a0c();
    std::locale getloc() const;
    char pad0[0x38];
    std::codecvt<_E, char, int>* m38;
    char pad3c[0x08];
    std::basic_string<_E>* m44;
    char pad48[0x04];
    std::locale m4c;
    unsigned long m50;
};
// FUNCTION: STARFLEETOP 0x00404a0c
template<class _E> void fn_00404230_ObjT<_E>::fn_00404a0c()
{
    m38 = (std::codecvt<_E, char, int>*)&std::use_facet(getloc(), (std::codecvt<_E, char, int>*)0, true);
    m4c = std::_ADDFAC(m4c, m38);
    if (m38->always_noconv())
        m38 = 0;
    if (m44 == 0)
        m44 = new std::basic_string<_E>;
}
template class fn_00404230_ObjT<char>;

// HYPOTHESIS: std::locale's copy constructor: share the implementation and bump its count.
class fn_00404aba_Locale
{
public:
    fn_00404aba_Locale(const fn_00404aba_Locale& other) throw();
    fn_00402b90_Obj* m0;
};

// FUNCTION: STARFLEETOP 0x00404aba
fn_00404aba_Locale::fn_00404aba_Locale(const fn_00404aba_Locale& other) throw()
{
    m0 = other.m0;
    m0->fn_00402b90();
}

// HYPOTHESIS: std::locale::operator=: release the old implementation, share the new one.
class fn_00404ad0_Base
{
public:
    virtual ~fn_00404ad0_Base();
};

class fn_00404ad0_Impl
{
public:
    fn_00404ad0_Base* fn_00402b3a();
    void fn_00402b90() throw();
};

class fn_00404ad0_Locale
{
public:
    fn_00404ad0_Locale& fn_00404ad0(const fn_00404ad0_Locale& other) throw();
    fn_00404ad0_Impl* m0;
};

// FUNCTION: STARFLEETOP 0x00404ad0
fn_00404ad0_Locale& fn_00404ad0_Locale::fn_00404ad0(const fn_00404ad0_Locale& other) throw()
{
    if (m0 != other.m0)
    {
        fn_00404ad0_Base* p = m0->fn_00402b3a();
        delete p;
        m0 = other.m0;
        m0->fn_00402b90();
    }
    return *this;
}

// HYPOTHESIS: std::ios_base::getloc: return a copy of the stream's locale (at +0x34).
class fn_00404aff_Locale
{
public:
    fn_00404aff_Locale(const fn_00404aff_Locale& other) throw()
    {
        m0 = other.m0;
        m0->fn_00402b90();
    }
    ~fn_00404aff_Locale();
    fn_00404ad0_Impl* m0;
};

class fn_00404aff_Ios
{
public:
    fn_00404aff_Locale fn_00404aff() const;
    char m0[0x34];
    fn_00404aff_Locale m34;
};

// FUNCTION: STARFLEETOP 0x00404aff
fn_00404aff_Locale fn_00404aff_Ios::fn_00404aff() const
{
    return m34;
}

// HYPOTHESIS: std::streambuf initialiser: each area pointer pair points at its own slots, then is cleared.
class fn_00404b1c_Streambuf
{
public:
    void fn_00404b1c();
    void fn_setp(char* f, char* l);
    void fn_setg(char* b, char* n, char* l);
    int m0;
    char* m4;
    char* m8;
    char** mc;
    char** m10;
    char* m14;
    char* m18;
    char** m1c;
    char** m20;
    int m24;
    int m28;
    int* m2c;
    int* m30;
};

// HYPOTHESIS: the library's setp and setg, inlined into the initialiser.
inline void fn_00404b1c_Streambuf::fn_setp(char* f, char* l)
{
    *m10 = f, *m20 = f;
    *m30 = l - f;
}

inline void fn_00404b1c_Streambuf::fn_setg(char* b, char* n, char* l)
{
    *mc = b, *m1c = n;
    *m2c = l - n;
}

// FUNCTION: STARFLEETOP 0x00404b1c
void fn_00404b1c_Streambuf::fn_00404b1c()
{
    mc = &m4, m10 = &m8;
    m1c = &m14, m20 = &m18;
    m2c = &m24, m30 = &m28;
    fn_setp(0, 0), fn_setg(0, 0, 0);
}

// HYPOTHESIS: the library's use_facet for ctype<char>, the out-of-line instantiation of the
// header template; written here as an explicit specialization with the header's body.
// FUNCTION: STARFLEETOP 0x00404b5e
template<> const std::ctype<char>& __cdecl std::use_facet(const std::locale& _L,
    const std::ctype<char> *, bool _Cfacet)
{
    static const std::locale::facet *_Psave = 0;
    std::_Lockit _Lk;
    size_t _Id = std::ctype<char>::id;
    const std::locale::facet *_Pf = _L._Getfacet(_Id, true);
    if (_Pf != 0)
        ;
    else if (!_Cfacet || !_L._Iscloc())
        throw std::bad_cast("missing locale facet");
    else if (_Psave == 0)
        _Pf = _Psave = std::_Tidyfac<std::ctype<char> >::_Save(new std::ctype<char>);
    else
        _Pf = _Psave;
    return (*(const std::ctype<char> *)_Pf);
}

// HYPOTHESIS: istream::ipfx (in another unit) calls this out-of-line instantiation.
const std::ctype<char>& (__cdecl *g_fn_00404b5e)(const std::locale&, const std::ctype<char> *, bool)
    = &std::use_facet;

// HYPOTHESIS: the out-of-line copy of the library's ctype<char> constructor, written as a stand-in
// class because the header defines it in the class; +0x10 is the table, +0x14 the delete flag, and
// 0x00404d6e is the private _Init taking a _Locinfo.
extern const short *g_00c80334;

class fn_00404cb2_Ctype : public std::locale::facet
{
public:
    fn_00404cb2_Ctype(const short *_Tab, bool _Df, size_t _Refs);
    void fn_00404d6e(const std::_Locinfo&);
    void fn_00404dd0(unsigned char);
    char *fn_00404de4(char *, const char *);
    char fn_00404e0f(unsigned char);
    char *fn_00404e23(char *, const char *);
    size_t m08;
    size_t m0c;
    const short *m10;
    size_t m14;
};

// HYPOTHESIS: the atexit handler that frees the shared "C" table; defined elsewhere in the exe.
extern void __cdecl fn_00404dc3();

// FUNCTION: STARFLEETOP 0x00404d6e
void fn_00404cb2_Ctype::fn_00404d6e(const std::_Locinfo& _Lobj)
{
    std::_Lockit _Lk;
    *(_Ctypevec *)&m08 = _Lobj._Getctype();
    if (g_00c80334 == 0)
    {
        g_00c80334 = m10;
        atexit(fn_00404dc3);
        m14 = 0;
    }
}

// FUNCTION: STARFLEETOP 0x00404cb2
fn_00404cb2_Ctype::fn_00404cb2_Ctype(const short *_Tab, bool _Df, size_t _Refs)
    : std::locale::facet(_Refs)
{
    std::_Lockit _Lk;
    fn_00404d6e(std::_Locinfo("C"));
    if (m14)
    {
        operator delete((void *)m10);
        m14 = 0;
    }
    if (_Tab == 0)
        m10 = g_00c80334;
    else
        m10 = _Tab, m14 = _Df;
}

// HYPOTHESIS: callee 0x0074cdc8 is a library helper taking a flag and the facet's _Ctypevec at +8.
extern char __cdecl fn_0074cdc8(int, void *);

// FUNCTION: STARFLEETOP 0x00404dd0
void fn_00404cb2_Ctype::fn_00404dd0(unsigned char flag)
{
    fn_0074cdc8(flag, &m08);
}

// FUNCTION: STARFLEETOP 0x00404de4
char *fn_00404cb2_Ctype::fn_00404de4(char *first, const char *last)
{
    for (; first != last; ++first)
        *first = fn_0074cdc8(*first, &m08);
    return first;
}

// HYPOTHESIS: callee 0x0074cfd7 is the sibling library helper of 0x0074cdc8, same arguments.
extern char __cdecl fn_0074cfd7(int, void *);

// FUNCTION: STARFLEETOP 0x00404e0f
char fn_00404cb2_Ctype::fn_00404e0f(unsigned char c)
{
    return fn_0074cfd7(c, &m08);
}

// FUNCTION: STARFLEETOP 0x00404e23
char *fn_00404cb2_Ctype::fn_00404e23(char *first, const char *last)
{
    for (; first != last; ++first)
        *first = fn_0074cfd7(*first, &m08);
    return first;
}
