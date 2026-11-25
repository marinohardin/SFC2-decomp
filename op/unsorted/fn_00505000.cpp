// FUNCTION: STARFLEETOP 0x005050e2
float fn_005050e2(float param_1)
{
    return param_1 * 0.00390625f;
}

#include "unsorted/fn_008cc620_object.h"










extern char g_00967698;
extern long g_0085829c;
extern long g_008582a0;

// FUNCTION: STARFLEETOP 0x005050ed
void fn_005050ed()
{
    if (!g_00967698)
    {
        g_0085829c = g_008cc620->vfunction2("assets/textures/pulsar_1.bmp", 1, 0, 0);
        g_008582a0 = g_008cc620->vfunction2("assets/textures/pulsar_2.bmp", 1, 0, 0);
    }
    g_00967698 = 1;
}
