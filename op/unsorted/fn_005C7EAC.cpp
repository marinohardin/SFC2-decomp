#include <sstream>
#include <string>
#include "unsorted/tCurrentTime.h"

// FUNCTION: STARFLEETOP 0x005c7eac
bool __fastcall fn_005c7eac(char* param_1)
{
    return *(long*)(param_1 + 0xc) > -1;
}

// HYPOTHESIS: tCurrentTime's own accessors at these addresses; the class header has no names for them.
long __fastcall fn_005c7fb7(tCurrentTime* param_1);
long __fastcall fn_005c7f5d(tCurrentTime* param_1);
long __fastcall fn_005e8f25(tCurrentTime* param_1);

// FUNCTION: STARFLEETOP 0x005c7eb6
// HYPOTHESIS: a tCurrentTime member returning a std::string (a date text); tCurrentTime.h
// (outside this lane) has no declaration for it, so a free function stands in.
std::string __fastcall fn_005c7eb6(tCurrentTime* param_1)
{
    std::stringstream stream;
    stream << fn_005c7fb7(param_1) + fn_005c7f5d(param_1) << "." << param_1->fn_0055f269() % fn_005e8f25(param_1);
    return stream.str();
}

// FUNCTION: STARFLEETOP 0x005c7f5d
long __fastcall fn_005c7f5d(tCurrentTime* param_1)
{
    long divisor = fn_005e8f25(param_1);
    if (divisor == 0)
    {
        divisor = 1;
    }
    return param_1->fn_0055f269() / divisor;
}

// HYPOTHESIS: really a tCurrentTime member taking one stack argument; tCurrentTime.h (outside
// this lane) has no declaration for it, so a free function stands in.
long __fastcall fn_005c7fbb(tCurrentTime* param_1, long param_2);

// FUNCTION: STARFLEETOP 0x005c7f7c
long __fastcall fn_005c7f7c(tCurrentTime* param_1)
{
    long i = 0;
    do
    {
        fn_005c7f5d(param_1);
        fn_005c7fbb(param_1, i);
        if (fn_005c7f5d(param_1) <= fn_005c7fbb(param_1, i))
        {
            return i;
        }
        i++;
    } while (i < 3);
    return i;
}

