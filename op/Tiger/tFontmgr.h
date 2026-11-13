//	tFontmgr.h: tCharInfo, one glyph of a font, and the functions that look glyphs up.

#ifndef tFontmgr_h
#define tFontmgr_h

// HYPOTHESIS: layout recovered from tFontTable::mMakeGlyph, which allocates the record and
// its glyph data in one block (data follows the 0x1c-byte record). Field names are offsets.
struct tCharInfo
{
    unsigned short  field_0;
    long            field_4;    // HYPOTHESIS: the glyph's width (tUniFont::mGetCharWidth returns it)
    unsigned long   field_8;    // copied from the font header +0x14
    unsigned long   field_c;    // copied from the font header +0x18
    unsigned long   field_10;   // size of the glyph data
    unsigned char*  field_14;   // the glyph data
    long            field_18;
};

const tCharInfo* _GetCharInfo(unsigned short c, unsigned long typeface);
unsigned long _GetCharHeight(unsigned long typeface, unsigned short c);

#endif
