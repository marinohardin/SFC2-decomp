#include <stdio.h>

#include "unsorted/tSFCTexMgr.h"

// HYPOTHESIS: 0x0040f9ea compares two names and returns 0 when they are equal.
int fn_0040f9ea(char *a, char *b);

// FUNCTION: STARFLEETOP 0x0040eedd
int tSFCTexMgrEx::vfunction3(char *name, char param_2, char param_3, int param_4)
{
    int i = 0;
    tSFCTexMgrEntry* entry = m_entries;
    for (; i < 0x1000; i++, entry++)
    {
        if (entry->m08 && fn_0040f9ea(entry->m08->m08, name) == 0)
        {
            entry->m04++;
            i++;
            if (param_2 && entry->m00)
                vfunction4(i);
            return i;
        }
    }

    fn_006a0490_Mgr* mgr = g_00c7c118;
    if (!mgr)
        return 0;
    i = mgr->fn_006a07d0(name);
    if (!i)
        return 0;

    entry = &m_entries[i - 1];
    entry->m00 = 1;
    entry->m04 = 1;
    entry->m08 = mgr->fn_006a0920(i);
    entry->m0c = param_3;
    entry->m10 = param_4;
    if (param_2)
        vfunction4(i);
    if (entry->m08->m10c > 8)
    {
        FILE *file = fopen("Above8BitTextures.txt", "a+");
        fprintf(file, "%s\n", name);
        fflush(file);
        fclose(file);
    }
    return i;
}
