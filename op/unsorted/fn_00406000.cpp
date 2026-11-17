#include <locale>

// FUNCTION: STARFLEETOP 0x00406003
std::string& std::string::append(const std::string& _X, size_type _P, size_type _M)
{
    if (_X.size() < _P)
        _Xran();
    size_type _N = _X.size() - _P;
    if (_N < _M)
        _M = _N;
    if (npos - _Len <= _M)
        _Xlen();
    if (0 < _M && _Grow(_N = _Len + _M))
    {
        std::char_traits<char>::copy(_Ptr + _Len, &_X.c_str()[_P], _M);
        _Eos(_N);
    }
    return (*this);
}

// HYPOTHESIS: a caller in the original uses this instantiation.
std::string& (std::string::*g_fn_00406003)(const std::string&, std::string::size_type, std::string::size_type) = &std::string::append;

int fn_00736a59(int ch, void* stream);
void fn_00736af0(void* stream);

// FUNCTION: STARFLEETOP 0x0040608b
bool fn_0040608b(const unsigned char* buf, void* stream, unsigned int count)
{
    const unsigned char* p = buf + count;
    while (count > 0)
    {
        --p;
        if (fn_00736a59(*p, stream) == -1)
            break;
        --count;
    }
    if (count == 0)
        return true;
    unsigned int want = 1;
    if (count < want)
    {
        unsigned int n = want - count;
        do
        {
            fn_00736af0(stream);
        } while (--n != 0);
    }
    return false;
}
class fn_00402b90_Obj;
fn_00402b90_Obj* fn_0040673f(fn_00402b90_Obj* p);

// HYPOTHESIS: the original is the library's use_facet instantiation for numpunct<char>.
// FUNCTION: STARFLEETOP 0x004060d8
const std::numpunct<char>& fn_004060d8(const std::locale& _L, const std::numpunct<char>*, bool _Cfacet)
{
    static const std::locale::facet* _Psave = 0;
    std::_Lockit _Lk;
    size_t _Id = std::numpunct<char>::id;
    const std::locale::facet* _Pf = _L._Getfacet(_Id, true);
    if (_Pf != 0)
        ;
    else if (!_Cfacet || !_L._Iscloc())
        throw std::bad_cast("missing locale facet");
    else if (_Psave != 0)
        _Pf = _Psave;
    else
        _Pf = _Psave = (const std::locale::facet*)fn_0040673f((fn_00402b90_Obj*)new std::numpunct<char>);
    return *(const std::numpunct<char>*)_Pf;
}

// FUNCTION: STARFLEETOP 0x0040619c
// HYPOTHESIS: the library's _Getloctxt instantiation for istreambuf_iterator<char>.
template<class _E, class _II>
int fn_0040619c(_II& _F, _II& _L, size_t _N, const _E* _S)
{
    for (size_t _I = 0; _S[_I] != (char)0; ++_I)
        if (_S[_I] == _S[0])
            ++_N;
    std::string _Str(_N, '\0');
    int _Ans = -2;
    for (size_t _J = 1; ; ++_J, ++_F, _Ans = -1)
    {
        bool _Pfx = false;
        size_t _I = 0, _K = 0;
        for (; _K < _N; ++_K)
        {
            for (; _S[_I] != (char)0 && _S[_I] != _S[0]; ++_I)
                ;
            if (_Str[_K] != '\0')
                _I += _Str[_K];
            else if (_S[_I += _J] == _S[0] || _S[_I] == (char)0)
            {
                _Str[_K] = _J < 127 ? _J : 127;
                _Ans = _K;
            }
            else if (_F == _L || _S[_I] != *_F)
                _Str[_K] = _J < 127 ? _J : 127;
            else
                _Pfx = true;
        }
        if (!_Pfx || _F == _L)
            break;
    }
    return (_Ans);
}
template int fn_0040619c<char, std::istreambuf_iterator<char> >(std::istreambuf_iterator<char>&, std::istreambuf_iterator<char>&, size_t, const char*);

fn_00402b90_Obj* fn_0040678d(fn_00402b90_Obj* p);

// HYPOTHESIS: the library's use_facet instantiation for collate<char>.
// FUNCTION: STARFLEETOP 0x00406355
const std::collate<char>& fn_00406355(const std::locale& _L, const std::collate<char>*, bool _Cfacet)
{
    static const std::locale::facet* _Psave = 0;
    std::_Lockit _Lk;
    size_t _Id = std::collate<char>::id;
    const std::locale::facet* _Pf = _L._Getfacet(_Id, true);
    if (_Pf != 0)
        ;
    else if (!_Cfacet || !_L._Iscloc())
        throw std::bad_cast("missing locale facet");
    else if (_Psave != 0)
        _Pf = _Psave;
    else
        _Pf = _Psave = (const std::locale::facet*)fn_0040678d((fn_00402b90_Obj*)new std::collate<char>);
    return *(const std::collate<char>*)_Pf;
}

// HYPOTHESIS: a locale-valued helper of the library; the types below only name what the
// instructions call (0x00404c82 id lookup, 0x0074c78e facet lookup on the locale argument).
class fn_00406419_Id
{
public:
    size_t fn_00404c82(int);
};

extern fn_00406419_Id g_00c80338;

class fn_00406419_Loc
{
public:
    fn_00406419_Loc(const fn_00406419_Loc&);
    ~fn_00406419_Loc();
    const std::locale* fn_0074c78e(int, size_t);
    std::locale m0;
};

