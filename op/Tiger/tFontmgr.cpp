//	tFontmgr.cpp: tFontTable, the font glyph table.

#include <string.h>

#include "Tiger/tFontmgr.h"
#include "Tiger/tstring.h"

// HYPOTHESIS: the 14-byte glyph record as stored in the font file; its long fields sit at
// odd 2-byte offsets, so the file format is packed. Its name is not known.
#pragma pack(push, 2)
struct fn_00670710_record
{
    unsigned short  field_0;
    long            field_2;
    unsigned long   field_6;    // size of the glyph data that follows
    unsigned long   field_a;
};
#pragma pack(pop)

// HYPOTHESIS: one entry of the typeface table, a typeface number and its name; the
// struct's name is not known.
struct fn_00c681f0_entry {
    long            field_0;
    tString         field_4;
};

// HYPOTHESIS: 56 entries, the bound mGetTypefacebyName scans.
extern fn_00c681f0_entry gTypefaceTable[56];
extern long gTypefaceTableIndex;
extern long gCustomTypeface;

#include "Tiger/tbasicfile.h"

// HYPOTHESIS: tFontFileIOBase supplies the file factory and the name of the font index file;
// the exe names both static members (mNewBasicFile, mGetFontIndexFilename).
class tFontFileIOBase {
public:
    static tBasicFile* mNewBasicFile(void);
    static const char* mGetFontIndexFilename(void);
};

// HYPOTHESIS: tFontFileIO holds the open file at +0x00 and the name it was opened with at
// +0x04; its inline wrappers below are inferred from the code inlined into mInit, and
// their names follow the tBasicFile calls they forward to.
class tFontFileIO {
public:
    tFontFileIO()
    {
        field_0 = tFontFileIOBase::mNewBasicFile();
    }
    ~tFontFileIO();
    unsigned char mOpenForRead(const char* name)
    {
        if (field_0 == 0)
            return 0;
        field_0->mClose();
        if (field_0->mOpenForRead(name)) {
            strcpy(field_4, name);
            return 1;
        }
        field_4[0] = 0;
        return 0;
    }
    unsigned long mRead(void* buffer, unsigned long size)
    {
        if (field_0 == 0)
            return 0;
        return field_0->mRead(buffer, size);
    }
    unsigned char mSeekSet(long position)
    {
        if (field_0 == 0)
            return 0;
        return field_0->mSeekSet(position);
    }
    void mClose(void)
    {
        if (field_0 != 0)
            field_0->mClose();
    }

    tBasicFile*     field_0;
    char            field_4[260]; // HYPOTHESIS: size unknown
};

// FUNCTION: STARFLEETOP 0x00670ca0
inline tFontFileIO::~tFontFileIO()
{
    if (field_0 != 0) {
        field_0->mClose();
        field_0->mDispose();
    }
}

// HYPOTHESIS: the 32-byte font file header that tFontTable::mInit reads into +0x04; its
// name is not known. Field names are offsets within the header.
struct fn_006702f0_header {
    char            field_0[4];     // "FONT"
    unsigned long   field_4;        // HYPOTHESIS: file offset of the typeface name
    unsigned long   field_8;        // HYPOTHESIS: format version, 1 to 3
    long            field_c;
    unsigned short  field_10;       // HYPOTHESIS: first character
    unsigned short  field_12;       // HYPOTHESIS: last character
    unsigned long   field_14;
    unsigned long   field_18;
    unsigned long   field_1c;
};

// HYPOTHESIS: layout recovered from mResetDataMembers and mInit; field names are offsets
// until other members of the class give evidence. +0x04..+0x23 is the 32-byte file
// header that mInit reads, which mResetDataMembers does not reset.
class tFontTable {
    friend class tFontMgr;
public:
    tFontTable()
    {
        mResetDataMembers();
    }
    ~tFontTable()
    {
        mDone();
    }
    unsigned char mInit(const char* name, tFontFileIO& file);
protected:
    void mResetDataMembers(void);
    unsigned char mStoreCustomTypeface(unsigned short* typeface);
    void mDone(void);
public:
    tCharInfo* mMakeGlyph(unsigned short c, tFontFileIO* file);
protected:

