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

// HYPOTHESIS: a 20-byte balanced tree (a head node at +0x04 whose left link is the first
// node, a second node at +0x08, its size at +0x10; each node holds its key at +0x0c and its
// value at +0x10). Its destructor (0x005d9281), its range erase (0x005d9329) and its
// iterator's increment (0x0052e144) are called out of line.
struct fn_005d9281_tree
{
    struct node
    {
        node *left;
        node *parent;
        node *right;
        long first;
        fn_005d9200_value *second;
        char color; // red 0, black 1
        char isnil;
    };
    class iterator
    {
    public:
        iterator(node *n) : m_node(n) {}
        node &operator*() const { return *m_node; }
        iterator &fn_0052e144();
        iterator &operator++() { return fn_0052e144(); }
        iterator operator++(int) { iterator tmp = *this; ++*this; return tmp; }
        bool operator!=(const iterator &other) const { return m_node != other.m_node; }
        node *m_node;
    };
    ~fn_005d9281_tree();
    iterator fn_005d9329(iterator first, iterator last);
    iterator fn_0060ac82(iterator where);
    void fn_005d9411(node *n);
    iterator begin() { return iterator(m_head->left); }
    iterator end() { return iterator(m_head); }
    char m_0x00[0x04];
    node *m_head;
    node *m_nil;
    char m_0x0c[0x04];
    long m_size;
};

// HYPOTHESIS: a member at +0x14, a second 20-byte balanced tree of the same shape whose
// destructor is 0x005d92b9 and whose range erase is 0x005d939d.
struct fn_005d92b9_object
{
    // HYPOTHESIS: its range erase steps with the same out-of-line increment (0x0052e144) as
    // fn_005d9281_tree, so the two trees share one node and iterator shape here.
    typedef fn_005d9281_tree::node node;
    typedef fn_005d9281_tree::iterator iterator;
    ~fn_005d92b9_object();
    iterator fn_005d939d(iterator first, iterator last);
    iterator fn_005d9441(iterator where);
    void fn_005d96ac(node *n);
    void fn_00556fd0(node *n);
    static node *fn_min(node *p);
    static node *fn_max(node *p);
    void fn_00558526(node *n);
    iterator begin() { return iterator(m_head->left); }
    iterator end() { return iterator(m_head); }
    char m_0x00[0x04];
    node *m_head;
    node *m_nil;
    char m_0x0c[0x04];
    long m_size;
};

// HYPOTHESIS: an object owning a tree of values at +0x00, deleting each value when destroyed.
class fn_005d9200_object
{
public:
    ~fn_005d9200_object();

    fn_005d9281_tree m00;
    fn_005d92b9_object m14;
};

// FUNCTION: STARFLEETOP 0x005d9200
fn_005d9200_object::~fn_005d9200_object()
{
    for (fn_005d9281_tree::iterator it = m00.begin(); it != m00.end(); ++it)
        delete (*it).second;
}

// FUNCTION: STARFLEETOP 0x005d9281
fn_005d9281_tree::~fn_005d9281_tree()
{
    fn_005d9329(begin(), end());
    operator delete(m_head);
    m_head = 0, m_size = 0;
    operator delete(m_nil);
    m_nil = 0;
}

// FUNCTION: STARFLEETOP 0x005d92b9
fn_005d92b9_object::~fn_005d92b9_object()
{
    fn_005d939d(begin(), end());
    operator delete(m_head);
    m_head = 0, m_size = 0;
    operator delete(m_nil);
    m_nil = 0;
}

// HYPOTHESIS: a doubly linked list of fn_005d913f_object values (an allocator byte at +0x00,
// its head node at +0x04, its size at +0x08; each node holds next, prev, then the value).
struct fn_005d92f1_list
{
    struct node
    {
        node *next;
        node *prev;
        fn_005d913f_object value;
    };
    class iterator
    {
    public:
        iterator(node *n) : m_node(n) {}
        iterator operator++(int) { iterator tmp = *this; m_node = m_node->next; return tmp; }
        node *m_node;
    };
    iterator erase(iterator where);
    char m_0x00[0x04];
    node *m_head;
    long m_size;
};

// FUNCTION: STARFLEETOP 0x005d92f1
fn_005d92f1_list::iterator fn_005d92f1_list::erase(iterator where)
{
    node *s = (where++).m_node;
    s->prev->next = s->next;
    s->next->prev = s->prev;
    (&s->value)->~fn_005d913f_object();
    operator delete(s);
    --m_size;
    return where;
}

