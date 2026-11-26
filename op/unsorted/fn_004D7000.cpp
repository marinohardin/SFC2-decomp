// HYPOTHESIS: the object is a counted array of 16-byte records at +4; the two callees
// are cdecl range helpers (sort, then compact) whose real names are unknown.
struct fn_004d703f_Array
{
    unsigned char m_count;
    unsigned char m_pad[3];
    char m_items[1][16];

    void fn_004d703f();
    float fn_004d707c(unsigned char index);
    unsigned char fn_004d7099(float value);
    float fn_004d70ce(unsigned char index);
};

void fn_004d8999(void* first, void* last, int arg);
void* fn_004d8967(void* first, void* last);

// FUNCTION: STARFLEETOP 0x004d703f
void fn_004d703f_Array::fn_004d703f()
{
    fn_004d8999(m_items, m_items[m_count], 0);
    m_count = ((char*)fn_004d8967(m_items, m_items[m_count]) - (char*)m_items) >> 4;
}

// FUNCTION: STARFLEETOP 0x004d707c
float fn_004d703f_Array::fn_004d707c(unsigned char index)
{
    if (index < m_count)
        return *(float*)m_items[index];
    return 0.0f;
}

// FUNCTION: STARFLEETOP 0x004d7099
unsigned char fn_004d703f_Array::fn_004d7099(float value)
{
    unsigned char i = 0;
    if (m_count <= 0)
        return 0xff;
    while (value > *(float*)m_items[i]) {
        i++;
        if (i >= m_count)
            return 0xff;
    }
    return i;
}

// FUNCTION: STARFLEETOP 0x004d70ce
float fn_004d703f_Array::fn_004d70ce(unsigned char index)
{
    if (index < m_count)
        return *(float*)(m_items[index] + 4);
    return 0.0f;
}

#include "unsorted/tBaseWeapon.h"
#include "unsorted/tIndividualShipSystem.h"

// FUNCTION: STARFLEETOP 0x004d7292
void tBaseWeapon::vfunction52()
{
    ((tIndividualShipSystem *)this)->tIndividualShipSystem::vfunction52();
    if (vfunction106())
    {
        vfunction62();
    }
}

// FUNCTION: STARFLEETOP 0x004d72dc
void tBaseWeapon::vfunction54(int param_1)
{
    ((tRepairableShipSystem *)this)->tRepairableShipSystem::vfunction54(param_1);
    if (vfunction106())
    {
        vfunction62();
    }
}

// FUNCTION: STARFLEETOP 0x004d7304
int tBaseWeapon::vfunction108()
{
    return field_104 + 1;
}

// FUNCTION: STARFLEETOP 0x004d730c
float tBaseWeapon::vfunction109()
{
    return field_dc;
}

// FUNCTION: STARFLEETOP 0x004d7313
float tBaseWeapon::vfunction110()
{
    return field_e0;
}

// FUNCTION: STARFLEETOP 0x004d731a
float tBaseWeapon::vfunction111()
{
    return field_e4;
}

// FUNCTION: STARFLEETOP 0x004d7321
void tBaseWeapon::vfunction124(int param_1)
{
    field_e8 = param_1;
}

// FUNCTION: STARFLEETOP 0x004d732e
void tBaseWeapon::vfunction125(int param_1)
{
    field_10c = param_1;
}

// FUNCTION: STARFLEETOP 0x004d733b
int tBaseWeapon::vfunction129()
{
    return field_114;
}

// FUNCTION: STARFLEETOP 0x004d7342
bool tBaseWeapon::vfunction138()
{
    return vfunction106() > 0;
}
