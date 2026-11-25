#include "unsorted/tEntityMgr.h"

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

// HYPOTHESIS: a list head at +0 with link at +0x38/+0x3c; the class is unknown.
struct fn_00423232_List {
    int* fHead;
    void fn_004231e9(int* node, int* other);
    void fn_004232aa(int* node);
    void fn_004232ff(int* node);
    void fn_00423232(int* node);
};

// FUNCTION: STARFLEETOP 0x00423232
void fn_00423232_List::fn_00423232(int* node)
{
    int* last = 0;
    int* cur = fHead;
    if (cur != 0) {
        do {
            int* next;
            if (*node == 0 && *(char*)(cur + 8) == 0) {
                fn_004231e9(cur, node);
                next = (int*)cur[0xe];
                fn_004232aa(cur);
            } else {
                next = (int*)cur[0xe];
                last = cur;
            }
            cur = next;
        } while (cur != 0);
    }
    if (last != 0) {
        last[0xe] = (int)node;
        node[0xe] = 0;
        node[0xf] = (int)last;
    } else {
        fHead = node;
        node[0xe] = 0;
        fHead[0xf] = 0;
    }
    if (*node == 1) {
        fn_004232ff(node);
    }
}

// HYPOTHESIS: a two-ended list (head at +0, tail at +4, links at +0x38/+0x3c); moves a node to the tail.
struct fn_004232aa_List {
    int* fHead;
    int* fTail;
    void fn_00423405(int* node);
    void fn_004232aa(int* node);
};

// FUNCTION: STARFLEETOP 0x004232aa
void fn_004232aa_List::fn_004232aa(int* node)
{
    int* prev = (int*)node[0xf];
    int* next = (int*)node[0xe];
    fn_00423405(node);
    if (prev != 0) {
        prev[0xe] = (int)next;
    } else {
        fHead = next;
    }
    if (next != 0) {
        next[0xf] = (int)prev;
    }
    if (fTail != 0) {
        fTail[0xf] = (int)node;
        node[0xf] = 0;
        node[0xe] = (int)fTail;
        fTail = node;
    } else {
        fTail = node;
        node[0xf] = 0;
        fTail[0xe] = 0;
    }
}

// HYPOTHESIS: the way point marker entity (0x1bc bytes, constructor 0x00410a4d); the class has no
// header yet, so it is declared here with what this function uses.
class tEntityWayPntMarker : public tEntity {
public:
    tEntityWayPntMarker();
    void fn_0041089e(tPoint3* aPos);
    char fPad[0x1bc - sizeof(tEntity)];
};

// HYPOTHESIS: the object the handler asks for the owner entity (slot 5), which carries an ID at +0x1c.
struct fn_004232ff_owner_entity {
    int fPad[7];
    tID fID;
};

// HYPOTHESIS: the object held at +8 of the handler; slot 5 returns the owner entity.
class fn_004232ff_owner {
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual fn_004232ff_owner_entity* vfunction5();
};

// HYPOTHESIS: the request: a point at +8 and an entity ID at +0x18.
struct fn_004232ff_request {
    int fPad[2];
    tPoint3 fPos;
    char fPad2[0x18 - 0x8 - sizeof(tPoint3)];
    tID fID;
};

// HYPOTHESIS: a handler with an owner object at +8; the class is unknown.
struct fn_004232ff_Handler {
    int fField0;
    int fField4;
    fn_004232ff_owner* fOwner;
    void fn_004232ff(fn_004232ff_request* request);
};

extern tEntityMgr gEntityMgr_008e1060;

// FUNCTION: STARFLEETOP 0x004232ff
void fn_004232ff_Handler::fn_004232ff(fn_004232ff_request* request)
{
    tEntityWayPntMarker* marker = new tEntityWayPntMarker;
    if (marker != 0) {
        tEntityMgr* mgr = &gEntityMgr_008e1060;
        if (!mgr->fn_004235fa(fOwner->vfunction5()->fID)) {
            marker->fProperties &= ~1;
        }
        marker->fID = request->fID;
        marker->fn_0041089e(&request->fPos);
        mgr->mAddEntity(marker);
    }
}
