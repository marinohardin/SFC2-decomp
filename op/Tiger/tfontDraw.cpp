// Unit:      Tiger/tfontDraw.cpp
#include "Tiger/tUniFont.h"

// HYPOTHESIS: a clip rectangle; the four fields are left, top, right, bottom.
struct tGLRectI
{
    int left;
    int top;
    int right;
    int bottom;
};

// HYPOTHESIS: a class of static drawing helpers.
class tFontDraw
{
public:
    static void mDrawString(unsigned int bits, long pitch, void* base, const tGLRectI& clip, int x, int y, unsigned int color, const unsigned short* text, tUniFont& font, tGLRectI* outRect, unsigned char draw, int count);
    enum eFontDrawFmt
    {
        eLeft,
        eCenter,
        eRight,
        eJustify
    };
    static void mDrawString(unsigned int bits, long pitch, void* base, const tGLRectI& clip, int x, int y, unsigned int color, const unsigned short* text, tUniFont& font, int width, eFontDrawFmt fmt, tGLRectI* outRect, unsigned char draw, int count);
    static void* mDrawGlyphClipped(const tGLRectI& clip, int x, int y, unsigned int color, unsigned int bits, long pitch, void* dest, const tCharInfo& info);
};

// FUNCTION: STARFLEETOP 0x00759e80
void* tFontDraw::mDrawGlyphClipped(const tGLRectI& clip, int x, int y, unsigned int color, unsigned int bits, long pitch, void* dest, const tCharInfo& info)
{
    const unsigned char* src = info.field_14;
    int height = info.field_8;
    int width = info.field_4;
    unsigned char* out = (unsigned char*)dest;
    int rowBytes = (int)((width + 31) & ~31) / 8;
    int visible = width;
    unsigned int bpp = bits >> 3;
    int skip;
    int skipBytes;
    int rows;
    int top;
    if (clip.left - x > 0)
        skip = clip.left - x;
    else
        skip = 0;
    if (skip > 0)
    {
        visible -= skip;
        skipBytes = skip * bpp;
    }
    if (width + x > clip.right)
        visible += clip.right - width - x;
    if (visible > 0)
    {
        rows = height;
        top = clip.top - y;
        if (top > 0)
        {
            rows -= top;
            src += top * rowBytes;
            out += top * pitch;
        }
        if (y + height > clip.bottom)
            rows += clip.bottom - height - y;
        int end = skip + visible;
        if (rows > 0)
        {
            unsigned int cur = color;
            do
            {
                const unsigned char* nextSrc = src + rowBytes;
                unsigned char* nextOut = out + pitch;
                int i;
                if (skip != 0)
                {
                    src += skip / 8;
                    if ((skip & 7) != 0)
                    {
                        cur = (unsigned int)*src << (skip & 7);
                        src++;
                    }
                    out += skipBytes;
                }
                for (i = skip; i < end; i++)
                {
                    if ((i & 7) == 0)
                    {
                        cur = *src;
                        src++;
                    }
                    if ((cur & 0x80) != 0)
                    {
                        switch (bits)
                        {
                        case 8:
                            *out = (unsigned char)color;
                            break;
                        case 16:
                            *(unsigned short*)out = (unsigned short)color;
                            break;
                        case 24:
                            out[0] = (unsigned char)color;
                            out[1] = (unsigned char)(color >> 8);
                            out[2] = (unsigned char)(color >> 16);
                            break;
                        case 32:
                            *(unsigned int*)out = color;
                        }
                    }
                    out += bpp;
                    cur <<= 1;
                }
                src = nextSrc;
                out = nextOut;
            } while (--rows != 0);
        }
    }
    return (unsigned char*)dest + bpp * width;
}

