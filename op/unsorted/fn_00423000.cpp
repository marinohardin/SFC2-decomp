// HYPOTHESIS: a constructor of a class with three zeroed dword members; the class is unknown.
// FUNCTION: STARFLEETOP 0x004230b9
int* __fastcall fn_004230b9(int* p)
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    return p;
}

// HYPOTHESIS: two linked lists (heads at +0 and +4, link at +0x38); the class is unknown.
struct fn_004230c6_List {
    int fHead[2];
    void fn_00423405(int* node);
    void fn_004230c6();
};

// FUNCTION: STARFLEETOP 0x004230c6
void fn_004230c6_List::fn_004230c6()
{
    int node = fHead[0];
    while (node != 0) {
        fn_00423405((int*)node);
        int next = *(int*)(node + 0x38);
        delete (void*)node;
        node = next;
    }
    node = fHead[1];
    while (node != 0) {
        int next = *(int*)(node + 0x38);
        delete (void*)node;
        node = next;
    }
    fHead[0] = 0;
    fHead[1] = 0;
}
