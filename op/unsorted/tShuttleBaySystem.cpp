// Unit:      unsorted/tShuttleBaySystem.cpp
// Functions of tShuttleBaySystem (0x0044e554 to 0x0044f16f); original file unknown.
#include "unsorted/tShuttleBaySystem.h"
#include "unsorted/tEntity.h"
#include "unsorted/fn_004e2b02_ship_type.h"

// FUNCTION: STARFLEETOP 0x0044e554
tValue<long>* tShuttleBaySystem::vfunction102()
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
    return field_a0;
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

// FUNCTION: STARFLEETOP 0x0044e60f
void tShuttleBaySystem::vfunction131(int param_1, int param_2)
{
    field_108[param_1] = param_2;
}

// FUNCTION: STARFLEETOP 0x0044e621
void tShuttleBaySystem::vfunction132(int param_1, int param_2)
{
    field_118[param_1] = param_2;
}

// FUNCTION: STARFLEETOP 0x0044e633
void tShuttleBaySystem::vfunction133(int param_1, int param_2)
{
    field_128[param_1] = param_2;
}

// FUNCTION: STARFLEETOP 0x0044e645
int tShuttleBaySystem::vfunction134(int param_1)
{
    return field_108[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e653
int tShuttleBaySystem::vfunction135(int param_1)
{
    return field_118[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e661
int tShuttleBaySystem::vfunction136(int param_1)
{
    return field_128[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e66f
// FUNCTION: STARFLEETOP 0x0044e844
tShuttleBaySystem::~tShuttleBaySystem()
{
    int i;
    for (i = 0; i < 32; i++)
    {
        fn_004e2b02_ship_type* ship = gObject_008e1060.fn_0042496f(field_1c8[i]);
        if (ship)
            ship->vfunction69(0);
    }
    for (i = 0; i < 4; i++)
    {
        fn_004e2b02_ship_type* ship = gObject_008e1060.fn_0042496f(field_3a8[i]);
        if (ship)
            ship->vfunction69(0);
    }
    std::list<tObject_0044e844_item*>::iterator it = field_4f0.begin();
    while (it != field_4f0.end())
    {
        std::list<tObject_0044e844_item*>::iterator cur = it++;
        delete *cur;
        field_4f0.erase(cur);
    }
}

// FUNCTION: STARFLEETOP 0x0044e68b
std::string tShuttleBaySystem::vfunction112(short param_1)
{
    return field_2d8[param_1];
}

// FUNCTION: STARFLEETOP 0x0044e6cb
void tShuttleBaySystem::vfunction113(short param_1, std::string param_2)
{
    field_2d8[param_1] = param_2;
    vfunction100();
}

// FUNCTION: STARFLEETOP 0x0044e9d8
void tShuttleBaySystem::vfunction123(long param_1)
{
    field_a0 = param_1;
    for (int i = 0; i < 32; i++)
    {
        field_1c8[i] = -1;
        field_248[i] = 0;
        field_148[i] = i < field_a0;
    }
}
