// Unit:      unsorted/fn_00460000.cpp
// Functions: 0x00460175 to 0x00460ede, 30 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: the object at 0x0090e00c has a virtual destructor in vtable slot 0;
// its class is not identified yet.
class fn_0090e00c_class
{
public:
    virtual ~fn_0090e00c_class();
};

extern fn_0090e00c_class *g_0090e00c;

// FUNCTION: STARFLEETOP 0x00460175
void fn_00460175()
{
    if (g_0090e00c != 0)
    {
        delete g_0090e00c;
        g_0090e00c = 0;
    }
}

#include "unsorted/tObject_0090b730.h"

// HYPOTHESIS: the object at 0x0090e008; only the non-virtual call below is known.
class fn_0090e008_class
{
public:
    void fn_005d46d3();
};

extern fn_0090e008_class *g_0090e008;

// HYPOTHESIS: the object at +0x100 has a virtual taking one argument in vtable slot 10.
class fn_0046018d_member_class
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10(long);
};

// HYPOTHESIS: the owning class is not identified; members are named by offset.
class fn_0046018d_class
{
public:
    void fn_0046018d();

    char pad_000[0xa4];
    long m_a4;
    char pad_0a8[0xe4 - 0xa8];
    char m_e4;
    char pad_0e5[0x100 - 0xe5];
    fn_0046018d_member_class *m_100;
};

// FUNCTION: STARFLEETOP 0x0046018d
void fn_0046018d_class::fn_0046018d()
{
    m_a4 = 0;
    m_e4 = 0;
    if (g_0090e008 != 0)
    {
        m_a4 = 6;
        if (m_100 != 0)
        {
            m_100->vfunction10(1);
        }
        m_100 = 0;
        g_0090e008->fn_005d46d3();
    }
    gObject_0090b730.fn_0060ca42(0);
}

#include <string>

// HYPOTHESIS: the owning class is not identified; the text at +0xac is a std::string
// (assigned through std::string::assign at 0x0040428e).
class fn_004601db_class
{
public:
    std::string fn_004601db(const std::string &arg);
    std::string fn_00460216(const std::string &arg);

    char pad_000[0xac];
    std::string m_ac;
};

// FUNCTION: STARFLEETOP 0x004601db
std::string fn_004601db_class::fn_004601db(const std::string &arg)
{
    m_ac = "";
    return fn_00460216(arg);
}

// FUNCTION: STARFLEETOP 0x00460216
std::string fn_004601db_class::fn_00460216(const std::string &arg)
{
    m_ac = m_ac + "\n" + arg;
    return m_ac;
}

#include <time.h>
#include "unsorted/fn_004d290d_object.h"

// HYPOTHESIS: a free function that takes a panel name and a rectangle (the same call is made
// from tPanelMgr.cpp); 0x0084411c holds a pointer to the panel name "Big Yes No".
void fn_004d31ed(void *arg, int a, int b, int width, int height);
extern void *g_0084411c;

// HYPOTHESIS: the owning class is not identified; members are named by offset. The text at
// +0xbc is a std::string (assigned through std::string::assign at 0x00403b49), and the
// times at +0xd0 and +0xd8 are doubles.
class fn_004602b2_class
{
public:
    void fn_004602b2(const std::string &text, int seconds, bool exit);
    void fn_0046052c();
    double fn_0046034f();

    char pad_000[0x2c];
    fn_004d290d_object m_2c;
    char pad_090[0xbc - 0x90];
    std::string m_bc;
    char pad_0cc[0xd0 - 0xcc];
    double m_d0;
    double m_d8;
    long m_e0;
};

// FUNCTION: STARFLEETOP 0x004602b2
void fn_004602b2_class::fn_004602b2(const std::string &text, int seconds, bool exit)
{
    m_bc = text;
    m_d0 = time(0);
    m_e0 = 0;
    m_d8 = seconds;
    fn_0046052c();
    fn_004d31ed(g_0084411c, 0, 0, 0x280, 0x1e0);
    m_2c.fn_004d269b(g_0084411c, "AcceptYesNo", false);
    m_2c.fn_004d269b(g_0084411c, "ExitYesNo", exit);
    fn_0046034f();
}
