#ifndef TIGER_TUNIFONT_H
#define TIGER_TUNIFONT_H

#include "Tiger/tFontmgr.h"

enum eTypeface {};
enum eFontStyle {};
enum ePointSize {};

class tUniFont {
public:
    tUniFont(eTypeface, eFontStyle, ePointSize);
    virtual ~tUniFont() {}
    void* scalar_deleting_destructor(unsigned char);
    void mSetFontID(unsigned long);
    const tCharInfo* mGetCharInfo(unsigned short) const;
    void mChangePtSize(ePointSize);
    void mSetFont(eTypeface, eFontStyle, ePointSize);
    const unsigned long mGetCharWidth(unsigned short) const;
    const unsigned long mGetTextWidth(const unsigned short*) const;
    const unsigned long mGetFontHeight(unsigned short) const;

    eTypeface mTypeface;   // HYPOTHESIS: name
    eFontStyle mStyle;     // HYPOTHESIS: name
    ePointSize mPointSize; // HYPOTHESIS: name
    unsigned long mFontID; // HYPOTHESIS: name
};

#endif
