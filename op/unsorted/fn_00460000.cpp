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

#include "Common/Database/DatabaseObject.h"

// HYPOTHESIS: 16-byte members of tCampaignInfo assigned through their own out-of-line
// assignments (0x0046090a, 0x00604f1d); their types are not identified.
// HYPOTHESIS: 0x0046090a is the library's vector assignment for 16-byte strings (allocator,
// first, last, end); the members and helpers below follow that layout.
class fn_0046090a_object
{
public:
    fn_0046090a_object &operator=(const fn_0046090a_object &other);
    const std::string *begin() const;
    const std::string *end() const;
    unsigned int size() const;
    unsigned int capacity() const;
    void _Destroy(std::string *first, std::string *last);
    std::string *_Ucopy(const std::string *first, const std::string *last, std::string *dest);

    std::allocator<std::string> allocator;
    std::string *_First;
    std::string *_Last;
    std::string *_End;
};

inline const std::string *fn_0046090a_object::begin() const
{
    return (_First);
}

inline const std::string *fn_0046090a_object::end() const
{
    return (_Last);
}

inline unsigned int fn_0046090a_object::size() const
{
    return (_First == 0 ? 0 : _Last - _First);
}

inline unsigned int fn_0046090a_object::capacity() const
{
    return (_First == 0 ? 0 : _End - _First);
}

inline void fn_0046090a_object::_Destroy(std::string *first, std::string *last)
{
    for (; first != last; ++first)
        allocator.destroy(first);
}

inline std::string *fn_0046090a_object::_Ucopy(const std::string *first, const std::string *last, std::string *dest)
{
    for (; first != last; ++dest, ++first)
        allocator.construct(dest, *first);
    return (dest);
}

// FUNCTION: STARFLEETOP 0x0046090a
fn_0046090a_object &fn_0046090a_object::operator=(const fn_0046090a_object &other)
{
    if (this == &other)
        ;
    else if (other.size() <= size())
    {
        std::string *s = std::copy(other.begin(), other.end(), _First);
        _Destroy(s, _Last);
        _Last = _First + other.size();
    }
    else if (other.size() <= capacity())
    {
        const std::string *s = other.begin() + size();
        std::copy(other.begin(), s, _First);
        _Ucopy(s, other.end(), _Last);
        _Last = _First + other.size();
    }
    else
    {
        _Destroy(_First, _Last);
        allocator.deallocate(_First, _End - _First);
        _First = allocator.allocate(other.size(), (void *)0);
        _Last = _Ucopy(other.begin(), other.end(), _First);
        _End = _Last;
    }
    return (*this);
}

class fn_00604f1d_object
{
public:
    fn_00604f1d_object &operator=(const fn_00604f1d_object &other);

    char pad_000[0x10];
};

// HYPOTHESIS: a 0x30-byte member of tCampaignInfo assigned through 0x0045be62.
class fn_0045be62_object
{
public:
    fn_0045be62_object &operator=(const fn_0045be62_object &other);

    char pad_000[0x30];
};

// HYPOTHESIS: declared here until its own unit has a header. The size (0x168) comes from the
// stack layout below, the destructor is at 0x0042ceae, and the members are placed by the
// assignment at 0x00460736; their names are not known.
class tCampaignInfo : public nDatabase::tDatabaseObject
{
public:
    ~tCampaignInfo();
    tCampaignInfo &operator=(const tCampaignInfo &other);
    // HYPOTHESIS: 0x0054a8d4 lies in tCampaignInfo's own code and returns a text member.
    const std::string &fn_0054a8d4();

    std::string m_48;
    std::string m_58;
    std::string m_68;
    std::string m_78;
    std::string m_88;
    fn_0046090a_object m_98;
    fn_00604f1d_object m_a8;
    long m_b8;
    long m_bc;
    long m_c0;
    std::string m_c4;
    std::string m_d4;
    std::string m_e4;
    long m_f4;
    std::string m_f8;
    fn_0045be62_object m_108;
    long m_138;
    long m_13c;
    long m_140;
    long m_144;
    std::string m_148;
    std::string m_158;
};

