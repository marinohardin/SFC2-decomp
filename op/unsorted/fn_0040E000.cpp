// HYPOTHESIS: tSFCTexMgr and tSFCTexMgrEx are declared here only until their own units land.
struct tSFCTexMgrEntry
{
    char m00;
    char pad01[3];
    int m04;
    int m08;
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

class tSFCTexMgrEx : public tSFCTexMgr
{
public:
    tSFCTexMgrEx();
    virtual ~tSFCTexMgrEx();
};

// FUNCTION: STARFLEETOP 0x0040ede5
// FUNCTION: STARFLEETOP 0x0040eddb
tSFCTexMgrEx g_tSFCTexMgrEx;

// FUNCTION: STARFLEETOP 0x0040edfb
tSFCTexMgrEx::tSFCTexMgrEx()
{
}
