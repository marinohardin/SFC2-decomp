#include <string>
#include "unsorted/tState_Skirmish.h"

// HYPOTHESIS: free function (hidden return slot, one stack argument); the owner class is unknown.
// FUNCTION: STARFLEETOP 0x0047f05d
std::string __stdcall fn_0047f05d(int index)
{
    std::string s;
    switch (index) {
    case 0:
        s.assign(1, 'F');
        break;
    case 1:
        s.assign(1, 'K');
        break;
    case 2:
        s.assign(1, 'R');
        break;
    case 3:
        s.assign(1, 'L');
        break;
    case 4:
        s.assign(1, 'H');
        break;
    case 5:
        s.assign(1, 'G');
        break;
    case 6:
        s.assign(1, 'I');
        break;
    case 7:
        s.assign(1, 'Z');
        break;
    case 16:
        s.assign(1, 'O');
        break;
    case 8:
        s.assign(1, 'P');
        break;
    case 9:
        s.assign(1, 'P');
        break;
    case 10:
        s.assign(1, 'P');
        break;
    case 11:
        s.assign(1, 'P');
        break;
    case 12:
        s.assign(1, 'P');
        break;
    case 13:
        s.assign(1, 'P');
        break;
    case 14:
        s.assign(1, 'P');
        break;
    case 15:
        s.assign(1, 'P');
        break;
    default:
        s.assign(1, ' ');
        break;
    }
    return s + "_Loading";
}

class c_005cf073_Object;
c_005cf073_Object *fn_004bdc4d();
const std::string &fn_004bdcb7();
const std::string &fn_004bdcbd();
const std::string &fn_004bdcc3();
const std::string &fn_004bdcc9();

// HYPOTHESIS: owner class unknown; the two helpers are thiscall methods on the same object.
class c_0047f439_Owner
{
public:
    void fn_0047f4ff();
    void fn_0047f62e(const char *text, const std::string &name);
    void fn_0047f439(int code);

    char pad_000[0xac];
    char m_ac;
    char pad_0ad;
    char m_ae;
};

#include "Common/Thread/ThreadPool.h"
#include "unsorted/tObject_0090b730.h"

// HYPOTHESIS: the object at 0x0090e008; fn_00460000.cpp declares the same global as
// fn_0090e008_class (a .cpp, not includable), so this unit keeps its own view of it.
class fn_0090e008_object
{
public:
    void fn_005d46d3();
};

extern fn_0090e008_object *g_0090e008;

// FUNCTION: STARFLEETOP 0x0047f439
void c_0047f439_Owner::fn_0047f439(int code)
{
    std::string s;
    fn_0047f4ff();
    switch (code) {
    case 5: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcbd());
        break;
    }
    case 6: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcc3());
        break;
    }
    case 9: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcc9());
        break;
    }
    default: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcb7());
        break;
    }
    }
}

// FUNCTION: STARFLEETOP 0x0047f4ff
void c_0047f439_Owner::fn_0047f4ff()
{
    m_ac = 0;
    if (g_0090e008 != 0)
    {
        m_ae = 1;
        tThreadPool::Instance()->EmptyWorkQue(true);
        g_0090e008->fn_005d46d3();
    }
    gObject_0090b730.fn_0060ca42(0);
}

void fn_004d32c4(const char *name);
extern const char *g_0084a4c4;

// FUNCTION: STARFLEETOP 0x0047f621
void fn_0047f621()
{
    fn_004d32c4(g_0084a4c4);
}

#include <string.h>

// HYPOTHESIS: owner class unknown; the std::string member sits at 0x20.
class c_0047f6b1_Owner
{
public:
    void fn_0047f6b1();

    char pad_000[0x20];
    std::string m_20;
    char pad_030[0x74];
    char m_a4;
};

extern const char *g_0084a4c8;
extern char g_008930a4[];
extern std::string g_00893090;

// FUNCTION: STARFLEETOP 0x0047f6b1
void c_0047f6b1_Owner::fn_0047f6b1()
{
    m_a4 = 0;
    const char *p = g_0084a4c8;
    if (!m_20.empty())
        p = m_20.c_str();
    fn_004d32c4(p);
    g_00893090.assign(g_008930a4, strlen(g_008930a4));
}

// HYPOTHESIS: returns the address of a global object at 0x00914190 (type unknown).
extern char g_00914190[];

// FUNCTION: STARFLEETOP 0x0047f720
char *fn_0047f720()
{
    return g_00914190;
}

void fn_004bd431();

// FUNCTION: STARFLEETOP 0x0047f764
void fn_0047f764()
{
    atexit(fn_004bd431);
}

// FUNCTION: STARFLEETOP 0x0047f770
void fn_0047f770()
{
    atexit(fn_004bd431);
}

// FUNCTION: STARFLEETOP 0x0047f77c
void fn_0047f77c()
{
    atexit(fn_004bd431);
}

// HYPOTHESIS: the pointer kept at 0x00915540 to the static skirmish state (type unknown here).
extern void *g_00915540;

// FUNCTION: STARFLEETOP 0x0047f79f
void *fn_0047f79f()
{
    return g_00915540;
}

// FUNCTION: STARFLEETOP 0x0047f7a5
char *tState_Skirmish::vfunction13()
{
    return "skirmish";
}

// FUNCTION: STARFLEETOP 0x0047f7ab
tState_Skirmish::tState_Skirmish()
{
    // HYPOTHESIS: the vectors at 0x1124, 0x1138 and 0x119c are zeroed by inlined constructors.
    *(char *)&fUnknown_1124[0] = 0;
    *(long *)&fUnknown_1124[4] = 0;
    *(long *)&fUnknown_1124[8] = 0;
    *(long *)&fUnknown_1124[12] = 0;
    field_0x1134 = 0;
    field_0x1154 = 0;
    field_0x12ac = 0;
    field_0x1120 = 0;
    field_0x1121 = 0;
}
