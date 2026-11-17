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
// HYPOTHESIS: a pattern matcher over an input stream iterator; the first character of the
// pattern is the escape that introduces a one-character class.
int fn_0040619c(std::istreambuf_iterator<char>& _First, const std::istreambuf_iterator<char>& _Last, unsigned int _N, const char* _Pat)
{
    for (const char* _Q = _Pat; *_Q != 0; ++_Q)
        if (*_Q == *_Pat)
            ++_N;
    std::string _Str;
    _Str.reserve(_N);
    unsigned int _Cnt = 1;
    int _Res = -2;
    bool _Found = false;
    for (;;)
    {
        _Found = false;
        unsigned int _I;
        unsigned int _J = 0;
        for (_I = 0; _I < _N; ++_I)
        {
            while (_Pat[_J] != 0 && _Pat[_J] != *_Pat)
                ++_J;
            if (_Str[_I] != 0)
            {
                _J += _Str[_I];
            }
            else
            {
                _J += _Cnt;
                if (_Pat[_J] != *_Pat && _Pat[_J] != 0)
                {
                    if (!_First.equal(_Last) && *_First == _Pat[_J])
                    {
                        _Found = true;
                        continue;
                    }
                    _Cnt = _Cnt < 0x7f ? _Cnt : 0x7f;
                }
                else
                {
                    _Cnt = _Cnt < 0x7f ? _Cnt : 0x7f;
                    _Res = _I;
                }
            }
        }
        if (!_Found || _First.equal(_Last))
            break;
        ++_Cnt;
        ++_First;
        _Res = -1;
    }
    return _Res;
}
