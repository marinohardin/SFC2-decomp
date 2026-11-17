#include <ios>

// HYPOTHESIS: a stream holder whose ifstream part sits at +0x5c. The exe calls
// ~basic_ifstream (0x0040345f) on that part, then ~basic_ios (0x00402a1b) on the same.
class fn_004021b8_Part
{
public:
    void fn_0040345f();
};

class fn_004021b8_Stream
{
public:
    char pad[0x5c];
    void fn_004021b8();
};

// FUNCTION: STARFLEETOP 0x004021b8
void fn_004021b8_Stream::fn_004021b8()
{
    std::basic_ios<char>* p = (std::basic_ios<char>*)(pad + 0x5c);
    ((fn_004021b8_Part*)p)->fn_0040345f();
    p->~basic_ios();
}

// HYPOTHESIS: an object with a dword at +8 that this method clears.
class fn_004021cc_Object
{
public:
    int pad[2];
    int m08;
    void fn_004021cc();
};

// FUNCTION: STARFLEETOP 0x004021cc
void fn_004021cc_Object::fn_004021cc()
{
    m08 = 0;
}