    tFontTable*     field_0;    // HYPOTHESIS: the next table in tFontMgr's list
    fn_006702f0_header field_4;
    unsigned long*  field_24;
    char*           field_28;
    tCharInfo**     field_2c[256]; // HYPOTHESIS: 256 pages of 256 glyph pointers each
};


// FUNCTION: STARFLEETOP 0x006702c0
void tFontTable::mResetDataMembers(void)
{
    field_0 = 0;
    field_4.field_c = -1;
    field_4.field_10 = 0;
    field_4.field_12 = 0;
    field_4.field_14 = 0;
    field_4.field_18 = 0;
    field_4.field_1c = 0;
    field_24 = 0;
    field_28 = 0;
    for (int i = 0; i < 256; i++)
        field_2c[i] = 0;
}

// FUNCTION: STARFLEETOP 0x006702f0
unsigned char tFontTable::mInit(const char* name, tFontFileIO& file)
{
    mDone();

    field_28 = new char[strlen(name) + 1];
    if (field_28 == 0) {
        mDone();
        return 0;
    }
    strcpy(field_28, name);

    char* header = field_4.field_0;

    if (!file.mOpenForRead(name) ||
        file.mRead(header, 0x20) != 0x20 ||
        header[0] != 'F' || header[1] != 'O' ||
        header[2] != 'N' || header[3] != 'T' ||
        (field_4.field_8 != 1 && field_4.field_8 != 2 && field_4.field_8 != 3)) {
        mDone();
        return 0;
    }

    int count = field_4.field_12 - field_4.field_10 + 1;
    field_24 = new unsigned long[count];
    if (field_24 == 0 ||
        file.mRead(field_24, count * sizeof(unsigned long)) != count * sizeof(unsigned long)) {
        mDone();
        return 0;
    }

    if (field_4.field_8 == 2) {
        unsigned short typeface[32];
        memset(typeface, 0, sizeof(typeface));
        file.mSeekSet(field_4.field_4);
        if (file.mRead(typeface, 0x20) == 0x20) {
            if (!mStoreCustomTypeface(typeface))
                mDone();
            return 1;
        }
        mDone();
    } else if (field_4.field_8 == 3) {
        unsigned long length = 0;
        file.mSeekSet(field_4.field_4);
        if (file.mRead(&length, 4) == 4) {
            unsigned short* typeface = new unsigned short[length + 1];
            unsigned long size = length * sizeof(unsigned short);
            if (file.mRead(typeface, size) == size) {
                typeface[length] = 0;
                if (!mStoreCustomTypeface(typeface))
                    mDone();
                return 1;
            }
        }
        mDone();
    }
    return 1;
}

// FUNCTION: STARFLEETOP 0x00670520
unsigned char tFontTable::mStoreCustomTypeface(unsigned short* typeface)
{
    // HYPOTHESIS: bits 8..15 of the header word at +0x0c hold the font's typeface; 201
    // marks a font that names a custom typeface of its own.
    // HYPOTHESIS: the original reads the byte into a wider local (an eTypeface, perhaps);
    // the byte load zero-extended into eax and the 32-bit compare need it.
    unsigned long current = ((unsigned char*)&field_4.field_c)[1];
    if (current != 201)
        return 0;

    int i;
    long face;  // left unset when the table is full; the original stores it as it is

    for (i = 0; i < gTypefaceTableIndex; i++) {
        if (typeface == gTypefaceTable[i].field_4) {
            face = gTypefaceTable[i].field_0;
            goto store;
        }
    }
    if (gCustomTypeface < 256) {
        gTypefaceTable[gTypefaceTableIndex].field_0 = gCustomTypeface;
        gTypefaceTable[gTypefaceTableIndex].field_4 = typeface;
        face = gCustomTypeface++;
        gTypefaceTableIndex++;
    }
store:
    field_4.field_c = (field_4.field_c & 0xffff00ff) | (face << 8);
    return 1;
}

