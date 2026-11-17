#include "Tiger/tstring.h"
#include "Tiger/tvectormath.h"
#include "unsorted/sfcstreams.h"






































// FUNCTION: STARFLEETOP 0x0040d63f
tSFCStream::tSFCStream(int32 size)
{
    fOwned = 1;
    fBuffer = (int8u*)operator new(size);
    fMaxPtr = fBuffer + size;
    if (fBuffer == 0)
        fMaxPtr = 0;
    fVersionNumber = -1;
    fGetPtr = fBuffer;
    fPutPtr = fBuffer;
    fReadLock = 0;
    fWriteLock = 0;
    fTabNumber = 0;
}

// FUNCTION: STARFLEETOP 0x0040d67d
void tSFCStream::fn_0040d67d()
{
    fGetPtr = fBuffer;
    fPutPtr = fBuffer;
    fReadLock = 0;
    fWriteLock = 0;
    fTabNumber = 0;
}

// FUNCTION: STARFLEETOP 0x0040d692
void tSFCStream::fn_0040d692()
{
    if (fBuffer != 0) {
        if (fOwned == 1)
            operator delete(fBuffer);
        fBuffer = 0;
        fGetPtr = 0;
        fPutPtr = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0040d6b7
tSFCStream* tSFCStream::fn_0040d6b7(int* src)
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
tSFCStream* tSFCStream::fn_0040d705(int* src)
{
    int* p = (int*)src[0];
    fn_0040d867(&p, 4, 1);
    p = (int*)src[1];
    fn_0040d867(&p, 4, 1);
    return this;
}

// FUNCTION: STARFLEETOP 0x0040d73e
tSFCStream* tSFCStream::fn_0040d73e(tString* str)
{
    iBasicStringElement* p = str->mWideStr();
    int len = str->mStrLen();
    for (int i = 0; i < len + 1; i++)
        fn_0040d867(&p[i], 2, 1);
    return this;
}









// FUNCTION: STARFLEETOP 0x0040d777
tSFCStream& tSFCStream::operator>>(tPoint3& num)
{
    fn_0040d897(&num.x, 4, 1);
    fn_0040d897(&num.y, 4, 1);
    fn_0040d897(&num.z, 4, 1);
    return *this;
}








// FUNCTION: STARFLEETOP 0x0040d7ae
tSFCStream& tSFCStream::operator>>(tPoint2& num)
{
    fn_0040d897(&num.x, 4, 1);
    fn_0040d897(&num.y, 4, 1);
    return *this;
}

// FUNCTION: STARFLEETOP 0x0040d7d6
tSFCStream* tSFCStream::fn_0040d7d6(tString* str)
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
void tSFCStream::fn_0040d815()
{
    char b = 9;
    for (int i = 0; i < fTabNumber; i++)
        fn_0040d867(&b, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040d841
void tSFCStream::fn_0040d841()
{
    char b;
    for (int i = 0; i < fTabNumber; i++)
        fn_0040d897(&b, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040d867
void tSFCStream::fn_0040d867(void* data, int size, int count)
{
    if (fReadLock != 1 && data != 0) {
        char* src = (char*)data;
        while (size > 0 && fPutPtr < fMaxPtr) {
            *fPutPtr = *src;
            fPutPtr++;
            src++;
            size--;
        }
    }
}

// FUNCTION: STARFLEETOP 0x0040d897
int tSFCStream::fn_0040d897(void* data, int size, int count)
{
    int total = size;
    if (fWriteLock == 1)
        return 0;
    if (data == 0)
        return 0;
    char* dst = (char*)data;
    while (size > 0 && fGetPtr < fPutPtr) {
        *dst = *fGetPtr;
        fGetPtr++;
        dst++;
        size--;
    }
    return total - size;
}

// FUNCTION: STARFLEETOP 0x0040d8cf
void tSFCStream::fn_0040d8cf(int n)
{
    fPutPtr = fGetPtr + n;
    if (fPutPtr > fMaxPtr)
        fPutPtr = fMaxPtr;
}

// FUNCTION: STARFLEETOP 0x0040d8e8
void tSFCStream::fn_0040d8e8(int8u* p)
{
    fPutPtr = p;
    if (p > fMaxPtr)
        fPutPtr = fMaxPtr;
    if (fPutPtr < fGetPtr)
        fPutPtr = fGetPtr;
}

// FUNCTION: STARFLEETOP 0x0040d908
void fn_0040d908(char* s, tSFCStream& buf)
{
    buf.fn_0040e6d1(s);
}

// FUNCTION: STARFLEETOP 0x0040d916
void fn_0040d916(int n, tSFCStream& buf)
{
    buf.fn_0040e699((char*)n);
}

// HYPOTHESIS: variable-length encoding of a signed value: sign and low 6 bits in the first byte, then 7 bits per byte.
// FUNCTION: STARFLEETOP 0x0040d924
void fn_0040d924(int* p, tSFCStream& buf)
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
void fn_0040d9bf(unsigned int* p, tSFCStream& buf)
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
void fn_0040da33(unsigned int* p, tSFCStream& buf)
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
void fn_0040da79(unsigned int* p, tSFCStream& buf)
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
void fn_0040dac7(int* src, tSFCStream& buf)
{
    int* p = (int*)src[0];
    buf.fn_0040d867(&p, 4, 1);
}

// FUNCTION: STARFLEETOP 0x0040dae4
void fn_0040dae4(int* dst, tSFCStream& buf)
{
    buf.fn_0040d897(dst, 4, 1);
}

// FUNCTION: STARFLEETOP 0x0040daf6
void fn_0040daf6(int* src, tSFCStream& buf)
{
    int* p = (int*)src[0];
    fn_0040dac7((int*)&p, buf);
    p = (int*)src[1];
    fn_0040dac7((int*)&p, buf);
    p = (int*)src[2];
    fn_0040dac7((int*)&p, buf);
}

// FUNCTION: STARFLEETOP 0x0040db38
void fn_0040db38(float* dst, tSFCStream& buf)
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
void fn_0040db7b(int* src, tSFCStream& buf)
{
    int* p = (int*)src[0];
    fn_0040dac7((int*)&p, buf);
    p = (int*)src[1];
    fn_0040dac7((int*)&p, buf);
}

// FUNCTION: STARFLEETOP 0x0040dbab
void fn_0040dbab(float* dst, tSFCStream& buf)
{
    float v;
    fn_0040dae4((int*)&v, buf);
    dst[0] = v;
    fn_0040dae4((int*)&v, buf);
    dst[1] = v;
}

// FUNCTION: STARFLEETOP 0x0040dbdc
void fn_0040dbdc(tString* str, tSFCStream& buf)
{
    buf.fn_0040d73e(str);
}

// FUNCTION: STARFLEETOP 0x0040dbea
void fn_0040dbea(tString* str, tSFCStream& buf)
{
    buf.fn_0040d7d6(str);
}

// FUNCTION: STARFLEETOP 0x0040dbf8
void fn_0040dbf8(double* src, tSFCStream& buf)
{
    float f = (float)*src;
    fn_0040dac7((int*)&f, buf);
}

// FUNCTION: STARFLEETOP 0x0040dc13
void fn_0040dc13(double* dst, tSFCStream& buf)
{
    float f;
    fn_0040dae4((int*)&f, buf);
    *dst = (double)f;
}

// FUNCTION: STARFLEETOP 0x0040dc2e
void fn_0040dc2e(short* p, tSFCStream& buf)
{
    int v = *p;
    fn_0040d924(&v, buf);
}

// FUNCTION: STARFLEETOP 0x0040dc4a
void fn_0040dc4a(short* p, tSFCStream& buf)
{
    unsigned int v;
    fn_0040d9bf(&v, buf);
    *p = (short)v;
}

// FUNCTION: STARFLEETOP 0x0040dc67
void fn_0040dc67(unsigned char* p, tSFCStream& buf)
{
    unsigned char b = *p;
    buf.fn_0040d867(&b, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040dc84
void fn_0040dc84(unsigned char* p, tSFCStream& buf)
{
    buf.fn_0040d897(p, 1, 0);
}

// FUNCTION: STARFLEETOP 0x0040dc96
void fn_0040dc96(int size, void* p, tSFCStream& buf)
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
void fn_0040dceb(int size, void* p, tSFCStream& buf)
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

void fn_0040dd45(char* name, long* value, tSFCStream& buf)
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

void fn_0040dd83(char* name, long* value, tSFCStream& buf)
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
void fn_0040dde1(char* name, unsigned long* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %lu", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040de1f
void fn_0040de1f(char* name, unsigned long* value, tSFCStream& buf)
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
void fn_0040de7d(char* name, short* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %ld", name, (int)*value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040debd
void fn_0040debd(char* name, short* value, tSFCStream& buf)
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
void fn_0040df26(char* name, char* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %ld", name, (int)*value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040df66
void fn_0040df66(char* name, char* value, tSFCStream& buf)
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
void fn_0040dfcd(char* name, unsigned char* value, tSFCStream& buf)
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
void fn_0040e034(char* name, unsigned char* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %lu", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e074
void fn_0040e074(char* name, unsigned char* value, tSFCStream& buf)
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
void fn_0040e0db(char* name, float* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %f", name, (double)*value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e11e
void fn_0040e11e(char* name, float* value, tSFCStream& buf)
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
void fn_0040e17c(char* name, float* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = x=%f y=%f z=%f", name, (double)value[0], (double)value[1], (double)value[2]);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e1cf
void fn_0040e1cf(char* name, float* value, tSFCStream& buf)
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
void fn_0040e24a(char* name, float* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = x=%f y=%f", name, (double)value[0], (double)value[1]);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e295
void fn_0040e295(char* name, float* value, tSFCStream& buf)
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
void fn_0040e306(tString* name, tString* value, tSFCStream& buf)
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
void fn_0040e37a(tString* name, tString* value, tSFCStream& buf)
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
void fn_0040e42b(char* name, double* value, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    sprintf(text, "%s = %lf", name, *value);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e46e
void fn_0040e46e(char* name, double* value, tSFCStream& buf)
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
void fn_0040e4cc(char* name, int size, void* value, tSFCStream& buf)
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
void fn_0040e52b(char* name, int size, void* value, tSFCStream& buf)
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
void fn_0040e591(char* a, char* b, tSFCStream& buf)
{
    char text[256];
    buf.fn_0040d815();
    // HYPOTHESIS: the format text is a guess; the string relocation is unverified.
    sprintf(text, "%s = %s", a, b);
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e5cd
void fn_0040e5cd(char* a, char* b, tSFCStream& buf)
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
void fn_0040e631(char* text, tSFCStream& buf)
{
    buf.fn_0040d815();
    buf.fn_0040e744(text);
}

// FUNCTION: STARFLEETOP 0x0040e648
void fn_0040e648(char* text, tSFCStream& buf)
{
    buf.fn_0040d841();
    buf.fn_0040e70f(text);
}

// FUNCTION: STARFLEETOP 0x0040e65f
void fn_0040e65f(tSFCStream& buf)
{
    char c = 0;
    buf.fn_0040e744(&c);
}

// FUNCTION: STARFLEETOP 0x0040e675
void fn_0040e675(tSFCStream& buf)
{
    char text[32];
    buf.fn_0040e70f(text);
}

// FUNCTION: STARFLEETOP 0x0040e689
void fn_0040e689(tSFCStream& buf)
{
    buf.mIncrementTabNumber();
}

// FUNCTION: STARFLEETOP 0x0040e691
void fn_0040e691(tSFCStream& buf)
{
    buf.mDecrementTabNumber();
}

// HYPOTHESIS: reads bytes until a terminating zero; returns the count read.
// FUNCTION: STARFLEETOP 0x0040e699
int tSFCStream::fn_0040e699(char* p)
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

// FUNCTION: STARFLEETOP 0x0040e6d1
void tSFCStream::fn_0040e6d1(char* s)
{
    unsigned int len = s ? strlen(s) + 1 : 0;
    if (len > 0) {
        fn_0040d867(s, len, 0);
    } else {
        // HYPOTHESIS: the original wrote a lone zero byte through an address inside its own argument slot.
        char* z = (char*)&s + 3;
        *z = 0;
        fn_0040d867(z, 1, 0);
    }
}

// FUNCTION: STARFLEETOP 0x0040e744
void tSFCStream::fn_0040e744(char* text)
{
    unsigned int len = text ? strlen(text) : 0;
    if (len > 0)
        fn_0040d867(text, len, 0);
    // HYPOTHESIS: the original wrote the newline through an address inside its own argument slot.
    char* nl = (char*)&text + 3;
    *nl = '\n';
    fn_0040d867(nl, 1, 0);
}

// HYPOTHESIS: a flag bit selects the writer or the reader of the variable-length encoding.
// FUNCTION: STARFLEETOP 0x0040e786
void fn_0040e786(unsigned int* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2)
        fn_0040d924((int*)p, buf);
    else
        fn_0040d9bf(p, buf);
}

// HYPOTHESIS: unsigned counterpart of fn_0040e786.
// FUNCTION: STARFLEETOP 0x0040e7a5
void fn_0040e7a5(unsigned int* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2)
        fn_0040da33(p, buf);
    else
        fn_0040da79(p, buf);
}

// HYPOTHESIS: short counterpart of fn_0040e786.
// FUNCTION: STARFLEETOP 0x0040e7c4
void fn_0040e7c4(short* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2)
        fn_0040dc2e(p, buf);
    else
        fn_0040dc4a(p, buf);
}

// HYPOTHESIS: unsigned char counterpart of fn_0040e7c4.
// FUNCTION: STARFLEETOP 0x0040e7e3
void fn_0040e7e3(unsigned char* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2)
        fn_0040dc67(p, buf);
    else
        fn_0040dc84(p, buf);
}

// HYPOTHESIS: bit 8 selects a raw four-byte transfer instead of the helper.
// FUNCTION: STARFLEETOP 0x0040e802
void fn_0040e802(int* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2) {
        if (flags & 8) {
            p = (int*)*p;
            buf.fn_0040d867(&p, 4, 1);
            return;
        }
        fn_0040dac7(p, buf);
    } else {
        if (flags & 8) {
            buf.fn_0040d897(p, 4, 1);
            return;
        }
        fn_0040dae4(p, buf);
    }
}

// HYPOTHESIS: the three-float counterpart of 0x0040e802.
// FUNCTION: STARFLEETOP 0x0040e85e
void fn_0040e85e(tPoint3* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2) {
        if (flags & 8) {
            buf.fn_0040d6b7((int*)p);
            return;
        }
        fn_0040daf6((int*)p, buf);
    } else {
        if (flags & 8) {
            buf >> *p;
            return;
        }
        fn_0040db38((float*)p, buf);
    }
}

// FUNCTION: STARFLEETOP 0x0040e8a9
void fn_0040e8a9(tString* str, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2) {
        if (flags & 8) {
            buf.fn_0040d73e(str);
            return;
        }
        fn_0040dbdc(str, buf);
    } else {
        if (flags & 8) {
            buf.fn_0040d7d6(str);
            return;
        }
        fn_0040dbea(str, buf);
    }
}

// HYPOTHESIS: the two-float counterpart of 0x0040e85e.
// FUNCTION: STARFLEETOP 0x0040e8f4
void fn_0040e8f4(tPoint2* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2) {
        if (flags & 8) {
            buf.fn_0040d705((int*)p);
            return;
        }
        fn_0040db7b((int*)p, buf);
    } else {
        if (flags & 8) {
            buf >> *p;
            return;
        }
        fn_0040dbab((float*)p, buf);
    }
}

// HYPOTHESIS: the double counterpart of 0x0040e8f4.
// FUNCTION: STARFLEETOP 0x0040e93f
void fn_0040e93f(double* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2) {
        if (flags & 8) {
            double d = *p;
            buf.fn_0040d867(&d, 8, 1);
            return;
        }
        fn_0040dbf8(p, buf);
    } else {
        if (flags & 8) {
            buf.fn_0040d897(p, 8, 1);
            return;
        }
        fn_0040dc13(p, buf);
    }
}

// FUNCTION: STARFLEETOP 0x0040e99d
void fn_0040e99d(int size, void* p, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2)
        fn_0040dc96(size, p, buf);
    else
        fn_0040dceb(size, p, buf);
}

// FUNCTION: STARFLEETOP 0x0040e9c0
void fn_0040e9c0(int n, tSFCStream& buf, unsigned char flags)
{
    if (flags & 2)
        fn_0040d908((char*)n, buf);
    else
        fn_0040d916(n, buf);
}

// FUNCTION: STARFLEETOP 0x0040ea18
void fn_0040ea18(unsigned long* value, tSFCStream& buf, unsigned char flags, char* name)
{
    if (flags & 4) {
        if (flags & 2)
            fn_0040dde1(name, value, buf);
        else
            fn_0040de1f(name, value, buf);
    } else
        fn_0040e7a5((unsigned int*)value, buf, flags);
}

// FUNCTION: STARFLEETOP 0x0040ea51
void fn_0040ea51(short* value, tSFCStream& buf, unsigned char flags, char* name)
{
    if (flags & 4) {
        if (flags & 2)
            fn_0040de7d(name, value, buf);
        else
            fn_0040debd(name, value, buf);
    } else
        fn_0040e7c4(value, buf, flags);
}

// FUNCTION: STARFLEETOP 0x0040ea8a
void fn_0040ea8a(unsigned char* value, tSFCStream& buf, unsigned char flags, char* name)
{
    if (flags & 4) {
        if (flags & 2)
            fn_0040df26(name, (char*)value, buf);
        else
            fn_0040df66(name, (char*)value, buf);
    } else
        fn_0040e7e3(value, buf, flags);
}

// FUNCTION: STARFLEETOP 0x0040e9df
void fn_0040e9df(long* value, tSFCStream& buf, unsigned char flags, char* name)
{
    if (flags & 4) {
        if (flags & 2)
            fn_0040dd45(name, value, buf);
        else
            fn_0040dd83(name, value, buf);
    } else
        fn_0040e786((unsigned int*)value, buf, flags);
}

// FUNCTION: STARFLEETOP 0x0040e70f
int tSFCStream::fn_0040e70f(char* text)
{
    int n = 0;
    while (fn_0040d897(text, 1, 0) == 1 && *text != '\n') {
        text++;
        n++;
    }
    if (text != 0)
        *text = 0;
    return n;
}

