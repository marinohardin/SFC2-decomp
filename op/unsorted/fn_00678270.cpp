#include <vector>
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
