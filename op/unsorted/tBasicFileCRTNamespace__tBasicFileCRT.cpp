// Unit:      unsorted/tBasicFileCRTNamespace__tBasicFileCRT.cpp
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)

#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>

void _DoneNewBasicFileCRT();

namespace tBasicFileCRTNamespace {

// HYPOTHESIS: the stream handle lives in a non-polymorphic base whose constructor clears
// it before the derived constructor stores the vftable pointer (the exe clears the handle
// first); the base name is ours.
class tBasicFileCRTHandle {
public:
    tBasicFileCRTHandle() { mFile = 0; }
    FILE* mFile; // 0x04 in the derived object
};

class tBasicFileCRT : public tBasicFileCRTHandle {
public:
    tBasicFileCRT();
    virtual unsigned char mClose();
    virtual unsigned char mIsOpen();
    virtual unsigned char mEOF();
    virtual unsigned long mRead(void* buf, unsigned long n);
    virtual unsigned long vfunction8(const void* buf, unsigned long n);
    virtual unsigned long mFilePos();
    virtual unsigned long mFileSize();
    virtual unsigned char mSeekSet(long pos);
    virtual unsigned char mSeekCur(long pos);
    virtual unsigned char mSeekEnd(long pos);

    // The exe folds this with _InitNewBasicFileCRT (same body, same address).
    static unsigned char mInit(unsigned char useModuleDir);

    // HYPOTHESIS: the shutdown entry point reaches the private flag as a friend; the exe
    // gives no class member for it.
    friend void ::_DoneNewBasicFileCRT();

private:
    // Names from the exe's symbols (private statics); the array length is from the
    // GetModuleFileNameA call that fills it.
    static unsigned char fInitialized;
    static char fStartDir[MAX_PATH];
};

unsigned char tBasicFileCRT::fInitialized;
char tBasicFileCRT::fStartDir[MAX_PATH];

// HYPOTHESIS: constructor shape is ours; it exists so the compiler emits the class vftable.
inline tBasicFileCRT::tBasicFileCRT()
{
}

class tBasicFile;

// FUNCTION: STARFLEETOP 0x0066d480
// HYPOTHESIS: initialises the class with the module directory, then allocates one stream
// object; the tBasicFile base is not declared in this unit, so the result is returned as is.
tBasicFile* _NewBasicFileCRT()
{
    tBasicFileCRT::mInit(1);
    return (tBasicFile*)new tBasicFileCRT;
}

// FUNCTION: STARFLEETOP 0x0066d1c0
unsigned char tBasicFileCRT::mClose()
{
    if (mFile) {
        fclose(mFile);
        mFile = 0;
    }
    return 1;
}

// FUNCTION: STARFLEETOP 0x0066d1e0
unsigned char tBasicFileCRT::mIsOpen()
{
    return mFile != 0;
}

// FUNCTION: STARFLEETOP 0x0066d1f0
unsigned char tBasicFileCRT::mEOF()
{
    if (mFile)
        return feof(mFile) != 0;
    return 1;
}

// FUNCTION: STARFLEETOP 0x0066d210
unsigned long tBasicFileCRT::mRead(void* buf, unsigned long n)
{
    if (!mFile)
        return 0;
    return fread(buf, 1, n, mFile);
}

// FUNCTION: STARFLEETOP 0x0066d240
// HYPOTHESIS: the write counterpart of mRead (fwrite on the stream); the exe and SDK give
// no name for this slot, so it keeps the analysis name.
unsigned long tBasicFileCRT::vfunction8(const void* buf, unsigned long n)
{
    if (!mFile)
        return 0;
    return fwrite(buf, 1, n, mFile);
}

// FUNCTION: STARFLEETOP 0x0066d270
unsigned long tBasicFileCRT::mFilePos()
{
    if (!mFile)
        return 0;
    return ftell(mFile);
}

// FUNCTION: STARFLEETOP 0x0066d290
unsigned long tBasicFileCRT::mFileSize()
{
    if (!mFile)
        return 0;
    long pos = ftell(mFile);
    fseek(mFile, 0, SEEK_END);
    unsigned long size = ftell(mFile);
    fseek(mFile, pos, SEEK_SET);
    return size;
}

// FUNCTION: STARFLEETOP 0x0066d2e0
unsigned char tBasicFileCRT::mSeekSet(long pos)
{
    if (!mFile)
        return 0;
    return fseek(mFile, pos, SEEK_SET) == 0;
}

// FUNCTION: STARFLEETOP 0x0066d310
unsigned char tBasicFileCRT::mSeekCur(long pos)
{
    if (!mFile)
        return 0;
    return fseek(mFile, pos, SEEK_CUR) == 0;
}

// FUNCTION: STARFLEETOP 0x0066d340
unsigned char tBasicFileCRT::mSeekEnd(long pos)
{
    if (!mFile)
        return 0;
    return fseek(mFile, pos, SEEK_END) == 0;
}

} // namespace tBasicFileCRTNamespace

using tBasicFileCRTNamespace::tBasicFileCRT;

// Turns backslashes into forward slashes in place and drops one trailing slash.
// HYPOTHESIS: an inline helper of the original source; it is expanded twice below.
static inline void fn_0066d370_fixSlashes(char* path)
{
    if (path && *path) {
        char* src = path;
        char* dst = path;
        for (;;) {
            char c = *src++;
            if (c == '\\')
                c = '/';
            *dst = c;
            if (!c)
                break;
            dst++;
        }
        if (*--dst == '/')
            *dst = 0;
    }
}

// HYPOTHESIS: `useModuleDir` is our name for the flag that picks the exe's directory
// over the working directory.
// HYPOTHESIS: inline, so _InitNewBasicFileCRT expands it; the exe holds one body for both
// names, which identical-code folding explains.
inline unsigned char tBasicFileCRT::mInit(unsigned char useModuleDir)
{
    if (!tBasicFileCRT::fInitialized) {
        if (useModuleDir) {
            GetModuleFileNameA(0, tBasicFileCRT::fStartDir, MAX_PATH);
            char* slash = strrchr(tBasicFileCRT::fStartDir, '\\');
            if (slash) {
                *slash = 0;
                fn_0066d370_fixSlashes(tBasicFileCRT::fStartDir);
            }
        }
        if (!tBasicFileCRT::fStartDir[0]) {
            getcwd(tBasicFileCRT::fStartDir, MAX_PATH - 1);
            fn_0066d370_fixSlashes(tBasicFileCRT::fStartDir);
            int len = strlen(tBasicFileCRT::fStartDir);
            if (len > 1 && tBasicFileCRT::fStartDir[len - 1] == '/')
                tBasicFileCRT::fStartDir[len - 1] = 0;
        }
        tBasicFileCRT::fInitialized = 1;
    }
    return 1;
}

// FUNCTION: STARFLEETOP 0x0066d370
unsigned char _InitNewBasicFileCRT(unsigned char param_1)
{
    return tBasicFileCRT::mInit(param_1);
}

// FUNCTION: STARFLEETOP 0x0066d460
void _DoneNewBasicFileCRT()
{
    if (tBasicFileCRT::fInitialized)
        tBasicFileCRT::fInitialized = 0;
}

