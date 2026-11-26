#include "unsorted/tBitmapFontEx.h"

// HYPOTHESIS: a constructor that zeroes six dwords; the owning class is unknown
// FUNCTION: STARFLEETOP 0x00537bc2
int* __fastcall fn_00537bc2(int* self)
{
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
    self[4] = 0;
    self[5] = 0;
    return self;
}

extern int g_00c3244c;
extern int g_00c32450;

// HYPOTHESIS: links the object at the head of a global list (head g_00c3244c, count g_00c32450)
// FUNCTION: STARFLEETOP 0x00537bd8
void __fastcall fn_00537bd8(int* self)
{
    self[6] = 0;
    self[7] = g_00c3244c;
    if (g_00c3244c != 0) {
        ((int*)g_00c3244c)[6] = (int)self;
    }
    g_00c32450++;
    g_00c3244c = (int)self;
}

// HYPOTHESIS: unlinks the object from the global list started by fn_00537bd8
// FUNCTION: STARFLEETOP 0x00537bfd
void __fastcall fn_00537bfd(int* self)
{
    if (g_00c32450 == 0) {
        g_00c32450 = 1;
    }
    if (g_00c3244c == (int)self) {
        g_00c3244c = self[7];
    }
    if (self[6] != 0) {
        ((int*)self[6])[7] = self[7];
    }
    if (self[7] != 0) {
        ((int*)self[7])[6] = self[6];
    }
    self[7] = 0;
    self[6] = 0;
    g_00c32450--;
}

extern int g_00c32454;

// FUNCTION: STARFLEETOP 0x00537c4a
tBitmapFontEx::tBitmapFontEx()
{
    g_00c32454++;
}

void __fastcall fn_004afa40(void* self);
void __stdcall fn_00736273(void* self, unsigned size, int count, void (__fastcall *dtor)(void*));

// HYPOTHESIS: vector deleting destructor of an unknown 0x18-byte class (destructor 0x004afa40);
// the class is not in a header, so it is written as a free function
// FUNCTION: STARFLEETOP 0x00537c73
void* __fastcall fn_00537c73(void* self, int, unsigned char flags)
{
    if (flags & 2) {
        int* base = (int*)self - 1;
        fn_00736273(self, 0x18, *base, fn_004afa40);
        if (flags & 1) {
            operator delete(base);
        }
        return base;
    }
    fn_004afa40(self);
    if (flags & 1) {
        operator delete(self);
    }
    return self;
}

// FUNCTION: STARFLEETOP 0x00537cbe
tBitmapFontEx::~tBitmapFontEx()
{
    if (field_08 != 0) {
        fn_00537c73(field_08, 0, 3);
    }
    if (field_04 != 0) {
        operator delete(field_04);
    }
    g_00c32454--;
    if (field_20 != 0) {
        field_20 = 0;
    }
}

// FUNCTION: STARFLEETOP 0x00537cf5
void *tBitmapFontEx::vfunction1(unsigned short index)
{
    if (index < field_0C) {
        return (char *)field_08 + index * 0x18;
    }
    return 0;
}

#include "unsorted/tMetaClientMapPanel.h"

// FUNCTION: STARFLEETOP 0x00537d0f
int tBitmapFontEx::vfunction3( unsigned short *text, int length )
{
    int i;
    int width = 0;
    if (text == 0) {
        return 0;
    }
    if (length < 0) {
        while (text[length] != 0) {
            length++;
        }
    }

    for (i = 0; i < length; i++) {
        // HYPOTHESIS: the glyph record shares the width getter 0x005e8efa with other classes
        tMetaClientMapQueue *glyph = (tMetaClientMapQueue *)vfunction1(text[i]);
        if (glyph != 0) {
            width += glyph->fn_005e8efa();
        }
    }
    return width;
}

#include <string.h>

// FUNCTION: STARFLEETOP 0x00537d66
void tBitmapFontEx::vfunction4( void *buffer, int size )
{
    if (field_04 != 0 && size != 0) {
        strncpy((char *)buffer, (char *)field_04, size);
    }
}