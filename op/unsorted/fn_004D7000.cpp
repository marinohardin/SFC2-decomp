// HYPOTHESIS: the object is a counted array of 16-byte records at +4; the two callees
// are cdecl range helpers (sort, then compact) whose real names are unknown.
struct fn_004d703f_Array
{
    unsigned char m_count;
    unsigned char m_pad[3];
    char m_items[1][16];

    void fn_004d703f();
};

void fn_004d8999(void* first, void* last, int arg);
void* fn_004d8967(void* first, void* last);

// FUNCTION: STARFLEETOP 0x004d703f
void fn_004d703f_Array::fn_004d703f()
{
    fn_004d8999(m_items, m_items[m_count], 0);
    m_count = ((char*)fn_004d8967(m_items, m_items[m_count]) - (char*)m_items) >> 4;
}
