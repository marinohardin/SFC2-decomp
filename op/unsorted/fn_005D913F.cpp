#include <windows.h>
#include <string>
#include "unsorted/tMetaMapHex.h"
#include "Common/Settings.h"

// HYPOTHESIS: 0x0046de63 is a non-virtual member called on a held pointer (a release
// or close); its class is not known yet.
class fn_0046de63_object
{
public:
    fn_0046de63_object(const char *data, unsigned long size, int param_3);
    void FUN_0046de63();

    long m00;
    long m04; // HYPOTHESIS: a reference count, raised with InterlockedIncrement on copy
    long m08;
    long m0c;
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
    fn_005d91cc_object() : m00(0) {}
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

// HYPOTHESIS: a global at 0x00c38968 holding a byte flag and the dword after it, both cleared
// by an inline constructor; its destructor (0x005d978b) tests the flag before releasing the
// dword, like an owning pointer. The type's name is not known. The original's atexit destructor
// (0x005d976b) is guarded by a bit at 0x00c38960, which a plain global does not produce; it
// may have been a static data member of a template. The flag and pointer pair, cleared together
// and deleted only while the flag is set, has the shape of the standard library's auto_ptr.
// HYPOTHESIS: the pointee's destructor is 0x005d97a8 (reached through the jump at 0x005d9786);
// tCampaignState.cpp assigns that address to its c_0045dece, declared only in that unit.
class fn_005d97a8_object
{
public:
    ~fn_005d97a8_object();
    void fn_005f7aeb();
    void fn_005d97c1();
private:
    char m_00[0x4];
    void **m_items;
    unsigned int m_count;
    char m_0c[0x4];
    unsigned int m_size;
};

class fn_005d978b_object
{
public:
    fn_005d978b_object() : m_flag(0), m_value(0) {}
    ~fn_005d978b_object();
private:
    char m_flag;
    fn_005d97a8_object *m_value;
};

// The compiler generates from this definition the dynamic initializer (0x005d9750, the inline
// constructor expanded) and the atexit registration of the destructor (0x005d975f).
// FUNCTION: STARFLEETOP 0x005d975f
// FUNCTION: STARFLEETOP 0x005d9750
fn_005d978b_object g_00c38968;

// FUNCTION: STARFLEETOP 0x005d978b
fn_005d978b_object::~fn_005d978b_object()
{
    if (m_flag)
        delete m_value;
}

// FUNCTION: STARFLEETOP 0x005d97a8
fn_005d97a8_object::~fn_005d97a8_object()
{
    while (m_size != 0)
        fn_005f7aeb();
    fn_005d97c1();
}

// FUNCTION: STARFLEETOP 0x005d97c1
void fn_005d97a8_object::fn_005d97c1()
{
    for (unsigned int i = m_count; i > 0; )
        operator delete(m_items[--i]);
    operator delete(m_items);
    m_count = 0;
    m_items = 0;
}

// FUNCTION: STARFLEETOP 0x005d9831
// HYPOTHESIS: returns a byte truth value; mov al, 1 / xor al, al rules out bool's eax forms.
unsigned char fn_005d9831(int param_1)
{
    if (param_1 >= 8 && param_1 <= 15)
        return 1;
    return 0;
}

// FUNCTION: STARFLEETOP 0x005d9845
// HYPOTHESIS: returns a byte truth value, as fn_005d9831 does.
unsigned char fn_005d9845(int param_1)
{
    if (param_1 < 0 || param_1 > 7)
        return 0;
    return 1;
}

// FUNCTION: STARFLEETOP 0x005d9859
int fn_005d9859(int param_1)
{
    if (fn_005d9831(param_1))
        return 1;
    if (fn_005d9845(param_1))
        return 0;
    return -1;
}

// HYPOTHESIS: param_1 is a tMetaMapHex (0x0053a2fc returns its map, 0x0053aa7a is that map's
// find); a key of -1 falls back to the entry for key 0.
// FUNCTION: STARFLEETOP 0x005d987d
void fn_005d987d(tMetaMapHex *param_1, int param_2, float *param_3, int *param_4)
{
    if (param_3 || param_4)
    {
        std::map< int32, float >::iterator it;
        int32 key = fn_005d9859(param_2);
        it = param_1->fn_0053a2fc().find(key);
        // MATCH-ONLY: the original loads the value with mov before the test, an integer copy;
        // the map's value type may be a 32-bit integer rather than the float tMetaMapHex.h guesses.
        int32 value = *(int32 *)&it->second;
        if (key == -1)
        {
            if (param_3)
                *param_3 = param_1->fn_0053a2fc().find(0)->second;
            if (param_4)
                *param_4 = 0;
        }
        else
        {
            if (param_3)
                *(int32 *)param_3 = value;
            if (param_4)
                *param_4 = key;
        }
    }
}

// HYPOTHESIS: returns the game's settings object; its name and owner are not known.
tSettings *fn_0040c868();

// FUNCTION: STARFLEETOP 0x005d9901
std::string fn_005d9901(unsigned int param_1)
{
    std::string unused;
    std::vector< std::string > names;
    fn_0040c868()->GetList("RaceNames/FullEmpireNames", &names, 0);
    if (names.size() > param_1)
        return names[param_1];
    return "UNKNOWN RACE";
}

// FUNCTION: STARFLEETOP 0x005d9a6f
BOOL fn_005d9a6f(const char *param_1)
{
    char buf[20] = "0x";
    __int64 value;
    unsigned int check;
    strncpy(buf + 2, param_1 + 1, 3);
    strncpy(buf + 5, param_1 + 5, 4);
    strncpy(buf + 9, param_1 + 10, 3);
    buf[12] = 0;
    sscanf(buf, "%I64x", &value);
    strncpy(buf + 2, param_1 + 15, 4);
    buf[6] = 0;
    sscanf(buf, "%x", &check);
    return (short)check == (short)((short)(value % 0xffff) ^ (short)0x9249);
}

class fn_006ed560_object;
class fn_006ee3d0_result;

// HYPOTHESIS: an object built from a string: the string at +0x00, a dword at +0x10 and a
// byte at +0x14, both cleared.
class fn_005d9b26_object
{
public:
    fn_005d9b26_object(const std::string &name);
    void fn_005d9b58(const char *param_1);
    void fn_005d9bb8(const std::string &param_1);
    std::string fn_005d9be5();
    void fn_005d9cd3(fn_006ed560_object *param_1) const;
    fn_006ee3d0_result fn_005d9db1() const;

