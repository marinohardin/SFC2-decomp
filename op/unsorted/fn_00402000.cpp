#include <ios>
#include <string>
#include <string.h>

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

// HYPOTHESIS: a running checksum object (value at +4) whose constructor is 0x006b9b00
// and whose virtual slot 1 feeds it bytes; the holder keeps it at +0xc and the result at +8.
class fn_004021d1_Checksum
{
public:
    fn_004021d1_Checksum();
    virtual void fn_slot0();
    virtual void Update(const void* data, int size);
    int m04;
};

struct fn_004021d1_Item
{
    int a;
    int b;
    int c;
    char d;
    char pad[3];
};

struct fn_004021d1_Record
{
    char pad0[4];
    int m04;
    char pad1[0x10];
    int m18;
    int m1c;
    int m20;
    int m24;
    int m28;
    int m2c;
    int m30;
    int m34;
    int m38;
    int m3c;
    int m40;
    int m44;
    char pad2[0x20];
    fn_004021d1_Item items[5];
};

class fn_004021d1_Holder
{
public:
    int pad[2];
    int m08;
    fn_004021d1_Checksum* m0c;
    void fn_004021d1(const fn_004021d1_Record* rec);
};

// FUNCTION: STARFLEETOP 0x004021d1
void fn_004021d1_Holder::fn_004021d1(const fn_004021d1_Record* rec)
{
    if (m0c == 0)
    {
        m0c = new fn_004021d1_Checksum;
        m0c->m04 = -1;
    }
    m0c->Update(&rec->m04, 4);
    m0c->Update(&rec->m18, 4);
    m0c->Update(&rec->m1c, 4);
    m0c->Update(&rec->m20, 4);
    m0c->Update(&rec->m24, 4);
    m0c->Update(&rec->m28, 4);
    m0c->Update(&rec->m2c, 4);
    m0c->Update(&rec->m30, 4);
    m0c->Update(&rec->m34, 4);
    m0c->Update(&rec->m38, 4);
    m0c->Update(&rec->m3c, 4);
    m0c->Update(&rec->m40, 4);
    m0c->Update(&rec->m44, 4);
    for (int i = 0; i < 5; i++)
    {
        int a = rec->items[i].a;
        int b = rec->items[i].b;
        int c = rec->items[i].c;
        char d = rec->items[i].d;
        m0c->Update(&a, 4);
        m0c->Update(&b, 4);
        m0c->Update(&c, 4);
        m0c->Update(&d, 1);
    }
    m08 = ~m0c->m04;
}

// HYPOTHESIS: an object whose 0x0040257f turns a string into another string and a count,
// and whose 0x004026ec consumes that result; neither is written yet.
class fn_0040233b_Object
{
public:
    std::string fn_0040257f(std::string text, char* count);
    bool fn_004026ec(const std::string& text, int arg, char count);
    bool fn_0040233b(std::string text, int arg);
};

// FUNCTION: STARFLEETOP 0x0040233b
bool fn_0040233b_Object::fn_0040233b(std::string text, int arg)
{
    bool result;
    char count;
    std::string parsed = fn_0040257f(text, &count);
    if (count > 0)
        result = fn_004026ec(parsed, arg, count);
    return result;
}

// HYPOTHESIS: an object with two int members at +0x20 and +0x24 whose change
// triggers three helper calls; the helpers are not written yet.
class fn_00402407_A;

class fn_004023bb_Sub
{
public:
    int m00;
    int m04;
    int m08;
    void fn_00404044(int a, int b);
    void fn_00402d77(fn_00402407_A* item);
    void fn_00402d77(const std::string& name);
};

// HYPOTHESIS: element of the 0xb8-byte records held between m14 and m18.
class fn_00402407_A
{
public:
    char m00[0x30];
};

class fn_00402407_B
{
public:
    char m00[0x70];
};

class fn_00402407_Item
{
public:
    int m00;
    int m04;
    fn_00402407_A m08;
    int m38;
    char m3c[0xc];
    fn_00402407_B m48;
};

extern char g_0083a3dc[];
char fn_0040503b(fn_00402407_B* self, char* key);

class fn_004023bb_Object
{
public:
    char m00[0x14];
    fn_00402407_Item* m14;
    fn_00402407_Item* m18;
    char m1c[4];
    int m20;
    int m24;
    fn_004023bb_Sub m28;
    void fn_00402407(int a, int b);
    void fn_00402452(int a, int b);
    fn_004023bb_Sub* fn_004023bb(int a, int b);
};

