// Unit:      unsorted/tShuttleBaySystem.cpp
// Functions of tShuttleBaySystem (0x0044e554 to 0x0044f16f); original file unknown.
#include "unsorted/tShuttleBaySystem.h"

// FUNCTION: STARFLEETOP 0x0044e554
int* tShuttleBaySystem::vfunction102()
{
    return &field_a0;
}

// FUNCTION: STARFLEETOP 0x0044e55b
tValue<long>* tShuttleBaySystem::vfunction103()
{
    return &field_c4;
}

// FUNCTION: STARFLEETOP 0x0044e562
int tShuttleBaySystem::vfunction104()
{
    return field_b8;
}

// FUNCTION: STARFLEETOP 0x0044e569
int tShuttleBaySystem::vfunction105()
{
    return field_bc;
}

// FUNCTION: STARFLEETOP 0x0044e570
int tShuttleBaySystem::vfunction106()
{
    return field_c0;
}

// FUNCTION: STARFLEETOP 0x0044e577
int tShuttleBaySystem::vfunction107()
{
    return field_c4;
}

// FUNCTION: STARFLEETOP 0x0044e57e
int tShuttleBaySystem::vfunction108()
{
    return field_e8;
}

// FUNCTION: STARFLEETOP 0x0044e585
int tShuttleBaySystem::vfunction109()
{
    return field_f4;
}

// FUNCTION: STARFLEETOP 0x0044e58c
int tShuttleBaySystem::vfunction110(int param_1)
{
    return field_2c8[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e59a
void tShuttleBaySystem::vfunction111(int param_1, int param_2)
{
    field_2c8[param_1] = param_2;
}

// FUNCTION: STARFLEETOP 0x0044e5ac
int tShuttleBaySystem::vfunction116(int param_1)
{
    return field_138[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e5ba
int tShuttleBaySystem::vfunction117(int param_1)
{
    return field_248[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e5c8
void tShuttleBaySystem::vfunction126(int param_1)
{
    field_104 = param_1;
}

// FUNCTION: STARFLEETOP 0x0044e5d5
int tShuttleBaySystem::vfunction127()
{
    return field_104;
}

// FUNCTION: STARFLEETOP 0x0044e5dc
void tShuttleBaySystem::vfunction128(long param_1)
{
    field_c4 = param_1;
}

// FUNCTION: STARFLEETOP 0x0044e5f5
void tShuttleBaySystem::vfunction129(int param_1)
{
    field_e8 = param_1;
}

// FUNCTION: STARFLEETOP 0x0044e602
void tShuttleBaySystem::vfunction130(int param_1)
{
    field_f4 = param_1;
}
