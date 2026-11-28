#include <stdio.h>

#include "unsorted/tSFCTexMgr.h"
#include "unsorted/tApp3dBase.h"
#include "unsorted/tBmpSurf.h"

// HYPOTHESIS: 0x0040f9ea compares two names and returns 0 when they are equal.
int fn_0040f9ea(char *a, char *b);

// FUNCTION: STARFLEETOP 0x0040eedd
int tSFCTexMgrEx::vfunction3(char *name, char param_2, char param_3, int param_4)
{
    int i = 0;
    tSFCTexMgrEntry* entry = m_entries;
    for (; i < 0x1000; i++, entry++)
    {
        if (entry->m08 && fn_0040f9ea(entry->m08->field_08, name) == 0)
        {
            entry->m04++;
            i++;
            if (param_2 && entry->m00)
                vfunction4(i);
            return i;
        }
    }

    tTexMgr* mgr = tTexMgr::fList;
    if (!mgr)
        return 0;
    i = mgr->mNewTexBmp(name);
    if (!i)
        return 0;

    entry = &m_entries[i - 1];
    entry->m00 = 1;
    entry->m04 = 1;
    entry->m08 = mgr->mIDToTexBmp(i);
    entry->m0c = param_3;
    entry->m10 = param_4;
    if (param_2)
        vfunction4(i);
    if (entry->m08->field_10c > 8)
    {
        FILE *file = fopen("Above8BitTextures.txt", "a+");
        fprintf(file, "%s\n", name);
        fflush(file);
        fclose(file);
    }
    return i;
}

// FUNCTION: STARFLEETOP 0x0040efcb
int tSFCTexMgrEx::vfunction2(int handle)
{
    int i = handle - 1;
    if (i >= 0 && i < 0x1000)
    {
        tSFCTexMgrEntry* entry = &m_entries[i];
        if (entry->m04 > 0)
        {
            entry->m04--;
            if (entry->m04 == 0)
            {
                entry->m00 = 0;
                entry->m08 = 0;
                if (tTexMgr::fList)
                    tTexMgr::fList->mDeleteTexBmp(handle);
            }
            return entry->m04;
        }
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x0040f015
void tSFCTexMgrEx::vfunction4(int handle)
{
    int i = handle - 1;
    if (i >= 0 && i < 0x1000)
    {
        tSFCTexMgrEntry* entry = &m_entries[i];
        if (entry->m00)
        {
            tTexBmp* bmp = entry->m08;
            if (bmp && g_pApp3dBase)
            {
                tView3D* view = g_pApp3dBase->m_field10;
                if (view && tTexMgr::fList && bmp->mLock())
                {
                    tBmpSurf surf(*bmp);
                    view->vfunction85(handle, &surf, entry->m0c, entry->m10);
                    bmp->mUnlock();
                    entry->m00 = 0;
                }
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x0040f0b9
char tSFCTexMgrEx::vfunction5(int handle)
{
    int i = handle - 1;
    if (i < 0 || i >= 0x1000)
        return 0;
    return m_entries[i].m00;
}