// FUNCTION: STARFLEETOP 0x00670650
void tFontTable::mDone(void)
{
    if (field_24 != NULL) {
        delete[] field_24;
        field_24 = NULL;
    }
    if (field_28 != NULL) {
        delete[] field_28;
        field_28 = NULL;
    }
    int i;
    for (i = 0; i < 256; i++) {
        tCharInfo** page = field_2c[i];
        if (page) {
            for (int j = 0; j < 256; j++) {
                if (page[j])
                    delete page[j];
            }
            delete[] page;
        }
        // HYPOTHESIS: the original clears the slot inside the if (its branch skips the
        // store); clearing it unconditionally is equivalent and comes closer.
        field_2c[i] = NULL;
    }
    // HYPOTHESIS: the reset is inlined here, so the original spelled out what
    // mResetDataMembers does (or had an inline copy of it) rather than calling it.
    field_0 = 0;
    field_4.field_c = -1;
    field_4.field_10 = 0;
    field_4.field_12 = 0;
    field_4.field_14 = 0;
    field_4.field_18 = 0;
    field_4.field_1c = 0;
    field_24 = 0;
    field_28 = 0;
    for (i = 0; i < 256; i++)
        field_2c[i] = 0;
}

// FUNCTION: STARFLEETOP 0x00670710
tCharInfo* tFontTable::mMakeGlyph(unsigned short c, tFontFileIO* file)
{
    // Reopen the font file unless it is already open on this font.
    if (file->field_0 == 0 || !file->field_0->mIsOpen() ||
        strcmp(file->field_4, field_28) != 0) {
        if (!file->mOpenForRead(field_28))
            return 0;
    }

    if (!file->mSeekSet(field_24[c - field_4.field_10])) {
        file->mClose();
        return 0;
    }

    fn_00670710_record record;
    if (file->mRead(&record, sizeof(record)) != sizeof(record)) {
        file->mClose();
        return 0;
    }

    tCharInfo* info = (tCharInfo*)new unsigned char[sizeof(tCharInfo) + record.field_6];
    unsigned char* data = (unsigned char*)(info + 1);
    if (info == 0) {
        file->mClose();
        return 0;
    }
    if (file->mRead(data, record.field_6) != record.field_6) {
        file->mClose();
        delete[] (unsigned char*)info;
        return 0;
    }

    info->field_0 = record.field_0;
    info->field_4 = record.field_2;
    info->field_8 = field_4.field_14;
    info->field_c = field_4.field_18;
    info->field_10 = record.field_6;
    info->field_14 = data;
    info->field_18 = 0;
    return info;
}

// HYPOTHESIS: tTextFontFileIO reads a text font file through a 512-byte buffer. mGetC finds
// the tFontFileIO file pointer at +0x04, which fits a tFontFileIO base placed after the
// vtable this class adds for its virtual mOpenForRead (the exe's symbol says virtual). Field
// names are offsets.
class tTextFontFileIO : public tFontFileIO {
public:
    tTextFontFileIO()
    {
        field_30c = 0;
        field_310 = 0;
        field_314 = 0;
        field_318 = -1;
    }
    virtual unsigned char mOpenForRead(const char* name)
    {
        if (field_0 == 0)
            return 0;
        field_0->mClose();
        if (field_0 == 0 || !field_0->mOpenForRead(name))
            return 0;
        field_30c = 0;
        field_310 = 0;
        field_314 = 0;
        field_318 = -1;
        return 1;
    }
    unsigned char mEOF(void)
    {
        return field_30c;
    }
    // HYPOTHESIS: reads one line, without its line break, into the line buffer at +0x31c;
    // name not known.
    char* fn_mGetLine(void)
    {
        if (mEOF())
            return 0;
        char* p = field_31c;
        int n = sizeof(field_31c) - 1;
        while (n--) {
            int c = mGetC();
            if (c == -1)
                break;
            if (c == '\r') {
                c = mGetC();
                if (c != -1 && c != '\n')
                    field_318 = c;
                break;
            }
            if (c == '\n')
                break;
            *p++ = (char)c;
        }
        *p = 0;
        return field_31c;
    }
    int mGetC(void);
protected:
    char            field_10c[0x200];   // the read buffer
    unsigned char   field_30c;          // set at end of file
    long            field_310;          // read position in the buffer
    long            field_314;          // bytes in the buffer
    int             field_318;          // HYPOTHESIS: a pushed-back character, -1 when none
    char            field_31c[0x200];   // the line buffer
};