// FUNCTION: STARFLEETOP 0x00406419
std::locale fn_00406419(std::locale _L, int _X)
{
    std::_Lockit _T;
    size_t _Id = g_00c80338.fn_00404c82(2);
    return *((fn_00406419_Loc*)&_L)->fn_0074c78e(_X, _Id);
}

const std::ctype<char>& fn_00404b5e(const std::locale&, const std::ctype<char>*, bool);

// HYPOTHESIS: the library's isdigit(char, locale) instantiation (ctype mask 4 is _DIGIT).
// FUNCTION: STARFLEETOP 0x00406487
bool fn_00406487(char _C, const std::locale& _L)
{
    return fn_00404b5e(_L, 0, true).is(std::ctype_base::digit, _C);
}

// HYPOTHESIS: tFighterSpec is declared only here; the argument is its copy source.
class tFighterSpec
{
public:
    tFighterSpec(const tFighterSpec&);
};

// HYPOTHESIS: the library allocator's construct() instantiation for tFighterSpec.
// FUNCTION: STARFLEETOP 0x004064a8
void fn_004064a8(tFighterSpec* _P, const tFighterSpec& _V)
{
    new ((void*)_P) tFighterSpec(_V);
}

// HYPOTHESIS: the library allocator's construct() instantiation for std::string.
// FUNCTION: STARFLEETOP 0x004064d5
void fn_004064d5(std::string* _P, const std::string& _V)
{
    new ((void*)_P) std::string(_V);
}

// HYPOTHESIS: the library's string::compare(const string&) instance, written on a stand-in
// class of the string's layout; it calls the library's compare at 0x004067db.
class fn_0040651e_String
{
public:
    int fn_0040651e(const std::string& _X) const;
    char _A;
    char* _Ptr;
    size_t _Len;
    size_t _Res;
};

// FUNCTION: STARFLEETOP 0x0040651e
int fn_0040651e_String::fn_0040651e(const std::string& _X) const
{
    return (((const std::string*)this)->compare(0, _Len, _X.c_str(), _X.size()));
}

// HYPOTHESIS: the library's istreambuf_iterator<char>::equal instance, written on a stand-in
// class of the iterator's layout; 0x00405fdf is _Peek.
class fn_00406540_Iter
{
public:
    bool fn_00406540(const fn_00406540_Iter& _X) const;
    char fn_00405fdf();
    std::streambuf* _Sbuf;
    bool _Got;
    char _Val;
};

// FUNCTION: STARFLEETOP 0x00406540
bool fn_00406540_Iter::fn_00406540(const fn_00406540_Iter& _X) const
{
    if (!_Got)
        ((fn_00406540_Iter *)this)->fn_00405fdf();
    if (!_X._Got)
        ((fn_00406540_Iter *)&_X)->fn_00405fdf();
    return (_Sbuf == 0 && _X._Sbuf == 0
        || _Sbuf != 0 && _X._Sbuf != 0);
}

// FUNCTION: STARFLEETOP 0x0040657f
// HYPOTHESIS: the library's collate<char> constructor instantiation (default locale info, _Init inlined).
template<> std::collate<char>::collate(size_t _R)
    : std::locale::facet(_R)
{
    std::_Locinfo _Lobj;
    _Init(_Lobj);
}

// HYPOTHESIS: the library's codecvt<char,char,int>::do_in and do_length instances (identity
// conversion), written on a stand-in class; do_in returns noconv.
class fn_004065d3_Codecvt
{
public:
    std::codecvt_base::result fn_004065d3(int&, const char* _F1, const char*, const char*& _M1, char* _F2, char*, char*& _M2) const;
    int fn_004065ed(int&, const char* _F1, const char* _L1, size_t _N2) const;
};

// FUNCTION: STARFLEETOP 0x004065d3
std::codecvt_base::result fn_004065d3_Codecvt::fn_004065d3(int&, const char* _F1, const char*, const char*& _M1, char* _F2, char*, char*& _M2) const
{
    _M1 = _F1;
    _M2 = _F2;
    return (std::codecvt_base::noconv);
}

// FUNCTION: STARFLEETOP 0x004065ed
int fn_004065d3_Codecvt::fn_004065ed(int&, const char* _F1, const char* _L1, size_t _N2) const
{
    size_t _N = _L1 - _F1;
    if (_N2 < _N)
        _N = _N2;
    return (int)_N;
}

// HYPOTHESIS: the base that 0x00402b3a returns from the facet-like object of
// fn_00402b90_Obj.h; only its destructor is named.
class fn_00406887_Base
{
public:
    virtual ~fn_00406887_Base();
};

#include "unsorted/fn_00402b90_Obj.h"






extern fn_00402b90_Obj* g_00893184;
void fn_00406887();

// HYPOTHESIS: registers a library-owned facet: remember it, take a reference and arrange its release at exit.
// FUNCTION: STARFLEETOP 0x004066a3
fn_00402b90_Obj* fn_004066a3(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_00893184 = p;
    p->fn_00402b90();
    atexit(fn_00406887);
    return p;
}

extern fn_00402b90_Obj* g_00893188;
void fn_004068d4();

// HYPOTHESIS: the same registration as 0x004066a3, for a second library-owned facet.
// FUNCTION: STARFLEETOP 0x004066f1
fn_00402b90_Obj* fn_004066f1(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_00893188 = p;
    p->fn_00402b90();
    atexit(fn_004068d4);
    return p;
}

extern fn_00402b90_Obj* g_0089318c;
void fn_004068e0();

// HYPOTHESIS: the same registration as 0x004066a3, for a third library-owned facet.
// FUNCTION: STARFLEETOP 0x0040673f
fn_00402b90_Obj* fn_0040673f(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_0089318c = p;
    p->fn_00402b90();
    atexit(fn_004068e0);
    return p;
}

