// HYPOTHESIS: the first entry of the table tPhaserXBank.cpp hands to fn_004d6f22.
extern const float g_00856fa0[];

// HYPOTHESIS: the original ends in ret 4, so this is a member taking one unused
// argument, likely a float (unsigned char) virtual of tPhaserXBank, whose class is
// declared only in tPhaserXBank.cpp; written free until it can move there.
// FUNCTION: STARFLEETOP 0x004e5e83
float fn_004e5e83()
{
    return g_00856fa0[0];
}