// FUNCTION: STARFLEETOP 0x006708c0
int tTextFontFileIO::mGetC(void)
{
    if (field_30c)
        return -1;
    int c = field_318;
    if (c != -1) {
        field_318 = -1;
        return c;
    }
    if (field_310 >= field_314) {
        field_310 = 0;
        field_314 = mRead(field_10c, 0x200);
        if (field_314 == 0) {
            field_30c = 1;
            return -1;
        }
    }
    return field_10c[field_310++];
}

// HYPOTHESIS: a page of 256 glyph pointers that clears itself when made; mGetCharInfo's
// allocation tests the new block before clearing it, as a constructor call does. Its name
// is not known.
struct fn_00670dd0_page {
    fn_00670dd0_page()
    {
        memset(field_0, 0, sizeof(field_0));
    }
    tCharInfo*      field_0[256];
};

// HYPOTHESIS: eTypeface's enumerators are not known here; the type is from the exe's symbol.
enum eTypeface { };

// HYPOTHESIS: tFontMgr layout recovered from mInit: the status at +0x00, the list of loaded
// font tables at +0x04 (linked through tFontTable +0x00), the glyph file at +0x10.
class tFontMgr {
public:
    enum eInitStatus { kNotInitialized, kInitialized, kInitFailed };
    eInitStatus mInit(void);
    const tCharInfo* mGetCharInfo(unsigned short c, unsigned long typeface);
    eTypeface mGetTypefacebyName(const tString& name);
    // HYPOTHESIS: an inline member that returns the height (header +0x14) of the table
    // holding the character in the typeface; inlined into _GetCharHeight, name not known.
    unsigned long fn_mGetCharHeight(unsigned short c, unsigned long typeface)
    {
        if (field_0 == kNotInitialized)
            field_0 = mInit();
        if (field_0 == kInitialized) {
            tFontTable* table = fn_mGetTable(c, typeface);
            if (table != 0)
                return table->field_4.field_14;
        }
        return 0;
    }
protected:
    void mDone(void);
    // HYPOTHESIS: an inline helper that finds the loaded table holding the character in
    // the typeface and caches it; its name is not known.
    tFontTable* fn_mFindTable(unsigned short c, unsigned long typeface)
    {
        for (tFontTable* table = field_4; table != 0; table = table->field_0) {
            if (table->field_4.field_c == (long)typeface &&
                c >= table->field_4.field_10 && c <= table->field_4.field_12) {
                field_8 = typeface;
                field_c = table;
                return table;
            }
        }
        return 0;
    }
    // HYPOTHESIS: an inline helper that returns the table that served the last character
    // when it still fits, and searches the list otherwise; its name is not known.
    tFontTable* fn_mGetTable(unsigned short c, unsigned long typeface)
    {
        if (field_8 == typeface &&
            c >= field_c->field_4.field_10 && c <= field_c->field_4.field_12)
            return field_c;
        return fn_mFindTable(c, typeface);
    }
    // HYPOTHESIS: an inline helper that allocates a cleared page of 256 glyph pointers;
    // its name is not known.
    static tCharInfo** fn_mNewPage(void)
    {
        tCharInfo** page = new tCharInfo*[256];
        if (page == 0)
            return 0;
        memset(page, 0, 256 * sizeof(tCharInfo*));
        return page;
    }

    eInitStatus     field_0;
    tFontTable*     field_4;
    unsigned long   field_8;    // HYPOTHESIS: the typeface of the cached table
    tFontTable*     field_c;    // HYPOTHESIS: the table that last served a character
    tFontFileIO*    field_10;
};

