// HYPOTHESIS: the global's type is unknown
extern char g_008e0f38;

// FUNCTION: STARFLEETOP 0x004234cf
void* fn_004234cf()
{
    return &g_008e0f38;
}

#include "unsorted/tEntityMgr.h"

// FUNCTION: STARFLEETOP 0x00423517
tEntityMgr g_tEntityMgr;
