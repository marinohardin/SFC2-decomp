#include "Tiger/tstring.h"

// HYPOTHESIS: a buffer-like class; the constructor allocates param_1 bytes and sets up a cursor.
class fn_0040d63f_Buf
{
public:
    fn_0040d63f_Buf(int size);
    void fn_0040d67d();
    void fn_0040d692();
    fn_0040d63f_Buf* fn_0040d6b7(int* src);
    fn_0040d63f_Buf* fn_0040d705(int* src);
    fn_0040d63f_Buf* fn_0040d73e(tString* str);
    // HYPOTHESIS: 0x0040d867 is a member of this buffer class taking (data, size, count).
    void fn_0040d867(void* data, int size, int count);
    // HYPOTHESIS: 0x0040d897 is the read counterpart of 0x0040d867.
    int fn_0040d897(void* data, int size, int count);
    void fn_0040d8cf(int n);
    void fn_0040d8e8(char* p);
    fn_0040d63f_Buf* fn_0040d7d6(tString* str);
    void fn_0040d815();
    void fn_0040d841();
    fn_0040d63f_Buf* fn_0040d777(struct fn_0040d777_Point3& p);
    fn_0040d63f_Buf* fn_0040d7ae(struct fn_0040d7ae_Point2& p);
    int m00;
    char* m04;
    char* m08;
    char* m0c;
    char* m10;
    char m14;
    char m15;
    char m16;
    int m18;
};

// FUNCTION: STARFLEETOP 0x0040d63f
fn_0040d63f_Buf::fn_0040d63f_Buf(int size)
{
    m14 = 1;
    m04 = (char*)operator new(size);
    m10 = m04 + size;
    if (m04 == 0)
        m10 = 0;
    m00 = -1;
    m08 = m04;
    m0c = m04;
    m15 = 0;
    m16 = 0;
    m18 = 0;
}

// FUNCTION: STARFLEETOP 0x0040d67d
void fn_0040d63f_Buf::fn_0040d67d()
{
    m08 = m04;
    m0c = m04;
    m15 = 0;
    m16 = 0;
    m18 = 0;
}

// FUNCTION: STARFLEETOP 0x0040d692
void fn_0040d63f_Buf::fn_0040d692()
{
    if (m04 != 0) {
        if (m14 == 1)
            operator delete(m04);
        m04 = 0;
        m08 = 0;
        m0c = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0040d6b7
fn_0040d63f_Buf* fn_0040d63f_Buf::fn_0040d6b7(int* src)
{
    int* p = (int*)src[0];
    fn_0040d867(&p, 4, 1);
    p = (int*)src[1];
    fn_0040d867(&p, 4, 1);
    p = (int*)src[2];
    fn_0040d867(&p, 4, 1);
    return this;
}

// FUNCTION: STARFLEETOP 0x0040d705
fn_0040d63f_Buf* fn_0040d63f_Buf::fn_0040d705(int* src)
{
    int* p = (int*)src[0];
    fn_0040d867(&p, 4, 1);
    p = (int*)src[1];
    fn_0040d867(&p, 4, 1);
    return this;
}

// FUNCTION: STARFLEETOP 0x0040d73e
fn_0040d63f_Buf* fn_0040d63f_Buf::fn_0040d73e(tString* str)
{
    iBasicStringElement* p = str->mWideStr();
    int len = str->mStrLen();
    for (int i = 0; i < len + 1; i++)
        fn_0040d867(&p[i], 2, 1);
    return this;
}

// HYPOTHESIS: stand-in for tPoint3 (three floats), read member by member.
struct fn_0040d777_Point3
{
    float x;
    float y;
    float z;
};

// FUNCTION: STARFLEETOP 0x0040d777
fn_0040d63f_Buf* fn_0040d63f_Buf::fn_0040d777(fn_0040d777_Point3& p)
{
    fn_0040d897(&p.x, 4, 1);
    fn_0040d897(&p.y, 4, 1);
    fn_0040d897(&p.z, 4, 1);
    return this;
}

// HYPOTHESIS: stand-in for tPoint2 (two floats), read member by member.
struct fn_0040d7ae_Point2
{
    float x;
    float y;
};

// FUNCTION: STARFLEETOP 0x0040d7ae
fn_0040d63f_Buf* fn_0040d63f_Buf::fn_0040d7ae(fn_0040d7ae_Point2& p)
{
    fn_0040d897(&p.x, 4, 1);
    fn_0040d897(&p.y, 4, 1);
    return this;
}

// FUNCTION: STARFLEETOP 0x0040d7d6
fn_0040d63f_Buf* fn_0040d63f_Buf::fn_0040d7d6(tString* str)
{
    iBasicStringElement c;
    *str = "";
    for (;;) {
        fn_0040d897(&c, 2, 1);
        if (c == 0)
            break;
        *str += c;
    }
    return this;
}

// FUNCTION: STARFLEETOP 0x0040d815
void fn_0040d63f_Buf::fn_0040d815()
{
    char b = 9;
    for (int i = 0; i < m18; i++)
        fn_0040d867(&b, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040d841
void fn_0040d63f_Buf::fn_0040d841()
{
    char b;
    for (int i = 0; i < m18; i++)
        fn_0040d897(&b, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040d867
void fn_0040d63f_Buf::fn_0040d867(void* data, int size, int count)
{
    if (m15 != 1 && data != 0) {
        char* src = (char*)data;
        while (size > 0 && m0c < m10) {
            *m0c = *src;
            m0c++;
            src++;
            size--;
        }
    }
}

// FUNCTION: STARFLEETOP 0x0040d897
int fn_0040d63f_Buf::fn_0040d897(void* data, int size, int count)
{
    int total = size;
    if (m16 == 1)
        return 0;
    if (data == 0)
        return 0;
    char* dst = (char*)data;
    while (size > 0 && m08 < m0c) {
        *dst = *m08;
        m08++;
        dst++;
        size--;
    }
    return total - size;
}

// FUNCTION: STARFLEETOP 0x0040d8cf
void fn_0040d63f_Buf::fn_0040d8cf(int n)
{
    m0c = m08 + n;
    if (m0c > m10)
        m0c = m10;
}

// FUNCTION: STARFLEETOP 0x0040d8e8
void fn_0040d63f_Buf::fn_0040d8e8(char* p)
{
    m0c = p;
    if (p > m10)
        m0c = m10;
    if (m0c < m08)
        m0c = m08;
}
