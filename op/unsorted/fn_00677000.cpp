#include <vector>
#include "unsorted/tAsset.h"
#include "Tiger/tstring.h"
#include "Tiger/tStream.h"
#include <utility>
// HYPOTHESIS: owning class unknown; this+0x170 is a vector of pointers, as tCastMgr holds there.
class fn_00677060_Owner
{
public:
    char pad[0x170];
    std::vector<void *> field_170;
    void* fn_00677060(unsigned int index);
    int fn_00677380(unsigned int index);
    void fn_006773e0(unsigned int index);
};

// HYPOTHESIS: element type unknown; 0x34 bytes per element, vector at this+0.
struct fn_00677b60_Element
{
    char pad[0x34];
};

class fn_00677b60_Owner
{
public:
    std::vector<fn_00677b60_Element> field_0;
    int fn_00677b60();
};

void fn_00695180( void *p ) throw();

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

// FUNCTION: STARFLEETOP 0x006773e0
void fn_00677060_Owner::fn_006773e0(unsigned int index)
{
    if (index != 0)
    {
        if (index <= field_170.size())
        {
            tCastItem* item = dynamic_cast<tCastItem*>(static_cast<tAsset*>(field_170[index - 1]));
            if (item && item->field_60 != 0 && --item->field_60 == 0 && item->field_66 != 0)
            {
                fn_00695180((void*)item->field_66);
                item->field_66 = 0;
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x00677b60
int fn_00677b60_Owner::fn_00677b60()
{
    return field_0.size();
}

// FUNCTION: STARFLEETOP 0x00677b90
void __stdcall fn_00677b90(std::pair<const tString, int>* first, std::pair<const tString, int>* last)
{
    // HYPOTHESIS: elements are string/int pairs, as the destructor symbol at the callee says.
    typedef std::pair<const tString, int> tPair;
    // HYPOTHESIS: the elements are 0x34 bytes, a pair followed by 8 more bytes.
    for (; first != last; first = (tPair*)((char*)first + 0x34))
        first->~tPair();
}

// HYPOTHESIS: 0x34-byte element: a string followed by a second, zero-initialised field group.
struct fn_00677bd0_Element
{
    tString name;
    int field_a;
    int field_b;
    int field_c;
    fn_00677bd0_Element();
    void fn_00677bf0(tStream* stream);
};

// FUNCTION: STARFLEETOP 0x00677bd0
fn_00677bd0_Element::fn_00677bd0_Element()
{
    field_b = 0;
    field_a = 0;
    field_c = 0;
}

// FUNCTION: STARFLEETOP 0x00677bf0
void fn_00677bd0_Element::fn_00677bf0(tStream* stream)
{
    tStream& s = *stream >> name;
    unsigned short* p = (unsigned short*)&field_a;
    s.fn_slot2(p, 4);
    if (s.fSwap)
    {
        unsigned short hi = p[1];
        unsigned short lo = p[0];
        p[0] = (unsigned short)((hi >> 8) | (hi << 8));
        p[1] = (unsigned short)((lo >> 8) | (lo << 8));
    }
    p = (unsigned short*)&field_b;
    s.fn_slot2(p, 4);
    if (s.fSwap)
    {
        unsigned short hi = p[1];
        unsigned short lo = p[0];
        p[0] = (unsigned short)((hi >> 8) | (hi << 8));
        p[1] = (unsigned short)((lo >> 8) | (lo << 8));
    }
    p = (unsigned short*)&field_c;
    s.fn_slot2(p, 4);
    if (s.fSwap)
    {
        unsigned short hi = p[1];
        unsigned short lo = p[0];
        p[0] = (unsigned short)((hi >> 8) | (hi << 8));
        p[1] = (unsigned short)((lo >> 8) | (lo << 8));
    }
}
