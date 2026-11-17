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
