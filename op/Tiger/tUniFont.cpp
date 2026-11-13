typedef unsigned long ulong;

void __cdecl operator delete(void*);

// HYPOTHESIS: the font ID packs typeface/style bits; 0x40000 bold, 0x20000 italic,
// 0x10000 plain, all style bits in 0xff0000.
// HYPOTHESIS: the globals have C linkage (the SDK object names them _gOverrideBoldFonts).
extern "C" bool gOverrideBoldFonts;
extern "C" bool gOverrideItalicFonts;

#include "Tiger/tFontmgr.h"

enum eTypeface {};
enum eFontStyle {};
enum ePointSize {};

class tUniFont {
public:
    tUniFont(eTypeface, eFontStyle, ePointSize);
    virtual ~tUniFont() {}
    void* scalar_deleting_destructor(unsigned char);
    void mSetFontID(ulong);
    const tCharInfo* mGetCharInfo(unsigned short) const;
    void mChangePtSize(ePointSize);
    void mSetFont(eTypeface, eFontStyle, ePointSize);
    const ulong mGetCharWidth(unsigned short) const;
    const ulong mGetTextWidth(const unsigned short*) const;
    const ulong mGetFontHeight(unsigned short) const;

    eTypeface mTypeface;   // HYPOTHESIS: name
    eFontStyle mStyle;     // HYPOTHESIS: name
    ePointSize mPointSize; // HYPOTHESIS: name
    ulong mFontID; // HYPOTHESIS: name
};

// FUNCTION: STARFLEETOP 0x0066c270
void tUniFont::mSetFontID(ulong id)
{
    mFontID = id;
    // The original tests with | rather than &, so these conditions are always true.
    if (gOverrideBoldFonts && (mFontID | 0x40000))
        mFontID &= ~0x40000;
    if (gOverrideItalicFonts && (mFontID | 0x20000))
        mFontID &= ~0x20000;
    if (!(mFontID & 0xff0000))
        mFontID |= 0x10000;
}

// FUNCTION: STARFLEETOP 0x0066c2d0
tUniFont::tUniFont(eTypeface face, eFontStyle style, ePointSize size)
{
    mTypeface = face;
    mStyle = style;
    mPointSize = size;
    ulong id;
    if (size < 0x100 && face < 0x100 && style < 0x100)
        id = (((ulong)style << 8) | face) << 8 | size;
    else
        id = 0x1010c;
    // MATCH-ONLY: the original calls mSetFontID(id) here and the SDK's Tiger library was
    // compiled with auto-inlining (/Ob2), which inlined it while keeping the out-of-line copy
    // at 0x0066c270. The unit's profile has /O2 alone, so the body is written out instead.
    mFontID = id;
    if (gOverrideBoldFonts && (mFontID | 0x40000))
        mFontID &= ~0x40000;
    if (gOverrideItalicFonts && (mFontID | 0x20000))
        mFontID &= ~0x20000;
    if (!(mFontID & 0xff0000))
        mFontID |= 0x10000;
}

// FUNCTION: STARFLEETOP 0x0066c370
// HYPOTHESIS: the destructor body is empty; this is the compiler-generated scalar deleting
// destructor of the virtual destructor, annotated at its own address.
void* tUniFont::scalar_deleting_destructor(unsigned char flags)
{
    // MATCH-ONLY: spells out the compiler-generated deleting destructor, since the
    // harness annotates a function by definition and cannot name the generated one.
    this->tUniFont::~tUniFont();
    if (flags & 1)
        ::operator delete(this);
    return this;
}

// FUNCTION: STARFLEETOP 0x0066c3a0
const tCharInfo* tUniFont::mGetCharInfo(unsigned short ch) const
{
    return _GetCharInfo(ch, mFontID);
}

// FUNCTION: STARFLEETOP 0x0066c3c0
void tUniFont::mChangePtSize(ePointSize size)
{
    // HYPOTHESIS: the original reads the typeface and style before storing the new size
    // (the loads come first in the exe); written here as locals.
    eTypeface face = mTypeface;
    eFontStyle style = mStyle;
    mPointSize = size;
    ulong id;
    if (size < 0x100 && face < 0x100 && style < 0x100)
        id = (((ulong)style << 8) | face) << 8 | size;
    else
        id = 0x1010c;
    // MATCH-ONLY: inlined mSetFontID(id), as in the constructor.
    mFontID = id;
    if (gOverrideBoldFonts && (mFontID | 0x40000))
        mFontID &= ~0x40000;
    if (gOverrideItalicFonts && (mFontID | 0x20000))
        mFontID &= ~0x20000;
    if (!(mFontID & 0xff0000))
        mFontID |= 0x10000;
}

// FUNCTION: STARFLEETOP 0x0066c450
void tUniFont::mSetFont(eTypeface face, eFontStyle style, ePointSize size)
{
    mTypeface = face;
    mStyle = style;
    mPointSize = size;
    ulong id;
    if (size < 0x100 && face < 0x100 && style < 0x100)
        id = (((ulong)style << 8) | face) << 8 | size;
    else
        id = 0x1010c;
    // MATCH-ONLY: inlined mSetFontID(id), as in the constructor.
    mFontID = id;
    if (gOverrideBoldFonts && (mFontID | 0x40000))
        mFontID &= ~0x40000;
    if (gOverrideItalicFonts && (mFontID | 0x20000))
        mFontID &= ~0x20000;
    if (!(mFontID & 0xff0000))
        mFontID |= 0x10000;
}

// FUNCTION: STARFLEETOP 0x0066c4f0
const ulong tUniFont::mGetCharWidth(unsigned short ch) const
{
    const tCharInfo* info = _GetCharInfo(ch, mFontID);
    if (info)
        return info->field_4;
    return 0;
}

// FUNCTION: STARFLEETOP 0x0066c510
const ulong tUniFont::mGetTextWidth(const unsigned short* text) const
{
    ulong width = 0;
    for (const unsigned short* p = text; *p != 0; p++) {
        // HYPOTHESIS: the original added mGetCharWidth(*p), inlined (see the constructor's
        // note on /Ob2); the local stands for that call's parameter, and the register
        // assignment matches only with it.
        unsigned short ch = *p;
        const tCharInfo* info = _GetCharInfo(ch, mFontID);
        if (info != 0)
            width += info->field_4;
    }
    return width;
}

// FUNCTION: STARFLEETOP 0x0066c550
const ulong tUniFont::mGetFontHeight(unsigned short ch) const
{
    return _GetCharHeight(mFontID, ch);
}
