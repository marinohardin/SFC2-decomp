#include <fstream>

// HYPOTHESIS: the library's std::ifstream constructor taking (name, mode), written out as a
// stand-in class with the same layout (filebuf at +8, virtual ios base at +0x5c).
class fn_004033ab_Ifstream : public std::istream
{
public:
    fn_004033ab_Ifstream(const char* name, int mode);
    ~fn_004033ab_Ifstream();
    std::filebuf _Fb;
};

// FUNCTION: STARFLEETOP 0x004033ab
fn_004033ab_Ifstream::fn_004033ab_Ifstream(const char* name, int mode)
    : std::basic_istream<char, std::char_traits<char> >(&_Fb), _Fb()
{
    if (_Fb.open(name, mode | in) == 0)
        setstate(failbit);
}

// FUNCTION: STARFLEETOP 0x0040345f
fn_004033ab_Ifstream::~fn_004033ab_Ifstream()
{
}

// FUNCTION: STARFLEETOP 0x004034a5
std::filebuf::~filebuf()
{
    if (_Closef)
        close();
    delete _Str;
}

#include <string>

// FUNCTION: STARFLEETOP 0x00403af0
std::string& std::string::append(size_type _M, char _C)
{
    if (npos - _Len <= _M)
        _Xlen();
    size_type _N;
    if (0 < _M && _Grow(_N = _Len + _M))
    {
        std::char_traits<char>::assign(_Ptr + _Len, _M, _C);
        _Eos(_N);
    }
    return (*this);
}

// FUNCTION: STARFLEETOP 0x00403b49
std::string& std::string::assign(const std::string& _X, size_type _P, size_type _M)
{
    if (_X.size() < _P)
        _Xran();
    size_type _N = _X.size() - _P;
    if (_M < _N)
        _N = _M;
    if (this == &_X)
        erase((size_type)(_P + _N)), erase(0, _P);
    else if (0 < _N && _N == _X.size() && _Refcnt(_X.c_str()) < (char)_FROZEN) // MATCH-ONLY: the exe compares the byte against -1 as a signed int
    {
        _Tidy(true);
        _Ptr = (char*)_X.c_str();
        _Len = _X.size();
        _Res = _X.capacity();
        ++_Refcnt(_Ptr);
    }
    else if (_Grow(_N, true))
    {
        std::char_traits<char>::copy(_Ptr, &_X.c_str()[_P], _N);
        _Eos(_N);
    }
    return (*this);
}

// HYPOTHESIS: a caller in the original (e.g. string::operator=) references this instantiation.
std::string& (std::string::*g_fn_00403b49)(const std::string&, std::string::size_type, std::string::size_type) = &std::string::assign;

// FUNCTION: STARFLEETOP 0x00403c1a
std::string& std::string::insert(size_type _P0, size_type _M, char _C)
{
    if (_Len < _P0)
        _Xran();
    if (npos - _Len <= _M)
        _Xlen();
    size_type _N;
    if (0 < _M && _Grow(_N = _Len + _M))
    {
        std::char_traits<char>::move(_Ptr + _P0 + _M, _Ptr + _P0, _Len - _P0);
        std::char_traits<char>::assign(_Ptr + _P0, _M, _C);
        _Eos(_N);
    }
    return (*this);
}

// HYPOTHESIS: a caller in the original uses this instantiation.
std::string& (std::string::*g_fn_00403c1a)(std::string::size_type, std::string::size_type, char) = &std::string::insert;

// FUNCTION: STARFLEETOP 0x00403c99
std::string& std::string::erase(size_type _P0, size_type _M)
{
    if (_Len < _P0)
        _Xran();
    if (_Len - _P0 < _M)
        _M = _Len - _P0;
    if (0 < _M)
    {
        _Freeze();
        std::char_traits<char>::move(_Ptr + _P0, _Ptr + _P0 + _M, _Len - _P0 - _M);
        size_type _N;
        if (_Grow(_N = _Len - _M))
            _Eos(_N);
    }
    return (*this);
}

// HYPOTHESIS: a caller in the original uses this instantiation.
std::string& (std::string::*g_fn_00403c99)(std::string::size_type, std::string::size_type) = &std::string::erase;
