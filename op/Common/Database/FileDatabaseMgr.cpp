// Common/Database/FileDatabaseMgr.cpp

#include <string>
#include <vector>

typedef unsigned char byte;
typedef unsigned long int32u;
typedef long int32;

// Declarations from the SDK headers (Object.h, DataStore/DataStoreBuffer.h,
// DataStore/DataStoreOutput.h, DataStore/Streamable.h, DateAndTime.h), reduced to what this
// unit uses until those headers land in game/.
class tObject
{
public:
    tObject();
    virtual ~tObject();

protected:
    virtual bool CheckIntegrity() const;

    mutable bool fCheckingIntegrity;
};

namespace nException
{
class tBaseException : public std::exception
{
public:
    tBaseException() throw();
    tBaseException(const char* pDescription, const char* pFile = NULL, int32 lineNumber = -1) throw();
    virtual ~tBaseException() throw();

    virtual void ReportHeader(const char* caughtDescription = NULL) throw();
    virtual void ReportFooter() throw();
    virtual void ReportException(const char* caughtDescription = NULL) throw();

    virtual const char* GetDescription() throw();
    virtual const char* GetFile() throw();
    virtual int32 GetLineNumber() throw();

protected:
    const char* fpFileName;
    int32 fLineNumber;
};

class tOutOfBounds : public tBaseException
{
public:
    tOutOfBounds(const char* object, int32 min, int32 max, int32 value, const char* file, int32 lineNumber);

    const char* fObject;
    int32 fMin;
    int32 fMax;
    int32 fValue;
};
} // namespace nException

namespace except = nException;

namespace nDataStore
{
typedef int32u iPos;

class tBuffer : public tObject
{
public:
    inline void Write(const byte* data, int32u size);

    inline byte Get(iPos getPos) const;
    inline void Read(byte* data, int32 size, iPos getPos) const;

    inline int32u Size() const;

protected:
    typedef std::vector<byte> iBuffer;

    iBuffer fBuffer;
};

void tBuffer::Write(const byte* data, int32u size)
{
    fBuffer.insert(fBuffer.end(), data, data + size);
}

byte tBuffer::Get(iPos getPos) const
{
    if (getPos >= Size())
    {
        throw except::tOutOfBounds("tBuffer", 0, Size(), getPos, __FILE__, __LINE__);
    }

    return fBuffer[getPos];
}

void tBuffer::Read(byte* data, int32 size, iPos getPos) const
{
    if (getPos + size > Size())
    {
        throw except::tOutOfBounds("tBuffer", 0, Size(), getPos + size, __FILE__, __LINE__);
    }

    iBuffer::const_iterator start = fBuffer.begin() + getPos;
    iBuffer::const_iterator end = start + size;

    std::copy(start, end, data);
}

int32u tBuffer::Size() const
{
    return fBuffer.size();
}

class tInput
{
public:
    typedef int32u iInputFlags;

    inline byte Get();
    inline void Read(byte* data, int32 size);

    inline tInput& operator>>(std::string& string);

protected:
    iInputFlags fInputFlags;
    bool fCleanUpBuffer;
    iPos fGetPos;
    const tBuffer* fBuffer;
};

byte tInput::Get()
{
    return fBuffer->Get(fGetPos++);
}

void tInput::Read(byte* data, int32 size)
{
    fBuffer->Read(data, size, fGetPos);

    fGetPos += size;
}

tInput& tInput::operator>>(std::string& string)
{
    string.erase();

    int32u size = 0;
    Read((byte*)&size, sizeof(size));

    for (int32u i = 0; i < size; i++)
    {
        string += Get();
    }

    return *this;
}

class tOutput
{
public:
    typedef int32u iOutputFlags;

    inline void Write(const byte* data, int32 size);

    inline tOutput& operator<<(const std::string& string);

protected:
    iOutputFlags fOutputFlags;
    bool fCleanUpBuffer;
    tBuffer* fBuffer;
};

void tOutput::Write(const byte* data, int32 size)
{
    fBuffer->Write(data, size);
}

tOutput& tOutput::operator<<(const std::string& string)
{
    int32u size = string.size();
    Write((const byte*)&size, sizeof(size));

    Write((const byte*)string.data(), size);

    return *this;
}

class tStreamable;
tInput& operator>>(tInput& input, tStreamable& object);
tOutput& operator<<(tOutput& output, const tStreamable& object);

class tStreamable
{
public:
    typedef unsigned long iObjectID;
    typedef unsigned char iObjectVersion;

