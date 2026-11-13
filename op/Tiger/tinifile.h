//	tinifile.h: tINIFile and tINISection, the interface to an INI file and its sections. Only the
//	members a unit here uses are declared.

#ifndef tinifile_h
#define tinifile_h

class tBasicFile;
class tINISection;

// The factory a tINIFile reads its file through (the exe's mangled name for mNewINIFile gives
// its type); the typedef's name is ours.
typedef tBasicFile* (*fn_BasicFileFactory)();

// HYPOTHESIS: the abstract base; its inline destructor is seen only as the vtable store at the
// end of the derived destructor
class tINIFile {
public:
    // HYPOTHESIS: an inline constructor, seen only inlined in mNewINIFile; with no factory the
    // file is read through mNewBasicFile
    tINIFile(tBasicFile* (*createStream)())
    {
        if (createStream)
            mCreateStream = createStream;
        else
            mCreateStream = mNewBasicFile;
    }

    static tINIFile* mNewINIFile(const char* name, tBasicFile* (*createStream)());
    static tBasicFile* mNewBasicFile();

protected:
    virtual ~tINIFile() {}

public:
    // HYPOTHESIS: the slots the exe's tINIFile vtable leaves pure
    virtual int mAddRef() = 0;
    virtual int mRelease() = 0;
    virtual tINISection* mGetSection(const char* name, unsigned char create) = 0;
    virtual unsigned char mFlush() = 0;

    fn_BasicFileFactory mCreateStream; // HYPOTHESIS: a factory for the stream the file is read through; the name is ours
};

// HYPOTHESIS: the abstract section interface; the exe has its type name but no vtable or
// destructor of its own, so it declares only pure virtual members. The order is the exe's
// tINISectionSimple vtable's.
class tINISection {
public:
    virtual unsigned long mGetInt32u(const char* key, unsigned long def) = 0;
    virtual unsigned char mPutInt32u(const char* key, unsigned long value) = 0;
    virtual long mGetInt32s(const char* key, long def) = 0;
    virtual unsigned char mPutInt32s(const char* key, long value) = 0;
    virtual unsigned long mGetString(const char* key, const char* def, char* buffer, unsigned long size) = 0;
    virtual unsigned char mPutString(const char* key, const char* value) = 0;
    virtual unsigned char mFlush() = 0;
};

#endif	// tinifile_h
