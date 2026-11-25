// HYPOTHESIS: the symbol table gives std::vector<tObject_0051e3ac_entry>::erase(first, last);
// that element type is declared only inside tAICaptain.cpp, so a reduced layout stands in here
// (16-byte elements, first at +4, last at +8) until it has a shared header.
struct fn_005208d2_Elem {
    int a;
    int b;
    int c;
    int d;
};

struct fn_005208d2_Vec {
    int field_0;
    fn_005208d2_Elem* first;
    fn_005208d2_Elem* last;
    fn_005208d2_Elem* fn_005208d2(fn_005208d2_Elem* dest, fn_005208d2_Elem* src);
};

// MATCH-ONLY: stands for the copy and destroy helpers the original inlined; they take the
// position by reference.
inline fn_005208d2_Elem* fn_005208d2_Copy(fn_005208d2_Elem* f, fn_005208d2_Elem* l, fn_005208d2_Elem* d)
{
    for (; f != l; ++f, ++d) {
        *d = *f;
    }
    return d;
}

inline void fn_005208d2_Destroy(fn_005208d2_Elem*& f, fn_005208d2_Elem* l)
{
    f = l;
}

// FUNCTION: STARFLEETOP 0x005208d2
fn_005208d2_Elem* fn_005208d2_Vec::fn_005208d2(fn_005208d2_Elem* dest, fn_005208d2_Elem* src)
{
    fn_005208d2_Elem* i = fn_005208d2_Copy(src, last, dest);
    fn_005208d2_Destroy(src, last);
    last = i;
    return dest;
}

// HYPOTHESIS: a tree-node allocator (first two links default to the node itself); the
// node's third word is left unset. Pointer-sized words stand in for the node type.
// FUNCTION: STARFLEETOP 0x0052090a
int* __stdcall fn_0052090a(int* a, int* b)
{
    int* node = (int*)operator new(0xc);
    node[0] = (int)(a ? a : node);
    node[1] = (int)(b ? b : node);
    return node;
}

#include "unsorted/tAIData.h"

// FUNCTION: STARFLEETOP 0x00520966
tAIData::tAIData(char param_1, int param_2)
{
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    m_10 = param_1;
    field_14 = param_2;
    field_18 = 0;
    field_1c = 0;
}

// HYPOTHESIS: the real body at 0x005209eb stores the tETrait vtable, calls tETrait's destructor and
// frees the pointer at +0x18 through 0x005288cb. tETrait has no header and 0x005288cb no
// declaration yet, so the body stays empty and only the deleting destructor is claimed.
// MATCH-ONLY: the empty body stands in for the real destructor so the compiler emits the exact deleting destructor.
// FUNCTION: STARFLEETOP 0x005209a3
tAIData::~tAIData()
{
}
