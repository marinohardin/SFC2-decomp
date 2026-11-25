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

// HYPOTHESIS: a node pool: free list head at +0, a stored value at +4, link at +0x38/+0x3c.
struct fn_0042310b_Pool {
    int fField0;
    int* fFree;
    int fValue;
    int* fn_0042310b(int value);
};

// FUNCTION: STARFLEETOP 0x0042310b
int* fn_0042310b_Pool::fn_0042310b(int value)
{
    if (fValue == 0) {
        fValue = value;
    }
    int* node = fFree;
    if (node != 0) {
        int* next = (int*)node[0xe];
        fFree = next;
        if (next != 0) {
            next[0xf] = 0;
        }
        node[0xe] = 0;
        node[0xf] = 0;
    } else {
        node = (int*)operator new(0x40);
    }
    return node;
}

// HYPOTHESIS: a list head at +0 with link at +0x38; each node with a nonzero first dword is handled.
struct fn_00423142_List {
    int* fHead;
    void fn_004232aa(int* node);
    void fn_00423142();
};

// FUNCTION: STARFLEETOP 0x00423142
void fn_00423142_List::fn_00423142()
{
    int* cur = fHead;
    while (cur != 0) {
        int* next = (int*)cur[0xe];
        if (*cur != 0) {
            fn_004232aa(cur);
        }
        cur = next;
    }
}

// HYPOTHESIS: two linked lists (heads at +0 and +4, link at +0x38/+0x3c); the class is unknown.
struct fn_00423165_List {
    int* fHead;
    int* fOld;
    void fn_00423405(int* node);
    void fn_004232ff(int* node);
    void fn_00423165(int* node);
};

// FUNCTION: STARFLEETOP 0x00423165
void fn_00423165_List::fn_00423165(int* cur)
{
    int* first = fHead;
    int* old = fOld;
    int* node = cur;
    fHead = node;
    if (first != 0) {
        node[0xf] = 0;
        fHead[0xe] = 0;
        if (old != 0) {
            while (old[0xe] != 0) {
                old = (int*)old[0xe];
            }
            old[0xe] = (int)first;
            first[0xf] = (int)old;
            do {
                fn_00423405(first);
                first = (int*)first[0xe];
            } while (first != 0);
        } else {
            fOld = first;
            cur = first;
            do {
                fn_00423405(cur);
                cur = (int*)cur[0xe];
            } while (cur != 0);
            first[0xf] = 0;
        }
    } else {
        node[0xe] = 0;
        fHead[0xf] = 0;
    }
    if (*node == 1) {
        fn_004232ff(node);
    }
}
