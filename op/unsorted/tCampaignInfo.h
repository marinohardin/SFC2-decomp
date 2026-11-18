//	tCampaignInfo.h: tCampaignInfo and the member types its assignment uses.
// Evidence:  class (RTTI; original file unknown)

#ifndef tCampaignInfo_h
#define tCampaignInfo_h 1

#include <memory>
#include <string>
#include "Common/Database/DatabaseObject.h"

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

// HYPOTHESIS: a 0x30-byte member of tCampaignInfo assigned through 0x0045be62.
class fn_0045be62_object
{
public:
    fn_0045be62_object &operator=(const fn_0045be62_object &other);

    char pad_000[0x30];
};

// HYPOTHESIS: a database class. The size (0x168) comes from the stack layout in 0x0046062e,
// the destructor is at 0x0042ceae, and the members are placed by the assignment at 0x00460736;
// their names are not known. vfunction6 to vfunction9 follow the base's virtuals.
class tCampaignInfo : public nDatabase::tDatabaseObject
{
public:
    ~tCampaignInfo();
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
    fn_0045be62_object m_108;
    long m_138;
    long m_13c;
    long m_140;
    long m_144;
    std::string m_148;
    std::string m_158;

    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
};

#endif
