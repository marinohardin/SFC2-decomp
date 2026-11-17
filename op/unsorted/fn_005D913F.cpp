#include <windows.h>
#include <string>

// HYPOTHESIS: 0x0046de63 is a non-virtual member called on a held pointer (a release
// or close); its class is not known yet.
class fn_0046de63_object
{
public:
    void FUN_0046de63();

    long m00;
    long m04; // HYPOTHESIS: a reference count, raised with InterlockedIncrement on copy
};

// HYPOTHESIS: an object holding a std::string at +0x00 (its destructor is the inlined
// _Tidy(true) at the end) and a pointer at +0x10.
class fn_005d913f_object
{
public:
    fn_005d913f_object(const fn_005d913f_object &other);
    ~fn_005d913f_object();

    std::string m00;
    fn_0046de63_object *m10;
};

// FUNCTION: STARFLEETOP 0x005d913f
fn_005d913f_object::~fn_005d913f_object()
{
    if (m10)
        m10->FUN_0046de63();
}

// FUNCTION: STARFLEETOP 0x005d917a
fn_005d913f_object::fn_005d913f_object(const fn_005d913f_object &other)
    : m00(other.m00)
{
    fn_0046de63_object *p = other.m10;
    m10 = p ? (::InterlockedIncrement(&p->m04), p) : 0;
}

// HYPOTHESIS: a counted pointer to an fn_0046de63_object held at +0x00; this assigns a
// raw pointer, taking a reference on the new one and releasing the old.
class fn_005d91cc_object
{
public:
    fn_0046de63_object *operator=(fn_0046de63_object *p);

    fn_0046de63_object *m00;
};

// FUNCTION: STARFLEETOP 0x005d91cc
fn_0046de63_object *fn_005d91cc_object::operator=(fn_0046de63_object *p)
{
    if (m00 != p)
    {
        if (m00)
            m00->FUN_0046de63();
        m00 = p ? (::InterlockedIncrement(&p->m04), p) : 0;
    }
    return p;
}

// HYPOTHESIS: the object a tree node at 0x005d9200 points to: a counted pointer at +0x00,
// released by its inlined destructor.
class fn_005d9200_value
{
public:
    ~fn_005d9200_value();

    fn_0046de63_object *m00;
};

inline fn_005d9200_value::~fn_005d9200_value()
{
    if (m00)
        m00->FUN_0046de63();
}

// HYPOTHESIS: a 16-byte balanced tree (a head node at +0x04 whose left link is the first
// node; each node holds its key at +0x0c and its value at +0x10). Its destructor
// (0x005d9281) and its iterator's increment (0x0052e144) are called out of line.
struct fn_005d9281_tree
{
    struct node
    {
        node *left;
        node *parent;
        node *right;
        long first;
        fn_005d9200_value *second;
    };
    class iterator
    {
    public:
        iterator(node *n) : m_node(n) {}
        node &operator*() const { return *m_node; }
        iterator &fn_0052e144();
        iterator &operator++() { return fn_0052e144(); }
        bool operator!=(const iterator &other) const { return m_node != other.m_node; }
        node *m_node;
    };
    ~fn_005d9281_tree();
    iterator begin() { return iterator(m_head->left); }
    iterator end() { return iterator(m_head); }
    char m_0x00[0x04];
    node *m_head;
    char m_0x08[0x08];
};

// HYPOTHESIS: a member at +0x14 whose destructor is 0x005d92b9.
class fn_005d92b9_object
{
public:
    ~fn_005d92b9_object();

    long m00;
};

// HYPOTHESIS: an object owning a tree of values at +0x00, deleting each value when destroyed.
class fn_005d9200_object
{
public:
    ~fn_005d9200_object();

    fn_005d9281_tree m00;
    long m10;
    fn_005d92b9_object m14;
};

// FUNCTION: STARFLEETOP 0x005d9200
fn_005d9200_object::~fn_005d9200_object()
{
    for (fn_005d9281_tree::iterator it = m00.begin(); it != m00.end(); ++it)
        delete (*it).second;
}
