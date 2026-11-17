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

#include <string.h>

// HYPOTHESIS: the library's std::string::append(size_type, char), written out as a stand-in
// class with the same layout (allocator at +0, pointer at +4, length at +8, reserve at +0xc).
namespace std
{
void _Xlen();
}

class fn_00403af0_String
{
public:
    fn_00403af0_String& append(unsigned int n, char c);
    bool _Grow(unsigned int n, bool trim);
    static const unsigned int npos;
    char _Alloc;
    char* _Ptr;
    unsigned int _Len;
    unsigned int _Res;
};

// FUNCTION: STARFLEETOP 0x00403af0
fn_00403af0_String& fn_00403af0_String::append(unsigned int n, char c)
{
    if (npos - _Len <= n)
        std::_Xlen();
    if (n > 0)
    {
        unsigned int newLen = n + _Len;
        if (_Grow(newLen, false))
        {
            memset(_Ptr + _Len, c, n);
            _Len = newLen;
            _Ptr[newLen] = 0;
        }
    }
    return *this;
}
