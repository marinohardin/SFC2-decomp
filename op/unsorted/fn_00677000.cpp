#include <vector>
#include "unsorted/tAsset.h"

// HYPOTHESIS: owning class unknown; this+0x170 is a vector of pointers, as tCastMgr holds there.
class fn_00677060_Owner
{
public:
    char pad[0x170];
    std::vector<void *> field_170;
    void* fn_00677060(unsigned int index);
    int fn_00677380(unsigned int index);
};

// FUNCTION: STARFLEETOP 0x00677060
void* fn_00677060_Owner::fn_00677060(unsigned int index)
{
    if (index != 0)
    {
        if (index <= field_170.size())
            return field_170[index - 1];
    }
    return 0;
}

// FUNCTION: STARFLEETOP 0x00677380
int fn_00677060_Owner::fn_00677380(unsigned int index)
{
    if (index != 0)
    {
        if (index <= field_170.size())
        {
            tCastItem* item = dynamic_cast<tCastItem*>(static_cast<tAsset*>(field_170[index - 1]));
            if (item)
                return item->field_66;
        }
    }
    return 0;
}
