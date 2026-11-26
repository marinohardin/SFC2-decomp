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

// HYPOTHESIS: the original is the clear-and-free teardown of a tree whose layout (head at +4,
// nil at +8, size at +0x10) the standard map here does not have; this is a placeholder attempt.
class fn_00678600_Tree
{
public:
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    ~fn_00678600_Tree();
};

// FUNCTION: STARFLEETOP 0x00678600
fn_00678600_Tree::~fn_00678600_Tree()
{
    operator delete((void *)field_4);
    field_4 = 0;
    field_10 = 0;
    operator delete((void *)field_8);
    field_8 = 0;
}

// HYPOTHESIS: the original is a std::map tree's Erase(node); node links are left at +0 and
// right at +8, the nil node is at +8 of the tree.
struct fn_006786d0_Node
{
    fn_006786d0_Node* left;
    fn_006786d0_Node* parent;
    fn_006786d0_Node* right;
};

class fn_006786d0_Tree
{
public:
    int field_0;
    int field_4;
    fn_006786d0_Node* field_8;
    void fn_006786d0(fn_006786d0_Node* node);
    fn_006786d0_Node* Nil() { return field_8; }
};

// FUNCTION: STARFLEETOP 0x006786d0
void fn_006786d0_Tree::fn_006786d0(fn_006786d0_Node* node)
{
    for (fn_006786d0_Node* y = node; y != Nil(); node = y) {
        fn_006786d0(y->right);
        y = y->left;
        operator delete(node);
    }
}

// HYPOTHESIS: the original is a std::map<tString, int> tree's Erase(node); the node holds
// the string at +0xc, the nil node is at +8 of the tree.
struct fn_00678aa0_Node
{
    fn_00678aa0_Node* left;
    fn_00678aa0_Node* parent;
    fn_00678aa0_Node* right;
    std::pair<const tString, int> value;
};

class fn_00678aa0_Tree
{
public:
    int field_0;
    int field_4;
    fn_00678aa0_Node* field_8;
    void fn_00678aa0(fn_00678aa0_Node* node);
};

// FUNCTION: STARFLEETOP 0x00678aa0
void fn_00678aa0_Tree::fn_00678aa0(fn_00678aa0_Node* node)
{
    for (fn_00678aa0_Node* y = node; y != field_8; node = y) {
        fn_00678aa0(y->right);
        y = y->left;
        node->value.~pair();
        operator delete(node);
    }
}
