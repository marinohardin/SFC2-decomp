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

// HYPOTHESIS: unlinks the object from the global list started by fn_00537bd8
// FUNCTION: STARFLEETOP 0x00537bfd
void __fastcall fn_00537bfd(int* self)
{
    if (g_00c32450 == 0) {
        g_00c32450 = 1;
    }
    if (g_00c3244c == (int)self) {
        g_00c3244c = self[7];
    }
    if (self[6] != 0) {
        ((int*)self[6])[7] = self[7];
    }
    if (self[7] != 0) {
        ((int*)self[7])[6] = self[6];
    }
    self[7] = 0;
    self[6] = 0;
    g_00c32450--;
}