// FUNCTION: STARFLEETOP 0x005d9329
fn_005d9281_tree::iterator fn_005d9281_tree::fn_005d9329(iterator first, iterator last)
{
    if (m_size == 0 || first.m_node != m_head->left || last.m_node != m_head)
    {
        while (first != last)
            fn_0060ac82(first++);
        return first;
    }
    else
    {
        fn_005d9411(m_head->parent);
        m_head->parent = m_nil;
        m_size = 0;
        m_head->left = m_head;
        m_head->right = m_head;
        return begin();
    }
}

// FUNCTION: STARFLEETOP 0x005d939d
fn_005d92b9_object::iterator fn_005d92b9_object::fn_005d939d(iterator first, iterator last)
{
    if (m_size == 0 || first.m_node != m_head->left || last.m_node != m_head)
    {
        while (first != last)
            fn_005d9441(first++);
        return first;
    }
    else
    {
        fn_005d96ac(m_head->parent);
        m_head->parent = m_nil;
        m_size = 0;
        m_head->left = m_head;
        m_head->right = m_head;
        return begin();
    }
}

// FUNCTION: STARFLEETOP 0x005d9411
void fn_005d9281_tree::fn_005d9411(node *n)
{
    for (node *p = n; p != m_nil; n = p)
    {
        fn_005d9411(p->right);
        p = p->left;
        operator delete(n);
    }
}

// HYPOTHESIS: the value a node of fn_005d92b9_object holds at +0x0c; 0x005d6a66 destroys it
// in place.
class fn_005d6a66_value
{
public:
    void fn_005d6a66();
};

// MATCH-ONLY: the leftmost and rightmost node searches, inlined into the erase below.
inline fn_005d92b9_object::node *fn_005d92b9_object::fn_min(node *p)
{
    while (!p->left->isnil)
        p = p->left;
    return p;
}

inline fn_005d92b9_object::node *fn_005d92b9_object::fn_max(node *p)
{
    while (!p->right->isnil)
        p = p->right;
    return p;
}

// HYPOTHESIS: the library's red-black tree erase; 0x00556fd0 and 0x00558526 are its left
// and right rotations.
// FUNCTION: STARFLEETOP 0x005d9441
fn_005d92b9_object::iterator fn_005d92b9_object::fn_005d9441(iterator where)
{
    node *x;
    node *z = (where++).m_node;
    node *y = z;
    if (y->left == m_nil)
        x = y->right;
    else if (y->right == m_nil)
        x = y->left;
    else
    {
        y = fn_min(y->right);
        x = y->right;
    }
    if (y != z)
    {
        z->left->parent = y;
        y->left = z->left;
        if (y == z->right)
            x->parent = y;
        else
        {
            x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        }
        if (m_head->parent == z)
            m_head->parent = y;
        else if (z->parent->left == z)
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        char c = y->color;
        y->color = z->color;
        z->color = c;
        y = z;
    }
    else
    {
        x->parent = y->parent;
        if (m_head->parent == z)
            m_head->parent = x;
        else if (z->parent->left == z)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (m_head->left != z)
            ;
        else if (z->right == m_nil)
            m_head->left = z->parent;
        else
            m_head->left = fn_min(x);
        if (m_head->right != z)
            ;
        else if (z->left == m_nil)
            m_head->right = z->parent;
        else
            m_head->right = fn_max(x);
    }
    if (y->color == 1)
    {
        for (; x != m_head->parent && x->color == 1; x = x->parent)
        {
            if (x == x->parent->left)
            {
                node *w = x->parent->right;
                if (w->color == 0)
                {
                    w->color = 1;
                    x->parent->color = 0;
                    fn_00556fd0(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == 1 && w->right->color == 1)
                    w->color = 0;
                else
                {
                    if (w->right->color == 1)
                    {
                        w->left->color = 1;
                        w->color = 0;
                        fn_00558526(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 1;
                    w->right->color = 1;
                    fn_00556fd0(x->parent);
                    break;
                }
            }
            else
            {
                node *w = x->parent->left;
                if (w->color == 0)
                {
                    w->color = 1;
                    x->parent->color = 0;
                    fn_00558526(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == 1 && w->left->color == 1)
                    w->color = 0;
                else
                {
                    if (w->left->color == 1)
                    {
                        w->right->color = 1;
                        w->color = 0;
                        fn_00556fd0(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 1;
                    w->left->color = 1;
                    fn_00558526(x->parent);
                    break;
                }
            }
        }
        x->color = 1;
    }
    ((fn_005d6a66_value *)&y->first)->fn_005d6a66();
    operator delete(y);
    --m_size;
    return where;
}

// FUNCTION: STARFLEETOP 0x005d96ac
void fn_005d92b9_object::fn_005d96ac(node *n)
{
    for (node *p = n; p != m_nil; n = p)
    {
        fn_005d96ac(p->right);
        p = p->left;
        ((fn_005d6a66_value *)&n->first)->fn_005d6a66();
        operator delete(n);
    }
}