    tStreamable(iObjectVersion versionNumber = 0);
    virtual ~tStreamable();

    virtual void StreamOut(nDataStore::tOutput& output) const = 0;
    virtual void StreamIn(nDataStore::tInput& input) = 0;

    mutable iObjectID fObjectID;
    iObjectVersion fObjectVersion;
};
} // namespace nDataStore

class tDateAndTime : public nDataStore::tStreamable
{
public:
    tDateAndTime();

    void StreamOut(nDataStore::tOutput& output) const;
    void StreamIn(nDataStore::tInput& input);

protected:
    long fTime;
};

// Declarations from the SDK header Exceptions/Validate.h.
namespace nException
{
class tValidationFailed : public tBaseException
{
public:
    tValidationFailed(const char* expression, const char* file, int32 lineNumber);

    virtual void ReportException(const char* caughtDescription = NULL) throw();

protected:
    const char* fExpression;
};
} // namespace nException

#define validate(expr)                                                     \
    if (!(expr))                                                           \
    {                                                                      \
        nException::tValidationFailed failed(#expr, __FILE__, __LINE__);   \
        failed.ReportException();                                          \
    }

namespace nDatabase
{

// HYPOTHESIS: minimal declarations; the constructors are those called at 0x007036b2 and
// 0x0070b30e.
class tDatabaseStorage
{
};

class tStreamDatabaseStorage : public tDatabaseStorage
{
public:
    tStreamDatabaseStorage(const std::string& name);

    // HYPOTHESIS: members unknown. The original allocates 0x40 bytes for this class (New and
    // Load); padding it to that size made objdiff time out on New, so the size is not
    // modelled yet.
};

class tDatabase : public tObject
{
public:
    tDatabase(tDatabaseStorage* storage);

    int32 fPad[0x18]; // HYPOTHESIS: members unknown; only the size (0x68) is known from New
};

class tFileDatabaseMgr
{
public:
    virtual ~tFileDatabaseMgr();

    class tDatabaseInfo : public nDataStore::tStreamable
    {
    public:
        tDatabaseInfo();

        void StreamOut(nDataStore::tOutput& output) const;
        void StreamIn(nDataStore::tInput& input);

        // HYPOTHESIS: the member at +0xc is a std::string (allocator byte and three zeroed
        // words); a std::vector would compile the same way. Names are not known.
        std::string fField0c;
        tDateAndTime fField1c;
    };

    // The virtual functions in vtable order (0x007bb500), with the signatures of the SDK
    // library's symbols. MSVC places overloads in reverse declaration order.
    virtual void SetPath(const std::string& path);
    virtual tDatabase* New(const tDatabaseInfo& info);
    virtual tDatabase* Load(const tDatabaseInfo& info);
    virtual tDatabase* Load(int32 index);
    virtual void Save(const std::string& name);
    virtual void Delete(const tDatabaseInfo& info);
    virtual void Delete(int32 index);
    virtual void FilterName(std::string* name);
    virtual void CloseCurDB();
    virtual std::string GetVersionInfo(const std::string& path);
    // HYPOTHESIS: slot 11 (0x007589b0) is a folded trivial function; its name and signature
    // are not known.
    virtual void fn_slot11() = 0;
    virtual tDatabaseInfo GetCurDBInfo();
    virtual const tDatabaseInfo* GetDBInfo(int32 index) const;
    virtual const std::vector<tDatabaseInfo>& GetListOfDatabases(bool flag);

protected:
    virtual void AddDBInfo(const std::string& name);
    virtual void DeleteDirectory(const std::string& path, bool flag);
    virtual void CopyDirectory(const std::string& from, const std::string& to);
    virtual bool ReadDatabaseInfo(const std::string& path, tDatabaseInfo* info);
    virtual void WriteDatabaseInfo(const tDatabaseInfo& info, const std::string& path);
    virtual void WriteVersionInfo(const std::string& path);

    // HYPOTHESIS: the layout below is read from the destructor (0x00708820); names and the
    // pointee type of fField50 are not known (it is deleted through a virtual destructor in
    // vtable slot 0).
    std::string fField04;
    std::string fField14;
    tDatabaseInfo fField24;
    tDatabase* fField50;
    std::vector<tDatabaseInfo> fField54;

    static const std::string kDatabaseInfoFileName;
    static const std::string kDatabaseVersionFileName;
    static const std::string kDatabaseSaveDirectory;
};

// tFileDatabaseMgr's scalar deleting destructor (0x00708800) is generated by the compiler from
// the virtual destructor, with no source line of its own; its annotation is stacked on that
// destructor's definition below.

// FUNCTION: STARFLEETOP 0x00707e80
tFileDatabaseMgr::tDatabaseInfo::tDatabaseInfo()
    : nDataStore::tStreamable(1)
{
}

// FUNCTION: STARFLEETOP 0x00707ef0
void tFileDatabaseMgr::tDatabaseInfo::StreamOut(nDataStore::tOutput& output) const
{
    output << fField0c;
    output << fField1c;
}

// FUNCTION: STARFLEETOP 0x00708140
void tFileDatabaseMgr::tDatabaseInfo::StreamIn(nDataStore::tInput& input)
{
    input >> fField0c;
    input >> fField1c;
}

// The compiler generates three functions from each of these string definitions, with no
// source line of their own: the dynamic initializer, the atexit registration and the atexit
// destructor. The destructors (0x00708420, 0x00708580, 0x007086e0) are reached only through
// atexit and are not yet in the function list, so only the first two are claimed.
// FUNCTION: STARFLEETOP 0x00708410
// FUNCTION: STARFLEETOP 0x00708330
const std::string tFileDatabaseMgr::kDatabaseInfoFileName = "db.info";

// The original's inlined basic_string::_Grow has no _FROZEN reference-count test and no
// decrement before _Tidy, unlike the toolchain's <xstring>; 0x00708330 differs the same way.
// FUNCTION: STARFLEETOP 0x00708570
// FUNCTION: STARFLEETOP 0x00708490
const std::string tFileDatabaseMgr::kDatabaseVersionFileName = "version.info";

// Same inlined basic_string::_Grow difference as 0x00708330 and 0x00708490.
// FUNCTION: STARFLEETOP 0x007086d0
// FUNCTION: STARFLEETOP 0x007085f0
const std::string tFileDatabaseMgr::kDatabaseSaveDirectory = "\\saves";

// The inlined basic_string::_Tidy lacks the toolchain's _FROZEN test (as at 0x00708330).
// FUNCTION: STARFLEETOP 0x00708800
// FUNCTION: STARFLEETOP 0x00708820
tFileDatabaseMgr::~tFileDatabaseMgr()
{
    delete fField50;
}

// FUNCTION: STARFLEETOP 0x00708fc0
tFileDatabaseMgr::tDatabaseInfo tFileDatabaseMgr::GetCurDBInfo()
{
    return fField24;
}

// FUNCTION: STARFLEETOP 0x00709060
const tFileDatabaseMgr::tDatabaseInfo* tFileDatabaseMgr::GetDBInfo(int32 index) const
{
    if (index > -1 && index < (int32)fField54.size())
    {
        return &fField54[index];
    }

    return NULL;
}

// FUNCTION: STARFLEETOP 0x007090b0
tDatabase* tFileDatabaseMgr::New(const tDatabaseInfo& info)
{
    delete fField50;

    DeleteDirectory(fField14.c_str(), false);

    fField24 = info;

    fField50 = new tDatabase(new tStreamDatabaseStorage(fField14.c_str()));

    return fField50;
}

// Remaining differences: the validate's __LINE__ (223 in the original file), the storage
// allocation size (see tStreamDatabaseStorage), and the inlined basic_string::_Tidy without
// the _FROZEN test (the library difference noted at 0x00708820).
// FUNCTION: STARFLEETOP 0x00709270
tDatabase* tFileDatabaseMgr::Load(const tDatabaseInfo& info)
{
    delete fField50;

    DeleteDirectory(fField14.c_str(), false);

    std::string saveDirectory = fField14 + kDatabaseSaveDirectory + "\\" + info.fField0c;
    CopyDirectory(saveDirectory, fField14);

    // HYPOTHESIS: this path is built but not used; the original computes it the same way.
    std::string databaseDirectory = saveDirectory + "\\" + info.fField0c;

    bool result = ReadDatabaseInfo(saveDirectory, &fField24);
    validate(result);

    fField50 = new tDatabase(new tStreamDatabaseStorage(fField14.c_str()));

    return fField50;
}

} // namespace nDatabase
