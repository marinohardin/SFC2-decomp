#include "unsorted/tSFCTexMgr.h"

// HYPOTHESIS: the texture handle table built at 0x006a0490 (0x1000 slots, linked through
// g_00c7c118); its own unit has not landed, so it is declared here with the one member used.
class fn_006a0490_Mgr
{
public:
    void fn_006a08e0(int handle);
};

extern fn_006a0490_Mgr* g_00c7c118;

// FUNCTION: STARFLEETOP 0x0040ee47
tSFCTexMgrEx::~tSFCTexMgrEx()
{
    fn_006a0490_Mgr* mgr = g_00c7c118;
    if (mgr)
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (m_entries[i].m08)
            {
                mgr->fn_006a08e0(i + 1);
                m_entries[i].m08 = 0;
            }
        }
    }
}
