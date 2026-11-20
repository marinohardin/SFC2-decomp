#include <fstream>

// The library's std::ifstream constructor taking (name, mode); the exe names its vtable
// 0x00795704 basic_ifstream<char>.
// FUNCTION: STARFLEETOP 0x004033ab
std::ifstream::ifstream(const char* _S, std::ios_base::openmode _M)
    : std::basic_istream<char, std::char_traits<char> >(&_Fb)
{
    if (_Fb.open(_S, _M | in) == 0)
        setstate(failbit);
}

// FUNCTION: STARFLEETOP 0x0040345f
std::ifstream::~ifstream()
{
}

// HYPOTHESIS: a caller in the original constructs a std::ifstream and so uses this instantiation.
template class std::basic_ifstream<char, std::char_traits<char> >;

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

// HYPOTHESIS: the library's string::find(const char*, pos, n) instance, written on a stand-in
// class of the string's layout (allocator byte, pointer, length, reserve).
class fn_00403d05_String
{
public:
    size_t fn_00403d05(const char* _S, size_t _P, size_t _N) const;
    char _A;
    char* _Ptr;
    size_t _Len;
    size_t _Res;
};

// FUNCTION: STARFLEETOP 0x00403d05
size_t fn_00403d05_String::fn_00403d05(const char* _S, size_t _P, size_t _N) const
{
    size_t _Nm;
    if (_N == 0 && _P <= _Len)
        return (_P);
    else if (_P < _Len && _N <= (_Nm = _Len - _P))
    {
        const char *_U, *_V;
        for (_Nm -= _N - 1, _V = _Ptr + _P;
            (_U = std::char_traits<char>::find(_V, _Nm, *_S)) != 0; _Nm -= _U - _V + 1, _V = _U + 1)
            if (std::char_traits<char>::compare(_U, _S, _N) == 0)
                return (_U - _Ptr);
    }
    return (std::string::npos);
}

// HYPOTHESIS: a caller in the original uses this instantiation.
std::string::size_type (std::string::*g_fn_00403d05)(const char*, std::string::size_type, std::string::size_type) const = &std::string::find;

// FUNCTION: STARFLEETOP 0x00403d87
void std::string::_Freeze()
{
    if (_Ptr != 0 && _Refcnt(_Ptr) != 0)
        _Grow(_Len);
    if (_Ptr != 0)
        _Refcnt(_Ptr) = 0;
}

// FUNCTION: STARFLEETOP 0x00403dae
bool std::string::_Grow(size_type _N, bool _Trim)
{
    if (max_size() < _N)
        _Xlen();
    if (_Trim)
        _Len = 0;
    else if (_N < _Len)
        _Len = _N;
    if (_Ptr == 0 || _Refcnt(_Ptr) == 0)
    {
        if (_N == 0)
        {
            if (_Trim)
            {
                _Tidy(true);
                return (false);
            }
            if (_Ptr == 0)
                return (false);
            _Len = 0;
            _Eos(0);
            return (false);
        }
        if (_Trim)
        {
            if (_Res <= 0x1f && _Res >= _N)
                return (true);
            _Tidy(true);
        }
        else if (_Res >= _N)
            return (true);
    }
    else if (_N == 0)
    {
        _Tidy(true);
        return (false);
    }
    return (_Copy(_N), true);
}

// The library's fpos<int> constructor: the exe names 0x00c81938 fpos<int>::_Stz.














// FUNCTION: STARFLEETOP 0x00403e38
std::fpos<int>::fpos(std::streamoff _O)
    : _Off(_O), _Fpos(_Fpz)
{
    _Lockit _Lk;
    _State = _Stz;
}

// FUNCTION: STARFLEETOP 0x00403e72
std::istream::istream(std::streambuf* _Strbuf, bool _Isstd)
    : _Chcount(0)
{
    init(_Strbuf, _Isstd);
}

// HYPOTHESIS: the library's streambuf::sbumpc instance, written on a stand-in class derived
// from the stream buffer so that it reaches its protected members.
class fn_00403ff1_Streambuf : public std::streambuf
{
public:
    int_type fn_00403ff1();
};

// FUNCTION: STARFLEETOP 0x00403ff1
std::streambuf::int_type fn_00403ff1_Streambuf::fn_00403ff1()
{
    return (gptr() != 0 && gptr() < egptr()
        ? std::char_traits<char>::to_int_type(*_Gninc())
        : uflow());
}
