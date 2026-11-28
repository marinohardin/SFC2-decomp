#ifndef TSFCTEXMGR_H
#define TSFCTEXMGR_H

// HYPOTHESIS: tSFCTexMgr and tSFCTexMgrEx are declared here only until their own units land.
#include "dmp3d/common/texBmp.h"

struct tSFCTexMgrEntry
{
    char m00;
    char pad01[3];
    int m04;
    tTexBmp* m08;
    char m0c;
    char pad0d[3];
    int m10;
};

class tSFCTexMgr
{
public:
    // MATCH-ONLY: the original inlined this constructor into tSFCTexMgrEx's.
    __forceinline tSFCTexMgr()
    {
        tSFCTexMgrEntry* p = m_entries;
        int n = 0x1000;
        do
        {
            p->m00 = 0;
            p->m04 = 0;
            p->m08 = 0;
            p->m0c = 0;
            p->m10 = 0;
            p++;
        } while (--n != 0);
    }
    virtual ~tSFCTexMgr();
    tSFCTexMgrEntry m_entries[0x1000];
};

// FUNCTION: STARFLEETOP 0x0040ee2a
// FUNCTION: STARFLEETOP 0x0040eed6
inline tSFCTexMgr::~tSFCTexMgr()
{
}

class tSFCTexMgrEx : public tSFCTexMgr
{
public:
    tSFCTexMgrEx();
    virtual ~tSFCTexMgrEx();
    // HYPOTHESIS: vfunction2 releases a texture handle; vfunction3 loads a texture by name.
    virtual int vfunction2(int handle);
    virtual int vfunction3(char *name, char param_2, char param_3, int param_4);
    virtual void vfunction4(int handle);
    virtual char vfunction5(int handle);
};

// HYPOTHESIS: the texture handle table built at 0x006a0490 (0x1000 slots); its own unit has not
// landed, so it is declared here with the members used, by their SDK symbols. The list pointer is
// a protected static member, so the managers that read it are declared friends here.
class tTexMgr
{
    friend class tSFCTexMgr;
    friend class tSFCTexMgrEx;
public:
    int mNewTexBmp(const char *name);
    void mDeleteTexBmp(int handle);
    tTexBmp* mIDToTexBmp(int handle);
protected:
    static tTexMgr* fList;
};

#endif
