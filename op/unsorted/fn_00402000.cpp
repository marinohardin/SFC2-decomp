#include <ios>
#include <string>

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
