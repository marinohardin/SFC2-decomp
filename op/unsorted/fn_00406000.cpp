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

// FUNCTION: STARFLEETOP 0x004060d8
const std::locale::facet* fn_0040673f(const std::locale::facet* pf);

// HYPOTHESIS: the original is the library's use_facet instantiation for numpunct<char>.
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
        _Pf = _Psave = fn_0040673f(new std::numpunct<char>);
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

// FUNCTION: STARFLEETOP 0x00406355
const std::locale::facet* fn_0040678d(const std::locale::facet* pf);

// HYPOTHESIS: the library's use_facet instantiation for collate<char>.
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
        _Pf = _Psave = fn_0040678d(new std::collate<char>);
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

// HYPOTHESIS: the library's compare(const string&) instantiation.
// FUNCTION: STARFLEETOP 0x0040651e
int std::string::compare(const std::string& _X) const
{
    return (compare(0, _Len, _X.c_str(), _X.size()));
}

// HYPOTHESIS: a caller in the original uses this instantiation.
int (std::string::*g_fn_0040651e)(const std::string&) const = &std::string::compare;

// HYPOTHESIS: the library's istreambuf_iterator<char>::equal instantiation; 0x00405fdf is _Peek.
// FUNCTION: STARFLEETOP 0x00406540
template<> bool std::istreambuf_iterator<char>::equal(const std::istreambuf_iterator<char>& _X) const
{
    if (!_Got)
        ((std::istreambuf_iterator<char> *)this)->_Peek();
    if (!_X._Got)
        ((std::istreambuf_iterator<char> *)&_X)->_Peek();
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

// FUNCTION: STARFLEETOP 0x004065d3
// HYPOTHESIS: the library's codecvt<char,char,int>::do_in instantiation (identity conversion, returns noconv).
template<> std::codecvt_base::result std::codecvt<char, char, int>::do_in(int&, const char* _F1, const char*, const char*& _M1, char* _F2, char*, char*& _M2) const
{
    _M1 = _F1;
    _M2 = _F2;
    return (noconv);
}

// HYPOTHESIS: a caller in the original instantiates the facet, which emits its vtable and do_in.
std::codecvt<char, char, int> g_fn_004065d3;

// FUNCTION: STARFLEETOP 0x004065ed
// HYPOTHESIS: the library's codecvt<char,char,int>::do_length instantiation (identity conversion, min of available and requested).
template<> int std::codecvt<char, char, int>::do_length(int&,const char* _F1, const char* _L1, size_t _N2) const
{
    size_t _N = _L1 - _F1;
    if (_N2 < _N)
        _N = _N2;
    return (int)_N;
}

// FUNCTION: STARFLEETOP 0x004066a3
// HYPOTHESIS: the facet-like object whose reference count 0x00402b90 increments; declared in
// full only in fn_00402000.cpp, so only the member used here is named.
class fn_00406887_Base
{
public:
    virtual ~fn_00406887_Base();
};

class fn_00402b90_Obj
{
public:
    void fn_00402b90();
    fn_00406887_Base* fn_00402b3a();
};

extern fn_00402b90_Obj* g_00893184;
void fn_00406887();

// HYPOTHESIS: registers a library-owned facet: remember it, take a reference and arrange its release at exit.
fn_00402b90_Obj* fn_004066a3(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_00893184 = p;
    p->fn_00402b90();
    atexit(fn_00406887);
    return p;
}

// FUNCTION: STARFLEETOP 0x004066f1
extern fn_00402b90_Obj* g_00893188;
void fn_004068d4();

// HYPOTHESIS: the same registration as 0x004066a3, for a second library-owned facet.
fn_00402b90_Obj* fn_004066f1(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_00893188 = p;
    p->fn_00402b90();
    atexit(fn_004068d4);
    return p;
}

// FUNCTION: STARFLEETOP 0x0040673f
extern fn_00402b90_Obj* g_0089318c;
void fn_004068e0();

// HYPOTHESIS: the same registration as 0x004066a3, for a third library-owned facet.
fn_00402b90_Obj* fn_0040673f(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_0089318c = p;
    p->fn_00402b90();
    atexit(fn_004068e0);
    return p;
}

// FUNCTION: STARFLEETOP 0x0040678d
extern fn_00402b90_Obj* g_00893190;
void fn_004068ec();

// HYPOTHESIS: the same facet registration as 0x004066a3, for a fourth library-owned facet.
fn_00402b90_Obj* fn_0040678d(fn_00402b90_Obj* p)
{
    std::_Lockit lock;
    g_00893190 = p;
    p->fn_00402b90();
    atexit(fn_004068ec);
    return p;
}

// FUNCTION: STARFLEETOP 0x004067db
int std::string::compare(size_type _P0, size_type _N0, const char* _S, size_type _M) const
{
    if (_Len < _P0)
        _Xran();
    if (_Len - _P0 < _N0)
        _N0 = _Len - _P0;
    size_type _Ans = std::char_traits<char>::compare(_Psum(_Ptr, _P0), _S, _N0 < _M ? _N0 : _M);
    return (_Ans != 0 ? _Ans : _N0 < _M ? -1 : _N0 != _M);
}

// FUNCTION: STARFLEETOP 0x0040683e
void std::numpunct<char>::_Init(const std::_Locinfo& _Lobj)
{
    const lconv* _Ptr = _Lobj._Getlconv();
    _Dp = _Ptr->decimal_point[0];
    _Ks = _Ptr->thousands_sep[0];
    _Gr = _Maklocstr(_Ptr->grouping, (char*)0);
    _Nf = _Maklocstr("false", (char*)0);
    _Nt = _Maklocstr("true", (char*)0);
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
