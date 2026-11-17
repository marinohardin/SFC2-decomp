// HYPOTHESIS: the owning class of these 7-byte member-address getters is not known yet;
// the getters return the address of members at +0x340, +0x3b4, +0x4e8, +0x260, +0x270 and +0x1e4.
class fn_0057a481_object {
public:
    char *fn_0057a481();
    char *fn_0057a488();
    char *fn_0057a48f();
    char *fn_0057a496();
    char *fn_0057a49d();
    char *fn_0057a4a4();

    char field_0[0x1e4];
    char field_1e4;
    char field_1e5[0x260 - 0x1e5];
    char field_260;
    char field_261[0x270 - 0x261];
    char field_270;
    char field_271[0x340 - 0x271];
    char field_340;
    char field_341[0x3b4 - 0x341];
    char field_3b4;
    char field_3b5[0x4e8 - 0x3b5];
    char field_4e8;
};

// FUNCTION: STARFLEETOP 0x0057a481
char *fn_0057a481_object::fn_0057a481()
{
    return &field_340;
}

// FUNCTION: STARFLEETOP 0x0057a488
char *fn_0057a481_object::fn_0057a488()
{
    return &field_3b4;
}

// FUNCTION: STARFLEETOP 0x0057a48f
char *fn_0057a481_object::fn_0057a48f()
{
    return &field_4e8;
}

// FUNCTION: STARFLEETOP 0x0057a496
char *fn_0057a481_object::fn_0057a496()
{
    return &field_260;
}

// FUNCTION: STARFLEETOP 0x0057a49d
char *fn_0057a481_object::fn_0057a49d()
{
    return &field_270;
}

// FUNCTION: STARFLEETOP 0x0057a4a4
char *fn_0057a481_object::fn_0057a4a4()
{
    return &field_1e4;
}

// HYPOTHESIS: 0x0057a577 is the tail of a larger stream function that shares its parent frame; the parent is not rebuilt yet.
// FUNCTION: STARFLEETOP 0x0057a577
void *fn_0057a577()
{
    return 0;
}

// HYPOTHESIS: 0x0057a5be is a list's clear: member +4 is the sentinel node, +8 the count;
// the node callee and the deallocator are not identified yet.
struct fn_0057a5be_node {
    fn_0057a5be_node *next;
};

struct fn_0057a5be_alloc {
    int field_0;
};

void fn_00583342(fn_0057a5be_alloc *alloc, fn_0057a5be_node *node);

class fn_0057a5be_object {
public:
    void fn_0057a5be();

    int field_0;
    fn_0057a5be_node *field_4;
    int field_8;
};

// FUNCTION: STARFLEETOP 0x0057a5be
void fn_0057a5be_object::fn_0057a5be()
{
    fn_0057a5be_alloc alloc;
    fn_0057a5be_node *head = field_4;
    fn_0057a5be_node *node = head->next;
    while (node != head) {
        fn_0057a5be_node *cur = node;
        node = node->next;
        fn_00583342(&alloc, cur);
    }
    delete field_4;
    field_4 = 0;
    field_8 = 0;
}

// HYPOTHESIS: 0x0057a5f6 is a list's assignment operator in the standard library's shape
// (node: next, prev, value at +8); the element type, whose assignment is 0x005b63e9, is not
// identified yet.
class fn_0057a5f6_value {
public:
    fn_0057a5f6_value &operator=(const fn_0057a5f6_value &other);
};

struct fn_0057a5f6_node {
    fn_0057a5f6_node *next;
    fn_0057a5f6_node *prev;
    fn_0057a5f6_value value;
};

class fn_0057a5f6_iterator {
public:
    fn_0057a5f6_iterator(fn_0057a5f6_node *p) : ptr(p) {}
    fn_0057a5f6_value &operator*() const { return ptr->value; }
    fn_0057a5f6_iterator &operator++() { ptr = ptr->next; return *this; }
    fn_0057a5f6_iterator operator++(int) { fn_0057a5f6_iterator tmp = *this; ++*this; return tmp; }
    bool operator==(const fn_0057a5f6_iterator &other) const { return ptr == other.ptr; }
    bool operator!=(const fn_0057a5f6_iterator &other) const { return !(*this == other); }

    fn_0057a5f6_node *ptr;
};

class fn_0057a5f6_list {
public:
    fn_0057a5f6_list &operator=(const fn_0057a5f6_list &other);
    fn_0057a5f6_iterator begin() const { return fn_0057a5f6_iterator(field_4->next); }
    fn_0057a5f6_iterator end() const { return fn_0057a5f6_iterator(field_4); }
    fn_0057a5f6_iterator erase(fn_0057a5f6_iterator where);
    fn_0057a5f6_iterator insert(fn_0057a5f6_iterator where, const fn_0057a5f6_value &value);
    void insert(fn_0057a5f6_iterator where, fn_0057a5f6_iterator first, fn_0057a5f6_iterator last)
        { for (; first != last; ++first) insert(where, *first); }
    fn_0057a5f6_iterator erase(fn_0057a5f6_iterator first, fn_0057a5f6_iterator last)
        { while (first != last) erase(first++); return first; }

    int field_0;
    fn_0057a5f6_node *field_4;
    int field_8;
};

// FUNCTION: STARFLEETOP 0x0057a5f6
fn_0057a5f6_list &fn_0057a5f6_list::operator=(const fn_0057a5f6_list &other)
{
    if (this != &other) {
        fn_0057a5f6_iterator first1 = begin();
        fn_0057a5f6_iterator last1 = end();
        fn_0057a5f6_iterator first2 = other.begin();
        fn_0057a5f6_iterator last2 = other.end();
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            *first1 = *first2;
        erase(first1, last1);
        insert(last1, first2, last2);
    }
    return *this;
}
