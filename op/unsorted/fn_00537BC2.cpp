// HYPOTHESIS: a constructor that zeroes six dwords; the owning class is unknown
// FUNCTION: STARFLEETOP 0x00537bc2
int* __fastcall fn_00537bc2(int* self)
{
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
    self[4] = 0;
    self[5] = 0;
    return self;
}
