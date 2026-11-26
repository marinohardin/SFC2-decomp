#include <vector>
#include <utility>
#include "Tiger/tstring.h"

// HYPOTHESIS: class unknown. 0x10..0x24 is a std::map<tString, int> in the original, whose
// constructor is inlined there; the build does not inline it (see the notes in the handoff).
class fn_00678500_Class
{
public:
    std::vector<int> field_0;
    char pad10[0x14];
    int field_24;
    tString field_28;
    fn_00678500_Class();
};

// FUNCTION: STARFLEETOP 0x00678500
fn_00678500_Class::fn_00678500_Class()
{
    field_24 = 0;
}

// HYPOTHESIS: the original is an instantiated std::vector<tString> member (its clear-and-free
// code); written here as a plain class because the annotation cannot sit on a template member.
class fn_006785c0_StringVector
{
public:
    int field_0;
    std::pair<const tString, int>* field_4;
    std::pair<const tString, int>* field_8;
    std::pair<const tString, int>* field_C;
    void fn_006785c0();
};

// FUNCTION: STARFLEETOP 0x006785c0
void fn_006785c0_StringVector::fn_006785c0()
{
    std::pair<const tString, int>* end = field_8;
    for (std::pair<const tString, int>* p = field_4; p != end; ++p)
        p->~pair();
    operator delete(field_4);
    field_C = field_8 = field_4 = 0;
}
