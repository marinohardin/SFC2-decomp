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
    int fn_0040e699(char* p);
    // HYPOTHESIS: 0x0040e744 is a member taking a formatted text line.
    void fn_0040e744(char* text);
    // HYPOTHESIS: 0x0040e70f is the read counterpart of 0x0040e744 (fetches a text line).
    void fn_0040e70f(char* text);
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
    buf.fn_0040e699((char*)n);
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

// FUNCTION: STARFLEETOP 0x0040db7b
void fn_0040db7b(int* src, fn_0040d63f_Buf& buf)
{
    int* p = (int*)src[0];
    fn_0040dac7((int*)&p, buf);
    p = (int*)src[1];
    fn_0040dac7((int*)&p, buf);
}

// FUNCTION: STARFLEETOP 0x0040dbab
void fn_0040dbab(float* dst, fn_0040d63f_Buf& buf)
{
    float v;
    fn_0040dae4((int*)&v, buf);
    dst[0] = v;
    fn_0040dae4((int*)&v, buf);
    dst[1] = v;
}

// FUNCTION: STARFLEETOP 0x0040dbdc
void fn_0040dbdc(tString* str, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d73e(str);
}

// FUNCTION: STARFLEETOP 0x0040dbea
void fn_0040dbea(tString* str, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d7d6(str);
}

// FUNCTION: STARFLEETOP 0x0040dbf8
void fn_0040dbf8(double* src, fn_0040d63f_Buf& buf)
{
    float f = (float)*src;
    fn_0040dac7((int*)&f, buf);
}

// FUNCTION: STARFLEETOP 0x0040dc13
void fn_0040dc13(double* dst, fn_0040d63f_Buf& buf)
{
    float f;
    fn_0040dae4((int*)&f, buf);
    *dst = (double)f;
}

// FUNCTION: STARFLEETOP 0x0040dc2e
void fn_0040dc2e(short* p, fn_0040d63f_Buf& buf)
{
    int v = *p;
    fn_0040d924(&v, buf);
}

// FUNCTION: STARFLEETOP 0x0040dc4a
void fn_0040dc4a(short* p, fn_0040d63f_Buf& buf)
{
    unsigned int v;
    fn_0040d9bf(&v, buf);
    *p = (short)v;
}