extern fn_00402b90_Obj* g_00893190;
void fn_004068ec();

// HYPOTHESIS: the same facet registration as 0x004066a3, for a fourth library-owned facet.
// FUNCTION: STARFLEETOP 0x0040678d
fn_00402b90_Obj* fn_0040678d(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_00893190 = p;
    p->fn_00402b90();
    atexit(fn_004068ec);
    return p;
}

// HYPOTHESIS: the library's string::compare(pos, n, const char*, m) instance, written on a
// stand-in class of the string's layout (allocator byte, pointer, length, reserve).
class fn_004067db_String
{
public:
    int fn_004067db(size_t _P0, size_t _N0, const char* _S, size_t _M) const;
    char _A;
    char* _Ptr;
    size_t _Len;
    size_t _Res;
    static const char* _Psum(const char* _P, size_t _N)
        {return (_P == 0 ? 0 : _P + _N); }
};

// FUNCTION: STARFLEETOP 0x004067db
int fn_004067db_String::fn_004067db(size_t _P0, size_t _N0, const char* _S, size_t _M) const
{
    if (_Len < _P0)
        std::_Xran();
    if (_Len - _P0 < _N0)
        _N0 = _Len - _P0;
    size_t _Ans = std::char_traits<char>::compare(_Psum(_Ptr, _P0), _S, _N0 < _M ? _N0 : _M);
    return (_Ans != 0 ? _Ans : _N0 < _M ? -1 : _N0 != _M);
}

// HYPOTHESIS: the library's numpunct<char>::_Init instance, written on a stand-in class of
// the facet's layout (vtable and reference count, then grouping, the two separators and the
// two names).
class fn_0040683e_Numpunct
{
public:
    void fn_0040683e(const std::_Locinfo& _Lobj);
    void* m_00;
    size_t m_04;
    const char* _Gr;
    char _Dp;
    char _Ks;
    const char* _Nf;
    const char* _Nt;
};

// FUNCTION: STARFLEETOP 0x0040683e
void fn_0040683e_Numpunct::fn_0040683e(const std::_Locinfo& _Lobj)
{
    const lconv* _Ptr = _Lobj._Getlconv();
    _Dp = _Ptr->decimal_point[0];
    _Ks = _Ptr->thousands_sep[0];
    _Gr = std::_Maklocstr(_Ptr->grouping, (char*)0);
    _Nf = std::_Maklocstr("false", (char*)0);
    _Nt = std::_Maklocstr("true", (char*)0);
}

// HYPOTHESIS: the library's cleanup of the classic locale pointer; 0x00402b3a is the
// reference-count release of the object at g_00893184 (declared in fn_00402000.cpp).
// FUNCTION: STARFLEETOP 0x00406887
void fn_00406887()
{
    std::_Lockit lock;
    fn_00406887_Base* p = g_00893184->fn_00402b3a();
    delete p;
    g_00893184 = 0;
}

// HYPOTHESIS: the same release as 0x00406887, for the object at g_00893188.
// FUNCTION: STARFLEETOP 0x004068d4
void fn_004068d4()
{
    std::_Lockit lock;
    fn_00406887_Base* p = g_00893188->fn_00402b3a();
    delete p;
    g_00893188 = 0;
}

// HYPOTHESIS: the same release as 0x00406887, for the object at g_0089318c.
// FUNCTION: STARFLEETOP 0x00406921
void fn_00406921()
{
    std::_Lockit lock;
    fn_00406887_Base* p = g_0089318c->fn_00402b3a();
    delete p;
    g_0089318c = 0;
}

// HYPOTHESIS: the same release as 0x00406887, for the object at g_00893190.
// FUNCTION: STARFLEETOP 0x0040696e
void fn_0040696e()
{
    std::_Lockit lock;
    fn_00406887_Base* p = g_00893190->fn_00402b3a();
    delete p;
    g_00893190 = 0;
}

// FUNCTION: STARFLEETOP 0x004069bb
char* fn_004069bb(const char* s)
{
    unsigned int n = strlen(s) + 1;
    char* p = new char[n];
    char* d = p;
    for (; n > 0; n--) {
        *d++ = *s++;
    }
    return p;
}

void fn_004bd431();

// HYPOTHESIS: arranges at exit the cleanup at 0x004bd431 (Ghidra labels it tApp3dBase::vfunction5, unconfirmed).
// FUNCTION: STARFLEETOP 0x00406a35
void fn_00406a35()
{
    atexit(fn_004bd431);
}

// HYPOTHESIS: arranges at exit the cleanup that Ghidra labels tApp3dBase::vfunction5, unconfirmed.
// FUNCTION: STARFLEETOP 0x00406a41
void fn_00406a41()
{
    atexit(fn_004bd431);
}

// HYPOTHESIS: arranges at exit the cleanup that Ghidra labels tApp3dBase::vfunction5, unconfirmed.
// FUNCTION: STARFLEETOP 0x00406a4d
void fn_00406a4d()
{
    atexit(fn_004bd431);
}

// HYPOTHESIS: arranges at exit the cleanup that Ghidra labels tApp3dBase::vfunction5, unconfirmed.
// FUNCTION: STARFLEETOP 0x00406a59
void fn_00406a59()
{
    atexit(fn_004bd431);
}

// HYPOTHESIS: runs the destructor (0x00407c92) of the global at 0x008949a0 at exit; the
// class is declared in another unit and unnamed here.
class fn_00406a86_Global
{
public:
    ~fn_00406a86_Global();
};

extern fn_00406a86_Global g_008949a0;

