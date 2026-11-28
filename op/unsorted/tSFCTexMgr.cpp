#include "unsorted/tSFCTexMgr.h"

// FUNCTION: STARFLEETOP 0x0040ee47
// FUNCTION: STARFLEETOP 0x0040ee73
tSFCTexMgrEx::~tSFCTexMgrEx()
{
    tTexMgr* mgr = tTexMgr::fList;
    if (mgr)
    {
        for (int i = 0; i < 0x1000; i++)
        {
            if (m_entries[i].m08)
            {
                mgr->mDeleteTexBmp(i + 1);
                m_entries[i].m08 = 0;
            }
        }
    }
}
