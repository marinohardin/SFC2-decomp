// Unit:      unsorted/t3DListenerProfile.cpp
// Functions: 0x004355c7 to 0x004356ac, 16 in game/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include "unsorted/t3DListenerProfile.h"

// FUNCTION: STARFLEETOP 0x004355c7
t3DListenerProfile::~t3DListenerProfile()
{
}

// FUNCTION: STARFLEETOP 0x004355ce
void t3DListenerProfile::fn_004355ce(const t3DListenerProfile_field_08 *param_1)
{
    field_08.field_00 = param_1->field_00;
    field_08.field_04 = param_1->field_04;
    field_08.field_08 = param_1->field_08;
}

// FUNCTION: STARFLEETOP 0x004355e6
// HYPOTHESIS: sets field_08 from three separate components (likely x, y, z); the copies go
// through eax, so the components are 32-bit values, kept as int to match field_08.
void t3DListenerProfile::fn_004355e6(int param_1, int param_2, int param_3)
{
    field_08.field_00 = param_1;
    field_08.field_04 = param_2;
    field_08.field_08 = param_3;
}

// FUNCTION: STARFLEETOP 0x004355fe
t3DListenerProfile_field_08 *t3DListenerProfile::fn_004355fe()
{
    return &field_08;
}

// FUNCTION: STARFLEETOP 0x00435602
void t3DListenerProfile::fn_00435602(const t3DListenerProfile_field_08 *param_1)
{
    field_14.field_00 = param_1->field_00;
    field_14.field_04 = param_1->field_04;
    field_14.field_08 = param_1->field_08;
}

// FUNCTION: STARFLEETOP 0x0043561a
// HYPOTHESIS: sets field_14 from three separate components, as 0x004355e6 does for field_08.
void t3DListenerProfile::fn_0043561a(int param_1, int param_2, int param_3)
{
    field_14.field_00 = param_1;
    field_14.field_04 = param_2;
    field_14.field_08 = param_3;
}

// FUNCTION: STARFLEETOP 0x00435632
t3DListenerProfile_field_08 *t3DListenerProfile::fn_00435632()
{
    return &field_14;
}

// FUNCTION: STARFLEETOP 0x00435636
void t3DListenerProfile::fn_00435636(const t3DListenerProfile_field_08 *param_1)
{
    field_20.field_00 = param_1->field_00;
    field_20.field_04 = param_1->field_04;
    field_20.field_08 = param_1->field_08;
}

// FUNCTION: STARFLEETOP 0x0043564e
// HYPOTHESIS: sets field_20 from three separate components, as 0x004355e6 does for field_08.
void t3DListenerProfile::fn_0043564e(int param_1, int param_2, int param_3)
{
    field_20.field_00 = param_1;
    field_20.field_04 = param_2;
    field_20.field_08 = param_3;
}

// FUNCTION: STARFLEETOP 0x00435666
t3DListenerProfile_field_08 *t3DListenerProfile::fn_00435666()
{
    return &field_20;
}

// FUNCTION: STARFLEETOP 0x0043566a
void t3DListenerProfile::fn_0043566a(const t3DListenerProfile_field_08 *param_1)
{
    field_2c.field_00 = param_1->field_00;
    field_2c.field_04 = param_1->field_04;
    field_2c.field_08 = param_1->field_08;
}

// FUNCTION: STARFLEETOP 0x00435682
// HYPOTHESIS: sets field_2c from three separate components, as 0x004355e6 does for field_08.
void t3DListenerProfile::fn_00435682(int param_1, int param_2, int param_3)
{
    field_2c.field_00 = param_1;
    field_2c.field_04 = param_2;
    field_2c.field_08 = param_3;
}

// FUNCTION: STARFLEETOP 0x0043569a
t3DListenerProfile_field_08 *t3DListenerProfile::fn_0043569a()
{
    return &field_2c;
}

// FUNCTION: STARFLEETOP 0x0043569e
void t3DListenerProfile::fn_0043569e(double param_1)
{
    // MATCH-ONLY: cast stands in for a double member the original layout held unpadded.
    *(double *)field_38 = param_1;
}

// FUNCTION: STARFLEETOP 0x004356a8
// HYPOTHESIS: returns the double at +0x38 that 0x0043569e sets.
double t3DListenerProfile::fn_004356a8()
{
    // MATCH-ONLY: cast stands in for a double member the original layout held unpadded.
    return *(double *)field_38;
}