// HYPOTHESIS: the member-wise assignment the compiler would generate, held out of line.
// FUNCTION: STARFLEETOP 0x00460736
tCampaignInfo &tCampaignInfo::operator=(const tCampaignInfo &other)
{
    // MATCH-ONLY: VC6 rejects the qualified base call (C2352); assign through the base.
    (nDatabase::tDatabaseObject &)*this = other;
    m_48 = other.m_48;
    m_58 = other.m_58;
    m_68 = other.m_68;
    m_78 = other.m_78;
    m_88 = other.m_88;
    m_98 = other.m_98;
    m_a8 = other.m_a8;
    m_b8 = other.m_b8;
    m_bc = other.m_bc;
    m_c0 = other.m_c0;
    m_c4 = other.m_c4;
    m_d4 = other.m_d4;
    m_e4 = other.m_e4;
    m_f4 = other.m_f4;
    m_f8 = other.m_f8;
    m_108 = other.m_108;
    m_138 = other.m_138;
    m_13c = other.m_13c;
    m_140 = other.m_140;
    m_144 = other.m_144;
    m_148 = other.m_148;
    m_158 = other.m_158;
    return *this;
}

// HYPOTHESIS: a 20-byte value built from a text and a number; it is copied as plain data.
class fn_006cf2d0_object
{
public:
    fn_006cf2d0_object(const std::string &text, long value);

    char pad_000[0x14];
};

// HYPOTHESIS: the record built at 0x0045e5f7; its members are placed by their stack offsets.
class fn_0045e5f7_object
{
public:
    fn_0045e5f7_object();

    tCampaignInfo m_000;
    tDateAndTime m_168;
    fn_006cf2d0_object m_178;
};

// HYPOTHESIS: the argument of 0x0046062e; only these calls on it are known.
class fn_0046062e_arg
{
public:
    tCampaignInfo *fn_0060a827();
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
        record.m_000 = *arg->fn_0060a827();
        record.m_168 = now;
        m_ec.fn_00460c0c(record);
    }
}

#include "unsorted/tState_CampaignSelector.h"

// HYPOTHESIS: the state's one instance; its initializer follows this unit's code.
// FUNCTION: STARFLEETOP 0x004608f4
// FUNCTION: STARFLEETOP 0x004608ea
tState_Namespace_CampaignSelector::tState_CampaignSelector g_0090d158;

// HYPOTHESIS: a list-like container whose nodes start with the next link; 0x00460acb tears it
// down like the library list's destructor, plus a second block at +8.
struct fn_00460acb_node
{
    fn_00460acb_node *_Next;
};

// HYPOTHESIS: the container's iterator, a node pointer with a constructor like the library's.
class fn_00460acb_iterator
{
public:
    fn_00460acb_iterator(fn_00460acb_node *p) : _Ptr(p) {}

    fn_00460acb_node *_Ptr;
};

class fn_00460acb_class
{
public:
    // HYPOTHESIS: 0x00460c53 erases the nodes in [first, last) and returns the iterator after.
    fn_00460acb_iterator fn_00460c53(fn_00460acb_iterator first, fn_00460acb_iterator last);
    fn_00460acb_iterator begin();
    fn_00460acb_iterator end();
    void fn_00460acb();

    char pad_000[0x4];
    fn_00460acb_node *_Head;
    void *m_8;
    char pad_00c[0x4];
    unsigned int _Size;
};

inline fn_00460acb_iterator fn_00460acb_class::begin()
{
    return (fn_00460acb_iterator(_Head->_Next));
}

inline fn_00460acb_iterator fn_00460acb_class::end()
{
    return (fn_00460acb_iterator(_Head));
}

// FUNCTION: STARFLEETOP 0x00460acb
void fn_00460acb_class::fn_00460acb()
{
    fn_00460c53(begin(), end());
    operator delete(_Head);
    _Head = 0, _Size = 0;
    operator delete(m_8);
    m_8 = 0;
}

// HYPOTHESIS: a second container of the same shape as fn_00460acb_class, with another erase
// at 0x00460cc7.
class fn_00460b03_class
{
public:
    fn_00460acb_iterator fn_00460cc7(fn_00460acb_iterator first, fn_00460acb_iterator last);
    fn_00460acb_iterator begin();
    fn_00460acb_iterator end();
    void fn_00460b03();

    char pad_000[0x4];
    fn_00460acb_node *_Head;
    void *m_8;
    char pad_00c[0x4];
    unsigned int _Size;
};