    std::string m00;
    fn_005d91cc_object m10;
    char m14;
};

// FUNCTION: STARFLEETOP 0x005d9b26
fn_005d9b26_object::fn_005d9b26_object(const std::string &name)
    : m00(name), m10(), m14(0)
{
}

// HYPOTHESIS: the length passed with the data to 0x006e15b0, the 16-byte counted object's
// constructor.
extern unsigned long g_00c38970;

// FUNCTION: STARFLEETOP 0x005d9b58
void fn_005d9b26_object::fn_005d9b58(const char *param_1)
{
    m10 = new fn_0046de63_object(param_1, g_00c38970, 0);
    m14 = fn_005d9a6f(param_1) != FALSE;
}

// FUNCTION: STARFLEETOP 0x005d9bb8
void fn_005d9b26_object::fn_005d9bb8(const std::string &param_1)
{
    m14 = 0;
    m10 = 0;
    fn_005d9b58(param_1.c_str());
}

// HYPOTHESIS: copies the held object's data (its +0x08, g_00c38970 bytes long) out as a
// string; an empty string when nothing is held.
// FUNCTION: STARFLEETOP 0x005d9be5
std::string fn_005d9b26_object::fn_005d9be5()
{
    if (m10.m00 == 0)
        return "";
    char *buf = new char[g_00c38970 + 1];
    buf[g_00c38970] = 0;
    strncpy(buf, m10.m00 ? (const char *)m10.m00->m08 : 0, g_00c38970);
    std::string result(buf);
    delete buf;
    return result;
}

// HYPOTHESIS: a running hash over bytes: two dwords set by its constructor (0x006e1660),
// fed by 0x006e1680; the result is the two dwords exclusive-ored.
class fn_006e1660_object
{
public:
    fn_006e1660_object(long param_1, long param_2);
    void fn_006e1680(const void *data, unsigned int size);

    long m00;
    long m04;
};

// HYPOTHESIS: the object 0x006ee3d0 returns by value: one dword, cleared when empty, with a
// destructor.
class fn_006ee3d0_result
{
public:
    fn_006ee3d0_result() : m00(0) {}
    ~fn_006ee3d0_result();

    long m00;
};

// HYPOTHESIS: an object 0x006ed560 (763 bytes, in the SDK's code) fills from a byte buffer
// and its length; 0x10b4 bytes, built by 0x006ed510, run over a buffer by 0x006ee3d0 and
// destroyed by 0x005d9e48. Its name and purpose are not known.
class fn_006ed560_object
{
public:
    fn_006ed560_object();
    ~fn_006ed560_object();
    void fn_006ed560(const unsigned char *data, unsigned int size);
    fn_006ee3d0_result fn_006ee3d0(long data, long size);

    char m_0x00[0x1048];
    // HYPOTHESIS: two byte strings, destroyed by 0x005d9e48 through 0x005da20a (_Tidy).
    std::basic_string< unsigned char > m1048;
    std::basic_string< unsigned char > m1058;
    char m_0x1068[0x4c];
};

// HYPOTHESIS: hashes the string, then each of its first three characters, keeping two bytes
// of every running hash in a byte string (the empty-string pointer 0x007b4be4 is a second
// string instantiation's, so the buffer is not a std::string).
// FUNCTION: STARFLEETOP 0x005d9cd3
void fn_005d9b26_object::fn_005d9cd3(fn_006ed560_object *param_1) const
{
    fn_006e1660_object hash(0, 0);
    std::basic_string< unsigned char > key;
    hash.fn_006e1680(m00.c_str(), m00.size());
    long value = hash.m04 ^ hash.m00;
    key.assign((const unsigned char *)&value, 2);
    for (int i = 0; i < 3 && i < m00.size(); i++)
    {
        char c = m00[i];
        hash.fn_006e1680(&c, 1);
        value = hash.m04 ^ hash.m00;
        key.append((const unsigned char *)&value, 2);
    }
    param_1->fn_006ed560(key.c_str(), key.size());
}

// FUNCTION: STARFLEETOP 0x005d9db1
fn_006ee3d0_result fn_005d9b26_object::fn_005d9db1() const
{
    if (!m14)
        return fn_006ee3d0_result();
    fn_006ed560_object object;
    fn_005d9cd3(&object);
    return object.fn_006ee3d0(m10.m00 ? m10.m00->m08 : 0, m10.m00 ? m10.m00->m0c : 0);
}

// FUNCTION: STARFLEETOP 0x005d9e48
fn_006ed560_object::~fn_006ed560_object()
{
}
