// Tiger/tinifile.cpp

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "Tiger/tbasicfile.h"
#include "Tiger/tinifile.h"

namespace tINISimpleNamespace {

// HYPOTHESIS: layouts from the offsets this unit uses; field names are ours
struct tINILine {
    // HYPOTHESIS: an inline constructor
    tINILine(char* text)
        : mNext(0), mText(text), mSize(strlen(text) + 1)
    {
    }

    tINILine* mNext;
    char* mText;
    unsigned long mSize; // HYPOTHESIS: capacity of mText, terminator included
};

// HYPOTHESIS: a block of the file's string pool; blocks are chained for freeing together
struct fn_PoolBlock {
    fn_PoolBlock* mNext;
    char mText[4];
};

} // namespace tINISimpleNamespace

namespace tINISimpleNamespace {

class tINISectionSimple;

class tINIFileSimple : public tINIFile {
    friend class tINIFile; // HYPOTHESIS: mNewINIFile walks fMasterList

public:
    // HYPOTHESIS: an inline helper that allocates pool text; the original's name is not known
    char* fn_Alloc(unsigned long size)
    {
        if (!size)
            return 0;
        fn_PoolBlock* block = (fn_PoolBlock*)new char[size + 7];
        if (!block)
            return 0;
        block->mNext = mPool;
        mPool = block;
        return block->mText;
    }

    // HYPOTHESIS: vtable order from the exe's tINIFileSimple functions; mFlush is slot 4
    // HYPOTHESIS: an inline constructor, seen only inlined in tINIFile::mNewINIFile
    tINIFileSimple(tBasicFile* (*createStream)())
        : tINIFile(createStream)
    {
        mNext = fMasterList;
        fMasterList = this;
        mRefCount = 1;
        mFileName = 0;
        mSections = 0;
        mLastSection = 0;
        mText = 0;
        mPool = 0;
        mDirty = 0;
    }

protected:
    // HYPOTHESIS: protected, from the ??_G mangling (MAE)
    virtual ~tINIFileSimple();

public:
    virtual int mAddRef();
    virtual int mRelease();
    virtual tINISection* mGetSection(const char* name, unsigned char create);
    virtual unsigned char mFlush();

    unsigned char mInit(const char* name);

protected:
    unsigned char mDone();

    static tINIFileSimple* fMasterList;

public:
    tINIFileSimple* mNext; // HYPOTHESIS: link in fMasterList; the name is ours
    int mRefCount; // HYPOTHESIS: the count mAddRef raises; the name is ours
    char* mFileName; // HYPOTHESIS: a pool copy of the name mInit was given; the name is ours
    tINISectionSimple* mSections; // HYPOTHESIS: the file's sections in order; the name is ours
    tINISectionSimple* mLastSection; // HYPOTHESIS: the name is ours
    char* mText; // HYPOTHESIS: the whole file's text, split into lines in place; the name is ours
    fn_PoolBlock* mPool;
    unsigned char mDirty;
};

class tINISectionSimple : public tINISection {
    friend class tINIFileSimple;

public:
    // HYPOTHESIS: an inline constructor, seen only inlined in mGetSection
    tINISectionSimple(tINIFileSimple* file)
        : mFile(file)
    {
        mNext = 0;
        mLines = 0;
        mLast = 0;
        mName = 0;
        mNameLen = 0;
    }

    // HYPOTHESIS: an inline, non-virtual destructor that frees the line records
    ~tINISectionSimple()
    {
        tINILine* line = mLines;
        while (line) {
            tINILine* next = line->mNext;
            delete line;
            line = next;
        }
    }

    // HYPOTHESIS: an inline helper that gives a new section its "[name]" header line; the
    // original's name for it is not known
    unsigned char fn_Init(const char* name)
    {
        unsigned long length = strlen(name);
        char* buffer = mFile->fn_Alloc(length + 3);
        if (!buffer)
            return 0;
        sprintf(buffer, "[%s]", name);
        mName = buffer + 1;
        mNameLen = length;
        tINILine* line = new tINILine(buffer);
        if (!line)
            return 0;
        if (mLast) {
            mLast->mNext = line;
            mLast = line;
        } else {
            mLines = mLast = line;
        }
        return 1;
    }