// FUNCTION: STARFLEETOP 0x004023bb
fn_004023bb_Sub* fn_004023bb_Object::fn_004023bb(int a, int b)
{
    if (a == m24 && m20 == b)
        return &m28;
    fn_004023bb_Sub* sub = &m28;
    sub->fn_00404044(sub->m04, sub->m08);
    fn_00402407(a, b);
    fn_00402452(a, b);
    return sub;
}

// FUNCTION: STARFLEETOP 0x00402407
void fn_004023bb_Object::fn_00402407(int a, int b)
{
    for (fn_00402407_Item* it = m14; it != m18; it++)
    {
        if (b >= it->m38 && a == it->m04)
        {
            if (!fn_0040503b(&it->m48, g_0083a3dc))
                m28.fn_00402d77(&it->m08);
        }
    }
}

// HYPOTHESIS: the global at 0x008949a0 is a list of 0x5e8-byte records; 0x005e8f21 counts
// them and 0x0040a699 fetches one by index (or 0).
struct fn_00402452_Record
{
    char m00[0xc];
    int m0c;
    char m10[4];
    char m14[0x20];
    int m34;
    char m38[0x5a8];
    int m5e0;
};

class fn_00402452_List
{
public:
    int fn_005e8f21();
    fn_00402452_Record* fn_0040a699(int index);
};

extern fn_00402452_List g_008949a0;

// FUNCTION: STARFLEETOP 0x00402452
void fn_004023bb_Object::fn_00402452(int a, int b)
{
    for (int i = 0; i < g_008949a0.fn_005e8f21(); i++)
    {
        fn_00402452_Record* rec = g_008949a0.fn_0040a699(i);
        if (rec && rec->m34 == 1)
        {
            int t = rec->m5e0;
            if (b >= t)
            {
                int id = rec->m0c;
                if (a == id)
                {
                    const char* name = rec->m14;
                    m28.fn_00402d77(std::string(name));
                }
            }
        }
    }
}

// HYPOTHESIS: a string helper taking the string on the stack (callee pops it); a second
// tab directly after a tab gets a '0' inserted before it.
// FUNCTION: STARFLEETOP 0x00402501
void __stdcall fn_00402501(std::string& s)
{
    s.reserve(s.capacity() * 2);
    std::string::iterator it = s.begin();
    bool prevTab = false;
    for (; it != s.end(); it++)
    {
        if (*it == '\t')
        {
            if (prevTab)
            {
                s.insert(it, '0');
                it++;
                if (it == s.end())
                    return;
            }
            else
                prevTab = true;
        }
        else
            prevTab = false;
    }
}

// FUNCTION: STARFLEETOP 0x0040257f
// HYPOTHESIS: splits "text:N" into the text before the colon and sets *found from the
// digit after it; without a colon, the whole string is the result and *found is true.
std::string __stdcall fn_0040257f(std::string src, char* found)
{
    std::string result("");
    std::string second;
    *found = false;
    int pos = src.find(':');
    if (pos != std::string::npos && pos > 0)
    {
        result.assign(src.substr(0, pos));
        second.assign(src.substr(pos + 1, 1));
        *found = atoi(second.c_str());
    }
    else if (src.size() > 0)
    {
        result.assign(src);
        *found = true;
    }
    return result;
}

void __cdecl fn_007364c0(char* dst, const char* src);
extern char g_0083a3e0[];
char fn_0040503b(const std::string& self, char* key);

// HYPOTHESIS: the names and layouts below are guesses from the access pattern.
struct fn_004026ec_Name : public std::string
{
    int fn_0040651e(void* p);
};

struct fn_004027c8_Rec
{
    int a;
    int b;
    int c;
    char d;
};

struct fn_004026ec_Entry
{
    int pad[2];
    char data[0xb0];
};

struct fn_004026ec_Dest
{
    char pad[0x14];
    char m14[4];
    void fn_00407db2(int v);
    void fn_00407dbf(int v);
    void fn_00407dcc(int v);
    void fn_00407dd9(int v);
    void fn_00407de6(int v);
    void fn_00407df3(int v);
};