// FUNCTION: STARFLEETOP 0x0075a080
void tFontDraw::mDrawString(unsigned int bits, long pitch, void* base, const tGLRectI& clip, int x, int y, unsigned int color, const unsigned short* text, tUniFont& font, tGLRectI* outRect, unsigned char draw, int count)
{
    if (x < clip.right && y < clip.bottom)
    {
        int bottom = font.mGetFontHeight(*text) + y;
        if (bottom > clip.top)
        {
            base = (unsigned char*)base + pitch * y;
            if (count == -1)
            {
                count = 0;
                const unsigned short* p = text;
                while (*p != 0)
                {
                    p++;
                    count++;
                }
            }
            unsigned short ch = *text++;
            int cx = x;
            void* dest;
            int i = 0;
            while (i < count)
            {
                const tCharInfo* info = font.mGetCharInfo(ch);
                if (info != 0)
                {
                    if (info->field_4 + cx >= clip.left)
                    {
                        if (i < count && draw != 0)
                            dest = (unsigned char*)base + (bits >> 3) * cx;
                        break;
                    }
                    cx += info->field_4;
                }
                ch = *text++;
                i++;
            }
            // HYPOTHESIS: dest is uninitialized when no glyph reaches the clip.
            int left = cx;
            for (; i < count; i++)
            {
                if (draw != 0)
                {
                    const tCharInfo* info = font.mGetCharInfo(ch);
                    if (info != 0)
                    {
                        dest = mDrawGlyphClipped(clip, cx, y, color, bits, pitch, dest, *info);
                        cx += info->field_4;
                    }
                }
                ch = *text++;
            }
            if (outRect != 0)
            {
                if (left < clip.left)
                    left = clip.left;
                if (y < clip.top)
                    y = clip.top;
                if (cx > clip.right)
                    cx = clip.right;
                if (bottom > clip.bottom)
                    bottom = clip.bottom;
                outRect->left = left;
                outRect->top = y;
                outRect->right = cx;
                outRect->bottom = bottom;
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x0075a240
void tFontDraw::mDrawString(unsigned int bits, long pitch, void* base, const tGLRectI& clip, int x, int y, unsigned int color, const unsigned short* text, tUniFont& font, int width, eFontDrawFmt fmt, tGLRectI* outRect, unsigned char draw, int count)
{
    unsigned int bpp = bits >> 3;
    unsigned long height = font.mGetFontHeight(*text);
    long step = height * pitch;
    const unsigned short* str = text;
    void* row = (unsigned char*)base + pitch * y;
    int lineY = y;
    // HYPOTHESIS: the bounds are one rectangle; its four ints sit together above the other locals.
    tGLRectI bounds;
    bounds.left = 0x7fffffff;
    bounds.top = 0x7fffffff;
    bounds.right = -0x7fffffff - 1;
    bounds.bottom = -0x7fffffff - 1;
    void* dest;
    if (count == -1)
    {
        count = 0;
        for (const unsigned short* p = text; *p != 0; p++)
            count++;
    }
    while (str != 0)
    {
        int spaces = 0;
        const unsigned short* start = str;
        const unsigned short* p = str;
        const unsigned short* next;
        int nBreak = 0;
        const unsigned short* breakAt = 0;
        int spaceW;
        int w = 0;
        int lineW;
        int n;
        int i;
        for (i = 0; i < count; p++, i++)
        {
            unsigned short ch = *p;
            if (ch == 10)
            {
                lineW = w;
                n = i;
                next = p + 1;
                goto found;
            }
            int cw = font.mGetCharWidth(ch);
            if (cw != 0)
            {
                if (ch == 0x20)
                {
                    spaceW = w;
                    nBreak = i;
                    breakAt = p + 1;
                }
                if (w + cw > width)
                {
                    if (breakAt != 0)
                    {
                        lineW = spaceW;
                        n = nBreak;
                        next = breakAt;
                    }
                    else
                    {
                        lineW = w;
                        n = i;
                        next = p;
                    }
                    goto found;
                }
                w += cw;
            }
        }
        lineW = w;
        n = i;
        next = 0;
found:
        int lx;
        int extra = lineW;
        switch (fmt)
        {
        case 0:
            lx = x;
            break;
        case 1:
            lx = (width - lineW) / 2 + x;
            break;
        case 2:
            lx = x - lineW + width;
            break;
        case 3:
            lx = x;
            if (next != 0)
            {
                for (int k = 0; k < n; k++)
                {
                    if (str[k] == 0x20)
                        spaces++;
                }
                extra = width - lineW;
                if (extra < 0)
                    extra = 0;
            }
            break;
        default:
            lx = x;
            break;
        }
        if (draw != 0)
            dest = (unsigned char*)row + lx * bpp;
        for (i = 0; i < n; i++)
        {
            unsigned short ch = *str++;
            if (draw != 0)
            {
                const tCharInfo* info = font.mGetCharInfo(ch);
                if (info != 0)
                {
                    dest = mDrawGlyphClipped(clip, lx, lineY, color, bits, pitch, dest, *info);
                    lx += info->field_4;
                }
            }
            if (spaces != 0 && ch == 0x20)
            {
                int add = extra / spaces;
                extra -= add;
                lx += add;
                dest = (unsigned char*)dest + add * bpp;
                spaces--;
            }
        }
        if (lineY < bounds.top)
            bounds.top = lineY;
        row = (unsigned char*)row + step;
        lineY += height;
        if (lineY > bounds.bottom)
            bounds.bottom = lineY;
        if (lx < bounds.left)
            bounds.left = lx;
        lx += lineW;
        if (lx > bounds.right)
            bounds.right = lx;
        if (lineY >= clip.bottom)
            break;
        if (next != 0)
        {
            count -= next - start;
            if (count <= 0)
                break;
        }
        str = next;
    }
    if (outRect != 0)
    {
        if (bounds.left < clip.left)
            outRect->left = clip.left;
        else
            outRect->left = bounds.left;
        if (bounds.top < clip.top)
            outRect->top = clip.top;
        else
            outRect->top = bounds.top;
        if (bounds.right > clip.right)
            outRect->right = clip.right;
        else
            outRect->right = bounds.right;
        if (bounds.bottom > clip.bottom)
            outRect->bottom = clip.bottom;
        else
            outRect->bottom = bounds.bottom;
    }
}

// HYPOTHESIS: not a real function; these two bytes (mov edi,edi) lie inside mDrawGlyphClipped's range and are probably mis-split data.
// FUNCTION: STARFLEETOP 0x0075a042
void fn_0075a042()
{
}