    // the order is the exe's tINISectionSimple vtable's
    virtual unsigned long mGetInt32u(const char* key, unsigned long def);
    virtual unsigned char mPutInt32u(const char* key, unsigned long value);
    virtual long mGetInt32s(const char* key, long def);
    virtual unsigned char mPutInt32s(const char* key, long value);
    virtual unsigned long mGetString(const char* key, const char* def, char* buffer, unsigned long size);
    virtual unsigned char mPutString(const char* key, const char* value);
    virtual unsigned char mFlush();

private:
    tINIFileSimple* mFile;
    tINISectionSimple* mNext; // HYPOTHESIS: link in the file's section list; the name is ours
    tINILine* mLines;
    tINILine* mLast;
    char* mName; // HYPOTHESIS: the name inside the header line's text; the name is ours
    unsigned long mNameLen; // HYPOTHESIS: the name is ours

    tINILine* mFindLine(const char* key, const char** value);
    unsigned long mGetInteger(const char* key, unsigned long def, unsigned char isSigned);
    unsigned char mPutInteger(const char* key, unsigned long value, unsigned char isSigned);
};

// HYPOTHESIS: an inline helper that returns the first non-blank character, or null on a blank
// line; the original's name for it is not known
static inline const char* fn_SkipSpace(const char* p)
{
    while (*p && isspace(*p))
        p++;
    if (*p)
        return p;
    return 0;
}

// HYPOTHESIS: an inline helper that steps over "= " after a key and returns the value text, or
// null when there is no '=' or no value; the original's name for it is not known
static inline const char* fn_SkipEquals(const char* p)
{
    p = fn_SkipSpace(p);
    if (!p)
        return 0;
    if (*p != '=')
        return 0;
    p++;
    if (!*p)
        return 0;
    return fn_SkipSpace(p);
}

// HYPOTHESIS: an inline helper that compares a counted name with a terminated one without regard
// to case and returns zero when they agree; the original's name for it is not known
static inline unsigned long fn_CompareText(const char* a, unsigned long count, const char* b)
{
    if (count != strlen(b))
        return 1;
    for (; count; count--) {
        int c = tolower((unsigned char)*a++);
        if (c != tolower((unsigned char)*b++))
            break;
    }
    return count;
}

// HYPOTHESIS: an inline helper that compares two terminated names without regard to case and
// returns zero when they agree; the original's name for it is not known
static inline int fn_CompareNoCase(const char* a, const char* b)
{
    int c, d;
    unsigned long i = 0;
    do {
        c = tolower((unsigned char)a[i]);
        d = tolower((unsigned char)b[i++]);
    } while (c && c == d);
    return c - d;
}

// HYPOTHESIS: an inline helper that recognises a "[name]" section header line, optionally
// followed by a comment, and returns the name inside it; the original's name is not known
static inline bool fn_ParseHeader(char* line, char** name, unsigned long* length)
{
    char* t = (char*)fn_SkipSpace(line);
    if (!t || *t != '[')
        return 0;
    t++;
    unsigned long count;
    for (count = 0; t[count] != ']'; count++) {
        if (!t[count])
            return 0;
    }
    if (count <= 0)
        return 0;
    const char* rest = fn_SkipSpace(t + count + 1);
    if (rest && *rest != ';')
        return 0;
    *name = t;
    *length = count;
    return 1;
}

// FUNCTION: STARFLEETOP 0x0062b550
tINILine* tINISectionSimple::mFindLine(const char* key, const char** value)
{
    for (tINILine* line = mLines; line; line = line->mNext) {
        const char* t = fn_SkipSpace(line->mText);
        if (t) {
            const char* k = key;
            while (*k) {
                int c = tolower((unsigned char)*k++);
                if (c != tolower((unsigned char)*t++))
                    goto next;
            }
            if (!isalnum(*t)) {
                if (value)
                    *value = t;
                return line;
            }
        }
    next:;
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0062b5f0
unsigned long tINISectionSimple::mGetInteger(const char* key, unsigned long def, unsigned char isSigned)
{
    if (!mFindLine(key, &key))
        return def;
    key = fn_SkipEquals(key);
    if (!key)
        return def;

    bool negative = *key == '-';
    // HYPOTHESIS: the original tests the second character for '+', not the first
    if (negative || key[1] == '+') {
        if (!isSigned)
            return def;
        key++;
        if (!*key)
            return def;
    }

    int digits[11];
    int count = 0;
    int c;
    while (count < 11) {
        c = key[count];
        if (!c || c == ';')
            break;
        if (isspace(c)) {
            // HYPOTHESIS: the original rereads the same character here, never advancing
            while ((c = key[count + 1]) != 0 && c != ';') {
                if (!isspace(c))
                    return def;
            }
            break;
        }
        if (!isdigit(c))
            return def;
        digits[count++] = c - '0';
    }
    if (count > 10)
        return def;

    unsigned long result = 0;
    unsigned long scale = 1;
    while (count--) {
        if (digits[count] * scale + result < result)
            return def;
        result += digits[count] * scale;
        scale *= 10;
    }

    if (isSigned) {
        if (negative) {
            result = -result;
            if ((long)result < 0)
                return result;
            return def;
        }
        if (result > 0x7fffffff)
            return def;
    }
    return result;
}

// FUNCTION: STARFLEETOP 0x0062b790
unsigned char tINISectionSimple::mPutInteger(const char* key, unsigned long value, unsigned char isSigned)
{
    char text[40];
    // the exe's format texts are the other way round from the flag: a signed value is written
    // with %lu and an unsigned one with %ld
    if (isSigned)
        sprintf(text, "=%lu", value);
    else
        sprintf(text, "=%ld", value);

    unsigned long size = strlen(key) + strlen(text) + 2;
    const char* rest;
    tINILine* line = mFindLine(key, &rest);
    if (!line) {
        if (strchr(key, '\r') || strchr(key, '\n'))
            return 0;
        char* buffer = mFile->fn_Alloc(size);
        if (!buffer)
            return 0;
        strcpy(buffer, key);
        strcat(buffer, text);
        line = new tINILine(buffer);
        if (!line)
            return 0;
        if (mLast) {
            mLast->mNext = line;
            mLast = line;
        } else {
            mLines = mLast = line;
        }
        mFile->mDirty = 1;
        return 1;
    }
    if (line->mSize >= size) {
        strcpy(line->mText, key);
        strcat(line->mText, text);
    } else {
        char* buffer = mFile->fn_Alloc(size);
        if (!buffer)
            return 0;
        strcpy(buffer, key);
        strcat(buffer, text);
        line->mText = buffer;
        line->mSize = size;
    }
    mFile->mDirty = 1;
    return 1;
}

// FUNCTION: STARFLEETOP 0x0062ba10
unsigned long tINISectionSimple::mGetInt32u(const char* key, unsigned long def)
{
    return mGetInteger(key, def, 0);
}

// FUNCTION: STARFLEETOP 0x0062ba30
long tINISectionSimple::mGetInt32s(const char* key, long def)
{
    return mGetInteger(key, def, 1);
}

// FUNCTION: STARFLEETOP 0x0062ba50
unsigned char tINISectionSimple::mPutInt32u(const char* key, unsigned long value)
{
    return mPutInteger(key, value, 0);
}

// FUNCTION: STARFLEETOP 0x0062ba70
unsigned char tINISectionSimple::mPutInt32s(const char* key, long value)
{
    return mPutInteger(key, value, 1);
}

// FUNCTION: STARFLEETOP 0x0062ba90
unsigned long tINISectionSimple::mGetString(const char* key, const char* def, char* buffer, unsigned long size)
{
    // HYPOTHESIS: the original reuses the def parameter as mFindLine's out value
    const char* value = def;
    if (mFindLine(key, &def)) {
        def = fn_SkipEquals(def);
        if (def)
            value = def;
    }

    unsigned long count;
    if (buffer) {
        if (value) {
            for (count = 0; count < size; count++) {
                char c = value[count];
                if (!c || c == ';')
                    break;
                buffer[count] = c;
            }
            buffer[count] = 0;
            return count;
        }
    } else if (value) {
        for (count = 0; value[count] && value[count] != ';'; count++)
            ;
        return count;
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0062bb80
unsigned char tINISectionSimple::mPutString(const char* key, const char* value)
{
    if (strchr(value, '\r') || strchr(value, '\n'))
        return 0;

    unsigned long size = strlen(value) + strlen(key) + 3;
    const char* rest;
    tINILine* line = mFindLine(key, &rest);
    if (!line) {
        if (strchr(key, '\r') || strchr(key, '\n'))
            return 0;
        char* buffer = mFile->fn_Alloc(size);
        if (!buffer)
            return 0;
        strcpy(buffer, key);
        strcat(buffer, "=");
        strcat(buffer, value);
        line = new tINILine(buffer);
        if (!line)
            return 0;
        if (mLast) {
            mLast->mNext = line;
            mLast = line;
        } else {
            mLines = mLast = line;
        }
        mFile->mDirty = 1;
        return 1;
    }
    if (line->mSize >= size) {
        strcpy(line->mText, key);
        strcat(line->mText, "=");
        strcat(line->mText, value);
    } else {
        char* buffer = mFile->fn_Alloc(size);
        if (!buffer)
            return 0;
        strcpy(buffer, key);
        strcat(buffer, "=");
        strcat(buffer, value);
        line->mText = buffer;
        line->mSize = size;
    }
    mFile->mDirty = 1;
    return 1;
}

// FUNCTION: STARFLEETOP 0x0062be80
unsigned char tINISectionSimple::mFlush()
{
    if (mFile)
        return mFile->mFlush();
    return 0;
}

// FUNCTION: STARFLEETOP 0x0062be90
// FUNCTION: STARFLEETOP 0x0062beb0
tINIFileSimple::~tINIFileSimple()
{
    mDone();
    tINIFileSimple* prev = 0;
    for (tINIFileSimple* file = fMasterList; file; file = file->mNext) {
        if (file == this) {
            if (prev)
                prev->mNext = mNext;
            else
                fMasterList = mNext;
            break;
        }
        prev = file;
    }
}

// FUNCTION: STARFLEETOP 0x0062bf30
int tINIFileSimple::mAddRef()
{
    return ++mRefCount;
}

// FUNCTION: STARFLEETOP 0x0062bf40
int tINIFileSimple::mRelease()
{
    if (mRefCount > 0)
    {
        if (--mRefCount != 0)
        {
            return mRefCount;
        }
        delete this;
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0062bf60
tINISection* tINIFileSimple::mGetSection(const char* name, unsigned char create)
{
    tINISectionSimple* section;
    for (section = mSections; section; section = section->mNext) {
        if (section->mName && !fn_CompareText(section->mName, section->mNameLen, name))
            return section;
    }
    if (!create)
        return 0;

    section = new tINISectionSimple(this);
    if (!section)
        return 0;
    if (!section->fn_Init(name)) {
        delete section;
        return 0;
    }
    section->mNext = 0;
    if (mLastSection) {
        mLastSection->mNext = section;
        mLastSection = section;
    } else {
        mSections = mLastSection = section;
    }
    return section;
}

// FUNCTION: STARFLEETOP 0x0062c130
unsigned char tINIFileSimple::mInit(const char* name)
{
    mDone();
    tBasicFile* stream = 0;
    try {
        mFileName = fn_Alloc(strlen(name) + 1);
        if (!mFileName)
            throw (unsigned char)0;
        strcpy(mFileName, name);

        stream = mCreateStream();
        if (!stream)
            throw (unsigned char)0;
        if (!stream->mOpenForRead(name))
            throw (unsigned char)0;
        unsigned long size = stream->mFileSize();
        if (!size)
            throw (unsigned char)1; // HYPOTHESIS: an empty file is a success
        mText = new char[size + 1];
        if (!mText)
            throw (unsigned char)0;
        if (stream->mRead(mText, size) != size)
            throw (unsigned char)0;
        stream->mClose();
        delete stream;
        stream = 0;

        tINISectionSimple* section = 0;
        mText[size] = 0;
        char* next = mText;
        char* end = next + size;
        while (next < end) {
            char* line = next;
            while (next < end) {
                char c = *next++;
                if (c == '\r') {
                    next[-1] = 0;
                    if (next < end && *next == '\n')
                        next++;
                    break;
                }
                if (c == '\n') {
                    next[-1] = 0;
                    break;
                }
            }

            char* headerName;
            unsigned long headerLen;
            bool isHeader = fn_ParseHeader(line, &headerName, &headerLen);
            if (!section || isHeader) {
                section = new tINISectionSimple(this);
                if (!section)
                    throw (unsigned char)0;
                if (mLastSection) {
                    mLastSection->mNext = section;
                    mLastSection = section;
                } else {
                    mSections = mLastSection = section;
                }
                if (isHeader) {
                    section->mName = headerName;
                    section->mNameLen = headerLen;
                }
            }

            tINILine* record = new tINILine(line);
            if (!record)
                throw (unsigned char)0;
            if (section->mLast) {
                section->mLast->mNext = record;
                section->mLast = record;
            } else {
                section->mLines = section->mLast = record;
            }
        }
    } catch (unsigned char result) {
        if (stream) {
            stream->mClose();
            delete stream;
        }
        if (!result) {
            mDone();
            return 0;
        }
    }
    return 1;
}

// FUNCTION: STARFLEETOP 0x0062c480
unsigned char tINIFileSimple::mDone()
{
    unsigned char result = mFlush();

    tINISectionSimple* section = mSections;
    while (section) {
        tINISectionSimple* next = section->mNext;
        delete section;
        section = next;
    }
    mSections = mLastSection = 0;

    if (mText) {
        delete[] mText;
        mText = 0;
    }

    fn_PoolBlock* block = mPool;
    while (block) {
        fn_PoolBlock* next = block->mNext;
        delete[] (char*)block;
        block = next;
    }
    mPool = 0;
    mFileName = 0;
    return result;
}

// FUNCTION: STARFLEETOP 0x0062c510
unsigned char tINIFileSimple::mFlush()
{
    static const char kCRLF[] = "\r\n";
    if (!mDirty)
        return 1;
    tBasicFile* stream = mCreateStream();
    try {
        if (!stream)
            throw (unsigned char)0;
        if (!stream->mOpenForWrite(mFileName))
            throw (unsigned char)0;
        for (tINISectionSimple* section = mSections; section; section = section->mNext) {
            for (tINILine* line = section->mLines; line; line = line->mNext) {
                unsigned long length = strlen(line->mText);
                if (stream->mWrite(line->mText, length) != length)
                    throw (unsigned char)0;
                if (stream->mWrite(kCRLF, 2) != 2)
                    throw (unsigned char)0;
            }
        }
        stream->mClose();
        delete stream;
    } catch (unsigned char) {
        if (stream) {
            stream->mClose();
            delete stream;
        }
        return 0;
    }
    mDirty = 0;
    return 1;
}

} // namespace tINISimpleNamespace

// FUNCTION: STARFLEETOP 0x0062c670
tINIFile* tINIFile::mNewINIFile(const char* name, tBasicFile* (*createStream)())
{
    using namespace tINISimpleNamespace;

    tINIFileSimple* file;
    for (file = tINIFileSimple::fMasterList; file; file = file->mNext) {
        if (file->mFileName && !fn_CompareNoCase(file->mFileName, name))
            break;
    }
    if (file) {
        file->mAddRef();
        return file;
    }

    file = new tINIFileSimple(createStream);
    if (!file)
        return 0;
    if (file->mInit(name))
        return file;
    file->mRelease();
    return 0;
}
