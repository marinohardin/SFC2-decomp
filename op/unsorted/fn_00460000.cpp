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