// FUNCTION: STARFLEETOP 0x0040dc67
void fn_0040dc67(unsigned char* p, fn_0040d63f_Buf& buf)
{
    unsigned char b = *p;
    buf.fn_0040d867(&b, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040dc84
void fn_0040dc84(unsigned char* p, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d897(p, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040dc96
void fn_0040dc96(int size, void* p, fn_0040d63f_Buf& buf)
{
    switch (size) {
    case 1: {
        unsigned char b = *(unsigned char*)p;
        fn_0040dc67(&b, buf);
        break;
    }
    case 2: {
        short s = *(short*)p;
        fn_0040dc2e(&s, buf);
        break;
    }
    case 4: {
        int v = *(int*)p;
        fn_0040d924(&v, buf);
        break;
    }
    }
}

// FUNCTION: STARFLEETOP 0x0040dceb
void fn_0040dceb(int size, void* p, fn_0040d63f_Buf& buf)
{
    switch (size) {
    case 1: {
        unsigned char b;
        fn_0040dc84(&b, buf);
        *(unsigned char*)p = b;
        break;
    }
    case 2: {
        short s;
        fn_0040dc4a(&s, buf);
        *(short*)p = s;
        break;
    }
    case 4: {
        unsigned int v;
        fn_0040d9bf(&v, buf);
        *(unsigned int*)p = v;
        break;
    }
    }
}

// FUNCTION: STARFLEETOP 0x0040dd45
extern "C" int sprintf(char* buffer, const char* format, ...);

void fn_0040dd45(char* name, long* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %ld", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040dd83
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" int sscanf(const char* buffer, const char* format, ...);

void fn_0040dd83(char* name, long* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %ld");
    buf.fn_0040e70f(text);
    sscanf(text, format, value);
}

// FUNCTION: STARFLEETOP 0x0040dde1
void fn_0040dde1(char* name, unsigned long* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %lu", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040de1f
void fn_0040de1f(char* name, unsigned long* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %lu");
    buf.fn_0040e70f(text);
    sscanf(text, format, value);
}

// FUNCTION: STARFLEETOP 0x0040de7d
void fn_0040de7d(char* name, short* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %ld", name, (int)*value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040debd
void fn_0040debd(char* name, short* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    long n;
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %ld");
    buf.fn_0040e70f(text);
    sscanf(text, format, &n);
    *value = (short)n;
}

// FUNCTION: STARFLEETOP 0x0040df26
void fn_0040df26(char* name, char* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %ld", name, (int)*value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040df66
void fn_0040df66(char* name, char* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    long n;
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %ld");
    buf.fn_0040e70f(text);
    sscanf(text, format, &n);
    *value = (char)n;
}

// FUNCTION: STARFLEETOP 0x0040dfcd
void fn_0040dfcd(char* name, unsigned char* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    unsigned long n;
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %lu");
    buf.fn_0040e70f(text);
    sscanf(text, format, &n);
    *value = (unsigned char)n;
}

// FUNCTION: STARFLEETOP 0x0040e034
void fn_0040e034(char* name, unsigned char* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %lu", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e074
void fn_0040e074(char* name, unsigned char* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    unsigned long n;
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %lu");
    buf.fn_0040e70f(text);
    sscanf(text, format, &n);
    *value = (unsigned char)n;
}

// FUNCTION: STARFLEETOP 0x0040e0db
void fn_0040e0db(char* name, float* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %f", name, (double)*value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e11e
void fn_0040e11e(char* name, float* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %f");
    buf.fn_0040e70f(text);
    sscanf(text, format, value);
}

// FUNCTION: STARFLEETOP 0x0040e17c
void fn_0040e17c(char* name, float* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = x=%f y=%f z=%f", name, (double)value[0], (double)value[1], (double)value[2]);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e1cf
void fn_0040e1cf(char* name, float* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    float z;
    float x;
    float y;
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = x=%f y=%f z=%f");
    buf.fn_0040e70f(text);
    sscanf(text, format, &x, &y, &z);
    value[0] = x;
    value[1] = y;
    value[2] = z;
}

// FUNCTION: STARFLEETOP 0x0040e24a
void fn_0040e24a(char* name, float* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = x=%f y=%f", name, (double)value[0], (double)value[1]);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e295
void fn_0040e295(char* name, float* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    float x;
    float y;
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = x=%f y=%f");
    buf.fn_0040e70f(text);
    sscanf(text, format, &x, &y);
    value[0] = x;
    value[1] = y;
}

// FUNCTION: STARFLEETOP 0x0040e306
void fn_0040e306(tString* name, tString* value, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d815();
    tString line(*name);
    line += " = ";
    line += *value;
    buf.fn_0040d73e(&line);
    // MATCH-ONLY: the block scope lets nl share the dead parameter's slot, as the original's [ebp+0xb] does.
    {
        char nl = 10;
        buf.fn_0040d867(&nl, 1, 0);
    }
}

// FUNCTION: STARFLEETOP 0x0040e37a
void fn_0040e37a(tString* name, tString* value, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d841();
    tString line(*name);
    line += " = ";
    tString text;
    buf.fn_0040d7d6(&text);
    *value = text.substr(line.mStrLen());
    char nl;
    buf.fn_0040d897(&nl, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040e42b
void fn_0040e42b(char* name, double* value, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %lf", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e46e
void fn_0040e46e(char* name, double* value, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    buf.fn_0040d841();
    strcpy(format, name);
    strcat(format, " = %lf");
    buf.fn_0040e70f(text);
    sscanf(text, format, value);
}

// FUNCTION: STARFLEETOP 0x0040e4cc
// HYPOTHESIS: writes a 1, 2 or 4 byte value by size
void fn_0040e4cc(char* name, int size, void* value, fn_0040d63f_Buf& buf)
{
    switch (size) {
    case 1: {
        char c = *(char*)value;
        fn_0040df26(name, &c, buf);
        break;
    }
    case 2: {
        short s = *(short*)value;
        fn_0040de7d(name, &s, buf);
        break;
    }
    case 4: {
        long l = *(long*)value;
        fn_0040dd45(name, &l, buf);
        break;
    }
    }
}

// FUNCTION: STARFLEETOP 0x0040e52b
// HYPOTHESIS: reads a 1, 2 or 4 byte value by size
void fn_0040e52b(char* name, int size, void* value, fn_0040d63f_Buf& buf)
{
    switch (size) {
    case 1: {
        char c;
        fn_0040df66(name, &c, buf);
        *(char*)value = c;
        break;
    }
    case 2: {
        short s;
        fn_0040debd(name, &s, buf);
        *(short*)value = s;
        break;
    }
    case 4: {
        long l;
        fn_0040dd83(name, &l, buf);
        *(long*)value = l;
        break;
    }
    }
}

// FUNCTION: STARFLEETOP 0x0040e591
void fn_0040e591(char* a, char* b, fn_0040d63f_Buf& buf)
{
    char text[256];
    buf.fn_0040d815();
    // HYPOTHESIS: the format text is a guess; the string relocation is unverified.
    sprintf(text, "%s = %s", a, b);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e5cd
void fn_0040e5cd(char* a, char* b, fn_0040d63f_Buf& buf)
{
    char format[256];
    char text[256];
    buf.fn_0040d841();
    strcpy(format, a);
    strcat(format, " = ");
    buf.fn_0040e70f(text);
    strcpy(b, text + strlen(format));
}

// FUNCTION: STARFLEETOP 0x0040e631
void fn_0040e631(char* text, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d815();
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e648
void fn_0040e648(char* text, fn_0040d63f_Buf& buf)
{
    buf.fn_0040d841();
    buf.fn_0040e70f(text);
}

// FUNCTION: STARFLEETOP 0x0040e65f
void fn_0040e65f(fn_0040d63f_Buf& buf)
{
    char c = 0;
    buf.fn_0040e744(&c);
}

// FUNCTION: STARFLEETOP 0x0040e675
void fn_0040e675(fn_0040d63f_Buf& buf)
{
    char text[32];
    buf.fn_0040e70f(text);
}

// FUNCTION: STARFLEETOP 0x0040e689
void fn_0040e689(fn_0040d63f_Buf& buf)
{
    buf.m18++;
}

// FUNCTION: STARFLEETOP 0x0040e691
void fn_0040e691(fn_0040d63f_Buf& buf)
{
    buf.m18--;
}

// HYPOTHESIS: reads bytes until a terminating zero; returns the count read.
// FUNCTION: STARFLEETOP 0x0040e699
int fn_0040d63f_Buf::fn_0040e699(char* p)
{
    int n = 0;
    int r;
    while ((r = fn_0040d897(p, 1, 0)) == 1) {
        if (*p == 0)
            break;
        p++;
        n++;
    }
    if (r == 0 && p != 0)
        *p = 0;
    return n;
}

