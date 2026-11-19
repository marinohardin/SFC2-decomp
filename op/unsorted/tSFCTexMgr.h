#ifndef TSFCTEXMGR_H
#define TSFCTEXMGR_H

// HYPOTHESIS: tSFCTexMgr and tSFCTexMgrEx are declared here only until their own units land.
// HYPOTHESIS: the texture object returned by 0x006a0920; +0x8 holds its name and +0x10c its
// bit depth.
struct fn_006a0920_Tex
{
    char pad00[8];
    char m08[0x104];
    int m10c;
};

struct tSFCTexMgrEntry
{
    char m00;
    char pad01[3];
    int m04;
    fn_006a0920_Tex* m08;
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
};

// HYPOTHESIS: the texture handle table built at 0x006a0490 (0x1000 slots, linked through
// g_00c7c118); its own unit has not landed, so it is declared here with the members used.
class fn_006a0490_Mgr
{
public:
    int fn_006a07d0(char *name);
    void fn_006a08e0(int handle);
    fn_006a0920_Tex* fn_006a0920(int handle);
};

extern fn_006a0490_Mgr* g_00c7c118;

#endif
