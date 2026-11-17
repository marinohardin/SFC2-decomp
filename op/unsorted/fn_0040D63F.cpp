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
    // HYPOTHESIS: 0x0040e6d1 writes a zero-terminated string (it takes strlen + 1).
    void fn_0040e6d1(char* s);
    void fn_0040e699(int n);
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

// FUNCTION: STARFLEETOP 0x0040d908
void fn_0040d908(char* s, fn_0040d63f_Buf& buf)
{
    buf.fn_0040e6d1(s);
}

// FUNCTION: STARFLEETOP 0x0040d916
void fn_0040d916(int n, fn_0040d63f_Buf& buf)
{
    buf.fn_0040e699(n);
}

// HYPOTHESIS: variable-length encoding of a signed value: sign and low 6 bits in the first byte, then 7 bits per byte.
// FUNCTION: STARFLEETOP 0x0040d924
void fn_0040d924(int* p, fn_0040d63f_Buf& buf)
{
    unsigned int v = *p;
    unsigned char first;
    if ((int)v < 0) {
        v = -(int)v;
        if (v < 0x40) {
            unsigned char b = (unsigned char)v | 0x40;
            buf.fn_0040d867(&b, 1, 0);
            return;
        }
        first = (unsigned char)v | 0xc0;
    } else {
        if (v < 0x40) {
            unsigned char b = (unsigned char)v;
            buf.fn_0040d867(&b, 1, 0);
            return;
        }
        first = ((unsigned char)v & 0x3f) | 0x80;
    }
    // MATCH-ONLY: the block scope lets b share the dead parameter's stack slot with the
    // single-byte cases' b, as the original's [ebp+0xb] does.
    {
        unsigned char b = first;
        buf.fn_0040d867(&b, 1, 0);
        v >>= 6;
        while (v >= 0x80) {
            b = (unsigned char)v | 0x80;
            buf.fn_0040d867(&b, 1, 0);
            v >>= 7;
        }
        b = (unsigned char)v;
        buf.fn_0040d867(&b, 1, 0);
    }
}

// HYPOTHESIS: reader for the encoding written by fn_0040d924.
// FUNCTION: STARFLEETOP 0x0040d9bf
void fn_0040d9bf(unsigned int* p, fn_0040d63f_Buf& buf)
{
    unsigned char b[4];
    b[2] = 0;
    buf.fn_0040d897(&b[3], 1, 0);
    *p = b[3] & 0x3f;
    if ((b[3] & 0x40) > 0) {
        b[2] = 1;
    }
    if ((b[3] & 0x80) > 0) {
        int shift = 0x40;
        while (1) {
            buf.fn_0040d897(&b[3], 1, 0);
            if ((b[3] & 0x80) <= 0) {
                break;
            }
            *p += (b[3] & 0x7f) * shift;
            shift <<= 7;
        }
        *p += (b[3] & 0x7f) * shift;
    }
    if (b[2] == 1) {
        *p = -*p;
    }
}

// HYPOTHESIS: unsigned variant of the writer fn_0040d924, 7 bits per byte.
// FUNCTION: STARFLEETOP 0x0040da33
void fn_0040da33(unsigned int* p, fn_0040d63f_Buf& buf)
{
    unsigned char b;
    unsigned int v = *p;
    while (v >= 0x80) {
        b = (unsigned char)v | 0x80;
        buf.fn_0040d867(&b, 1, 0);
        v >>= 7;
    }
    b = (unsigned char)v;
    buf.fn_0040d867(&b, 1, 0);
}

// HYPOTHESIS: unsigned reader for the encoding written by fn_0040da33.
// FUNCTION: STARFLEETOP 0x0040da79
void fn_0040da79(unsigned int* p, fn_0040d63f_Buf& buf)
{
    unsigned char b[4];
    int shift = 1;
    buf.fn_0040d897(&b[3], 1, 0);
    *p = 0;
    while ((b[3] & 0x80) > 0) {
        *p += (b[3] & 0x7f) * shift;
        shift <<= 7;
        buf.fn_0040d897(&b[3], 1, 0);
    }
    *p += (b[3] & 0x7f) * shift;
}

// FUNCTION: STARFLEETOP 0x0040dac7
void fn_0040dac7(int* src, fn_0040d63f_Buf& buf)
{
    int* p = (int*)src[0];
    buf.fn_0040d867(&p, 4, 1);
}

// FUNCTION: STARFLEETOP 0x0040dae4
void fn_0040dae4(int* dst, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d897(dst, 4, 1);
}

// FUNCTION: STARFLEETOP 0x0040daf6
void fn_0040daf6(int* src, fn_0040d63f_Buf& buf)
{
    int* p = (int*)src[0];
    fn_0040dac7((int*)&p, buf);
    p = (int*)src[1];
    fn_0040dac7((int*)&p, buf);
    p = (int*)src[2];
    fn_0040dac7((int*)&p, buf);
}

// FUNCTION: STARFLEETOP 0x0040db38
void fn_0040db38(float* dst, fn_0040d63f_Buf& buf)
{
    float v;
    fn_0040dae4((int*)&v, buf);
    dst[0] = v;
    fn_0040dae4((int*)&v, buf);
    dst[1] = v;
    fn_0040dae4((int*)&v, buf);
    dst[2] = v;
}
