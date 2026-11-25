// HYPOTHESIS: a constructor of a class with three zeroed dword members; the class is unknown.
// FUNCTION: STARFLEETOP 0x004230b9
int* __fastcall fn_004230b9(int* p)
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    return p;
}

void __fastcall fn_00423405(int* list, int unused, int* node);

// HYPOTHESIS: two linked lists (heads at +0 and +4, link at +0x38); the class is unknown.
// FUNCTION: STARFLEETOP 0x004230c6
void __fastcall fn_004230c6(int* p)
{
    int node = p[0];
    while (node != 0) {
        fn_00423405(p, 0, (int*)node);
        int next = *(int*)(node + 0x38);
        delete (void*)node;
        node = next;
    }
    node = p[1];
    while (node != 0) {
        int next = *(int*)(node + 0x38);
        delete (void*)node;
        node = next;
    }
    p[0] = 0;
    p[1] = 0;
}
