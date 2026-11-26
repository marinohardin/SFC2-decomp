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

extern int g_00c3244c;
extern int g_00c32450;

// HYPOTHESIS: links the object at the head of a global list (head g_00c3244c, count g_00c32450)
// FUNCTION: STARFLEETOP 0x00537bd8
void __fastcall fn_00537bd8(int* self)
{
    self[6] = 0;
    self[7] = g_00c3244c;
    if (g_00c3244c != 0) {
        ((int*)g_00c3244c)[6] = (int)self;
    }
    g_00c32450++;
    g_00c3244c = (int)self;
}
