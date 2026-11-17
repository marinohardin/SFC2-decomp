//	FileDatabaseMgr.h: nDatabase::tFileDatabaseMgr, the manager of the databases kept as files.

#if !defined( FileDatabaseMgr_h )
#define FileDatabaseMgr_h

#pragma once

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/DateAndTime.h"

#include <string>
#include <vector>

namespace nDatabase
{

class tDatabase;

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

};

#endif
