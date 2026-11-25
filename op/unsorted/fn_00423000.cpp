// HYPOTHESIS: a constructor of a class with three zeroed dword members; the class is unknown.
// FUNCTION: STARFLEETOP 0x004230b9
int* __fastcall fn_004230b9(int* p)
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    return p;
}
