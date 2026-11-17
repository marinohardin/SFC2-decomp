#include <string>

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