// FUNCTION: STARFLEETOP 0x00670950
tFontMgr::eInitStatus tFontMgr::mInit(void)
{
    if (field_0 != kNotInitialized)
        return field_0;

    tTextFontFileIO indexFile;
    if (!indexFile.mOpenForRead(tFontFileIOBase::mGetFontIndexFilename())) {
        field_0 = kInitFailed;
        return kInitFailed;
    }

    tFontFileIO fontFile;
    char* line;
    unsigned char ok = 1;
    while ((line = indexFile.fn_mGetLine()) != 0) {
        if (line[0] == 0 || line[0] == ';')
            continue;
        tFontTable* table = new tFontTable;
        if (table == 0) {
            ok = 0;
            break;
        }
        if (!table->mInit(line, fontFile)) {
            delete table;
            continue;
        }
        table->field_0 = field_4;
        field_4 = table;
    }

    if (ok) {
        field_10 = new tFontFileIO;
        if (field_10 == 0) {
            mDone();
            field_0 = kInitFailed;
            return kInitFailed;
        }
        field_0 = kInitialized;
        return kInitialized;
    }

    mDone();
    field_0 = kInitFailed;
    return kInitFailed;
}

// FUNCTION: STARFLEETOP 0x00670d50
void tFontMgr::mDone(void)
{
    if (field_4 != 0) {
        tFontTable* table = field_4;
        while (table != 0) {
            tFontTable* next = table->field_0;
            delete table;
            table = next;
        }
        field_4 = 0;
    }
    // HYPOTHESIS: the reset of the members is spelled out (or an inline helper whose name
    // is not known); +0x08 starts at -1 as tFontTable's typeface word does.
    field_0 = kNotInitialized;
    field_4 = 0;
    field_8 = -1;
    field_c = 0;
    if (field_10 != 0) {
        field_10->mClose();
        delete field_10;
        field_10 = 0;
    }
}

// FUNCTION: STARFLEETOP 0x00670dd0
const tCharInfo* tFontMgr::mGetCharInfo(unsigned short c, unsigned long typeface)
{
    if (field_0 == kNotInitialized)
        field_0 = mInit();
    if (field_0 != kInitialized)
        return 0;

    // Try the table that served the last character before searching the list.
    tFontTable* table = fn_mGetTable(c, typeface);
    if (table == 0)
        return 0;

    unsigned long page = c >> 8;
    unsigned long index = c & 0xff;
    tCharInfo** glyphs = table->field_2c[page];
    if (glyphs == 0) {
        glyphs = (new fn_00670dd0_page)->field_0;
        if (glyphs == 0)
            return 0;
        table->field_2c[page] = glyphs;
    }
    tCharInfo* info = glyphs[index];
    if (info == 0) {
        info = table->mMakeGlyph(c, field_10);
        glyphs[index] = info;
    }
    if (info != 0)
        info->field_18++;
    return info;
}

// FUNCTION: STARFLEETOP 0x00670ee0
eTypeface tFontMgr::mGetTypefacebyName(const tString& name)
{
    if (field_0 == kNotInitialized)
        field_0 = mInit();
    if (field_0 == kInitialized) {
        // HYPOTHESIS: the three built-in typefaces are numbered 1 to 3; their enumerator
        // names are not known.
        tString cyberbit("Bitstream Cyberbit");
        tString courier("Courier New");
        tString times("Times New Roman");
        if (name == cyberbit)
            return (eTypeface)1;
        if (name == courier)
            return (eTypeface)2;
        if (name == times)
            return (eTypeface)3;
        for (int i = 0; i < 56; i++) {
            if (name == gTypefaceTable[i].field_4)
                return (eTypeface)gTypefaceTable[i].field_0;
        }
    }
    return (eTypeface)0;
}

// The exe's font manager; the SDK library names it gFontMgr.
extern tFontMgr gFontMgr;

// FUNCTION: STARFLEETOP 0x00671110
const tCharInfo* _GetCharInfo(unsigned short c, unsigned long typeface)
{
    return gFontMgr.mGetCharInfo(c, typeface);
}

// FUNCTION: STARFLEETOP 0x00671130
unsigned long _GetCharHeight(unsigned long typeface, unsigned short c)
{
    return gFontMgr.fn_mGetCharHeight(c, typeface);
}
