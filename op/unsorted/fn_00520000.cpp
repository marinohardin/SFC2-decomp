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

// FUNCTION: STARFLEETOP 0x005208d2
fn_005208d2_Elem* fn_005208d2_Vec::fn_005208d2(fn_005208d2_Elem* dest, fn_005208d2_Elem* src)
{
    fn_005208d2_Elem* end = last;
    fn_005208d2_Elem* p = dest;
    for (; src != end; ++src, ++p) {
        *p = *src;
    }
    last = p;
    return dest;
}
