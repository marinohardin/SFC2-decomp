//	tCampaignInfo.h: tCampaignInfo and the member types its assignment uses.
// Evidence:  class (RTTI; original file unknown)

#ifndef tCampaignInfo_h
#define tCampaignInfo_h 1

#include <iosfwd>
#include <memory>
#include <string>
#include "Common/Database/DatabaseObject.h"
#include "unsorted/tCurrentTime.h"

// HYPOTHESIS: 16-byte members of tCampaignInfo assigned through their own out-of-line
// assignments (0x0046090a, 0x00604f1d); their types are not identified.
// HYPOTHESIS: 0x0046090a is the library's vector assignment for 16-byte strings (allocator,
// first, last, end); the members and helpers below follow that layout.
class fn_0046090a_object
{
public:
    fn_0046090a_object &operator=(const fn_0046090a_object &other);
    const std::string *begin() const;
    const std::string *end() const;
    unsigned int size() const;
    unsigned int capacity() const;
    void _Destroy(std::string *first, std::string *last);
    std::string *_Ucopy(const std::string *first, const std::string *last, std::string *dest);

    std::allocator<std::string> allocator;
    std::string *_First;
    std::string *_Last;
    std::string *_End;
};

class fn_00604f1d_object
{
public:
    fn_00604f1d_object &operator=(const fn_00604f1d_object &other);

    char pad_000[0x10];
};

// HYPOTHESIS: a database class. The size (0x168) comes from the stack layout in 0x0046062e,
// the destructor is at 0x0042ceae, and the members are placed by the assignment at 0x00460736;
// their names are not known. 0x0056c414 reads the starting date index at +0xb8 and the clock at
// +0x108, a tCurrentTime (its assignment is 0x0045be62); 0x0056e124 streams the object from +8.
class tCampaignInfo : public nDatabase::tDatabaseObject
{
public:
    tCampaignInfo();
    // HYPOTHESIS: constructed from a stream at 0x005486ff; 0x0042cd23 builds one from the
    // campaign file.
    tCampaignInfo(std::istream &in);
    // HYPOTHESIS: 0x00461528 copies a campaign info through the base's copy constructor
    // (0x006fa6d0) and then each text member.
    tCampaignInfo(const tCampaignInfo &other);
    ~tCampaignInfo();
    // Overrides, as the SDK's tDatabaseObject declares them; the executable's vtables hold
    // tCampaignInfo's own code at: tStreamable's StreamOut 0x00548cf8 and StreamIn 0x00548e75
    // (slot 0 of that vtable, 0x00461927, is the destructor's thunk), and the allocated-class
    // vtable's SQLFillInsertStream 0x005492dc, SQLFillUpdateStream 0x005497c3, SQLTableName
    // 0x00549d84 and SQLFieldNames 0x00549d8a.
    virtual void StreamOut(nDataStore::tOutput &output) const;
    virtual void StreamIn(nDataStore::tInput &input);
    virtual void SQLFillInsertStream(std::stringstream *SQL, bool lastOne = false);
    virtual void SQLFillUpdateStream(std::stringstream *SQL, bool lastOne = false);
    virtual const char *SQLTableName() const;
    virtual const char *SQLFieldNames() const;
    tCampaignInfo &operator=(const tCampaignInfo &other);
    // HYPOTHESIS: 0x0054a8d4 lies in tCampaignInfo's own code and returns a text member.
    const std::string &fn_0054a8d4();

    std::string m_48;
    std::string m_58;
    std::string m_68;
    std::string m_78;
    std::string m_88;
    fn_0046090a_object m_98;
    fn_00604f1d_object m_a8;
    long m_b8;
    long m_bc;
    long m_c0;
    std::string m_c4;
    std::string m_d4;
    std::string m_e4;
    long m_f4;
    std::string m_f8;
    tCurrentTime m_108;
    long m_138;
    long m_13c;
    long m_140;
    long m_144;
    std::string m_148;
    std::string m_158;
};

// The size comes from the stack layout in 0x0046062e (see above); the members end with the
// string at 0x158.
typedef char tCampaignInfo_size_is_0x168[sizeof(tCampaignInfo) == 0x168 ? 1 : -1];

#endif