inline fn_00460acb_iterator fn_00460b03_class::begin()
{
    return (fn_00460acb_iterator(_Head->_Next));
}

inline fn_00460acb_iterator fn_00460b03_class::end()
{
    return (fn_00460acb_iterator(_Head));
}

// FUNCTION: STARFLEETOP 0x00460b03
void fn_00460b03_class::fn_00460b03()
{
    fn_00460cc7(begin(), end());
    operator delete(_Head);
    _Head = 0, _Size = 0;
    operator delete(m_8);
    m_8 = 0;
}

// HYPOTHESIS: a third container of the same shape as fn_00460acb_class, with another erase
// at 0x00460d3b.
class fn_00460b3b_class
{
public:
    fn_00460acb_iterator fn_00460d3b(fn_00460acb_iterator first, fn_00460acb_iterator last);
    fn_00460acb_iterator begin();
    fn_00460acb_iterator end();
    void fn_00460b3b();

    char pad_000[0x4];
    fn_00460acb_node *_Head;
    void *m_8;
    char pad_00c[0x4];
    unsigned int _Size;
};

inline fn_00460acb_iterator fn_00460b3b_class::begin()
{
    return (fn_00460acb_iterator(_Head->_Next));
}

inline fn_00460acb_iterator fn_00460b3b_class::end()
{
    return (fn_00460acb_iterator(_Head));
}

// FUNCTION: STARFLEETOP 0x00460b3b
void fn_00460b3b_class::fn_00460b3b()
{
    fn_00460d3b(begin(), end());
    operator delete(_Head);
    _Head = 0, _Size = 0;
    operator delete(m_8);
    m_8 = 0;
}

// HYPOTHESIS: 0x00460b73 is the library's vector fill constructor for 396-byte elements; the
// element type is CMiniDockFrameWnd (the function list names its destructor 0x0045e63a), and
// 0x00461406 is its copy constructor.
class CMiniDockFrameWnd
{
public:
    CMiniDockFrameWnd(const CMiniDockFrameWnd &other);
    virtual ~CMiniDockFrameWnd();

    char pad_004[0x188];
};

class fn_00460b73_class
{
public:
    fn_00460b73_class(unsigned int n, const CMiniDockFrameWnd &v, const std::allocator<CMiniDockFrameWnd> &al);
    ~fn_00460b73_class();
    void _Destroy(CMiniDockFrameWnd *_F, CMiniDockFrameWnd *_L);

    std::allocator<CMiniDockFrameWnd> allocator;
    CMiniDockFrameWnd *_First;
    CMiniDockFrameWnd *_Last;
    CMiniDockFrameWnd *_End;
};

// FUNCTION: STARFLEETOP 0x00460b73
fn_00460b73_class::fn_00460b73_class(unsigned int n, const CMiniDockFrameWnd &v, const std::allocator<CMiniDockFrameWnd> &al)
    : allocator(al)
{
    _First = allocator.allocate(n, (void *)0);
    std::uninitialized_fill_n(_First, n, v);
    _Last = _First + n, _End = _Last;
}

inline void fn_00460b73_class::_Destroy(CMiniDockFrameWnd *_F, CMiniDockFrameWnd *_L)
{
    for (; _F != _L; ++_F)
        _F->CMiniDockFrameWnd::~CMiniDockFrameWnd();
}

// HYPOTHESIS: 0x00460bd6 is the library's vector destructor for the same elements.
// FUNCTION: STARFLEETOP 0x00460bd6
fn_00460b73_class::~fn_00460b73_class()
{
    _Destroy(_First, _Last);
    allocator.deallocate(_First, _End - _First);
    _First = 0, _Last = 0, _End = 0;
}

// HYPOTHESIS: 0x00460c0c forwards the member at +8 to 0x00460ede with a constant 1; the callee
// pops its own three arguments, so it is __stdcall.
void __stdcall fn_00460ede(void *p, int n, int v);

class fn_00460c0c_class
{
public:
    void fn_00460c0c(int v);

    char pad_000[0x8];
    void *m_8;
};

// FUNCTION: STARFLEETOP 0x00460c0c
void fn_00460c0c_class::fn_00460c0c(int v)
{
    fn_00460ede(m_8, 1, v);
}
