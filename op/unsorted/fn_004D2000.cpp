// HYPOTHESIS: a red-black tree (std::set<unsigned long>) lower_bound; node layout from the exe.
struct fn_004d2143_node
{
    fn_004d2143_node* m00;
    fn_004d2143_node* m04;
    fn_004d2143_node* m08;
    unsigned long m0c;
};

struct fn_004d2143_tree
{
    int m00;
    fn_004d2143_node* m04;
    fn_004d2143_node* m08;
    fn_004d2143_node* fn_004d2143(const unsigned long& key);
};

// FUNCTION: STARFLEETOP 0x004d2143
fn_004d2143_node* fn_004d2143_tree::fn_004d2143(const unsigned long& key)
{
    fn_004d2143_node* x = m04->m04;
    fn_004d2143_node* y = m04;
    while (x != m08)
    {
        if (x->m0c < key)
            x = x->m08;
        else
        {
            y = x;
            x = x->m00;
        }
    }
    return y;
}

// HYPOTHESIS: a red-black tree iterator step (operator-- of a set iterator); node layout from the exe.
struct fn_004d216d_node
{
    fn_004d216d_node* m00;
    fn_004d216d_node* m04;
    fn_004d216d_node* m08;
    unsigned long m0c;
    char m10;
    char m11;
};

struct fn_004d216d_iter
{
    fn_004d216d_node* m00;
    void fn_004d216d();
};

// FUNCTION: STARFLEETOP 0x004d216d
void fn_004d216d_iter::fn_004d216d()
{
    fn_004d216d_node* y;
    if (m00->m10 == 0 && m00->m04->m04 == m00)
        y = m00->m08;
    else
    {
        fn_004d216d_node* x = m00->m00;
        if (x->m11 == 0)
        {
            while (x->m08->m11 == 0)
                x = x->m08;
            m00 = x;
            return;
        }
        while (y = m00->m04, m00 == y->m00)
            m00 = y;
    }
    m00 = y;
}

#include "Common/Thread/Optex.h"

// HYPOTHESIS: the SDK's tSingleAccessWrapper<tObject_005391b0> (declared only inside
// unsorted/tClientCharacter.cpp); this is its destructor, Delete then the optex member.
class tSingleAccessWrapper_005391b0
{
public:
	~tSingleAccessWrapper_005391b0();
	void Delete();

protected:
	void* fpPointer;
	mutable tOptex fPointerOptex;
};

// FUNCTION: STARFLEETOP 0x004d21a8
tSingleAccessWrapper_005391b0::~tSingleAccessWrapper_005391b0()
{
	Delete();
}