// FUNCTION: STARFLEETOP 0x00406a86
void fn_00406a86()
{
    g_008949a0.~fn_00406a86_Global();
}

void fn_00406a9c();

// HYPOTHESIS: arranges at exit the cleanup at 0x00406a9c.
// FUNCTION: STARFLEETOP 0x00406a90
void fn_00406a90()
{
    atexit(fn_00406a9c);
}

// HYPOTHESIS: a class holding a std::string followed by two ints; the original type is unknown.
class fn_00406aa6_Obj
{
public:
    fn_00406aa6_Obj(const char* s, int v);
    std::string m0;
    int m10;
    int m14;
};

// FUNCTION: STARFLEETOP 0x00406aa6
fn_00406aa6_Obj::fn_00406aa6_Obj(const char* s, int v)
    : m0(s)
{
    m10 = 0;
    m14 = v;
}

// HYPOTHESIS: a class holding a std::string followed by a flag set to 1 and three ints; the original type is unknown.
class fn_00406add_Obj
{
public:
    fn_00406add_Obj(const char* s, int a, int b, int c);
    std::string m0;
    int m10;
    int m14;
    int m18;
    int m1c;
    int m20;
};

// FUNCTION: STARFLEETOP 0x00406add
fn_00406add_Obj::fn_00406add_Obj(const char* s, int a, int b, int c)
    : m0(s)
{
    m10 = 1;
    m18 = a;
    m1c = b;
    m20 = c;
}

// HYPOTHESIS: a class holding a std::string followed by a flag set to 2 and two ints at 0x24 and 0x28; the original type is unknown.
class fn_00406b23_Obj
{
public:
    fn_00406b23_Obj(const char* s, int a, float b);
    std::string m0;
    int m10;
    int m14;
    int m18;
    int m1c;
    int m20;
    int m24;
    float m28;
};

// FUNCTION: STARFLEETOP 0x00406b23
fn_00406b23_Obj::fn_00406b23_Obj(const char* s, int a, float b)
    : m0(s)
{
    m10 = 2;
    m24 = a;
    m28 = b;
}

// HYPOTHESIS: a class holding a std::string followed by a flag set to 3 and two ints at 0x2c and 0x30; the original type is unknown.
class fn_00406b63_Obj
{
public:
    fn_00406b63_Obj(const char* s, int a, int b);
    std::string m0;
    int m10;
    int m14;
    int m18;
    int m1c;
    int m20;
    int m24;
    int m28;
    int m2c;
    int m30;
};

// FUNCTION: STARFLEETOP 0x00406b63
fn_00406b63_Obj::fn_00406b63_Obj(const char* s, int a, int b)
    : m0(s)
{
    m10 = 3;
    m2c = a;
    m30 = b;
}

// HYPOTHESIS: a class holding a std::string followed by a flag set to 4 and an int at 0x34; the original type is unknown.
class fn_00406ba3_Obj
{
public:
    fn_00406ba3_Obj(const char* s, int a);
    std::string m0;
    int m10;
    int m14;
    int m18;
    int m1c;
    int m20;
    int m24;
    int m28;
    int m2c;
    int m30;
    int m34;
};

// FUNCTION: STARFLEETOP 0x00406ba3
fn_00406ba3_Obj::fn_00406ba3_Obj(const char* s, int a)
    : m0(s)
{
    m10 = 4;
    m34 = a;
}

// HYPOTHESIS: a class holding a std::string first; the original type is unknown.
class fn_00406bdd_Obj
{
public:
    bool fn_00406bdd(const char* s);
    std::string m0;
};

// FUNCTION: STARFLEETOP 0x00406bdd
bool fn_00406bdd_Obj::fn_00406bdd(const char* s)
{
    bool equal = m0.compare(s) == 0;
    return equal;
}

// HYPOTHESIS: the first argument is an object with two parsing members (0x00408393, 0x00408365); the original type is unknown.
class fn_00406c05_Source
{
public:
    float fn_00408393(const char* s, float def);
    int fn_00408365(const char* s, int def);
    int m0;
};

// HYPOTHESIS: the callbacks' receiver class is unknown.
class fn_00406c05_Target
{
public:
    int m0;
};

typedef bool (fn_00406c05_Source::*fn_00406c05_Cb0)(int, const char*);
typedef bool (fn_00406c05_Source::*fn_00406c05_Cb1)(int, const char*, int, int);
typedef void (fn_00406c05_Target::*fn_00406c05_Cb2)(float);
typedef void (fn_00406c05_Target::*fn_00406c05_Cb3)(int);
typedef void (fn_00406c05_Target::*fn_00406c05_Cb4)(const char*);

// HYPOTHESIS: a class holding a std::string, a kind selector and callbacks taking parsed arguments; the original type is unknown.
class fn_00406c05_Obj
{
public:
    bool fn_00406c05(fn_00406c05_Source* a, fn_00406c05_Target* b, const char* s);
    std::string m0;
    int m10;
    fn_00406c05_Cb0 m14;
    fn_00406c05_Cb1 m18;
    int m1c;
    int m20;
    fn_00406c05_Cb2 m24;
    float m28;
    fn_00406c05_Cb3 m2c;
    int m30;
    fn_00406c05_Cb4 m34;
};

