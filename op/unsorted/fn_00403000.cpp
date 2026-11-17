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
