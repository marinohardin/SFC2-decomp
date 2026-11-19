#include "unsorted/tSFCTexMgr.h"

// FUNCTION: STARFLEETOP 0x0040ee47
// FUNCTION: STARFLEETOP 0x0040ee73
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