// FUNCTION: STARFLEETOP 0x00406c05
bool fn_00406c05_Obj::fn_00406c05(fn_00406c05_Source* a, fn_00406c05_Target* b, const char* s)
{
    bool result;
    if (m10 == 0) {
        result = (a->*m14)((int)b, s);
    } else if (m10 == 1) {
        result = (a->*m18)((int)b, s, m1c, m20);
    } else if (m10 == 2) {
        (b->*m24)(a->fn_00408393(s, m28));
        result = true;
    } else if (m10 == 3) {
        (b->*m2c)(a->fn_00408365(s, m30));
        result = true;
    } else if (m10 == 4) {
        (b->*m34)(s);
        result = true;
    } else {
        result = false;
    }
    return result;
}

// HYPOTHESIS: the callbacks are member functions of an unknown class (setters and parsers of
// the ship list columns); they are passed here by their addresses.
void fn_0040a741();
void fn_0040a76a();
void fn_0040755c();
void fn_0040a788();
void fn_004077a0();
void fn_0040a7bb();
void fn_004077ad();
void fn_004077ba();
void fn_004075bf();
void fn_0040a7dc();
void fn_004075cc();
void fn_004075d9();
void fn_004075e6();
void fn_004075f3();
void fn_00407600();
void fn_0040760d();
void fn_0040761a();
void fn_00407627();
void fn_00407634();
void fn_00407641();
void fn_0040a7fd();
void fn_0040764e();
void fn_0040a834();
void fn_0040a870();
void fn_0040a84b();
void fn_0040765b();
void fn_00407668();
void fn_00407675();
void fn_00407682();
void fn_0040768f();
void fn_00407863();
void fn_0040769c();
void fn_004076a9();
void fn_004076b6();
void fn_004076c3();
void fn_004076d0();
void fn_004076dd();
void fn_00407704();
void fn_00407711();
void fn_004076ea();
void fn_004076f7();
void fn_0040771e();
void fn_0040772b();
void fn_0040a8c1();
void fn_0040a8e0();
void fn_00407738();
void fn_00407745();
void fn_00407752();
void fn_0040775f();
void fn_0040776c();
void fn_00407779();
void fn_00407786();
void fn_0040a921();
void fn_00407793();
void fn_004077c7();
void fn_004077d4();
void fn_004077e1();
void fn_004077ee();
void fn_004077fb();
void fn_00407808();
void fn_00407815();
void fn_00407822();
void fn_0040782f();
void fn_0040783c();
void fn_00407849();
void fn_00407856();
void fn_00407870();
void fn_0040787d();
void fn_0040788a();
void fn_00407897();
void fn_004078a4();
void fn_004078b1();
void fn_0040756e();
void fn_00407580();
void fn_00407595();
void fn_004075aa();

// HYPOTHESIS: the ship list columns, one object per column, constructed in column order;
// the objects live at fixed addresses and are built in place.
extern fn_00406aa6_Obj g_008931c8;
extern fn_00406aa6_Obj g_00893200;
extern fn_00406ba3_Obj g_00893238;
extern fn_00406aa6_Obj g_00893270;
extern fn_00406b63_Obj g_008932a8;
extern fn_00406aa6_Obj g_008932e0;
extern fn_00406b63_Obj g_00893318;
extern fn_00406b63_Obj g_00893350;
extern fn_00406b63_Obj g_00893388;
extern fn_00406aa6_Obj g_008933c0;
extern fn_00406b23_Obj g_008933f8;
extern fn_00406b63_Obj g_00893430;
extern fn_00406b63_Obj g_00893468;
extern fn_00406b63_Obj g_008934a0;
extern fn_00406b63_Obj g_008934d8;
extern fn_00406b63_Obj g_00893510;
extern fn_00406b63_Obj g_00893548;
extern fn_00406b63_Obj g_00893580;
extern fn_00406b63_Obj g_008935b8;
extern fn_00406b63_Obj g_008935f0;
extern fn_00406add_Obj g_00893628;
extern fn_00406add_Obj g_00893660;
extern fn_00406add_Obj g_00893698;
extern fn_00406add_Obj g_008936d0;
extern fn_00406b63_Obj g_00893708;
extern fn_00406aa6_Obj g_00893740;
extern fn_00406aa6_Obj g_00893778;
extern fn_00406add_Obj g_008937b0;
extern fn_00406add_Obj g_008937e8;
extern fn_00406add_Obj g_00893820;
extern fn_00406add_Obj g_00893858;
extern fn_00406add_Obj g_00893890;
extern fn_00406add_Obj g_008938c8;
extern fn_00406add_Obj g_00893900;
extern fn_00406add_Obj g_00893938;
extern fn_00406add_Obj g_00893970;
extern fn_00406add_Obj g_008939a8;
extern fn_00406add_Obj g_008939e0;
extern fn_00406add_Obj g_00893a18;
extern fn_00406add_Obj g_00893a50;
extern fn_00406add_Obj g_00893a88;
extern fn_00406add_Obj g_00893ac0;
extern fn_00406add_Obj g_00893af8;
extern fn_00406add_Obj g_00893b30;
extern fn_00406add_Obj g_00893b68;
extern fn_00406add_Obj g_00893ba0;
extern fn_00406add_Obj g_00893bd8;
extern fn_00406add_Obj g_00893c10;
extern fn_00406add_Obj g_00893c48;
extern fn_00406add_Obj g_00893c80;
extern fn_00406add_Obj g_00893cb8;
extern fn_00406add_Obj g_00893cf0;
extern fn_00406b63_Obj g_00893d28;
extern fn_00406b63_Obj g_00893d60;
extern fn_00406b63_Obj g_00893d98;
extern fn_00406b63_Obj g_00893dd0;
extern fn_00406b63_Obj g_00893e08;
extern fn_00406b63_Obj g_00893e40;
extern fn_00406b63_Obj g_00893e78;
extern fn_00406b63_Obj g_00893eb0;
extern fn_00406b63_Obj g_00893ee8;
extern fn_00406b63_Obj g_00893f20;
extern fn_00406b63_Obj g_00893f58;
extern fn_00406b63_Obj g_00893f90;
extern fn_00406b63_Obj g_00893fc8;
extern fn_00406b63_Obj g_00894000;
extern fn_00406b63_Obj g_00894038;
extern fn_00406b63_Obj g_00894070;
extern fn_00406b63_Obj g_008940a8;
extern fn_00406b63_Obj g_008940e0;
extern fn_00406add_Obj g_00894118;
extern fn_00406add_Obj g_00894150;
extern fn_00406add_Obj g_00894188;
extern fn_00406add_Obj g_008941c0;
extern fn_00406add_Obj g_008941f8;
extern fn_00406add_Obj g_00894230;
extern fn_00406add_Obj g_00894268;
extern fn_00406add_Obj g_008942a0;
extern fn_00406b63_Obj g_008942d8;
extern fn_00406b63_Obj g_00894310;
extern fn_00406b63_Obj g_00894348;
extern fn_00406b63_Obj g_00894380;
extern fn_00406b63_Obj g_008943b8;
extern fn_00406b63_Obj g_008943f0;
extern fn_00406b63_Obj g_00894428;
extern fn_00406aa6_Obj g_00894460;
extern fn_00406b63_Obj g_00894498;
extern fn_00406b63_Obj g_008944d0;
extern fn_00406b63_Obj g_00894508;
extern fn_00406b63_Obj g_00894540;
extern fn_00406b63_Obj g_00894578;
extern fn_00406b63_Obj g_008945b0;
extern fn_00406b63_Obj g_008945e8;
extern fn_00406b63_Obj g_00894620;
extern fn_00406b63_Obj g_00894658;
extern fn_00406b63_Obj g_00894690;
extern fn_00406b63_Obj g_008946c8;
extern fn_00406b63_Obj g_00894700;
extern fn_00406b63_Obj g_00894738;
extern fn_00406b63_Obj g_00894770;
extern fn_00406b63_Obj g_008947a8;
extern fn_00406b63_Obj g_008947e0;
extern fn_00406b63_Obj g_00894818;
extern fn_00406b23_Obj g_00894850;
extern fn_00406ba3_Obj g_00894888;
extern fn_00406ba3_Obj g_008948c0;
extern fn_00406ba3_Obj g_008948f8;
extern fn_00406ba3_Obj g_00894930;
extern fn_00406ba3_Obj g_00894968;

