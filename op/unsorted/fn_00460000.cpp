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

class fn_005e8f25_object;

// HYPOTHESIS: the object at 0x0090e008; only the non-virtual calls below are known.
class fn_0090e008_class
{
public:
    void fn_005d46d3();
    fn_005e8f25_object *fn_005e8f25();
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
void fn_004d31ed(const char *arg, int a, int b, int width, int height);
extern char *g_0084411c;

// HYPOTHESIS: the owning class is not identified; members are named by offset. The text at
// +0xbc is a std::string (assigned through std::string::assign at 0x00403b49), and the
// times at +0xd0 and +0xd8 are doubles.
class fn_004602b2_class
{
public:
    void fn_004602b2(const std::string &text, int seconds, bool exit);
    void fn_0046052c();
    double fn_0046034f();
    void fn_00460539(const std::string &text, const std::string &name);
    void fn_00460610();

    char pad_000[0x2c];
    fn_004d290d_object m_2c;
    bool m_90;
    std::string m_94;
    char pad_0a4[0xbc - 0xa4];
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

#include <sstream>
#include "unsorted/fn_004d288f_object.h"

// HYPOTHESIS: the object fn_005e8f25 returns; a std::string sits at +0x48.
class fn_005e8f25_object
{
public:
    char pad_000[0x48];
    std::string m_48;
};

// HYPOTHESIS: the text pieces of the countdown line; their contents are not recovered.
extern const char g_00843190[];
extern const char g_00844538[];
extern const char g_0083b8cc[];
extern const char g_008443b0[];

// The original calls the string inserter out of line (0x00404e8a, the SDK STL's sputc loop);
// the stock STL's inserter inlines here, which shifts the registers of the stream code.
// FUNCTION: STARFLEETOP 0x0046034f
double fn_004602b2_class::fn_0046034f()
{
    double remaining = 0;
    if (m_d8 != 0)
    {
        double elapsed = difftime(time(0), (time_t)m_d0);
        remaining = m_d8 - elapsed;
        if (remaining <= 0)
        {
            remaining = 0;
        }
        if (remaining == 0)
        {
            fn_0046052c();
        }
        else
        {
            long seconds = (long)remaining;
            if (seconds != m_e0)
            {
                m_e0 = seconds;
                long minutes = seconds / 60;
                seconds = seconds % 60;
                std::stringstream stream;
                stream << g_0090e008->fn_005e8f25()->m_48 << g_00843190 << minutes << g_00844538
                       << (seconds < 10 ? "0" : "") << seconds << g_0083b8cc << m_bc;
                m_2c.fn_004d288f(g_008443b0)->fn_004d2573(stream.str());
            }
        }
    }
    else
    {
        m_2c.fn_004d288f(g_008443b0)->fn_004d2573(m_bc);
    }
    return remaining;
}

// HYPOTHESIS: closes the panel whose name 0x0084411c points to (the same call is made from
// tPanelMgr.cpp).
void fn_004d32c4(const char *arg);

// FUNCTION: STARFLEETOP 0x0046052c
void fn_004602b2_class::fn_0046052c()
{
    fn_004d32c4(g_0084411c);
}

// HYPOTHESIS: 0x00844120 holds a pointer to a panel name, like 0x0084411c; 0x008443c0 is passed
// by address (as in tPanelMgr.cpp).
extern char *g_00844120;
extern const char g_008443c0[];

// FUNCTION: STARFLEETOP 0x00460539
void fn_004602b2_class::fn_00460539(const std::string &text, const std::string &name)
{
    m_90 = true;
    if (name.empty())
    {
        fn_004d31ed(g_00844120, 0, 0, 0x280, 0x1e0);
        m_2c.fn_004d288f(g_008443c0)->fn_004d2573(text);
        m_94 = g_00844120;
    }
    else
    {
        fn_004d31ed(name.c_str(), 0, 0, 0x280, 0x1e0);
        m_94 = name;
    }
}

// FUNCTION: STARFLEETOP 0x00460610
void fn_004602b2_class::fn_00460610()
{
    m_90 = false;
    fn_004d32c4(m_94.c_str());
}

#include "Common/DateAndTime.h"

// HYPOTHESIS: declared here until its own unit has a header; only the size (0x168, from the
// stack layout below) and the destructor at 0x0042ceae are known.
class tCampaignInfo
{
public:
    ~tCampaignInfo();

    char pad_000[0x168];
};

// HYPOTHESIS: a 20-byte value built from a text and a number; it is copied as plain data.
class fn_006cf2d0_object
{
public:
    fn_006cf2d0_object(const std::string &text, long value);

    char pad_000[0x14];
};

// HYPOTHESIS: the record built at 0x0045e5f7; its members are placed by their stack offsets.
class fn_0060a827_object;
class fn_0045e5f7_object
{
public:
    fn_0045e5f7_object();
    void fn_00460736(fn_0060a827_object *arg);

    tCampaignInfo m_000;
    tDateAndTime m_168;
    fn_006cf2d0_object m_178;
};

class fn_0060a827_object
{
public:
    const std::string &fn_0054a8d4();
};

// HYPOTHESIS: the argument of 0x0046062e; only these calls on it are known.
class fn_0046062e_arg
{
public:
    fn_0060a827_object *fn_0060a827();
    std::string fn_005db8d3();
    long fn_005db919();
};

// HYPOTHESIS: the member at +0xec of the owning class keeps the records.
class fn_00460c0c_list
{
public:
    void fn_00460c0c(const fn_0045e5f7_object &record);
};

extern char *g_00844064;

class fn_0046062e_class
{
public:
    void fn_0046062e(fn_0046062e_arg *arg);

    char pad_000[0xec];
    fn_00460c0c_list m_ec;
};

// FUNCTION: STARFLEETOP 0x0046062e
void fn_0046062e_class::fn_0046062e(fn_0046062e_arg *arg)
{
    const char *name = g_00844064;
    const std::string &text = arg->fn_0060a827()->fn_0054a8d4();
    if (text == name)
    {
        fn_006cf2d0_object value(arg->fn_005db8d3(), arg->fn_005db919());
        tDateAndTime now;
        fn_0045e5f7_object record;
        record.m_178 = value;
        record.fn_00460736(arg->fn_0060a827());
        record.m_168 = now;
        m_ec.fn_00460c0c(record);
    }
}