class fn_004026ec_Owner
{
public:
    int pad[5];
    fn_004026ec_Entry* m14;
    fn_004026ec_Entry* m18;
    void fn_00402763(fn_004026ec_Dest* d, int a);
    void fn_004027c8(fn_004026ec_Entry* e, fn_004026ec_Dest* d, int a);
    char fn_004026ec(fn_004026ec_Name* name, fn_004026ec_Dest* d, int a);
};

// FUNCTION: STARFLEETOP 0x004026ec
char fn_004026ec_Owner::fn_004026ec(fn_004026ec_Name* name, fn_004026ec_Dest* d, int a)
{
    fn_00402763(d, a);
    for (fn_004026ec_Entry* e = m14; e != m18; e++)
    {
        if (name->fn_0040651e(e->data) == 0)
        {
            fn_007364c0(d->m14, name->c_str());
            fn_004027c8(e, d, a);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00402763
void fn_004026ec_Owner::fn_00402763(fn_004026ec_Dest* d, int a)
{
    int* p = (int*)d;
    p[0x398 / 4] = 0;
    p[0x39c / 4] = 0;
    p[900 / 4] = 0x3e800000;
    p[0x620 / 4] = 0x1e;
    p[0x5d8 / 4] = 100;
    p[0x628 / 4] = 1;
    p[0x614 / 4] = 6;
    p[0x610 / 4] = 6;
    p[0x380 / 4] = 0;
    p[0x388 / 4] = 0;
    p[0x38c / 4] = 0;
    p[0x10 / 4] = 0x16;
}

// FUNCTION: STARFLEETOP 0x004027c8
void fn_004026ec_Owner::fn_004027c8(fn_004026ec_Entry* e, fn_004026ec_Dest* d, int a)
{
    int n = (char)a;
    int i;
    int* pe = (int*)e;
    int* p = (int*)d;
    p[0x628 / 4] = n;
    p[0x570 / 4] = pe[0x20 / 4] * n;
    p[0x5dc / 4] = pe[0x30 / 4] * n;
    int* q = &p[0x618 / 4];
    *q = pe[0x44 / 4] * n;
    p[0xc / 4] = pe[4 / 4];
    *q = 6;
    p[0x5e0 / 4] = pe[0x38 / 4];
    p[0x5e4 / 4] = 999;
    fn_007364c0((char*)&p[0x38 / 4], ((std::string*)&pe[0x58 / 4])->c_str());
    fn_007364c0((char*)&p[0x238 / 4], ((std::string*)&pe[0x8 / 4])->c_str());
    fn_007364c0((char*)&p[0x138 / 4], ((std::string*)&pe[0x48 / 4])->c_str());
    if (fn_0040503b(((std::string*)&pe[0x8 / 4])->substr(((std::string*)&pe[0x8 / 4])->size() - 2, std::string::npos), g_0083a3e0))
        p[0x10 / 4] = 0x17;
    d->fn_00407db2(pe[0x1c / 4]);
    d->fn_00407dd9(pe[0x2c / 4]);
    d->fn_00407de6(pe[0x28 / 4]);
    d->fn_00407df3(pe[0x18 / 4]);
    d->fn_00407dbf(pe[0x34 / 4]);
    d->fn_00407dcc(pe[0x40 / 4]);
    for (i = 0; i < n; i++)
    {
        fn_004027c8_Rec* dst = (fn_004027c8_Rec*)((char*)d + 0x3c8) + (char)(i * 5);
        fn_004027c8_Rec* src = (fn_004027c8_Rec*)((char*)e + 0x68);
        for (int k = 0; k < 5; k++)
        {
            dst[k].a = src[k].a;
            dst[k].b = src[k].b;
            dst[k].c = src[k].c;
            dst[k].d = src[k].d;
        }
    }
}

class fn_00402967_Obj
{
public:
    fn_00402967_Obj();
    ~fn_00402967_Obj();
};

// FUNCTION: STARFLEETOP 0x00402971
// FUNCTION: STARFLEETOP 0x00402967
fn_00402967_Obj g_fn_00402967_Obj;

class fn_00402987_Obj
{
public:
    fn_00402987_Obj(const fn_00402987_Obj& x);
    void fn_005da20a(bool);
    void fn_00403b49(const fn_00402987_Obj&, unsigned int, unsigned int);
    char m0;
};

// HYPOTHESIS: this is the std::string constructor taking (string, pos, npos); 0x005da20a is _Tidy and 0x00403b49 is assign
extern unsigned int g_00795648;

// FUNCTION: STARFLEETOP 0x00402987
fn_00402987_Obj::fn_00402987_Obj(const fn_00402987_Obj& x)
{
    m0 = x.m0;
    fn_005da20a(false);
    fn_00403b49(x, 0, g_00795648);
}

class fn_004029b1_Obj
{
public:
    void fn_00403d87();
    void fn_00403c1a(unsigned int, unsigned int, char);
    char* fn_004029b1(char*, char);
    char m0;
    char* m4;
};

// HYPOTHESIS: this is the std::string insert(iterator, char); 0x00403d87 is begin and 0x00403c1a is insert(pos, n, c)
// FUNCTION: STARFLEETOP 0x004029b1
char* fn_004029b1_Obj::fn_004029b1(char* p, char c)
{
    fn_00403d87();
    unsigned int d = p - m4;
    unsigned int pos = p ? d : 0;
    fn_00403c1a(pos, 1, c);
    fn_00403d87();
    return m4 + pos;
}

struct fn_004029e8_Alloc
{
};

class fn_004029e8_Obj
{
public:
    fn_004029e8_Obj(const fn_004029e8_Obj& x, unsigned int pos, unsigned int n, const fn_004029e8_Alloc& a = fn_004029e8_Alloc())
        : m0(a)
    {
        fn_005da20a(false);
        fn_00403b49(x, pos, n);
    }
    ~fn_004029e8_Obj();
    fn_004029e8_Obj fn_004029e8(unsigned int pos, unsigned int n) const;
    void fn_005da20a(bool);
    void fn_00403b49(const fn_004029e8_Obj&, unsigned int, unsigned int);
    fn_004029e8_Alloc m0;
};

// HYPOTHESIS: std::string::substr(pos, n), returning basic_string(*this, pos, n); m0 is the allocator copied from a default temporary
// FUNCTION: STARFLEETOP 0x004029e8
fn_004029e8_Obj fn_004029e8_Obj::fn_004029e8(unsigned int pos, unsigned int n) const
{
    return fn_004029e8_Obj(*this, pos, n);
}

// HYPOTHESIS: std::istream::operator>>(long&) as the library header writes it (num_get's do_get for
// long sits at vtable +0x18), around ipfx (0x00403eda); the class stands in for basic_istream<char>.
// The analysis ends the function at the jump over its catch funclet, so the rebuilt tail scores as extra.
class fn_00402a26_Istream : public virtual std::ios
{
public:
    typedef std::istreambuf_iterator<char, std::char_traits<char> > _Iter;
    typedef std::num_get<char, _Iter> _Nget;
    bool fn_00403eda(bool noskip);
    fn_00402a26_Istream& fn_00402a26(long& x);
};

// FUNCTION: STARFLEETOP 0x00402a26
fn_00402a26_Istream& fn_00402a26_Istream::fn_00402a26(long& x)
{
    iostate _St = goodbit;
    const bool _Ok = fn_00403eda(false);
    if (_Ok)
        {const _Nget& _Fac = std::use_facet(getloc(), (_Nget *)0, true);
        _TRY_IO_BEGIN
        _Fac.get(_Iter(rdbuf()), _Iter(0), *this, _St, x);
        _CATCH_IO_END}
    setstate(_St);
    return (*this);
}

class fn_00402b22_Base
{
public:
    virtual ~fn_00402b22_Base();
};

// HYPOTHESIS: a reference-counted facet; this is its release step
class fn_00402b22_Inner : public fn_00402b22_Base
{
public:
    fn_00402b22_Base* fn_00402b3a();
    unsigned long m4;
};

// HYPOTHESIS: an owner that deletes what its first member yields; 0x00402b3a is a method of that member
class fn_00402b22_Obj
{
public:
    void fn_00402b22();
    fn_00402b22_Inner* m0;
};

// FUNCTION: STARFLEETOP 0x00402b22
void fn_00402b22_Obj::fn_00402b22()
{
    if (m0 != 0)
    {
        fn_00402b22_Base* p = m0->fn_00402b3a();
        delete p;
    }
}

// FUNCTION: STARFLEETOP 0x00402b3a
fn_00402b22_Base* fn_00402b22_Inner::fn_00402b3a()
{
    std::_Lockit lock;
    if (0 < m4 && m4 < (unsigned long)-1)
        --m4;
    return m4 == 0 ? this : 0;
}