// FUNCTION: STARFLEETOP 0x00406c9b
void fn_00406c9b()
{
    g_008931c8.fn_00406aa6_Obj::fn_00406aa6_Obj("Race", (int)fn_0040a741);
    g_00893200.fn_00406aa6_Obj::fn_00406aa6_Obj("Hull Type", (int)fn_0040a76a);
    g_00893238.fn_00406ba3_Obj::fn_00406ba3_Obj("Class", (int)fn_0040755c);
    g_00893270.fn_00406aa6_Obj::fn_00406aa6_Obj("Class Type", (int)fn_0040a788);
    g_008932a8.fn_00406b63_Obj::fn_00406b63_Obj("BPV", (int)fn_004077a0, 0);
    g_008932e0.fn_00406aa6_Obj::fn_00406aa6_Obj("Special Role", (int)fn_0040a7bb);
    g_00893318.fn_00406b63_Obj::fn_00406b63_Obj("Year First Available", (int)fn_004077ad, 0);
    g_00893350.fn_00406b63_Obj::fn_00406b63_Obj("Year Last Available", (int)fn_004077ba, 0);
    g_00893388.fn_00406b63_Obj::fn_00406b63_Obj("Size Class", (int)fn_004075bf, 4);
    g_008933c0.fn_00406aa6_Obj::fn_00406aa6_Obj("Turn mode", (int)fn_0040a7dc);
    g_008933f8.fn_00406b23_Obj::fn_00406b23_Obj("Move Cost", (int)fn_004075cc, 0.0f);
    g_00893430.fn_00406b63_Obj::fn_00406b63_Obj("HET +2 Turns & Nimble", (int)fn_004075d9, 0);
    g_00893468.fn_00406b63_Obj::fn_00406b63_Obj("HET Breakdown", (int)fn_004075e6, 0);
    g_008934a0.fn_00406b63_Obj::fn_00406b63_Obj("Stealth (+ECM) ", (int)fn_004075f3, 0);
    g_008934d8.fn_00406b63_Obj::fn_00406b63_Obj("Regular Crew", (int)fn_00407600, 0);
    g_00893510.fn_00406b63_Obj::fn_00406b63_Obj("Boarding Parties Base", (int)fn_0040760d, 0);
    g_00893548.fn_00406b63_Obj::fn_00406b63_Obj("Boarding Parties Max", (int)fn_0040761a, 0);
    g_00893580.fn_00406b63_Obj::fn_00406b63_Obj("Deck Crews", (int)fn_00407627, 0);
    g_008935b8.fn_00406b63_Obj::fn_00406b63_Obj("Total Crew", (int)fn_00407634, 0);
    g_008935f0.fn_00406b63_Obj::fn_00406b63_Obj("Min Crew", (int)fn_00407641, 0);
    g_00893628.fn_00406add_Obj::fn_00406add_Obj("Shield 1", (int)fn_0040a7fd, 1, 0);
    g_00893660.fn_00406add_Obj::fn_00406add_Obj("Shield 2 _ 6", (int)fn_0040a7fd, 2, 6);
    g_00893698.fn_00406add_Obj::fn_00406add_Obj("Shield 3 _ 5", (int)fn_0040a7fd, 3, 5);
    g_008936d0.fn_00406add_Obj::fn_00406add_Obj("Shield 4", (int)fn_0040a7fd, 4, 0);
    g_00893708.fn_00406b63_Obj::fn_00406b63_Obj("Cloak", (int)fn_0040764e, 0);
    g_00893740.fn_00406aa6_Obj::fn_00406aa6_Obj("Num", (int)fn_0040a834);
    g_00893778.fn_00406aa6_Obj::fn_00406aa6_Obj("Arc", (int)fn_0040a870);
    g_008937b0.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 1", (int)fn_0040a84b, 1, 0);
    g_008937e8.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 2", (int)fn_0040a84b, 2, 0);
    g_00893820.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 3", (int)fn_0040a84b, 3, 0);
    g_00893858.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 4", (int)fn_0040a84b, 4, 0);
    g_00893890.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 5", (int)fn_0040a84b, 5, 0);
    g_008938c8.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 6", (int)fn_0040a84b, 6, 0);
    g_00893900.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 7", (int)fn_0040a84b, 7, 0);
    g_00893938.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 8", (int)fn_0040a84b, 8, 0);
    g_00893970.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 9", (int)fn_0040a84b, 9, 0);
    g_008939a8.fn_00406add_Obj::fn_00406add_Obj("Heavy Weapon 10", (int)fn_0040a84b, 10, 0);
    g_008939e0.fn_00406add_Obj::fn_00406add_Obj("Weapon 11", (int)fn_0040a84b, 11, 0);
    g_00893a18.fn_00406add_Obj::fn_00406add_Obj("Weapon 12", (int)fn_0040a84b, 12, 0);
    g_00893a50.fn_00406add_Obj::fn_00406add_Obj("Weapon 13", (int)fn_0040a84b, 13, 0);
    g_00893a88.fn_00406add_Obj::fn_00406add_Obj("Weapon 14", (int)fn_0040a84b, 14, 0);
    g_00893ac0.fn_00406add_Obj::fn_00406add_Obj("Weapon 15", (int)fn_0040a84b, 15, 0);
    g_00893af8.fn_00406add_Obj::fn_00406add_Obj("Weapon 16", (int)fn_0040a84b, 16, 0);
    g_00893b30.fn_00406add_Obj::fn_00406add_Obj("Weapon 17", (int)fn_0040a84b, 17, 0);
    g_00893b68.fn_00406add_Obj::fn_00406add_Obj("Weapon 18", (int)fn_0040a84b, 18, 0);
    g_00893ba0.fn_00406add_Obj::fn_00406add_Obj("Weapon 19", (int)fn_0040a84b, 19, 0);
    g_00893bd8.fn_00406add_Obj::fn_00406add_Obj("Weapon 20", (int)fn_0040a84b, 20, 0);
    g_00893c10.fn_00406add_Obj::fn_00406add_Obj("Weapon 21", (int)fn_0040a84b, 21, 0);
    g_00893c48.fn_00406add_Obj::fn_00406add_Obj("Weapon 22", (int)fn_0040a84b, 22, 0);
    g_00893c80.fn_00406add_Obj::fn_00406add_Obj("Weapon 23", (int)fn_0040a84b, 23, 0);
    g_00893cb8.fn_00406add_Obj::fn_00406add_Obj("Weapon 24", (int)fn_0040a84b, 24, 0);
    g_00893cf0.fn_00406add_Obj::fn_00406add_Obj("Weapon 25", (int)fn_0040a84b, 25, 0);
    g_00893d28.fn_00406b63_Obj::fn_00406b63_Obj("Probes", (int)fn_0040765b, 0);
    g_00893d60.fn_00406b63_Obj::fn_00406b63_Obj("T Bombs Base", (int)fn_00407668, 0);
    g_00893d98.fn_00406b63_Obj::fn_00406b63_Obj("T Bombs Max", (int)fn_00407675, 0);
    g_00893dd0.fn_00406b63_Obj::fn_00406b63_Obj("Nuclear Mine Base", (int)fn_00407682, 0);
    g_00893e08.fn_00406b63_Obj::fn_00406b63_Obj("Nuclear Mine Max", (int)fn_0040768f, 0);
    g_00893e40.fn_00406b63_Obj::fn_00406b63_Obj("Drone Control", (int)fn_00407863, 0);
    g_00893e78.fn_00406b63_Obj::fn_00406b63_Obj("ADD_6", (int)fn_0040769c, 0);
    g_00893eb0.fn_00406b63_Obj::fn_00406b63_Obj("ADD_12", (int)fn_004076a9, 0);
    g_00893ee8.fn_00406b63_Obj::fn_00406b63_Obj("Shuttles Size", (int)fn_004076b6, 0);
    g_00893f20.fn_00406b63_Obj::fn_00406b63_Obj("Launch Rate", (int)fn_004076c3, 0);
    g_00893f58.fn_00406b63_Obj::fn_00406b63_Obj("General Base", (int)fn_004076d0, 0);
    g_00893f90.fn_00406b63_Obj::fn_00406b63_Obj("General Max", (int)fn_004076dd, 0);
    g_00893fc8.fn_00406b63_Obj::fn_00406b63_Obj("Suicide Base", (int)fn_00407704, 0);
    g_00894000.fn_00406b63_Obj::fn_00406b63_Obj("Suicide Max", (int)fn_00407711, 0);
    g_00894038.fn_00406b63_Obj::fn_00406b63_Obj("ScatterPack Base", (int)fn_004076ea, 0);
    g_00894070.fn_00406b63_Obj::fn_00406b63_Obj("ScatterPack Max", (int)fn_004076f7, 0);
    g_008940a8.fn_00406b63_Obj::fn_00406b63_Obj("Wild Weasel Base", (int)fn_0040771e, 0);
    g_008940e0.fn_00406b63_Obj::fn_00406b63_Obj("Wild Weasel Max", (int)fn_0040772b, 0);
    g_00894118.fn_00406add_Obj::fn_00406add_Obj("Fighter Bay 1", (int)fn_0040a8c1, 0, 0);
    g_00894150.fn_00406add_Obj::fn_00406add_Obj("Fighter Type 1", (int)fn_0040a8e0, 0, 0);
    g_00894188.fn_00406add_Obj::fn_00406add_Obj("Fighter Bay 2", (int)fn_0040a8c1, 1, 0);
    g_008941c0.fn_00406add_Obj::fn_00406add_Obj("Fighter Type 2", (int)fn_0040a8e0, 1, 0);
    g_008941f8.fn_00406add_Obj::fn_00406add_Obj("Fighter Bay 3", (int)fn_0040a8c1, 2, 0);
    g_00894230.fn_00406add_Obj::fn_00406add_Obj("Fighter Type 3", (int)fn_0040a8e0, 2, 0);
    g_00894268.fn_00406add_Obj::fn_00406add_Obj("Fighter Bay 4", (int)fn_0040a8c1, 3, 0);
    g_008942a0.fn_00406add_Obj::fn_00406add_Obj("Fighter Type 4", (int)fn_0040a8e0, 3, 0);
    g_008942d8.fn_00406b63_Obj::fn_00406b63_Obj("Armor", (int)fn_00407738, 0);
    g_00894310.fn_00406b63_Obj::fn_00406b63_Obj("Forward Hull", (int)fn_00407745, 0);
    g_00894348.fn_00406b63_Obj::fn_00406b63_Obj("Center Hull", (int)fn_00407752, 0);
    g_00894380.fn_00406b63_Obj::fn_00406b63_Obj("Aft Hull", (int)fn_0040775f, 0);
    g_008943b8.fn_00406b63_Obj::fn_00406b63_Obj("Cargo", (int)fn_0040776c, 0);
    g_008943f0.fn_00406b63_Obj::fn_00406b63_Obj("Barracks", (int)fn_00407779, 0);
    g_00894428.fn_00406b63_Obj::fn_00406b63_Obj("Repair", (int)fn_00407786, 0);
    g_00894460.fn_00406aa6_Obj::fn_00406aa6_Obj("R_L_Warp", (int)fn_0040a921);
    g_00894498.fn_00406b63_Obj::fn_00406b63_Obj("C_Warp", (int)fn_00407793, 0);
    g_008944d0.fn_00406b63_Obj::fn_00406b63_Obj("Impulse", (int)fn_004077c7, 0);
    g_00894508.fn_00406b63_Obj::fn_00406b63_Obj("Apr", (int)fn_004077d4, 0);
    g_00894540.fn_00406b63_Obj::fn_00406b63_Obj("Battery", (int)fn_004077e1, 0);
    g_00894578.fn_00406b63_Obj::fn_00406b63_Obj("Bridge", (int)fn_004077ee, 0);
    g_008945b0.fn_00406b63_Obj::fn_00406b63_Obj("Security", (int)fn_004077fb, 0);
    g_008945e8.fn_00406b63_Obj::fn_00406b63_Obj("Lab", (int)fn_00407808, 0);
    g_00894620.fn_00406b63_Obj::fn_00406b63_Obj("Transporters", (int)fn_00407815, 0);
    g_00894658.fn_00406b63_Obj::fn_00406b63_Obj("Tractors", (int)fn_00407822, 0);
    g_00894690.fn_00406b63_Obj::fn_00406b63_Obj("Mech Tractors", (int)fn_0040782f, 0);
    g_008946c8.fn_00406b63_Obj::fn_00406b63_Obj("Special Sensors", (int)fn_0040783c, 0);
    g_00894700.fn_00406b63_Obj::fn_00406b63_Obj("Sensors", (int)fn_00407849, 0);
    g_00894738.fn_00406b63_Obj::fn_00406b63_Obj("Scanners", (int)fn_00407856, 0);
    g_00894770.fn_00406b63_Obj::fn_00406b63_Obj("Explosion Strength", (int)fn_00407870, 0);
    g_008947a8.fn_00406b63_Obj::fn_00406b63_Obj("Acceleration", (int)fn_0040787d, 0);
    g_008947e0.fn_00406b63_Obj::fn_00406b63_Obj("Damage Control", (int)fn_0040788a, 0);
    g_00894818.fn_00406b63_Obj::fn_00406b63_Obj("Extra Damage", (int)fn_00407897, 0);
    g_00894850.fn_00406b23_Obj::fn_00406b23_Obj("Ship Cost", (int)fn_004078a4, 0.0f);
    g_00894888.fn_00406ba3_Obj::fn_00406ba3_Obj("Refit Base Class", (int)fn_004078b1);
    g_008948c0.fn_00406ba3_Obj::fn_00406ba3_Obj("Geometry", (int)fn_0040756e);
    g_008948f8.fn_00406ba3_Obj::fn_00406ba3_Obj("UI", (int)fn_00407580);
    g_00894930.fn_00406ba3_Obj::fn_00406ba3_Obj("Full Name", (int)fn_00407595);
    g_00894968.fn_00406ba3_Obj::fn_00406ba3_Obj("refits", (int)fn_004075aa);
}
