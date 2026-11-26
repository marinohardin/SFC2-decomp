#include <vector>

// HYPOTHESIS: owning class unknown; this+0x170 is a vector of pointers, as tCastMgr holds there.
class fn_00677060_Owner
{
public:
    char pad[0x170];
    std::vector<void *> field_170;
    void* fn_00677060(unsigned int index);
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
